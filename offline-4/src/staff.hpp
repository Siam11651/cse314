#pragma once

#include <thread>
#include <functional>
#include <shared_mutex>
#include "individual.hpp"

namespace offline_4
{
    class staff : public individual
    {
    private:
        static size_t submission_count, reader_count;
        static std::shared_mutex read_write_lock;

    public:
        staff(const size_t &id = 0);
        void action();
        static void add_submission();
    };
}