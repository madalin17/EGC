#pragma once

#include <lab_m1/Tema1/defines.h>

namespace m1
{
    class Hero
    {
    public:
        float x, y;
        int health;
        int score;
	    bool alive;

        Hero();
        Hero(float x, float y, int score = 0, int health = HERO_HEALTH, bool alive = true);
        ~Hero();
    };
}   // namespace m1