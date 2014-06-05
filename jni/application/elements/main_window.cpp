/*
 * main_window.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: azorej
 */
#include "main_window.hpp"
#include "application.hpp"
#include "subsystems/graphic.hpp"

main_window_if::main_window_if()
: element_if({0.f, 0.f}, {0.f, 0.f})
{
	auto& graphic = application_t::singleton().subsystem<subsystems::graphic_t>();
	float aspect_ratio = graphic.get_screen_ratio();

	vec2 size;
	if(aspect_ratio>1)
	{
		size.x = 100.f;
		size.y = 100.f * aspect_ratio;
	}
	else
	{
		size.x = 100.f / aspect_ratio;
		size.y = 100.f;
	}

	set_size(size);
}
