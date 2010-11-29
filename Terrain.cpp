#include "Terrain.h"

Terrain::Terrain(vector<double> terrainPoints, GLuint textureID)
{
	texture = textureID;
	createTerrain(terrainPoints);
	createDisplayList();
	updateBoundingBox();
}

Terrain::Terrain(void)
{
}

Terrain::~Terrain(void)
{
}

void Terrain::createTerrain(vector<double> terrainPoints)
{
	int n2Dpoints = terrainPoints.size()/2;
	for (int i = 0; i < n2Dpoints; i++)
	{
		int x = 2*i;
		int y = x + 1;
		Point p1(0, terrainPoints[y], -terrainPoints[x]);
		Point p2(TERRAIN_WIDTH, terrainPoints[y], -terrainPoints[x]);
		Vertex v1(p1);
		Vertex v2(p2);
		vertices.push_back(v1);
		vertices.push_back(v2);
	}
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
		glBindTexture(GL_TEXTURE_2D, texture);
		for (int i = 0; i < faces.size(); i++)
		{
			faces[i].render(vertices);
		}
		glDisable(GL_TEXTURE_2D);
	glEndList();
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