#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "cell.h"
#include "colors.h"
#include "defines.h"
#include "dancer.h"
#include "object2D.h"
#include "cone.h"
#include "globe.h"
#include <vector>

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture,
            glm::vec3 globePosition, float time, const glm::vec3& color = BLACK);
        void RenderComplexMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
            std::vector<glm::vec3> _light_positions, std::vector<glm::vec3> _light_colors,
            Texture2D* texture, glm::vec3 globePosition, float time);
        void RenderGlobeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, float time);
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

        void createFloor();
        void createWalls();
        void createDancers();
        void createGlobe();
        void createCones();

        void drawFloor();
        void drawWalls();
        void drawDancers(float deltaTimeSeconds);
        void drawGlobe(float deltaTimeSeconds);
        void drawCones(float deltaTimeSeconds);

        FloorCell* floor[DISCO_LEN][DISCO_LEN];
        WallCell* left_wall[DISCO_LEN][WALL_LEN];
        WallCell* right_wall[DISCO_LEN][WALL_LEN];
        WallCell* back_wall[DISCO_LEN][WALL_LEN];
        FloorCell* ceil[DISCO_LEN][DISCO_LEN];
        std::vector<Dancer*> dancers;
        std::vector<Cone*> cones;
        std::vector<glm::vec3> spotlight_directions;
        std::vector<glm::vec3> spotlight_positions;
        std::vector<glm::vec3> spotlight_colors;
        Globe* globe;
        float cut_off;
    };
}   // namespace m1
