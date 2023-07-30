#include <iostream>
#include <vector>
#include "student.hpp"
#include "group.hpp"

int main()
{
    size_t n, m;

    std::cin >> n >> m;

    if(n <= 0 || m <= 0)
    {
        std::cerr << "Number of students and size of groups should be a positive integer" << std::endl;

        return -1;
    }
    else if(n % m != 0)
    {
        std::cerr << "Size of groups should be divisible by number of groups" << std::endl;

        return -1;
    }

    size_t w, x, y;

    std::cin >> w >> x >> y;

    if(w <= 0 || x <= 0 || y <= 0)
    {
        std::cerr << "Relative time should be a positive integer" << std::endl;

        return -1;
    }

    offline_4::student::set_print_delay(w);
    offline_4::student::set_bind_delay(x);
    offline_4::student::set_submission_delay(y);

    std::vector<offline_4::student> students(n);
    std::vector<offline_4::group> groups(n / m);

    for(size_t i = 0; i < n; ++i)
    {
        students[i].set_id(i + 1);
    }

    for(size_t i = 0; i * m < n; ++i)
    {
        size_t leader_id = (i + 1) * m;

        groups[i].set_id(i + 1);
        groups[i].set_leader(&students[leader_id - 1]);

        for(size_t j = leader_id - m; j < leader_id; ++j)
        {
            students[j].set_group(&groups[i]);
        }
    }

    for(size_t i = 0; i < n; ++i)
    {
        students[i].start_action();
    }

    for(size_t i = 0; i < n; ++i)
    {
        students[i].join_thread();
    }

    return 0;
}