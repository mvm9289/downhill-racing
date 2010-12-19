#include "Vertex.h"

Vertex::Vertex(const Point& coordinates):coord(coordinates)
{
	normal.x = 0;
	normal.y = 0;
	normal.z = 0;
}

Vertex::~Vertex(void) {}