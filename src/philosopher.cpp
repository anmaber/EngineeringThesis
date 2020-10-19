#include <iostream>
#include "philosopher.hpp"
#include <thread>
#include <functional>
#include <algorithm>

std::mutex coutMutex;

void Philosopher::eat()
{
    if(!isFull())
    {
        std::scoped_lock lockForks(leftFork_.forkMutex_,rightFork_.forkMutex_);
        print("started eating");
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 2000));
        print("stopped eating");
        lastMeal_ = std::chrono::steady_clock::now();
        isHungry_ = false;
    }
}

void Philosopher::think(std::atomic<int>& everyoneIsReady)
{
    if(!isHungry_)
    {
        
        if(processedIdeasCounter < 10)
        {
            print("started thinking");
            auto start = std::chrono::steady_clock::now();
            auto result = processIdea(processedIdeasCounter);

            std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 2000));
            auto end = std::chrono::steady_clock::now();
            auto period = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

            writeConclusionToBook(id_, ideas_.get().at(processedIdeasCounter), result, period);
            processedIdeasCounter++;
            
            print("stopped thinking");
            isHungry_ = true;
        }
        if(processedIdeasCounter == 10)
        {
            chooseSolution();
            readyToGiveSolution = true;
            everyoneIsReady--;
        }
    }

}

Philosopher::Philosopher(int id, Fork& leftFork, Fork& rightFork, Book& book) :
    id_(id),
    leftFork_(leftFork),
    rightFork_(rightFork),
    book_(book)
{
    lastMeal_ = std::chrono::steady_clock::now();
}

void Philosopher::dine(bool& feastHasPlace, std::atomic<int>& everyoneIsReady)
{
    while(!feastHasPlace);
    lastMeal_ = std::chrono::steady_clock::now();
    while(feastHasPlace || everyoneIsReady.load() != 0)
    {
        if(isAlive() && !readyToGiveSolution)
        {
            think(everyoneIsReady);
            eat();
        }
        if(!isAlive())
        {
            print("DIED");
            return;
        }        
    }
    if(readyToGiveSolution)
    {
        print(giveSolution());
    }
    else
    {
        print("fest was too short for me to give you solution");
    }
    
}

void Philosopher::print(const std::string& what) const
{
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout<< id_<<"\t"<<what<<"\n";
}

Philosopher::Philosopher(Philosopher&& other) :
    id_(other.id_),
    leftFork_(other.leftFork_),
    rightFork_(other.rightFork_),
    book_(other.book_)
{}

bool Philosopher::isFull() const
{
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastEating = std::chrono::duration_cast<std::chrono::seconds>(now - lastMeal_).count();
    return timeSinceLastEating < timeToRestAfterEating;
}

bool Philosopher::isAlive() const
{
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastEating = std::chrono::duration_cast<std::chrono::seconds>(now - lastMeal_).count();
    return timeSinceLastEating < lifetimeWithoutEating;
}

int Philosopher::processIdea(int indexOfIdea)
{
    std::string idea = ideas_.get().at(indexOfIdea);
    auto inspiration = getInspirationFromBook();
    std::hash<std::string> h;
    return h(idea + inspiration)%666;
}

std::string Philosopher::getInspirationFromBook()
{
    std::string inspiration;
    std::shared_lock<std::shared_mutex> lock(book_.bookMutex_);
    if(book_.conclusions_.size() > 0)
    {
        auto index = std::rand() % book_.conclusions_.size();
        inspiration = book_.conclusions_.at(index).answer;
    }
    lock.unlock();

    return inspiration;
}

void Philosopher::writeConclusionToBook(int id,const std::string& answer, int result, int period)
{
    std::lock_guard<std::shared_mutex> lock(book_.bookMutex_);
    book_.conclusions_.push_back({id, answer, result, period});
}

void Philosopher::chooseSolution()
{
    std::shared_lock<std::shared_mutex> lock(book_.bookMutex_);
    std::vector<Conclusion> conclusions;
    std::copy_if(book_.conclusions_.begin(),book_.conclusions_.end(),
                std::back_inserter(conclusions), [id=id_](const auto& conclusion){return conclusion.philosopher == id;});
    lock.unlock();
    std::vector<int> results;
    std::transform(conclusions.begin(),conclusions.end(),std::back_inserter(results),
                    [](const auto& conclusion)->int{return conclusion.result*conclusion.period;});
    auto bestResult = std::max_element(results.begin(), results.end());
    int index = std::distance(results.begin(), bestResult);
    Conclusion bestConclusion = conclusions.at(index);

    std::lock_guard<std::shared_mutex> exclusiveLock(book_.bookMutex_);
    auto iter = std::find_if(book_.conclusions_.begin(),book_.conclusions_.end(),
                                [best=bestConclusion](auto& conclusion)mutable{return best == conclusion;});
    iter->chosen = true;
}

std::string Philosopher::giveSolution()
{
    std::shared_lock<std::shared_mutex> lock(book_.bookMutex_);
    auto iter = std::find_if(book_.conclusions_.begin(),book_.conclusions_.end(),
                                [id = id_](auto& conclusion)mutable{return conclusion.philosopher == id && conclusion.chosen == true;});
    return "My solution is: " + iter->answer;
}
