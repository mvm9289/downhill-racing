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
	vector<Player*>::iterator it = players.begin();
	for (; it != players.end(); ++it)
		(*it)->render();
}

bool Scene::init(string levelPath)
{
	bool loadOK = level.loadLevel(levelPath);
	if (!loadOK) return false;

	Player *player1 = new Player(0, level.startupPoint(), 1.2, false);
	Player *player2 = new Player(1, level.startupPoint() + Point(3, 0, 0), 1);
	Player *player3 = new Player(2, level.startupPoint() + Point(-3, 0, 0), 1);
	Texture playerTexture1;
	Texture playerTexture2;
	Texture playerTexture3;
	playerTexture1.load("textures/player4.png", GL_RGBA);
	playerTexture2.load("textures/player1.png", GL_RGBA);
	playerTexture3.load("textures/player3.png", GL_RGBA);
	player1->setTextureID(playerTexture1.getID());
	player2->setTextureID(playerTexture2.getID());
	player3->setTextureID(playerTexture3.getID());

	player1->setTerrain(level.getTerrain());
	player2->setTerrain(level.getTerrain());
	player3->setTerrain(level.getTerrain());
	player1->init();
	player2->init();
	player3->init();

	players.push_back(player1);
	players.push_back(player2);
	players.push_back(player3);

	return true;
}

vector<Player*> Scene::getPlayers() {
	return players;
}

Point Scene::getPlayerPosition()
{
	return players[0]->getPosition();
}

void Scene::movePlayer(unsigned int i, float dx)
{
	players[i]->move(dx, players);
}

void Scene::jumpPlayer(unsigned int i)
{
	players[i]->jump();
}

void Scene::advancePlayers()
{
	for (unsigned int i = 0; i != players.size(); ++i)
		players[i]->advance(players);
}

void Scene::stopPlayer(unsigned int i) {
	players[i]->stopPlayer();
}

void Scene::turboPlayer(unsigned int i) {
	players[i]->activateTurbo();
}