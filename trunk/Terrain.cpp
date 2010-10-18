#include "Terrain.h"

#include <cstdlib>
#include <cstring>
#include <fstream>

Terrain::Terrain(string level):verticesArray(0),normalsArray(0),colorsArray(0),indicesArray(0)
{
	readLevelTxt(level);
	createTerrain();
	createVertexArrays();
}

Terrain::~Terrain(void)
{
	free(verticesArray);
	free(normalsArray);
	free(colorsArray);
	free(indicesArray);
}

void Terrain::readLevelTxt(string level)
{
	ifstream inputFile;
	inputFile.open(level.c_str());

	if (inputFile.is_open()) {
		while (!inputFile.eof()) {
			int aux;
			inputFile >> aux;
			heights.push_back(aux);
		}
		inputFile.close();
	}
}

void Terrain::readLevelXML(string level)
{

}

void Terrain::createTerrain(void)
{
	vertices.push_back(Vertex(Point(0.0, (float)heights[0], 0.0)));
	vertices.push_back(Vertex(Point(TERRAIN_WIDTH, vertices[0].coord.y, vertices[0].coord.z)));

	int n = heights.size();
	for(int i = 1, j = 0; i < n; ++i)
	{
		vertices.push_back(Vertex(Point(0.0, (float)heights[i], vertices[j++].coord.z - TERRAIN_DEPTH)));
		vertices.push_back(Vertex(Point(TERRAIN_WIDTH, (float)heights[i], vertices[j++].coord.z - TERRAIN_DEPTH)));

		Face f;
		f.addVertex(j - 2);
		f.addVertex(j - 1);
		f.addVertex(j + 1);
		f.addVertex(j);
		faces.push_back(f);
	}

	n = faces.size();
	for(int i = 0; i < n; ++i)
	{
		faces[i].computeNormal(vertices);
		faces[i].addVerticesNormals(vertices);
	}

	n = vertices.size();
	for(int i = 0; i < n; ++i)
	{
		vertices[i].computeNormal();
	}
}

void Terrain::createVertexArrays(void)
{
	int n = vertices.size();

	verticesArray = (GLfloat*)malloc(sizeof(GLfloat)*n*VERTEX_COMPONENTS);
	normalsArray = (GLfloat*)malloc(sizeof(GLfloat)*n*NORMAL_COMPONENTS);
	colorsArray = (GLfloat*)malloc(sizeof(GLfloat)*n*COLOR_COMPONENTS);
	indicesArray = (GLuint*)malloc(sizeof(GLuint)*faces.size()*VERTICES_PER_FACE);

	for(int i = 0; i < n; ++i)
	{
		verticesArray[i*VERTEX_COMPONENTS] = vertices[i].coord.x;
		verticesArray[i*VERTEX_COMPONENTS + 1] = vertices[i].coord.y;
		verticesArray[i*VERTEX_COMPONENTS + 2] = vertices[i].coord.z;

		normalsArray[i*NORMAL_COMPONENTS] = vertices[i].normal.x;
		normalsArray[i*NORMAL_COMPONENTS + 1] = vertices[i].normal.y;
		normalsArray[i*NORMAL_COMPONENTS + 2] = vertices[i].normal.z;

		colorsArray[i*COLOR_COMPONENTS] = 0.0;
		colorsArray[i*COLOR_COMPONENTS + 1] = 1.0;
		colorsArray[i*COLOR_COMPONENTS + 2] = 0.0;
	}

	n = faces.size();
	for(int i = 0, j = 0; i < n; ++i)
	{
		indicesArray[j++] = faces[i].indices[0];
		indicesArray[j++] = faces[i].indices[1];
		indicesArray[j++] = faces[i].indices[2];
		indicesArray[j++] = faces[i].indices[3];
	}
}

Box Terrain::boundingBox( void )
{
	Box box;

	box.init(vertices[0].coord);
	int n = vertices.size();
	for(int i = 0; i < n; ++i)
		box.update(vertices[i].coord);

	return box;
}

void Terrain::render(void)
{
	glVertexPointer(3, GL_FLOAT, 0, verticesArray);
	glNormalPointer(GL_FLOAT, 0 , normalsArray);
	glColorPointer(3, GL_FLOAT, 0, colorsArray);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawElements(GL_QUADS, faces.size()*VERTICES_PER_FACE, GL_UNSIGNED_INT, indicesArray);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}