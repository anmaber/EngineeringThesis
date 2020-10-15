#include <iostream>
#include "philosopher.hpp"
#include <thread>

std::mutex coutMutex;

void Philosopher::eat() const
{
    std::scoped_lock lockForks(leftFork_.forkMutex_,rightFork_.forkMutex_);
    print("started eating");
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 50+50));
    print("stopped eating");
}

void Philosopher::think() const
{
    print("started thinking");
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 100+50));
    print("stopped thinking");
}

Philosopher::Philosopher(int id, Fork& leftFork, Fork& rightFork) :
    id_(id),
    leftFork_(leftFork),
    rightFork_(rightFork)
{}

void Philosopher::dine(bool& isAlive)
{
    while(!isAlive);
    while(isAlive)
    {
        think();
        eat();
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