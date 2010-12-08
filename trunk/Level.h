#pragma once

#include "Terrain.h"
#include "Box.h"
#include "Sphere.h"

#include <string>

class Level
{
private:
	Terrain *terrain;
	Sphere *skydome;

public:
	Level(string level);
	Level(void);
	virtual ~Level(void);
	Point startupPoint();
	bool loadLevel(string level);
	void boundingSphere(Point& center, float& radius);
	Box boundingBox();
	void render();
};
