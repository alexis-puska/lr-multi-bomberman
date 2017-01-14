#include <stdio.h>
#include <cstdlib>
#include "../Grid.h"

#ifndef __MYCLASS_AStarCell
#define __MYCLASS_AStarCell

class AStarCell {
	public:
		AStarCell();
		void configure(int x, int y, bool origin, int endX, int endY);
		~AStarCell();
		void printHimself();
		void setHeuristicCost(int cost);
		int getHeuristicCost();
		void setFinalCost(int cost);
		int getFinalCost();
		void setParent(AStarCell * parent);
		AStarCell * getParent();
		int getX();
		int getY();
		int getIndex();
		bool isOriginal();

	private:
		int finalCost; //G+H
		int heuristicCost;
		int x, y;
		bool origin;

		AStarCell * parent;
};
#endif
