/*
 * sprite.cpp
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#include "elements/space_ship.hpp"

#include "application.hpp"
#include "subsystems/graphic.hpp"
#include "subsystems/resource_manager.hpp"
#include "subsystems/scheduler.hpp"
#include "graphics/texture_shader.hpp"
#include "elements/game_window.hpp"
#include "elements/bullet.hpp"

#define SHOOTING_FREQUENCY 1.f

space_ship_t::space_ship_t(vec2 const& position, std::shared_ptr<physics::world_t> world, game_window_t *game)
        : physic_element_if(position, SPACE_SHIP_SIZE, world),
          _game(game)
{
    set_collision_group(physics::COLLISION_GROUP_1);

    set_background("space_shuttle.png");

    auto& scheduler = application_t::singleton().subsystem<subsystems::scheduler_t>();
    auto interval = std::chrono::milliseconds(long(1000.f / SHOOTING_FREQUENCY));
    _shooting_task = scheduler.schedule_repeat(std::bind(&space_ship_t::shoot, this), interval);
}

void space_ship_t::on_collide(physic_element_if *)
{
    _game->game_over();
}

void space_ship_t::shoot()
{
    auto parent = get_parent();
    if (parent) parent->add_child(new bullet_t(get_position(), {0, 1}, get_world()));
}
