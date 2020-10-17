#pragma once

#include <string>
#include <chrono>

struct Conclusion{
    std::string philosopher;
    std::string answer;
    int result;
    int period;
    bool chosen = false;
};