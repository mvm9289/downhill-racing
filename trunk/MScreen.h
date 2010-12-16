#include <gl/glut.h>

#include "MSelectable.h"
#include "MItem.h"
#include <vector>
using namespace std;

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
private:
	int texture;
	float ra;
	vector<MItem*> items;
	MSelectable *selectedItem;
};

