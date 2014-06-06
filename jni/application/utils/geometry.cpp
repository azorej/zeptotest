#include "geometry.hpp"

#include "utils/random.hpp"

#include <array>
#include <algorithm>
#include <ctime>

namespace lines
{
    vec2 get_offset_point(std::array<vec2, 2> const& line, float offset)
    {
        my_assert(line[0] != line[1]);

        vec2 ret;
        ret.x = (1 - offset) * line[0].x + offset * line[1].x;
        ret.y = (1 - offset) * line[0].y + offset * line[1].y;

        return ret;
    }
} //end namespace lines

rect_t evaluate_aabb(const std::vector<vec2> &vertices)
{
    my_assert(!vertices.empty());

    auto l_b = vertices[0], r_t = vertices[0];
    for (auto const& point : vertices)
    {
        l_b.x = std::min(l_b.x, point.x);
        l_b.y = std::min(l_b.y, point.y);

        r_t.x = std::max(r_t.x, point.x);
        r_t.y = std::max(r_t.y, point.y);
    }

    return
    {   r_t-l_b, (r_t+l_b)/2};
}

polygon_t polygon_t::create_random(size_t verices_count, vec2 const& size)
{
    my_assert(verices_count > 2);

    std::vector<vec2> verices;
    verices.reserve(verices_count);

    //pseudo-random triangle
    std::uniform_real_distribution<float> coord_distribution(50.f, 100.f);
    auto coord_roll = std::bind(coord_distribution, my_random_generator());
    verices.push_back({100.f - coord_roll(), coord_roll()});
    verices.push_back({-coord_roll(), -coord_roll()});
    verices.push_back({coord_roll(), -coord_roll()});

    for (size_t cur_ver_count = 3; cur_ver_count < verices_count; ++cur_ver_count)
    {
        //find a random vertex, delete the next and insert two new verties
        std::uniform_int_distribution<size_t> vertex_distribution(0, cur_ver_count - 1);
        size_t last_idx = vertex_distribution(my_random_generator());
        size_t cur_idx = (last_idx + 1) % cur_ver_count;
        size_t next_idx = (cur_idx + 1) % cur_ver_count;

        std::array<vec2, 2> first_side = {{verices[last_idx], verices[cur_idx]}};
        std::array<vec2, 2> second_side = {{verices[cur_idx], verices[next_idx]}};

        //find two random points on the those sides
        std::uniform_real_distribution<float> line_offset_roll(0.25f, 0.75f);
        vec2 first_point = lines::get_offset_point(first_side, line_offset_roll(my_random_generator()));
        vec2 second_point = lines::get_offset_point(second_side, line_offset_roll(my_random_generator()));

        //delete old vertex, insert two new ones
        verices.emplace(verices.begin() + cur_idx, first_point);
        //next_idx isn't valid because we have added new element
        verices[cur_idx + 1] = second_point;
    }

    polygon_t polygon(std::move(verices));
    polygon.set_position({0.f, 0.f});

    //scale the polygon
    auto aabb = polygon.get_aabb();
    vec2 scaling_factor = size / aabb.size;

    polygon.scale(scaling_factor);

    return polygon;
}

polygon_t::polygon_t(const std::vector<vec2> &vertices)
{
    auto aabb = evaluate_aabb(vertices);

    _vertices.reserve(vertices.size());
    for (auto const& point : vertices)
    {
        _vertices.push_back(point - aabb.position);
    }
    _position = aabb.position;
}

polygon_t::polygon_t(std::vector<vec2> &&vertices)
        : _vertices(vertices)
{
    auto aabb = evaluate_aabb(_vertices);

    for (auto& point : _vertices)
    {
        point -= aabb.position;
    }
    _position = aabb.position;
}

polygon_t::polygon_t(const std::vector<vec2> &vertices, vec2 position)
        : _vertices(vertices),
          _position(position)
{
}

polygon_t::polygon_t(std::vector<vec2> &&vertices, vec2 position)
        : _vertices(vertices),
          _position(position)
{
}

void polygon_t::scale(const vec2 &factor)
{
    for (auto& point : _vertices)
    {
        point *= factor;
    }
}

rect_t polygon_t::get_aabb() const
{
    auto ret = evaluate_aabb(_vertices);
    ret.position = _position;
    return ret;
}

const std::vector<vec2> &polygon_t::get_vertices() const
{
    return _vertices;
}

void polygon_t::set_position(vec2 const& position)
{
    _position = position;
}

vec2 const& polygon_t::get_position() const
{
    return _position;
}

std::vector<triangle_t> polygon_t::triangulate() const
{
    //polygon is convex

    std::vector<triangle_t> ret;

    size_t v_count = _vertices.size();
    size_t triangle_count = v_count - 2;
    ret.reserve((decltype(ret)::size_type)triangle_count);

    for (size_t i = 0; i < triangle_count; ++i)
    {
        ret.push_back({_vertices[0] + _position, _vertices[i + 1] + _position, _vertices[i + 2] + _position});
    }

    return ret;
}

bool is_intersect(const vec2& point, const rect_t& rect)
{
    auto l_b = rect.left_bottom();
    auto r_t = rect.right_top();
    return (point.x >= l_b.x && point.x <= r_t.x && point.y >= l_b.y && point.y <= r_t.y);
}

bool is_intersect(const rect_t& rect, const vec2& point)
{
    return is_intersect(point, rect);
}

bool is_intersect(const rect_t& rect1, const rect_t& rect2)
{
    auto l_b1 = rect1.left_bottom(), r_t1 = rect1.right_top();
    auto l_b2 = rect2.left_bottom(), r_t2 = rect2.right_top();
    if (l_b1.x > r_t2.x || r_t1.x < l_b2.x || r_t1.y < l_b2.y || l_b1.y > r_t2.y) return false;
    return true;
}
