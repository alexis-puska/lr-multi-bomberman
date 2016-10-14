#include <queue>
#include <vector>
#include <stdio.h>
#include "grid.h"


#ifndef __MYCLASS_BFS
#define __MYCLASS_BFS


class BFS {
	public:
		BFS(int * tab);
		~BFS();
		int solve(int startIndex, int typeSearch);
		void reset();
		void printTested();
	private:
		std::queue<int> open;
		int * tab;
		bool tested[sizeX * sizeY];
};
#endif
