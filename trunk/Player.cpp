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
		if (offsetY <= terrain->getPosition(platform, offsetZ).y) {
			jumping = false;
			jumpAvailable = true;
			offsetY = terrain->getPosition(platform, offsetZ).y;
		}
		computeCenter();
	}
	checkColisions(pl, me);
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

void Player::checkColisions(vector<Player*> &pl, int me) {
	for (unsigned int i = 0; i < pl.size(); ++i) {
		if (i != me) {
			if ((pl[i]->getPosition() - center).length() < pl[i]->radius + radius){
				if (pl[i]->getPosition().z < pl[me]->getPosition().z) blocked = true;
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
	alpha += 10;
}

void Player::setBlocked(bool b) {
	blocked = b;
}

bool Player::getBlocked() {
	return blocked;
}