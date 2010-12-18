#include "Game.h"

#include "Sphere.h"
#include "Player.h"
#include "Texture.h"

#include "MLabel.h"
#include "MText.h"

#include <cmath>
#include <gl/glut.h>

#include <iostream>
using namespace std;

Game::Game(void) : ai(NUM_PLAYERS)
{
}

Game::~Game(void)
{
	if (mainScreen) delete mainScreen;
	if (levelsScreen) delete levelsScreen;
	if (creditsScreen) delete creditsScreen;
	if (pauseScreen) delete pauseScreen;
	if (menuCamera) delete menuCamera;
	if (gameCamera) delete gameCamera;
	if (debugCamera) delete debugCamera;
	if (gamepad) delete gamepad;
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

	float ambient[] = {1, 1, 1, 1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glLineWidth(2.0);

	gameStarted = false;

	// Cameras initialization
	aspectRatio = (float)GAME_WIDTH/(float)GAME_HEIGHT;

	// Menu camera
	Point obs(10.0*aspectRatio/2.0, 5, 10);
	Point vrp(10.0*aspectRatio/2.0, 5, 0);
	Vector up(0, 1, 0);
	menuCamera = new Camera(obs, vrp, up, -10.0*aspectRatio/2.0, 10.0*aspectRatio/2.0, -5, 5, 0.1, 15);

	// Menus initialization
	createMenus();
	currentScreen->setRatio(aspectRatio);

	// GUI initialization
	createGUI();

	// Game mode initialization
	mode = MENU;
	currentCamera = menuCamera;
	currentCamera->init();

	//XBOX controller
	gamepad = new CXBOXController(1);

	return true;
}

void Game::createMainMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	mainScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Downhill Racing", c1, 0.01);
	
	//create menu items
	MText *opPlay = new MText(Point(5*aspectRatio, 5, 0), "Play", c1, c2, 0.005, true);
	MText *opCredits = new MText(Point(5*aspectRatio, 4, 0), "Credits", c1, c2, 0.005);
	MText *opExit = new MText(Point(5*aspectRatio, 2, 0), "Exit", c1, c2, 0.005);

	//set menu directions
	opPlay->setUp(opExit);
	opPlay->setDown(opCredits);
	opCredits->setUp(opPlay);
	opCredits->setDown(opExit);
	opExit->setUp(opCredits);
	opExit->setDown(opPlay);

	//set menu actions
	opPlay->setAction(ACTION_START);
	opCredits->setAction(ACTION_CREDITS);
	opExit->setAction(ACTION_EXIT);

	//add to main screen
	mainScreen->add(mainTitle);
	mainScreen->add(opPlay);
	mainScreen->add(opCredits);
	mainScreen->add(opExit);
	mainScreen->setSelected(opPlay);

	//set current menu screen
	currentScreen = mainScreen;
}

void Game::createLevelsMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	levelsScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Choose a level", c1, 0.01);
	
	//create menu items
	MText *level1 = new MText(Point(5*aspectRatio, 5, 0), "Level 1", c1, c2, 0.005, true);
	MText *level2 = new MText(Point(5*aspectRatio, 4, 0), "Level 2", c1, c2, 0.005);
	MText *opBack = new MText(Point(5*aspectRatio, 2, 0), "Back", c1, c2, 0.005);

	//set menu directions
	level1->setUp(opBack);
	level1->setDown(level2);
	level2->setUp(level1);
	level2->setDown(opBack);
	opBack->setUp(level2);
	opBack->setDown(level1);

	//set menu actions
	level1->setAction(ACTION_LEVEL_1);
	level2->setAction(ACTION_LEVEL_2);
	opBack->setAction(ACTION_BACK);

	//add to main screen
	levelsScreen->add(mainTitle);
	levelsScreen->add(level1);
	levelsScreen->add(level2);
	levelsScreen->add(opBack);
	levelsScreen->setSelected(level1);
}

void Game::createCreditsMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	creditsScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Credits", c1, 0.01);

	//create menu items
	MLabel *name1 = new MLabel(Point(5*aspectRatio, 5, 0.1), "Albert Arnedo", c1, 0.005);
	MLabel *name2 = new MLabel(Point(5*aspectRatio, 4, 0.1), "Miguel Angel Vico", c1, 0.005);
	MText *opBack = new MText(Point(5*aspectRatio, 2, 0), "Back", c1, c2, 0.005, true);

	//set menu directions
	opBack->setUp(opBack);
	opBack->setDown(opBack);

	//set menu actions
	opBack->setAction(ACTION_BACK);

	//add to main screen
	creditsScreen->add(mainTitle);
	creditsScreen->add(name1);
	creditsScreen->add(name2);
	creditsScreen->add(opBack);
	creditsScreen->setSelected(opBack);
}

void Game::createPauseMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	pauseScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Game paused", c1, 0.01);

	//create menu items
	MText *opResume = new MText(Point(5*aspectRatio, 5, 0), "Resume", c1, c2, 0.005, true);
	MText *opRestart = new MText(Point(5*aspectRatio, 4, 0), "Restart", c1, c2, 0.005);
	MText *opMenu = new MText(Point(5*aspectRatio, 2, 0), "Go to menu", c1, c2, 0.005);

	//set menu directions
	opResume->setUp(opMenu);
	opResume->setDown(opRestart);
	opRestart->setUp(opResume);
	opRestart->setDown(opMenu);
	opMenu->setUp(opRestart);
	opMenu->setDown(opResume);

	//set menu actions
	opResume->setAction(ACTION_RESUME);
	opRestart->setAction(ACTION_RESTART);
	opMenu->setAction(ACTION_MENU);

	//add to main screen
	pauseScreen->add(mainTitle);
	pauseScreen->add(opResume);
	pauseScreen->add(opRestart);
	pauseScreen->add(opMenu);
	pauseScreen->setSelected(opResume);
}

void Game::createMenus()
{
	createMainMenu();
	createLevelsMenu();
	createCreditsMenu();
	createPauseMenu();
}

void Game::createGUI() {
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};

	MLabel *p1 = new MLabel(Point(0, 0, 0), "Player", c1, 0.003);
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

void Game::initCameras()
{
	// Game camera
	float radius;
	Point center;
	scene.boundingSphere(center, radius);
	Point obs = scene.getPlayerPosition() + Vector(0, 5, 7);
	Point vrp = scene.getPlayerPosition() + Vector(0, 2, 0);
	Vector up(0, 1, 0);
	if (gameCamera) delete gameCamera;
	gameCamera = new Camera(center, obs, vrp, up, 60, aspectRatio, 0.1, 5*radius);
	gameCamera->init();
	currentCamera = gameCamera;
	mode = GAME;
	gameStarted = true;
	// Debug camera
	debugCamera = new Camera(center, obs, vrp, up, 60, aspectRatio, 0.1, 5*radius);
}


bool Game::loadLevel(string level)
{
	if (scene.init(level))
	{
		initCameras();
		return true;
	}
	return false;
}

// Process
bool Game::Process()
{
	bool res = true;
	
	// Process Input
	//if (keys[GLUT_KEY_SCAPE]) res = false;

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
		if (keys[GLUT_KEY_ENTER] == GLUT_KEY_RELEASE)
		{
			int action = currentScreen->getAction();
			switch(action)
			{
			case ACTION_START:
				currentScreen = levelsScreen;
				break;
			case ACTION_LEVEL_1:
				res = loadLevel("levels/level1.txt");
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case ACTION_LEVEL_2:
				res = loadLevel("levels/level2.txt");
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case ACTION_RESUME:
				mode = GAME;
				currentCamera = gameCamera;
				currentCamera->init();
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case ACTION_RESTART:
				scene.restartLevel();
				initCameras();
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case ACTION_MENU:
				currentScreen = mainScreen;
				gameStarted = false;
				break;
			case ACTION_CREDITS:
				currentScreen = creditsScreen;
				break;
			case ACTION_BACK:
				currentScreen = mainScreen;
				break;
			case ACTION_EXIT:
				res = false;
				break;
			default:
				break;
			}
			keys[GLUT_KEY_ENTER] = GLUT_KEY_NONE;
		}
		break;
	case GAME:
		// Game logic
		for (unsigned int i = 1; i < scene.getPlayers().size(); ++i) {
			int val = ai.compute(i, scene.getPlayers());
			if (val & IA_TURBO) {
				scene.turboPlayer(i);
			}
			if (val & IA_JUMP) {
				scene.jumpPlayer(i);
			}
			if (val & IA_TURN_LEFT) {
				scene.movePlayer(i, -0.2);
			}
			if (val & IA_TURN_RIGHT) {
				scene.movePlayer(i, 0.2);
			}
			if (val & IA_STOP) {
				scene.stopPlayer(i);
			}
		}
		scene.advancePlayers();
		if (!scene.getPlayers()[0]->getBlocked()) currentCamera->move(scene.getPlayerPosition());

		// Process Input
		// Keyboard Input
		if (keys[GLUT_KEY_LEFT] == GLUT_KEY_PRESS) scene.movePlayer(0, -0.2);
		else keys[GLUT_KEY_LEFT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_RIGHT] == GLUT_KEY_PRESS) scene.movePlayer(0, 0.2);
		else keys[GLUT_KEY_RIGHT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_UP] == GLUT_KEY_PRESS) scene.turboPlayer(0);
		else keys[GLUT_KEY_UP] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_DOWN] == GLUT_KEY_PRESS) scene.stopPlayer(0);
		else keys[GLUT_KEY_DOWN] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_SPACE] == GLUT_KEY_PRESS) scene.jumpPlayer(0);
		else keys[GLUT_KEY_SPACE] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_F11] == GLUT_KEY_RELEASE)
		{
			mode = DEBUG;
			currentCamera = debugCamera;
			for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
		}
		if (keys[GLUT_KEY_SCAPE] == GLUT_KEY_RELEASE)
		{
			currentScreen = pauseScreen;
			currentCamera = menuCamera;
			currentCamera->init();
			mode = MENU;
			for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
		}

		// XBOX Input
		if (gamepad->IsConnected()) {
			if (gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) scene.jumpPlayer(0);
			float mv = gamepad->GetState().Gamepad.sThumbLX/(2*32767.0);
			if (mv > 0.1 || mv < -0.1) scene.movePlayer(0, mv);
			if (gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X) scene.turboPlayer(0);
			if (gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) scene.stopPlayer(0);
			if (gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START)
			{
				currentScreen = pauseScreen;
				currentCamera = menuCamera;
				currentCamera->init();
				mode = MENU;
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
			}
		}
		break;
	case DEBUG:
		// Process Input
		if (keys[GLUT_KEY_LEFT] == GLUT_KEY_PRESS) currentCamera->move(-2, 0.);
		else keys[GLUT_KEY_LEFT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_RIGHT] == GLUT_KEY_PRESS) currentCamera->move(2, 0.);
		else keys[GLUT_KEY_RIGHT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_UP] == GLUT_KEY_PRESS) currentCamera->move(0., -2);
		else keys[GLUT_KEY_UP] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_DOWN] == GLUT_KEY_PRESS) currentCamera->move(0., 2);
		else keys[GLUT_KEY_DOWN] = GLUT_KEY_NONE;

		if (keys['j'] == GLUT_KEY_PRESS) currentCamera->rotate(0.0, -1.0);
		else keys['j'] = GLUT_KEY_NONE;
		if (keys['l'] == GLUT_KEY_PRESS) currentCamera->rotate(0.0, 1.0);
		else keys['l'] = GLUT_KEY_NONE;
		if (keys['i'] == GLUT_KEY_PRESS) currentCamera->rotate(-1.0, 0.0);
		else keys['i'] = GLUT_KEY_NONE;
		if (keys['k'] == GLUT_KEY_PRESS) currentCamera->rotate(1.0, 0.0);
		else keys['k'] = GLUT_KEY_NONE;

		if (keys[GLUT_KEY_F11] == GLUT_KEY_RELEASE)
		{
			mode = GAME;
			currentCamera = gameCamera;
			for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
		}
		break;
	default:
		break;
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
	else if (gameStarted) {
		scene.render();

		vector<Player*> pl = scene.getPlayers();
		vector<Player*>::iterator it = pl.begin();
		vector<MLabel*>::iterator lit = pNames.begin();
		
		Vector camDir = currentCamera->getVisionDir();
		float angle = acos(-camDir.z)*180.0/3.14159265;
		//if (camDir.y > 0)  angle = acos(camDir.z)*180.0/3.14159265;

		for (; it != pl.end(); ++it, ++lit) {
			Point p = (*it)->getPosition() + Vector(-0.5, 1.5, 0);
			(*lit)->setPosition(p);
			(*lit)->render(true, angle);
		}
	}

	glutSwapBuffers();
}
