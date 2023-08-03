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
        bool waiting;
        std::mutex has_printed_lock;
        std::mutex waiting_lock;
        std::binary_semaphore *wait_semaphore;
        static bool printer_busy[4];
        static std::mutex printer_locks[4];
        static std::counting_semaphore<2> binder_semaphore;

        static void test_printer(const size_t &index);
        
    public:
        student(const uint64_t &id = 0);
        void set_group(offline_4::group *group);
        void set_has_printed(const bool &value);
        offline_4::group *get_group() const;
        void action();
        bool release_print_if_waiting();
        bool operator < (const student &other) const;
        ~student();
    };
}