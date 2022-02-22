#include "lab_m1/Tema1/hero.h"

using namespace m1;

Hero::Hero()
{
	this->x = 0;
	this->y = 0;
	this->score = 0;
	this->health = HERO_HEALTH;
	this->alive = true;
}

Hero::Hero(float x, float y, int score, int health, bool alive)
{
	this->x = x;
	this->y = y;
	this->score = score;
	this->health = health;
	this->alive = alive;
}

Hero::~Hero()
{
}