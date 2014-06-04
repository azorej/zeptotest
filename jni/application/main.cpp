#include "defs.hpp"

#include "application.hpp"

#include <android_native_app_glue.h>
#include <android/native_window.h>

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* and_app, AInputEvent* event)
{
	application_t* app = (application_t*)and_app->userData;

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		app->on_touch(event);

        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* and_app, int32_t cmd) {
    application_t* app = (application_t*)and_app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            //engine->app->savedState = malloc(sizeof(struct saved_state));
            //*((struct saved_state*)engine->app->savedState) = engine->state;
            //engine->app->savedStateSize = sizeof(struct saved_state);

            app->on_save_state();
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (and_app->window != NULL) {
            	app->on_shown();
                //engine_draw_frame(engine);

                //application_t::singleton().on_shown();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
        	app->on_close();
            break;
        case APP_CMD_GAINED_FOCUS:
            // our app gains focus
            break;
        case APP_CMD_LOST_FOCUS:
            // our app loses focus
            // stop animating.
            //engine->animating = 0;
            //engine_draw_frame(engine);
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* java_app) {

    // Make sure glue isn't stripped.
    app_dummy();

    application_t::singleton().init(java_app);

    java_app->userData = (void*)&application_t::singleton();
    java_app->onAppCmd = engine_handle_cmd;
    java_app->onInputEvent = engine_handle_input;

    if (java_app->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
    }

    // loop waiting for stuff to do.
    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(0 , NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(java_app, source);
            }

            // Check if we are exiting.
            if (java_app->destroyRequested != 0) {
                //engine_term_display(&engine);
            	application_t::singleton().on_close();
                return;
            }
        }

        application_t::singleton().tick();
    }
}



