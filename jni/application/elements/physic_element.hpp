/*
 * base_element.hpp
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_PHYSIC_ELEMENT_HPP_
#define ZEPTOTEST_PHYSIC_ELEMENT_HPP_

#include "defs.hpp"
#include "utils/vecmath.hpp"
#include "elements/element_if.hpp"
#include "physics/world.hpp"

#include <memory>

class physic_element_if : public element_if
{
public:
    physic_element_if(vec2 const& position, vec2 const& size, std::shared_ptr<physics::world_t> world)
    : element_if(position, size)
    , _world(world)
    , _velocity(0.f, 0.f)
	, _rotation_speed(0.f)
    {
        _world->register_object(this);
    }

    ~physic_element_if()
    {
        _world->unregister_object(this);
    }

    //nothing by default
    virtual void on_collide(physic_element_if*)
    {

    }

    //object will move for value 1.f per 1 second by velocity value 1.f (length of vector)
    void set_velocity(vec2 const& v)
    {
        _velocity = v;
    }
    vec2 get_velocity() const
    {
        return _velocity;
    }

    //object will rotate for 1 degree per 1 second by rotation speed value 1.f
    void set_rotation_speed(float spd)
    {
        _rotation_speed = spd;
    }
    float get_rotation_speed() const
    {
        return _rotation_speed;
    }

    std::shared_ptr<physics::world_t> get_world()
	{
    	return _world;
	}

private:
    vec2 _velocity;
    float _rotation_speed;
    std::shared_ptr<physics::world_t> _world;
};

#endif /* ZEPTOTEST_PHYSIC_ELEMENT_HPP_ */
