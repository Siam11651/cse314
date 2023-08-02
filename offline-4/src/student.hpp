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
        // bool print_waiting;
        std::mutex has_printed_lock;
        // std::mutex print_waiting_lock;
        // std::binary_semaphore *printer_wait_semaphore;
        static std::mutex printers_lock;
        // static std::mutex printer_busy_locks[4];
        static std::counting_semaphore<1> printer_semaphores[4];
        static std::counting_semaphore<2> binder_semaphore;
        static offline_4::printer_state printer_states[4];
        // static bool printer_busy[4];

        static void test_printer(const size_t &index);
        
    public:
        student(const uint64_t &id = 0);
        void set_group(offline_4::group *group);
        void set_has_printed(const bool &value);
        offline_4::group *get_group() const;
        bool get_has_printed();
        // bool get_is_print_waiting();
        void action();
        void release_print();
        bool operator < (const student &other) const;
        ~student();
    };
}