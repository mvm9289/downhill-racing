#include "MLabel.h"


MLabel::MLabel(Point p, std::string str, float c1[3], float c2[3]) : MItem(p), text(str)
{
	cSel = c1;
	cNoSel = c2;
}


MLabel::~MLabel(void)
{
}
