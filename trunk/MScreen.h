#include <gl/glut.h>

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
private:
	GLuint texture;
	float ra;
	vector<MItem*> items;
};

