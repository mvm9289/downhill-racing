#pragma once

#include <gl/glut.h>
#include "Vertex.h"
#include "Face.h"
#include "Box.h"

#include <vector>
#include <string>
using namespace std;

#define TERRAIN_WIDTH 10.0
#define STARTUP_DEPTH 40.0
#define SCALE_FACTOR 3.0

class Terrain
{
private:
	vector<Vertex> vertices;
	vector<Face> faces;
	Box _boundingBox;
	GLuint texture;
	GLuint goalTexture;
	GLuint displayList;

public:
	vector<double> heights;

private:
	void createTerrain(vector<double> terrainPoints);
	void createDisplayList();
	void updateBoundingBox();

public:
	Terrain(vector<double> terrainPoints, GLuint textureID, GLuint goalTextureID);
	Terrain(void);
	~Terrain(void);
	Point startupPoint();
	Box boundingBox(void);
	void render(void);
	Point getPosition(unsigned int platform, float offset);
	float getPlatformLength(unsigned int platform);
	Vector getDirection(unsigned int platform);
	Vector getNormal(unsigned int platform);
};
