#include "Scene.h"
#include "Texture.h"

Scene::Scene(void) {}

Scene::~Scene(void)
{
	vector<Player*>::iterator it = players.begin();
	for (; it != players.end(); ++it)
		if (*it) delete *it;
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

void Scene::render(float angle)
{
	level.render();
	vector<Player*>::iterator it = players.begin();
	for (; it != players.end(); ++it)
		(*it)->render(level.isHellMode(), angle);
}

bool Scene::init(string levelPath)
{
	bool loadOK = level.loadLevel(levelPath);
	if (!loadOK) return false;

	Player *player1 = new Player(0, level.startupPoint(), 1.25);
	Player *player2 = new Player(1, level.startupPoint() + Point(3, 0, 0), 1.12);
	Player *player3 = new Player(2, level.startupPoint() + Point(-3, 0, 0), 1.5);
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

	players.clear();
	players.push_back(player1);
	players.push_back(player2);
	players.push_back(player3);

	win = -1;

	return true;
}

void Scene::restartLevel()
{
	int n = players.size();
	for (int i = 0; i < n; i++) players[i]->init();
	win = -1;
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
	if (level.endPoint().z < players[i]->getPosition().z) players[i]->move(dx, players);
}

bool Scene::jumpPlayer(unsigned int i)
{
	if (level.endPoint().z < players[i]->getPosition().z) return players[i]->jump();
	return false;
}

int Scene::advancePlayers()
{
	for (unsigned int i = 0; i != players.size(); ++i) {
		if (level.endPoint().z < players[i]->getPosition().z) {
			players[i]->advance(players);
		}
		else {
			if (win < 0) win = i;
			if (!i && !win) return WINNER;
			else if (!i && win) return LOSER;
		}
	}
	return NOTHING;
}

bool Scene::stopPlayer(unsigned int i) {
	return players[i]->stopPlayer();
}

bool Scene::turboPlayer(unsigned int i) {
	if (level.endPoint().z < players[i]->getPosition().z) return players[i]->activateTurbo();
	return false;
}