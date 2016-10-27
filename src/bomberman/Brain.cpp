#include "Brain.h"

Brain::Brain(unsigned short * keystate, int tab[sizeX * sizeY], float * tabCord, int nbPlayer, int playerNumber) {
	this->keystate = keystate;
	this->tab = tab;
	this->tabCord = tabCord;
	this->nbPlayer = nbPlayer;
	this->playerNumber = playerNumber;
	this->targetPlayer = -1;
//	nbTick = 0;
	astar = new AStar(tab);
	bfs = new BFS(tab);
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

	if (tabCord[playerNumber * 2] != -1) {
		int startIndex = int(floor(tabCord[playerNumber * 2]) + floor(tabCord[playerNumber * 2 + 1]) * sizeX);
		int wallIndex = 0;

		bfs->reset();
		wallIndex = bfs->solve(startIndex, 3, false);
//		fprintf(stderr, "br %i %i ", playerNumber, wallIndex);

		bfs->reset();
		wallIndex = bfs->findNextBrick(startIndex);
//		fprintf(stderr, "%i ", wallIndex);

		bfs->resetSecure();
		int secureIndex = bfs->findSecure(startIndex);
//			bfs->printTestedSecure();

//	targetPlayer = findNearPlayer();

//	astar->init(tabCord[targetPlayer * 2], tabCord[targetPlayer * 2 + 1], tabCord[this->playerNumber * 2], tabCord[this->playerNumber * 2 + 1], 2);
//	astar->solve();

		if (secureIndex != -1) {
			astar->init(secureIndex % sizeX, floor(secureIndex / sizeX), tabCord[this->playerNumber * 2], tabCord[this->playerNumber * 2 + 1], 2);
			astar->solve();
			if (astar->isSolved()) {
				AStarCell current = astar->getEnd();
				AStarCell * parent;
				//current.printHimself();
				parent = current.getParent();

				//			if(parent->getFinalCost() == 0){
				//				*keystate += brainKeyA;
				//			}

				if (parent) {
					if (parent->getX() > current.getX()) {
						*keystate += (short) brainKeyRight;
					} else if (parent->getX() < current.getX()) {
						*keystate += (short) brainKeyLeft;
					} else if (parent->getY() < current.getY()) {
						*keystate += (short) brainKeyUp;
					} else if (parent->getY() > current.getY()) {
						*keystate += (short) brainKeyDown;
					}
				}

				while (true) {
					if (parent) {
//							fprintf(stderr, " ");
//							parent->printHimself();
						parent = parent->getParent();
					} else {
						break;
					}
				}
				parent = NULL;

//				fprintf(stderr, "\n");

//			} else {

//				fprintf(stderr, "NO PATH\n");

			}
		}
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
