#include "MItem.h"


MItem::MItem(Point p) : position(p)
{
}


MItem::~MItem()
{
}

void MItem::render () {}

void MItem::setPosition(Point p) {
	position = p;
}