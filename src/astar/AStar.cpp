#include "AStar.h"

#define V_H_COST 10

AStar::AStar(int * table, int sizeeX, int sizeeY) {
	tab = table;
	sizeX = sizeeX;
	sizeY = sizeeY;
	inOpen = new bool[sizeX * sizeY];
}

void AStar::init(int startX, int startY, int endX, int endY) {
	startI = startX;
	startJ = startY;
	endI = endX;
	endJ = endY;
}

bool AStar::solve() {
	//add the start location to open list.

	inOpen[startI + startJ * sizeX] = true;
	open.push(grid[startI + startJ * sizeX]);

	Cell * current;

	while (true) {
		current = open.top();
		open.pop();
		if (current == NULL) {
			break;
		}
		closed[current->i + current->j * sizeX] = true;

		if (current == grid[endI + endJ * sizeX]) {
			return closed[endI + endJ * sizeX];
		}

		Cell * t;
		if (current->i - 1 >= 0) {
			t = grid[current->i - 1 + current->j * sizeX];
			checkAndUpdateCost(current, t, current->finalCost + V_H_COST);
		}

		if (current->j - 1 >= 0) {
			t = grid[current->i + (current->j - 1) * sizeX];
			checkAndUpdateCost(current, t, current->finalCost + V_H_COST);
		}

		if (current->j + 1 < sizeX) {
			t = grid[current->i + (current->j + 1) * sizeX];
			checkAndUpdateCost(current, t, current->finalCost + V_H_COST);
		}

		if (current->i + 1 < sizeY) {
			t = grid[(current->i + 1) + current->j * sizeX];
			checkAndUpdateCost(current, t, current->finalCost + V_H_COST);
		}
	}
	return closed[endI + endJ * sizeX];
}

void AStar::checkAndUpdateCost(Cell * current, Cell * t, int cost) {
	if (t == NULL || closed[t->i + t->j * sizeX]) {
		return;
	}
	int t_final_cost = t->heuristicCost + cost;
	bool inOpenVal = inOpen[t->i + t->j * sizeX];
	if (!inOpenVal || t_final_cost < t->finalCost) {
		t->finalCost = t_final_cost;
		t->parent = current;
		if (!inOpenVal) {
			inOpen[t->i + t->j * sizeX] = true;
			open.push(t);
		}
	}
}

