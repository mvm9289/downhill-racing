#include "MScreen.h"


MScreen::MScreen(GLuint textureID, float ratio)
{
	texture = textureID;
	ra = ratio;
}


MScreen::~MScreen()
{
}

void MScreen::add(MItem *item) {
	items.push_back(item);
}

void MScreen::render() {
	//glBindTexture(GL_TEXTURE_2D, texture);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glColor3f(0,0,0);
	glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2d(1, 0);
		glVertex3f(10.0*ra, 0, 0);
		glTexCoord2d(1, 1);
		glVertex3f(10.0*ra, 0, 10.0);
		glTexCoord2d(0, 1);
		glVertex3f(0, 0, 10.0);
	glEnd();
	
	for (vector<MItem*>::iterator it = items.begin(); it != items.end(); ++it)
		(*it)->render();
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}