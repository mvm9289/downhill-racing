#include "Scene.h"
#include "Texture.h"

Scene::Scene(void) {}

Scene::~Scene(void)
{
	vector<Player*>::iterator it = players.begin();
	for (; it != players.end(); ++it)
		delete *it;
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
	vector<Player*>::iterator it = players.begin();
	for (; it != players.end(); ++it)
		(*it)->render();
}

bool Scene::init(string levelPath)
{
	bool loadOK = level.loadLevel(levelPath);
	if (!loadOK) return false;

	Player *player1 = new Player(level.startupPoint());
	Player *player2 = new Player(level.startupPoint() + Point(3, 0, 0), 1.2);
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

	players.push_back(player1);
	players.push_back(player2);

	return true;
}

Point Scene::getPlayerPosition()
{
	return players[0]->getPosition();
}

void Scene::movePlayer(float dx)
{
	players[0]->move(dx, players, 0);
}

void Scene::jumpPlayer()
{
	players[0]->jump();
}

void Scene::advancePlayer()
{
	for (int i = 0; i != players.size(); ++i)
		players[i]->advance(players, i);
}