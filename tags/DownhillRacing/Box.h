#pragma once

#include "Point.h"

class Box
{
public:
	Point minimum;
	Point maximum;

public:
	Box(const Point& minim=Point(), const Point& maxim=Point());
	~Box(void);
	void init(const Point& p);
	void update(const Point& p);
};
