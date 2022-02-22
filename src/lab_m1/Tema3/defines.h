#pragma once

/* Wall cells constants */
#define THICKNESS 0.01f
#define WALL_LEN 5

/* Floor cells constants*/
#define FIRST 0
#define LAST 7
#define DISCO_START_X -4
#define DISCO_START_Z 0
#define DISCO_LEN 8

/* Uniforms */
#define KE_FLOOR 0.55f
#define KD_WALL 0.5f
#define KS_WALL 0.5f
#define SHININESS_WALL 30
#define KD_DANCER 0.6f
#define KS_DANCER 0.7f
#define SHININESS_DANCER 20
#define LIGHT_RADIUS 0.75f

/* Random vector defines */
#define IMPLICIT glm::vec3(1)
#define OUTSIDE glm::vec3(-100)

/* Light contributions */
#define WALL_CONTRIBUTIONS 3
#define DANCER_CONTRIBUTIONS 9

/* Dancer constants */
#define SPEED 1.5f
#define HALF 0.5f
#define HEIGHT 1.5f
#define PEOPLE 9
#define ERR 0.5f

/* Cones constants */
#define CONES 4
#define CENTER glm::vec3(0)
#define CONE_INIT_HEIGHT 1
#define CONE_HEIGHT 5
#define CONE_MAX_HEIGHT 5.5f
#define MAX_ROTATE_OX 0.07f
#define MAX_ROTATE_OZ 0.3f
#define LIGHT_DIRECTION glm::vec3(0, -1, 0)

/* Axis constants */
#define Ox glm::vec3(1, 0, 0)
#define Oz glm::vec3(0, 0, -1)

/* Globe constants */
#define GLOBE_PIXELS 16