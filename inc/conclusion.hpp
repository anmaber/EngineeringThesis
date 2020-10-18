#pragma once

#include <string>
#include <chrono>

struct Conclusion{
    int philosopher;
    std::string answer;
    int result;
    int period;
    bool chosen = false;
};