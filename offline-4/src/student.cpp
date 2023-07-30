#include <iostream>
#include <syncstream>
#include "student.hpp"
#include "stuff.hpp"

uint64_t offline_4::student::print_delay;
uint64_t offline_4::student::bind_delay;
uint64_t offline_4::student::submission_delay;
std::chrono::high_resolution_clock::time_point offline_4::student::start_point = std::chrono::high_resolution_clock::now();
std::mutex offline_4::student::rng_lock;
std::mutex offline_4::student::printers_lock;
std::counting_semaphore<1> offline_4::student::printer_semaphores[4] =
{
    std::counting_semaphore<1>(0),
    std::counting_semaphore<1>(0),
    std::counting_semaphore<1>(0),
    std::counting_semaphore<1>(0)
};
std::counting_semaphore<1> offline_4::student::print_done_semaphore(0);
std::counting_semaphore<2> offline_4::student::binder_semaphore(2);
std::mt19937_64 offline_4::student::mt(39);
std::poisson_distribution<uint64_t> offline_4::student::poisson_distribution(2.5);
offline_4::printer_state offline_4::student::printer_states[4];

void offline_4::student::sleep(const size_t &seconds)
{
    std::chrono::seconds delay_seconds(seconds);

    std::this_thread::sleep_for(delay_seconds);
}

uint64_t offline_4::student::get_microseconds_count() const
{
    std::chrono::high_resolution_clock::time_point current_point = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(current_point - offline_4::student::start_point);

    return duration.count();
}

void offline_4::student::test_printer(const size_t &index)
{
    size_t index_left = (index + 3) % 4;
    size_t index_right = (index + 1) % 4;

    if(printer_states[index] == offline_4::printer_state::PRE_PRINT
        && printer_states[index_left] != offline_4::printer_state::PRINT
        && printer_states[index_right] != offline_4::printer_state::PRINT)
    {
        printer_states[index] = offline_4::printer_state::PRINT;
        offline_4::student::printer_semaphores[index].release();
    }
}

offline_4::student::student(const size_t &id)
{
    set_id(id);

    action = [this]()
    {
        student::rng_lock.lock();

        size_t random_delay = offline_4::student::poisson_distribution(offline_4::student::mt) % 4 + 1;

        student::rng_lock.unlock();
        sleep(random_delay);

        std::osyncstream(std::cout) << "Student " << get_id() << " has arrived at the print station at time " << get_microseconds_count() << std::endl;

        size_t target_printer_id = get_id() % 4;
        size_t left_printer_id = (target_printer_id + 3) % 4;
        size_t right_printer_id = (target_printer_id + 1) % 4;
        student::printers_lock.lock();

        printer_states[target_printer_id] = offline_4::printer_state::PRE_PRINT;

        student::test_printer(target_printer_id);
        student::printers_lock.unlock();
        student::printer_semaphores[target_printer_id].acquire();

        sleep(student::print_delay);
        student::printers_lock.lock();

        printer_states[target_printer_id] = offline_4::printer_state::POST_PRINT;

        student::test_printer(left_printer_id);
        student::test_printer(right_printer_id);
        student::printers_lock.unlock();

        std::osyncstream(std::cout) << "Student " << get_id() << " has finished printing at time " << get_microseconds_count() << std::endl;

        set_has_printed(true);

        student::print_done_semaphore.release();

        if(get_group()->get_leader()->get_id() != get_id())
        {
            // only leader can pass through print phase

            return;
        }

        size_t group_size = get_group()->get_size();

        for(size_t i = 0; i < group_size; ++i)
        {
            offline_4::student::print_done_semaphore.acquire();
        }

        std::osyncstream(std::cout) << "Group " << get_group()->get_id() << " has finished printing at time " << get_microseconds_count() << std::endl;

        student::binder_semaphore.acquire();

        std::osyncstream(std::cout) << "Group " << get_group()->get_id() << " has started binding at time " << get_microseconds_count() << std::endl;

        sleep(student::bind_delay);
        student::binder_semaphore.release();

        std::osyncstream(std::cout) << "Group " << get_group()->get_id() << " has finished binding at time " << get_microseconds_count() << std::endl;

        offline_4::stuff::add_submission();

        std::osyncstream(std::cout) << "Group " << get_group()->get_id() << " has submitted the report at time " << get_microseconds_count() << std::endl;
    };
}

void offline_4::student::set_has_printed(const bool &value)
{
    printed_lock.lock();

    has_printed = value;

    printed_lock.unlock();
}

void offline_4::student::set_id(const size_t &id)
{
    this->id = id;
}

void offline_4::student::set_group(offline_4::group *group)
{
    this->group = group;
}

bool offline_4::student::get_has_printed()
{
    printed_lock.lock();

    bool to_return = has_printed;

    printed_lock.unlock();

    return has_printed;
}

size_t offline_4::student::get_id() const
{
    return id;
}

offline_4::group *offline_4::student::get_group() const
{
    return group;
}

void offline_4::student::set_print_delay(const uint64_t &delay)
{
    print_delay = delay;
}

void offline_4::student::set_bind_delay(const uint64_t &delay)
{
    bind_delay = delay;
}

void offline_4::student::set_submission_delay(const uint64_t &delay)
{
    submission_delay = delay;
}

void offline_4::student::start_action()
{
    thread = std::thread(action);
}

void offline_4::student::join_thread()
{
    thread.join();
}

bool offline_4::student::operator < (const student &other) const
{
    return id < other.id;
}