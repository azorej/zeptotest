/*
 * inputcontroller.hpp
 *
 *  Created on: Jun 2, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_INPUT_CONTROLLER_HPP_
#define ZEPTOTEST_INPUT_CONTROLLER_HPP_

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/subsystem.hpp"
#include "elements/base_element.hpp"
#include "application.hpp"

#include <list>
#include <chrono>

struct touch_t
{
	int32_t id;
	float begin;
	float end;
};

namespace subsystems {

class input_controller_t  : private noncopyable_t, public subsystem_t
{
public:
	virtual void start() override;
	virtual void stop() override;

	void on_touch(raw_touch_arr raw_touches);

private:
	std::list<touch_t> _touches;
};

} /* namespace subsystems */

#endif /* ZEPTOTEST_INPUT_CONTROLLER_HPP_ */
