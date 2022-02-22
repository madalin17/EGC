#include "lab_m1/Tema1/pickup.h"

using namespace m1;

PickUp::PickUp(float x, float y, PickUpType type, bool alive)
{
	this->x = x;
	this->y = y;
	this->type = type;
	this->alive = alive;
}

PickUp::~PickUp()
{
}