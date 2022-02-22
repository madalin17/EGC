#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class BallObject
    {
    public:
        glm::vec2 Position;
        float Radius;

        BallObject(glm::vec2 Position, float Radius);
        ~BallObject();
    };
}   // namespace m1