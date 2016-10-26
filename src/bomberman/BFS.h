#include <queue>
#include <vector>
#include <stdio.h>
#include "Grid.h"

#ifndef __MYCLASS_BFS
#define __MYCLASS_BFS

class BFS {
	public:
		BFS(int * tab);
		~BFS();
		int solve(int startIndex, int typeSearch, bool ignoreWall);
		int findNextBrick(int startIndex);
		void reset();
		void printTested();
	private:
		void push(int index);
		std::queue<int> open;
		int * tab;
		bool tested[sizeX * sizeY];
};
#endif
