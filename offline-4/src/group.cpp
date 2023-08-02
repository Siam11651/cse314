#include "group.hpp"
#include "student.hpp"

std::vector<offline_4::group> offline_4::group::groups;

offline_4::group::group(const size_t &id)
{
    set_id(id);

    print_done_semaphore = new std::counting_semaphore<1>(0);
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

    for(std::map<size_t, student *>::iterator i = members.begin(); i != members.end(); ++i)
    {
        all_printed &= i->second->get_has_printed();
    }

    return all_printed;
}

std::counting_semaphore<1> *offline_4::group::get_print_done_semaphore() const
{
    return print_done_semaphore;
}

void offline_4::group::insert_student(student *student)
{
    members[student->get_id()] = student;
}

// void offline_4::group::notify()
// {
//     for(std::map<uint64_t, offline_4::student *>::iterator i = members.begin(); i != members.end(); ++i)
//     {
//         if(i->second->get_is_print_waiting())
//         {
//             i->second->release_print();

//             return;
//         }
//     }

//     for(size_t i = 0; i < groups.size(); ++i)
//     {
//         if(&groups[i] != this)
//         {
//             for(std::map<uint64_t, offline_4::student *>::iterator it = groups[i].members.begin(); it != groups[i].members.end(); ++it)
//             {
//                 if(it->second->get_is_print_waiting())
//                 {
//                     it->second->release_print();

//                     return;
//                 }
//             }
//         }
//     }
// }

std::vector<offline_4::group> &offline_4::group::get_groups()
{
    return groups;
}

offline_4::group::~group()
{
    delete print_done_semaphore;
}