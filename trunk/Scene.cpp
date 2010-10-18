#include "Scene.h"

Scene::Scene(void):terrain("levels/level1.txt") {}

Scene::~Scene(void) {}

Box Scene::boundingBox(void)
{
	return terrain.boundingBox();
}

void Scene::boundingSphere(Point& center, float& radius)
{
	Box box = boundingBox();
	center = (box.maximum + box.minimum)/2;
	radius = (box.maximum - center).length();
}

void Scene::render(void)
{
	terrain.render();
}