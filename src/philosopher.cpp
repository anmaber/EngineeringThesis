#include <iostream>
#include "philosopher.hpp"
#include <thread>

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

void Philosopher::think()
{
    if(!isHungry_)
    {
        print("started thinking");
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 2000));
        print("stopped thinking");
        isHungry_ = true;
    }

}

Philosopher::Philosopher(int id, Fork& leftFork, Fork& rightFork) :
    id_(id),
    leftFork_(leftFork),
    rightFork_(rightFork)
{
    lastMeal_ = std::chrono::steady_clock::now();
}

void Philosopher::dine(bool& feastHasPlace)
{
    while(!feastHasPlace);
    lastMeal_ = std::chrono::steady_clock::now();
    while(feastHasPlace)
    {
        if(isAlive())
        {
            think();
            eat();
        }
        else
        {
            print("DIED");
            return;
        }        
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
    rightFork_(other.rightFork_)
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
