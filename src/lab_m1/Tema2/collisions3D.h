#pragma once

#include "lab_m1/Tema2/GameObject3D.h"
#include "lab_m1/Tema2/BallObject3D.h"
#include <algorithm>
#include <math.h>

using namespace m1;

namespace collisions3d
{
    bool CheckCollisionRR(GameObject3d one, GameObject3d two); // AABB - AABB collision

    bool CheckCollisionRC(BallObject3d one, GameObject3d two); // AABB - Circle collision

    bool CheckCollisionCC(BallObject3d one, BallObject3d two); // Circle - Circle collision
}   // namespace collisions3d
