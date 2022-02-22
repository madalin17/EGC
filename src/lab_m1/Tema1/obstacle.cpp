#include "lab_m1/Tema1/obstacle.h"

using namespace m1;

Obstacle::Obstacle(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

Obstacle::~Obstacle()
{
}