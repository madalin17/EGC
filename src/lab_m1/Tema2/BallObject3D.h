#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class BallObject3d
    {
    public:
        glm::vec3 Position;
        float Radius;

        BallObject3d(glm::vec3 Position, float Radius);
        ~BallObject3d();
    };
}   // namespace m1