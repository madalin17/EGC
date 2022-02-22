#pragma once

#include "components/simple_scene.h"
#include "defines.h"
#include "colors.h"
#include <vector>
#include <math.h>

namespace m1
{
    class Dancer
    {
    public:
        glm::vec3 coord;
        glm::vec3 scale;
        glm::vec3 follow;
        int i, j;

        Dancer()
            : coord(IMPLICIT)
            , scale(IMPLICIT)
            , follow(IMPLICIT)
            , i(0), j(0) { }

        void create();
        void updateTarget();
        bool outside();
        void move(float deltaTimeSeconds);
        void getSquare();
    };
}   // namespace m1