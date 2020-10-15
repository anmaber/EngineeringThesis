#pragma once

#include <string>
#include <mutex>

#include "fork.hpp"

class Philosopher
{
private:
    int id_;
    Fork& leftFork_;
    Fork& rightFork_;

    void eat() const;
    void think() const;
    void print(const std::string &what) const;

public:
    Philosopher(int id, Fork& leftFork, Fork& rightFork);
    Philosopher(Philosopher&& other);
    
    void dine(bool& isAlive);
};
