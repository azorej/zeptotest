/*
 * shader_program_if.hpp
 *
 *  Created on: May 28, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_SHADER_PROGRAM_IF_HPP_
#define ZEPTOTEST_SHADER_PROGRAM_IF_HPP_

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/vecmath.hpp"

#include <GLES2/gl2.h>

class shader_program_if: private noncopyable_t
{
public:
    shader_program_if();
    virtual ~shader_program_if() = 0;

    void release();
    virtual void do_release() = 0;

    void activate(mat3 const& mvp_matrix);

protected:
    GLuint& vsh()
    {
        return _vsh;
    }
    GLuint& fsh()
    {
        return _fsh;
    }
    GLuint& program()
    {
        return _program;
    }

    void compile_shader(const GLenum type, const char* source);
    void link_program();

    virtual void draw() = 0;
    virtual void bind_attribute_locations()
    {
    }

private:
    GLuint _vsh;
    GLuint _fsh;
    GLuint _program;
    GLint _mvp_matrix;
};

#endif /* SHADER_PROGRAM_IF_HPP_ */
