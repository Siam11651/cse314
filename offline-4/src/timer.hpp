#pragma once

#include <chrono>

namespace offline_4
{
    class timer
    {
    private:
        static std::chrono::high_resolution_clock::time_point start_point;

    public:
        static uint64_t get_microseconds_count();
    };
}