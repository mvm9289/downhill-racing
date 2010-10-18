#include "Face.h"

Face::Face(void) {}

Face::~Face(void) {}

void Face::addVertex(int v)
{
	indices.push_back(v);
}

void Face::computeNormal(const vector<Vertex>& vertices)
{
	normal.x=0.0;
	normal.y=0.0;
	normal.z=0.0;

	int n = indices.size();
	for (int i = 0; i < n; ++i)
	{
		int j = (i + 1)%n;
		float vx = vertices[indices[i]].coord.x;
		float vy = vertices[indices[i]].coord.y;
		float vz = vertices[indices[i]].coord.z;
		float v1x = vertices[indices[j]].coord.x;
		float v1y = vertices[indices[j]].coord.y;
		float v1z = vertices[indices[j]].coord.z;
		normal.x+=((vz + v1z)*(vy - v1y));
		normal.y+=((vx + v1x)*(vz - v1z));
		normal.z+=((vy + v1y)*(vx - v1x));
	}

	normal.x*=0.5;
	normal.y*=0.5;
	normal.z*=0.5;

	normal.normalize();
}

void Face::addVerticesNormals(vector<Vertex>& vertices)
{
	int n = vertices.size();
	for (int i = 0; i < n; i++)
		vertices[indices[i]].addNormal(normal);
}