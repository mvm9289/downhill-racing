#pragma once

#include "Level.h"
#include "Box.h"
#include "Point.h"

class Scene
{
private:
	Level level;

public:
	Scene(void);
	~Scene(void);
	bool init(string level);
	Box boundingBox(void);
	void boundingSphere(Point& center, float& radius);
	void render(void);
};
