#ifndef ZEPTOTEST_PHYSICS_WORLD_HPP
#define ZEPTOTEST_PHYSICS_WORLD_HPP

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/vecmath.hpp"
#include "scheduled_task.hpp"
#include "elements/element_if.hpp"

#include <chrono>
#include <list>
#include <vector>

//forward declaration
class physic_element_if;

namespace physics
{
    enum
    {
        COLLISION_GROUP_1 = 1,
        COLLISION_GROUP_2 = COLLISION_GROUP_1 << 1,
        COLLISION_GROUP_3 = COLLISION_GROUP_2 << 1,
        COLLISION_GROUP_4 = COLLISION_GROUP_3 << 1,
        COLLISION_GROUP_5 = COLLISION_GROUP_4 << 1,
        COLLISION_GROUP_6 = COLLISION_GROUP_5 << 1
    };

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

        void detect_collisions(std::vector<std::shared_ptr<element_if> >& objects);
        void clean_up(std::vector<std::shared_ptr<element_if> >& objects);
        //перед операциями со списками необходимо защититься от удаления объектов во время переборов контейнера
        std::vector<std::shared_ptr<element_if> > lock_objects();
    };

}//end namespace physic

#endif // ZEPTOTEST_PHYSICS_WORLD_HPP
