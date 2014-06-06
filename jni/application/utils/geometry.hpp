#ifndef ZEPTOTEST_POLYGON_HPP
#define ZEPTOTEST_POLYGON_HPP

#include "defs.hpp"
#include "utils/vecmath.hpp"

#include <vector>
#include <array>

struct triangle_t
{
    vec2 v1, v2, v3;
};

struct rect_t
{
    vec2 size;
    vec2 position;

    void scale(vec2 const& factor)
    {
        size *= factor;
    }

    void move(vec2 const& vector)
    {
        position += vector;
    }

    vec2 left_bottom() const
    {
        return
        {   position.x - size.x/2, position.y - size.y/2};
    }

    vec2 right_top() const
    {
        return
        {   position.x + size.x/2, position.y + size.y/2};
    }
};

class polygon_t
{
public:
    static polygon_t create_random(size_t verices_count, vec2 const& size);

    explicit polygon_t(std::vector<vec2> const& vertices);
    explicit polygon_t(std::vector<vec2>&& vertices);
    polygon_t(std::vector<vec2> const& vertices, vec2 position);
    polygon_t(std::vector<vec2>&& vertices, vec2 position);

    void scale(vec2 const& factor);

    rect_t get_aabb() const;

    std::vector<vec2> const& get_vertices() const;

    void set_position(vec2 const& position);
    vec2 const& get_position() const;

    std::vector<triangle_t> triangulate() const;

private:
    std::vector<vec2> _vertices;
    vec2 _position;
};

namespace lines
{
    vec2 get_offset_point(std::array<vec2, 2> const& line, float offset);
} //end namespace lines

rect_t evaluate_aabb(const std::vector<vec2> &vertices);

bool is_intersect(vec2 const& point, rect_t const& rect);
bool is_intersect(rect_t const& rect, vec2 const& point);
bool is_intersect(rect_t const& rect1, rect_t const& rect2);

#endif // POLYGON_HPP