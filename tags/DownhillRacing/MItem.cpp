#include "MItem.h"


MItem::MItem(Point p) : position(p)
{
}


MItem::~MItem()
{
}

void MItem::render (bool pname, float angle) {}

void MItem::setPosition(Point p) {
	position = p;
}