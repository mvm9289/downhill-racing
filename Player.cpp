#include "Player.h"
#include "Terrain.h"
#include <cmath>

#include <iostream>
using namespace std;

Player::Player(unsigned int id, Point c, float r): playerID(id), Sphere(Point(c.x, c.y + r, c.z), r), initPoint(c) {
}

void Player::init() {
	speed = SPEED_MIN;
	platform = 0;
	offsetZ = terrain->getPlatformLength(platform)/2.0;
	offsetX = initPoint.x;
	offsetY = initPoint.y + radius;
	computeCenter();
	turboLeft = 0;
	turboWait = 0;
	jumping =false;
	jumpAvailable = true;
	blocked = false;
}

Player::~Player(void) {}

void Player::move(float dx, vector<Player*> &pl) {
	if (!blocked && platform) {
		offsetX += dx;
		if (offsetX < radius) offsetX = radius;
		if (offsetX > TERRAIN_WIDTH*SCALE_FACTOR - radius) offsetX = TERRAIN_WIDTH*SCALE_FACTOR - radius;
		computeCenter();
	}
	checkColisions(pl);
}

void Player::jump() {
	if (!jumping && jumpAvailable && platform) {
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

		//if (playerID == 0) cout << "Speed: " << speed << " Platform: " << platform << " Offset: " << offsetZ << endl;

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
	checkColisions(pl);
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
			if ((pl[i]->getPosition() - center).length() < pl[i]->radius + radius) {
				if (pl[i]->getPosition().z < pl[playerID]->getPosition().z) {
					blocked = true;
					speed = SPEED_MIN;
				}
				else {
					blocked = false;
					speed += 2;
				}
			}
		}
	}
	blocked = false;
}

bool Player::isJumping() {
	return !jumpAvailable;
}

void Player::render() {
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
	glColor4f(1, 1, 1, 0.8);
	*/
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
		glPushMatrix();
			glRotatef(-alpha, 1, 0, 0);
			gluSphere(quad, radius, DEPTH, DEPTH);
		glPopMatrix();

		//Turbo bars
		glLineWidth(6.0);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glTranslatef(0, -radius, 0);
		if (turboWait) {
			glColor3f(1, 1, 0);
			glBegin(GL_LINES);
				glVertex3f(radius + 0.1, 0, 0);
				glVertex3f(radius + 0.1, 2*radius*(TURBO_TIME - turboWait)/TURBO_TIME, 0);
			glEnd();
		}
		else if (!turboLeft) {
			glColor3f(0, 1, 0);
			glBegin(GL_LINES);
				glVertex3f(radius + 0.1, 0, 0);
				glVertex3f(radius + 0.1, 2*radius, 0);
			glEnd();
			glEnable(GL_LIGHTING);
		}
		else {
			glColor3f(0, 1, 0);
			glBegin(GL_LINES);
				glVertex3f(radius + 0.1, 0, 0);
				glVertex3f(radius + 0.1, 2*radius*(turboLeft)/TURBO_STEPS, 0);
			glEnd();
			glEnable(GL_LIGHTING);
		}
	glPopMatrix();
	glColor3f(1, 1, 1);

	//glDisable(GL_BLEND);
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
	if (!turboLeft && !turboWait && jumpAvailable && platform) {
		turboLeft = TURBO_STEPS;
	}
}

bool Player::getJumpAvailable() {
	return jumpAvailable;
}
