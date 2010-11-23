#pragma once
#include "MLabel.h"
#include "MSelectable.h"

class MText : public MLabel, public MSelectable
{
public:
	MText(Point p, std::string str, float c[3], float cSel[3], bool sel = false);
	~MText();
	void render();
private:
	float colorSel[3];
};

