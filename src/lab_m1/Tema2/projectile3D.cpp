#include "lab_m1/Tema2/projectile3D.h"

using namespace m1;

Projectile3d::Projectile3d(glm::vec3 coord, float angle, float speed, float time, bool alive)
{
	this->coord = coord;
	this->angle = angle;
	this->speed = speed;
	this->time = time;
	this->alive = alive;
}

Projectile3d::~Projectile3d()
{
}

/* Adjusting projectile coordinates and time  */
void Projectile3d::move(float deltaTimeSeconds)
{
	this->coord.x += this->speed * deltaTimeSeconds * glm::sin(-this->angle);
	this->coord.z -= this->speed * deltaTimeSeconds * glm::cos(this->angle);

	this->time += deltaTimeSeconds;
}
