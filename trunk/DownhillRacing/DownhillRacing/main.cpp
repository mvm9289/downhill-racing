#include <iostream>
using namespace std;

#include "Terrain.h"

int main() {
	Terrain terra;
	terra.readLevelTxt();

	cout << "Terrain loader." << endl;
	cout << terra.heights[2] << endl;
	system("pause");
}