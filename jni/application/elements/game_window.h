#ifndef ZEPTOTEST_GAME_WINDOW_H
#define ZEPTOTEST_GAME_WINDOW_H

#include "defs.hpp"
#include "elements/element_if.hpp"
#include "physics/world.hpp"

#include <memory>

class game_window_t : public element_if
{
public:
    explicit game_window_t(vec2 const& size);

    virtual void on_touch(touch_t& touch) override;
private:
    std::shared_ptr<physics::world_t> _world;
};

#endif // GAME_WINDOW_H
