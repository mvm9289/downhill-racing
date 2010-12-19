#include "IA.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

IA::IA(unsigned int n) : nPlayers(n)
{
	srand(clock());
	turns = vector<int>(n, 0);
}


IA::~IA(void)
{
}

int IA::compute(unsigned int i, vector<Player*> pl) {
	unsigned int ret = IA_NONE;
	unsigned int r = 0;
	int dX = pl[0]->getPosition().x - pl[i]->getPosition().x;
	for (int j = 1; j < pl.size(); ++j){
		if (j != i) dX += pl[0]->getPosition().x - pl[j]->getPosition().x;
	}
	int dY = pl[0]->getPosition().y - pl[i]->getPosition().y;
	int dZ = pl[0]->getPosition().z - pl[i]->getPosition().z;

	if (dZ < 0) { //player > computer
		r = rand() % 100;
		if (abs(dZ) > 10 && r > 95) ret += IA_TURBO;
		r = rand() % 100;
		if (abs(dZ) < 10 && r > 30 && !pl[0]->isJumping() && !pl[i]->isJumping()) ret += IA_JUMP;
	}
	else { //computer > player
		r = rand() % 100;
		if (abs(dZ) < 5 && r > 95) ret += IA_TURBO;
		r = rand() % 100;
		if (abs(dZ) < 5 && r > 80 && pl[0]->isJumping() && !pl[i]->isJumping()) ret += IA_JUMP;
	}

	if (turns[i] > 0) {
		--turns[i];
		ret += IA_TURN_RIGHT;
	}
	else if (turns[i] < 0) {
		++turns[i];
		ret += IA_TURN_LEFT;
	}
	else {
		r = rand() % 100;
		if (dZ < 0  && dX > 0 && r > 25) ret += IA_TURN_LEFT;
		else if (dZ < 0 && dX < 0 && r > 25) ret += IA_TURN_RIGHT;
		else if (dZ > 0 && dX > 0 && r > 25) ret += IA_TURN_RIGHT;
		else if (dZ > 0 && dX < 0 && r > 25) ret += IA_TURN_LEFT;
		/*
		r = rand() % 100;
		if (r > 60) {
			ret += IA_TURN_LEFT;
			turns[i] = -TURN_STEPS;
		}
		else if (r > 30) {
			ret += IA_TURN_RIGHT;
			turns[i] = TURN_STEPS;
		}
		*/
	}

	return ret;
}