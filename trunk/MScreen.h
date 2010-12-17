#include <gl/glut.h>

#include "MSelectable.h"
#include "MItem.h"
#include <vector>
using namespace std;

#define ACTION_START 1
#define ACTION_LEVEL_1 2
#define ACTION_LEVEL_1 3
#define ACTION_CREDITS

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

