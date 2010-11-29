#include "Game.h"
#include <gl/glut.h>

#include "Sphere.h"
#include "Player.h"
#include "Texture.h"

#include "MLabel.h"
#include "MText.h"

Sphere *skydome;
Player *player;

Game::Game(void)
{
}

Game::~Game(void)
{
	delete player;
	delete skydome;
	delete currentScreen;
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

	glLineWidth(4.0);

	ra = (float)GAME_WIDTH/(float)GAME_HEIGHT;
	setProjection();

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

	createMenus();
	mode = MENU;

	return res;
}

void Game::createMenus() {
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	MScreen *mainScreen = new MScreen(menuTexture.getID(), ra);
	MScreen *creditsScreen = new MScreen(menuTexture.getID(), ra);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*ra, 8, 0), "Downhill Racing", c1);
	
	//create menu items
	MText *opPlay = new MText(Point(4.5*ra, 5, 0), "Play", c1, c2, true);
	MText *opOptions = new MText(Point(4*ra, 4, 0), "Options", c1, c2);
	MText *opCredits = new MText(Point(4*ra, 3, 0), "Credits", c1, c2);

	//set menu directions
	opPlay->setUp(opCredits);
	opPlay->setDown(opOptions);
	opOptions->setUp(opPlay);
	opOptions->setDown(opCredits);
	opCredits->setUp(opOptions);
	opCredits->setDown(opPlay);

	//add to main screen
	mainScreen->add(mainTitle);
	mainScreen->add(opPlay);
	mainScreen->add(opOptions);
	mainScreen->add(opCredits);
	mainScreen->setSelected(opPlay);

	//set current menu screen
	currentScreen = mainScreen;
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
		glOrtho(-10.0*ra/2.0, 10.0*ra/2.0, -5, 5, 0.1, 15);
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
	keyUp = (keys[GLUT_KEY_UP] && !press);
	keyDown = (keys[GLUT_KEY_DOWN] && !press);
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
		if (keyUp) {
			currentScreen->up();
			keyUp = false;
		}
		if (keyDown) {
			currentScreen->down();
			keyDown = false;
		}
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
		gluLookAt(10.0*ra/2.0, 5, 10, 10.0*ra/2.0, 5, 0, 0, 1, 0);
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
		glColor3f(1, 0, 0); glVertex3f(0,0,0); glVertex3f(100, 0, 0);
		glColor3f(0, 1, 0); glVertex3f(0,0,0); glVertex3f(0, 100, 0);
		glColor3f(0, 0, 1); glVertex3f(0,0,0); glVertex3f(0, 0, 100);
	glEnd();
	glEnable(GL_LIGHTING);

	if (mode == MENU) {
		//MENUS
		currentScreen->render();
	}
	else {
		currentScreen->render();
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
