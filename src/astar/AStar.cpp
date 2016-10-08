#include "AStar.h"

#define V_H_COST 10

AStar::AStar(int * tab) {
	this->tab = tab;
	grid = new Cell*[sizeX * sizeY];
	closed = new bool[sizeX * sizeY];
	inOpen = new bool[sizeX * sizeY];
}

AStar::~AStar() {
	tab = NULL;
	free (inOpen);
	free (closed);
	for (int i = 0; i < sizeX * sizeY; i++) {
		free (grid[i]);
	}
	free (grid);
}

void AStar::init(int startX, int startY, int endX, int endY) {
	startI = startX;
	startJ = startY;
	endI = endX;
	endJ = endY;
	
		
	if(open.size()!=0){
		while(open.size()!=0){
			open.pop();	
		}	
	}
	
		
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if(grid[i + j * sizeX]){
				free(grid[i + j * sizeX]);
			}
		
			if (startI == i && startJ == j) {
				grid[i + j * sizeX] = new Cell(i, j, true, endX, endY);
			} else {
				grid[i + j * sizeX] = new Cell(i, j, false, endX, endY);
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

	Cell * current;

	while (true) {
		current = open.top();
		fprintf(stderr,"value cell : %i\n",current->getFinalCost());
		open.pop();
		if (!current) {
			break;
		}
		closed[current->getX() + current->getY() * sizeX] = true;

		if (current->getX() == endI && current->getY() == endJ) {
			return;
		}

		Cell * t;
		if (current->getX() - 1 >= 0) {
			t = grid[current->getX() - 1 + current->getY() * sizeX];
			checkAndUpdateCost(current, t, current->getFinalCost() + V_H_COST);
		}

		if (current->getY() - 1 >= 0) {
			t = grid[current->getX() + (current->getY() - 1) * sizeX];
			checkAndUpdateCost(current, t, current->getFinalCost() + V_H_COST);
		}

		if (current->getY() + 1 < sizeX) {
			t = grid[current->getX() + (current->getY() + 1) * sizeX];
			checkAndUpdateCost(current, t, current->getFinalCost() + V_H_COST);
		}

		if (current->getX() + 1 < sizeY) {
			t = grid[(current->getX() + 1) + current->getY() * sizeX];
			checkAndUpdateCost(current, t, current->getFinalCost() + V_H_COST);
		}
	}

}

Cell * AStar::getEnd() {
	return grid[endI + endJ * sizeX];
}

bool AStar::isSolved() {
	return closed[endI + endJ * sizeX];
}

void AStar::checkAndUpdateCost(Cell * current, Cell * t, int cost) {
	if (!t || closed[t->getX() + t->getY() * sizeX]) {
		return;
	}
	int t_final_cost = t->getHeuristicCost() + cost;
	bool inOpenVal = inOpen[t->getX() + t->getY() * sizeX];
	if (!inOpenVal || t_final_cost < t->getFinalCost()) {
		t->setFinalCost(t_final_cost);
		t->setParent(current);
		if (!inOpenVal) {
			inOpen[t->getX() + t->getY() * sizeX] = true;
			open.push(t);
		}
	}
}

