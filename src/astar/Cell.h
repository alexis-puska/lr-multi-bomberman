#include <stdio.h>

#ifndef __MYCLASS_CELL
#define __MYCLASS_CELL

class Cell{
	public:
		Cell(int i, int j);
		void printHimself();
		void setHeuristicCost(int cost);
		int getHeuristicCost();
		void setFinalCost(int cost);
		int getFinalCost();
		void setParent(Cell * Parent);
		Cell * getParent();
		
		
	private:
		int heuristicCost = 0;
        int finalCost = 0; //G+H
        int i, j;
        Cell * parent; 
};
#endif