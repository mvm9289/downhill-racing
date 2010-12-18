#include "Player.h"
#include "Terrain.h"
#include <cmath>

#include <iostream>
using namespace std;

Player::Player(unsigned int id, Point c, float r, bool comp): playerID(id), Sphere(Point(c.x, c.y + r, c.z), r) {
}

void Player::init() {
	speed = SPEED_MIN;
	platform = 0;
	offsetZ = terrain->getPlatformLength(platform)/2.0;
	offsetX = center.x;
	offsetY = center.y + radius;
	computeCenter();
	turboLeft = 0;
	turboWait = 0;
	jumping =false;
	jumpAvailable = true;
	blocked = false;
}

Player::~Player(void) {}

void Player::move(float dx, vector<Player*> &pl) {
	if (!blocked) {
		offsetX += dx;
		if (offsetX < radius) offsetX = radius;
		if (offsetX > TERRAIN_WIDTH*SCALE_FACTOR - radius) offsetX = TERRAIN_WIDTH*SCALE_FACTOR - radius;
		computeCenter();
	}
	//checkColisions(pl);
}

void Player::jump() {
	if (!jumping && jumpAvailable) {
		jumping = true;
		jumpAvailable = false;
		jumped = 0;
	}
}

Point Player::getPosition() {
	return center;
}

void Player::advance(vector<Player*> &pl) {
	if (!blocked) {
		float advance = (float)speed*PLAYER_STEP;
		if (turboLeft) advance += 2.0*PLAYER_STEP;

		if (terrain->getDirection(platform).slopeYZ() < 0) {
			if (++speed > SPEED_MAX) speed = SPEED_MAX;
		}
		else {
			if (--speed < SPEED_MIN) speed = SPEED_MIN;
		}

		if (playerID == 0) cout << "Speed: " << speed << " Platform: " << platform << " Offset: " << offsetZ << endl;

		if (turboLeft) {
			--turboLeft;
			if (!turboLeft) turboWait = TURBO_TIME;
		}
		else if (turboWait) {
			--turboWait;
		}

		offsetZ += advance;
		while (offsetZ > terrain->getPlatformLength(platform)) {
			offsetZ -= terrain->getPlatformLength(platform);
			++platform;
		}
	
		if (jumping) {
			if (jumped < JUMP_STEPS)
				offsetY += JUMP_FACTOR*cos(jumped/JUMP_STEPS);
			else if (jumped > JUMP_STEPS)
				offsetY += JUMP_FACTOR*cos(jumped/JUMP_STEPS);
			if (++jumped == 2*JUMP_STEPS)
				jumping = false;
		}
		else {
			offsetY -= 1.5*FALL_STEP;
		}
		if (offsetY <= terrain->getPosition(platform, offsetZ).y) {
			jumping = false;
			jumpAvailable = true;
			offsetY = terrain->getPosition(platform, offsetZ).y;
		}
		computeCenter();
		alpha += 10;
	}
	//checkColisions(pl);
}

void Player::setTerrain(Terrain *t) {
	terrain = t;
}

void Player::computeCenter() {
	center = terrain->getPosition(platform, offsetZ);
	Vector normal = terrain->getNormal(platform);
	center.x = offsetX;
	center.y = offsetY + radius*normal.y;
	center.z += radius*normal.z;
}

void Player::checkColisions(vector<Player*> &pl) {
	for (unsigned int i = 0; i < pl.size(); ++i) {
		if (i != playerID) {
			if ((pl[i]->getPosition() - center).length() < pl[i]->radius + radius){
				if (pl[i]->getPosition().z < pl[playerID]->getPosition().z) blocked = true;
				return;
			}
		}
	}
	blocked = false;
}

bool Player::isJumping() {
	return jumping;
}

void Player::render() {
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	
	if (textID > 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textID);
		gluQuadricTexture(quad, GL_TRUE);

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); //OBJECT for rotations
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		glRotatef(-alpha, 1, 0, 0);
		gluSphere(quad, radius, DEPTH, DEPTH);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}

void Player::stopPlayer() {
	speed /= 2;
}

void Player::setBlocked(bool b) {
	blocked = b;
}

bool Player::getBlocked() {
	return blocked;
}

void Player::activateTurbo() {
	if (!turboWait) {
		turboLeft = TURBO_STEPS;
	}
}