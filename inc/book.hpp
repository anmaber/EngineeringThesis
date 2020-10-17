#pragma once

#include <shared_mutex>
#include <vector>

#include "conclusion.hpp"

struct Book
{
    std::shared_mutex bookMutex_;
    std::vector<Conclusion> conclusions_;
};