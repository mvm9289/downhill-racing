#include "MLabel.h"

#include <gl\glut.h>

// Render a string
void render_string(void* font, const char* string)
{
	int i,len = strlen(string);
	for(i=0;i<len;i++)
		glutBitmapCharacter(font, string[i]);
}

MLabel::MLabel(Point p) : MItem(p) {}

MLabel::MLabel(Point p, std::string str, float c1[3], float c2[3]) : MItem(p), text(str)
{
	memcpy(cSel, c1, 3*sizeof(float));
	memcpy(cSel, c2, 3*sizeof(float));
}


MLabel::~MLabel(void)
{
}

void MLabel::render() {
	render_string(GLUT_BITMAP_HELVETICA_10, text.c_str());
}
