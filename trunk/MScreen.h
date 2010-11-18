#include <gl/glut.h>

#pragma once
class MScreen
{
public:
	MScreen(GLuint textureID);
	~MScreen(void);
private:
	GLuint texture;
};

