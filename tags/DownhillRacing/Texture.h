#pragma once

#include <gl/glut.h>

#pragma comment (lib, "corona")

class Texture
{
private:
	GLuint id;
	int width;
	int height;

public:
	Texture(void);
	virtual ~Texture(void);

	bool load(char *filename, int type = GL_RGBA, int wraps = GL_REPEAT, int wrapt = GL_REPEAT,
		int magf = GL_LINEAR, int minf = GL_LINEAR, bool mipmap = false);
	int getID();
	void getSize(int *w, int *h);
};
