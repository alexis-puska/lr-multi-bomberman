#include "AStar.h"

#define V_H_COST 10

AStar::AStar(int * table, int sizzX, int sizzY) {
	tab = table;
	sizX = sizzX;
	sizY = sizzY;
	grid = new Cell*[sizX * sizY];
	closed = new bool[sizX * sizY];
	inOpen = new bool[sizX * sizY];
}

AStar::~AStar() {
	tab = NULL;
	free (inOpen);
	free (closed);
	for (int i = 0; i < sizX * sizY; i++) {
		free (grid[i]);
	}
	free (grid);
}

void AStar::init(int startX, int startY, int endX, int endY) {
	startI = startX;
	startJ = startY;
	endI = endX;
	endJ = endY;
	for (int i = 0; i < sizX; i++) {
		for (int j = 0; j < sizY; j++) {
			if (startI == i && startJ == j) {
				grid[i + j * sizX] = new Cell(i, j, true, endX, endY);
			} else {
				grid[i + j * sizX] = new Cell(i, j, false, endX, endY);
			}
			closed[i + j * sizX] = false;
			inOpen[i + j * sizX] = false;
		}
	}
}

void AStar::solve() {

	//add the start location to open list.

	inOpen[startI + startJ * sizX] = true;
	open.push(grid[startI + startJ * sizX]);

	Cell * current;

	while (true) {
		current = open.top();
		open.pop();
		if (!current) {
			break;
		}
		closed[current->getX() + current->getY() * sizX] = true;

		if (current->getX() == endI && current->getY() == endJ) {
			return;
		}

		Cell * t;
		if (current->getX() - 1 >= 0) {
			t = grid[current->getX() - 1 + current->getY() * sizX];
			checkAndUpdateCost(current, t, current->getFinalCost() + V_H_COST);
		}

		if (current->getY() - 1 >= 0) {
			t = grid[current->getX() + (current->getY() - 1) * sizX];
			checkAndUpdateCost(current, t, current->getFinalCost() + V_H_COST);
		}

		if (current->getY() + 1 < sizX) {
			t = grid[current->getX() + (current->getY() + 1) * sizX];
			checkAndUpdateCost(current, t, current->getFinalCost() + V_H_COST);
		}

		if (current->getX() + 1 < sizY) {
			t = grid[(current->getX() + 1) + current->getY() * sizX];
			checkAndUpdateCost(current, t, current->getFinalCost() + V_H_COST);
		}
	}
}

Cell * AStar::getEnd() {
	return grid[endI + endJ * sizX];
}

bool AStar::isSolved() {
	return closed[endI + endJ * sizX];
}

void AStar::checkAndUpdateCost(Cell * current, Cell * t, int cost) {
	if (!t || closed[t->getX() + t->getY() * sizX]) {
		return;
	}
	int t_final_cost = t->getHeuristicCost() + cost;
	bool inOpenVal = inOpen[t->getX() + t->getY() * sizX];
	if (!inOpenVal || t_final_cost < t->getFinalCost()) {
		t->setFinalCost(t_final_cost);
		t->setParent(current);
		if (!inOpenVal) {
			inOpen[t->getX() + t->getY() * sizX] = true;
			open.push(t);
		}
	}
}

