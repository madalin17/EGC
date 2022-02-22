#pragma once

#include <string>
#include "lab_m1/Tema3/colors.h"
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace object2D
{
    /* Creates a cone from a triangle fan given a center, a radius, the number of triangles, the height and the color */
    Mesh* MakeCone(const std::string& name, glm::vec3 center, float radius,
        int sides, int height, glm::vec3 color = BLACK, bool fill = false);
}
