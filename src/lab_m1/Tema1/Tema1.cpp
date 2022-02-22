#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <math.h>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/colors.h"
#include "lab_m1/Tema1/defines.h"
#include "lab_m1/Tema1/collisions.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    // Set camera
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = LOWER_CORNER_LOGIC_SPACE;        // Logic x
    logicSpace.y = LOWER_CORNER_LOGIC_SPACE;        // Logic y
    logicSpace.width = UPPER_CORNER_LOGIC_SPACE;    // Logic width
    logicSpace.height = UPPER_CORNER_LOGIC_SPACE;   // Logic height

    minimapLogicSpace.x = MAP_CORNER;               // Minimap logic x
    minimapLogicSpace.y = MAP_CORNER;               // Minimap logic y
    minimapLogicSpace.width = MAP_LENGTH;           // Minimap logic width
    minimapLogicSpace.height = MAP_LENGTH;          // Minimap logic height

    borderLogicSpace.x = LOWER_CORNER_LOGIC_SPACE;  // Border logic x
    borderLogicSpace.y = LOWER_CORNER_LOGIC_SPACE;  // Border logic y
    borderLogicSpace.width = IMPLICIT_LEN;          // Border logic width
    borderLogicSpace.height = IMPLICIT_LEN;         // Border logic height

    polygonMode = GL_FILL;                          // Fill mode(SPACE changes to GL_LINES or GL_POINTS)

    // Set hero coordinates
    hero.x = (logicSpace.x + logicSpace.width) / 2;
    hero.y = (logicSpace.y + logicSpace.height) / 2;

    // Add obstacles to list(corner.x, corner.y, width, height)
    obstacles.push_back(Obstacle(-2, -2, 1.3f, 2));
    obstacles.push_back(Obstacle(-2, -3.3f, 3.3f, 1.3f));
    obstacles.push_back(Obstacle(-5.5f, 3, 3.5f, 1.5f));
    obstacles.push_back(Obstacle(3.5f, 2.5f, 1.5f, 2.5f));
    obstacles.push_back(Obstacle(6, -4, 1.3f, 2.6f));
    obstacles.push_back(Obstacle(-3.9f, -3, 1, 2));

    // Add enemy coordinates
    enemies.push_back(make_pair(-3, 0));
    enemies.push_back(make_pair(3, 4));
    enemies.push_back(make_pair(6, 3));
    enemies.push_back(make_pair(3, -5));
    enemies.push_back(make_pair(-3, -4));

    // Add enemy speeds
    enemySpeed.push_back(1);
    enemySpeed.push_back(0.5f);
    enemySpeed.push_back(0.75f);
    enemySpeed.push_back(1.25f);
    enemySpeed.push_back(0.25f);

    // Set timing
    enemyTime = 0;
    fireRate = 0;
    pickUpTime = 0;
    fastFireRate = false;
    fastFireRateDuration = 0;

    // Without the double-up pick-up, the score doesn't scale
    pointsFactor = 1;

    // Add meshes
    Mesh* square = object2D::MakeSquare("square", CENTER, IMPLICIT_LEN, WHITE, true);
    AddMeshToList(square);

    Mesh* empty_square = object2D::MakeSquare("empty_square", CENTER, IMPLICIT_LEN, WHITE);
    AddMeshToList(empty_square);

    Mesh* circle = object2D::MakeCircle("circle", CENTER, IMPLICIT_LEN, SIDES, WHITE, true);
    AddMeshToList(circle);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema1::Update(float deltaTimeSeconds)
{
    srand(time(NULL));  // Randomizer

    // SPACE changes to GL_LINES or GL_POINTS
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Make surfaces oerride eachother
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Create viewspace and set as the screen
    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);
     
    // Compute the 2D visualization matrix and draw scene
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene(visMatrix, deltaTimeSeconds);

    //Draw border between map and minimap with a new 2D visualization matrix
    minimapViewSpace = ViewportSpace(0, 0, resolution.x / MINIMAP_SCALE, resolution.y / MINIMAP_SCALE);
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(borderLogicSpace, minimapViewSpace);
    DrawBorder(visMatrix);

    // Draw minimap with a new 2D visualization matrix
    minimapViewSpace = ViewportSpace(0, 0, resolution.x / MINIMAP_SCALE, resolution.y / MINIMAP_SCALE);
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(minimapLogicSpace, minimapViewSpace);
    DrawScene(visMatrix, deltaTimeSeconds);

    // Closes the program if the hero is dead
    GAMEOVER();
}

void Tema1::FrameEnd()
{
}

// 2D visualization matrix -> Lab3_Vis2D
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes) -> Lab3_Vis2D
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

// Sets the viewport for the screen -> Lab3_Vis2D
void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

// Creates the square map in range: (-6,  -6) - (10, 10)
void Tema1::CreateMap(glm::mat3 visMatrix)
{
    glm::mat3 modelMatrixMap;

    modelMatrixMap = glm::mat3(1);
    modelMatrixMap *= visMatrix * transform2D::Translate(MAP_CORNER, MAP_CORNER);
    modelMatrixMap *= transform2D::Scale(MAP_LENGTH, MAP_LENGTH);
    RenderMesh2D(meshes["square"], modelMatrixMap, LIGHT_BLUE);
}

// Creates all obstacles on the map
void Tema1::CreateObstacles(glm::mat3 visMatrix)
{
    glm::mat3 modelMatrixObs;

    for (Obstacle& obstacle : obstacles) {
        modelMatrixObs = glm::mat3(1);
        modelMatrixObs *= visMatrix * transform2D::Translate(obstacle.x, obstacle.y);
        modelMatrixObs *= transform2D::Scale(obstacle.width, obstacle.height);
        RenderMesh2D(meshes["square"], modelMatrixObs, SLATE_GREY);
    }
}

// Creates the hero
void Tema1::CreateHero(glm::mat3 visMatrix)
{
    glm::mat3 modelMatrixHero;

    // Check collisions with alive enemies
    for (Enemy& enemy : currEnemies) {
        if (enemy.alive && collisions::CheckCollisionCC(BallObject(glm::vec2(hero.x, hero.y), BODY_LEN / 2),
                BallObject(glm::vec2(enemy.x, enemy.y), BODY_LEN / 2))) {
            hero.score -= HIT_BY_ENEMY;     // Decreases score if enemy hit hero
            cout << "Enemy hit you! New score: " << hero.score << endl;

            hero.health -= HEALTH_DECAY;    // Decreases health if enemy hit hero
            enemy.alive = false;            // Kills enemy
        }
    }

    // Checks if hero still alive
    hero.alive = hero.health > 0 ? true : false;

    // Create hero body
    modelMatrixHero = glm::mat3(1);
    modelMatrixHero *= visMatrix * transform2D::Translate(hero.x, hero.y);
    modelMatrixHero *= transform2D::Rotate(angularMouse);
    modelMatrixHero *= transform2D::Scale(BODY_LEN / 2, BODY_LEN / 2);
    RenderMesh2D(meshes["circle"], modelMatrixHero, YELLOW);

    // Create hero first eye
    modelMatrixHero = glm::mat3(1);
    modelMatrixHero *= visMatrix * transform2D::Translate(hero.x, hero.y);
    modelMatrixHero *= transform2D::Rotate(angularMouse);
    modelMatrixHero *= transform2D::Rotate(-ANGULAR_EYE);
    modelMatrixHero *= transform2D::Translate(0, -BODY_LEN / 2);
    modelMatrixHero *= transform2D::Scale(HEAD_LEN / 2, HEAD_LEN / 2);
    RenderMesh2D(meshes["circle"], modelMatrixHero, PALE_YELLOW);

    // Create hero second eye
    modelMatrixHero = glm::mat3(1);
    modelMatrixHero *= visMatrix * transform2D::Translate(hero.x, hero.y);
    modelMatrixHero *= transform2D::Rotate(angularMouse);
    modelMatrixHero *= transform2D::Rotate(ANGULAR_EYE);
    modelMatrixHero *= transform2D::Translate(0, -BODY_LEN / 2);
    modelMatrixHero *= transform2D::Scale(HEAD_LEN / 2, HEAD_LEN / 2);
    RenderMesh2D(meshes["circle"], modelMatrixHero, PALE_YELLOW);
}

// Creates the enemies
void Tema1::CreateEnemies(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    glm::mat3 modelMatrixEnemy;

    int rand1, rand2;
    // Creates a random position and speed enemy every 3 seconds
    if (!enemyTime) {
        rand1 = rand() % ENEMIES;
        rand2 = rand() % ENEMIES;

        currEnemies.push_back(Enemy(enemies[rand1].first, enemies[rand1].second, enemySpeed[rand2]));
    }

    // Update and reset the contor for the enemies spawn
    enemyTime += deltaTimeSeconds;
    enemyTime = enemyTime > SPAWN_SPEED ? 0 : enemyTime;

    for (Enemy &enemy : currEnemies) {
        if (enemy.alive) {
            // Update coordinates and angle for every enemy
            enemy.angle = glm::atan((float) hero.x - enemy.x, (float) hero.y - enemy.y);
            enemy.y += enemy.speed * deltaTimeSeconds * glm::cos(enemy.angle);
            enemy.x += enemy.speed * deltaTimeSeconds * glm::sin(enemy.angle);

            enemy.angle = M_PI - enemy.angle;

            // If alive enemy doesn't collide with the map, render enemy
            if (!collisions::CheckColissionCS(BallObject(glm::vec2(enemy.x, enemy.y), BODY_LEN / 2),
                    GameObject(glm::vec2(MAP_CORNER, MAP_CORNER), glm::vec2(MAP_LENGTH, MAP_LENGTH)))) {
                // Create enemy body
                modelMatrixEnemy = glm::mat3(1);
                modelMatrixEnemy *= visMatrix * transform2D::Translate(enemy.x, enemy.y);
                modelMatrixEnemy *= transform2D::Rotate(enemy.angle);
                modelMatrixEnemy *= transform2D::Scale(BODY_LEN / 2, BODY_LEN / 2);
                RenderMesh2D(meshes["circle"], modelMatrixEnemy, RED);

                // Create enemy first eye
                modelMatrixEnemy = glm::mat3(1);
                modelMatrixEnemy *= visMatrix * transform2D::Translate(enemy.x, enemy.y);
                modelMatrixEnemy *= transform2D::Rotate(enemy.angle);
                modelMatrixEnemy *= transform2D::Rotate(ANGULAR_EYE);
                modelMatrixEnemy *= transform2D::Translate(0, -BODY_LEN / 2);
                modelMatrixEnemy *= transform2D::Scale(HEAD_LEN / 2, HEAD_LEN / 2);
                RenderMesh2D(meshes["circle"], modelMatrixEnemy, ORANGE_RED);

                // Create enemy second eye
                modelMatrixEnemy = glm::mat3(1);
                modelMatrixEnemy *= visMatrix * transform2D::Translate(enemy.x, enemy.y);
                modelMatrixEnemy *= transform2D::Rotate(enemy.angle);
                modelMatrixEnemy *= transform2D::Rotate(-ANGULAR_EYE);
                modelMatrixEnemy *= transform2D::Translate(0, -BODY_LEN / 2);
                modelMatrixEnemy *= transform2D::Scale(HEAD_LEN / 2, HEAD_LEN / 2);
                RenderMesh2D(meshes["circle"], modelMatrixEnemy, ORANGE_RED);
            }
        }
    }
}

// Renders the projectiles
void Tema1::CreateProjectiles(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    glm::mat3 modelMatrixProjectile;
    bool anyAlive = false;

    // For decreased fire rate pick-up: update time and end pick-up duration at 4 seconds
    fastFireRateDuration += fastFireRate ? deltaTimeSeconds : 0;
    fastFireRate = (fastFireRateDuration >= FAST_FIRE_RATE_DURATION || !fastFireRateDuration) ? false : true;

    // For double-up pick-up: update time and end double-up duration at 10 seconds
    doubleUpDuration = pointsFactor == SCALE ? doubleUpDuration + deltaTimeSeconds : 0;
    pointsFactor = (doubleUpDuration >= DOUBLE_UP_DURATION || !doubleUpDuration) ? 1 : SCALE;

    for (Projectile& projectile : projectiles) {
        if (projectile.alive) {                                         // For any alive projectile
            // Check if projectile hits an obstacle; if yes, projectile doesn't render anymore
            for (Obstacle& obstacle : obstacles) {
                if (collisions::CheckCollisionRC(BallObject(glm::vec2(projectile.x, projectile.y), PROJECTILE_LEN / 2),
                        GameObject(glm::vec2(obstacle.x, obstacle.y), glm::vec2(obstacle.width, obstacle.height)))) {
                    projectile.alive = false;
                }
            }

            for (Enemy& enemy : currEnemies) {
                // Check if projectile hits an enemy
                if (enemy.alive && collisions::CheckCollisionCC(BallObject(glm::vec2(projectile.x, projectile.y), PROJECTILE_LEN / 2),
                        BallObject(glm::vec2(enemy.x, enemy.y), BODY_LEN / 2))) {
                    hero.score += pointsFactor * SCORE_PER_ENEMY;       // Increases score if projectile hit enemy
                    cout << "Enemy killed! New score: " << hero.score << endl;

                    enemy.alive = false;                                // Enemy dies
                    projectile.alive = false;                           // Projectile doesn't render anymore
                }
            }

            // Check if projectile hits the edge of the map
            if (collisions::CheckColissionCS(BallObject(glm::vec2(projectile.x, projectile.y), PROJECTILE_LEN / 2),
                    GameObject(glm::vec2(MAP_CORNER, MAP_CORNER), glm::vec2(MAP_LENGTH, MAP_LENGTH)))) {
                projectile.alive = false;                               // Projectile doesn't render anymore
            }

            if (projectile.alive) {                                     // If projectile is still alive
                anyAlive = true;

                // Create projectile
                modelMatrixProjectile = glm::mat3(1);
                modelMatrixProjectile *= visMatrix * transform2D::Translate(projectile.x, projectile.y);
                modelMatrixProjectile *= transform2D::Rotate(projectile.angle);
                modelMatrixProjectile *= transform2D::Scale(PROJECTILE_LEN / 2, PROJECTILE_LEN / 2);
                RenderMesh2D(meshes["circle"], modelMatrixProjectile, SADDLE_BROWN);

                // Update projectile position
                projectile.x += projectile.speed * deltaTimeSeconds * glm::cos(projectile.angle);
                projectile.y += projectile.speed * deltaTimeSeconds * glm::sin(projectile.angle);

                // If projectile renders for more than 3 seconds, don't render projectile anymore from next frame
                projectile.time += deltaTimeSeconds;
                if (projectile.time > DEAD_PROJECTILE) {
                    projectile.alive = false;
                }
            }
        }
    }

    // Update fire rate if there are projectiles on the map
    if (anyAlive) {
        fireRate += deltaTimeSeconds;
    }
}

// Creates healthbar top-right of the screen
void Tema1::CreateHealthbar(glm::mat3 visMatrix)
{
    glm::mat3 modelMatrixHealthbar;

    float xHB = logicSpace.x + WHERE_HEALTH_BAR * logicSpace.width;
    float yHB = logicSpace.y + WHERE_HEALTH_BAR * logicSpace.height;
    float scale = (float) hero.health / HERO_HEALTH;

    // Create full empty healthbar
    modelMatrixHealthbar = glm::mat3(1);
    modelMatrixHealthbar *= visMatrix * transform2D::Translate(xHB, yHB);
    modelMatrixHealthbar *= transform2D::Scale(WIDTH_HEALTH_BAR, HEIGHT_HEALTH_BAR);
    RenderMesh2D(meshes["empty_square"], modelMatrixHealthbar, LIME_GREEN);

    // Create current healthbar, given the current health of the hero 
    modelMatrixHealthbar = glm::mat3(1);
    modelMatrixHealthbar *= visMatrix * transform2D::Translate(xHB, yHB);
    modelMatrixHealthbar *= transform2D::Scale(WIDTH_HEALTH_BAR * scale, HEIGHT_HEALTH_BAR);
    RenderMesh2D(meshes["square"], modelMatrixHealthbar, GREEN);
}

// Creates pick-ups on the map
void Tema1::CreatePickUps(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    glm::mat3 modelMatrixPickUp;
    int rand1, rand2, rand3;

    // Create a random pick-up every 10 seconds
    if (pickUpTime >= PICKUP_RATE) {
        rand1 = rand() % (MAP_LENGTH - 2) + (MAP_CORNER + 1);
        rand2 = rand() % (MAP_LENGTH - 2) + (MAP_CORNER + 1);
        rand3 = rand() % PICK_UPS;

        // Either create health increase or fire rate decrease pick-up
        if (rand3 == 0) {
            pickUps.push_back(PickUp(rand1, rand2, HEALTH_INCREASE));
        }
        else  if (rand3 == 1) {
            pickUps.push_back(PickUp(rand1, rand2, FIRE_RATE_DECREASE));
        }
        else if (rand3 == 2) {
            pickUps.push_back(PickUp(rand1, rand2, DOUBLE_UP));
        }

        pickUpTime = 0;
    }
    pickUpTime += deltaTimeSeconds;

    for (PickUp& pickUp : pickUps) {
        if (pickUp.alive) {                         // For every alive pick-up
            // Check if it collides with an obstacle
            for (Obstacle& obstacle : obstacles) {
                if (collisions::CheckCollisionRC(BallObject(glm::vec2(pickUp.x, pickUp.y), PICK_UP_LEN / 2),
                        GameObject(glm::vec2(obstacle.x, obstacle.y), glm::vec2(obstacle.width, obstacle.height)))) {
                    pickUp.alive = false;           // Pick-up doesn't render anymore
                }
            }

            // Check if it collides with an enemy
            for (Enemy& enemy : currEnemies) {
                if (enemy.alive && collisions::CheckCollisionCC(BallObject(glm::vec2(pickUp.x, pickUp.y), PICK_UP_LEN / 2),
                        BallObject(glm::vec2(enemy.x, enemy.y), BODY_LEN / 2))) {
                    pickUp.alive = false;           // Pick-up doesn't render anymore
                }
            }

            // Check if it collides with the hero
            if (collisions::CheckCollisionCC(BallObject(glm::vec2(pickUp.x, pickUp.y), PICK_UP_LEN / 2),
                    BallObject(glm::vec2(hero.x, hero.y), BODY_LEN / 2))) {
                pickUp.alive = false;               // Pick-up doesn't render anymore
                if (pickUp.type == HEALTH_INCREASE) {
                    // If pick-up is a health type, increase hero's health
                    hero.health = hero.health >= HERO_HEALTH - HEALTH_ADDED ? HERO_HEALTH : hero.health + HEALTH_ADDED;

                    cout << "Health increased by " << HEALTH_ADDED << endl;
                }
                else if (pickUp.type == FIRE_RATE_DECREASE) {
                    // If pick-up is a fire rate type, decrease fire rate(fastFireRate is true now)
                    fastFireRate = true;
                    fastFireRateDuration = 0.01f;

                    cout << "Fire rate decreased from " << FIRE_RATE / 2 << " to " << FAST_FIRE_RATE / 2 <<
                        " for " << FAST_FIRE_RATE_DURATION / 2 << " seconds" << endl;
                }
                else if (pickUp.type == DOUBLE_UP) {
                    // If pick-up is a double-up type, increase the number of points the hero gets for killing an enemy for some seconds
                    pointsFactor = SCALE;
                    doubleUpDuration = 0.01f;

                    cout << "Points for killing an enemy are doubled for the next " << DOUBLE_UP_DURATION / 2 << " seconds" << endl;
                }
            }

            // If pick-up is a health type, render a green circle with a white plus
            if (pickUp.type == HEALTH_INCREASE && pickUp.alive) {
                modelMatrixPickUp = glm::mat3(1);
                modelMatrixPickUp *= visMatrix * transform2D::Translate(pickUp.x, pickUp.y);
                modelMatrixPickUp *= transform2D::Scale(PICK_UP_LEN / 2, PICK_UP_LEN / 2);
                RenderMesh2D(meshes["circle"], modelMatrixPickUp, DARK_GREEN);

                modelMatrixPickUp = glm::mat3(1);
                modelMatrixPickUp *= visMatrix * transform2D::Translate(pickUp.x - PLUS_WIDTH / 2, pickUp.y - PLUS_HEIGHT / 2);
                modelMatrixPickUp *= transform2D::Scale(PLUS_WIDTH, PLUS_HEIGHT);
                RenderMesh2D(meshes["square"], modelMatrixPickUp, WHITE);

                modelMatrixPickUp = glm::mat3(1);
                modelMatrixPickUp *= visMatrix * transform2D::Translate(pickUp.x - PLUS_HEIGHT / 2, pickUp.y - PLUS_WIDTH / 2);
                modelMatrixPickUp *= transform2D::Scale(PLUS_HEIGHT, PLUS_WIDTH);
                RenderMesh2D(meshes["square"], modelMatrixPickUp, WHITE);
            }
            // If pick-up is a fire rate type, render a brown circle with a white square
            else if (pickUp.type == FIRE_RATE_DECREASE && pickUp.alive) {
                modelMatrixPickUp = glm::mat3(1);
                modelMatrixPickUp *= visMatrix * transform2D::Translate(pickUp.x, pickUp.y);
                modelMatrixPickUp *= transform2D::Scale(PICK_UP_LEN / 2, PICK_UP_LEN / 2);
                RenderMesh2D(meshes["circle"], modelMatrixPickUp, MAROON);

                modelMatrixPickUp = glm::mat3(1);
                modelMatrixPickUp *= visMatrix * transform2D::Translate(pickUp.x - FIRE_LEN / 2, pickUp.y - FIRE_LEN / 2);
                modelMatrixPickUp *= transform2D::Scale(FIRE_LEN, FIRE_LEN);
                RenderMesh2D(meshes["square"], modelMatrixPickUp, WHITE);
            }
            // If pick-up is a double-up type, render a gold circle with a red circle inside
            else if (pickUp.type == DOUBLE_UP && pickUp.alive) {
                modelMatrixPickUp = glm::mat3(1);
                modelMatrixPickUp *= visMatrix * transform2D::Translate(pickUp.x, pickUp.y);
                modelMatrixPickUp *= transform2D::Scale(PICK_UP_LEN / 2, PICK_UP_LEN / 2);
                RenderMesh2D(meshes["circle"], modelMatrixPickUp, GOLD);

                modelMatrixPickUp = glm::mat3(1);
                modelMatrixPickUp *= visMatrix * transform2D::Translate(pickUp.x, pickUp.y);
                modelMatrixPickUp *= transform2D::Scale(DOUBLE_UP_LEN, DOUBLE_UP_LEN);
                RenderMesh2D(meshes["circle"], modelMatrixPickUp, RED);
            }
        }
    }
}

// Draws the whole scene
void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    // Create map
    CreateMap(visMatrix);

    // Create obstacles
    CreateObstacles(visMatrix);

    // Create hero
    CreateHero(visMatrix);

    // Create and move enemies
    CreateEnemies(visMatrix, deltaTimeSeconds);

    // Create and move projectiles
    CreateProjectiles(visMatrix, deltaTimeSeconds);

    // Create healthbar
    CreateHealthbar(visMatrix);

    // Create health and fire rate pick-ups
    CreatePickUps(visMatrix, deltaTimeSeconds);
}

// Draws a border between the map and the minimap
void Tema1::DrawBorder(glm::mat3 visMatrix)
{
    RenderMesh2D(meshes["empty_square"], visMatrix, BLACK);
}

// If hero is dead, closes the app
void Tema1::GAMEOVER()
{
    if (!hero.alive) {
        cout << "GAME OVER!" << endl;
        exit(0);
    }
}

// Checks if no projectile is rendering anymore
bool Tema1::allProjectilesDead()
{
    for (Projectile& projectile : projectiles) {
        if (projectile.alive) {
            return false;
        }
    }
    return true;
}

// CALLBACK FUNCTIONS

// Moving the hero and the logic space
void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    bool collide = false;
    // Check if hero doesn't hit an obstacle
    for (Obstacle& obstacle : obstacles) {
        if (collisions::CheckCollisionRC(BallObject(glm::vec2(hero.x, hero.y), BODY_LEN / 2),
            GameObject(glm::vec2(obstacle.x, obstacle.y), glm::vec2(obstacle.width, obstacle.height)))) {
            collide = true;
        }
    }

    // Check if hero can move up without colliding with the map
    if (window->KeyHold(GLFW_KEY_W) && !collisions::CheckColissionCSUp(BallObject(glm::vec2(hero.x, hero.y), BODY_LEN / 2),
            GameObject(glm::vec2(MAP_CORNER, MAP_CORNER), glm::vec2(MAP_LENGTH, MAP_LENGTH))) && !collide) {

        collide = false;
        // Check if hero can move up without colliding with an obstacle
        for (Obstacle& obstacle : obstacles) {
            if (collisions::CheckCollisionRC(BallObject(glm::vec2(hero.x, hero.y + deltaTime), BODY_LEN / 2),
                GameObject(glm::vec2(obstacle.x, obstacle.y), glm::vec2(obstacle.width, obstacle.height)))) {
                collide = true;
            }
        }

        // Update hero position and logic space
        if (!collide) {
            logicSpace.y += deltaTime;
            hero.y += deltaTime;
        }
    }
    // Check if hero can move left without colliding with the map
    else if (window->KeyHold(GLFW_KEY_A) && !collisions::CheckColissionCSLeft(BallObject(glm::vec2(hero.x, hero.y), BODY_LEN / 2),
            GameObject(glm::vec2(MAP_CORNER, MAP_CORNER), glm::vec2(MAP_LENGTH, MAP_LENGTH))) && !collide) {

        collide = false;
        // Check if hero can move left without colliding with an obstacle
        for (Obstacle& obstacle : obstacles) {
            if (collisions::CheckCollisionRC(BallObject(glm::vec2(hero.x - deltaTime, hero.y), BODY_LEN / 2),
                GameObject(glm::vec2(obstacle.x, obstacle.y), glm::vec2(obstacle.width, obstacle.height)))) {
                collide = true;
            }
        }

        // Update hero position and logic space
        if (!collide) {
            logicSpace.x -= deltaTime;
            hero.x -= deltaTime;
        }
        
    }
    // Check if hero can move down without colliding with the map
    else if (window->KeyHold(GLFW_KEY_S) && !collisions::CheckColissionCSDown(BallObject(glm::vec2(hero.x, hero.y), BODY_LEN / 2),
            GameObject(glm::vec2(MAP_CORNER, MAP_CORNER), glm::vec2(MAP_LENGTH, MAP_LENGTH))) && !collide) {

        collide = false;
        // Check if hero can move down without colliding with an obstacle
        for (Obstacle& obstacle : obstacles) {
            if (collisions::CheckCollisionRC(BallObject(glm::vec2(hero.x, hero.y - deltaTime), BODY_LEN / 2),
                GameObject(glm::vec2(obstacle.x, obstacle.y), glm::vec2(obstacle.width, obstacle.height)))) {
                collide = true;
            }
        }

        // Update hero position and logic space
        if (!collide) {
            logicSpace.y -= deltaTime;
            hero.y -= deltaTime;
        }
        
    }
    // Check if hero can move right without colliding with the map
    else if (window->KeyHold(GLFW_KEY_D) && !collisions::CheckColissionCSRight(BallObject(glm::vec2(hero.x, hero.y), BODY_LEN / 2),
            GameObject(glm::vec2(MAP_CORNER, MAP_CORNER), glm::vec2(MAP_LENGTH, MAP_LENGTH))) && !collide) {

        collide = false;
        // Check if hero can move right without colliding with an obstacle
        for (Obstacle& obstacle : obstacles) {
            if (collisions::CheckCollisionRC(BallObject(glm::vec2(hero.x + deltaTime, hero.y), BODY_LEN / 2),
                GameObject(glm::vec2(obstacle.x, obstacle.y), glm::vec2(obstacle.width, obstacle.height)))) {
                collide = true;
            }
        }

        // Update hero position and logic space
        if (!collide) {
            logicSpace.x += deltaTime;
            hero.x += deltaTime;
        }
    }

    // Zoom in
    if (window->KeyHold(GLFW_KEY_Z)) {
        logicSpace.x += deltaTime / 2;
        logicSpace.y += deltaTime / 2;
        logicSpace.width -= deltaTime;
        logicSpace.height -= deltaTime;
    }
    // Zoom out
    else if (window->KeyHold(GLFW_KEY_X)) {
        logicSpace.x -= deltaTime / 2;
        logicSpace.y -= deltaTime / 2;
        logicSpace.width += deltaTime;
        logicSpace.height += deltaTime;
    }
}

void Tema1::OnKeyPress(int key, int mods)
{
    // Switch between fill view, point view and line view for polygons
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

// Change hero rotation towards the mouse
void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 resolution = window->GetResolution();
    angularMouse = glm::atan((float) mouseX - resolution.x / 2, (float) mouseY - resolution.y / 2);
}

// If left botton of the mouse is clicked, add a new projectile with hero's position and current angle of rotation made with the mouse
void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    glm::ivec2 resolution = window->GetResolution();
    angularMouseProjectile = glm::atan((float) (resolution.y - mouseY) - resolution.y / 2, (float)mouseX - resolution.x / 2);

    if (button == GLFW_MOUSE_BUTTON_2 && (projectiles.size() == 0 || (fireRate > FIRE_RATE && !fastFireRate) ||
            (fireRate > FAST_FIRE_RATE && fastFireRate) || allProjectilesDead())) {
        projectiles.push_back(Projectile(hero.x, hero.y, angularMouseProjectile));
        fireRate = 0;
    }
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
