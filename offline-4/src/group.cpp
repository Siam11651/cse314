#include "group.hpp"
#include "student.hpp"

offline_4::group::group(const size_t &id)
{
    set_id(id);
}

void offline_4::group::set_id(const size_t &id)
{
    this->id = id;
}

void offline_4::group::set_leader(offline_4::student *leader)
{
    this->leader = leader;
}

size_t offline_4::group::get_id() const
{
    return id;
}

size_t offline_4::group::get_size() const
{
    return members.size();
}

offline_4::student *offline_4::group::get_leader() const
{
    return leader;
}

bool offline_4::group::get_is_all_printed()
{
    bool all_printed = true;

    for(std::map<size_t, offline_4::student *>::iterator i = members.begin(); i != members.end(); ++i)
    {
        all_printed &= i->second->get_has_printed();
    }

    return all_printed;
}

void offline_4::group::insert_student(offline_4::student *student)
{
    members[student->get_id()] = student;
}