/*
 * application.cpp
 *
 *  Created on: May 23, 2014
 *      Author: azorej
 */

#include "application.hpp"

#include "elements/game_window.h"

#include "subsystems/graphic.hpp"
#include "subsystems/scheduler.hpp"
#include "subsystems/resource_manager.hpp"
#include "subsystems/input_controller.hpp"

#include <thread>

void application_t::init(android_app* java_app)
{
	//std::this_thread::sleep_for(std::chrono::seconds(1));
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

    auto& graphic = subsystem<subsystems::graphic_t>();
    float aspect_ratio = graphic.get_screen_ratio();

    vec2 size;
    if(aspect_ratio>1)
    {
        size.x = 100.f;
        size.y = 100.f * aspect_ratio;
    }
    else
    {
    	size.x = 100.f / aspect_ratio;
    	size.y = 100.f;
    }

    _main_window.reset(new game_window_t(size));
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
