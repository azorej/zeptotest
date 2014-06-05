/*
 * space_ship.hpp
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_SPRITE_HPP_
#define ZEPTOTEST_SPRITE_HPP_

#include "defs.hpp"
#include "elements/physic_element.hpp"
#include "scheduled_task.hpp"

#include <memory>

#define SPACE_SHIP_SIZE {10.f, 10.f}

//forward declarations
class game_window_t;

class space_ship_t: public physic_element_if
{
public:
    space_ship_t(vec2 const& position, std::shared_ptr<physics::world_t> world, game_window_t* game);

    void on_collide(physic_element_if *);

    void shoot();

private:
    game_window_t* _game;
    scheduled_task_t _shooting_task;
};

#endif /* SPRITE_HPP_ */
