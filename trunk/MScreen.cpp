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

void MScreen::up() {
	if (selectedItem) {
		selectedItem = selectedItem->getUp();
		selectedItem->setSelected(true);
	}
}

void MScreen::down() {
	if (selectedItem) {
		selectedItem = selectedItem->getDown();
		selectedItem->setSelected(true);
	}
}

void MScreen::setSelected(MSelectable *item) {
	selectedItem = item;
}

void MScreen::setRatio(float ratio) {
	ra = ratio;
}

void MScreen::render() {
	//background
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glMatrixMode(GL_MODELVIEW);
	if (texture > 0) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glEnable(GL_TEXTURE_2D);
		
		glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2d(0, 0);
			glVertex3f(0, 0, 0);
			glTexCoord2d(1, 0);
			glVertex3f(10.0*ra, 0, 0);
			glTexCoord2d(1, 1);
			glVertex3f(10.0*ra, 10.0, 0);
			glTexCoord2d(0, 1);
			glVertex3f(0, 10.0, 0);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	
	//items
	for (vector<MItem*>::iterator it = items.begin(); it != items.end(); ++it)
		(*it)->render();
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}