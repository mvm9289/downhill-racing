#include "Player.h"


Player::Player(Point c, float r) : Sphere(c, r)
{
}


Player::~Player(void)
{
}

void Player::move(float dx) {
	center += Point(dx, 0, 0);
}

Point Player::getPosition() {
	return center;
}