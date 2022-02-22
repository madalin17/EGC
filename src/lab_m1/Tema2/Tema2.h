#pragma once

#include <math.h>
#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/camera.h"
#include "lab_m1/Tema2/hero3D.h"
#include "lab_m1/Tema2/projectile3D.h" 
#include "lab_m1/Tema2/enemy3D.h"
#include "lab_m1/Tema2/Maze.h"
#include "lab_m1/Tema2/collisions3D.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
            glm::vec3 color = WHITE, bool deformed = false, float time_to_die = 0.0f);

        void setPolygonMode();
        void createGame(float deltaTimeSeconds);
        void createHuds(float deltaTimeSeconds);
        void setMapCameraViewProjection();
        void setHudsProjection();
        void setMinimapCameraViewProjection();

        void drawHero(int scale);
        void drawProjectiles(float deltaTimeSeconds);
        void drawEnemies(float deltaTimeSeconds, int scale);
        void drawMaze();
        void drawHealthHud();
        void drawTimeHud();
        void drawMinimap(float deltaTimeSeconds);

        void moveCameraFirstToThirdAndBack();
        bool checkHeroWallsCollision(float newX, float newZ);

        void WINNING();
        void RESETGAME();
        void GAMEOVER();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        GLenum polygonMode;

        camera::Camera *camera;
        glm::mat4 projectionMatrix, proMatrix, ortoMatrix, minimapMatrix;
        glm::vec3 cameraProjectionPosition, cameraMinimapPosition;

        Hero3d *hero;
        Maze *maze;
        std::vector<Projectile3d*> projectiles;
        std::vector<std::pair<int, int>> levels;
        std::vector<glm::vec3> floors;

        bool attackMode;
        float fireRate;
        bool anyAlive;
        int level;
        float timePassed;
    };
}   // namespace m1
