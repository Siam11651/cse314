#include "group.hpp"
#include "student.hpp"

std::vector<offline_4::group> *offline_4::group::groups = nullptr;

offline_4::group::group(const size_t &id)
{
    set_id(id);

    print_done_semaphore = new std::counting_semaphore<1>(0);
}

void offline_4::group::set_id(const size_t &id)
{
    this->id = id;
    leader = nullptr;
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

std::counting_semaphore<1> *offline_4::group::get_print_done_semaphore() const
{
    return print_done_semaphore;
}

void offline_4::group::insert_student(student *student)
{
    members.push_back(student);
}

void offline_4::group::notify(const uint64_t &from)
{
    size_t target_printer_id = from % 4;

    for(size_t i = 0; i < members.size(); ++i)
    {
        size_t member_target_printer_id = members[i]->get_id() % 4;

        if(member_target_printer_id == target_printer_id && members[i]->release_print_if_waiting())
        {
            return;
        }
    }

    for(size_t i = 0; i < groups->size(); ++i)
    {
        if(&(*groups)[i] != this)
        {
            for(size_t j = 0; j < (*groups)[i].members.size(); ++j)
            {
                size_t member_target_printer_id = (*groups)[i].members[j]->get_id() % 4;

                if(member_target_printer_id == target_printer_id && (*groups)[i].members[j]->release_print_if_waiting())
                {
                    return;
                }
            }
        }
    }
}

void offline_4::group::set_groups(std::vector<group> *groups)
{
    group::groups = groups;
}

std::vector<offline_4::group> *offline_4::group::get_groups()
{
    return groups;
}

offline_4::group::~group()
{
    delete print_done_semaphore;
}