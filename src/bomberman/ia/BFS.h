#include <queue>
#include <vector>
#include <stdio.h>
#include "../Grid.h"

#ifndef __MYCLASS_BFS
#define __MYCLASS_BFS

class BFS {
	public:
		BFS(int * tab);
		~BFS();
		int solve(int startIndex, int typeSearch, bool ignoreWall);
		int findNextBrick(int startIndex);
		void addIgnoreCase(int index);
		
		int findSecure(int startIndex);
		bool isSecure(int indexTest);

		bool checkDropBombe(int startIndex);
		bool isChecked(int idx, int startIndex);

		void resetSecure();
		void reset(bool withIgnoredCase);
		void resetCheckDropBombe();
		
		void printTested();
		void printTestedSecure();
		
		
	private:
		int * tab;
		std::queue<int> open;
		std::vector<int> ignoredCase;
		bool tested[sizeX * sizeY];
		std::queue<int> openSecure;
		bool testedSecure[sizeX * sizeY];
		
		bool checkCase[sizeX * sizeY];
		std::queue<int> openCheck;
			
		void push(int index);
		void pushSecure(int index);
		void pushChecked(int index);
};
#endif
