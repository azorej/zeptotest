#include "random.hpp"

std::default_random_engine& my_random_generator()
{
    //not the best idea to use time as a seed
    //but random_device can be not implemented
    static std::default_random_engine random_generator((unsigned int) time(0));
    return random_generator;
}
