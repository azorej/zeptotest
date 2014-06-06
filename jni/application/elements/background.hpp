/*
 * background.h
 *
 *  Created on: Jun 5, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_BACKGROUND_HPP_
#define ZEPTOTEST_BACKGROUND_HPP_

#include "defs.hpp"
#include "elements/element_if.hpp"

class background_t: public element_if
{
public:
    explicit background_t(std::string const& texture_name);

    void draw(mat3 const& mvp_matrix) override;

private:
    std::string _texture;
};

#endif /* ZEPTOTEST_BACKGROUND_HPP_ */
