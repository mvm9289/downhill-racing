#include <vector>
using namespace std;

#pragma once
class Terrain
{
public:
	Terrain();
	~Terrain();

	void readLevelTxt();
	void readLevelXML();

	vector<int> heights;
};

