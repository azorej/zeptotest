/*
 * graphic.cpp
 *
 *  Created on: May 25, 2014
 *      Author: azorej
 */

#include "subsystems/graphic.hpp"

#include "graphics/texture_shader.hpp"
#include "graphics/fill_shader.hpp"
#include "application.hpp"
#include "subsystems/scheduler.hpp"
#include "opengl_utils.hpp"
#include "utils/vecmath.hpp"

#include <GLES2/gl2.h>

#include <android/native_window.h>

namespace subsystems {

	graphic_t::graphic_t(android_app* java_app)
	: _java_app(java_app)
	, _display(EGL_NO_DISPLAY)
    , _context(EGL_NO_CONTEXT)
    , _surface(EGL_NO_SURFACE)
	{
	}

	void graphic_t::start()
	{
		init_context();

		load_shaders();

		//schedule redraw
		auto interval = std::chrono::milliseconds((long)((1.f/FPS)*1000.f));
		_redraw_task = application_t::singleton().subsystem<subsystems::scheduler_t>().schedule_repeat(std::bind(&subsystems::graphic_t::draw, this), interval);
	}

	void graphic_t::stop()
	{
		_shaders.clear();

		release_context();
	}

	void graphic_t::draw()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		auto* main_window = application_t::singleton().get_main_window();
		my_assert(main_window != nullptr);
		main_window->do_draw(mat3::scaling(2/COORDINATE_FACTOR, 2/COORDINATE_FACTOR));

		eglSwapBuffers(_display, _surface);

		CHECK_GL_ERRORS("");
	}

	void graphic_t::init_context()
	{
		// initialize OpenGL ES and EGL

		/*
		 * Here specify the attributes of the desired configuration.
		 * Below, we select an EGLConfig with at least 8 bits per color
		 * component compatible with on-screen windows
		 */
		const EGLint config_attribs[] = {
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_BLUE_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_RED_SIZE, 8,
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_NONE
		};

		const EGLint context_attribs[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};

		EGLint dummy, format;
		EGLint numConfigs;
		EGLConfig config;

		_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(_display, 0, 0);

		/* Here, the application chooses the configuration it desires. In this
		 * sample, we have a very simplified selection process, where we pick
		 * the first EGLConfig that matches our criteria */
		eglChooseConfig(_display, config_attribs, &config, 1, &numConfigs);

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		 * As soon as we picked a EGLConfig, we can safely reconfigure the
		 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
		eglGetConfigAttrib(_display, config, EGL_NATIVE_VISUAL_ID, &format);

		ANativeWindow_setBuffersGeometry(_java_app->window, 0, 0, format);

		_surface = eglCreateWindowSurface(_display, config, _java_app->window, NULL);
		_context = eglCreateContext(_display, config, NULL, context_attribs);

		my_assert((eglMakeCurrent(_display, _surface, _surface, _context) != EGL_FALSE) && "Unable to eglMakeCurrent");

		eglQuerySurface(_display, _surface, EGL_WIDTH, &_width);
		eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

		// Initialize GL state.
		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		CHECK_GL_ERRORS("");
	}

	void graphic_t::release_context()
	{
		/**
		 * Tear down the EGL context currently associated with the display.
		 */

		if (_display != EGL_NO_DISPLAY) {
			eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if (_context != EGL_NO_CONTEXT) {
				eglDestroyContext(_display, _context);
			}
			if (_surface != EGL_NO_SURFACE) {
				eglDestroySurface(_display, _surface);
			}
			eglTerminate(_display);
		}

		_display = EGL_NO_DISPLAY;
		_context = EGL_NO_CONTEXT;
		_surface = EGL_NO_SURFACE;

		CHECK_GL_ERRORS("");
	}

	void graphic_t::load_shaders()
	{
		_shaders[&typeid(fill_shader_t)] = std::make_shared<fill_shader_t>();
		_shaders[&typeid(texture_shader_t)] = std::make_shared<texture_shader_t>();
	}

} /* namespace subsystems */
