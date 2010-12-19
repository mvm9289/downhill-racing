#include "Point.h"

#pragma once
class MItem
{
public:
	MItem(Point p);
	~MItem();
	virtual void render(bool pname = false, float angle = 0.0);
	void setPosition(Point p);
protected:
	Point position;
};

