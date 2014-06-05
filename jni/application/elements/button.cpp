#include "button.hpp"

button_t::button_t(const std::string &texture_name, const vec2 &pos, const vec2 &size)
: element_if(pos, size)
, _is_set(false)
{
    set_background(texture_name);
}

void button_t::set_on_click(std::function<void()> const& func)
{
    _on_click_func = func;
    _is_set = true;
}

void button_t::on_touch(touch_t &touch)
{
    if(_is_set)
    {
        auto on_end = [this](touch_t* touch) {
        	this->_on_click_func();
        };

        touch.set_on_move(on_end);
    }
}
