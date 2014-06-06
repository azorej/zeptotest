#ifndef ZEPTOTEST_BULLET_HPP
#define ZEPTOTEST_BULLET_HPP

#include "defs.hpp"
#include "physic_element.hpp"
#include "utils/vecmath.hpp"

class bullet_t: public physic_element_if
{
public:
    bullet_t(vec2 const& position, vec2 const& direction, std::shared_ptr<physics::world_t> world);

    void on_collide(physic_element_if *other);
};

#endif // ZEPTOTEST_BULLET_HPP