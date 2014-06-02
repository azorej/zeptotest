#include "fill_shader.hpp"

#include "opengl_utils.hpp"

static const char* vsh_source = ""
								"uniform mat3 u_mvpMatrix;"
								"uniform vec4 u_color;"
								"attribute vec2 a_position;"
								""
								"varying vec4 v_color;"
								"void main()"
								"{"
									"gl_Position = vec4(u_mvpMatrix * vec3(a_position, 1), 1);"
									"v_color = u_color;"
								"}";

static const char* fsh_source = ""
								"precision mediump float;"
								""
								"varying vec4 v_color;"
								"void main()"
								"{"
								"	gl_FragColor =  v_color;"
								"}";



fill_shader_t::fill_shader_t()
: _color(1, 0, 0, 1) //red by default
, _vbo(0)
, _indices_count(0)
{
	try
	{
		compile_shader(GL_VERTEX_SHADER, vsh_source);
		compile_shader(GL_FRAGMENT_SHADER, fsh_source);

		link_program();

		_u_color = glGetUniformLocation(program(), "u_color");

		glGenBuffers(1, &_vbo);

		CHECK_GL_ERRORS("");

	} catch(std::runtime_error const& e)
	{
		LOG_FATAL_ERROR("Fill shader construction error: %s", e.what());
	}
}

void fill_shader_t::draw()
{
	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	// Pass the vertex data
	glVertexAttribPointer(ATTRIBUTE_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ATTRIBUTE_VERTEX);

	glUniform4fv(_u_color, 1, (GLfloat*)&_color);

	glDrawArrays(GL_TRIANGLES, (GLint)0,  (GLsizei)_indices_count);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	CHECK_GL_ERRORS("");
}

void fill_shader_t::set_color(const vec4& color)
{
	_color = color;
}

void fill_shader_t::set_vertices(const std::vector<GLfloat>& vertices)
{
	_indices_count = vertices.size();
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertices.size()*sizeof(GLfloat)), vertices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	CHECK_GL_ERRORS("");
}


void fill_shader_t::bind_attribute_location()
{
	glBindAttribLocation(program(), ATTRIBUTE_VERTEX, "a_position");
	check_gl_errors_and_throw("Couldn't bind attribute vertex object.");
}

void fill_shader_t::do_release()
{
	if(_vbo)
	{
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	CHECK_GL_ERRORS("");
}
