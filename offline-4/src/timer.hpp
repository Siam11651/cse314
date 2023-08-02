#pragma once

#include <chrono>

namespace offline_4
{
    class timer
    {
    private:
        static std::chrono::high_resolution_clock::time_point start_point;
        static std::chrono::seconds print_delay, bind_delay, submission_delay;

    public:
        static uint64_t get_microseconds_count();
        static void set_print_delay(const uint64_t &delay);
        static void set_bind_delay(const uint64_t &delay);
        static void set_submission_delay(const uint64_t &delay);
        static std::chrono::seconds get_print_delay();
        static std::chrono::seconds get_bind_delay();
        static std::chrono::seconds get_submission_delay();
    };
}