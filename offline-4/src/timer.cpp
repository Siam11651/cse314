#include "timer.hpp"

std::chrono::high_resolution_clock::time_point offline_4::timer::start_point = std::chrono::high_resolution_clock::now();
std::chrono::seconds offline_4::timer::print_delay;
std::chrono::seconds offline_4::timer::bind_delay;
std::chrono::seconds offline_4::timer::submission_delay;

uint64_t offline_4::timer::get_microseconds_count()
{
    std::chrono::high_resolution_clock::time_point current_point = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(current_point - start_point);

    return duration.count();
}

void offline_4::timer::set_print_delay(const uint64_t &delay)
{
    print_delay = std::chrono::seconds(delay);
}

void offline_4::timer::set_bind_delay(const uint64_t &delay)
{
    bind_delay = std::chrono::seconds(delay);
}

void offline_4::timer::set_submission_delay(const uint64_t &delay)
{
    submission_delay = std::chrono::seconds(delay);
}

std::chrono::seconds offline_4::timer::get_print_delay()
{
    return print_delay;
}

std::chrono::seconds offline_4::timer::get_bind_delay()
{
    return bind_delay;
}

std::chrono::seconds offline_4::timer::get_submission_delay()
{
    return submission_delay;
}