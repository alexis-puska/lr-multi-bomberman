#include "BFS.h"

#define V_H_COST 10

BFS::BFS(int * tab) {
	this->tab = tab;
	resetCheckDropBombe();
	for (int i = 0; i < sizeX * sizeY; i++) {
		tested[i] = false;
		if (tab[i] == wallElement || tab[i] == bombeElement || tab[i] == brickElement) {
			testedSecure[i] = true;
		} else {
			testedSecure[i] = false;
		}
	}
}

BFS::~BFS() {
	tab = NULL;
}

void BFS::reset(bool withIgnoredCase) {
	for (int i = 0; i < sizeX * sizeY; i++) {
		tested[i] = false;
	}
	if(withIgnoredCase){
		ignoredCase.clear();	
	}else{
		for (unsigned int i = 0; i < ignoredCase.size(); i++){
			tested[ignoredCase[i]] = true;
		}
	}
	while (!open.empty()) {
		open.pop();
	}
}

void BFS::printTested() {
	for (int i = 0; i < sizeX * sizeY; i++) {
		if (i % sizeX == 0) {
			fprintf(stderr, "\n");
		}
		fprintf(stderr, "%i, ", tested[i] == true ? 1 : 0);
	}
	fprintf(stderr, "\n\n\n\n\n");
}

void BFS::addIgnoreCase(int index){
	ignoredCase.push_back(index);
}

/*
 * function for brain to search an object 
 */

int BFS::solve(int startIndex, int typeSearch, bool ignoreWall) {
	int indexTest = 0;
	open.push(startIndex);
	while (open.size() > 0) {
		indexTest = open.front();
		open.pop();
		if (tab[indexTest] == typeSearch) {
			tested[indexTest] = true;
			return indexTest;
		}

		if (((indexTest % sizeX) - 1) >= 0) {
			if (!tested[indexTest - 1]) {
				if (ignoreWall) {
					if (tab[indexTest - 1] != wallElement) {
						push(indexTest - 1);
					} else {
						tested[indexTest - 1] = true;
					}
				} else {
					push(indexTest - 1);
				}
			}
		}
		if (((indexTest % sizeX) + 1) < sizeX) {
			if (!tested[indexTest + 1]) {
				if (ignoreWall) {
					if (tab[indexTest + 1] != wallElement) {
						push(indexTest + 1);
					} else {
						tested[indexTest + 1] = true;
					}
				} else {
					push(indexTest + 1);

				}
			}
		}
		if ((indexTest - sizeX) >= 0) {
			if (!tested[indexTest - sizeX]) {
				if (ignoreWall) {
					if (tab[indexTest - sizeX] != wallElement) {
						push(indexTest - sizeX);
					} else {
						tested[indexTest - sizeX] = true;
					}
				} else {
					push(indexTest - sizeX);
				}
			}
		}
		if ((indexTest + sizeX) < sizeY * sizeX) {
			if (!tested[indexTest + sizeX]) {
				if (ignoreWall) {
					if (tab[indexTest + sizeX] != wallElement) {
						push(indexTest + sizeX);
					} else {
						tested[indexTest + sizeX] = true;
					}
				} else {
					push(indexTest + sizeX);
				}
			}
		}
	}
	return -1;
}

void BFS::push(int index) {
	tested[index] = true;
	open.push(index);
}

int BFS::findNextBrick(int startIndex) {
	return solve(startIndex, brickElement, true);
}

/*
 *	function for brain to search a secure case
 */

void BFS::printTestedSecure() {
	for (int i = 0; i < sizeX * sizeY; i++) {
		if (i % sizeX == 0) {
			fprintf(stderr, "\n");
		}
		fprintf(stderr, "%i, ", testedSecure[i] == true ? 1 : 0);
	}
	fprintf(stderr, "\n\n\n\n\n");
}

void BFS::resetSecure() {
	for (int i = 0; i < sizeX * sizeY; i++) {
		if (tab[i] == wallElement || tab[i] == bombeElement || tab[i] == brickElement) {
			testedSecure[i] = true;
		} else {
			testedSecure[i] = false;
		}
	}
	while (!openSecure.empty()) {
		openSecure.pop();
	}
}

int BFS::findSecure(int startIndex) {
	int indexTest = 0;
	openSecure.push(startIndex);
	while (openSecure.size() > 0) {
		indexTest = openSecure.front();
		openSecure.pop();

		if (isSecure(indexTest)) {
			return indexTest;
		} else {
			testedSecure[indexTest] = true;
		}

		if (((indexTest % sizeX) - 1) >= 0) {
			if (!testedSecure[indexTest - 1]) {
				pushSecure(indexTest - 1);
			}
		}
		if (((indexTest % sizeX) + 1) < sizeX) {
			if (!testedSecure[indexTest + 1]) {
				pushSecure(indexTest + 1);
			}
		}
		if ((indexTest - sizeX) >= 0) {
			if (!testedSecure[indexTest - sizeX]) {
				pushSecure(indexTest - sizeX);
			}
		}
		if ((indexTest + sizeX) < sizeY * sizeX) {
			if (!testedSecure[indexTest + sizeX]) {
				pushSecure(indexTest + sizeX);
			}
		}
	}

	int startX = startIndex % sizeX;
	int startY = floor(startIndex / sizeX);
	int endX = indexTest % sizeX;
	int endY = floor(indexTest / sizeX);

	//même ligne
	if (endX == startX) {
		if (startY >= endY) {
			//up
			return -1;
		} else if (startY < endY) {
			//down
			return -3;
		}
	} else if (startY == endY) {
		if (startX <= endX) {
			//right
			return -2;
		} else if (startX > endX) {
			//left
			return -4;
		}
	}
	return -1;
}

void BFS::pushSecure(int index) {
	testedSecure[index] = true;
	openSecure.push(index);
}

bool BFS::isSecure(int idx) {
	int calcX = idx % sizeX;
	int calcY = floor(idx / sizeX);
	int test = 0;
	int x = calcX;
	int y = calcY;
	while (x >= 0) {
		test = x + y * sizeX;
		if (tab[test] == brickElement || tab[test] == wallElement) {
			break;
		} else if (tab[test] == bombeElement) {
			return false;
		}
		x--;
	}
	x = calcX;
	y = calcY;
	while (x < sizeX) {
		test = x + y * sizeX;
		if (tab[test] == brickElement || tab[test] == wallElement) {
			break;
		} else if (tab[test] == bombeElement) {
			return false;
		}
		x++;
	}
	x = calcX;
	y = calcY;
	while (y >= 0) {
		test = x + y * sizeX;
		if (tab[test] == brickElement || tab[test] == wallElement) {
			break;
		} else if (tab[test] == bombeElement) {
			return false;
		}
		y--;
	}
	x = calcX;
	y = calcY;
	while (y < sizeY) {
		test = x + y * sizeX;
		if (tab[test] == brickElement || tab[test] == wallElement) {
			break;
		} else if (tab[test] == bombeElement) {
			return false;
		}
		y++;
	}
	return true;
}












void BFS::resetCheckDropBombe() {
	for (int i = 0; i < sizeX * sizeY; i++) {
		if (tab[i] == wallElement || tab[i] == bombeElement || tab[i] == brickElement) {
			checkCase[i] = true;
		} else {
			checkCase[i] = false;
		}
	}
	while (!openCheck.empty()) {
		openCheck.pop();
	}
}

bool BFS::checkDropBombe(int startIndex) {
	int indexTest = 0;
	int index = 0;
	resetCheckDropBombe();
	openCheck.push(startIndex);
	while (openCheck.size() > 0) {
		indexTest = openCheck.front();
		openCheck.pop();

		if (isChecked(indexTest, startIndex)) {
			return true;
		} else {
			checkCase[indexTest] = true;
		}


		
		if (((indexTest % sizeX) - 1) >= 0) {
			index = indexTest - 1;
			if (tab[index] == wallElement || tab[index] == bombeElement || tab[index] == brickElement) {
				checkCase[index] = true;
			}else{
				if (!checkCase[index]) {
					pushChecked(index);
				}
			}
		}
		if (((indexTest % sizeX) + 1) < sizeX) {
			index = indexTest + 1;
			if (tab[index] == wallElement || tab[index] == bombeElement || tab[index] == brickElement) {
				checkCase[index] = true;
			}else{
				if (!checkCase[index]) {
					pushChecked(index);
				}
			}
		}
		if ((indexTest - sizeX) >= 0) {
			index = indexTest - sizeX;
			if (tab[index] == wallElement || tab[index] == bombeElement || tab[index] == brickElement) {
				checkCase[index] = true;
			}else{
				if (!checkCase[index]) {
					pushChecked(index);
				}
			}
		}
		if ((indexTest + sizeX) < sizeY * sizeX) {
			index = indexTest + sizeX;
			if (tab[index] == wallElement || tab[index] == bombeElement || tab[index] == brickElement) {
				checkCase[index] = true;
			}else{
				if (!checkCase[index]) {
					pushChecked(index);
				}
			}
		}
	}

	return false;
}

void BFS::pushChecked(int index) {
	checkCase[index] = true;
	openCheck.push(index);
}

bool BFS::isChecked(int idx, int startIndex) {
	if(idx == startIndex){
		return false;
	}
	int calcX = idx % sizeX;
	int calcY = floor(idx / sizeX);
	int startCalcX = startIndex % sizeX;
	int startCalcY = floor(startIndex / sizeX);
	if(calcX != startCalcX && calcY != startCalcY){
		return true;
	}else{
		return false;	
	}
}
