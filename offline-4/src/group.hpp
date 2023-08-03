#pragma once

#include <vector>
#include <map>
#include <cstdint>
#include <semaphore>

namespace offline_4
{
    class student;

    class group
    {
    private:
        size_t id;
        offline_4::student *leader;
        std::vector<student *> members;
        std::counting_semaphore<1> *print_done_semaphore;
        static std::vector<group> groups;

    public:
        group(const size_t &id = 0);
        void set_id(const size_t &id);
        void set_leader(student *leader);
        size_t get_id() const;
        size_t get_size() const;
        student *get_leader() const;
        std::counting_semaphore<1> *get_print_done_semaphore() const;
        void insert_student(student *student);
        void notify();
        static std::vector<group> &get_groups();
        ~group();
    };
}