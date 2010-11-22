#pragma once
#include "mitem.h"
#include <string>

class MLabel : public MItem
{
public:
	MLabel(Point p);
	MLabel(Point p, std::string str, float c1[3], float c2[3]);
	~MLabel(void);
	void render();
private:
	std::string text;
	float cSel[3];
	float cNoSel[3];
};

