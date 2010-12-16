#include "Player.h"
#include "Terrain.h"
#include <cmath>

Player::Player(Point c, float r, bool comp): Sphere(Point(c.x, c.y + r, c.z), r), jumping(false), jumpAvailable(true), IA(comp) {
}

void Player::init() {
	speed = (SPEED_MAX + SPEED_MIN)/2.0;
	platform = 0;
	offsetZ = terrain->getPlatformLength(platform)/2.0;
	offsetX = center.x;
	offsetY = center.y + radius;
	blocked = false;
}

Player::~Player(void) {}

void Player::move(float dx, vector<Player*> &pl, int me) {
	if (!blocked) {
		offsetX += dx;
		if (offsetX < radius) offsetX = radius;
		if (offsetX > TERRAIN_WIDTH*SCALE_FACTOR - radius) offsetX = TERRAIN_WIDTH*SCALE_FACTOR - radius;
		computeCenter();
	}
	checkColisions(pl, me);
}

void Player::jump() {
	if (!jumping && jumpAvailable) {
		jumping = true;
		jumpAvailable = false;
		jumped = 0;
		speed = SPEED_MAX;
	}
}

Point Player::getPosition() {
	return center;
}

void Player::advance(vector<Player*> &pl, int me) {
	if (!blocked) {
		if (me != 0) doIA(pl, me);
		float advance = (radius)*speed;

		if (terrain->getDirection(platform).slopeYZ() > 0) speed = (speed + SPEED_MAX)/2.0 - (radius - 1)*0.3;
		else speed = (speed + SPEED_MIN)/2.0 - (radius - 1)*0.3;

		while (advance > 0) {
			if (advance < terrain->getPlatformLength(platform) - offsetZ) {
				offsetZ += advance;
				advance = 0;
			}
			else {
				advance -= offsetZ;
				offsetZ = 0;
				++platform;
			}
		}
	
		if (jumping) {
			if (jumped < JUMP_STEPS) {
				offsetY += JUMP_FACTOR*cos(jumped/JUMP_STEPS);
			}
			else if (jumped > JUMP_STEPS) {
				offsetY += JUMP_FACTOR*cos(jumped/JUMP_STEPS);
			}
			if (++jumped == 2*JUMP_STEPS) {
				//offsetY = 0;
				jumping = false;
			}
		}
		else {
			offsetY -= 1.5*PLAYER_STEP;
		}
		if (offsetY <= terrain->getPosition(platform, offsetZ).y + radius) {
			jumping = false;
			jumpAvailable = true;
			offsetY = terrain->getPosition(platform, offsetZ).y + radius;
		}
		computeCenter();
	}
	checkColisions(pl, me);
}

void Player::doIA(vector<Player*> &pl, int me) {
	int minD = (pl[0]->getPosition() - pl[me]->getPosition()).length();
	unsigned int closest;
}

void Player::setTerrain(Terrain *t) {
	terrain = t;
}

void Player::computeCenter() {
	center = terrain->getPosition(platform, offsetZ);
	center.x = offsetX;
	center.y = offsetY;
}

void Player::checkColisions(vector<Player*> &pl, int me) {
	for (unsigned int i = 0; i < pl.size(); ++i) {
		if (i != me) {
			if ((pl[i]->getPosition() - center).length() < pl[i]->radius + radius){
				blocked = true;
				return;
			}
		}
	}
	blocked = false;
}

bool Player::isJumping() {
	return jumping;
}