/*
 * application.cpp
 *
 *  Created on: May 23, 2014
 *      Author: azorej
 */

#include "application.hpp"

#include "elements/mainwindow.hpp"

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

	_main_window.reset(new main_window_t());
}

void application_t::on_close()
{
	stop();
	_main_window.reset();
}

void application_t::on_save_state()
{
}

void application_t::tick()
{
	//let sheduler handle this
	subsystem<subsystems::scheduler_t>().tick();
}

void application_t::on_touch(raw_touch_arr const& touches)
{
	subsystem<subsystems::input_controller_t>().on_touch(touches);
}