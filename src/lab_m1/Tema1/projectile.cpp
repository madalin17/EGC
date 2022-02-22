#include "lab_m1/Tema1/projectile.h"

using namespace m1;

Projectile::Projectile(float x, float y, float angle, float speed, float time, bool alive)
{
	this->x = x;
	this->y = y;
	this->angle = angle;
	this->speed = speed;
	this->time = time;
	this->alive = alive;
}

Projectile::~Projectile()
{
}