/*
 * application.cpp
 *
 *  Created on: May 23, 2014
 *      Author: azorej
 */

#include "application.hpp"

#include "elements/game_window.hpp"
#include "subsystems/graphic.hpp"
#include "subsystems/scheduler.hpp"
#include "subsystems/resource_manager.hpp"
#include "subsystems/input_controller.hpp"

void application_t::init(android_app* java_app)
{
    _java_app = java_app;

    //order is important!
    //the susbsystems will be started in that order and stopped in reverse order
    add_managed_subsystem<subsystems::graphic_t>(java_app);
    add_managed_subsystem<subsystems::resource_manager_t>(java_app);
    add_managed_subsystem<subsystems::scheduler_t>();
    add_managed_subsystem<subsystems::input_controller_t>();
}

void application_t::on_shown()
{
    start();

    set_main_window(new game_window_t());
}

void application_t::on_close()
{
    _main_window.reset();
    stop();
}

void application_t::on_save_state()
{
}

void application_t::tick()
{
    //let sheduler handle this
    subsystem<subsystems::scheduler_t>().tick();
}

void application_t::on_touch(AInputEvent* event)
{
    subsystem<subsystems::input_controller_t>().on_touch(event);
}

element_if* application_t::get_main_window()
{
    return _main_window.get();
}

void application_t::set_main_window(element_if* new_main_window)
{
    _main_window.reset(new_main_window);
}
