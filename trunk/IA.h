#include "Player.h"
#include <vector>
using namespace std;

#define IA_NONE 0x0
#define IA_TURN_LEFT 0x1
#define IA_TURN_RIGHT 0x2
#define IA_JUMP 0x4
#define IA_TURBO 0x8
#define IA_STOP 0x10

#define TURN_STEPS 25

#pragma once
class IA
{
public:
	IA(unsigned int n);
	~IA(void);
	int compute(unsigned int i, vector<Player*> pl);
private:
	unsigned int nPlayers;
	vector<int> turns;
};

