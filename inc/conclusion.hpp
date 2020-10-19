#pragma once

#include <string>
#include <chrono>

struct Conclusion{
    int philosopher;
    std::string answer;
    int result;
    int period;
    bool chosen = false;

    bool operator==(const Conclusion& other)
    {
        return philosopher == other.philosopher
               && answer == other.answer
               && result == other.result
               && period == other.period
               && chosen == other.chosen;
    }
};
