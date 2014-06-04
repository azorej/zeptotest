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
#include "utils/vecmath.hpp"
#include "touch.hpp"

#include "android/input.h"

#include <map>

namespace subsystems {

class input_controller_t  : private noncopyable_t, public subsystem_t
{
public:
	input_controller_t();

	virtual void start() override;
	virtual void stop() override;

	void on_touch(AInputEvent* event);

private:
	std::map<int32_t, touch_t> _touches;

	bool _is_enabled;
};

} /* namespace subsystems */

#endif /* ZEPTOTEST_INPUT_CONTROLLER_HPP_ */
