#include "Game.h"
#include <gl/glut.h>

#include "Sphere.h"
#include "Player.h"
#include "Texture.h"

Sphere *skydome;
Player *player;

Game::Game(void)
{
}

Game::~Game(void)
{
	delete player;
	delete skydome;
	delete mainScreen;
}

bool Game::Init()
{
	bool res = true;

	// Graphics initialization
	glClearColor(0.35f, 0.35f, 0.6f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	player = new Player(Point(5, 31, 0));

	//Menus creation
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};
	//title = new MLabel(Point(1, 1, 0), "HOLA", c1, c2);
	Texture menuTexture;
	menuTexture.load("textures/sky1.png", GL_RGBA);
	mainScreen = new MScreen(menuTexture.getID(), 4.0/3.0);

	mode = MENU;

	return res;
}

bool Game::Loop()
{
	bool res = true;

	int t1 = glutGet(GLUT_ELAPSED_TIME);
	res = Process();
	if(res) Render();

	int t2;
	do {
		t2 = glutGet(GLUT_ELAPSED_TIME);
	} while (t2 - t1 < 20); //50 fps => 20 ms = 1000ms/50

	return res;
}

void Game::Finalize()
{
}

void Game::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	ra = (float)width/(float)height;
	setProjection();
}

void Game::setProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (mode == MENU) {
		glOrtho(0, 10*ra + 1, 0, 10 + 1, 0.1, 15);
	}
	else {
		float radius;
		Point center;
		scene.boundingSphere(center, radius);

		gluPerspective(60, ra, 0.1, 5*radius);
	}
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
	if (keys[GLUT_KEY_SCAPE]) res = false;

	switch(mode) {
	case MENU:
		break;
	case GAME:
		if (keys[GLUT_KEY_LEFT]) player->move(-0.2);
		if (keys[GLUT_KEY_RIGHT]) player->move(0.2);
		if (keys[GLUT_KEY_UP]) player->jump();
		break;
	case DEBUG:
		break;
	}

	if (keys[GLUT_KEY_NUM1]) {mode = MENU; setProjection();}
	if (keys[GLUT_KEY_NUM2]) {mode = GAME; setProjection();}
	if (keys[GLUT_KEY_NUM3]) {mode = DEBUG; setProjection();}

	return res;
}

// Output
void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//CAMERA SETTINGS
	float radius;
	Point center;
	scene.boundingSphere(center, radius);

	Point OBS = player->getPosition() + Vector(0, 1, 10);
	Point VRP = player->getPosition() + Vector(0, 2, 0);
	
	switch (mode) {
	case MENU:
		gluLookAt(10*ra/2, 10, 5, 10*ra/2, 0, 5, 0, 0, 1);
		break;
	case GAME:
		gluLookAt(OBS.x, OBS.y, OBS.z, VRP.x, VRP.y, VRP.z, 0, 1, 0);
		break;
	case DEBUG:
		gluLookAt(center.x + 2*radius, center.y + 2*radius, center.z, center.x, center.y, center.z, -1, 1, 0);
		break;
	}

	//Coordinate system
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1, 0, 0); glVertex3f(0,0,0); glVertex3f(1, 0, 0);
		glColor3f(0, 1, 0); glVertex3f(0,0,0); glVertex3f(0, 1, 0);
		glColor3f(0, 0, 1); glVertex3f(0,0,0); glVertex3f(0, 0, 1);
	glEnd();
	glEnable(GL_LIGHTING);

	if (mode == MENU) {
		//MENUS
		mainScreen->render();
	}
	else {
		//SCENE
		scene.render();

		//SKYDOME
		//desactivar iluminació i pintar amb textura
		glDisable(GL_LIGHTING);
		glCullFace(GL_FRONT);
		skydome->render();
		glCullFace(GL_BACK);
		glEnable(GL_LIGHTING);

		//PLAYERS
		glColor3f(1, 0, 0);
		player->render();
	}

	glutSwapBuffers();
}
