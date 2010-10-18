#include "Box.h"

Box::Box(const Point& minim, const Point& maxim):minimum(minim), maximum(maxim) {}

Box::~Box(void) {}

void Box::init(const Point& p)
{
	minimum = p;
	maximum = p;
}

void Box::update(const Point& p)
{
	if (p.x < minimum.x) minimum.x=p.x;
	if (p.y < minimum.y) minimum.y=p.y;
	if (p.z < minimum.z) minimum.z=p.z;
	if (p.x > maximum.x) maximum.x=p.x;
	if (p.y > maximum.y) maximum.y=p.y;
	if (p.z > maximum.z) maximum.z=p.z;
}