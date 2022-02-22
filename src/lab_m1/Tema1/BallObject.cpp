#include "lab_m1/Tema1/BallObject.h"

using namespace m1;

BallObject::BallObject(glm::vec2 Position, float Radius)
{
	this->Position = Position;
	this->Radius = Radius;
}

BallObject::~BallObject()
{
}