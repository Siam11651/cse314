#include "individual.hpp"

offline_4::individual::individual(const uint64_t &id)
{
    set_id(id);
}

void offline_4::individual::set_id(const uint64_t &id)
{
    this->id = id;
}

uint64_t offline_4::individual::get_id() const
{
    return id;
}

void offline_4::individual::start_action()
{
    thread = std::thread([this]()
    {
        action();
    });
}

void offline_4::individual::join_thread()
{
    thread.join();
}

void offline_4::individual::action()
{

}