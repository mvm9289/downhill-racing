#include "Game.h"
#include <gl/glut.h>

#include "Sphere.h"
#include "Player.h"
#include "MLabel.h"
#include "Texture.h"

Sphere *skydome;
Player *player;
MLabel *title;

Game::Game(void)
{
}

Game::~Game(void)
{
}

bool Game::Init()
{
	bool res = true;

	// Graphics initialization
	glClearColor(0.35f, 0.35f, 0.6f, 1.0f);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LIGHTING); 
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//Init scene and get bSphere
	res = scene.init("levels/level1.txt");
	float r;
	Point c;
	scene.boundingSphere(c, r);

	//Skydome creation
	skydome = new Sphere(c, r);
	Texture skydomeText;
	skydomeText.load("textures/sky2.png", GL_RGBA);
	skydome->setTextureID(skydomeText.getID());

	//Player creation
	player = new Player(Point(5, 30.5, 0));

	//Menus creation
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};
	title = new MLabel(Point(0,0,0), "HOLA", c1, c2);

	return res;
}

bool Game::Loop()
{
	bool res = true;

	res = Process();
	if(res) Render();

	return res;
}

void Game::Finalize()
{
}

void Game::Reshape(int width, int height)
{
	float radius;
	Point center;
	scene.boundingSphere(center, radius);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)width/(float)height, 0.1, 5*radius);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(center.x+2*radius, center.y+2*radius, center.z, center.x, center.y, center.z, -1, 1, 0);

	Point OBS = player->getPosition() - Vector(0, 1, 5);
	Point VRP = player->getPosition();
	gluLookAt(OBS.x, OBS.y, OBS.z, VRP.x, VRP.y, VRP.z, 0, 1, 0);
}

// Input
void Game::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void Game::ReadMouse(int button, int state, int x, int y)
{
}

// Process
bool Game::Process()
{
	bool res = true;

	// Process Input
	if(keys[27])	res = false;

	return res;
}

// Output
void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	//glLoadIdentity();

	//title->render();

	scene.render();

	//SKYDOME
	//desactivar iluminació i pintar amb textura
	glDisable(GL_LIGHTING);
	glCullFace(GL_FRONT);
	//glColor3f(1, 0, 0);
	skydome->render();
	glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);

	glColor3f(1, 0, 0);
	player->render();

	glutSwapBuffers();
}
