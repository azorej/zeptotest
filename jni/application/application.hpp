/*
 * application.hpp
 *
 *  Created on: May 23, 2014
 *      Author: azorej
 */
#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/subsystem.hpp"
#include "elements/mainwindow.hpp"

#include "android_native_app_glue.h"

#include <memory>

#ifndef ZEPTOTEST_APPLICATION_HPP_
#define ZEPTOTEST_APPLICATION_HPP_

struct raw_touch_t
{
	int32_t id;
	float x;
	float y;
};

std::vector<raw_touch_t> raw_touch_arr;

class application_t : private noncopyable_t, public master_t
{
public:
	void init(android_app* java_app);

	/*
	 * The window is being shown
	 */
	void on_shown();

	/*
	 * The window is being hidden or closed
	 */
	void on_close();

	void on_save_state();

	void on_touch(raw_touch_arr const& touches);

	void tick();

    static application_t& singleton()
    {
    	static application_t instance;
    	return instance;
    }

    main_window_t* get_main_window()
    {
    	return _main_window.get();
    }

private:
    application_t() = default;

	android_app* _java_app;
	std::unique_ptr<main_window_t> _main_window;
};

#endif /* ZEPTOTEST_APPLICATION_HPP_ */
