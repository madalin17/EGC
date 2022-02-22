#pragma once

#include "components/simple_scene.h"
#include "defines.h"
#include <vector>
#include <math.h>

namespace m1
{
    class Cone
    {
    public:
        glm::vec3 coord;
        glm::vec3 color;
        float speed;
        float rx, rz;
        int signX, signZ;

        Cone(glm::vec3 _coord, glm::vec3 _color, float _speed)
            : coord(_coord)
            , color(_color)
            , speed(_speed)
            , rx(0), rz(0)
            , signX(1), signZ(1) { }

        void update(float deltaTimeSeconds);
    };
}   // namespace m1