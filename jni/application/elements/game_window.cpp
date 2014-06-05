#include "game_window.hpp"

#include "physics/world.hpp"
#include "elements/space_ship.hpp"
#include "elements/meteorite.hpp"
#include "elements/space_ship.hpp"
#include "elements/control_zone.hpp"
#include "elements/menu_window.hpp"
#include "subsystems/scheduler.hpp"
#include "application.hpp"
#include "utils/random.hpp"

static const std::chrono::seconds UPDATE_LOGIC_INTERVAL = std::chrono::seconds(1);
static const std::chrono::seconds BASE_METEORITE_CREATING_INTERVAL = std::chrono::seconds(5);
static const std::chrono::milliseconds MIN_METEORITE_CREATING_INTERVAL = std::chrono::milliseconds(100);
static const std::chrono::seconds LEVEL_UP_TIME = std::chrono::seconds(10);
static const float BASE_METEOR_SPEED = 10.f;
static const float METEOR_LEVEL_ACCELERATON = 10.f;

game_window_t::game_window_t()
: _time_from_start(0)
, _last_meteor_time(BASE_METEORITE_CREATING_INTERVAL)
{
    init_world();

    _player = add_child(new space_ship_t(vec2(0.f, -30.f), _world, this));

    add_child(new control_zone_t(vec2(0.f, 0.f), vec2(100.f, 100.f), _player));

    auto& scheduler = application_t::singleton().subsystem<subsystems::scheduler_t>();
    _update_logic_task = scheduler.schedule_repeat(std::bind(&game_window_t::update_logic, this, std::placeholders::_1), UPDATE_LOGIC_INTERVAL);
}

void game_window_t::game_over()
{
    application_t::singleton().set_main_window(new menu_window_t());
}

void game_window_t::init_world()
{
    _world.reset(new physics::world_t());
    _world->set_size(vec2(130.f, 130.f));
}

void game_window_t::update_logic(std::chrono::milliseconds dt)
{
	using namespace std::chrono;

	_time_from_start += dt;
	_last_meteor_time += dt;

	uint32_t level = (LEVEL_UP_TIME + _time_from_start) / LEVEL_UP_TIME;

	milliseconds meteor_interval = duration_cast<milliseconds>(BASE_METEORITE_CREATING_INTERVAL) / level;
	if(meteor_interval < MIN_METEORITE_CREATING_INTERVAL)
		meteor_interval = MIN_METEORITE_CREATING_INTERVAL;

	if(_last_meteor_time > meteor_interval)
	{
		create_meteorite(level);
		_last_meteor_time = milliseconds(0);
	}
}

void game_window_t::create_meteorite(uint32_t level)
{
	std::uniform_real_distribution<float> x_roll(-50,50);
	std::uniform_real_distribution<float> size_roll(8,17);

    vec2 pos = {x_roll(my_random_generator()), 55.f};
    vec2 size = {size_roll(my_random_generator()), size_roll(my_random_generator())};
    auto meteor = add_child(new meteorite_t(pos, size, _world));

    vec2 dest = {x_roll(my_random_generator()), -50.f};

    vec2 velocity = dest - pos;
    velocity.normalize();
    velocity *= BASE_METEOR_SPEED + METEOR_LEVEL_ACCELERATON*level;
    meteor->set_velocity(velocity);

    float rotation_speed = 10.f;
    meteor->set_rotation_speed(rotation_speed);
}
