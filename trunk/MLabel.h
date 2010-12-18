#pragma once
#include "mitem.h"
#include <string>

class MLabel : public MItem
{
public:
	MLabel(Point p, std::string str, float c1[3], float s, float lw);
	~MLabel();
	void render(bool pname = false, float angle = 0.0);
protected:
	std::string text;
	float color[3];
	float scale;
	float lineWidth;

	void render_string(void* font, const char* string);
};

