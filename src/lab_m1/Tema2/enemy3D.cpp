#include "lab_m1/Tema2/enemy3D.h"

using namespace m1;
using namespace std;

/* Setting up a basic enemy */
Enemy3d::Enemy3d(glm::vec3 coord, float speed, float angle, float time_to_die, bool deformed, bool alive)
{
	this->coord = coord;
	this->speed = speed;
	this->angle = angle;
	this->deformed = deformed;
	this->time_to_die = time_to_die;
	this->alive = alive;
	this->movePattern = glm::vec3(0, 0, 0);
	this->boss = false;
	this->lives = 1;
}

Enemy3d::~Enemy3d()
{
}

/* Moving an enemy inside the box in a 8-shape */
void Enemy3d::move(float deltaTimeSeconds)
{
	angle += speed * deltaTimeSeconds;
	angle -= angle > 3 * M_PI_2 ? 2 * M_PI : 0;

	movePattern = MOVE_IN_BOX * glm::vec3(glm::cos(angle), 0, glm::cos(angle) * glm::sin(angle));
}

/* Setting up boss characters */
void Enemy3d::isBoss(float angle)
{
	this->boss = true;
	this->lives = BOSS_ENEMY_HP;
	this->projectile = new Projectile3d(coord + movePattern, angle);
}

/* Reset position and time for boss enemy's projectile */
void Enemy3d::resetProjectile()
{
	this->projectile->coord = coord + movePattern;
	this->projectile->time = 0;
}