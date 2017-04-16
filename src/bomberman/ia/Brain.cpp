#include "Brain.h"

Brain::Brain(unsigned short * keystate, int tab[sizeX * sizeY], int playerNumber, Player * player) {
	this->player = player;
	this->keystate = keystate;
	this->tab = tab;
	this->playerNumber = playerNumber;
	this->targetPlayer = -1;
	this->idxOwnBombe = -1;

	this->objectifIndex = 0;
	astar = new AStar(tab);
	bfs = new BFS(tab);
	prevDir = none;
	switch (GameConfig::Instance().getIALevel()) {
		case 1:
			brainStep = lvl1CheckCanDropBomb;
			break;
		case 2:
			brainStep = lvl2CheckCanDropBomb;
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
	srand (time(NULL));}

Brain::~Brain() {
	delete astar;
	delete bfs;
	keystate = NULL;
	tab = NULL;
	player = NULL;
}

void Brain::think() {

	*keystate = (short) 0;

	if (GameConfig::Instance().getPlayerPosX(playerNumber) != -1) {
		currentIndex = GameConfig::Instance().getPlayerIndex(playerNumber);
	} else {
		currentIndex = -1;
	}
	if (currentIndex >= 0 && currentIndex < sizeX * sizeY) {
		if (tab[currentIndex] == wallElement) {
			//player blocked by a hole
			if (tab[currentIndex + 1] < brickElement) {
				*keystate += (short) brainKeyRight;
				prevDir = right;
			} else if (tab[currentIndex - 1] < brickElement) {
				*keystate += (short) brainKeyLeft;
				prevDir = left;
			} else if (tab[currentIndex - sizeX] < brickElement) {
				*keystate += (short) brainKeyUp;
				prevDir = up;
			} else if (tab[currentIndex + sizeX] < brickElement) {
				*keystate += (short) brainKeyDown;
				prevDir = down;
			}
		} else {
			switch (GameConfig::Instance().getIALevel()) {
				case 1:
					level1();
					break;
				case 2:
					level2();
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

	switch (brainStep) {
		case lvl1CheckCanDropBomb:
			bfs->resetCheckDropBombe();
			bfs->reset(false);
			objectifIndex = bfs->findNextBrick(currentIndex);
			if (objectifIndex != -1) {
				brainStep = lvl1WalkToNearWall;
			} else {
				if ((rand() % 7 + 1) >= 1) {
					while (true) {
						objectifIndex = rand() % (sizeX * sizeY) + 1;
						if (tab[objectifIndex] == emptyElement) {
							brainStep = lvl1WalkToNearWall;
							break;
						}

					}
				} else {
					brainStep = lvl1DropBomb;
				}
			}
			break;
		case lvl1WalkToNearWall:
			if (walkToObjectif(objectifIndex) == 1) {
				if (bfs->checkDropBombe(currentIndex)) {
					brainStep = lvl1DropBomb;
				} else {
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
			if (objectifIndex >= 0) {
				if (walkToObjectif(objectifIndex) == 0) {
					brainStep = lvl1WaitBombeExplode;
				}
			}
			break;
		case lvl1WaitBombeExplode:
			if (player->getBombeType() == radioBombeType) {
				player->brainPressButton();
			}
			if (tab[idxOwnBombe] == 0) {
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
	switch (brainStep) {
		case lvl2CheckCanDropBomb:
			bfs->resetCheckDropBombe();
			bfs->reset(false);
			objectifIndex = bfs->findNextBrick(currentIndex);
			if (objectifIndex != -1) {
				brainStep = lvl2WalkToNearWall;
			} else {
				if ((rand() % 7 + 1) >= 1) {
					while (true) {
						objectifIndex = rand() % (sizeX * sizeY) + 1;
						if (tab[objectifIndex] == emptyElement) {
							brainStep = lvl2WalkToNearWall;
							break;
						}

					}
				} else {
					brainStep = lvl2DropBomb;
				}
			}
			break;
		case lvl2WalkToNearWall:
			if (walkToObjectif(objectifIndex) == 1) {
				if (bfs->checkDropBombe(currentIndex)) {
					brainStep = lvl2DropBomb;
				} else {
					bfs->addIgnoreCase(objectifIndex);
					objectifIndex = bfs->findNextBrick(currentIndex);
				}
			}
			break;
		case lvl2DropBomb:
			*keystate += brainKeyA;
			idxOwnBombe = currentIndex;
			brainStep = lvl2GoSecure;
			break;
		case lvl2GoSecure:
			bfs->resetSecure();
			objectifIndex = bfs->findSecure(currentIndex);
			if (objectifIndex >= 0) {
				if (walkToObjectif(objectifIndex) == 0) {
					brainStep = lvl2WaitBombeExplode;
				}
			}
			break;
		case lvl2WaitBombeExplode:
			if (player->getBombeType() == radioBombeType) {
				player->brainPressButton();
			}
			if (tab[idxOwnBombe] == 0) {
				brainStep = lvl2FindNearWall;
				break;
			}
			bfs->resetSecure();
			objectifIndex = bfs->findSecure(currentIndex);
			if (objectifIndex >= 0) {
				brainStep = lvl2GoSecure;
			}
			break;
		case lvl2FindNearWall:
			bfs->reset(true);
			objectifIndex = bfs->findNextBrick(currentIndex);
			brainStep = lvl2CheckCanDropBomb;
			break;
	}
}

void Brain::level3() {
	switch (brainStep) {
		case lvl3CheckCanDropBomb:
			bfs->resetCheckDropBombe();
			bfs->reset(false);
			objectifIndex = bfs->findNextBrick(currentIndex);
			if (objectifIndex != -1) {
				brainStep = lvl3WalkToNearWall;
			} else {
				if ((rand() % 7 + 1) >= 1) {
					while (true) {
						objectifIndex = rand() % (sizeX * sizeY) + 1;
						if (tab[objectifIndex] == emptyElement) {
							brainStep = lvl3WalkToNearWall;
							break;
						}
					}
				} else {
					brainStep = lvl3DropBomb;
				}
			}
			break;
		case lvl3WalkToNearWall:
			if (walkToObjectif(objectifIndex) == 1) {
				if (bfs->checkDropBombe(currentIndex)) {
					brainStep = lvl3DropBomb;
				} else {
					bfs->addIgnoreCase(objectifIndex);
					objectifIndex = bfs->findNextBrick(currentIndex);
				}
			}
			break;
		case lvl3DropBomb:
			*keystate += brainKeyA;
			idxOwnBombe = currentIndex;
			brainStep = lvl3GoSecure;
			break;
		case lvl3GoSecure:
			bfs->resetSecure();
			objectifIndex = bfs->findSecure(currentIndex);
			if (objectifIndex >= 0) {
				if (walkToObjectif(objectifIndex) == 0) {
					brainStep = lvl3WaitBombeExplode;
				}
			}
			break;
		case lvl3WaitBombeExplode:
			if (player->getBombeType() == radioBombeType) {
				player->brainPressButton();
			}
			if (tab[idxOwnBombe] == 0) {
				brainStep = lvl3FindNearWall;
				break;
			}
			bfs->resetSecure();
			objectifIndex = bfs->findSecure(currentIndex);
			if (objectifIndex >= 0) {
				brainStep = lvl3GoSecure;
			}
			break;
		case lvl3FindNearWall:
			bfs->reset(true);
			objectifIndex = bfs->findNextBrick(currentIndex);
			brainStep = lvl3CheckCanDropBomb;
			break;
	}
}

void Brain::level4() {
	level1();
}

void Brain::level5() {
	level1();
}

int Brain::walkToObjectif(int objectif) {

	astar->init(objectif % sizeX, floor(objectif / sizeX), GameConfig::Instance().getPlayerPosX(this->playerNumber), GameConfig::Instance().getPlayerPosY(this->playerNumber), 2);
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
		} else {
			switch (prevDir) {
				case right:
					*keystate += (short) brainKeyRight;
					if (GameConfig::Instance().getPlayerPosX(this->playerNumber) - floor(GameConfig::Instance().getPlayerPosX(this->playerNumber)) > 0.4) {
						prevDir = none;
					}
					break;
				case left:
					*keystate += (short) brainKeyLeft;
					if (GameConfig::Instance().getPlayerPosX(this->playerNumber) - floor(GameConfig::Instance().getPlayerPosX(this->playerNumber)) < 0.6) {
						prevDir = none;
					}
					break;
				case up:
					*keystate += (short) brainKeyUp;
					if (GameConfig::Instance().getPlayerPosY(this->playerNumber) - floor(GameConfig::Instance().getPlayerPosY(this->playerNumber)) < 0.6) {
						prevDir = none;
					}
					break;
				case down:
					*keystate += (short) brainKeyDown;
					if (GameConfig::Instance().getPlayerPosY(this->playerNumber) - floor(GameConfig::Instance().getPlayerPosY(this->playerNumber)) > 0.4) {
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
		if (playerNumber != i && GameConfig::Instance().getPlayerPosX(i) != -1) {

			res = calcDistance(GameConfig::Instance().getPlayerPosX(playerNumber), GameConfig::Instance().getPlayerPosY(playerNumber), GameConfig::Instance().getPlayerPosX(i),
					GameConfig::Instance().getPlayerPosY(i));
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
