#pragma once

#include "lab_m1/Tema1/enemy.h"
#include "lab_m1/Tema1/hero.h"
#include "lab_m1/Tema1/projectile.h"
#include "lab_m1/Tema1/obstacle.h"
#include "lab_m1/Tema1/pickup.h"

#include "components/simple_scene.h"


using namespace std;

namespace m1
{
    // Lab3_Vis2D
    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    // Lab3_Vis2D
    struct LogicSpace
    {
        LogicSpace() : x(0), y(0), width(1), height(1) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
        float x;
        float y;
        float width;
        float height;
    };

    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // Lab3_Vis2D
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear);

        // Added by me
        void CreateMap(glm::mat3 visMatrix);
        void CreateObstacles(glm::mat3 visMatrix);
        void CreateHero(glm::mat3 visMatrix);
        void CreateEnemies(glm::mat3 visMatrix, float deltaTimeSeconds);
        void CreateProjectiles(glm::mat3 visMatrix, float deltaTimeSeconds);
        void CreateHealthbar(glm::mat3 visMatrix);
        void CreatePickUps(glm::mat3 visMatrix, float deltaTimeSeconds);
        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);
        void DrawBorder(glm::mat3 visMatrix);
        void GAMEOVER();
        bool allProjectilesDead();

     protected:
        glm::mat3 modelMatrix, visMatrix;
        GLenum polygonMode;
        ViewportSpace viewSpace, minimapViewSpace;
        LogicSpace logicSpace, minimapLogicSpace, borderLogicSpace;

        // Added by me
        Hero hero;
        float angularMouse, angularMouseProjectile;
        vector<Obstacle> obstacles;
        vector<pair<float, float>> enemies;
        vector<Enemy> currEnemies;
        vector<float> enemySpeed;
        float enemyTime;
        vector<Projectile> projectiles;
        float fireRate;
        vector<PickUp> pickUps;
        float pickUpTime;
        bool fastFireRate;
        float fastFireRateDuration;
        int pointsFactor;
        float doubleUpDuration;
    };
}   // namespace m1
