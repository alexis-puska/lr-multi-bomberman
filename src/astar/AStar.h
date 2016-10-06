#include <queue>
#include <vector>
#include <stdio.h>

#include "Cell.h"

#ifndef __MYCLASS_ASTAR
#define __MYCLASS_ASTAR

struct CompareCell: public std::binary_function<Cell*, Cell*, bool> {
		bool operator()(Cell* c1, Cell* c2) const {
			return c1->getFinalCost() < c2->getFinalCost() ? -1 : c1->getFinalCost() > c2->getFinalCost() ? 1 : 0;
		}
};

class AStar {
	public:
		AStar(int * table, int sizzX, int sizzY);
		~AStar();
		void init(int startX, int startY, int endX, int endY);
		void solve();
		bool isSolved();
		Cell * getEnd();
	private:
		std::priority_queue<Cell*, std::vector<Cell*>, CompareCell> open;

		int * tab;
		int sizX;
		int sizY;
		Cell ** grid;
		bool * closed;
		bool * inOpen;
		int startI, startJ;
		int endI, endJ;

		void checkAndUpdateCost(Cell * current, Cell * t, int cost);

};
#endif
