#include "random.hpp"

std::mutex offline_4::random::rng_lock;
std::mt19937_64 offline_4::random::mt(39);
std::poisson_distribution<uint64_t> offline_4::random::poisson_distribution(2.5);

uint64_t offline_4::random::get_poisson_distribution()
{
    rng_lock.lock();

    uint64_t to_return = poisson_distribution(mt);

    rng_lock.unlock();

    return to_return;
}