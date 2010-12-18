#pragma once
#include "MLabel.h"
#include "MSelectable.h"

class MText : public MLabel, public MSelectable
{
public:
	MText(Point p, std::string str, float c[3], float cSel[3], float s, float lw, bool sel = false);
	~MText();
	void render(bool pname = false, float angle = 0.0);
private:
	float colorSel[3];
};

