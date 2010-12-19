#pragma once

#include "Level.h"
#include "Box.h"
#include "Point.h"
#include "Player.h"

#include <vector>
using namespace std;

#define NUM_PLAYERS 3
#define NOTHING 0
#define WINNER 1
#define LOSER 2

class Scene
{
private:
	Level level;
	vector<Player*> players;
	int win;

	void createGUI();

public:
	Scene(void);
	~Scene(void);
	bool init(string level);
	void restartLevel();
	Box boundingBox(void);
	void boundingSphere(Point& center, float& radius);
	void render(void);
	vector<Player*> getPlayers();
	Point getPlayerPosition();
	void movePlayer(unsigned int i, float dx);
	bool jumpPlayer(unsigned int i);
	int advancePlayers();
	bool stopPlayer(unsigned int i);
	bool turboPlayer(unsigned int i);
};
