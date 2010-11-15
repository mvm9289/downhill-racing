#include "Level.h"

#include "Texture.h"

#include <cstdlib>
#include <cstring>
#include <fstream>

Level::Level(string level)
{
	loadLevel(level);
}

Level::Level(void) {}

Level::~Level(void) {}

bool Level::loadLevel(string level)
{
	bool res = false;

	ifstream inputFile;
	inputFile.open(level.c_str());

	if (inputFile.is_open())
	{
		string textureBitMap;
		if (!inputFile.eof()) inputFile >> textureBitMap;
		Texture texture;
		if (!texture.load((char *)("textures/" + textureBitMap).c_str(), GL_RGB)) return false;

		vector<int> terrainHeights;
		while (!inputFile.eof()) {
			int aux;
			inputFile >> aux;
			terrainHeights.push_back(aux);
		}
		inputFile.close();

		terrain = new Terrain(terrainHeights, texture.getID());

		res = true;
	}

	return res;
}

Box Level::boundingBox()
{
	if (terrain != NULL) return terrain->boundingBox();
	else return Box();
}

void Level::render()
{
	if (terrain != NULL) terrain->render();
}