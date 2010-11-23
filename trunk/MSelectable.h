#pragma once
class MSelectable
{
public:
	MSelectable(bool sel = false);
	~MSelectable();
protected:
	bool selected;
};

