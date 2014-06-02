/*
 * mainwindow.h
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_MAINWINDOW_HPP_
#define ZEPTOTEST_MAINWINDOW_HPP_

#include "defs.hpp"
#include "elements/base_element.hpp"
#include "utils/vecmath.hpp"
#include "touch.hpp"

class main_window_t
{
public:
	main_window_t();

	void draw()
	{
		for(auto& child_ptr : _children)
		{
			child_ptr->do_draw(_projection_matrix);
		}
	}

	base_element_t* add_children(base_element_t* child)
	{
		_children.push_back(std::shared_ptr<base_element_t>(child));
		return child;
	}

	void on_touch(touch_array_t& touches);

	protected:
		std::list<std::shared_ptr<base_element_t> > _children;
		mat3 _projection_matrix;
};

#endif /* ZEPTOTEST_MAINWINDOW_HPP_ */
