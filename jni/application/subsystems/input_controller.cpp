/*
 * inputcontroller.cpp
 *
 *  Created on: Jun 2, 2014
 *      Author: azorej
 */

#include "input_controller.hpp"

#include "application.hpp"
#include "elements/element_if.hpp"

namespace subsystems
{

} /* namespace subsystems */

void subsystems::input_controller_t::start()
{
    _is_enabled = true;
}

void subsystems::input_controller_t::stop()
{
    _is_enabled = false;
}

subsystems::input_controller_t::input_controller_t()
        : _is_enabled(false)
{
}

void subsystems::input_controller_t::on_touch(AInputEvent* event)
{
    if (_is_enabled)
    {
        int32_t action = AMotionEvent_getAction(event);
        unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
        switch (flags)
        {
            case AMOTION_EVENT_ACTION_DOWN:
            {
                auto id = AMotionEvent_getPointerId(event, 0);
                vec2 begin = {AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0)};
                auto res = _touches.insert(std::make_pair(id, touch_t(begin)));

                auto main_wind = application_t::singleton().get_main_window();
                if (main_wind)
                {
                    main_wind->on_touch(res.first->second);
                }
                break;
            }
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
            {
                int32_t idx = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                        >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                auto id = AMotionEvent_getPointerId(event, idx);
                vec2 begin = {AMotionEvent_getX(event, idx), AMotionEvent_getY(event, idx)};
                auto res = _touches.insert(std::make_pair(id, touch_t(begin)));

                auto main_wind = application_t::singleton().get_main_window();
                if (main_wind)
                {
                    main_wind->on_touch(res.first->second);
                }
                break;
            }
            case AMOTION_EVENT_ACTION_UP:
            {
                for (auto& p : _touches)
                {
                    int32_t idx = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                            >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                    p.second.set_move({AMotionEvent_getX(event, idx), AMotionEvent_getY(event, idx)});
                    p.second.on_end();
                }
                _touches.clear();
                break;
            }
            case AMOTION_EVENT_ACTION_POINTER_UP:
            {
                int32_t idx = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                        >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                int32_t released_pointer_id = AMotionEvent_getPointerId(event, idx);
                auto it = _touches.find(released_pointer_id);
                my_assert(it != _touches.end());

                it->second.set_move({AMotionEvent_getX(event, idx), AMotionEvent_getY(event, idx)});
                it->second.on_end();

                _touches.erase(it);
                break;
            }
            case AMOTION_EVENT_ACTION_MOVE:
            {
                int32_t idx = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                        >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                auto id = AMotionEvent_getPointerId(event, idx);
                auto it = _touches.find(id);
                my_assert(it != _touches.end());

                it->second.set_move({AMotionEvent_getX(event, idx), AMotionEvent_getY(event, idx)});
                it->second.on_move();

                break;
            }
            case AMOTION_EVENT_ACTION_CANCEL:
            case AMOTION_EVENT_ACTION_OUTSIDE:
            {
                for (auto& p : _touches)
                {
                    p.second.on_cancel();
                }
                _touches.clear();
                break;
            }
        }
    }
}
