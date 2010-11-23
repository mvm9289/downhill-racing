#include "MLabel.h"

#include <gl\glut.h>

// Render a string
void MLabel::render_string(void* font, const char* string)
{
	int i,len = strlen(string);
	for(i=0;i<len;i++)
		glutStrokeCharacter(font, string[i]);
}

MLabel::MLabel(Point p, std::string str, float c1[3]) : MItem(p), text(str)
{
	memcpy(color, c1, 3*sizeof(float));
}


MLabel::~MLabel()
{
}

void MLabel::render() {
	glPushMatrix();
	glColor3fv(color);
	//glRasterPos2f(position.x, position.y);
	//glRasterPos3f(3, 0, 7);
	glTranslatef(position.x, position.y, 0);
	glScalef(0.1, 0.1, 0.1);
	render_string(GLUT_STROKE_ROMAN, text.c_str());
	glPopMatrix();
}
