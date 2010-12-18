#include <gl/glut.h>

#include "MSelectable.h"
#include "MItem.h"
#include <vector>
using namespace std;

#define ACTION_START 1
#define ACTION_LEVEL_1 2
#define ACTION_LEVEL_2 3
#define ACTION_CREDITS 4
#define ACTION_BACK 5
#define ACTION_EXIT 6
#define ACTION_RESUME 7
#define ACTION_RESTART 8
#define ACTION_MENU 9
#define ACTION_HOW_TO 10

#pragma once
class MScreen
{
public:
	MScreen(GLuint textureID, float ratio);
	~MScreen();

	void render();

	void add(MItem *item);
	void up();
	void down();
	void setSelected(MSelectable *item);
	void setRatio(float ratio);

	int getAction();
private:
	int texture;
	float ra;
	vector<MItem*> items;
	MSelectable *selectedItem;
};

