#include "AStar.h"

#define V_H_COST 10

AStar::AStar(int * tab) {
	this->tab = tab;
}

AStar::~AStar() {
	tab = NULL;
}

void AStar::init(int startX, int startY, int endX, int endY) {
	startI = startX;
	startJ = startY;
	endI = endX;
	endJ = endY;
		while(!open.empty()){
		open.pop();
	}

	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if (startI == i && startJ == j) {
				grid[i + j * sizeX].configure(i, j, true, endX, endY);
			} else {
				grid[i + j * sizeX].configure(i, j, false, endX, endY);
			}
			closed[i + j * sizeX] = false;
			inOpen[i + j * sizeX] = false;
		}
	}
}

void AStar::solve() {
	//add the start location to open list.

	inOpen[startI + startJ * sizeX] = true;
	open.push(grid[startI + startJ * sizeX]);

	Cell current;

	
	while (true) {
		
		if(open.empty()){
			break;	
		}
		current = open.top();
		open.pop();
		
		closed[current.getX() + current.getY() * sizeX] = true;

		if (current.getX() == endI && current.getY() == endJ) {
			return;
		}

		if (current.getX() - 1 >= 0) {
			if(checkAndUpdateCost(&current, &grid[current.getX() - 1 + current.getY() * sizeX], current.getFinalCost() + V_H_COST))
				open.push(grid[current.getX() - 1 + current.getY() * sizeX]);

		}

		if (current.getY() - 1 >= 0) {
			if(checkAndUpdateCost(&current, &grid[current.getX() + (current.getY() - 1) * sizeX], current.getFinalCost() + V_H_COST))
				open.push(grid[current.getX() + (current.getY() - 1) * sizeX]);

		}

		if (current.getY() + 1 < sizeY) {
			if(checkAndUpdateCost(&current, &grid[current.getX() + (current.getY() + 1) * sizeX], current.getFinalCost() + V_H_COST))
				open.push(grid[current.getX() + (current.getY() + 1) * sizeX]);

		}

		if (current.getX() + 1 < sizeX) {
			if(checkAndUpdateCost(&current, &grid[(current.getX() + 1) + current.getY() * sizeX], current.getFinalCost() + V_H_COST))
				open.push(grid[(current.getX() + 1) + current.getY() * sizeX]);
		}
		
	}

}

Cell AStar::getEnd() {
	return grid[endI + endJ * sizeX];
}

bool AStar::isSolved() {
	return closed[endI + endJ * sizeX];
}

bool AStar::checkAndUpdateCost(Cell * current, Cell * t, int cost) {
	if (closed[t->getX() + t->getY() * sizeX]) {
		return false;
	}
	int t_final_cost = t->getHeuristicCost() + cost;
	bool inOpenVal = inOpen[t->getX() + t->getY() * sizeX];
	if (!inOpenVal || t_final_cost < t->getFinalCost()) {
		t->setFinalCost(t_final_cost);
		t->setParent(&grid[current->getIndex()]);
		if (!inOpenVal) {
			inOpen[t->getX() + t->getY() * sizeX] = true;
			return true;
		}
	}
	return false;
}

