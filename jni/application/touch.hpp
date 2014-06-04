#ifndef ZEPTOTEST_TOUCH_HPP
#define ZEPTOTEST_TOUCH_HPP

#include "defs.hpp"
#include "utils/vecmath.hpp"

#include <functional>
#include <memory>

class touch_t
{
public:
    typedef std::function<void (touch_t* )> touch_callback_t;

    explicit touch_t(vec2 const& begin);

    void set_move(vec2 const& new_pos);

    vec2 get_begin() const;
    vec2 get_end() const;
    vec2 get_move() const;

    void set_on_move(touch_callback_t const& func);
    void set_on_end(touch_callback_t const& func);
    void set_on_cancel(touch_callback_t const& func);

    void on_move();
    void on_end();
    void on_cancel();

    void add_transform_matrix(mat3 const& mat);

private:
    std::shared_ptr<touch_callback_t> _on_move_func;
    std::shared_ptr<touch_callback_t> _on_end_func;
    std::shared_ptr<touch_callback_t> _on_cancel_func;

    vec2 _begin;
    vec2 _end;
    vec2 _move;

    mat3 _transform_mat;
};

#endif // ZEPTOTEST_TOUCH_HPP
