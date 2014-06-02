#ifndef ZEPTOTEST_PHYSICS_WORLD_HPP
#define ZEPTOTEST_PHYSICS_WORLD_HPP

#include "defs.hpp"
#include "utils/noncopyable.hpp"

#include <chrono>

namespace physics
{

    class world_t : private noncopyable_t
    {
    public:
        void update(std::chrono::milliseconds dt);
    };

}//end namespace physics

#endif // ZEPTOTEST_PHYSICS_WORLD_HPP
