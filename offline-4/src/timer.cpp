#include "timer.hpp"

std::chrono::high_resolution_clock::time_point offline_4::timer::start_point = std::chrono::high_resolution_clock::now();

uint64_t offline_4::timer::get_microseconds_count()
{
    std::chrono::high_resolution_clock::time_point current_point = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(current_point - start_point);

    return duration.count();
}