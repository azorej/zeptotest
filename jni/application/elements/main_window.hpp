/*
 * main_window.hpp
 *
 *  Created on: Jun 5, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_MAIN_WINDOW_HPP_
#define ZEPTOTEST_MAIN_WINDOW_HPP_

#include "defs.hpp"
#include "elements/element_if.hpp"

class main_window_if: public element_if
{
public:
    main_window_if();

    virtual ~main_window_if() = 0;
};

inline main_window_if::~main_window_if()
{

}

#endif /* ZEPTOTEST_MAIN_WINDOW_HPP_ */
