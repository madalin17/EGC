#pragma once

namespace m1
{
    class Obstacle
    {
    public:
        float x, y;
        float width, height;

        Obstacle(float x, float y, float width, float height);
        ~Obstacle();
    };
}   // namespace m1