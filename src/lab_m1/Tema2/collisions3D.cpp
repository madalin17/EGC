#include "lab_m1/Tema2/collisions3d.h"
#include <iostream>
using namespace m1;

bool collisions3d::CheckCollisionRR(GameObject3d one, GameObject3d two) // AABB - AABB collision
{   
    bool collisionX = abs(one.Position.x - two.Position.x) <= (one.Size.x + two.Size.x);
    bool collisionY = abs(one.Position.y - two.Position.y) <= (one.Size.y + two.Size.y);
    bool collisionZ = abs(one.Position.z - two.Position.z) <= (one.Size.z + two.Size.z);

    return collisionX && collisionY && collisionZ;
}

bool collisions3d::CheckCollisionRC(BallObject3d one, GameObject3d two) // AABB - Sphere collision
{
    auto x = std::max(two.Position.x - two.Size.x, std::min(one.Position.x, two.Position.x + two.Size.x));
    auto y = std::max(two.Position.y - two.Size.y, std::min(one.Position.y, two.Position.y + two.Size.y));
    auto z = std::max(two.Position.z - two.Size.z, std::min(one.Position.z, two.Position.z + two.Size.z));

    auto distance = (x - one.Position.x) * (x - one.Position.x) +
        (y - one.Position.y) * (y - one.Position.y) +
        (z - one.Position.z) * (z - one.Position.z);

    return distance <= one.Radius * one.Radius;
}

bool collisions3d::CheckCollisionCC(BallObject3d one, BallObject3d two) // Sphere - Sphere collision
{
    float x = two.Position.x - one.Position.x;
    float y = two.Position.y - one.Position.y;
    float z = two.Position.z - one.Position.z;
    float r = two.Radius + one.Radius;

    return x * x + y * y + z * z <= r * r;
}
