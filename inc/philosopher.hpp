#pragma once

#include <string>
#include <mutex>

#include "fork.hpp"

class Philosopher
{
private:
    std::string name_;
    mutable std::mutex coutMutex_;
    using Lock = std::lock_guard<std::mutex>;
    Fork& leftFork_;
    Fork& rightFork_;


public:
    Philosopher(const std::string& name, Fork& leftFork, Fork& rightFork);
    ~Philosopher() = default;

    void eat() const;
    void think() const;
    std::string getName() const;
};
