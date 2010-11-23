#include "MText.h"
#include <gl/glut.h>

// Render a string
void render_string(void* font, const char* string)
{
	int i,len = strlen(string);
	for(i=0;i<len;i++)
		glutBitmapCharacter(font, string[i]);
}

MText::MText(Point p, std::string str, float c[3], float cSel[3], bool sel) : MLabel(p, str, c), MSelectable(sel)
{
	memcpy(colorSel, cSel, 3*sizeof(float));
}


MText::~MText()
{
}

void MText::render() {
	if (selected) glColor3fv(colorSel);
	else glColor3fv(color);

	//glRasterPos2f(position.x, position.y);
	glRasterPos3f(3, 0, 7);
	render_string(GLUT_BITMAP_HELVETICA_18, text.c_str());
}