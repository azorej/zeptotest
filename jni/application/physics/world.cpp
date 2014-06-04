#include "physics/world.hpp"

#include "application.hpp"
#include "utils/vecmath.hpp"
#include "utils/geometry.hpp"
#include "elements/physic_element.hpp"
#include "subsystems/scheduler.hpp"

#include <cmath>

#define PHYSICS_UPDATE_FREQUENCY 60.f


namespace physics
{

    world_t::world_t()
    : _time_buffer(std::chrono::milliseconds(0))
    , _size(100, 100)
    {
        auto& scheduler = application_t::singleton().subsystem<subsystems::scheduler_t>();
        _interval = std::chrono::milliseconds(long(1000.f/PHYSICS_UPDATE_FREQUENCY));

        _update_task = scheduler.schedule_repeat(std::bind(&physics::world_t::update, this, std::placeholders::_1), _interval);
    }

    void world_t::update(std::chrono::milliseconds dt)
    {
        _time_buffer += dt;

        size_t cycles = _time_buffer/_interval;
        _time_buffer -= _interval * cycles;

        for(size_t i=0; i < cycles; ++i)
        {
            for(auto& obj : _objects)
            {
                vec2 path = (obj->get_velocity() * _interval.count())/1000.f;
                obj->set_position(obj->get_position() + path);
            }

            detect_collisions();

            clean_up();
        }
    }

    void world_t::detect_collisions()
    {
        for(auto& obj1 : _objects)
        {
            auto aabb_1 = obj1->get_aabb();
            for(auto& obj2 : _objects)
            {
                auto aabb_2 = obj2->get_aabb();
                if(is_intersect(aabb_1, aabb_2))
                {
                    obj1->on_collide();
                    obj2->on_collide();
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

    void world_t::clean_up()
    {
        for(auto& obj : _objects)
        {
            auto aabb = obj->get_aabb();
            auto max_x = aabb.size.x/2 + std::abs(aabb.position.x);
            auto max_y = aabb.size.y/2 + std::abs(aabb.position.y);

            if(max_x > _size.x || max_y > _size.y) obj->destroy();
        }
    }

}//end namespace physics
