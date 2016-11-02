#include "Brain.h"

Brain::Brain(unsigned short * keystate, int tab[sizeX * sizeY], float * tabCord, int nbPlayer, int playerNumber, int cpuLevel) {
	this->keystate = keystate;
	this->tab = tab;
	this->tabCord = tabCord;
	this->nbPlayer = nbPlayer;
	this->playerNumber = playerNumber;
	this->targetPlayer = -1;
	this->cpuLevel = cpuLevel;
//	nbTick = 0;
	astar = new AStar(tab);
	bfs = new BFS(tab);
	prevDir = none;
	dropBombe = false;
}

Brain::~Brain() {
	free (astar);
	free (bfs);
	keystate = NULL;
	tab = NULL;
	tabCord = NULL;
}

void Brain::think() {
	*keystate = (short) 0;
	
	if(dropBombe){
		//*keystate += brainKeyA;
		dropBombe = false;
	}

	if (tabCord[playerNumber * 2] != -1) {
		int startIndex = int(floor(tabCord[playerNumber * 2]) + floor(tabCord[playerNumber * 2 + 1]) * sizeX);
		int wallIndex = 0;

		bfs->reset();
		wallIndex = bfs->solve(startIndex, 3, false);

		bfs->reset();
		wallIndex = bfs->findNextBrick(startIndex);

		bfs->resetSecure();
		int secureIndex = bfs->findSecure(startIndex);

		if(secureIndex >= 0){
			walkToObjectif(secureIndex);
		}else{
			switch (wallIndex) {
				case -1:
					*keystate += (short) brainKeyUp;
					break;
				case -2:
					*keystate += (short) brainKeyRight;
					break;
				case -3:
					*keystate += (short) brainKeyDown;
					break;
				case -4:
					*keystate += (short) brainKeyLeft;
					break;
			}	
		}	
	}
}

int Brain::walkToObjectif(int objectif){
	astar->init(objectif % sizeX, floor(objectif / sizeX), tabCord[this->playerNumber * 2], tabCord[this->playerNumber * 2 + 1], 2);
	astar->solve();
	if (astar->isSolved()) {
		AStarCell current = astar->getEnd();
		AStarCell * parent;

		parent = current.getParent();
		
		if((int)floor(tabCord[this->playerNumber * 2]) +  (int)floor(tabCord[this->playerNumber * 2 + 1]) * sizeX == objectif){
			dropBombe = true;
		}
		
		if (parent) {
			if (parent->getX() > current.getX()) {
				*keystate += (short) brainKeyRight;
				prevDir = right;
			} else if (parent->getX() < current.getX()) {
				*keystate += (short) brainKeyLeft;
				prevDir = left;
			} else if (parent->getY() < current.getY()) {
				*keystate += (short) brainKeyUp;
				prevDir = up;
			} else if (parent->getY() > current.getY()) {
				*keystate += (short) brainKeyDown;
				prevDir = down;
			}
		}else{
			switch(prevDir){
				case right:
					*keystate += (short) brainKeyRight;
					if(tabCord[this->playerNumber * 2] - floor(tabCord[this->playerNumber * 2]) > 0.4){
						prevDir = none;	
					}
				break;
				case left:
					*keystate += (short) brainKeyLeft;
					if(tabCord[this->playerNumber * 2] - floor(tabCord[this->playerNumber * 2]) < 0.6){
						prevDir = none;	
					}
				break;
				case up:
					*keystate += (short) brainKeyUp;
					if(tabCord[this->playerNumber * 2+1] - floor(tabCord[this->playerNumber * 2+1]) < 0.6){
						prevDir = none;
					}
				break;
				case down:
					*keystate += (short) brainKeyDown;
					if(tabCord[this->playerNumber * 2+1] - floor(tabCord[this->playerNumber * 2+1]) > 0.4){
						prevDir = none;
					}
				break;
			}
		}
			
		while (true) {
			if (parent) {
				fprintf(stderr, " ");
				parent->printHimself();
				parent = parent->getParent();
			} else {
				break;
			}
		}
		parent = NULL;
	}
}


int Brain::findNearPlayer() {
	float minDistance = -1.0;
	float res = 0.0;
	int target = -1;
	for (int i = 0; i < 16; i++) {
		if (playerNumber != i && tabCord[i * 2] != -1) {

			res = calcDistance(tabCord[playerNumber * 2], tabCord[playerNumber * 2 + 1], tabCord[i * 2], tabCord[i * 2 + 1]);
			if (minDistance == -1.0 || res < minDistance) {
				minDistance = res;
				target = i;
			}

		}
	}
	return target;
}

float Brain::calcDistance(int x1, int y1, int x2, int y2) {
	if (x1 == x2) {
		return abs(y1 - y2);
	} else if (y1 == y2) {
		return abs(x1 - x2);
	} else {
		return sqrt(pow(abs(x1 - x2), 2) + pow(abs(y1 - y2), 2));
	}
}
