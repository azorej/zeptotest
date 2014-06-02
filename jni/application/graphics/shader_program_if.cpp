#include "shader_program_if.hpp"

#include "opengl_utils.hpp"

#include <GLES2/gl2.h>

#include <cstring>

shader_program_if::shader_program_if()
: _vsh(0)
, _fsh(0)
, _program(0)
{
}

shader_program_if::~shader_program_if()
{
	release();
}

void shader_program_if::release()
{
	do_release();

	if (_vsh) {
		glDeleteShader(_vsh);
		_vsh = 0;
	}
	if (_fsh) {
		glDeleteShader(_fsh);
		_fsh = 0;
	}
	if (_program) {
		glDeleteProgram(_program);
		_program = 0;
	}

	CHECK_GL_ERRORS("");
}

void shader_program_if::compile_shader(const GLenum type, const char* source)
{
    auto id = glCreateShader(type);
    my_assert(id != 0);

    GLint size = std::strlen(source);
    glShaderSource(id, (GLsizei)1, (const GLchar**)&source, &size);
    glCompileShader(id);

#ifndef NDEBUG
    GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if(status == 0)
	{
    	GLint log_length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);

		/* The maxLength includes the NULL character */
		char* log = new char[log_length];
		MAKE_GUARD(log, [](char* ptr){delete[] ptr;});

		glGetShaderInfoLog(id, log_length, &log_length, log);

		throw std::runtime_error(log);
	}
#endif

    if(type == GL_VERTEX_SHADER) _vsh = id;
    else _fsh = id;

    if(check_gl_errors()) throw std::runtime_error("Shader compile error");
}

void shader_program_if::activate(const mat3& mvp_matrix)
{
	glUseProgram(_program);

	glUniformMatrix3fv(_mvp_matrix, 1, GL_FALSE, (GLfloat*)mvp_matrix.ptr());

	draw();

	CHECK_GL_ERRORS("");
}

void shader_program_if::link_program()
{
	_program = glCreateProgram();
	my_assert(_program != 0);

	glAttachShader(_program, _vsh);
	glAttachShader(_program, _fsh);

	bind_attribute_locations();

	glLinkProgram(_program);

#ifndef NDEBUG
	GLint status;
	glGetProgramiv(_program, GL_LINK_STATUS, &status);

    if(status == 0)
	{
    	GLint log_length;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &log_length);

		/* The maxLength includes the NULL character */
		char* log = new char[log_length];
		MAKE_GUARD(log, [](char* ptr){delete[] ptr;});

		glGetProgramInfoLog(_program, log_length, &log_length, log);

		throw std::runtime_error(log);
	}
#endif

    _mvp_matrix = glGetUniformLocation(_program, "u_mvpMatrix");
    check_gl_errors_and_throw("Couldn't get mvpMatrix uniform location.");
}
