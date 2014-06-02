/*
 * sprite.cpp
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#include "elements/sprite.hpp"

#include "application.hpp"
#include "subsystems/graphic.hpp"
#include "subsystems/resource_manager.hpp"
#include "graphics/texture_shader.hpp"

sprite_t::sprite_t(vec2 const& position, vec2 const& size)
: base_element_t(position, size)
{
}


void sprite_t::draw(mat3 const& mvp_matrix)
{
	auto& graph_sys = application_t::singleton().subsystem<subsystems::graphic_t>();
	auto& shader = graph_sys.get_shader<texture_shader_t>();

	shader.set_texture(application_t::singleton().subsystem<subsystems::resource_manager_t>().get_texture("space_shuttle.png"));
	shader.activate(mvp_matrix);
}
