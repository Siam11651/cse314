#pragma once

#include <mutex>
#include <random>

namespace offline_4
{
    class random
    {
    private:
        static std::mutex rng_lock;
        static std::mt19937_64 mt;
        static std::poisson_distribution<uint64_t> poisson_distribution;

    public:
        static uint64_t get_next_uint64_t();
    };
}