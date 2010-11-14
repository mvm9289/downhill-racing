#pragma once

#include "Point.h"
#include "Vector.h"

class Vertex
{
public:
	Point coord;
	Vector normal;

public:
	Vertex(const Point& coordinates);
	~Vertex(void);
};
