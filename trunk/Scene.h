#pragma once

#include "Terrain.h"
#include "Box.h"
#include "Point.h"

class Scene
{
// ATTRIBUTES
private:
	Terrain terrain;

// FUNCTIONS
public:
	Scene(void);
	~Scene(void);
	Box boundingBox(void);
	void boundingSphere(Point& center, float& radius);
	void render(void);
};
