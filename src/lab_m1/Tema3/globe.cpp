#include "globe.h"

using namespace m1;

/* Updating globe's time alive */
void Globe::update(float deltaTimeSeconds)
{
	this->time += deltaTimeSeconds;
}
