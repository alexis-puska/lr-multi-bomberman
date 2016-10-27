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
		int findSecure(int startIndex);
		bool isSecure(int indexTest);

		void resetSecure();
		void reset();
		void printTested();
		void printTestedSecure();
	private:
		int * tab;
		std::queue<int> open;
		bool tested[sizeX * sizeY];
		std::queue<int> openSecure;
		bool testedSecure[sizeX * sizeY];
		void push(int index);
		void pushSecure(int index);
};
#endif
