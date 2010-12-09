#pragma once
#include "Sphere.h"
#include "Terrain.h"

#define PLAYER_STEP 0.5
#define JUMP_STEPS 15.0
#define JUMP_FACTOR 0.5

class Player : public Sphere {
public:
	Player(Point c = Point(0,0,0), float r = 1);
	~Player(void);
	void init();
	void move(float dx);
	void jump();
	void advance();
	Point getPosition();
	void setTerrain(Terrain *t);

private:
	float weight;
	bool jumping;
	float jumped;
	unsigned int platform; //of the terrain
	float offsetX; //inside platform X
	float offsetY; //for jumping (absolute)
	float offsetZ; //inside platform Z
	Terrain *terrain;

	void computeCenter();
};

