#include <iostream>
#include <syncstream>
#include "stuff.hpp"

size_t offline_4::stuff::submission_count = 0;
size_t offline_4::stuff::reader_count = 0;
std::mutex offline_4::stuff::reader_count_lock;
std::mutex offline_4::stuff::read_write_lock;

offline_4::stuff::stuff(const size_t &id)
{
    set_id(id);

    action = [this]()
    {
        while(true)
        {
            offline_4::stuff::increment_reader_count();

            std::osyncstream(std::cout) << "Staff " << get_id() << " has started reading the entry book at time 13. No. of submission = " << offline_4::stuff::submission_count << std::endl;

            offline_4::stuff::decrement_reader_count();
        }
    };
}

void offline_4::stuff::set_id(const size_t &id)
{
    this->id = id;
}

size_t offline_4::stuff::get_id() const
{
    return id;
}

void offline_4::stuff::start_action()
{
    thread = std::thread(action);
}

void offline_4::stuff::join_thread()
{
    thread.join();
}

void offline_4::stuff::increment_reader_count()
{
    reader_count_lock.lock();

    size_t temp_reader_count = reader_count;

    reader_count_lock.unlock();

    if(temp_reader_count == 0)
    {
        read_write_lock.lock();
    }

    reader_count_lock.lock();

    ++reader_count;

    reader_count_lock.unlock();
}

void offline_4::stuff::decrement_reader_count()
{
    reader_count_lock.lock();

    size_t temp_reader_count = --reader_count;

    reader_count_lock.unlock();

    if(temp_reader_count == 0)
    {
        read_write_lock.unlock();
    }
}

void offline_4::stuff::add_submission()
{
    read_write_lock.lock();

    ++submission_count;

    read_write_lock.unlock();
}