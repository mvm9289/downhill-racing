#include "MLabel.h"

#include <gl\glut.h>

// Render a string
void MLabel::render_string(void* font, const char* string)
{
	int i,len = strlen(string);
	for(i=0;i<len;i++)
		glutStrokeCharacter(font, string[i]);
}

MLabel::MLabel(Point p, std::string str, float c1[3], float s) : MItem(p), text(str), scale(s)
{
	memcpy(color, c1, 3*sizeof(float));
}


MLabel::~MLabel()
{
}

void MLabel::render(bool pname, float angle) {
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glScalef(scale, scale, scale);
		if (pname) glRotatef(-angle, 1, 0, 0);
		glColor3fv(color);
		render_string(GLUT_STROKE_ROMAN, text.c_str());
	glPopMatrix();
	glEnable(GL_LIGHTING);
}
