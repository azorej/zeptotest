#include "control_zone.hpp"

#include "space_ship.hpp"

static const float MAX_SPEED = 80.f;
static const float SPEED_FACTOR = 5.f;

void control_zone_t::on_touch(touch_t &touch)
{
    //In case if the element will be destroyed while the touch isn't
    auto shared = _controlled_element->shared_from_this();
    std::weak_ptr<element_if> weak = shared;

    auto on_move = [weak](touch_t* touch)
    {
        if(auto lock = weak.lock())
        {
            auto physic_ptr = dynamic_cast<physic_element_if*>(lock.get());
            my_assert(physic_ptr);

            auto end = touch->get_end();
            auto dest = touch->get_end();
            dest.y += 12;
            auto pos = physic_ptr->get_position();
            auto velocity = dest - pos;

            if(velocity.length() > 1)
            {
                velocity *= SPEED_FACTOR;

                float length = velocity.length();
                if(length > MAX_SPEED)
                {
                    velocity.normalize();
                    velocity *= MAX_SPEED;
                }

                physic_ptr->set_velocity(velocity);
            }
            else physic_ptr->set_velocity(
                    {   0.f, 0.f});
        }
    };

    auto on_end = [weak](touch_t* touch)
    {
        if(auto lock = weak.lock())
        {
            auto physic_ptr = dynamic_cast<physic_element_if*>(lock.get());
            my_assert(physic_ptr);

            physic_ptr->set_velocity(
                    {   0.f, 0.f});
        }
    };

    touch.set_on_move(on_move);
    touch.set_on_end(on_end);
    touch.set_on_cancel(on_end);
}
