/*
 * scheduled_task.hpp
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_SCHEDULED_TASK_HPP_
#define ZEPTOTEST_SCHEDULED_TASK_HPP_

#include "defs.hpp"

#include <functional>
#include <memory>
#include <chrono>

typedef std::function<void(std::chrono::milliseconds)> task_t;

namespace detail
{
    struct task_info_t
    {
        task_info_t(task_t const& task, const std::chrono::milliseconds interval, const bool is_repeat)
                : task(task),
                  interval(interval),
                  is_repeat(is_repeat),
                  is_ended(false)
        {
            last_launch = std::chrono::steady_clock::now();
            next_launch = last_launch + interval;
        }

        const task_t task;
        std::chrono::milliseconds interval;
        const bool is_repeat;
        bool is_ended;
        std::chrono::time_point<std::chrono::steady_clock> next_launch;
        std::chrono::time_point<std::chrono::steady_clock> last_launch;
    };
}

/**
 *Copyable task guard
 */
class scheduled_task_t
{
public:
    scheduled_task_t() = default;

    explicit scheduled_task_t(std::shared_ptr<detail::task_info_t> task_info)
            : _task_info(task_info)
    {
    }

    bool is_ended() const
    {
        return _task_info->is_ended;
    }

    void destroy()
    {
        _task_info.reset();
    }

    std::chrono::milliseconds get_interval()
    {
        return _task_info->interval;
    }

    void set_interval(std::chrono::milliseconds interval)
    {
        _task_info->interval = interval;
    }

private:
    std::shared_ptr<detail::task_info_t> _task_info;
};

#endif /* SCHEDULED_TASK_HPP_ */
