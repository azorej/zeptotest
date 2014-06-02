/*
 * sprite.hpp
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_SPRITE_HPP_
#define ZEPTOTEST_SPRITE_HPP_

#include "defs.hpp"
#include "elements/base_element.hpp"

class sprite_t: public base_element_t
{
public:
	sprite_t(vec2 const& position, vec2 const& size);

	virtual void draw(mat3 const& mvp_matrix) override;
};

#endif /* SPRITE_HPP_ */
