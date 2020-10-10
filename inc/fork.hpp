#pragma once

#include <mutex>

struct Fork
{
    mutable std::mutex forkMutex_;
};
