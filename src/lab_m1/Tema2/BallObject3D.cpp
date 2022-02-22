#include "lab_m1/Tema2/BallObject3D.h"

using namespace m1;

BallObject3d::BallObject3d(glm::vec3 Position, float Radius)
{
	this->Position = Position;
	this->Radius = Radius;
}

BallObject3d::~BallObject3d()
{
}