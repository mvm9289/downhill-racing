#include "MText.h"
#include <gl/glut.h>

MText::MText(Point p, std::string str, float c[3], float cSel[3], bool sel) : MLabel(p, str, c), MSelectable(sel)
{
	memcpy(colorSel, cSel, 3*sizeof(float));
}


MText::~MText()
{
}

void MText::render() {
	glPushMatrix();
		if (selected) glColor3fv(colorSel);
		else glColor3fv(color);

		glTranslatef(position.x, position.y, 0.1);
		glScalef(0.005, 0.005, 0.005);
		render_string(GLUT_STROKE_ROMAN, text.c_str());
	glPopMatrix();
}