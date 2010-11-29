#pragma once

#include <gl/glut.h>
#include "Vertex.h"
#include "Face.h"
#include "Box.h"

#include <vector>
#include <string>
using namespace std;

#define TERRAIN_WIDTH 10.0

class Terrain
{
private:
	vector<Vertex> vertices;
	vector<Face> faces;
	Box _boundingBox;
	GLuint texture;
	GLuint displayList;

public:
	vector<double> heights;

private:
	void createTerrain(vector<double> terrainPoints);
	void createDisplayList();
	void updateBoundingBox();

public:
	Terrain(vector<double> terrainPoints, GLuint textureID);
	Terrain(void);
	~Terrain(void);
	Box boundingBox(void);
	void render(void);
};
