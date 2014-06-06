/*
 * meteorite.hpp
 *
 *  Created on: Jun 4, 2014
 *      Author: azorej
 */

#ifndef ZEPTOTEST_METEORITE_HPP_
#define ZEPTOTEST_METEORITE_HPP_

#include "defs.hpp"
#include "physic_element.hpp"
#include "utils/vecmath.hpp"
#include "physics/world.hpp"
#include "utils/geometry.hpp"

class meteorite_t: public physic_element_if
{
public:
    meteorite_t(vec2 const& pos, vec2 const& size, std::shared_ptr<physics::world_t> world);

    virtual void draw(mat3 const& mvp_matrix) override;

    void on_collide(physic_element_if* other) override;

private:
    std::vector<triangle_t> _vertices;
    bool is_descedant;
};

#endif /* ZEPTOTEST_METEORITE_HPP_ */
