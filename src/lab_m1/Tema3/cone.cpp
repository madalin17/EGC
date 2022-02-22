#include "cone.h"

using namespace m1;

/* Updating funtion for the rotation angles on Ox and 0z for the cone */
/* rx in (-MAX_ROTATE_OX, MAX_ROTATE_OX), rz in (MAX_ROTATE_OZ, MAX_ROTATE_OX) */
void Cone::update(float deltaTimeSeconds)
{
	rx += signX * speed * deltaTimeSeconds;
	rz += signZ * speed * deltaTimeSeconds;

	if (rx >= MAX_ROTATE_OX) {
		signX = -1;
	}
	else if (rx <= -MAX_ROTATE_OX) {
		signX = 1;
	}

	if (rz >= MAX_ROTATE_OZ) {
		signZ = -1;
	}
	else if (rz <= -MAX_ROTATE_OZ) {
		signZ = 1;
	}
}
