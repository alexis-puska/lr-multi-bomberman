#include "BFS.h"

#define V_H_COST 10

BFS::BFS(int * tab) {
	this->tab = tab;
	for(int i = 0; i < sizeX * sizeY; i++){
		tested[i] = false;	
	}
}

BFS::~BFS() {
	tab = NULL;
}

void BFS::reset(){
	for(int i = 0; i < sizeX * sizeY; i++){
		tested[i] = false;	
	}
	while(!open.empty())
  	{
	     open.pop();
  	}
}

void BFS::printTested(){
	for(int i = 0; i < sizeX * sizeY; i++){
		if(i%sizeX == 0){
			fprintf(stderr, "\n");	
		}
		fprintf(stderr, "%i, ", tested[i] == true?1:0);
	}
	fprintf(stderr, "\n\n\n\n\n");
}

int BFS::solve(int startIndex, int typeSearch){
	int indexTest = 0;
	open.push(startIndex);
	while(open.size() > 0){
		indexTest = open.front();
		open.pop();
		if(tab[indexTest] == typeSearch){
			return indexTest;
		}
		
		if(((indexTest%sizeX) - 1) >= 0){
			if(!tested[indexTest - 1]){
				tested[indexTest - 1] = true;
				open.push(indexTest - 1);
			}
		}
		if(((indexTest%sizeX) + 1) < sizeX){
			if(!tested[indexTest + 1]){
				tested[indexTest + 1] = true;
				open.push(indexTest + 1);
			}
		}
		if((indexTest - sizeX) >= 0){
			if(!tested[indexTest - sizeX]){
				tested[indexTest - sizeX] = true;
				open.push(indexTest - sizeX);
			}
		}
		if((indexTest + sizeX) < sizeY * sizeX){
			if(!tested[indexTest + sizeX]){
				tested[indexTest + sizeX] = true;
				open.push(indexTest + sizeX);
			}
		}
	}
	return -1;
}