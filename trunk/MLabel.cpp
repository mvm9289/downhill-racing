#include "MLabel.h"

#include <gl\glut.h>

// Render a string
void MLabel::render_string(void* font, const char* string)
{
	int i,len = strlen(string);
	for(i=0;i<len;i++)
		glutBitmapCharacter(font, string[i]);
}

MLabel::MLabel(Point p, std::string str, float c1[3]) : MItem(p), text(str)
{
	memcpy(color, c1, 3*sizeof(float));
}


MLabel::~MLabel()
{
}

void MLabel::render() {
	glColor3fv(color);
	//glRasterPos2f(position.x, position.y);
	glRasterPos3f(3, 0, 7);
	render_string(GLUT_BITMAP_HELVETICA_18, text.c_str());
}
