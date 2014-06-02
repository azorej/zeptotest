/*
 * sheduler.h
 *
 *  Created on: May 25, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_SHEDULER_H_
#define ZEPTOTEST_SHEDULER_H_

#include "defs.hpp"
#include "utils/noncopyable.hpp"
#include "utils/subsystem.hpp"
#include "scheduled_task.hpp"

#include <chrono>
#include <list>
#include <memory>

namespace subsystems {

class scheduler_t : private noncopyable_t, public subsystem_t
{
public:
	virtual void start() override;
	virtual void stop() override;

	void tick();

    scheduled_task_t schedule_repeat(task_t const& task, std::chrono::milliseconds interval);
    scheduled_task_t schedule_once(task_t const& task, std::chrono::milliseconds wait_for);

    void clear();

private:
    std::list<std::weak_ptr<detail::task_info_t> > _tasks;
};

} /* namespace subsystems */

#endif /* SHEDULER_H_ */
