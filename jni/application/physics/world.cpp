#include "physics/world.hpp"

#include "application.hpp"
#include "utils/vecmath.hpp"
#include "utils/geometry.hpp"
#include "elements/physic_element.hpp"
#include "subsystems/scheduler.hpp"

#include <cmath>

static const float PHYSICS_UPDATE_FREQUENCY = 30.f;

namespace physics
{

    world_t::world_t()
            : _time_buffer(std::chrono::milliseconds(0)),
              _size(100, 100)
    {
        auto& scheduler = application_t::singleton().subsystem<subsystems::scheduler_t>();
        _interval = std::chrono::milliseconds(long(1000.f / PHYSICS_UPDATE_FREQUENCY));

        _update_task = scheduler.schedule_repeat(std::bind(&physics::world_t::update, this, std::placeholders::_1),
                _interval);
    }

    void world_t::update(std::chrono::milliseconds dt)
    {
        _time_buffer += dt;

        size_t cycles = _time_buffer / _interval;

        for (size_t i = 0; i < cycles; ++i)
        {
            auto temp_objects = lock_objects();

            for (auto& obj : temp_objects)
            {
                auto p_obj = static_cast<physic_element_if*>(obj.get());

                vec2 path = (p_obj->get_velocity() * _interval.count()) / 1000.f;
                p_obj->set_position(obj->get_position() + path);
                //auto angle = p_obj->get_rotation_speed() * M_PI /(180 * 1000);
                //p_obj->set_angle( obj->get_angle() + angle );
            }

            detect_collisions(temp_objects);

            clean_up(temp_objects);
        }

        _time_buffer -= _interval * cycles;
    }

    void world_t::detect_collisions(std::vector<std::shared_ptr<element_if> >& objects)
    {
        for (auto it_1 = objects.begin(), end = objects.end(); it_1 != end; ++it_1)
        {
            auto p_obj1 = static_cast<physic_element_if*>((*it_1).get());

            auto group_1 = p_obj1->get_collision_group();
            auto aabb_1 = p_obj1->get_aabb();
            for (auto it_2 = (it_1 + 1); it_2 != end; ++it_2)
            {
                auto p_obj2 = static_cast<physic_element_if*>((*it_2).get());

                auto group_2 = p_obj2->get_collision_group();
                if (!(group_1 & group_2)) //Only objects of the same group can collide
                {
                    auto aabb_2 = p_obj2->get_aabb();
                    if (is_intersect(aabb_1, aabb_2))
                    {
                        p_obj1->on_collide(p_obj2);
                        p_obj2->on_collide(p_obj1);
                    }
                }
            }
        }
    }

    void world_t::register_object(physic_element_if* obj)
    {
        _objects.push_back(obj);
    }

    void world_t::unregister_object(physic_element_if* obj)
    {
        auto remove_me = std::find(_objects.begin(), _objects.end(), obj);
        my_assert(remove_me != _objects.end());
        _objects.erase(remove_me);
    }

    void world_t::clean_up(std::vector<std::shared_ptr<element_if> >& objects)
    {
        for (auto& obj : objects)
        {
            auto aabb = obj->get_aabb();
            auto max_x = aabb.size.x / 2 + std::abs(aabb.position.x);
            auto max_y = aabb.size.y / 2 + std::abs(aabb.position.y);

            if (max_x > _size.x || max_y > _size.y) obj->destroy();
        }
    }

    std::vector<std::shared_ptr<element_if> > world_t::lock_objects()
    {
        std::vector<std::shared_ptr<element_if> > ret;
        ret.reserve(_objects.size());

        for (auto obj : _objects)
        {
            ret.push_back(obj->shared_from_this());
        }

        return ret;
    }

} //end namespace physics
