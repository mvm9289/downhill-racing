#pragma once

#include "Vertex.h"

#include <vector>
using namespace std;

class Face
{
// ATTRIBUTES
public:
	vector<int> indices;
	Vector normal;

// FUNCTIONS
public:
	Face(void);
	~Face(void);
	void addVertex(int v);
	void computeNormal(const vector<Vertex>& vertices);
	void addVerticesNormals(vector<Vertex>& vertices);
};
