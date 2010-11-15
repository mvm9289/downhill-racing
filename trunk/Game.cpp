#include "Game.h"
#include <gl/glut.h>

#include "Sphere.h"
#include "Player.h"
Sphere *skydome;
Player *player;

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
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glEnable(GL_LIGHTING); 
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	float r;
	Point c;
	scene.boundingSphere(c, r);
	skydome = new Sphere(c, r);
	player = new Player(Point(0,0,0));
  
	res = scene.init("levels/level1.txt");

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

	gluLookAt(center.x+2*radius, center.y+2*radius, center.z, center.x, center.y, center.z, -1, 1, 0);
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

	scene.render();

	//glCullFace(GL_FRONT);
	skydome->render();
	//glCullFace(GL_BACK);

	glColor3f(1, 0, 0);
	player->render();

	glutSwapBuffers();
}
