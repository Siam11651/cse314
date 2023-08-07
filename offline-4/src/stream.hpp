#pragma once

#include <fstream>

namespace offline_4
{
    class stream
    {
    private:
        static std::ifstream input_stream;
        static std::ofstream output_stream;

    public:
        static std::ifstream &get_input_stream();
        static std::ofstream &get_output_stream();
    };
}