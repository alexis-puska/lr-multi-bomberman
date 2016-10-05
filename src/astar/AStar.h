#include <queue>
#include <vector>
#include <stdio.h>
#include "Cell.h"

#ifndef __MYCLASS_ASTAR
#define __MYCLASS_ASTAR

struct CompareCell: public std::binary_function<Cell*, Cell*, bool> {
		bool operator()(const Cell* c1, const Cell* c2) const {
			return c1->finalCost < c2->finalCost ? -1 : c1->finalCost > c2->finalCost ? 1 : 0;
		}
};

class AStar {
	public:
		AStar(int * table, int sizeX, int sizeY);
		void init(int startX, int startY, int endX, int endY);
		bool solve();
	private:
		std::priority_queue<Cell*, std::vector<Cell*>, CompareCell> open;

		int * tab;
		int sizeX;
		int sizeY;
		Cell ** grid;
		bool * closed;
		bool * inOpen;
		int startI, startJ;
		int endI, endJ;

		void checkAndUpdateCost(Cell * current, Cell * t, int cost);

};
#endif
