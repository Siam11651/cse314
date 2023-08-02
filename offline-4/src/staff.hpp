#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include "individual.hpp"

namespace offline_4
{
    class staff : public individual
    {
    private:
        uint64_t total_wxy;
        static size_t submission_count, reader_count;
        static std::mutex reader_count_lock, read_write_lock;

    public:
        staff(const size_t &id = 0);
        void action();
        static void add_submission();
    };
}