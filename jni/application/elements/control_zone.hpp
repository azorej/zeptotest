#ifndef ZEPTOTEST_CONTROL_ZONEL_HPP
#define ZEPTOTEST_CONTROL_ZONEL_HPP

#include "defs.hpp"
#include "physic_element.hpp"

class control_zone_t: public element_if
{
public:
    control_zone_t(vec2 const& pos, vec2 const& size, physic_element_if* controlled_element)
            : element_if(pos, size),
              _controlled_element(controlled_element)
    {
    }

    void on_touch(touch_t &touch) override;

private:
    physic_element_if* _controlled_element;
};

#endif // ZEPTOTEST_CONTROL_ZONEL_HPP