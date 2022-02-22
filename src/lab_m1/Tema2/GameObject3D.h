#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class GameObject3d
    {
    public:
        glm::vec3 Position;
        glm::vec3 Size;

        GameObject3d(glm::vec3 Position, glm::vec3 Size);
        ~GameObject3d();
    };
}   // namespace m1