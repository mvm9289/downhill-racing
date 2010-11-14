#pragma once

#include "Terrain.h"
#include "Box.h"

#include <string>

class Level
{
private:
	Terrain *terrain;

public:
	Level(string level);
	Level(void);
	virtual ~Level(void);

	bool loadLevel(string level);
	Box boundingBox();
	void render();
};
