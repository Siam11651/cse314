#include "stream.hpp"

std::ifstream offline_4::stream::input_stream("input.txt");
std::ofstream offline_4::stream::output_stream("output.txt");

std::ifstream &offline_4::stream::get_input_stream()
{
    return input_stream;
}

std::ofstream &offline_4::stream::get_output_stream()
{
    return output_stream;
}