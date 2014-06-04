/*
 * meteorite.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: azorej
 */

#include "meteorite.hpp"
#include "application.hpp"
#include "subsystems/graphic.hpp"
#include "graphics/fill_shader.hpp"

meteorite_t::meteorite_t(const vec2& pos, const vec2& size, std::shared_ptr<physics::world_t> world)
: physic_element_if(pos, size, world)
{
	auto polygon = polygon_t::create_random(17, vec2(100, 100));
	auto triangles = polygon.triangulate();
	for(auto& tr : triangles)
	{
		_vertices.push_back(tr.v1.x);
		_vertices.push_back(tr.v1.y);
		_vertices.push_back(tr.v2.x);
		_vertices.push_back(tr.v2.y);
		_vertices.push_back(tr.v3.x);
		_vertices.push_back(tr.v3.y);
	}
}

void meteorite_t::draw(mat3 const& mvp_matrix)
{
	auto& graph_sys = application_t::singleton().subsystem<subsystems::graphic_t>();
	auto& shader = graph_sys.get_shader<fill_shader_t>();

	shader.set_color({1.f, 0.f, 0.f, 1.f});
	shader.set_vertices(_vertices);
	shader.activate(mvp_matrix);
}
