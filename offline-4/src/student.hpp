#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include <semaphore>
#include <random>
#include "individual.hpp"
#include "group.hpp"
#include "printer_state.hpp"

namespace offline_4
{
    class student : public individual
    {
    private:
        offline_4::group *group;
        bool has_printed;
        std::mutex printed_lock;
        static uint64_t print_delay, bind_delay, submission_delay;
        static std::mutex rng_lock, printers_lock;
        static std::counting_semaphore<1> printer_semaphores[4], print_done_semaphore;
        static std::counting_semaphore<2> binder_semaphore;
        static std::mt19937_64 mt;
        static std::poisson_distribution<uint64_t> poisson_distribution;
        static offline_4::printer_state printer_states[4];

        static void test_printer(const size_t &index);
        
    public:
        student(const size_t &id = 0);
        void set_group(offline_4::group *group);
        void set_has_printed(const bool &value);
        offline_4::group *get_group() const;
        bool get_has_printed();
        void action();
        bool operator < (const student &other) const;
        static void set_print_delay(const uint64_t &delay);
        static void set_bind_delay(const uint64_t &delay);
        static void set_submission_delay(const uint64_t &delay);
    };
}