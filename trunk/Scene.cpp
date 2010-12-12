#include "Scene.h"
#include "Texture.h"

Scene::Scene(void) {}

Scene::~Scene(void)
{
	delete player1;
	delete player2;
}

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
	glColor3f(1., 0., 0.);
	player1->render();
	player2->render();
}

bool Scene::init(string levelPath)
{
	bool loadOK = level.loadLevel(levelPath);
	if (!loadOK) return false;

	player1 = new Player(level.startupPoint());
	player2 = new Player(level.startupPoint() + Point(3, 0, 0), 1.5);
	Texture playerTexture1;
	Texture playerTexture2;
	playerTexture1.load("textures/player4.png", GL_RGBA);
	//playerTexture.load("textures/player3.png", GL_RGBA);
	//playerTexture.load("textures/player2.png", GL_RGB);
	playerTexture2.load("textures/player1.png", GL_RGBA);
	player1->setTextureID(playerTexture1.getID());
	player2->setTextureID(playerTexture2.getID());

	player1->setTerrain(level.getTerrain());
	player2->setTerrain(level.getTerrain());
	player1->init();
	player2->init();

	return true;
}

Point Scene::getPlayerPosition()
{
	return player1->getPosition();
}

void Scene::movePlayer(float dx)
{
	player1->move(dx);
}

void Scene::jumpPlayer()
{
	player1->jump();
}

void Scene::advancePlayer()
{
	player1->advance();
	player2->advance();
}