#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

#include "philosopher.hpp"
#include "book.hpp"

namespace
{
    constexpr int numOfPhilosophers = 5;
}

int main()
{
    std::vector<Fork> forks(numOfPhilosophers);
    std::vector<Philosopher> philosophers;
    std::vector<std::thread> philosophersThreads;

    bool feastHasPlace = false;
    std::atomic<int> everyoneIsReady = numOfPhilosophers;
    Book book;

    for(int i = 0; i < numOfPhilosophers-1; ++i)
    {
        philosophers.emplace_back(i, forks.at(i), forks.at(i+1), book);
    }
    philosophers.emplace_back(numOfPhilosophers - 1, forks.at(numOfPhilosophers - 1), forks.at(0), book);

    for(int i = 0; i < numOfPhilosophers; ++i)
    {
        philosophersThreads.emplace_back(std::thread(&Philosopher::dine, &philosophers.at(i), std::ref(feastHasPlace),std::ref(everyoneIsReady)));
    }

    feastHasPlace = true;

    std::this_thread::sleep_for(std::chrono::seconds(60));

    feastHasPlace = false;

    for(auto &&philosopherThread : philosophersThreads)
    {
        if(philosopherThread.joinable())
        {
            philosopherThread.join();
        }
    }

    std::cout<< "hello future engineer!\n";
    return 0;
}