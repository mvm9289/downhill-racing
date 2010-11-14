#pragma once

#include "Scene.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480

class Game
{
private:
	unsigned char keys[256];
	Scene scene;

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

