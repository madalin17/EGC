#include "lab_m1/Tema1/GameObject.h"

using namespace m1;

GameObject::GameObject(glm::vec2 Position, glm::vec2 Size)
{
	this->Position = Position;
	this->Size = Size;
}

GameObject::~GameObject()
{
}