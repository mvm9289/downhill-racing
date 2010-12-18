#include "Terrain.h"

#include <iostream>
using namespace std;

Terrain::Terrain(vector<double> terrainPoints, GLuint textureID, GLuint goalTextureID)
{
	texture = textureID;
	goalTexture = goalTextureID;
	createTerrain(terrainPoints);
	createDisplayList();
	updateBoundingBox();
}

Terrain::Terrain(void) {}

Terrain::~Terrain(void) {}

void Terrain::createTerrain(vector<double> terrainPoints)
{
	int firstMax = 0;
	bool found = false;
	for (unsigned int i = 2; i < (terrainPoints.size() - 1) && !found; i+=2)
	{
		if (terrainPoints[i + 1] >= terrainPoints[firstMax + 1])
			firstMax = i;
		else found = true;
	}
	if (!found) firstMax = 0;
	cout << firstMax << endl;
	Point p1f(0, terrainPoints[firstMax + 1]*SCALE_FACTOR,
		(-terrainPoints[firstMax] + STARTUP_DEPTH)*SCALE_FACTOR);
	Point p2f(TERRAIN_WIDTH*SCALE_FACTOR,
		terrainPoints[firstMax + 1]*SCALE_FACTOR,
		-(terrainPoints[firstMax] - STARTUP_DEPTH)*SCALE_FACTOR);
	Vertex v1f(p1f);
	Vertex v2f(p2f);
	vertices.push_back(v1f);
	vertices.push_back(v2f);

	int lastMax = terrainPoints.size() - 2;
	found = false;
	for (unsigned int i = terrainPoints.size() - 4; i > 0 && !found; i-=2)
	{
		if (terrainPoints[i + 1] >= terrainPoints[lastMax + 1])
			lastMax = i;
		else found = true;
	}
	if (!found) lastMax = terrainPoints.size() - 2;

	int x, y;
	for (int i = firstMax/2; i <= lastMax/2; i++)
	{
		x = 2*i;
		y = x + 1;
		Point p1(0, terrainPoints[y]*SCALE_FACTOR,
			-terrainPoints[x]*SCALE_FACTOR);
		Point p2(TERRAIN_WIDTH*SCALE_FACTOR,
			terrainPoints[y]*SCALE_FACTOR,
			-terrainPoints[x]*SCALE_FACTOR);
		Vertex v1(p1);
		Vertex v2(p2);
		vertices.push_back(v1);
		vertices.push_back(v2);
	}

	Point p1l(0, terrainPoints[y]*SCALE_FACTOR,
		(-terrainPoints[x] - STARTUP_DEPTH)*SCALE_FACTOR);
	Point p2l(TERRAIN_WIDTH*SCALE_FACTOR,
		terrainPoints[y]*SCALE_FACTOR,
		(-terrainPoints[x] - STARTUP_DEPTH)*SCALE_FACTOR);
	Vertex v1l(p1l);
	Vertex v2l(p2l);
	vertices.push_back(v1l);
	vertices.push_back(v2l);

	int nVertices = vertices.size();
	for (int i = 0; i < nVertices - 3; i+=2)
	{
		Face f;
		f.addVertex(i);
		f.addVertex(i + 1);
		f.addVertex(i + 3);
		f.addVertex(i + 2);
		f.computeNormal(vertices);
		faces.push_back(f);
	}
}

void Terrain::createDisplayList()
{
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, goalTexture);
		faces[0].render(vertices);
		glBindTexture(GL_TEXTURE_2D, texture);
		for (unsigned int i = 1; i < faces.size() - 1; i++)
			faces[i].render(vertices);
		glBindTexture(GL_TEXTURE_2D, goalTexture);
		faces[faces.size() - 1].render(vertices);
		glDisable(GL_TEXTURE_2D);
	glEndList();
}

Point Terrain::startupPoint()
{
	Point p1 = vertices[faces[0].indices[0]].coord;
	Point p2 = vertices[faces[0].indices[1]].coord;
	Point p3 = vertices[faces[0].indices[2]].coord;
	Point p4 = vertices[faces[0].indices[3]].coord;

	Point p12 = (p1 + p2)/2;
	Point p34 = (p3 + p4)/2;

	return (p12 + p34)/2;
}

void Terrain::updateBoundingBox()
{
	_boundingBox.init(vertices[0].coord);
	int n = vertices.size();
	for(int i = 0; i < n; ++i)
		_boundingBox.update(vertices[i].coord);
}

Box Terrain::boundingBox(void)
{
	return _boundingBox;
}

void Terrain::render(void)
{
	glCallList(displayList);
}

Point Terrain::getPosition(unsigned int platform, float offset) {
	Point pini = vertices[faces[platform].indices[0]].coord;
	Point pfin = vertices[faces[platform].indices[3]].coord;
	Vector dir = pfin - pini;
	dir.normalize();
	return pini + offset*dir;
}

float Terrain::getPlatformLength(unsigned int platform) {
	return getDirection(platform).length();
}

Vector Terrain::getDirection(unsigned int platform) {
	Point pini = vertices[faces[platform].indices[0]].coord;
	Point pfin = vertices[faces[platform].indices[3]].coord;
	return pfin - pini;
}

Vector Terrain::getNormal(unsigned int platform) {
	return faces[platform].normal;
}