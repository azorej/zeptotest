#include "element_if.hpp"

#include "application.hpp"
#include "subsystems/graphic.hpp"
#include "subsystems/resource_manager.hpp"
#include "graphics/texture_shader.hpp"

element_if::element_if(vec2 const& position, vec2 const& size)
        : _position(position),
          _size(size),
          _angle(0),
          _matrixes_changed(true),
          _parent(nullptr),
          _collision_group(0)
{
}

element_if::~element_if()
{
    for (auto& child : _children)
    {
        child->set_parent(0);
    }
}

void element_if::do_draw(mat3 const& vp_matrix)
{
    mat3 new_vp_matrix = vp_matrix * child_to_parent_matrix();

    draw_background(new_vp_matrix);
    draw(new_vp_matrix);
    for (auto& child_ptr : _children)
    {
        child_ptr->do_draw(new_vp_matrix);
    }
}

void element_if::remove_child(element_if* child)
{
    for (auto& child_ptr : _children)
    {
        if (child_ptr.get() == child)
        {
            _children.remove(child_ptr);
            break;
        }
    }
}

void element_if::set_angle(float angle)
{
    _matrixes_changed = true;
    _angle = angle * 180.f / M_PI;
}

float element_if::get_angle() const
{
    return _angle;
}

void element_if::set_position(vec2 pos)
{
    _matrixes_changed = true;
    _position = pos;
}

vec2 const& element_if::get_position() const
{
    return _position;
}

void element_if::set_size(vec2 const& size)
{
    _matrixes_changed = true;
    _size = size;
}

vec2 element_if::get_size() const
{
    return _size;
}

mat3 element_if::parent_to_child_matrix()
{
    if (_matrixes_changed) calculate_transform_matrixes();
    return _parent_to_child_mat;
}

mat3 element_if::child_to_parent_matrix()
{
    if (_matrixes_changed) calculate_transform_matrixes();
    return _child_to_parent_mat;
}

rect_t element_if::get_aabb() const
{
    return
    {   _size, _position};
}

void element_if::destroy()
{
    if (_parent) _parent->remove_child(this);
}

void element_if::set_parent(element_if* parent)
{
    _parent = parent;
}
element_if* element_if::get_parent()
{
    return _parent;
}

void element_if::set_collision_group(uint8_t group)
{
    _collision_group = group;
}
uint8_t element_if::get_collision_group() const
{
    return _collision_group;
}

void element_if::set_background(std::string const& file_name)
{
    _background = file_name;
}

void element_if::draw_background(mat3 const& mvp_matrix)
{
    if (!_background.empty())
    {
        auto& graph_sys = application_t::singleton().subsystem<subsystems::graphic_t>();
        auto& shader = graph_sys.get_shader<texture_shader_t>();

        shader.set_texture(
                application_t::singleton().subsystem<subsystems::resource_manager_t>().get_texture(
                        _background.c_str()));
        shader.activate(mvp_matrix);
    }
}

element_if* element_if::get_child(vec2 const& coord)
{
    element_if* ret = 0;

    //reverse order
    for (auto it = _children.rbegin(), end = _children.rend(); it != end; ++it)
    {
        auto aabb = (*it)->get_aabb();
        if (is_intersect(coord, aabb))
        {
            ret = (*it).get();
            break;
        }
    }

    return ret;
}

void element_if::calculate_transform_matrixes()
{
    _child_to_parent_mat = mat3::translation(_position.x, _position.y) * mat3::rotation(_angle)
            * mat3::scaling(_size.x / COORDINATE_FACTOR, _size.y / COORDINATE_FACTOR);
    _parent_to_child_mat = _child_to_parent_mat;
    _parent_to_child_mat.inverse();

    _matrixes_changed = false;
}

