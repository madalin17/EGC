#pragma once

#include "lab_m1/Tema1/defines.h"

namespace m1
{
    class PickUp
    {
    public:
        float x, y;
        PickUpType type;
        bool alive;

        PickUp(float x, float y, PickUpType type, bool alive = true);
        ~PickUp();
    };
}   // namespace m1