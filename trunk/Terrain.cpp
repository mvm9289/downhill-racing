#include "Terrain.h"

#include <cstdlib>
#include <cstring>

Terrain::Terrain(string level):vertices(0),normals(0),colors(0)
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
}

void Terrain::readLevelTxt(string level)
{
	ifstream inputFile;
	inputFile.open(level);

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
	for(int i = 1; i < n; ++i)
	{
		vertices.push_back(Vertex(Point(0.0, (float)heights[i], vertices[(i - 1)*VERTICES_PER_HEIGHT].coord.z - TERRAIN_DEPTH)));
		vertices.push_back(Vertex(Point(TERRAIN_WIDTH, vertices[i*VERTICES_PER_HEIGHT].y, vertices[i*VERTICES_PER_HEIGHT].z)));

		Face f;
		f.addVertex(vertices[(i - 1)*VERTICES_PER_HEIGHT]);
		f.addVertex(vertices[(i - 1)*VERTICES_PER_HEIGHT + 1]);
		f.addVertex(vertices[i*VERTICES_PER_HEIGHT]);
		f.addVertex(vertices[i*VERTICES_PER_HEIGHT + 1]);
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
	indicesArray = (GLint*)malloc(sizeof(GLint)*faces.size()*VERTICES_PER_FACE);

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
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < VERTICES_PER_FACE; ++j)
			indicesArray[i*VERTICES_PER_FACE + j] = faces[i].indices[j];

	glVertexPointer(3, GL_FLOAT, 0, verticesArray);
	glNormalPointer(GL_FLOAT, 0 , normalsArray);
	glColorPointer(3, GL_FLOAT, 0, colorsArray);
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
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawElements(GL_QUADS, faces.size()*VERTICES_PER_FACE, GL_UNSIGNED_INT, indicesArray);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}