#ifndef ZEPTOTEST_BUTTON_HPP
#define ZEPTOTEST_BUTTON_HPP

#include "defs.hpp"
#include "elements/element_if.hpp"

#include <functional>
#include <memory>

class button_t: public element_if
{
public:
    button_t(std::string const& texture_name, vec2 const& pos, vec2 const& size);

    void set_on_click(std::function<void()> const& func);

    void on_touch(touch_t &touch) override;

private:
    std::function<void()> _on_click_func;
    bool _is_set;
};

#endif // ZEPTOTEST_BUTTON_HPP