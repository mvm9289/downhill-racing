#include "MText.h"
#include <gl/glut.h>

MText::MText(Point p, std::string str, float c[3], float cSel[3], float s, bool sel) : MLabel(p, str, c, s), MSelectable(sel)
{
	memcpy(colorSel, cSel, 3*sizeof(float));
}


MText::~MText()
{
}

void MText::render(bool pname, float angle) {
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		if (selected) glColor3fv(colorSel);
		else glColor3fv(color);

		glTranslatef(position.x, position.y, 0.1);
		glScalef(scale, scale, scale);
		render_string(GLUT_STROKE_ROMAN, text.c_str());
	glPopMatrix();
	glEnable(GL_LIGHTING);
}