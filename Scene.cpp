#include "Scene.h"
#include "Texture.h"

Scene::Scene(void) {}

Scene::~Scene(void)
{
	delete player;
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
	player->render();
}

bool Scene::init(string levelPath)
{
	bool loadOK = level.loadLevel(levelPath);
	if (!loadOK) return false;

	player = new Player(level.startupPoint());
	Texture playerTexture;
	playerTexture.load("textures/player4.png", GL_RGBA);
	//playerTexture.load("textures/player3.png", GL_RGBA);
	//playerTexture.load("textures/player2.png", GL_RGB);
	//playerTexture.load("textures/player1.png", GL_RGBA);
	player->setTextureID(playerTexture.getID());

	player->setTerrain(level.getTerrain());
	player->init();

	return true;
}

Point Scene::getPlayerPosition()
{
	return player->getPosition();
}

void Scene::movePlayer(float dx)
{
	player->move(dx);
}

void Scene::jumpPlayer()
{
	player->jump();
}

void Scene::advancePlayer()
{
	player->advance();
}