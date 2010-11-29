#pragma once

#include "Vertex.h"

#include <vector>
using namespace std;

class Face
{
public:
	vector<int> indices;
	Vector normal;

public:
	Face(void);
	~Face(void);
	void addVertex(int v);
	void computeNormal(vector<Vertex>& vertices);
	void render(const vector<Vertex>& vertices);
};
