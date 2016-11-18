#include "Brain.h"

Brain::Brain(unsigned short * keystate, int tab[sizeX * sizeY], float * tabCord, int nbPlayer, int playerNumber, int cpuLevel) {
	this->keystate = keystate;
	this->tab = tab;
	this->tabCord = tabCord;
	this->nbPlayer = nbPlayer;
	this->playerNumber = playerNumber;
	this->targetPlayer = -1;
	this->cpuLevel = cpuLevel;
	this->idxOwnBombe = -1;

	this->objectifIndex = 0;
	astar = new AStar(tab);
	bfs = new BFS(tab);
	prevDir = none;
	switch(this->cpuLevel){
		case 1:
			brainStep = lvl1CheckCanDropBomb;
			break;
		case 2:
			brainStep = lvl1CheckCanDropBomb;
			break;
		case 3:	
			brainStep = lvl1CheckCanDropBomb;
			break;
		case 4:
			brainStep = lvl1CheckCanDropBomb;
			break;
		case 5:	
			brainStep = lvl1CheckCanDropBomb;
			break;
	}
	srand (time(NULL));
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
		currentIndex = int(floor(tabCord[playerNumber * 2]) + floor(tabCord[playerNumber * 2 + 1]) * sizeX);
	}else{
		currentIndex = -1;
	}
	switch(this->cpuLevel){
		case 1:
			level1();
			break;
		case 2:
			level1();
			break;
		case 3:	
			level1();
			break;
		case 4:
			level1();
			break;
		case 5:	
			level1();
			break;
	}
}


/*
	lvl1CheckCanDropBomb = 0,	
	lvl1DropBomb = 1,
	lvl1GoSecure = 2,
	lvl1WaitBombeExplode = 3,
	lvl1FindNearWall = 4, 
	lvl1WalkToNearWall = 5
	lvl1WalkToWall = 6
*/


void Brain::level1() {
	
	switch(brainStep){
		case lvl1CheckCanDropBomb:
			bfs->resetCheckDropBombe();
			bfs->reset(false);
			objectifIndex = bfs->findNextBrick(currentIndex);
			if(objectifIndex!=-1){
				brainStep = lvl1WalkToNearWall;
			}else{
				fprintf(stderr,"here\n");
				
				if((rand() % 7 + 1)>=1) {
					while(true){
						objectifIndex = rand() % (sizeX * sizeY) + 1;
						if(tab[objectifIndex] == emptyElement) {
							brainStep = lvl1WalkToNearWall;
							break;	
						}

					}
				}else{
					brainStep = lvl1DropBomb;
				}
			}
			break;
		case lvl1WalkToNearWall:
			if(walkToObjectif(objectifIndex) == 1){
				if(bfs->checkDropBombe(currentIndex)){
					brainStep = lvl1DropBomb;
				}else{
					bfs->addIgnoreCase(objectifIndex);
					objectifIndex = bfs->findNextBrick(currentIndex);
				}
			}
			break;	
		case lvl1DropBomb:
				*keystate += brainKeyA;
				idxOwnBombe = currentIndex;
				brainStep = lvl1GoSecure;
			break;
		case lvl1GoSecure:
			bfs->resetSecure();
			objectifIndex = bfs->findSecure(currentIndex);
			if(objectifIndex >= 0){
				if(walkToObjectif(objectifIndex) == 0){
					brainStep = lvl1WaitBombeExplode;
				}
			}	
			break;
		case lvl1WaitBombeExplode:
			if(tab[idxOwnBombe] == 0){
				brainStep = lvl1FindNearWall;
			}
			break;
		case lvl1FindNearWall:
			bfs->reset(true);
			objectifIndex = bfs->findNextBrick(currentIndex);
			brainStep = lvl1CheckCanDropBomb;
			break;
		}
	
	
	
	

//	if (currentIndex != -1) {
//		
//		int wallIndex = 0;
//
//		bfs->reset();
//		wallIndex = bfs->solve(currentIndex, 3, false);
//
//		bfs->reset();
//		wallIndex = bfs->findNextBrick(currentIndex);
//
//		bfs->resetSecure();
//		int secureIndex = bfs->findSecure(currentIndex);
//
//		if(secureIndex >= 0){
//			walkToObjectif(secureIndex);
//		}else{
//			switch (wallIndex) {
//				case -1:
//					*keystate += (short) brainKeyUp;
//					break;
//				case -2:
//					*keystate += (short) brainKeyRight;
//					break;
//				case -3:
//					*keystate += (short) brainKeyDown;
//					break;
//				case -4:
//					*keystate += (short) brainKeyLeft;
//					break;
//			}	
//		}	
//	}	
	
}

void Brain::level2() {
	level1();
}

void Brain::level3() {
	level1();
}

void Brain::level4() {
	level1();
}

void Brain::level5() {
	level1();
}







int Brain::walkToObjectif(int objectif){

	astar->init(objectif % sizeX, floor(objectif / sizeX), tabCord[this->playerNumber * 2], tabCord[this->playerNumber * 2 + 1], 2);
	astar->solve();
	if (astar->isSolved()) {
		AStarCell current = astar->getEnd();
		AStarCell * parent;

		parent = current.getParent();
		
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
				
		int nbCaseRemaining = 0;
			
		while (true) {
			if (parent) {
//				fprintf(stderr, " ");
//				parent->printHimself();
				parent = parent->getParent();
				nbCaseRemaining++;
			} else {
				break;
			}
		}
		parent = NULL;
		return nbCaseRemaining;
	}
	return 0;
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
