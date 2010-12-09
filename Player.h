#pragma once
#include "Sphere.h"

#define PLAYER_STEP 0.2
#define JUMP_STEPS 15.0

class Player : public Sphere {
public:
	Player(Point c = Point(0,0,0), float r = 1);
	~Player(void);
	void move(float dx);
	void jump();
	void advance();
	Point getPosition();

private:
	float weight;
	bool jumping;
	float jumped;
	float platform; //of the terrain
	float offset; //inside platform
};

