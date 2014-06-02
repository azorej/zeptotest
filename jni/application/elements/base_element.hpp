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

#include <list>
#include <memory>

class base_element_t
{
public:
	base_element_t(vec2 const& position, vec2 const& size)
	: _position(position)
	, _size(size)
	, _angle(0)
	{
		_projection_matrix = mat3::view_to_world(COORDINATE_FACTOR, 1);
	}

	virtual ~base_element_t() = 0;

	virtual void draw(mat3 const&)
	{
		//do nothing by default
	}

	void do_draw(mat3 const& vp_matrix)
	{
		//двигаем элемент относительно родителя,
		mat3 new_vp_matrix = vp_matrix * mat3::translation(_position.x, _position.y) * mat3::rotation(_angle) * mat3::scaling(_size.x/2, _size.y/2);

		//пересчитаем координаты
		new_vp_matrix *= get_projection_matrix();
		draw(new_vp_matrix);
		for(auto& child_ptr : _children)
		{
			child_ptr->do_draw(new_vp_matrix);
		}
	}

	base_element_t* add_children(base_element_t* child)
	{
		_children.push_back(std::shared_ptr<base_element_t>(child));
		return child;
	}

	mat3 const& get_projection_matrix()
	{
		return _projection_matrix;
	}

	void set_angle(float angle)
	{
		_angle = angle;
	}

	float get_angle() const
	{
		return _angle;
	}

protected:
	std::list<std::shared_ptr<base_element_t> > _children;
	mat3 _projection_matrix;
	vec2 _position;
	vec2 _size;
	float _angle;

};

inline base_element_t::~base_element_t()
{
}



#endif /* ZEPTOTEST_BASE_ELEMENT_HPP_ */
