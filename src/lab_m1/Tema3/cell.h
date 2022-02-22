#pragma once

#include "components/simple_scene.h"
#include "defines.h"
#include "colors.h"
#include <vector>

namespace m1
{
    class FloorCell
    {
    public:
        glm::vec3 coord;
        glm::vec3 color;
        glm::vec3 scale;

        FloorCell()
            : coord(IMPLICIT)
            , color(BLACK)
            , scale(IMPLICIT) { }

        FloorCell(glm::vec3 _coord, glm::vec3 _scale)
            : coord(_coord)
            , color(BLACK)
            , scale(_scale) { }

        FloorCell(glm::vec3 _coord, glm::vec3 _color, glm::vec3 _scale)
            : coord(_coord)
            , color(_color)
            , scale(_scale) { }
    };

    class WallCell
    {
    public:
        glm::vec3 coord;
        glm::vec3 scale;
        std::vector<glm::vec3> light_positions;
        std::vector<glm::vec3> light_colors;

        WallCell(glm::vec3 _coord, glm::vec3 _scale, std::vector<glm::vec3> _light_positions, std::vector<glm::vec3> _light_colors)
            : coord(_coord)
            , scale(_scale)
            , light_positions(_light_positions)
            , light_colors(_light_colors) { }
    };
}   // namespace m1