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

Level::~Level(void)
{
	delete terrain;
	delete skydome;
}

Point Level::startupPoint()
{
	return terrain->startupPoint();
}

bool Level::loadLevel(string level)
{
	ifstream inputFile;
	inputFile.open(level.c_str());

	if (inputFile.is_open())
	{
		string textureBitMap;

		if (!inputFile.eof()) inputFile >> textureBitMap;
		Texture terrainTexture;
		if (!terrainTexture.load((char *)("textures/" + textureBitMap).c_str(), GL_RGB))
			return false;

		if (!inputFile.eof()) inputFile >> textureBitMap;
		Texture skydomeTexture;
		if (!skydomeTexture.load((char *)("textures/" + textureBitMap).c_str(), GL_RGBA))
			return false;

		vector<double> terrainPoints;
		while (!inputFile.eof()) {
			double aux;
			inputFile >> aux;
			terrainPoints.push_back(aux);
		}
		inputFile.close();

		terrain = new Terrain(terrainPoints, terrainTexture.getID());

		Point center;
		float radius;
		boundingSphere(center, radius);
		skydome = new Sphere(center, radius*1.1);
		skydome->setTextureID(skydomeTexture.getID());
	}

	return true;
}

void Level::boundingSphere(Point& center, float& radius)
{
	Box box = boundingBox();
	center = (box.maximum + box.minimum)/2;
	radius = (box.maximum - center).length();
}

Box Level::boundingBox()
{
	if (terrain != NULL) return terrain->boundingBox();
	else return Box();
}

void Level::render()
{
	if (terrain != NULL) terrain->render();
	if (skydome != NULL)
	{
		glDisable(GL_LIGHTING);
		glCullFace(GL_FRONT);
		skydome->render();
		glCullFace(GL_BACK);
		glEnable(GL_LIGHTING);
	}
}
