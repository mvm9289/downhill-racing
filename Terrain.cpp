#include "Terrain.h"

Terrain::Terrain(vector<int> terrainHeights, GLuint textureID):verticesArray(0),normalsArray(0),textureArray(0),indicesArray(0)
{
	texture = textureID;
	createTerrain(terrainHeights);
	createVertexArrays();
}

Terrain::Terrain(void)
{
}

Terrain::~Terrain(void)
{
	free(verticesArray);
	free(normalsArray);
	free(textureArray);
	free(indicesArray);
}

void Terrain::createTerrain(vector<int> terrainHeights)
{
	heights = terrainHeights;

	vertices.push_back(Vertex(Point(0.0, (float)heights[0], 0.0)));
	vertices.push_back(Vertex(Point(TERRAIN_WIDTH, vertices[0].coord.y, vertices[0].coord.z)));
	textureCoords.push_back(Point(0, 0));
	textureCoords.push_back(Point(TERRAIN_WIDTH, 0));

	int n = heights.size();
	for(int i = 1, j = 0; i < n; ++i)
	{
		vertices.push_back(Vertex(Point(0.0, (float)heights[i], vertices[j++].coord.z - TERRAIN_DEPTH)));
		vertices.push_back(Vertex(Point(TERRAIN_WIDTH, (float)heights[i], vertices[j++].coord.z - TERRAIN_DEPTH)));
		textureCoords.push_back(Point(0, TERRAIN_DEPTH));
		textureCoords.push_back(Point(TERRAIN_WIDTH, 0));

		Face f;
		f.addVertex(j - 2);
		f.addVertex(j - 1);
		f.addVertex(j + 1);
		f.addVertex(j);
		faces.push_back(f);
	}

	n = faces.size();
	for(int i = 0; i < n; ++i) faces[i].computeNormal(vertices);

	updateBoundingBox();
}

void Terrain::createVertexArrays(void)
{
	int n = vertices.size();

	verticesArray = (GLfloat*)malloc(sizeof(GLfloat)*n*VERTEX_COMPONENTS);
	normalsArray = (GLfloat*)malloc(sizeof(GLfloat)*n*NORMAL_COMPONENTS);
	textureArray = (GLfloat*)malloc(sizeof(GLfloat)*n*TEXTURE_COMPONENTS);
	indicesArray = (GLuint*)malloc(sizeof(GLuint)*faces.size()*VERTICES_PER_FACE);

	for(int i = 0; i < n; ++i)
	{
		verticesArray[i*VERTEX_COMPONENTS] = vertices[i].coord.x;
		verticesArray[i*VERTEX_COMPONENTS + 1] = vertices[i].coord.y;
		verticesArray[i*VERTEX_COMPONENTS + 2] = vertices[i].coord.z;

		normalsArray[i*NORMAL_COMPONENTS] = vertices[i].normal.x;
		normalsArray[i*NORMAL_COMPONENTS + 1] = vertices[i].normal.y;
		normalsArray[i*NORMAL_COMPONENTS + 2] = vertices[i].normal.z;

		textureArray[i*TEXTURE_COMPONENTS] = textureCoords[i].x;
		textureArray[i*TEXTURE_COMPONENTS + 1] = textureCoords[i].y;
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
	glVertexPointer(3, GL_FLOAT, 0, verticesArray);
	glNormalPointer(GL_FLOAT, 0 , normalsArray);
	glTexCoordPointer(2, GL_FLOAT, 0, textureArray);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_QUADS, faces.size()*VERTICES_PER_FACE, GL_UNSIGNED_INT, indicesArray);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}