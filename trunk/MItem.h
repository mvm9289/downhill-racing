#include "Point.h"

#pragma once
class MItem
{
public:
	MItem(Point p);
	~MItem();
	virtual void render();
protected:
	Point position;
};

