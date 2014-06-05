/*
 * base_element.hpp
 *
 *  Created on: May 26, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_BASE_ELEMENT_HPP_
#define ZEPTOTEST_BASE_ELEMENT_HPP_

#include "defs.hpp"

#include "utils/vecmath.hpp"
#include "utils/geometry.hpp"
#include "touch.hpp"

#include <list>
#include <memory>
#include <cmath>
#include <string>

class element_if : public std::enable_shared_from_this<element_if>
{
public:
    element_if(vec2 const& position, vec2 const& size);

    virtual ~element_if() = 0;

    virtual void draw(mat3 const&)
    {
        //do nothing by default
    }

    void do_draw(mat3 const& vp_matrix);

    template <typename T>
    T* add_child(T* child);

    void remove_child(element_if* child);

    void set_angle(float angle);
    float get_angle() const;

    void set_position(vec2 pos);

    vec2 const& get_position() const;

    void set_size(vec2 const& size);
    vec2 get_size() const;

    //delegate to children by default
    virtual void on_touch(touch_t& touch)
    {
        touch.add_transform_matrix(parent_to_child_matrix());

        auto pos = touch.get_begin();
        auto child = get_child(pos);
        if(child) child->on_touch(touch);
    }

    mat3 parent_to_child_matrix();
    mat3 child_to_parent_matrix();

    rect_t get_aabb() const;

    void destroy();

    void set_parent(element_if* parent);
    element_if* get_parent();

    void set_collision_group(uint8_t group);
    uint8_t get_collision_group() const;

    void set_background(std::string const& file_name);

protected:
    std::list<std::shared_ptr<element_if> > _children;
	vec2 _position;
	vec2 _size;
	float _angle;
	element_if* _parent;

    uint8_t _collision_group;

    mat3 _child_to_parent_mat;
    mat3 _parent_to_child_mat;
    bool _matrixes_changed;

    std::string _background;

    element_if* get_child(vec2 const& coord);
    void calculate_transform_matrixes();
    void draw_background(const mat3 &mvp_matrix);
};

template <typename T>
T* element_if::add_child(T* child)
{
    _children.push_back(std::shared_ptr<element_if>(child));
    child->set_parent(this);
    return child;
}



#endif /* ZEPTOTEST_BASE_ELEMENT_HPP_ */
