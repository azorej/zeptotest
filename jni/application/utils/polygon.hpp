#ifndef ZEPTOTEST_POLYGON_HPP
#define ZEPTOTEST_POLYGON_HPP

#include "defs.hpp"
#include "utils/vecmath.hpp"

#include <vector>
#include <array>

namespace lines
{
    inline vec2 get_offset_point(std::array<vec2, 2> const& line, float offset)
    {
        my_assert(line[0] != line[1]);

        vec2 ret;
        ret.x = (1-offset)*line[0].x + offset*line[1].x;
        ret.y = (1-offset)*line[0].y + offset*line[1].y;

        return ret;
    }
} //end namespace lines


struct rect_t
{
    vec2 size;
    vec2 position;

    void scale(vec2 const& factor)
    {
        size*=factor;
    }

    void move(vec2 const& vector)
    {
        position+=vector;
    }
};

struct triangle_t
{
    vec2 v1, v2, v3;
};

inline rect_t evaluate_aabb(const std::vector<vec2> &vertices)
{
    my_assert(!vertices.empty());

    auto l_b = vertices[0], r_t = vertices[0];
    for(auto const& point : vertices)
    {
        l_b.x = std::min(l_b.x, point.x);
        l_b.y = std::min(l_b.y, point.y);

        r_t.x = std::max(r_t.x, point.x);
        r_t.y = std::max(r_t.y, point.y);
    }

    return {r_t-l_b, (r_t+l_b)/2};
}

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

#endif // POLYGON_HPP
