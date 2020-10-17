#pragma once

#include <string>
#include <mutex>
#include <chrono>
#include <vector>
#include <string>

#include "fork.hpp"
#include "ideas.hpp"

class Philosopher
{
private:
    int id_;
    Fork& leftFork_;
    Fork& rightFork_;

    bool isAlive() const;
    bool isFull() const;
    bool isHungry_ = true;

    Ideas ideas;

    std::chrono::steady_clock::time_point lastMeal_;
    static constexpr int timeToRestAfterEating = 3;
    static constexpr int lifetimeWithoutEating = 20;

    void eat();
    void think();
    void print(const std::string &what) const;

public:
    Philosopher(int id, Fork& leftFork, Fork& rightFork);
    Philosopher(Philosopher&& other);
    
    void dine(bool& feastHasPlace);
};
