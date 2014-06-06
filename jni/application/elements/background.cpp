/*
 * background.cpp
 *
 *  Created on: Jun 5, 2014
 *      Author: azorej
 */

#include "background.hpp"

#include "graphics/texture_shader.hpp"
#include "application.hpp"
#include "subsystems/resource_manager.hpp"
#include "subsystems/graphic.hpp"

background_t::background_t(const std::string& texture_name)
        : element_if({0.f, 0.f}, {100.f, 100.f}),
          _texture(texture_name)
{
}

void background_t::draw(const mat3& mvp_matrix)
{
    auto& graph_sys = application_t::singleton().subsystem<subsystems::graphic_t>();
    auto& shader = graph_sys.get_shader<texture_shader_t>();

    shader.set_texture(
            application_t::singleton().subsystem<subsystems::resource_manager_t>().get_texture(_texture.c_str()));
    shader.activate(mvp_matrix);
}
