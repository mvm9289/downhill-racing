#pragma once

#include <gl/glut.h>
#include "Vertex.h"
#include "Face.h"
#include "Box.h"

#include <vector>
#include <string>
using namespace std;

#define TERRAIN_WIDTH 10.0
#define TERRAIN_DEPTH 1.0
#define VERTICES_PER_HEIGHT 2
#define VERTICES_PER_FACE 4
#define VERTEX_COMPONENTS 3
#define NORMAL_COMPONENTS 3
#define TEXTURE_COMPONENTS 2

class Terrain
{
private:
	vector<Vertex> vertices;
	vector<Face> faces;
	vector<Point> textureCoords;
	Box _boundingBox;
	GLuint texture;

	GLfloat* verticesArray;
	GLfloat* normalsArray;
	GLfloat* textureArray;
	GLuint* indicesArray;

public:
	vector<int> heights;

private:
	void createTerrain(vector<int> terrainHeights);
	void createVertexArrays(void);
	void updateBoundingBox();

public:
	Terrain(vector<int> terrainHeights, GLuint textureID);
	Terrain(void);
	~Terrain(void);
	Box boundingBox(void);
	void render(void);
};
