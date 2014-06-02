#ifndef ZEPTOTEST_TEXTURE_SHADER_HPP
#define ZEPTOTEST_TEXTURE_SHADER_HPP

#include "defs.hpp"
#include "shader_program_if.hpp"
#include "subsystems/resource_manager.hpp"

#include <GLES2/gl2.h>

class texture_shader_t : public shader_program_if
{
public:
	texture_shader_t();

	void set_texture(texture_t* tex);

private:
	GLint _u_texture;
	GLuint _vbo;

	texture_t* _cur_tex;

	virtual void do_release() override;
	virtual void draw() override;
	virtual void bind_attribute_locations() override;

	enum
	{
		ATTRIBUTE_VERTICES
	};
};

#endif // ZEPTOTEST_TEXTURE_SHADER_HPP
