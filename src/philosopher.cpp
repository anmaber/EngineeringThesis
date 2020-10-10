#include <iostream>
#include "philosopher.hpp"
#include <thread>

void Philosopher::eat() const
{
    std::scoped_lock lockForks(leftFork_.forkMutex_,rightFork_.forkMutex_);
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 3000));
    Lock l(coutMutex_);
    std::cout << getName() << "is eating";
}

void Philosopher::think() const
{
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 3000));
    Lock l(coutMutex_);
    std::cout << getName() << "is thinking";
}

std::string Philosopher::getName() const
{
    return name_;
}

Philosopher::Philosopher(const std::string& name, Fork& leftFork, Fork& rightFork) :
    name_(name),
    leftFork_(leftFork),
    rightFork_(rightFork)
{}