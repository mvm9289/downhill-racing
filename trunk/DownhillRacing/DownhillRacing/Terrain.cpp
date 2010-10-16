#include "Terrain.h"
#include <fstream>
using namespace std;

Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::readLevelTxt() {
	ifstream inputFile;
	inputFile.open("levels/level1.txt");

	if (inputFile.is_open()) {
		while (!inputFile.eof()) {
			int aux;
			inputFile >> aux;
			heights.push_back(aux);
		}
		inputFile.close();
	}
}

void Terrain::readLevelXML() {

}