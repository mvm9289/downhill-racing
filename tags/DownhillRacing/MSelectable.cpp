#include "MSelectable.h"


MSelectable::MSelectable(bool sel) : selected(sel)
{
}


MSelectable::~MSelectable()
{
}

void MSelectable::setUp(MSelectable *_up) {
	up = _up;
}

void MSelectable::setDown(MSelectable *_down) {
	down = _down;
}

MSelectable* MSelectable::getUp(bool select) {
	selected = select;
	return up;
}

MSelectable* MSelectable::getDown(bool select) {
	selected = select;
	return down;
}

void MSelectable::setSelected(bool s) {
	selected = s;
}

void MSelectable::setAction(int a) {
	action = a;
}

int MSelectable::getAction() {
	return action;
}