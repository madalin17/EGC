#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/defines.h"

namespace m1
{
    class Projectile3d
    {
    public:
        glm::vec3 coord;
        float angle;
        float speed;
        float time;
        bool alive;

        Projectile3d(glm::vec3 coord, float angle = 0.0f, float speed = PROJECTILE_SPEED, float time = 0.0f, bool alive = true);
        ~Projectile3d();

        void move(float deltaTimeSeconds);
    };
}   // namespace m1