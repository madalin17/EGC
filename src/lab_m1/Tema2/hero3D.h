#pragma once

#include "lab_m1/Tema2/defines.h"
#include "lab_m1/Tema2/colors.h"
#include "components/simple_scene.h"

namespace m1
{
    class Hero3d
    {
    public:
        glm::vec3 coord;
        float angle;
        float speed;
        int health;
	    bool alive;

        Hero3d(glm::vec3 coord, float angle = 0, float speed = HERO_SPEED, int health = HERO_HEALTH, bool alive = true);
        ~Hero3d();
        void reset();
    };
}   // namespace m1