#pragma once
#include <math.h>

namespace m1
{
    class Projectile
    {
    public:
        float x, y;
        float angle;
        float speed;
        float time;
        bool alive;

        Projectile(float x, float y, float angle = 0.0f, float speed = 1.0f, float time = 0.0f, bool alive = true);
        ~Projectile();
    };
}   // namespace m1