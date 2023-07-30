#pragma once

#include <thread>
#include <functional>
#include <mutex>

namespace offline_4
{
    class stuff
    {
    private:
        size_t id;
        std::thread thread;
        std::function<void()> action;
        static size_t submission_count, reader_count;
        static std::mutex reader_count_lock, read_write_lock;

    public:
        stuff(const size_t &id = 0);
        void set_id(const size_t &id);
        size_t get_id() const;
        void start_action();
        void join_thread();
        static void increment_reader_count();
        static void decrement_reader_count();
        static void add_submission();
    };
}