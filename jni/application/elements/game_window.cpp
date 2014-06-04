#include "game_window.h"

#include "physics/world.hpp"
#include "elements/sprite.hpp"
#include "elements/meteorite.hpp"

game_window_t::game_window_t(const vec2& size)
: element_if(vec2(0.f, 0.f), size)
{
	_world.reset(new physics::world_t());

	_world->set_size(vec2(120.f, 120.f));

	auto first_lev = add_child(new sprite_t(vec2(0, 0), vec2(20.f, 20.f)));
	auto second_lev = first_lev->add_child(new sprite_t(vec2(0,0), vec2(50, 50)));
	first_lev->set_angle(90);
	second_lev->set_angle(-30);

	auto meteor = add_child(new meteorite_t({20.f, -20.f}, {10.f, 10.f}, _world));
}

void game_window_t::on_touch(touch_t& touch)
{
    auto begin = touch.get_begin();
    LOG_FATAL_ERROR("Touch begin x:%f y:%f", begin.x, begin.y);
    touch.set_on_end([](touch_t* t) {
        auto end = t->get_end();
        LOG_FATAL_ERROR("Touch end x:%f y:%f", end.x, end.y);
    });
    touch.set_on_move([](touch_t* t) {
        auto move = t->get_move();
        LOG_FATAL_ERROR("Touch move x:%f y:%f", move.x, move.y);
    });
}
