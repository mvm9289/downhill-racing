#include "Player.h"
#include "Terrain.h"
#include <cmath>

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

bool Player::jump() {
	if (!jumping && jumpAvailable && platform) {
		jumping = true;
		jumpAvailable = false;
		jumped = 0;
		return true;
	}
	return false;
}

Point Player::getPosition() {
	return center;
}

void Player::advance(vector<Player*> &pl) {
	if (!blocked) {
		float advance = speed*PLAYER_STEP;
		if (turboLeft) advance += 2.0*PLAYER_STEP;

		if (platform && terrain->getDirection(platform).slopeYZ() < 0) {
			speed += 0.03*radius;
			if (speed > (SPEED_MAX + radius - 1)) speed = (SPEED_MAX + radius - 1);
		}
		else if (platform) {
			speed -= 0.03*radius;
			if (speed < (SPEED_MIN - radius + 1)) speed = (SPEED_MIN - radius + 1);
		}

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
					speed -= 0.1*radius;
				}
				else {
					blocked = false;
					speed += 0.1*radius;
				}
			}
		}
	}
	blocked = false;
}

bool Player::isJumping() {
	return !jumpAvailable;
}

void Player::render(bool hellMode, float angle) {
	GLUquadricObj *quad;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	if (hellMode) glColor3f(1, 0, 0);
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
		glRotatef(-angle, 1, 0, 0);
		if (turboWait) {
			glColor4f(1, 1, 0, 1);
			glBegin(GL_LINES);
				glVertex3f(radius + 0.15, 0, 0);
				glVertex3f(radius + 0.15, 2*radius*(TURBO_TIME - turboWait)/TURBO_TIME, 0);
			glEnd();
		}
		else if (!turboLeft) {
			glColor4f(0, 1, 0, 1);
			glBegin(GL_LINES);
				glVertex3f(radius + 0.15, 0, 0);
				glVertex3f(radius + 0.15, 2*radius, 0);
			glEnd();
			glEnable(GL_LIGHTING);
		}
		else {
			glColor4f(0, 1, 0, 1);
			glBegin(GL_LINES);
				glVertex3f(radius + 0.15, 0, 0);
				glVertex3f(radius + 0.15, 2*radius*(turboLeft)/TURBO_STEPS, 0);
			glEnd();
			glEnable(GL_LIGHTING);
		}
	glPopMatrix();
	glColor4f(1, 1, 1, 1);
}

bool Player::stopPlayer() {
	if (platform) {
		speed /= 1.1;
		return true;
	}
	return false;
}

void Player::setBlocked(bool b) {
	blocked = b;
}

bool Player::getBlocked() {
	return blocked;
}

bool Player::activateTurbo() {
	if (!turboLeft && !turboWait && jumpAvailable && platform) {
		turboLeft = TURBO_STEPS;
		return true;
	}
	return false;
}

bool Player::getJumpAvailable() {
	return jumpAvailable;
}

float Player::getRadius()
{
	return radius;
}
