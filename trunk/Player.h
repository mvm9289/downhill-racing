#pragma once
#include "Sphere.h"

class Player : public Sphere {
public:
	Player(Point c = Point(0,0,0), float r = 1);
	~Player(void);
	void move(float dx);
	void jump();
	Point getPosition();

private:
	float weight;
};

