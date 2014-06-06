#include "game_window.hpp"

#include "physics/world.hpp"
#include "elements/space_ship.hpp"
#include "elements/meteorite.hpp"
#include "elements/space_ship.hpp"
#include "elements/control_zone.hpp"
#include "elements/menu_window.hpp"
#include "elements/background.hpp"
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
        : _time_from_start(0),
          _last_meteor_time(BASE_METEORITE_CREATING_INTERVAL),
          _level(1)
{
    create_background();

    init_world();

    _player = add_child(new space_ship_t(vec2(0.f, -30.f), _world, this));

    add_child(new control_zone_t(vec2(0.f, 0.f), vec2(100.f, 100.f), _player));

    auto& scheduler = application_t::singleton().subsystem<subsystems::scheduler_t>();
    _update_logic_task = scheduler.schedule_repeat(std::bind(&game_window_t::update_logic, this, std::placeholders::_1),
            UPDATE_LOGIC_INTERVAL);
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

    _level = (LEVEL_UP_TIME + _time_from_start) / LEVEL_UP_TIME;

    milliseconds meteor_interval = duration_cast<milliseconds>(BASE_METEORITE_CREATING_INTERVAL) / _level;
    if (meteor_interval < MIN_METEORITE_CREATING_INTERVAL) meteor_interval = MIN_METEORITE_CREATING_INTERVAL;

    if (_last_meteor_time > meteor_interval)
    {
        create_meteorite();
        _last_meteor_time = milliseconds(0);
    }
}

void game_window_t::create_meteorite()
{
    std::uniform_real_distribution<float> x_roll(-50, 50);
    std::uniform_real_distribution<float> size_roll(8, 17);

    vec2 pos = {x_roll(my_random_generator()), 55.f};
    vec2 size = {size_roll(my_random_generator()), size_roll(my_random_generator())};
    auto meteor = add_child(new meteorite_t(pos, size, _world));

    vec2 dest = {x_roll(my_random_generator()), -50.f};

    vec2 velocity = dest - pos;
    velocity.normalize();
    velocity *= BASE_METEOR_SPEED + METEOR_LEVEL_ACCELERATON * _level;
    meteor->set_velocity(velocity);

    float rotation_speed = 10.f;
    meteor->set_rotation_speed(rotation_speed);
}

void game_window_t::create_background()
{
    auto bg_1 = add_child(new background_t("star_sky.png"));
    auto bg_2 = add_child(new background_t("star_sky.png"));
    bg_1->set_position({0.f, 0.f});
    bg_2->set_position({0.f, 100.f});
    auto move_background = [this, bg_1, bg_2] (std::chrono::milliseconds dt)
    {
        float dy = 0.3f;
        auto pos_1 = bg_1->get_position().y - dy;
        auto pos_2 = bg_2->get_position().y - dy;
        if(pos_1 < -100)
        {
            pos_1 = 100;
        }
        else if(pos_2 < -100)
        {
            pos_2 = 100;
        }
        bg_1->set_position(
                {   0.f, pos_1});
        bg_2->set_position(
                {   0.f, pos_2});

        auto interval = _move_background_task.get_interval();
        interval = std::chrono::milliseconds(20 + interval.count()/(_level+1));
        _move_background_task.set_interval(interval);
    };

    auto& scheduler = application_t::singleton().subsystem<subsystems::scheduler_t>();
    _move_background_task = scheduler.schedule_repeat(move_background, std::chrono::milliseconds(100));
}
