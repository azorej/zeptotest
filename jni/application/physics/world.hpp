#ifndef ZEPTOTEST_PHYSICS_WORLD_HPP
#define ZEPTOTEST_PHYSICS_WORLD_HPP

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/vecmath.hpp"
#include "scheduled_task.hpp"

#include <chrono>
#include <list>

//forward declaration
class physic_element_if;

namespace physics
{

    class world_t : private noncopyable_t
    {
    public:
        world_t();

        void update(std::chrono::milliseconds dt);

        void register_object(physic_element_if* obj);
        void unregister_object(physic_element_if* obj);

        void set_size(vec2 const& size)
        {
			_size = size;
		}

    private:
        scheduled_task_t _update_task;
        vec2 _size;

        std::chrono::milliseconds _time_buffer;
        std::chrono::milliseconds _interval;

        std::list<physic_element_if*> _objects;

        void detect_collisions();
        void clean_up();
    };

}//end namespace physic

#endif // ZEPTOTEST_PHYSICS_WORLD_HPP
