#include "touch.hpp"
#include "application.hpp"
#include "subsystems/graphic.hpp"

touch_t::touch_t(const vec2 &begin)
    : _transform_mat(mat3::identity())
    , _begin(begin)
    , _end(begin)
{
    //перейдем в пространство координат [-50;50][-50;50] (стандартное для всех элементов)
    //оси требуется нормализовать, ось y необходимо обратить и центр координат передвинуть в центр экрана
    auto& graphic = application_t::singleton().subsystem<subsystems::graphic_t>();
    auto w = graphic.get_screen_width();
    auto h = graphic.get_screen_height();

    _transform_mat = mat3::translation(-50.f, 50.f) * mat3::scaling(100.f/w, -100.f/h);
}

void touch_t::set_move(vec2 const& new_pos)
{
    _move = new_pos - _end;
    _end = new_pos;
}

vec2 touch_t::get_begin() const
{
    return vec2(_transform_mat * vec3(_begin.x, _begin.y, 1));
}
vec2 touch_t::get_end() const
{
    return vec2(_transform_mat * vec3(_end.x, _end.y, 1));
}
vec2 touch_t::get_move() const
{
    return vec2(_transform_mat * vec3(_move.x, _move.y, 1));
}

void touch_t::set_on_move(touch_callback_t const& func)
{
    _on_move_func.reset(new touch_callback_t(func));
}
void touch_t::set_on_end(touch_callback_t const& func)
{
    _on_end_func.reset(new touch_callback_t(func));
}
void touch_t::set_on_cancel(touch_callback_t const& func)
{
    _on_cancel_func.reset(new touch_callback_t(func));
}
void touch_t::on_move()
{
    if(_on_move_func) (*_on_move_func)(this);
}
void touch_t::on_end()
{
    if(_on_end_func) (*_on_end_func)(this);
}
void touch_t::on_cancel()
{
    if(_on_cancel_func) (*_on_cancel_func)(this);
}

void touch_t::add_transform_matrix(mat3 const& mat)
{
    _transform_mat = mat * _transform_mat;
}
