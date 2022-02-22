#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

// Lab3
namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* MakeSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    // Creates circle from a triangle fan given a center, a radius, the number of triangles and color
    Mesh* MakeCircle(const std::string& name, glm::vec3 center, float radius, int sides, glm::vec3 color, bool fill = false);
}
