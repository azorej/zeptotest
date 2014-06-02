#include "polygon.hpp"

#include <random>
#include <array>
#include <algorithm>
#include <ctime>

polygon_t polygon_t::create_random(size_t verices_count, vec2 const& size)
{
    my_assert(verices_count > 2);

    //не лучшая идея использовать time как seed, но random_device не всегда реализован
    std::default_random_engine random_generator((unsigned int)time(0));

    std::vector<vec2> verices;
    verices.reserve(verices_count);

    //создадим псевдо-случайный случайный треугольник
    std::uniform_real_distribution<float> coord_distribution(50.f, 100.f);
    auto coord_roll = std::bind(coord_distribution, random_generator);
    verices.push_back({100.f - coord_roll(),  coord_roll()});
    verices.push_back({-coord_roll()       , -coord_roll()});
    verices.push_back({coord_roll()        , -coord_roll()});

    for(size_t cur_ver_count = 3; cur_ver_count < verices_count; ++cur_ver_count)
    {
        //найдем случайную вершину, следующую за ней вершину удалим и заменим на две новые
        std::uniform_int_distribution<size_t> vertex_distribution(0, cur_ver_count-1);
        size_t last_idx = vertex_distribution(random_generator);
        size_t cur_idx = (last_idx + 1) % cur_ver_count;
        size_t next_idx = (cur_idx + 1) % cur_ver_count;

        std::array<vec2, 2> first_side = {{verices[last_idx], verices[cur_idx]}};
        std::array<vec2, 2> second_side = {{verices[cur_idx], verices[next_idx]}};

        //найдем случайные точки на этих двух сторонах
        std::uniform_real_distribution<float> line_offset_roll(0.25f, 0.75f);
        vec2 first_point = lines::get_offset_point(first_side, line_offset_roll(random_generator));
        vec2 second_point = lines::get_offset_point(second_side, line_offset_roll(random_generator));

        //удаляем старую вершину, ставим вместо неё 2 новых
        verices.emplace(verices.begin() + cur_idx, first_point);
        //next_idx не валиден, т.к. мы добавили новый элемент
        verices[cur_idx+1] = second_point;
    }

    polygon_t polygon(std::move(verices));
    polygon.set_position({0.f, 0.f});

    //масштабируем полигон
    auto aabb = polygon.get_aabb();
    vec2 scaling_factor = size/aabb.size;

    polygon.scale(scaling_factor);

    return polygon;
}

polygon_t::polygon_t(const std::vector<vec2> &vertices)
{
    auto aabb = evaluate_aabb(vertices);

    _vertices.reserve(vertices.size());
    for(auto const& point : vertices)
    {
        _vertices.push_back(point-aabb.position);
    }
    _position = aabb.position;
}

polygon_t::polygon_t(std::vector<vec2> &&vertices)
    : _vertices(vertices)
{
    auto aabb = evaluate_aabb(_vertices);

    for(auto& point : _vertices)
    {
        point -= aabb.position;
    }
    _position = aabb.position;
}

polygon_t::polygon_t(const std::vector<vec2> &vertices, vec2 position)
    : _vertices(vertices)
    , _position(position)
{
}

polygon_t::polygon_t(std::vector<vec2> &&vertices, vec2 position)
    : _vertices(vertices)
    , _position(position)
{
}


void polygon_t::scale(const vec2 &factor)
{
    for(auto& point : _vertices)
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
    //т.к. мноугольник выпуклый, то алгоритм элементарен
    //будем отрезать треугольники пока не закончатся вершины
    std::vector<triangle_t> ret;

    size_t v_count = _vertices.size();
    size_t triangle_count = v_count - 2;
    ret.reserve(triangle_count);

    for(size_t i=0; i<triangle_count; ++i)
    {
        ret.push_back({_vertices[0]+_position, _vertices[i+1]+_position, _vertices[i+2]+_position});
    }

    return ret;
}