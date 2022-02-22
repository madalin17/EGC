#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <math.h>

/* Function that creates a cone mesh from a triangle fan - insipred from lab3 and Tema1 */
Mesh* object2D::MakeCone(const std::string& name,
    glm::vec3 center,
    float radius,
    int sides,
    int height,
    glm::vec3 color,
    bool fill)
{
    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    std::vector<unsigned int> indices;
    std::vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(center, color));
    indices.push_back(0);
    for (int i = 1; i <= sides; i++) {
        float x = center.x + radius * cos(2 * i * M_PI / sides);
        float z = center.z + radius * sin(2 * i * M_PI / sides);

        vertices.push_back(VertexFormat(glm::vec3(x, -height, z), color));
        indices.push_back(i);
    }
    indices.push_back(1);
    
    circle->InitFromData(vertices, indices);
    return circle;
}