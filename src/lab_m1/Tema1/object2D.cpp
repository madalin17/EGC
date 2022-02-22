#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <math.h>

// Lab3 
Mesh* object2D::MakeSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

// Function that creates a circle mesh from a triangle fan
Mesh* object2D::MakeCircle(const std::string& name,
    glm::vec3 center,
    float radius,
    int sides,
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
        float y = center.y + radius * sin(2 * i * M_PI / sides);

        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
        indices.push_back(i);
    }
    indices.push_back(1);
    
    circle->InitFromData(vertices, indices);
    return circle;
}