#pragma once

#include "Point.h"
#include "Vector.h"

class Vertex
{
// ATTRIBUTES
public:
	Point coord;
	Vector normal;

// FUNCTIONS
public:
	Vertex(const Point& coordinates);
	~Vertex(void);
	void addNormal(const Vector& norm);
	void computeNormal(void);
};
