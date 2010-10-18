#include "Vertex.h"

Vertex::Vertex(const Point& coordinates):coord(coordinates) {}

Vertex::~Vertex(void) {}

void Vertex::addNormal(const Vector& norm)
{
	normal+=norm;
}

void Vertex::computeNormal( void )
{
	normal.normalize();
}