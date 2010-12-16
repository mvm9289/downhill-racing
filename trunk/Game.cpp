#include "Game.h"
#include <gl/glut.h>

#include "Sphere.h"
#include "Player.h"
#include "Texture.h"

#include "MLabel.h"
#include "MText.h"

Game::Game(void)
{
}

Game::~Game(void)
{
	delete currentScreen;
	delete menuCamera;
	delete gameCamera;
	delete debugCamera;
}

bool Game::Init()
{
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

	// Scene initialization
	if (!scene.init("levels/level3.txt")) return false; /////////////////////////solo cuando se selecciona un level

	// Cameras initialization
	aspectRatio = (float)GAME_WIDTH/(float)GAME_HEIGHT;

	// Menu camera
	Point obs(10.0*aspectRatio/2.0, 5, 10);
	Point vrp(10.0*aspectRatio/2.0, 5, 0);
	Vector up(0, 1, 0);
	menuCamera = new Camera(obs, vrp, up, -10.0*aspectRatio/2.0, 10.0*aspectRatio/2.0, -5, 5, 0.1, 15);

	// Game camera
	float radius;
	Point center;
	scene.boundingSphere(center, radius);
	obs = scene.getPlayerPosition() + Vector(0, 1, 10);
	vrp = scene.getPlayerPosition() + Vector(0, 2, 0);
	gameCamera = new Camera(center, obs, vrp, up, 60, aspectRatio, 0.1, 5*radius); /////////////////////////solo cuando se selecciona un level

	// Debug camera
	debugCamera = new Camera(center, obs, vrp, up, 60, aspectRatio, 0.1, 5*radius);

	xAnt = 0;
	yAnt = 0;

	// Menus initialization
	createMenus();
	currentScreen->setRatio(aspectRatio);

	// GUI initialization
	createGUI();

	// Game mode initialization
	mode = MENU;
	currentCamera = menuCamera;
	currentCamera->init();

	return true;
}

void Game::createMenus() {
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	MScreen *mainScreen = new MScreen(menuTexture.getID(), aspectRatio);
	MScreen *creditsScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Downhill Racing", c1, 0.01);
	
	//create menu items
	MText *opPlay = new MText(Point(4.5*aspectRatio, 5, 0), "Play", c1, c2, 0.005, true);
	MText *opOptions = new MText(Point(4*aspectRatio, 4, 0), "Options", c1, c2, 0.005);
	MText *opCredits = new MText(Point(4*aspectRatio, 3, 0), "Credits", c1, c2, 0.005);

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

void Game::createGUI() {
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};

	MLabel *p1 = new MLabel(Point(0, 0, 0), "Player", c2, 0.003);
	MLabel *p2 = new MLabel(Point(0, 0, 0), "Comp 1", c1, 0.003);
	MLabel *p3 = new MLabel(Point(0, 0, 0), "Comp 2", c1, 0.003);
	pNames.push_back(p1);
	pNames.push_back(p2);
	pNames.push_back(p3);
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
	aspectRatio = (float)width/(float)height;
	currentCamera->reshape(aspectRatio);
	if (currentScreen) currentScreen->setRatio(aspectRatio);
}

// Input
void Game::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = (press)?GLUT_KEY_PRESS:GLUT_KEY_RELEASE;
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
		// Process Input
		if (keys[GLUT_KEY_UP] == GLUT_KEY_RELEASE)
		{
			currentScreen->up();
			keys[GLUT_KEY_UP] = GLUT_KEY_NONE;
		}
		if (keys[GLUT_KEY_DOWN] == GLUT_KEY_RELEASE)
		{
			currentScreen->down();
			keys[GLUT_KEY_DOWN] = GLUT_KEY_NONE;
		}
		break;
	case GAME:
		// Process Input
		if (keys[GLUT_KEY_LEFT] == GLUT_KEY_PRESS) scene.movePlayer(-0.2);
		else keys[GLUT_KEY_LEFT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_RIGHT] == GLUT_KEY_PRESS) scene.movePlayer(0.2);
		else keys[GLUT_KEY_RIGHT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_UP] == GLUT_KEY_PRESS) scene.jumpPlayer();
		else keys[GLUT_KEY_UP] = GLUT_KEY_NONE;

		// Game logic
		scene.advancePlayer();
		currentCamera->move(scene.getPlayerPosition() + Vector(0, 2, 0));
		break;
	case DEBUG:
		// Process Input
		if (keys[GLUT_KEY_LEFT] == GLUT_KEY_PRESS) currentCamera->move(-0.2, 0.);
		else keys[GLUT_KEY_LEFT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_RIGHT] == GLUT_KEY_PRESS) currentCamera->move(0.2, 0.);
		else keys[GLUT_KEY_RIGHT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_UP] == GLUT_KEY_PRESS) currentCamera->move(0., -0.2);
		else keys[GLUT_KEY_UP] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_DOWN] == GLUT_KEY_PRESS) currentCamera->move(0., 0.2);
		else keys[GLUT_KEY_DOWN] = GLUT_KEY_NONE;

		if (keys['j'] == GLUT_KEY_PRESS) currentCamera->rotate(0.0, -1.0);
		else keys['j'] = GLUT_KEY_NONE;
		if (keys['l'] == GLUT_KEY_PRESS) currentCamera->rotate(0.0, 1.0);
		else keys['l'] = GLUT_KEY_NONE;
		if (keys['i'] == GLUT_KEY_PRESS) currentCamera->rotate(-1.0, 0.0);
		else keys['i'] = GLUT_KEY_NONE;
		if (keys['k'] == GLUT_KEY_PRESS) currentCamera->rotate(1.0, 0.0);
		else keys['k'] = GLUT_KEY_NONE;
		break;
	default:
		break;
	}

	if (keys[GLUT_KEY_NUM1] == GLUT_KEY_RELEASE)
	{
		mode = MENU;
		currentCamera = menuCamera;
		currentCamera->init();
		keys[GLUT_KEY_NUM1] = GLUT_KEY_NONE;
	}
	if (keys[GLUT_KEY_NUM2] == GLUT_KEY_RELEASE)
	{
		mode = GAME;
		currentCamera = gameCamera;
		currentCamera->init();
		keys[GLUT_KEY_NUM2] = GLUT_KEY_NONE;
	}
	if (keys[GLUT_KEY_NUM3] == GLUT_KEY_RELEASE)
	{
		mode = DEBUG;
		currentCamera = debugCamera;
		currentCamera->init();
		keys[GLUT_KEY_NUM3] = GLUT_KEY_NONE;
	}

	return res;
}

// Render a string
void render_string(void* font, const char* string)
{
	int i,len = strlen(string);
	for(i=0;i<len;i++)
		glutBitmapCharacter(font, string[i]);
}

// Output
void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (mode == MENU) currentScreen->render();
	else {
		scene.render();

		vector<Player*> pl = scene.getPlayers();
		vector<Player*>::iterator it = pl.begin();
		vector<MLabel*>::iterator lit = pNames.begin();

		for (; it != pl.end(); ++it, ++lit) {
			Point p = (*it)->getPosition() + Vector(-1, 1.5, 0);
			(*lit)->setPosition(p);
			(*lit)->render();
		}
	}

	glutSwapBuffers();
}
