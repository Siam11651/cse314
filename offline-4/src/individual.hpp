#pragma once

#include <thread>
#include <functional>

namespace offline_4
{
    class individual
    {
    protected:
        uint64_t id;
        std::thread thread;

    public:
        individual(const uint64_t &id = 0);
        void set_id(const uint64_t &id);
        uint64_t get_id() const;
        void start_action();
        void join_thread();
        virtual void action();
    };
}