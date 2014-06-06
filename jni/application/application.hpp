/*
 * application.hpp
 *
 *  Created on: May 23, 2014
 *      Author: azorej
 */
#ifndef ZEPTOTEST_APPLICATION_HPP_
#define ZEPTOTEST_APPLICATION_HPP_

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/subsystem.hpp"
#include "elements/element_if.hpp"

#include "android_native_app_glue.h"

#include <memory>

class application_t: private noncopyable_t, public master_t
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

    void on_touch(AInputEvent* event);

    void tick();

    static application_t& singleton()
    {
        static application_t instance;
        return instance;
    }

    element_if* get_main_window();
    void set_main_window(element_if* new_main_window);

private:
    application_t() = default;

    android_app* _java_app;
    std::unique_ptr<element_if> _main_window;
};

#endif /* ZEPTOTEST_APPLICATION_HPP_ */
