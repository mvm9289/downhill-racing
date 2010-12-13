#pragma once

#include "Level.h"
#include "Box.h"
#include "Point.h"
#include "Player.h"

#include <vector>
using namespace std;

class Scene
{
private:
	Level level;
	vector<Player*> players;

public:
	Scene(void);
	~Scene(void);
	bool init(string level);
	Box boundingBox(void);
	void boundingSphere(Point& center, float& radius);
	void render(void);
	Point getPlayerPosition();
	void movePlayer(float dx);
	void jumpPlayer();
	void advancePlayer();
};
