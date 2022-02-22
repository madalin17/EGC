#include "lab_m1/Tema2/GameObject3D.h"

using namespace m1;

GameObject3d::GameObject3d(glm::vec3 Position, glm::vec3 Size)
{
	this->Position = Position;
	this->Size = Size;
}

GameObject3d::~GameObject3d()
{
}