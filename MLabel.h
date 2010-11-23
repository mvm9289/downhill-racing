#pragma once
#include "mitem.h"
#include <string>

class MLabel : public MItem
{
public:
	MLabel(Point p, std::string str, float c1[3]);
	~MLabel();
	void render();
protected:
	std::string text;
	float color[3];
};

