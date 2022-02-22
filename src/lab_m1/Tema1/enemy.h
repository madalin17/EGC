#pragma once

namespace m1
{
    class Enemy
    {
    public:
        float x, y;
        float speed;
        float angle;
        bool alive;

        Enemy(float x, float y, float speed, float angle = 0, bool alive = true);
        ~Enemy();
    };
}   // namespace m1