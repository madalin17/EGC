#include "lab_m1/Tema2/hero3D.h"

using namespace m1;

Hero3d::Hero3d(glm::vec3 coord, float angle, float speed, int health, bool alive)
{
	this->coord = coord;
	this->angle = angle;
	this->speed = speed;
	this->health = health;
	this->alive = alive;
}

Hero3d::~Hero3d()
{
}

/* Reset hero for a new level */
void Hero3d::reset()
{
	this->coord = glm::vec3(0);
	this->angle = 0;
	this->alive = true;
}