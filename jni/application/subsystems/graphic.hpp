/*
 * graphic.hpp
 *
 *  Created on: May 25, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_SUBSYSTEM_GRAPHIC_HPP_
#define ZEPTOTEST_SUBSYSTEM_GRAPHIC_HPP_

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/subsystem.hpp"
#include "scheduled_task.hpp"
#include "graphics/shader_program_if.hpp"

#include "android_native_app_glue.h"

#include <EGL/egl.h>

#include <map>
#include <memory>

namespace subsystems {

class graphic_t : private noncopyable_t, public subsystem_t
{
public:
	explicit graphic_t(android_app* java_app);
	virtual void start() override;
	virtual void stop() override;

	void draw();

	template <typename ShaderType>
    ShaderType& get_shader();

    float get_screen_ratio() const
    {
    	return ((float)_width)/(float)_height;
    }

    size_t get_screen_width() const
    {
    	return _width;
    }

    size_t get_screen_height() const
	{
		return _height;
	}
private:
	android_app* _java_app;
	EGLint _width, _height;
	EGLDisplay _display;
	EGLSurface _surface;
	EGLContext _context;

	//task to redraw all game graphics
	scheduled_task_t _redraw_task;

    std::map<const std::type_info*, std::shared_ptr<shader_program_if>, type_info_comparator> _shaders;

    void init_context();
    void release_context();
    void load_shaders();
};

template <typename ShaderType>
ShaderType& graphic_t::get_shader()
{
	auto it = _shaders.find(&typeid(ShaderType));
	my_assert(it != _shaders.end());

	return static_cast<ShaderType&>(*(it->second));
}

} /* namespace subsystems */

#endif /* GRAPHIC_HPP_ */
