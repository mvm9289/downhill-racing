#pragma once
class MSelectable
{
public:
	MSelectable(bool sel = false);
	~MSelectable();

	void setUp(MSelectable *_up);
	void setDown(MSelectable *_down);

	MSelectable* getUp(bool select = false);
	MSelectable* getDown(bool select = false);

	void setSelected(bool s);
	void setAction(int a);
	int getAction();
protected:
	bool selected;
	MSelectable *up;
	MSelectable *down;
	int action;
};

