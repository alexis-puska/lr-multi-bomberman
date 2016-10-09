#include <queue>
#include <vector>
#include <stdio.h>

#include "Cell.h"

#ifndef __MYCLASS_ASTAR
#define __MYCLASS_ASTAR

struct CompareCell: public std::binary_function<Cell*, Cell*, bool> {
		bool operator()(Cell & c1, Cell & c2) const {
			return c1.getFinalCost() > c2.getFinalCost();
		}
};

class AStar {
	public:
		AStar(int * tab);
		~AStar();
		void init(int startX, int startY, int endX, int endY, int searchlevel);
		void solve();
		bool isSolved();
		Cell getEnd();
	private:
		std::priority_queue<Cell, std::vector<Cell>, CompareCell> open;

		int * tab;
		Cell grid[sizeX * sizeY];
		bool closed[sizeX * sizeY];
		bool inOpen[sizeX * sizeY];
		int startX, startY;
		int endX, endY;
		int searchlevel;

		bool checkAndUpdateCost(Cell * current, Cell * t, int cost);

};
#endif
