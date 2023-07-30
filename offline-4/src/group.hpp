#pragma once

#include <vector>
#include <map>

namespace offline_4
{
    class student;

    class group
    {
    private:
        size_t id;
        offline_4::student *leader;
        std::map<size_t, offline_4::student *> members;

    public:
        group(const size_t &id = 0);
        void set_id(const size_t &id);
        void set_leader(offline_4::student *leader);
        size_t get_id() const;
        size_t get_size() const;
        offline_4::student *get_leader() const;
        bool get_is_all_printed();
        void insert_student(offline_4::student *student);
    };
}