#include <fstream>
#include <syncstream>
#include "staff.hpp"
#include "group.hpp"
#include "timer.hpp"
#include "random.hpp"
#include "stream.hpp"

size_t offline_4::staff::submission_count = 0;
size_t offline_4::staff::reader_count = 0;
std::mutex offline_4::staff::reader_count_lock;
std::mutex offline_4::staff::read_write_lock;

offline_4::staff::staff(const size_t &id) : individual(id)
{
    
}

void offline_4::staff::action()
{
    std::ofstream &output_stream = offline_4::stream::get_output_stream();
    std::vector<group> *groups = group::get_groups();

    while(true)
    {
        reader_count_lock.lock();

        ++reader_count;

        if(reader_count == 1)
        {
            reader_count_lock.unlock();
            read_write_lock.lock();
        }
        else
        {
            reader_count_lock.unlock();
        }

        std::this_thread::sleep_for(timer::get_submission_delay());

        uint64_t temp_submission_count = submission_count;

        std::osyncstream(output_stream) << "Staff " << get_id() << " has started reading the entry book at time " << timer::get_microseconds_count() / 1000000 << ". No. of submission = " << submission_count << std::endl;

        reader_count_lock.lock();

        --reader_count;

        if(reader_count == 0)
        {
            reader_count_lock.unlock();
            read_write_lock.unlock();
        }
        else
        {
            reader_count_lock.unlock();
        }

        if(temp_submission_count == groups->size())
        {
            break;
        }

        uint64_t random_delay = random::get_next_uint64_t() % 5 + 1;
        std::chrono::seconds delay_seconds(random_delay);

        std::this_thread::sleep_for(delay_seconds);
    }
}

void offline_4::staff::add_submission()
{
    read_write_lock.lock();

    std::this_thread::sleep_for(timer::get_submission_delay());

    ++submission_count;

    read_write_lock.unlock();
}