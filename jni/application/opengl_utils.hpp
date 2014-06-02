/*
 * opengl_utils.hpp
 *
 *  Created on: Jun 1, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_OPENGL_UTILS_HPP_
#define ZEPTOTEST_OPENGL_UTILS_HPP_

#include "defs.hpp"

#include "GLES2/gl2.h"

#include <stdexcept>

#define CLEAR_GL_ERRORS() while(glGetError() != GL_NO_ERROR) { }

#define CHECK_GL_ERRORS(msg) \
	while(glGetError() != GL_NO_ERROR) \
	{ \
		LOG_FATAL_ERROR("OpenGL error: %s", msg); \
		my_assert(false); \
	}

inline bool check_gl_errors()
{
	bool is_error = false;
	while(glGetError() != GL_NO_ERROR)
	{
		is_error = true;
	}

	return is_error;
}

inline void check_gl_errors_and_throw(const char* msg)
{
	if(check_gl_errors()) throw std::runtime_error(msg);
}


#endif /* ZEPTOTEST_OPENGL_UTILS_HPP_ */
