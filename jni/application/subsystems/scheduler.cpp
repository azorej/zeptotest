/*
 * sheduler.cpp
 *
 *  Created on: May 25, 2014
 *      Author: azorej
 */

#include <subsystems/scheduler.hpp>

#include <algorithm>

namespace subsystems
{

    void scheduler_t::start()
    {
    }

    void scheduler_t::stop()
    {
        clear();
    }

    void scheduler_t::tick()
    {
        auto it = _tasks.begin(), end = _tasks.end();
        while (it != end)
        {
            //is task alive
            if (auto task_locked = it->lock())
            {
                auto cur_time = std::chrono::steady_clock::now();
                //is it time to start task
                if (cur_time > task_locked->next_launch)
                {
                    task_locked->task(
                            std::chrono::duration_cast<std::chrono::milliseconds>(cur_time - task_locked->last_launch));
                    //we need to repeat task
                    if (task_locked->is_repeat)
                    {
                        task_locked->last_launch = cur_time;
                        task_locked->next_launch = cur_time + task_locked->interval;
                        ++it;
                    }
                    else
                    {
                        task_locked->is_ended = true;
                        task_locked->last_launch = cur_time;
                        it = _tasks.erase(it);
                    }
                }
                else
                    ++it;
            }
            else //it is dead task
            {
                it = _tasks.erase(it);
            }
        }
    }

    scheduled_task_t scheduler_t::schedule_repeat(const task_t& task, std::chrono::milliseconds interval)
    {
        auto task_info = std::make_shared<detail::task_info_t>(task, interval, true);
        _tasks.push_back(task_info);

        return scheduled_task_t(task_info);
    }

    scheduled_task_t scheduler_t::schedule_once(task_t const& task, std::chrono::milliseconds wait_for)
    {
        auto task_info = std::make_shared<detail::task_info_t>(task, wait_for, false);
        _tasks.push_back(task_info);

        return scheduled_task_t(task_info);
    }

    void scheduler_t::clear()
    {
        for (auto& task : _tasks)
        {
            if (auto task_locked = task.lock())
            {
                task_locked->is_ended = true;
            }
        }

        _tasks.clear();
    }

} /* namespace subsystems */
