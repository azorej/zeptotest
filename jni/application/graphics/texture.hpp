/*
 * texture.hpp
 *
 *  Created on: Jun 6, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_TEXTURE_HPP_
#define ZEPTOTEST_TEXTURE_HPP_

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/opengl.hpp"

#include "android_native_app_glue.h"
#include <GLES2/gl2.h>

class aasset_buffer_t
{
public:
    explicit aasset_buffer_t(AAsset* asset);

    BYTE* data();

    uint32_t size() const;

    bool seek(uint32_t pos);

    bool read_next(BYTE* dest, uint32_t count);

private:
    AAsset* _asset;
    uint32_t _size;
    BYTE* _data;
    uint32_t _position;
};

class texture_t
{
public:
    explicit texture_t(aasset_buffer_t& buffer);

    void activate();
    void deactivate();

private:
    GLuint _id;
    bool _alpha;
};

#endif /* ZEPTOTEST_TEXTURE_HPP_ */
