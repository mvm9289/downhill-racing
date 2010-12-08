#pragma once

#include "Scene.h"
#include "MScreen.h"
#include "Camera.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480

#define MENU 0
#define GAME 1
#define DEBUG 2

#define GLUT_KEY_NONE 0
#define GLUT_KEY_PRESS 1
#define GLUT_KEY_RELEASE 2

#define GLUT_KEY_SCAPE 27
#define GLUT_KEY_NUM1 49
#define GLUT_KEY_NUM2 50
#define GLUT_KEY_NUM3 51

class Game {
private:
	Scene scene;
	MScreen *currentScreen;
	Camera *menuCamera;
	Camera *gameCamera;
	Camera *debugCamera;
	Camera *currentCamera;
	float aspectRatio;
	int xAnt;
	int yAnt;

	unsigned char keys[256];

	unsigned int mode;
	void createMenus();
public:
	Game(void);
	virtual ~Game(void);

	bool Init();
	bool Loop();
	void Finalize();
	void Reshape(int width, int height);

	// Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	// Process
	bool Process();
	// Output
	void Render();
};

