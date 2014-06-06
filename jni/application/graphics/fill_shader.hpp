#ifndef ZEPTOTEST_FILL_SHADER_HPP
#define ZEPTOTEST_FILL_SHADER_HPP

#include "shader_program_if.hpp"

#include "defs.hpp"
#include "utils/vecmath.hpp"
#include "utils/geometry.hpp"

#include <GLES2/gl2.h>

#include <vector>

class fill_shader_t: public shader_program_if
{
public:
    fill_shader_t();

    void set_color(vec4 const& color);

    void set_vertices(std::vector<GLfloat> const& vertices);
    void set_vertices(GLfloat* array, size_t size);

private:
    virtual void draw() override;
    virtual void bind_attribute_location();
    virtual void do_release() override;

    enum
    {
        ATTRIBUTE_VERTEX
    };

    GLint _u_color;
    vec4 _color;
    GLuint _vbo;
    size_t _indices_count;
};

#endif // SHADER_PROGRAM_H