#include "lab_m1/Tema1/enemy.h"

using namespace m1;

Enemy::Enemy(float x, float y, float speed, float angle, bool alive)
{
	this->x = x;
	this->y = y;
	this->speed = speed;
	this->angle = angle;
	this->alive = alive;
}

Enemy::~Enemy()
{
}