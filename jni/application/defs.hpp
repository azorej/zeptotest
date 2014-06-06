/*
 * defs.hpp
 *
 *  Created on: May 23, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_DEFS_HPP_
#define ZEPTOTEST_DEFS_HPP_

#include <jni.h>
#include <android/log.h>

#include <typeinfo>
#include <memory>

//fps instruction for graphic subsystem
#define FPS 30
//coordinate factor
#define COORDINATE_FACTOR 100.f

#define LOG_INFO(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOG_ERROR(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-activity", __VA_ARGS__))
#define LOG_FATAL_ERROR(...) ((void)__android_log_print(ANDROID_LOG_FATAL, "native-activity", __VA_ARGS__))

#ifdef NDEBUG
# define	my_assert(e)	((void)0)
#else
# define	my_assert(e)	((e) ? (void)0 : LOG_FATAL_ERROR("Assert on %s : %i. Function: %s. Message: %s", __FILE__, __LINE__, __func__, #e))
#endif

struct type_info_comparator
{
    bool operator ()(const std::type_info* a, const std::type_info* b) const
    {
        return a->before(*b);
    }
};

//guards for auto-cleaning resoures and etc.
template<typename T, typename Deleter>
std::unique_ptr<T, Deleter> make_guard(T* ptr, Deleter&& d)
{
    return std::unique_ptr<T, Deleter>(ptr, d);
}

#define MAKE_GUARD(ptr, deleter) auto ptr##_guard = make_guard(ptr, deleter)

typedef uint8_t BYTE;

#endif /* ZEPTOTEST_DEFS_HPP_ */
