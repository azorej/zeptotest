/*
 * base_element.hpp
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_BASE_ELEMENT_HPP_
#define ZEPTOTEST_BASE_ELEMENT_HPP_

#include "defs.hpp"

#include "utils/vecmath.hpp"
#include "utils/geometry.hpp"
#include "touch.hpp"

#include <list>
#include <memory>
#include <cmath>

class element_if
{
public:
    element_if(vec2 const& position, vec2 const& size)
	: _position(position)
	, _size(size)
	, _angle(0)
    , _matrixes_changed(true)
	, _parent(nullptr)
	{
	}

    virtual ~element_if() = 0;

    virtual void draw(mat3 const&)
    {
        //do nothing by default
    }

    void do_draw(mat3 const& vp_matrix)
    {
        mat3 new_vp_matrix = vp_matrix * child_to_parent_matrix();

        draw(new_vp_matrix);
        for(auto& child_ptr : _children)
        {
            child_ptr->do_draw(new_vp_matrix);
        }
    }

    element_if* add_child(element_if* child)
    {
        _children.push_back(std::shared_ptr<element_if>(child));
        child->set_parent(this);
        return child;
    }

    void remove_child(element_if* child)
    {
        for(auto& child_ptr : _children)
        {
            if(child_ptr.get() == child)
            {
                _children.remove(child_ptr);
                break;
            }
        }
    }

	void set_angle(float angle)
	{
        _matrixes_changed = true;
		_angle = angle * 180.f/M_PI;
	}

	float get_angle() const
	{
		return _angle;
    }

    void set_position(vec2 pos)
    {
        _matrixes_changed = true;
        _position = pos;
    }

    vec2 const& get_position() const
    {
        return _position;
    }

    void set_size(vec2 const& size)
    {
        _matrixes_changed = true;
        _size = size;
    }

    vec2 get_size() const
    {
        return _size;
    }

    //delegate to children by default
    virtual void on_touch(touch_t& touch)
    {
        touch.add_transform_matrix(parent_to_child_matrix());

        auto pos = touch.get_begin();
        for(auto& child_ptr : _children)
        {
            auto aabb = child_ptr->get_aabb();
            if(is_intersect(pos, aabb))
            {
                child_ptr->on_touch(touch);
            }
        }
    }

    mat3 parent_to_child_matrix()
    {
        if(_matrixes_changed) calculate_transform_matrixes();
        return _parent_to_child_mat;
    }

    mat3 child_to_parent_matrix()
    {
        if(_matrixes_changed) calculate_transform_matrixes();
        return _child_to_parent_mat;
    }

    rect_t get_aabb() const
    {
        return {_size, _position};
    }

    void destroy()
    {
        if(_parent) _parent->remove_child(this);
    }

    void set_parent(element_if* parent)
    {
    	_parent = parent;
    }

protected:
    std::list<std::shared_ptr<element_if> > _children;
	vec2 _position;
	vec2 _size;
	float _angle;
	element_if* _parent;

    mat3 _child_to_parent_mat;
    mat3 _parent_to_child_mat;
    bool _matrixes_changed;

    element_if* get_child(vec2 const& coord)
    {
        element_if* ret = 0;

        for(auto& child_ptr : _children)
        {
            auto aabb = child_ptr->get_aabb();
            if(is_intersect(coord, aabb))
            {
                ret = child_ptr.get();
                break;
            }
        }

        return ret;
    }

    void calculate_transform_matrixes()
    {
        _child_to_parent_mat = mat3::translation(_position.x, _position.y) * mat3::rotation(_angle) * mat3::scaling(_size.x/COORDINATE_FACTOR, _size.y/COORDINATE_FACTOR);
        _parent_to_child_mat = _child_to_parent_mat;
        _parent_to_child_mat.inverse();

        _matrixes_changed = false;
    }
};

inline element_if::~element_if()
{
}



#endif /* ZEPTOTEST_BASE_ELEMENT_HPP_ */
