#include "Scene.h"

Scene::Scene(void) {}

Scene::~Scene(void) {}

Box Scene::boundingBox(void)
{
	return level.boundingBox();
}

void Scene::boundingSphere(Point& center, float& radius)
{
	Box box = boundingBox();
	center = (box.maximum + box.minimum)/2;
	radius = (box.maximum - center).length();
}

void Scene::render(void)
{
	level.render();
}

bool Scene::init(string levelPath)
{
	return level.loadLevel(levelPath);
}