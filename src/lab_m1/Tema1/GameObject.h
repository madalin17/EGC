#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class GameObject
    {
    public:
        glm::vec2 Position;
        glm::vec2 Size;

        GameObject(glm::vec2 Position, glm::vec2 Size);
        ~GameObject();
    };
}   // namespace m1