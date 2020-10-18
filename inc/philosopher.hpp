#pragma once

#include <string>
#include <mutex>
#include <chrono>
#include <vector>
#include <string>

#include "fork.hpp"
#include "ideas.hpp"
#include "book.hpp"

class Philosopher
{
private:
    int id_;
    Fork& leftFork_;
    Fork& rightFork_;

    bool isAlive() const;
    bool isFull() const;
    bool isHungry_ = true;

    Ideas ideas_;
    int processedIdeasCounter;
    int processIdea(int indexOfIdea);

    Book & book_;
    std::string getInspirationFromBook();
    void writeConclusionToBook(int id,const std::string& answer, int result, int period);

    std::chrono::steady_clock::time_point lastMeal_;
    static constexpr int timeToRestAfterEating = 3;
    static constexpr int lifetimeWithoutEating = 20;

    void eat();
    void think();
    void print(const std::string &what) const;

public:
    Philosopher(int id, Fork& leftFork, Fork& rightFork, Book& book);
    Philosopher(Philosopher&& other);
    
    void dine(bool& feastHasPlace);
};
