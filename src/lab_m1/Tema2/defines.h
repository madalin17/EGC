#pragma once

/* Hero dimensions */
#define HEAD_LENGTH 0.15f
#define BODY_HEIGHT 0.4f
#define BODY_LENGTH 0.15f
#define ARM_HEIGHT 0.25f
#define ARM_LENGTH 0.10f
#define HAND_LENGTH 0.10f
#define LEG_HEIGHT 0.3f
#define LEG_LENGTH 0.15f
#define LEG_WIDTH 0.06f

/* Enemy dimensions */
#define ENEMY_HEAD_LENGTH 0.15f
#define ENEMY_BODY_HEIGHT 0.4f
#define ENEMY_BODY_LENGTH 0.15f
#define ENEMY_ARM_HEIGHT 0.25f
#define ENEMY_ARM_LENGTH 0.10f
#define ENEMY_LEG_HEIGHT 0.3f
#define ENEMY_LEG_LENGTH 0.15f

/* Projectile dimensions */
#define PROJECTILE_LENGTH 0.14f
#define PROJECTILE_HEIGHT 0.5f

/* Hero starting coordinates */
#define INIT_COORDS glm::vec3(0, 0, 0)

/* Camera set-up vectors */
#define CAMERA_HEIGHT 0.8f
#define LOOK_FROM_ABOVE 2.0f
#define IN_THE_BACK 1.4f
#define LOOK_AT glm::vec3(0, CAMERA_HEIGHT, 0)
#define CAMERA_COORDS glm::vec3(0, CAMERA_HEIGHT, IN_THE_BACK)
#define UP glm::vec3(0, 1, 0)
#define FORWARD glm::vec3(0, 0, -1)

/* Maze drawing constants */
#define BOX_LEN 3.0f
#define MAP_CORNER 15

/* Hero parts scaling and translating vectors */
#define TRANSLATE_HEAD glm::vec3(0, HEAD_LENGTH / 2 + BODY_HEIGHT + LEG_HEIGHT, 0)
#define SCALE_HEAD glm::vec3(HEAD_LENGTH, HEAD_LENGTH, HEAD_LENGTH)
#define TRANSLATE_BODY glm::vec3(0, BODY_HEIGHT / 2 + LEG_HEIGHT, 0)
#define SCALE_BODY glm::vec3(BODY_LENGTH, BODY_HEIGHT, BODY_LENGTH)
#define TRANSLATE_LEFT_ARM glm::vec3(-BODY_LENGTH / 2 - ARM_LENGTH / 2, BODY_HEIGHT / 2 + LEG_HEIGHT, 0)
#define TRANSLATE_RIGHT_ARM glm::vec3(BODY_LENGTH / 2 + ARM_LENGTH / 2, BODY_HEIGHT / 2 + LEG_HEIGHT, 0)
#define SCALE_ARM glm::vec3(ARM_LENGTH, ARM_HEIGHT, ARM_LENGTH)
#define TRANSLATE_LEFT_HAND glm::vec3(-BODY_LENGTH / 2 - ARM_LENGTH / 2, HAND_LENGTH / 2 + LEG_HEIGHT, 0)
#define TRANSLATE_RIGHT_HAND glm::vec3(BODY_LENGTH / 2 + ARM_LENGTH / 2, HAND_LENGTH / 2 + LEG_HEIGHT, 0)
#define SCALE_HAND glm::vec3(HAND_LENGTH, HAND_LENGTH, HAND_LENGTH)
#define TRANSLATE_LEFT_LEG glm::vec3(-LEG_LENGTH / 2 + LEG_WIDTH / 2, LEG_HEIGHT / 2, 0)
#define TRANSLATE_RIGHT_LEG glm::vec3(LEG_LENGTH / 2 - LEG_WIDTH / 2, LEG_HEIGHT / 2, 0)
#define SCALE_LEG glm::vec3(LEG_WIDTH, LEG_HEIGHT, LEG_LENGTH)

/* Enemy parts scaling and translating vectors */
#define ENEMY_TRANSLATE_HEAD glm::vec3(0, ENEMY_HEAD_LENGTH / 2 + ENEMY_BODY_HEIGHT + ENEMY_LEG_HEIGHT, 0)
#define ENEMY_SCALE_HEAD glm::vec3(ENEMY_HEAD_LENGTH, ENEMY_HEAD_LENGTH, ENEMY_HEAD_LENGTH)
#define ENEMY_TRANSLATE_BODY glm::vec3(0, ENEMY_BODY_HEIGHT / 2 + ENEMY_LEG_HEIGHT, 0)
#define ENEMY_SCALE_BODY glm::vec3(ENEMY_BODY_LENGTH, ENEMY_BODY_HEIGHT, ENEMY_BODY_LENGTH)
#define ENEMY_TRANSLATE_LEFT_ARM glm::vec3(-ENEMY_BODY_LENGTH / 2 - ENEMY_ARM_LENGTH / 2, ENEMY_BODY_HEIGHT / 2 + ENEMY_LEG_HEIGHT, 0)
#define ENEMY_TRANSLATE_RIGHT_ARM glm::vec3(ENEMY_BODY_LENGTH / 2 + ENEMY_ARM_LENGTH / 2, ENEMY_BODY_HEIGHT / 2 + ENEMY_LEG_HEIGHT, 0)
#define ENEMY_SCALE_ARM glm::vec3(ENEMY_ARM_LENGTH, ENEMY_ARM_HEIGHT, ENEMY_ARM_LENGTH)
#define ENEMY_TRANSLATE_LEG glm::vec3(0, ENEMY_LEG_HEIGHT / 2, 0)
#define ENEMY_SCALE_LEG glm::vec3(ENEMY_LEG_LENGTH, ENEMY_LEG_HEIGHT, ENEMY_LEG_LENGTH)

/* Floor scaling */
#define SCALE_FLOOR glm::vec3(BOX_LEN * MAP_CORNER, 0.01f, BOX_LEN * MAP_CORNER)

/* Wall scaling */
#define SCALE_BOX glm::vec3(BOX_LEN, 1, BOX_LEN)

/* Projectile scaling and translating vectors */
#define TRANLATE_PROJECTILE glm::vec3(0, PROJECTILE_HEIGHT, 0)
#define SCALE_PROJECTILE glm::vec3(PROJECTILE_LENGTH, PROJECTILE_LENGTH, PROJECTILE_LENGTH)

/* Hero health constants */
#define HERO_HEALTH 1000
#define HEALTH_DECAY 200
#define HERO_SPEED 1.5f

/* Enemy constants */
#define ENEMY_FACTOR 10
#define MINIMUM_SPEED 1.5f
#define MOVE_IN_BOX 1.2f
/* Stop deforming enemy */
#define STOP_DEFORMING 3.0f
/* Boss enemy constants */
#define BOSS_ENEMY_HP 3
#define BOSS_ENEMY_SCALE 2.25f
#define BOSS_ENEMY_SPEED 3.0f

/* Projectiles constants */
#define FIRE_RATE 0.7f
#define DEAD_PROJECTILE 2.0f
#define PROJECTILE_SPEED 1.5f

/* Wall constants(to be above ground) */
#define ABOVE_GROUND 0.5f

/* Exits constants */
#define EXITS_FACTOR 15

/* Hud constans */
#define TO_CENTER glm::vec3(0.5f, 0.5f, 0)
#define TRANSLATE_HEALTH_HUD glm::vec3(0.3f, 1.5f, 0)
#define TRANSLATE_TIME_HUD glm::vec3(-0.9f, 1.5f, 0)
#define SCALE_HUD glm::vec3(0.6f, 0.1f, 0.001f)

/* First-third person camera */
#define WALK_MODE 0
#define ATTACK_MODE 1
#define MOVE_FORWARD 1.6f

/* Collisions */
#define HUMAN_HEIGHT_2 (HEAD_LENGTH + BODY_HEIGHT + LEG_HEIGHT) / 2
#define HUMAN_SCALE glm::vec3(BODY_LENGTH / 2 + HAND_LENGTH, HUMAN_HEIGHT_2, BODY_LENGTH / 2 + HAND_LENGTH)
#define WALL_SCALE glm::vec3(BOX_LEN / 2, ABOVE_GROUND, BOX_LEN / 2)

/* Time to finish the maze */
#define TIME1 180.0f
#define TIME2 150.0f
#define TIME3 120.0f

/* Scales for map/minimap scale */
#define MAP_SCALE 1
#define MINIMAP_SCALE 5

/* Number of levels */
#define LEVELS 3
