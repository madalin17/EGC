#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

/* Initialize all objects and constants */
void Tema2::Init()
{
    /* Setting the coloring of the objects(full colored, only lines, or only points) */
    polygonMode = GL_FILL;

    /* Cube mesh */
    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    /* Sphere mesh */
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    /* Empty square mesh */
    {
        Mesh* mesh = new Mesh("square");
        std::vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0)),
            VertexFormat(glm::vec3(0.5f, -0.5f, 0)),
            VertexFormat(glm::vec3(0.5f, 0.5f, 0)),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0))
        };
        std::vector<unsigned int> indices = { 0, 1, 2, 3 };
        mesh->SetDrawMode(GL_LINE_LOOP);
        mesh->InitFromData(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    /* Shader for all objects rendered */
    {
        Shader* shader = new Shader("HomeworkShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    /* Create the hero */
    hero = new Hero3d(glm::vec3(0, 0, 0));

    /* Create all levels - vector of pairs: (map size, time to finish maze) */
    levels.push_back(make_pair(MAP_CORNER, TIME1));
    levels.push_back(make_pair(MAP_CORNER + 4, TIME2));
    levels.push_back(make_pair(MAP_CORNER + 8, TIME3));
    level = 0;

    /* Each level's floor color */
    floors.push_back(SANDY_BROWN);
    floors.push_back(MAGENTA);
    floors.push_back(DEEP_SKY_BLUE);

    /* Create and generate the maze and the enemies */
    maze = new Maze(levels[level].first);
    maze->generate();

    /* Set up camera behind hero and projection matrices */
    camera = new camera::Camera(CAMERA_COORDS, LOOK_AT, UP);
    proMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    ortoMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 200.0f);
    minimapMatrix = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.01f, 200.0f);

    /* Set other constants */
    attackMode = WALK_MODE;
    fireRate = 0;
    anyAlive = false;
    timePassed = 0;
}

/* For the beginning of each frame */
void Tema2::FrameStart()
{
    /* Clears the color buffer(using the previously set color) and depth buffer */
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* Frame update function */
void Tema2::Update(float deltaTimeSeconds)
{
    setPolygonMode();

    createGame(deltaTimeSeconds);
    createHuds(deltaTimeSeconds);

    WINNING();
    GAMEOVER();
}

/* For the ending of each frame */
void Tema2::FrameEnd()
{
}

/* Function for rendering all objects */
void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color, bool deformed, float time_to_die)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    /* Render an object using the specified shaderand the specified position */
    glUseProgram(shader->program);

    /* Send model matrix as uniform property */
    {
        int location = glGetUniformLocation(shader->program, "Model");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    }

    /* Send view matrix as uniform property */
    {
        int location = glGetUniformLocation(shader->program, "View");
        glm::mat4 viewMatrix = camera->GetViewMatrix();
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    }

    /* Send projection matrix as uniform property */
    {
        int location = glGetUniformLocation(shader->program, "Projection");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }

    /* Send color as uniform property */
    {
        int location = glGetUniformLocation(shader->program, "color");
        glUniform3fv(location, 1, glm::value_ptr(color));
    }

    /* Send if the object is deformed(for enemies) as uniform property */
    {
        int location = glGetUniformLocation(shader->program, "deformed");
        glUniform1i(location, deformed);
    }

    /* Send how much time passed after being hit(for enemies) as uniform property */
    {
        int location = glGetUniformLocation(shader->program, "time_to_die");
        glUniform1f(location, time_to_die);
    }

    /* Draw the object */
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/* Set coloring mode for polygons: fill, onlylines or only points */
void Tema2::setPolygonMode()
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
}

/* Creating the game: hero, maze, enemies, camera positioning */
void Tema2::createGame(float deltaTimeSeconds)
{
    setMapCameraViewProjection();
    drawHero(MAP_SCALE);
    drawProjectiles(deltaTimeSeconds);
    drawEnemies(deltaTimeSeconds, MAP_SCALE);
    drawMaze();
}

/* Creating the huds: health hud, time left hud and minimap */
void Tema2::createHuds(float deltaTimeSeconds)
{
    setHudsProjection();
    drawHealthHud();
    drawTimeHud();

    setMinimapCameraViewProjection();
    drawMinimap(deltaTimeSeconds);
}

/* Set camera, viewport and projection matrix for rendering the game */
void Tema2::setMapCameraViewProjection()
{
    projectionMatrix = proMatrix;
    /* Set the camera behind the hero */
    cameraProjectionPosition = glm::vec3(IN_THE_BACK * glm::sin(hero->angle) + hero->coord.x,
                                        CAMERA_HEIGHT,
                                        IN_THE_BACK * glm::cos(hero->angle) + hero->coord.z);
    camera->Set(cameraProjectionPosition, glm::vec3(hero->coord.x, CAMERA_HEIGHT, hero->coord.z), UP);
    /* Switch between attack mode and walk mode */
    moveCameraFirstToThirdAndBack();

    /* Set viewport on the whole window */
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

/* Set projection matrix to orto for health and time huds */
void Tema2::setHudsProjection()
{
    projectionMatrix = ortoMatrix;
}

/* Set camera, viewport and projection matrix for rendering the minimap */
void Tema2::setMinimapCameraViewProjection()
{
    projectionMatrix = minimapMatrix;
    /* Set camera to look from above */
    cameraMinimapPosition = glm::vec3(hero->coord.x, LOOK_FROM_ABOVE, hero->coord.z);
    camera->Set(cameraMinimapPosition, hero->coord, FORWARD);

    /* Set viewport in the bottom left corner */
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x / 5, resolution.y / 5);
}

/* Rendering the hero */
void Tema2::drawHero(int scale)
{
    /* Check if the hero is colliding with any enemies that are alive and not deformed */
    for (Enemy3d *enemy : maze->enemies) {
        /* If hero hits an enemy, his health decreases and the enemy starts deforming */
        if (enemy->alive && !enemy->deformed &&
                collisions3d::CheckCollisionRR(GameObject3d(glm::vec3(hero->coord.x, HUMAN_HEIGHT_2, hero->coord.z), HUMAN_SCALE),
                GameObject3d(glm::vec3(enemy->coord.x + enemy->movePattern.x, HUMAN_HEIGHT_2, enemy->coord.z + enemy->movePattern.z),
                    HUMAN_SCALE))) {
            hero->health -= HEALTH_DECAY;
            enemy->deformed = true;
        }
    }

    /* Hero is alive if the health is positive */
    hero->alive = hero->health > 0;

    /* Draw head */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_HEAD);
        modelMatrix = glm::scale(modelMatrix, SCALE_HEAD);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, SKIN_TONE);
    }

    /* Draw body */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_BODY);
        modelMatrix = glm::scale(modelMatrix, SCALE_BODY);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, SEA_GREEN);
    }

    /* Draw left arm */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_LEFT_ARM);
        modelMatrix = glm::scale(modelMatrix, SCALE_ARM);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, SEA_GREEN);
    }

    /* Draw right arm */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_RIGHT_ARM);
        modelMatrix = glm::scale(modelMatrix, SCALE_ARM);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, SEA_GREEN);
    }

    /* Draw left hand */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_LEFT_HAND);
        modelMatrix = glm::scale(modelMatrix, SCALE_HAND);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, SKIN_TONE);
    }

    /* Draw right hand */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_RIGHT_HAND);
        modelMatrix = glm::scale(modelMatrix, SCALE_HAND);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, SKIN_TONE);
    }

    /* Draw left leg */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_LEFT_LEG);
        modelMatrix = glm::scale(modelMatrix, SCALE_LEG);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, MEDIUM_BLUE);
    }

    /* Draw right leg */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_RIGHT_LEG);
        modelMatrix = glm::scale(modelMatrix, SCALE_LEG);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, MEDIUM_BLUE);
    }
}

/* Rendering the projectiles */
void Tema2::drawProjectiles(float deltaTimeSeconds)
{
    /* No projectile alive before iterating through the projectiles list */
    anyAlive = false;

    for (Projectile3d *projectile : projectiles) {
        /* Check collisions for the alive projectiles */
        if (projectile->alive) {
            /* Check wall collisions */
            for (int i = 0; i < maze->size; i++) {
                int z = BOX_LEN * (-maze->size / 2 + i);
                for (int j = 0; j < maze->size; j++) {
                    int x = BOX_LEN * (-maze->size / 2 + j);

                    /* If the projectile hits a wall, it dies */
                    if (maze->cells[i][j] == 1 && collisions3d::CheckCollisionRC(BallObject3d(projectile->coord, PROJECTILE_LENGTH / 2),
                        GameObject3d(glm::vec3(x, ABOVE_GROUND, z), WALL_SCALE))) {
                        projectile->alive = false;
                    }
                }
            }

            /* Check enemy collisions */
            for (Enemy3d* enemy : maze->enemies) {
                /* If the projectile hits a wall, it dies and the enemy starts deforming */
                if (enemy->alive && collisions3d::CheckCollisionRC(BallObject3d(projectile->coord, PROJECTILE_LENGTH / 2),
                        GameObject3d(glm::vec3(enemy->coord.x + enemy->movePattern.x, HUMAN_HEIGHT_2, enemy->coord.z + enemy->movePattern.z),
                            HUMAN_SCALE))) {
                    projectile->alive = false;
                    if (!enemy->boss) {
                        enemy->deformed = true;
                    }
                    else if (enemy->boss && enemy->lives > 0) {
                        enemy->lives--;
                        if (enemy->lives == 0) {
                            enemy->deformed = true;
                        }
                    }
                }
            }

            /* If the projectile is still alive, render it */
            if (projectile->alive) {
                /* draw projectile */
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, projectile->coord);
                    modelMatrix = glm::translate(modelMatrix, TRANLATE_PROJECTILE);
                    modelMatrix = glm::rotate(modelMatrix, projectile->angle, UP);
                    modelMatrix = glm::scale(modelMatrix, SCALE_PROJECTILE);

                    RenderSimpleMesh(meshes["sphere"], shaders["HomeworkShader"], modelMatrix, SILVER);
                }

                /* Update projectile position */
                projectile->move(deltaTimeSeconds);

                /* If projectile renders for more than 2 seconds, don't render it anymore from next frame */
                if (projectile->time >= DEAD_PROJECTILE) {
                    projectile->alive = false;
                }
                /* Else, at least one projectile is alive */
                else {
                    anyAlive = true;
                }
            }
        }
    }

    /* Update fire rate if there are alive projectiles on the map */
    fireRate += anyAlive ? deltaTimeSeconds : 0;
}

/* Render the enemies */
void Tema2::drawEnemies(float deltaTimeSeconds, int scale)
{
    for (Enemy3d *enemy : maze->enemies) {
        /* If the enemy is deformed, but still alive, update the time left to render */
        if (enemy->deformed && enemy->alive) {
            enemy->time_to_die += deltaTimeSeconds;
            /* If enemy deformed for more than 3 seconds, don't render him from next frame */
            if (enemy->time_to_die > STOP_DEFORMING) {
                enemy->alive = false;
            }
        }

        if (enemy->alive) {
            /* Don't move enemy if it renders in the minimap */
            if (scale == MAP_SCALE) {
                enemy->move(deltaTimeSeconds);
            }

            /* Draw head */
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, enemy->coord + enemy->movePattern);
                modelMatrix = glm::translate(modelMatrix, ENEMY_TRANSLATE_HEAD);
                modelMatrix = glm::scale(modelMatrix, ENEMY_SCALE_HEAD);
                modelMatrix = glm::scale(modelMatrix, enemy->boss ? glm::vec3(BOSS_ENEMY_SCALE) : glm::vec3(1));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

                RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, SKIN_BROWN_TONE, enemy->deformed, enemy->time_to_die);
            }

            /* Draw body */
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, enemy->coord + enemy->movePattern);
                modelMatrix = glm::translate(modelMatrix, ENEMY_TRANSLATE_BODY);
                modelMatrix = glm::scale(modelMatrix, ENEMY_SCALE_BODY);
                modelMatrix = glm::scale(modelMatrix, enemy->boss ? glm::vec3(BOSS_ENEMY_SCALE) : glm::vec3(1));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

                RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, DARK_RED, enemy->deformed, enemy->time_to_die);
            }

            /* Draw left arm */
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, enemy->coord + enemy->movePattern);
                modelMatrix = glm::translate(modelMatrix, ENEMY_TRANSLATE_LEFT_ARM);
                modelMatrix = glm::scale(modelMatrix, ENEMY_SCALE_ARM);
                modelMatrix = glm::scale(modelMatrix, enemy->boss ? glm::vec3(BOSS_ENEMY_SCALE) : glm::vec3(1));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

                RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, DARK_RED, enemy->deformed, enemy->time_to_die);
            }

            /* Draw right arm */
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, enemy->coord + enemy->movePattern);
                modelMatrix = glm::translate(modelMatrix, ENEMY_TRANSLATE_RIGHT_ARM);
                modelMatrix = glm::scale(modelMatrix, ENEMY_SCALE_ARM);
                modelMatrix = glm::scale(modelMatrix, enemy->boss ? glm::vec3(BOSS_ENEMY_SCALE) : glm::vec3(1));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

                RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, DARK_RED, enemy->deformed, enemy->time_to_die);
            }

            /* Draw leg */
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, enemy->coord + enemy->movePattern);
                modelMatrix = glm::translate(modelMatrix, ENEMY_TRANSLATE_LEG);
                modelMatrix = glm::scale(modelMatrix, ENEMY_SCALE_LEG);
                modelMatrix = glm::scale(modelMatrix, enemy->boss ? glm::vec3(BOSS_ENEMY_SCALE) : glm::vec3(1));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

                RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, SIENNA, enemy->deformed, enemy->time_to_die);
            }

            if (enemy->boss && enemy->projectile->alive) {
                /* draw projectile */
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, enemy->projectile->coord);
                    modelMatrix = glm::translate(modelMatrix, TRANLATE_PROJECTILE);
                    modelMatrix = glm::rotate(modelMatrix, enemy->projectile->angle, UP);
                    modelMatrix = glm::scale(modelMatrix, SCALE_PROJECTILE);

                    RenderSimpleMesh(meshes["sphere"], shaders["HomeworkShader"], modelMatrix, DIMGREY);
                }

                /* Update projectile position */
                enemy->projectile->move(deltaTimeSeconds);

                /* Check projectile collision with the hero */
                if (collisions3d::CheckCollisionRC(BallObject3d(enemy->projectile->coord, PROJECTILE_LENGTH / 2),
                        (GameObject3d(glm::vec3(hero->coord.x, HUMAN_HEIGHT_2, hero->coord.z), HUMAN_SCALE)))) {
                    hero->health -= HEALTH_DECAY;
                    //enemy->resetProjectile(); 
                    enemy->projectile->alive = false;
                }
                /* If projectile renders for more than 2 seconds, don't render it anymore from next frame */
                else if (enemy->projectile->time >= DEAD_PROJECTILE) {
                    enemy->resetProjectile();
                }
            }
        }
    }
}

/* Render the maze */
void Tema2::drawMaze()
{
    /* Draw the floor */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(BOX_LEN * levels[level].first, 0.01f, BOX_LEN * levels[level].first));

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, floors[level]);
    }

    /* Draw the maze */
    for (int i = 0; i < maze->size; i++) {
        int z = BOX_LEN * (-maze->size / 2 + i);
        for (int j = 0; j < maze->size; j++) {
            int x = BOX_LEN * (-maze->size / 2 + j);

            /* If this cell is a wall */
            if (maze->cells[i][j] == 1) {
                /* Draw wall */
                {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(x, ABOVE_GROUND, z));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(SCALE_BOX));

                    RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, SLATE_GREY);
                }
            }
        }
    }
}

/* Render healthbar hud */
void Tema2::drawHealthHud()
{
    /* Draw healthbar box */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::translate(modelMatrix, attackMode == ATTACK_MODE ?
            glm::vec3(-MOVE_FORWARD * glm::sin(hero->angle), 0, -MOVE_FORWARD * glm::cos(hero->angle)) : glm::vec3(0));
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_HEALTH_HUD);
        modelMatrix = glm::scale(modelMatrix, SCALE_HUD);
        modelMatrix = glm::translate(modelMatrix, TO_CENTER);

        RenderSimpleMesh(meshes["square"], shaders["HomeworkShader"], modelMatrix);
    }

    /* Draw current health in healthbar */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::translate(modelMatrix, attackMode == ATTACK_MODE ?
            glm::vec3(-MOVE_FORWARD * glm::sin(hero->angle), 0, -MOVE_FORWARD * glm::cos(hero->angle)) : glm::vec3(0));
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_HEALTH_HUD);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(SCALE_HUD.x * hero->health / HERO_HEALTH, SCALE_HUD.y, SCALE_HUD.z));
        modelMatrix = glm::translate(modelMatrix, TO_CENTER);

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, DARK_GREEN);
    }
}

/* Render time-left hud */
void Tema2::drawTimeHud()
{
    /* Draw time box */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::translate(modelMatrix, attackMode == ATTACK_MODE ?
            glm::vec3(-MOVE_FORWARD * glm::sin(hero->angle), 0, -MOVE_FORWARD * glm::cos(hero->angle)) : glm::vec3(0));
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_TIME_HUD);
        modelMatrix = glm::scale(modelMatrix, SCALE_HUD);
        modelMatrix = glm::translate(modelMatrix, TO_CENTER);

        RenderSimpleMesh(meshes["square"], shaders["HomeworkShader"], modelMatrix);
    }

    /* Draw time left in time box */
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, hero->coord);
        modelMatrix = glm::translate(modelMatrix, attackMode == ATTACK_MODE ?
            glm::vec3(-MOVE_FORWARD * glm::sin(hero->angle), 0, -MOVE_FORWARD * glm::cos(hero->angle)) : glm::vec3(0));
        modelMatrix = glm::rotate(modelMatrix, hero->angle, UP);
        modelMatrix = glm::translate(modelMatrix, TRANSLATE_TIME_HUD);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(SCALE_HUD.x * (1 - (Engine::GetElapsedTime() - timePassed) / levels[level].second),
                                                        SCALE_HUD.y, SCALE_HUD.z));
        modelMatrix = glm::translate(modelMatrix, TO_CENTER);

        RenderSimpleMesh(meshes["cube"], shaders["HomeworkShader"], modelMatrix, KAKHI);
    }
}

/* Render the minimap */
void Tema2::drawMinimap(float deltaTimeSeconds)
{
    drawHero(MINIMAP_SCALE);
    drawEnemies(deltaTimeSeconds, MINIMAP_SCALE);
    drawMaze();
}

/* Moving camera in first person mode */
void Tema2::moveCameraFirstToThirdAndBack()
{
    if (attackMode == ATTACK_MODE) {
        camera->MoveForward(MOVE_FORWARD);
    }
}

/* Verify if hero won */
void Tema2::WINNING()
{
    float out = BOX_LEN * (levels[level].first - 1) / 2 + BOX_LEN / 2;
    if (hero->coord.x < -out || hero->coord.x > out || hero->coord.z < -out || hero->coord.z > out) {
        cout << "You passed level " << level + 1 << "!" << endl;

        level++;
        if (level == LEVELS) {
            cout << "Congrats! You won the game!" << endl;
            exit(0);
        }
        else {
            RESETGAME();
        }
        
    }
}

/* Generating a new maze with new dimensions, reset hero position and delete projectilesw */
void Tema2::RESETGAME()
{
    maze = new Maze(levels[level].first);
    maze->generate();
    hero->reset();
    projectiles.clear();
    timePassed += Engine::GetElapsedTime() - timePassed;
}

/* Verify if the game ended */
void Tema2::GAMEOVER()
{
    /* Game ends if the hero is dead or if the time run out */
    if ((level < LEVELS && levels[level].second <= Engine::GetElapsedTime() - timePassed) || !hero->alive) {
        cout << "Game over!" << endl;
        exit(0);
    }
}

/* Checks if hero's new coordinates collide with any walls */
bool Tema2::checkHeroWallsCollision(float newX, float newZ)
{
    for (int i = 0; i < maze->size; i++) {
        int z = BOX_LEN * (-maze->size / 2 + i);
        for (int j = 0; j < maze->size; j++) {
            int x = BOX_LEN * (-maze->size / 2 + j);

            if (maze->cells[i][j] == 1 && collisions3d::CheckCollisionRR(GameObject3d(glm::vec3(newX, HUMAN_HEIGHT_2, newZ), HUMAN_SCALE),
                    GameObject3d(glm::vec3(x, ABOVE_GROUND, z), WALL_SCALE))) {
                return true;
            }
        }
    }

    return false;
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    /* Moving forward */
    if (window->KeyHold(GLFW_KEY_W)) {
        /* Compute new coordinates */
        float newZ = hero->coord.z - deltaTime * hero->speed * glm::cos(hero->angle);
        float newX = hero->coord.x + deltaTime * hero->speed * glm::sin(-hero->angle);
        /* Check if the new coordinates collide with a wall; if not, update camera and hero position */
        if (!checkHeroWallsCollision(newX, newZ)) {
            hero->coord.x = newX;
            hero->coord.z = newZ;
            camera->MoveForward(deltaTime);
        }
    }
    /* Moving backward */
    if (window->KeyHold(GLFW_KEY_S)) {
        /* Compute new coordinates */
        float newZ = hero->coord.z + deltaTime * hero->speed * glm::cos(hero->angle);
        float newX = hero->coord.x - deltaTime * hero->speed * glm::sin(-hero->angle);
        /* Check if the new coordinates collide with a wall; if not, update camera and hero position */
        if (!checkHeroWallsCollision(newX, newZ)) {
            hero->coord.x = newX;
            hero->coord.z = newZ;
            camera->MoveForward(-deltaTime);
        }
    }
    /* Moving left */
    if (window->KeyHold(GLFW_KEY_A)) {
        /* Compute new coordinates */
        float newX = hero->coord.x - deltaTime * hero->speed * glm::cos(hero->angle);
        float newZ = hero->coord.z + deltaTime * hero->speed * glm::sin(hero->angle);
        /* Check if the new coordinates collide with a wall; if not, update camera and hero position */
        if (!checkHeroWallsCollision(newX, newZ)) {
            hero->coord.x = newX;
            hero->coord.z = newZ;
            camera->TranslateRight(-deltaTime);
        }
    }
    /* Moving right */
    if (window->KeyHold(GLFW_KEY_D)) {
        /* Compute new coordinates */
        float newX = hero->coord.x + deltaTime * hero->speed * glm::cos(hero->angle);
        float newZ = hero->coord.z - deltaTime * hero->speed * glm::sin(hero->angle);
        /* Check if the new coordinates collide with a wall; if not, update camera and hero position */
        if (!checkHeroWallsCollision(newX, newZ)) {
            hero->coord.x = newX;
            hero->coord.z = newZ;
            camera->TranslateRight(deltaTime);
        }
    } 
}

void Tema2::OnKeyPress(int key, int mods)
{
    /* Switch between fill view, point viewand line view for polygons */
    if (key == GLFW_KEY_P) {
        switch (polygonMode) {
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

    /* Switch between walk mode and attack mode */
    if (key == GLFW_KEY_LEFT_CONTROL) {
        attackMode = attackMode == WALK_MODE? ATTACK_MODE : WALK_MODE;
    }

    /* If hero can shoot and is in attack mode */
    if (key == GLFW_KEY_SPACE && attackMode == ATTACK_MODE && (fireRate > FIRE_RATE || !anyAlive)) {
        projectiles.push_back(new Projectile3d(hero->coord, hero->angle));
        fireRate = 0;
    }

    /* Print a hint for the hero */
    if (key == GLFW_KEY_H) {
        cout << "Hero at: (" << hero->coord.x << ", " << hero->coord.z << ")" << endl << "Exits at: ";
        maze->printExits();
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    /* Rotate camera and hero, if player holds right click */
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivityOY = 0.001f;

        /* If hero attacks, move camera as first person */
        if (attackMode == ATTACK_MODE) {
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
        }
        /* Else, move camera as third person */
        else {
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }

        /* Update hero angle */
        hero->angle -= sensivityOY * deltaX;
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
