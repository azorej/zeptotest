#ifndef __MASTER_HPP__
#define __MASTER_HPP__

#include "noncopyable.hpp"

#include <vector>
#include <cassert>
#include <memory>

/** @class subsystem_t
 * @brief Interface for managed subsystem.
 */
class subsystem_t
{
public:
    virtual void start()
    {
    }
    virtual void stop()
    {
    }

    virtual ~subsystem_t() = 0;
};

/** @class master_t
 * @brief Subsystems manager.
 * @note
 * Only one instance of this class can be created
 * and only once during program execution.
 */
class master_t: private noncopyable_t
{
public:
    template<typename SubsystemType, typename ... Args>
    void add_managed_subsystem(Args ...args);

    template<typename SubsystemType, typename ... Args>
    void add_unmanaged_subsystem(Args ...args);

    template<typename SubsystemType>
    void add_external_subsystem(SubsystemType *raw_pointer);

    template<typename SubsystemType>
    static SubsystemType & subsystem();

    void start();
    void stop();

    master_t();

private:
    std::vector<std::unique_ptr<subsystem_t>> m_subsystems;
};

// Implementation

inline subsystem_t::~subsystem_t()
{
}

inline void master_t::start()
{
    for (auto &subsystem : m_subsystems)
    {
        subsystem->start();
    }
}

inline void master_t::stop()
{
    for (auto &subsystem : m_subsystems)
    {
        subsystem->stop();
    }
}

inline master_t::master_t()
{
    static bool was_created = false;
    assert(!was_created && "Object of type master_t can be created only once!");
    was_created = true;
}

namespace internal
{
    template<typename SubsystemType>
    inline SubsystemType ** subsystem_instance()
    {
        static SubsystemType *instance = 0;
        return &instance;
    }

    template<typename SubsystemType>
    struct unmanaged_holder_t: public subsystem_t
    {
        template<typename ... Args>
        inline unmanaged_holder_t(Args ...args)
                : holder(args...)
        {
        }
        SubsystemType holder;
    };
}

template<typename SubsystemType, typename ... Args>
inline void master_t::add_managed_subsystem(Args ...args)
{
    SubsystemType **instance = internal::subsystem_instance<SubsystemType>();
    assert(*instance == 0 && "Instance for this subsystem was already added!");

    *instance = new SubsystemType(args...);
    m_subsystems.push_back(std::unique_ptr<subsystem_t>(*instance));
}

template<typename SubsystemType, typename ... Args>
inline void master_t::add_unmanaged_subsystem(Args ...args)
{
    SubsystemType **instance = internal::subsystem_instance<SubsystemType>();
    assert(*instance == 0 && "Instance for this subsystem was already added!");

    internal::unmanaged_holder_t<SubsystemType> *unmanaged_holder = new internal::unmanaged_holder_t<SubsystemType>(
            args...);
    *instance = &(unmanaged_holder->holder);
    m_subsystems.push_back(std::unique_ptr<subsystem_t>(unmanaged_holder));
}

template<typename SubsystemType>
inline void master_t::add_external_subsystem(SubsystemType *raw_pointer)
{
    SubsystemType **instance = internal::subsystem_instance<SubsystemType>();
    assert(*instance == 0 && "Instance for this subsystem was already added!");

    *instance = raw_pointer;
}

template<typename SubsystemType>
inline SubsystemType & master_t::subsystem()
{
    SubsystemType **instance = internal::subsystem_instance<SubsystemType>();
    assert(*instance != 0 && "Instance for this  subsystem was never added!");

    return **instance;
}

#endif
