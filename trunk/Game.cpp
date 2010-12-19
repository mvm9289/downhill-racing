#include "Game.h"

#include "Sphere.h"
#include "Player.h"
#include "Texture.h"

#include "MLabel.h"
#include "MText.h"

#include <cmath>
#include <gl/glut.h>

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
	sJump->release();
	sPause->release();
	sMenu->release();
	sys->close();
	sys->release();
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
	Point obs(10.0*aspectRatio/2.0, 5, 1);
	Point vrp(10.0*aspectRatio/2.0, 5, 0);
	Vector up(0, 1, 0);
	menuCamera = new Camera(obs, vrp, up, -10.0*aspectRatio/2.0, 10.0*aspectRatio/2.0, -5, 5, 0.1, 2);

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
	waitRelease = 0;
	
	initMusic();

	return true;
}

void Game::initMusic() {
	//FMOD init
	result = FMOD::System_Create(&sys);
	if (result == FMOD_OK) {
		unsigned int version;
		result = sys->getVersion(&version);
		if (result == FMOD_OK && version >= FMOD_VERSION) {
			result = sys->init(32, FMOD_INIT_NORMAL, 0);
			if (result == FMOD_OK) {
				result = sys->createSound("sounds/jump.wav", FMOD_HARDWARE, 0, &sJump);
				result = sys->createSound("sounds/pause.mp3", FMOD_HARDWARE, 0, &sPause);
				result = sys->createSound("sounds/menu.mp3", FMOD_HARDWARE, 0, &sMenu);
				result = sys->createSound("sounds/turbo.wav", FMOD_HARDWARE, 0, &sTurbo);
				result = sys->createSound("sounds/stop.wav", FMOD_HARDWARE, 0, &sStop);
				result = sys->createSound("sounds/level1.mp3", FMOD_HARDWARE, 0, &sL1);
				result = sys->createSound("sounds/level2.mp3", FMOD_HARDWARE, 0, &sL2);
				result = sys->createSound("sounds/level3.mp3", FMOD_HARDWARE, 0, &sL3);
				sMenu->setMode(FMOD_LOOP_NORMAL);
				sL1->setMode(FMOD_LOOP_NORMAL);
				sL2->setMode(FMOD_LOOP_NORMAL);
				sL3->setMode(FMOD_LOOP_NORMAL);
			}
		}
	}
	sys->playSound(FMOD_CHANNEL_FREE, sMenu, false, &cMenu);
}

void Game::createMainMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};
	float c3[] = {1, 1, 0};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	mainScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Downhill Racing", c3, 0.01, 15);
	
	//create menu items
	MText *opPlay = new MText(Point(5*aspectRatio, 6, 0), "Play", c1, c2, 0.005, 3, true);
	MText *opOptions = new MText(Point(5*aspectRatio, 5, 0), "Screen options", c1, c2, 0.005, 3);
	MText *opHowTo = new MText(Point(5*aspectRatio, 4, 0), "How to play", c1, c2, 0.005, 3);
	MText *opCredits = new MText(Point(5*aspectRatio, 3, 0), "Credits", c1, c2, 0.005, 3);
	MText *opExit = new MText(Point(5*aspectRatio, 1.5, 0), "Exit", c1, c2, 0.005, 3);

	//set menu directions
	opPlay->setUp(opExit);
	opPlay->setDown(opOptions);
	opOptions->setUp(opPlay);
	opOptions->setDown(opHowTo);
	opHowTo->setUp(opOptions);
	opHowTo->setDown(opCredits);
	opCredits->setUp(opHowTo);
	opCredits->setDown(opExit);
	opExit->setUp(opCredits);
	opExit->setDown(opPlay);

	//set menu actions
	opPlay->setAction(ACTION_START);
	opOptions->setAction(ACTION_OPTIONS);
	opHowTo->setAction(ACTION_HOW_TO);
	opCredits->setAction(ACTION_CREDITS);
	opExit->setAction(ACTION_EXIT);

	//add to main screen
	mainScreen->add(mainTitle);
	mainScreen->add(opPlay);
	mainScreen->add(opOptions);
	mainScreen->add(opHowTo);
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
	float c3[] = {1, 1, 0};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	levelsScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Choose a level", c3, 0.01, 15);
	
	//create menu items
	MText *level1 = new MText(Point(5*aspectRatio, 6, 0), "Joy garden", c1, c2, 0.005, 3, true);
	MText *level2 = new MText(Point(5*aspectRatio, 5, 0), "Space race", c1, c2, 0.005, 3);
	MText *level3 = new MText(Point(5*aspectRatio, 4, 0), "Infernal hell", c1, c2, 0.005, 3);
	MText *opBack = new MText(Point(5*aspectRatio, 1.5, 0), "Back", c1, c2, 0.005, 3);

	//set menu directions
	level1->setUp(opBack);
	level1->setDown(level2);
	level2->setUp(level1);
	level2->setDown(level3);
	level3->setUp(level2);
	level3->setDown(opBack);
	opBack->setUp(level3);
	opBack->setDown(level1);

	//set menu actions
	level1->setAction(ACTION_LEVEL_1);
	level2->setAction(ACTION_LEVEL_2);
	level3->setAction(ACTION_LEVEL_3);
	opBack->setAction(ACTION_BACK);

	//add to main screen
	levelsScreen->add(mainTitle);
	levelsScreen->add(level1);
	levelsScreen->add(level2);
	levelsScreen->add(level3);
	levelsScreen->add(opBack);
	levelsScreen->setSelected(level1);
}

void Game::createHowToMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};
	float c3[] = {1, 1, 0};
	float c4[] = {1, 0.5, 0};
	float c5[] = {1, 0, 1};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	howToScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "How to play", c3, 0.01, 15);

	//create menu items
	MLabel *keyboard = new MLabel(Point(4.5*aspectRatio, 6.7, 0.1), "Keyboard", c4, 0.003, 2.2);
	MLabel *xboxControl = new MLabel(Point(6.5*aspectRatio, 6.7, 0.1), "Xbox Controller", c4, 0.003, 2.2);
	MLabel *moveLeft = new MLabel(Point(2*aspectRatio, 6, 0.1), "Move left", c1, 0.003, 2.2);
	MLabel *leftArrow = new MLabel(Point(4.4*aspectRatio, 6, 0.1), "Left arrow", c5, 0.003, 2.2);
	MLabel *joystickLeft = new MLabel(Point(6.6*aspectRatio, 6, 0.1), "Joystick left", c5, 0.003, 2.2);
	MLabel *moveRight = new MLabel(Point(2*aspectRatio, 5.5, 0.1), "Move right", c1, 0.003, 2.2);
	MLabel *rightArrow = new MLabel(Point(4.32*aspectRatio, 5.5, 0.1), "Right arrow", c5, 0.003, 2.2);
	MLabel *joystickRight = new MLabel(Point(6.5*aspectRatio, 5.5, 0.1), "Joystick right", c5, 0.003, 2.2);
	MLabel *jump = new MLabel(Point(2*aspectRatio, 5, 0.1), "Jump", c1, 0.003, 2.2);
	MLabel *spaceBar = new MLabel(Point(4.4*aspectRatio, 5, 0.1), "Space bar", c5, 0.003, 2.2);
	MLabel *aButton = new MLabel(Point(6.8*aspectRatio, 5, 0.1), "A button", c5, 0.003, 2.2);
	MLabel *turbo = new MLabel(Point(2*aspectRatio, 4.5, 0.1), "Turbo", c1, 0.003, 2.2);
	MLabel *upArrow = new MLabel(Point(4.46*aspectRatio, 4.5, 0.1), "Up arrow", c5, 0.003, 2.2);
	MLabel *xButton = new MLabel(Point(6.8*aspectRatio, 4.5, 0.1), "X button", c5, 0.003, 2.2);
	MLabel *stop = new MLabel(Point(2*aspectRatio, 4, 0.1), "Stop", c1, 0.003, 2.2);
	MLabel *downArrow = new MLabel(Point(4.32*aspectRatio, 4, 0.1), "Down arrow", c5, 0.003, 2.2);
	MLabel *bButton = new MLabel(Point(6.8*aspectRatio, 4, 0.1), "B button", c5, 0.003, 2.2);
	MLabel *pause = new MLabel(Point(2*aspectRatio, 3.5, 0.1), "Pause", c1, 0.003, 2.2);
	MLabel *escape = new MLabel(Point(4.65*aspectRatio, 3.5, 0.1), "Escape", c5, 0.003, 2.2);
	MLabel *startButton = new MLabel(Point(6.6*aspectRatio, 3.5, 0.1), "Start button", c5, 0.003, 2.2);
	MText *opBack = new MText(Point(5*aspectRatio, 1.5, 0), "Back", c1, c2, 0.005, 3, true);

	//set menu directions
	opBack->setUp(opBack);
	opBack->setDown(opBack);

	//set menu actions
	opBack->setAction(ACTION_BACK);

	//add to main screen
	howToScreen->add(mainTitle);
	howToScreen->add(keyboard);
	howToScreen->add(xboxControl);
	howToScreen->add(moveLeft);
	howToScreen->add(leftArrow);
	howToScreen->add(joystickLeft);
	howToScreen->add(moveRight);
	howToScreen->add(rightArrow);
	howToScreen->add(joystickRight);
	howToScreen->add(jump);
	howToScreen->add(spaceBar);
	howToScreen->add(aButton);
	howToScreen->add(turbo);
	howToScreen->add(upArrow);
	howToScreen->add(xButton);
	howToScreen->add(stop);
	howToScreen->add(downArrow);
	howToScreen->add(bButton);
	howToScreen->add(pause);
	howToScreen->add(escape);
	howToScreen->add(startButton);
	howToScreen->add(opBack);
	howToScreen->setSelected(opBack);
}

void Game::createCreditsMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};
	float c3[] = {1, 1, 0};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	creditsScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Credits", c3, 0.01, 15);

	//create menu items
	MLabel *name1 = new MLabel(Point(5*aspectRatio, 5, 0.1), "Albert Arnedo", c1, 0.005, 3);
	MLabel *name2 = new MLabel(Point(5*aspectRatio, 4, 0.1), "Miguel Angel Vico", c1, 0.005, 3);
	MText *opBack = new MText(Point(5*aspectRatio, 1.5, 0), "Back", c1, c2, 0.005, 3, true);

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
	float c3[] = {1, 1, 0};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	pauseScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Game paused", c3, 0.01, 15);

	//create menu items
	MText *opResume = new MText(Point(5*aspectRatio, 5, 0), "Resume", c1, c2, 0.005, 3, true);
	MText *opRestart = new MText(Point(5*aspectRatio, 4, 0), "Restart", c1, c2, 0.005, 3);
	MText *opMenu = new MText(Point(5*aspectRatio, 1.5, 0), "Go to menu", c1, c2, 0.005, 3);

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

void Game::createWinMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};
	float c3[] = {1, 1, 0};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	winScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "You win!", c3, 0.01, 15);

	//create menu items
	MText *opRestart = new MText(Point(5*aspectRatio, 5, 0), "Restart", c1, c2, 0.005, 3, true);
	MText *opMenu = new MText(Point(5*aspectRatio, 1.5, 0), "Go to menu", c1, c2, 0.005, 3);

	//set menu directions
	opRestart->setUp(opMenu);
	opRestart->setDown(opMenu);
	opMenu->setUp(opRestart);
	opMenu->setDown(opRestart);

	//set menu actions
	opRestart->setAction(ACTION_RESTART);
	opMenu->setAction(ACTION_MENU);

	//add to main screen
	winScreen->add(mainTitle);
	winScreen->add(opRestart);
	winScreen->add(opMenu);
	winScreen->setSelected(opRestart);
}

void Game::createLoseMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};
	float c3[] = {1, 1, 0};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	loseScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "You lose!", c3, 0.01, 15);

	//create menu items
	MText *opRestart = new MText(Point(5*aspectRatio, 5, 0), "Restart", c1, c2, 0.005, 3, true);
	MText *opMenu = new MText(Point(5*aspectRatio, 1.5, 0), "Go to menu", c1, c2, 0.005, 3);

	//set menu directions
	opRestart->setUp(opMenu);
	opRestart->setDown(opMenu);
	opMenu->setUp(opRestart);
	opMenu->setDown(opRestart);

	//set menu actions
	opRestart->setAction(ACTION_RESTART);
	opMenu->setAction(ACTION_MENU);

	//add to main screen
	loseScreen->add(mainTitle);
	loseScreen->add(opRestart);
	loseScreen->add(opMenu);
	loseScreen->setSelected(opRestart);
}

void Game::createOptionsMenu()
{
	float c1[] = {1, 0, 0};
	float c2[] = {1, 1, 1};
	float c3[] = {1, 1, 0};

	//Menu screens
	Texture menuTexture;
	menuTexture.load("textures/bkg.png", GL_RGBA);
	optionsScreen = new MScreen(menuTexture.getID(), aspectRatio);

	//title
	MLabel *mainTitle = new MLabel(Point(1.5*aspectRatio, 8, 0.1), "Screen options", c3, 0.01, 15);

	//create menu items
	MText *fullscreen = new MText(Point(5*aspectRatio, 6, 0), "Fullscreen", c1, c2, 0.005, 3);
	MText *windowed = new MText(Point(5*aspectRatio, 5, 0), "Windowed", c1, c2, 0.005, 3);
	MText *opBack = new MText(Point(5*aspectRatio, 1.5, 0), "Back", c1, c2, 0.005, 3, true);

	//set menu directions
	fullscreen->setUp(opBack);
	fullscreen->setDown(windowed);
	windowed->setUp(fullscreen);
	windowed->setDown(opBack);
	opBack->setUp(windowed);
	opBack->setDown(fullscreen);

	//set menu actions
	fullscreen->setAction(ACTION_FULLSCREEN);
	windowed->setAction(ACTION_WINDOWED);
	opBack->setAction(ACTION_BACK);

	//add to main screen
	optionsScreen->add(mainTitle);
	optionsScreen->add(fullscreen);
	optionsScreen->add(windowed);
	optionsScreen->add(opBack);
	optionsScreen->setSelected(opBack);
}

void Game::createMenus()
{
	createMainMenu();
	createLevelsMenu();
	createCreditsMenu();
	createPauseMenu();
	createHowToMenu();
	createWinMenu();
	createLoseMenu();
	createOptionsMenu();
}

void Game::createGUI() {
	float c1[] = {1, 0, 0};
	float c2[] = {0, 1, 0};

	MLabel *p1 = new MLabel(Point(0, 0, 0), "Player", c2, 0.003, 2);
	MLabel *p2 = new MLabel(Point(0, 0, 0), "Comp 1", c1, 0.003, 2);
	MLabel *p3 = new MLabel(Point(0, 0, 0), "Comp 2", c1, 0.003, 2);
	pNames.push_back(p1);
	pNames.push_back(p2);
	pNames.push_back(p3);
}

Vector Game::getYobs()
{
	glMatrixMode(GL_MODELVIEW);
	float modelview[4][4];
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelview[0][0]);
	Vector yobs(modelview[1][0], modelview[1][1], modelview[1][2]);
	yobs.normalize();

	return yobs;
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
	Point obs = scene.getPlayerPosition() + Vector(0, 5, 10);
	Point vrp = scene.getPlayerPosition() + Vector(0, 2, 0);
	Vector up(0, 1, 0);
	if (gameCamera) delete gameCamera;
	gameCamera = new Camera(center, obs, vrp, up, 60, aspectRatio, 0.1, 3*radius);
	gameCamera->init();
	currentCamera = gameCamera;
	mode = GAME;
	gameStarted = true;
	// Debug camera
	debugCamera = new Camera(center, obs, vrp, up, 60, aspectRatio, 0.1, 3*radius);
}


bool Game::loadLevel(string level)
{
	cMenu->setPaused(true);
	sys->playSound(FMOD_CHANNEL_FREE, sLevel, false, &cLevel);
	
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
	int status;
	int res_x, res_y, pos_x, pos_y;
	
	// Process Input
	//if (keys[GLUT_KEY_SCAPE]) res = false;

	switch(mode) {
	case MENU:
		// Xbox
		if (!waitRelease && gamepad->IsConnected()) {
			float mv = gamepad->GetState().Gamepad.sThumbLY/(2*32767.0);
			if (mv > 0.1) {
				currentScreen->up();
				waitRelease = XBOX_WAIT;
			}
			else if (mv < -0.1) {
				currentScreen->down();
				waitRelease = XBOX_WAIT;
			}
		}

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
		if (keys[GLUT_KEY_ENTER] == GLUT_KEY_RELEASE ||
			(!waitRelease && gamepad->IsConnected() && (gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)))
		{
			waitRelease = XBOX_WAIT;
			int action = currentScreen->getAction();
			switch(action)
			{
			case ACTION_START:
				currentScreen = levelsScreen;
				break;
			case ACTION_LEVEL_1:
				cMenu->setPaused(true);
				sLevel = sL1;
				res = loadLevel("levels/level1.txt");
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case ACTION_LEVEL_2:
				cMenu->setPaused(true);
				sLevel = sL2;
				res = loadLevel("levels/level2.txt");
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case ACTION_LEVEL_3:
				cMenu->setPaused(true);
				sLevel = sL3;
				res = loadLevel("levels/level3.txt");
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case ACTION_RESUME:
				cMenu->setPaused(true);
				cLevel->setPaused(false);
				channel->setPaused(false);
				mode = GAME;
				currentCamera = gameCamera;
				currentCamera->init();
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case ACTION_RESTART:
				cMenu->setPaused(true);
				cLevel->stop();
				sys->playSound(FMOD_CHANNEL_FREE, sLevel, false, &cLevel);
				channel->stop();
				scene.restartLevel();
				initCameras();
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case ACTION_MENU:
				currentScreen = mainScreen;
				gameStarted = false;
				break;
			case ACTION_OPTIONS:
				currentScreen = optionsScreen;
				break;
			case ACTION_FULLSCREEN:
				glutFullScreen();
				break;
			case ACTION_WINDOWED:
				res_x = glutGet(GLUT_SCREEN_WIDTH);
				res_y = glutGet(GLUT_SCREEN_HEIGHT);
				pos_x = (res_x>>1)-(GAME_WIDTH>>1);
				pos_y = (res_y>>1)-(GAME_HEIGHT>>1);
				glutPositionWindow(pos_x, pos_y);
				glutReshapeWindow(GAME_WIDTH, GAME_HEIGHT);
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
			case ACTION_HOW_TO:
				currentScreen = howToScreen;
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
		status = scene.advancePlayers();
		switch (status)
		{
			case WINNER:
				cMenu->setPaused(false);
				cLevel->stop();
				currentScreen = winScreen;
				currentCamera = menuCamera;
				currentCamera->init();
				mode = MENU;
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			case LOSER:
				cMenu->setPaused(false);
				cLevel->stop();
				currentScreen = loseScreen;
				currentCamera = menuCamera;
				currentCamera->init();
				mode = MENU;
				for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
				break;
			default:
				if (!scene.getPlayers()[0]->getBlocked()) currentCamera->move(scene.getPlayerPosition());
				break;
		}

		// Process Input
		// Keyboard Input
		if (keys[GLUT_KEY_LEFT] == GLUT_KEY_PRESS) scene.movePlayer(0, -0.2);
		else keys[GLUT_KEY_LEFT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_RIGHT] == GLUT_KEY_PRESS) scene.movePlayer(0, 0.2);
		else keys[GLUT_KEY_RIGHT] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_UP] == GLUT_KEY_PRESS) {
			if (scene.turboPlayer(0))
				sys->playSound(FMOD_CHANNEL_FREE, sTurbo, false, &channel);
		}
		else keys[GLUT_KEY_UP] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_DOWN] == GLUT_KEY_PRESS) {
			if (scene.stopPlayer(0) && !stopWait) {
				sys->playSound(FMOD_CHANNEL_FREE, sStop, false, &channel);
				stopWait = 50;
			}
		}
		else keys[GLUT_KEY_DOWN] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_SPACE] == GLUT_KEY_PRESS) {
			if (scene.jumpPlayer(0))
				sys->playSound(FMOD_CHANNEL_FREE, sJump, false, &channel);
		}
		else keys[GLUT_KEY_SPACE] = GLUT_KEY_NONE;
		if (keys[GLUT_KEY_F11] == GLUT_KEY_RELEASE)
		{
			mode = DEBUG;
			currentCamera = debugCamera;
			for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
		}
		if (keys[GLUT_KEY_SCAPE] == GLUT_KEY_RELEASE)
		{
			cLevel->setPaused(true);
			channel->setPaused(true);
			cMenu->setPaused(false);
			sys->playSound(FMOD_CHANNEL_FREE, sPause, false, &channel);
			currentScreen = pauseScreen;
			currentCamera = menuCamera;
			currentCamera->init();
			mode = MENU;
			for (int i = 0; i < 256; i++) keys[i] = GLUT_KEY_NONE;
		}

		// XBOX Input
		if (gamepad->IsConnected()) {
			if (!waitRelease && gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				if (scene.jumpPlayer(0))
					sys->playSound(FMOD_CHANNEL_FREE, sJump, false, &channel);
			}

			float mv = gamepad->GetState().Gamepad.sThumbLX/(2*32767.0);
			if (mv > 0.1 || mv < -0.1) scene.movePlayer(0, mv);

			if (gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X) {
				if (scene.turboPlayer(0))
					sys->playSound(FMOD_CHANNEL_FREE, sTurbo, false, &channel);
			}
			if (gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B) {
				if (scene.stopPlayer(0) && !stopWait) {
					sys->playSound(FMOD_CHANNEL_FREE, sStop, false, &channel);
					stopWait = 50;
				}
			}
			if (gamepad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START)
			{
				cLevel->setPaused(true);
				cMenu->setPaused(false);
				channel->setPaused(true);
				sys->playSound(FMOD_CHANNEL_FREE, sPause, false, &channel);
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

	if (stopWait) --stopWait;
	if (waitRelease) --waitRelease;

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
		Vector camDir = currentCamera->getVisionDir();
		float angle = acos(-camDir.z)*180.0/3.14159265;
		if (camDir.y > 0)  angle = acos(camDir.z)*180.0/3.14159265 + 180;

		scene.render(angle);

		Vector yobs = getYobs();
		yobs.z = -yobs.z;
		yobs.x -= 0.4;

		vector<Player*> pl = scene.getPlayers();
		vector<Player*>::iterator it = pl.begin();
		vector<MLabel*>::iterator lit = pNames.begin();
		for (; it != pl.end(); ++it, ++lit) {
			Point p = (*it)->getPosition() + yobs*(*it)->getRadius()*1.2;
			(*lit)->setPosition(p);
			(*lit)->render(true, angle);
		}
	}

	glutSwapBuffers();
}
