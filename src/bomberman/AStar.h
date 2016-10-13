#include <queue>
#include <vector>
#include <stdio.h>

#include "AStarCell.h"

#ifndef __MYCLASS_ASTAR
#define __MYCLASS_ASTAR

struct CompareAStarCell: public std::binary_function<AStarCell*, AStarCell*, bool> {
		bool operator()(AStarCell & c1, AStarCell & c2) const {
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
		AStarCell getEnd();
	private:
		std::priority_queue<AStarCell, std::vector<AStarCell>, CompareAStarCell> open;

		int * tab;
		AStarCell grid[sizeX * sizeY];
		bool closed[sizeX * sizeY];
		bool inOpen[sizeX * sizeY];
		int startX, startY;
		int endX, endY;
		int searchlevel;
		bool ignoreBombe;

		bool checkAndUpdateCost(AStarCell * current, AStarCell * t, int cost);

};
#endif
