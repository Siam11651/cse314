#include <iostream>
#include <syncstream>
#include "student.hpp"
#include "staff.hpp"
#include "timer.hpp"
#include "random.hpp"

std::mutex offline_4::student::printers_lock;
// std::mutex offline_4::student::printer_busy_locks[4];
std::counting_semaphore<1> offline_4::student::printer_semaphores[4] =
{
    std::counting_semaphore<1>(0),
    std::counting_semaphore<1>(0),
    std::counting_semaphore<1>(0),
    std::counting_semaphore<1>(0)
};
std::counting_semaphore<2> offline_4::student::binder_semaphore(2);
offline_4::printer_state offline_4::student::printer_states[4];
// bool offline_4::student::printer_busy[4];

void offline_4::student::test_printer(const size_t &index)
{
    size_t index_left = (index + 3) % 4;
    size_t index_right = (index + 1) % 4;

    if(printer_states[index] == printer_state::PRE_PRINT
        && printer_states[index_left] != printer_state::PRINT
        && printer_states[index_right] != printer_state::PRINT)
    {
        printer_states[index] = offline_4::printer_state::PRINT;
        student::printer_semaphores[index].release();
    }
}

offline_4::student::student(const uint64_t &id) : individual(id)
{
    // printer_wait_semaphore = new std::binary_semaphore(0);
}

void offline_4::student::set_has_printed(const bool &value)
{
    has_printed_lock.lock();

    has_printed = value;

    has_printed_lock.unlock();
}

void offline_4::student::set_group(offline_4::group *group)
{
    this->group = group;
}

bool offline_4::student::get_has_printed()
{
    has_printed_lock.lock();

    bool to_return = has_printed;

    has_printed_lock.unlock();

    return has_printed;
}

offline_4::group *offline_4::student::get_group() const
{
    return group;
}

// bool offline_4::student::get_is_print_waiting()
// {
//     print_waiting_lock.lock();

//     bool to_return = print_waiting;

//     print_waiting_lock.unlock();

//     return to_return;
// }

void offline_4::student::action()
{
    uint64_t random_delay = random::get_poisson_distribution() % 4 + 1;

    std::chrono::seconds delay_seconds(random_delay);

    std::this_thread::sleep_for(delay_seconds);

    std::osyncstream(std::cout) << "Student " << get_id() << " has arrived at the print station at time " << timer::get_microseconds_count() << std::endl;

    size_t target_printer_id = get_id() % 4;

    // student::printer_busy_locks[target_printer_id].lock();

    // if(student::printer_busy[target_printer_id])
    // {
    //     student::printer_busy_locks[target_printer_id].unlock();
    //     print_waiting_lock.lock();

    //     print_waiting = true;

    //     print_waiting_lock.unlock();
    //     printer_wait_semaphore->acquire();
    // }
    // else
    // {
    //     student::printer_busy_locks[target_printer_id].unlock();
    // }

    size_t left_printer_id = (target_printer_id + 3) % 4;
    size_t right_printer_id = (target_printer_id + 1) % 4;
    student::printers_lock.lock();

    printer_states[target_printer_id] = printer_state::PRE_PRINT;

    student::test_printer(target_printer_id);
    student::printers_lock.unlock();
    student::printer_semaphores[target_printer_id].acquire();
    // student::printer_busy_locks[target_printer_id].lock();

    // printer_busy[target_printer_id] = true;

    // student::printer_busy_locks[target_printer_id].unlock();

    std::this_thread::sleep_for(timer::get_print_delay());
    // student::printer_busy_locks[target_printer_id].lock();

    // printer_busy[target_printer_id] = false;

    // student::printer_busy_locks[target_printer_id].unlock();
    student::printers_lock.lock();

    printer_states[target_printer_id] = printer_state::POST_PRINT;

    student::test_printer(left_printer_id);
    student::test_printer(right_printer_id);
    student::printers_lock.unlock();

    std::osyncstream(std::cout) << "Student " << get_id() << " has finished printing at time " << timer::get_microseconds_count() << std::endl;

    set_has_printed(true);

    get_group()->get_print_done_semaphore()->release();

    if(get_group()->get_leader()->get_id() != get_id())
    {
        // only leader can pass through print phase

        return;
    }

    size_t group_size = get_group()->get_size();

    for(size_t i = 0; i < group_size; ++i)
    {
        get_group()->get_print_done_semaphore()->acquire();
    }

    std::osyncstream(std::cout) << "Group " << get_group()->get_id() << " has finished printing at time " << timer::get_microseconds_count() << std::endl;

    student::binder_semaphore.acquire();

    std::osyncstream(std::cout) << "Group " << get_group()->get_id() << " has started binding at time " << timer::get_microseconds_count() << std::endl;

    std::this_thread::sleep_for(timer::get_bind_delay());
    student::binder_semaphore.release();

    std::osyncstream(std::cout) << "Group " << get_group()->get_id() << " has finished binding at time " << timer::get_microseconds_count() << std::endl;

    staff::add_submission();

    std::osyncstream(std::cout) << "Group " << get_group()->get_id() << " has submitted the report at time " << timer::get_microseconds_count() << std::endl;
}

// void offline_4::student::release_print()
// {
//     print_waiting_lock.lock();

//     print_waiting = false;

//     print_waiting_lock.unlock();
//     printer_wait_semaphore->release();
// }

bool offline_4::student::operator < (const student &other) const
{
    return id < other.id;
}

offline_4::student::~student()
{
    // delete printer_wait_semaphore;
}