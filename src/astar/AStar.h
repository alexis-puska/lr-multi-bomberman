#include <queue>
#include <vector>
#include <stdio.h>
#include "Cell.h"

#ifndef __MYCLASS_ASTAR
#define __MYCLASS_ASTAR

struct CompareCell : public std::binary_function<Cell*, Cell*, bool>                                                                                     
{
	bool operator()(const Cell* c1, const Cell* c2) const {
		return c1 -> getFinalCost() < c2 -> getFinalCost() ? -1 : c1 -> getFinalCost() > c2 -> getFinalCost() ? 1 : 0;
	}
};

class AStar{
	public:
		AStar();
	private:
		std::priority_queue <Cell*,std::vector<Cell*>, CompareCell >  open;
};
#endif