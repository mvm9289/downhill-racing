#pragma once

#include <gl/glut.h>
#include "Vertex.h"
#include "Face.h"
#include "Box.h"

#include <vector>
#include <string>
using namespace std;

#define TERRAIN_WIDTH 5.0
#define TERRAIN_DEPTH 2.0
#define VERTICES_PER_HEIGHT 2
#define VERTICES_PER_FACE 4
#define VERTEX_COMPONENTS 3
#define NORMAL_COMPONENTS 3
#define COLOR_COMPONENTS 3

class Terrain
{
// ATTRIBUTES
private:
	vector<Vertex> vertices;
	vector<Face> faces;
	GLfloat* verticesArray;
	GLfloat* normalsArray;
	GLfloat* colorsArray;
	GLuint* indicesArray;

public:
	vector<int> heights;

// FUNCTIONS
private:
	void readLevelTxt(string level);
	void readLevelXML(string level);
	void createTerrain(void);
	void createVertexArrays(void);

public:
	Terrain(string level);
	~Terrain(void);
	Box boundingBox(void);
	void render(void);
};
