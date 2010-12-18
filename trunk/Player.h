#pragma once
#include "Sphere.h"
#include "Terrain.h"
#include "MLabel.h"

#include <vector>
using namespace std;

#define PLAYER_STEP 0.1
#define FALL_STEP 0.8
#define SPEED_MIN 4
#define SPEED_MAX 10
#define JUMP_STEPS 10.0
#define JUMP_FACTOR 0.5
#define TURBO_STEPS 100
#define TURBO_TIME 500

class Player : public Sphere {
public:
	Player(unsigned int id, Point c = Point(0,0,0), float r = 1, bool comp = true);
	~Player(void);
	void init();
	void move(float dx, vector<Player*> &pl);
	void jump();
	void advance(vector<Player*> &pl);
	void stopPlayer();
	void activateTurbo();
	
	Point getPosition();
	void setTerrain(Terrain *t);
	bool isJumping();
	void render();
	void setBlocked(bool b);
	bool getBlocked();

	bool getJumpAvailable();

private:
	//float weight; //not used, we use radius instead
	Point initPoint;
	bool jumping;
	bool jumpAvailable;
	float jumped;
	bool blocked; //if we have crashed
	unsigned int platform; //of the terrain
	float offsetX; //inside platform X
	float offsetY; //for jumping (absolute)
	float offsetZ; //inside platform Z
	Terrain *terrain;
	unsigned int speed;

	int turboLeft;
	int turboWait;

	unsigned int playerID;

	void computeCenter();
	void checkColisions(vector<Player*> &pl);
};

