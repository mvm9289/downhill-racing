#include "Face.h"

Face::Face(void) {}

Face::~Face(void) {}

void Face::addVertex(int v)
{
	indices.push_back(v);
}

void Face::computeNormal(vector<Vertex>& vertices)
{
	Point a = vertices[indices[0]].coord;
	Point b = vertices[indices[1]].coord;
	Point c = vertices[indices[2]].coord;

	Vector v1 = b - a;
	Vector v2 = c - a;

	normal.x = (v1.y*v2.z) - (v1.z*v2.y);
	normal.y = -((v2.z*v1.x) - (v2.x*v1.z));
	normal.z = (v1.x*v2.y) - (v1.y*v2.x);
	normal.normalize();

	int n = indices.size();
	for (int i = 0; i < n; i++)
	{
		vertices[indices[i]].normal.x += normal.x;
		vertices[indices[i]].normal.y += normal.y;
		vertices[indices[i]].normal.z += normal.z;
		vertices[indices[i]].normal.normalize();
	}
}
