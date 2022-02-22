#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

/* Initialize all objects and constants */
void Tema3::Init()
{
    /* Cube mesh */
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    /* Sphere mesh */
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    /* Cone mesh */
    {
        Mesh* mesh = object2D::MakeCone("cone", CENTER, 1, 120, CONE_INIT_HEIGHT);
        meshes[mesh->GetMeshID()] = mesh;
    }

    /* Shader for the floor cells */
    {
        Shader* shader = new Shader("FloorShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FloorVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FloorFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    /* Shader for the wall cells */
    {
        Shader* shader = new Shader("WallShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "WallVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "WallFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    /* Shader for the dancers */
    {
        Shader* shader = new Shader("DancerShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "DancerVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "DancerFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    /* Shader for the spotlights */
    {
        Shader* shader = new Shader("SpotShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "SpotVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "SpotFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    /* Shader for the disco globe */
    {
        Shader* shader = new Shader("GlobeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "GlobeVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "GlobeFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    /* Creating the scene */
    createFloor();
    createWalls();
    createDancers();
    createGlobe();
    createCones();

    /* Setting the cut-off angle for the cones */
    cut_off = (float) 1 / CONE_MAX_HEIGHT;
}

/* For the beginning of each frame */
void Tema3::FrameStart()
{
    /* Clears the color buffer(using the previously set color) and depth buffer */
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    /* Sets the screen area where to draw */
    glViewport(0, 0, resolution.x, resolution.y);
}

/* Frame update */
void Tema3::Update(float deltaTimeSeconds)
{
    /* Drawing the scene */
    drawFloor();
    drawWalls();
    drawDancers(deltaTimeSeconds);
    drawGlobe(deltaTimeSeconds);
    drawCones(deltaTimeSeconds);
}

/* For the ending of each frame */
void Tema3::FrameEnd()
{
}

/* Mesh rendering for the floor cells and the cones */
void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture,
    glm::vec3 globePosition, float time, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    /* Render an object using the specified shaderand the specified position */
    glUseProgram(shader->program);

    /* Send uniforms only for the floor cells */
    if (strcmp(shader->GetName(), "FloorShader") == 0) {
        /* Emissive constant uniform */
        int material_ke = glGetUniformLocation(shader->program, "material_ke");
        glUniform1f(material_ke, KE_FLOOR);

        /* Spotlight uniforms: coordinates, directions, colors and cut-off angle */
        int _spotlight_positions = glGetUniformLocation(shader->program, "spotlight_positions");
        glUniform3fv(_spotlight_positions, CONES, glm::value_ptr(spotlight_positions[0]));

        int _spotlight_directions = glGetUniformLocation(shader->program, "spotlight_directions");
        glUniform3fv(_spotlight_directions, CONES, glm::value_ptr(spotlight_directions[0]));

        int _spotlight_colors = glGetUniformLocation(shader->program, "spotlight_colors");
        glUniform3fv(_spotlight_colors, CONES, glm::value_ptr(spotlight_colors[0]));

        int _cut_off = glGetUniformLocation(shader->program, "cut_off");
        glUniform1f(_cut_off, cut_off);

        /* Globe uniforms: texture, coordinates and time spent rotating */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        int loc_texture = glGetUniformLocation(shader->program, "texture");
        glUniform1i(loc_texture, 0);

        int _globe_position = glGetUniformLocation(shader->program, "globe_position");
        glUniform3fv(_globe_position, 1, glm::value_ptr(globePosition));

        float scale = .1f;
        int loc_int = glGetUniformLocation(shader->program, "time");
        glUniform1f(loc_int, scale * time);
    }

    /* Objcet color uniform */
    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    /* Bind model matrix */
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    /* Bind view matrix */
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    /* Bind projection matrix */
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    /* Draw the object */
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/* Mesh rendering for the wall cells and the dancers */
void Tema3::RenderComplexMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
    vector<glm::vec3> light_positions, vector<glm::vec3> light_colors, Texture2D* texture, glm::vec3 globePosition, float time)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    /* Render an object using the specified shaderand the specified position */
    glUseProgram(shader->program);

    /* Decide between rendering a wall cell and a dancer */
    int contributions = strcmp(shader->GetName(), "WallShader") == 0 ? WALL_CONTRIBUTIONS : DANCER_CONTRIBUTIONS;

    /* Dance-floor uniforms: coordinates, colors and light radius */
    int _light_positions = glGetUniformLocation(shader->program, "light_positions");
    glUniform3fv(_light_positions, contributions, glm::value_ptr(light_positions[0]));

    int _light_colors = glGetUniformLocation(shader->program, "light_colors");
    glUniform3fv(_light_colors, contributions, glm::value_ptr(light_colors[0]));

    int _light_radius = glGetUniformLocation(shader->program, "light_radius");
    glUniform1f(_light_radius, LIGHT_RADIUS);

    /* Spotlight uniforms: coordinates, directions, colors and cut-off angle */
    int _spotlight_positions = glGetUniformLocation(shader->program, "spotlight_positions");
    glUniform3fv(_spotlight_positions, CONES, glm::value_ptr(spotlight_positions[0]));

    int _spotlight_directions = glGetUniformLocation(shader->program, "spotlight_directions");
    glUniform3fv(_spotlight_directions, CONES, glm::value_ptr(spotlight_directions[0]));

    int _spotlight_colors = glGetUniformLocation(shader->program, "spotlight_colors");
    glUniform3fv(_spotlight_colors, CONES, glm::value_ptr(spotlight_colors[0]));

    int _cut_off = glGetUniformLocation(shader->program, "cut_off");
    glUniform1f(_cut_off, cut_off);

    /* Globe uniforms: texture, coordinates and time spent rotating */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    int loc_texture = glGetUniformLocation(shader->program, "texture");
    glUniform1i(loc_texture, 0);

    int _globe_position = glGetUniformLocation(shader->program, "globe_position");
    glUniform3fv(_globe_position, 1, glm::value_ptr(globePosition));

    float scale = .1f;
    int loc_int = glGetUniformLocation(shader->program, "time");
    glUniform1f(loc_int, scale * time);

    /* Set eye position(camera position) uniform */
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(eye_position, 1, glm::value_ptr(eyePosition));

    /* Set material property uniforms(shininess, kd, ks) */
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, SHININESS_WALL);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, KD_WALL);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, KS_WALL);

    /* Bind model matrix */
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    /* Bind view matrix */
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    /* Bind projection matrix */
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    /* Draw the object */
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/* Mesh rendering for the globe */
void Tema3::RenderGlobeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, float time)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    /* Render an object using the specified shaderand the specified position */
    glUseProgram(shader->program);

    /* Globe uniforms: texture and time spent rotating */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    int loc_texture = glGetUniformLocation(shader->program, "texture");
    glUniform1i(loc_texture, 0);

    float scale = .1f;
    int loc_int = glGetUniformLocation(shader->program, "time");
    glUniform1f(loc_int, scale * time);

    /* Bind model matrix */
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    /* Bind view matrix */
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    /* Bind projection matrix */
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    /* Draw the object */
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/* Texture generator for the globe - inspired from lab9 */
Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    for (int i = 0; i < size; i++) {
        data[i] = (unsigned char)rand() % 256;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

/* Dance-floor grid generator */
void Tema3::createFloor()
{
    for (int i = 0; i < DISCO_LEN; i++) {
        for (int j = 0; j < DISCO_LEN; j++) {
            /* Center of the cell based on (i, j) */
            float z = DISCO_START_Z - i - HALF;
            float x = DISCO_START_X + j + HALF;

            /* Random color */
            float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

            floor[i][j] = new FloorCell(glm::vec3(x, 0, z), glm::vec3(r, g, b), glm::vec3(1, THICKNESS, 1));
        }
    }
}

/* Walls generator */
void Tema3::createWalls()
{
    /* Ceiling generator */
    for (int i = 0; i < DISCO_LEN; i++) {
        for (int j = 0; j < DISCO_LEN; j++) {
            float x = DISCO_START_X + j + HALF;
            float z = DISCO_START_Z - i - HALF;
            ceil[i][j] = new FloorCell(glm::vec3(x, WALL_LEN, z), glm::vec3(1, THICKNESS, 1));
        }
    }

    /* Side walls generator */
    for (int i = 0; i < DISCO_LEN; i++) {
        for (int j = 0; j < WALL_LEN; j++) {
            float x = DISCO_START_X + i + HALF;
            float y = j + HALF;
            float z = DISCO_START_Z - i - HALF;

            /* Get the list of floor cells that light up the wall */
            vector<glm::vec3> left_light_positions, left_light_colors;
            left_light_positions.push_back(j == 0 ? floor[i][FIRST]->coord : OUTSIDE);
            left_light_positions.push_back(j == 0 && i != FIRST ? floor[i - 1][FIRST]->coord : OUTSIDE);
            left_light_positions.push_back(j == 0 && i != LAST ? floor[i + 1][FIRST]->coord : OUTSIDE);
            left_light_colors.push_back(j == 0 ? floor[i][FIRST]->color : BLACK);
            left_light_colors.push_back(j == 0 && i != FIRST ? floor[i - 1][FIRST]->color : BLACK);
            left_light_colors.push_back(j == 0 && i != LAST ? floor[i + 1][FIRST]->color : BLACK);

            vector<glm::vec3> right_light_positions, right_light_colors;
            right_light_positions.push_back(j == 0 ? floor[i][LAST]->coord : OUTSIDE);
            right_light_positions.push_back(j == 0 && i != FIRST ? floor[i - 1][LAST]->coord : OUTSIDE);
            right_light_positions.push_back(j == 0 && i != LAST ? floor[i + 1][LAST]->coord : OUTSIDE);
            right_light_colors.push_back(j == 0 ? floor[i][LAST]->color : BLACK);
            right_light_colors.push_back(j == 0 && i != FIRST ? floor[i - 1][LAST]->color : BLACK);
            right_light_colors.push_back(j == 0 && i != LAST ? floor[i + 1][LAST]->color : BLACK);

            vector<glm::vec3> back_light_positions, back_light_colors;
            back_light_positions.push_back(j == 0 ? floor[LAST][i]->coord : OUTSIDE);
            back_light_positions.push_back(j == 0 && i != FIRST ? floor[LAST][i - 1]->coord : OUTSIDE);
            back_light_positions.push_back(j == 0 && i != LAST ? floor[LAST][i + 1]->coord : OUTSIDE);
            back_light_colors.push_back(j == 0 ? floor[LAST][i]->color : BLACK);
            back_light_colors.push_back(j == 0 && i != FIRST ? floor[LAST][i - 1]->color : BLACK);
            back_light_colors.push_back(j == 0 && i != LAST ? floor[LAST][i + 1]->color : BLACK);

            left_wall[i][j] = new WallCell(glm::vec3(DISCO_START_X, y, z), glm::vec3(THICKNESS, 1, 1),
                left_light_positions, left_light_colors);
            right_wall[i][j] = new WallCell(glm::vec3(DISCO_START_X + DISCO_LEN, y, z), glm::vec3(THICKNESS, 1, 1),
                right_light_positions, right_light_colors);
            back_wall[i][j] = new WallCell(glm::vec3(x, y, DISCO_START_Z - DISCO_LEN), glm::vec3(1, 1, THICKNESS),
                back_light_positions, back_light_colors);
        }
    }
}

/* Dancers generator */
void Tema3::createDancers()
{
    for (int i = 0; i < PEOPLE; i++) {
        Dancer* dancer = new Dancer();
        dancer->create();
        dancers.push_back(dancer);
    }
}

/* Globe generator */
void Tema3::createGlobe()
{
    globe = new Globe(glm::vec3(DISCO_START_X + DISCO_LEN / 2, WALL_LEN - HALF, DISCO_START_Z - DISCO_LEN / 2),
        CreateRandomTexture(GLOBE_PIXELS, GLOBE_PIXELS));
}

/* Spotlights generator */
void Tema3::createCones()
{
    cones.push_back(new Cone(glm::vec3(DISCO_START_X + 2, CONE_HEIGHT, DISCO_START_Z - 2), RED, 0.18f));
    cones.push_back(new Cone(glm::vec3(DISCO_START_X + 2, CONE_HEIGHT, DISCO_START_Z - 6), YELLOW, 0.15f));
    cones.push_back(new Cone(glm::vec3(DISCO_START_X + 6, CONE_HEIGHT, DISCO_START_Z - 2), DEEP_SKY_BLUE, 0.1f));
    cones.push_back(new Cone(glm::vec3(DISCO_START_X + 6, CONE_HEIGHT, DISCO_START_Z - 6), LIME_GREEN, 0.14f));

    for (int i = 0; i < CONES; i++) {
        spotlight_positions.push_back(cones[i]->coord);
        spotlight_directions.push_back(LIGHT_DIRECTION); 
        spotlight_colors.push_back(cones[i]->color);
    }
}

/* Dance-floor cells drawer */
void Tema3::drawFloor()
{
    for (int i = 0; i < DISCO_LEN; i++) {
        for (int j = 0; j < DISCO_LEN; j++) {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, floor[i][j]->coord);
                modelMatrix = glm::scale(modelMatrix, floor[i][j]->scale);
                RenderSimpleMesh(meshes["box"], shaders["FloorShader"], modelMatrix,
                    globe->texture, globe->coord, globe->time, floor[i][j]->color);
            }
        }
    }
}

/* Wall cells drawer */
void Tema3::drawWalls()
{
    /* Ceiling drawer */
    for (int i = 0; i < DISCO_LEN; i++) {
        for (int j = 0; j < DISCO_LEN; j++) {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, ceil[i][j]->coord);
                modelMatrix = glm::scale(modelMatrix, ceil[i][j]->scale);
                RenderSimpleMesh(meshes["box"], shaders["FloorShader"], modelMatrix, globe->texture, globe->coord, globe->time);
            }
        }
    }

    /* Side walls drawer */
    for (int i = 0; i < DISCO_LEN; i++) {
        for (int j = 0; j < WALL_LEN; j++) {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, left_wall[i][j]->coord);
                modelMatrix = glm::scale(modelMatrix, left_wall[i][j]->scale);
                RenderComplexMesh(meshes["box"], shaders["WallShader"], modelMatrix,
                    left_wall[i][j]->light_positions, left_wall[i][j]->light_colors, globe->texture, globe->coord, globe->time);
            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, right_wall[i][j]->coord);
                modelMatrix = glm::scale(modelMatrix, right_wall[i][j]->scale);
                RenderComplexMesh(meshes["box"], shaders["WallShader"], modelMatrix,
                    right_wall[i][j]->light_positions, right_wall[i][j]->light_colors, globe->texture, globe->coord, globe->time);
            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, back_wall[i][j]->coord);
                modelMatrix = glm::scale(modelMatrix, back_wall[i][j]->scale);
                RenderComplexMesh(meshes["box"], shaders["WallShader"], modelMatrix,
                    back_wall[i][j]->light_positions, back_wall[i][j]->light_colors, globe->texture, globe->coord, globe->time);
            }
        }
    }
}

/* Dancers drawer */
void Tema3::drawDancers(float deltaTimeSeconds)
{
    for (int c = 0; c < PEOPLE; c++) {
        /* Get the dance-floor cell the dancer stays on at the moment */
        dancers[c]->getSquare();

        /* Get the list of floor cells that light up the dancer */
        vector<glm::vec3> dancer_light_positions, dancer_light_colors;
        for (int u = -1; u <= 1; u++) {
            for (int v = -1; v <= 1; v++) {
                dancer_light_positions.push_back(floor[dancers[c]->i + u][dancers[c]->j + v]->coord);
                dancer_light_colors.push_back(floor[dancers[c]->i + u][dancers[c]->j + v]->color);
            }
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, dancers[c]->coord);
            modelMatrix = glm::scale(modelMatrix, dancers[c]->scale);
            RenderComplexMesh(meshes["box"], shaders["DancerShader"], modelMatrix,
                dancer_light_positions, dancer_light_colors, globe->texture, globe->coord, globe->time);
        }

        /* Move the dancer */
        dancers[c]->move(deltaTimeSeconds);
    }
}

/* Globe drawer */
void Tema3::drawGlobe(float deltaTimeSeconds)
{
    globe->update(deltaTimeSeconds);

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, globe->coord);
        RenderGlobeMesh(meshes["sphere"], shaders["GlobeShader"], modelMatrix, globe->texture, globe->time);
    }
}

/* Cones drawer */
void Tema3::drawCones(float deltaTimeSeconds)
{
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    spotlight_directions.clear();
    for (int i = 0; i < CONES; i++) {
        /* Update the cone-rotation angles */
        cones[i]->update(deltaTimeSeconds);
        /* Get the light direction at the moment */
        glm::vec3 lightDirection = glm::vec3(glm::rotate(glm::mat4(1), cones[i]->rx, Ox)
            * glm::rotate(glm::mat4(1), cones[i]->rz, Oz) * glm::vec4(LIGHT_DIRECTION, 1));
        spotlight_directions.push_back(lightDirection);

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, cones[i]->coord);
            modelMatrix = glm::rotate(modelMatrix, cones[i]->rx, Ox);
            modelMatrix = glm::rotate(modelMatrix, cones[i]->rz, Oz);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(CONE_MAX_HEIGHT * tan(cut_off),
                CONE_MAX_HEIGHT, CONE_MAX_HEIGHT * tan(cut_off)));
            RenderSimpleMesh(meshes["cone"], shaders["SpotShader"], modelMatrix,
                globe->texture, globe->coord, globe->time, cones[i]->color);
        }
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
