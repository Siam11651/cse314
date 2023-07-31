#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include "individual.hpp"

namespace offline_4
{
    class stuff : public individual
    {
    private:
        static size_t submission_count, reader_count;
        static std::mutex reader_count_lock, read_write_lock;

    public:
        stuff(const size_t &id = 0);
        void action();
        static void increment_reader_count();
        static void decrement_reader_count();
        static void add_submission();
    };
}