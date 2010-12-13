#pragma once
#include "Sphere.h"
#include "Terrain.h"

#include <vector>
using namespace std;

#define PLAYER_STEP 0.8
#define JUMP_STEPS 15.0
#define JUMP_FACTOR 0.5

class Player : public Sphere {
public:
	Player(Point c = Point(0,0,0), float r = 1);
	~Player(void);
	void init();
	void move(float dx, vector<Player*> &pl, int me);
	void jump();
	void advance(vector<Player*> &pl, int me);
	Point getPosition();
	void setTerrain(Terrain *t);

private:
	float weight;
	bool jumping;
	bool jumpAvailable;
	float jumped;
	bool blocked; //if we have crashed
	unsigned int platform; //of the terrain
	float offsetX; //inside platform X
	float offsetY; //for jumping (absolute)
	float offsetZ; //inside platform Z
	Terrain *terrain;

	void computeCenter();
	void checkColisions(vector<Player*> &pl, int me);
};

