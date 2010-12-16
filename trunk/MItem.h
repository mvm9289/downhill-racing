#include "Point.h"

#pragma once
class MItem
{
public:
	MItem(Point p);
	~MItem();
	virtual void render();
	void setPosition(Point p);
protected:
	Point position;
};

