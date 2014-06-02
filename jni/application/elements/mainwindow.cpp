/*
 * mainwindow.cpp
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#include "elements/mainwindow.hpp"

#include "application.hpp"
#include "subsystems/graphic.hpp"
#include "elements/sprite.hpp"

main_window_t::main_window_t()
{
	float aspect_ratio = application_t::singleton().subsystem<subsystems::graphic_t>().get_screen_ratio();

	_projection_matrix = mat3::view_to_world(COORDINATE_FACTOR, aspect_ratio);

	auto first_lev = add_children(new sprite_t(vec2(-15, 15/aspect_ratio), vec2(50, 25)));
	auto second_lev = first_lev->add_children(new sprite_t(vec2(0,0), vec2(50, 50)));
	first_lev->set_angle(-135);
	second_lev->set_angle(45);
}

void main_window_t::on_touch(touch_array_t& touches)
{
	auto& graphic = application_t::singleton().subsystem<subsystems::graphic_t>();
	auto w = graphic.get_screen_width();
	auto h = graphic.get_screen_height();

	static const mat3 transform = mat3::translation(-50.f, 50.f) * mat3::scaling(100.f/w, -100.f/h);

	for(auto& touch : touches)
	{
		float arr[9] = {
				100.f/w, 0.f     , 0.f,
				0.f    , -100.f/h, 0.f,
				-50.f  , 50.f    , 1.f
		};
		//mat3 transform(arr);

		vec3 coord(touch.x, touch.y, 1);
		vec3 new_coord = transform * coord;

		LOG_FATAL_ERROR("Touch x:%f y:%f", new_coord.x, new_coord.y);
	}
}
