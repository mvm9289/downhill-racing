#pragma once
#include "Point.h"
#include <gl/glut.h>

#define DEPTH 20

class Sphere
{
public:
	Sphere(Point c = Point(0,0,0), float r = 1);
	~Sphere(void);

	void render();
	void setTextureID(GLuint id);
protected:
	float radius;
	Point center;
	int textID;
};

