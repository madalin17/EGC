#pragma once

#include <math.h>
#include "components/simple_scene.h"
#include "lab_m1/Tema2/defines.h"
#include "lab_m1/Tema2/projectile3D.h"

namespace m1
{
    class Enemy3d
    {
    public:
        glm::vec3 coord;
        glm::vec3 movePattern;
        float speed;
        float angle;
        float time_to_die;
        bool deformed;
        bool alive;
        bool boss;
        int lives;
        Projectile3d* projectile;

        Enemy3d(glm::vec3 coord, float speed, float angle = -M_PI_2, float time_to_die = 0, bool deformed = false,  bool alive = true);
        ~Enemy3d();

        void move(float deltaTimeSeconds);
        void isBoss(float angle);
        void resetProjectile();
    };
}   // namespace m1