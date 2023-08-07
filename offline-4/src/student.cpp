#include <fstream>
#include <syncstream>
#include "student.hpp"
#include "staff.hpp"
#include "timer.hpp"
#include "random.hpp"
#include "stream.hpp"

std::vector<offline_4::student> *offline_4::student::students;
bool offline_4::student::printer_busy[4];
std::mutex offline_4::student::printer_locks[4];
std::counting_semaphore<2> offline_4::student::binder_semaphore(2);

offline_4::student::student(const uint64_t &id) : individual(id)
{
    wait_semaphore = new std::binary_semaphore(0);
}

void offline_4::student::set_group(offline_4::group *group)
{
    this->group = group;
}

offline_4::group *offline_4::student::get_group() const
{
    return group;
}

void offline_4::student::action()
{
    std::ofstream &output_stream = offline_4::stream::get_output_stream();   
    uint64_t random_delay = random::get_poisson_distribution() % 4 + 1;

    std::chrono::seconds delay_seconds(random_delay);

    std::this_thread::sleep_for(delay_seconds);

    size_t target_printer_id = get_id() % 4;

    printer_locks[target_printer_id].lock();

    if(printer_busy[target_printer_id])
    {
        printer_locks[target_printer_id].unlock();
        waiting_lock.lock();

        waiting = true;

        waiting_lock.unlock();
        wait_semaphore->acquire();
    }
    else
    {
        printer_busy[target_printer_id] = true;
        printer_locks[target_printer_id].unlock();
    }

    std::osyncstream(output_stream) << "Student " << get_id() << " has arrived at the print station at time " << timer::get_microseconds_count() << std::endl;

    std::this_thread::sleep_for(timer::get_print_delay());

    std::osyncstream(output_stream) << "Student " << get_id() << " has finished printing at time " << timer::get_microseconds_count() << std::endl;

    printer_locks[target_printer_id].lock();

    printer_busy[target_printer_id] = false;

    printer_locks[target_printer_id].unlock();

    get_group()->notify(get_id());
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

    std::osyncstream(output_stream) << "Group " << get_group()->get_id() << " has finished printing at time " << timer::get_microseconds_count() << std::endl;

    student::binder_semaphore.acquire();

    std::osyncstream(output_stream) << "Group " << get_group()->get_id() << " has started binding at time " << timer::get_microseconds_count() << std::endl;

    std::this_thread::sleep_for(timer::get_bind_delay());
    student::binder_semaphore.release();

    std::osyncstream(output_stream) << "Group " << get_group()->get_id() << " has finished binding at time " << timer::get_microseconds_count() << std::endl;

    staff::add_submission();

    std::osyncstream(output_stream) << "Group " << get_group()->get_id() << " has submitted the report at time " << timer::get_microseconds_count() << std::endl;
}

bool offline_4::student::release_print_if_waiting()
{
    bool to_return = false;

    waiting_lock.lock();

    if(waiting)
    {
        to_return = true;
        waiting = false;

        wait_semaphore->release();
    }

    waiting_lock.unlock();

    return to_return;
}

void offline_4::student::set_students(std::vector<student> *students)
{
    student::students = students;
}

std::vector<offline_4::student> *offline_4::student::get_students()
{
    return students;
}

bool offline_4::student::operator < (const student &other) const
{
    return id < other.id;
}

offline_4::student::~student()
{
    delete wait_semaphore;
}