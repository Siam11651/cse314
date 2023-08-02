#include <iostream>
#include <syncstream>
#include "staff.hpp"
#include "timer.hpp"
#include "random.hpp"

size_t offline_4::staff::submission_count = 0;
size_t offline_4::staff::reader_count = 0;
std::mutex offline_4::staff::reader_count_lock;
std::mutex offline_4::staff::read_write_lock;

offline_4::staff::staff(const size_t &id) : individual(id)
{
    total_wxy = timer::get_print_delay().count() + timer::get_bind_delay().count() + timer::get_submission_delay().count();
}

void offline_4::staff::action()
{
    while(true)
    {
        // offline_4::staff::increment_reader_count();

        reader_count_lock.lock();

        ++reader_count;

        if(reader_count == 1)
        {
            read_write_lock.lock();
        }

        reader_count_lock.unlock();

        std::this_thread::sleep_for(timer::get_submission_delay());

        std::osyncstream(std::cout) << "Staff " << get_id() << " has started reading the entry book at time " << timer::get_microseconds_count() << ". No. of submission = " << staff::submission_count << std::endl;

        reader_count_lock.lock();

        --reader_count;

        if(reader_count == 0)
        {
            read_write_lock.unlock();
        }

        reader_count_lock.unlock();

        uint64_t random_delay = random::get_poisson_distribution() % total_wxy + total_wxy;
        std::chrono::seconds delay_seconds(random_delay);

        std::this_thread::sleep_for(delay_seconds);
    }
}

void offline_4::staff::add_submission()
{
    read_write_lock.lock();

    ++submission_count;
    std::this_thread::sleep_for(timer::get_submission_delay());

    read_write_lock.unlock();
}