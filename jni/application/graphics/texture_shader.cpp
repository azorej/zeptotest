#include "texture_shader.hpp"

#include "opengl_utils.hpp"

static const char* vsh_source = ""
								"uniform mat3 u_mvpMatrix;"
								"attribute vec2 a_position;"
								""
								"varying vec2 v_texCoordinate;"
								""
								"void main()"
								"{"
								"	mat3 uvMatrix = mat3( \
								    0.01, 0.0 , 0.0, \
								    0.0 , 0.01, 0.0, \
								    0.5 , 0.5 , 1.0  \
								    );"
									""
									"v_texCoordinate = vec2(uvMatrix * vec3(a_position, 1));"
									""
									"gl_Position = vec4(u_mvpMatrix * vec3(a_position, 1), 1);"
								"}";

static const char* fsh_source = ""
								"precision mediump float;"
								""
								"uniform sampler2D u_texture;"
								""
								"varying vec2 v_texCoordinate;"
								""
								"void main()"
								"{"
								"  gl_FragColor = texture2D(u_texture, v_texCoordinate);"
								"}";



texture_shader_t::texture_shader_t()
: _cur_tex(nullptr)
, _vbo(0)
{
	try
	{
		compile_shader(GL_VERTEX_SHADER, vsh_source);
		compile_shader(GL_FRAGMENT_SHADER, fsh_source);

		link_program();

		_u_texture = glGetUniformLocation(program(), "u_texture");
		check_gl_errors_and_throw("Couldn't get texture uniform location");

		glGenBuffers(1, &_vbo);

		float vertices[12] = {-50.f,  50.f,
							  -50.f, -50.f,
							   50.f, -50.f,
							   50.f,  50.f,
							  -50.f,  50.f,
							   50.f, -50.f
							  };

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(vertices)*sizeof(GLfloat)), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		check_gl_errors_and_throw("Couldn't init vertices buffer object");

	} catch(std::runtime_error const& e)
	{
		LOG_FATAL_ERROR("Sprite shader construction error: %s", e.what());
	}
}

void texture_shader_t::bind_attribute_locations()
{
	glBindAttribLocation(program(), ATTRIBUTE_VERTICES, "a_position");
	check_gl_errors_and_throw("Couldn't bind attribute vertices object.");
}

void texture_shader_t::set_texture(texture_t* tex)
{
	my_assert(tex);

	_cur_tex = tex;
}

void texture_shader_t::do_release()
{
	if(_vbo)
	{
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	CHECK_GL_ERRORS("");
}

void texture_shader_t::draw()
{
	my_assert(_cur_tex && "Current texture is null");

	_cur_tex->activate();

	glUniform1i(_u_texture, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(ATTRIBUTE_VERTICES, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ATTRIBUTE_VERTICES);

	glDrawArrays(GL_TRIANGLES, (GLint)0,  (GLsizei)12);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_cur_tex->deactivate();

	CHECK_GL_ERRORS("");
}
