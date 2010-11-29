#pragma once

#include "Scene.h"
#include "MScreen.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480

#define MENU 0
#define GAME 1
#define DEBUG 2

#define GLUT_KEY_SCAPE 27
#define GLUT_KEY_NUM1 49
#define GLUT_KEY_NUM2 50
#define GLUT_KEY_NUM3 51

class Game {
private:
	unsigned char keys[256];
	bool keyUp;
	bool keyDown;
	Scene scene;

	MScreen *currentScreen;

	unsigned int mode;
	float ra;
	void setProjection();
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

