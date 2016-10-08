#include <stdio.h>
#include <cstdlib>
#include "../bomberman/Grid.h"

#ifndef __MYCLASS_CELL
#define __MYCLASS_CELL

class Cell {
	public:
		Cell();
		Cell(int x, int y, bool origin, int endX, int endY);
		void printHimself();
		void setHeuristicCost(int cost);
		int getHeuristicCost();
		void setFinalCost(int cost);
		int getFinalCost();
		void setParent(Cell * Parent);
		Cell * getParent();
		int getX();
		int getY();
		bool isOriginal();

	private:
		int finalCost; //G+H
		int heuristicCost;
		int x, y;
		bool origin;

		Cell * parent;
};
#endif
