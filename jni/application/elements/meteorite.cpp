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
#include "utils/random.hpp"

meteorite_t::meteorite_t(const vec2& pos, const vec2& size, std::shared_ptr<physics::world_t> world)
: physic_element_if(pos, size, world)
, is_descedant(false)
{
    set_collision_group(physics::COLLISION_GROUP_2);

    std::uniform_int_distribution<size_t> verices_count_roll(7,23);
    size_t verices_count = verices_count_roll(my_random_generator());

	auto polygon = polygon_t::create_random(verices_count, vec2(100, 100));
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

	set_rotation_speed(10.f);
}

void meteorite_t::draw(mat3 const& mvp_matrix)
{
	auto& graph_sys = application_t::singleton().subsystem<subsystems::graphic_t>();
	auto& shader = graph_sys.get_shader<fill_shader_t>();

	shader.set_color({0.5f, 0.25f, 0.f, 1.f});
	shader.set_vertices(_vertices);
    shader.activate(mvp_matrix);
}

void meteorite_t::on_collide(physic_element_if *other)
{
	if(!is_descedant)
	{
		auto descedant_1 = _parent->add_child(new meteorite_t(_position, _size/2, get_world()));
		auto descedant_2 = _parent->add_child(new meteorite_t(_position, _size/2, get_world()));

		descedant_1->is_descedant = true;
		descedant_2->is_descedant = true;

		vec2 vel_1 = vec2(mat3::rotation(30 * M_PI / 180) * (vec3(get_velocity(), 0)));
		vec2 vel_2 = vec2(mat3::rotation(-30 * M_PI / 180 ) * (vec3(get_velocity(), 0)));
		descedant_1->set_velocity(vel_1);
		descedant_2->set_velocity(vel_2);
	}

    this->destroy();
}
