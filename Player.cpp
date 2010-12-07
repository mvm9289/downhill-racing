#include "Player.h"
#include <cmath>

Player::Player(Point c, float r) : Sphere(c, r)
{
	jumping = false;
}


Player::~Player(void)
{
}

void Player::move(float dx) {
	center += Point(dx, 0, 0);
	if (center.x < radius) center.x = radius;
	if (center.x > 10 - radius) center.x = 10 - radius;
}

void Player::jump() {
	if (!jumping) {
		jumping = true;
		jumped = JUMP_STEPS;
	}
}

Point Player::getPosition() {
	return center;
}

void Player::advance() {
	center += Point(0, 0, -PLAYER_STEP);
	if (jumping) {
		if (jumped > 0) {
			center += Point(0, radius*cos(jumped/JUMP_STEPS), 0);
		}
		else if (jumped < 0) {
			center += Point(0, -radius*cos(jumped/JUMP_STEPS), 0);
		}
		if (--jumped == -JUMP_STEPS) jumping = false;
	}
}