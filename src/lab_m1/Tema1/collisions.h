#pragma once

#include "lab_m1/Tema1/GameObject.h"
#include "lab_m1/Tema1/BallObject.h"

using namespace m1;

namespace collisions
{

    bool CheckCollisionRR(GameObject& one, GameObject& two); // AABB - AABB collision

    bool CheckCollisionRC(BallObject& one, GameObject& two); // AABB - Circle collision

    bool CheckCollisionCC(BallObject& one, BallObject& two); // Circle - Circle collision

    inline bool CheckColissionCSUp(BallObject& one, GameObject& two) // Circle - Surface collision
    {
        return (two.Position.y + two.Size.y) < one.Position.y + one.Radius;
    }

    inline bool CheckColissionCSDown(BallObject& one, GameObject& two) // Circle - Surface collision
    {
        return two.Position.y > one.Position.y - one.Radius;
    }

    inline bool CheckColissionCSLeft(BallObject& one, GameObject& two) // Circle - Surface collision
    {
        return two.Position.x > one.Position.x - one.Radius;
    }

    inline bool CheckColissionCSRight(BallObject& one, GameObject& two) // Circle - Surface collision
    {
        return (two.Position.x + two.Size.x) < one.Position.x + one.Radius;
    }

    inline bool CheckColissionCS(BallObject& one, GameObject& two) // Circle - Surface collision
    {
        return CheckColissionCSUp(one, two) || CheckColissionCSDown(one, two) ||
            CheckColissionCSLeft(one, two) || CheckColissionCSRight(one, two);
    }

}   // namespace collisions
