#ifndef ZEPTOTEST_GAME_WINDOW_H
#define ZEPTOTEST_GAME_WINDOW_H

#include "defs.hpp"
#include "elements/element_if.hpp"
#include "elements/main_window.hpp"
#include "physics/world.hpp"
#include "scheduled_task.hpp"

#include <memory>
#include <chrono>

//forward declarations
class space_ship_t;

class game_window_t: public main_window_if
{
public:
    game_window_t();

    void game_over();

private:
    std::shared_ptr<physics::world_t> _world;
    scheduled_task_t _update_logic_task;
    scheduled_task_t _move_background_task;
    space_ship_t* _player;
    uint32_t _level;

    std::chrono::milliseconds _time_from_start;
    std::chrono::milliseconds _last_meteor_time;

    void init_world();

    void update_logic(std::chrono::milliseconds dt);
    void create_meteorite();
    void create_background();
};

#endif // GAME_WINDOW_H