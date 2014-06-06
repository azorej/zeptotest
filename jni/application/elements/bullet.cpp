#include "bullet.hpp"

static const vec2 BULLET_SIZE = {1.5f, 3.f};
static const float BULLET_SPEED = 85.f;

bullet_t::bullet_t(vec2 const& position, const vec2 &direction, std::shared_ptr<physics::world_t> world)
        : physic_element_if(position, BULLET_SIZE, world)
{
    set_collision_group(physics::COLLISION_GROUP_1);

    vec2 velocity = direction;
    velocity.normalize();
    set_velocity(velocity * BULLET_SPEED);

    set_background("bullet.png");
}

void bullet_t::on_collide(physic_element_if *other)
{
    this->destroy();
}
