#include "dancer.h"

using namespace m1;

/* Dancer generator - the dancer does't spawn on the sides of the dance-floor or ever go there*/
void Dancer::create()
{
	float tr_x = (DISCO_LEN - 2) * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
	float tr_z = (DISCO_LEN - 2) * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

	float x = (DISCO_START_X + 1) + tr_x;
	float z = (DISCO_START_Z - 1) - tr_z;

	this->coord = glm::vec3(x, HALF, z);
	this->scale = glm::vec3(HALF, HEIGHT, HALF);
	this->follow = this->coord;
	this->getSquare();
}

/* Set up a new target for the dancer to follow, if the last one is too close */
void Dancer::updateTarget()
{
	if (glm::distance(this->coord, this->follow) < ERR) {
		/* The target doesn't spawn on the sides */
		float tr_x = (DISCO_LEN - 2) * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		float tr_z = (DISCO_LEN - 2) * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

		float x = (DISCO_START_X + 1) + tr_x;
		float z = (DISCO_START_Z - 1) - tr_z;
		this->follow = glm::vec3(x, HALF, z);
	}
}

/* Verify if a dancer stays on the sides of the dance floor */
bool Dancer::outside()
{
	getSquare();
	if (this->i <= FIRST || this->i >= LAST || this->j <= FIRST || this->j >= LAST) {
		return true;
	}

	return false;
}

/* Moking the dancer follow the target */
void Dancer::move(float deltaTimeSeconds)
{
	this->updateTarget();
	float angle = glm::atan(this->follow.z - this->coord.z, this->follow.x - this->coord.x);
	glm::vec3 moving = SPEED * deltaTimeSeconds * glm::vec3(glm::cos(angle), 0, glm::sin(angle));
	this->coord += moving;

	/* If the dancer is on the sides of the dance floor after moving, revert the move and create another target */
	if (outside()) {
		this->create();
	}
}

/* Get the dance-floor cell the dancer stays on at the moment */
void Dancer::getSquare()
{
	this->j = (int) floor(this->coord.x - DISCO_START_X);
	this->i = (int) floor(abs(this->coord.z - DISCO_START_Z));
}
