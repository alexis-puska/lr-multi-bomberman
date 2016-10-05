#include "Cell.h"

Cell::Cell(){}

Cell::Cell(int ii, int jj){
	i = ii;
	j = jj;
}

void Cell::setHeuristicCost(int cost){
	heuristicCost = cost; 
}

int Cell::getHeuristicCost(){
	return heuristicCost;
}

void Cell::setFinalCost(int cost){
	finalCost = cost;
}

int Cell::getFinalCost(){
	return finalCost;
}

void Cell::setParent(Cell * Parentt){
	parent = Parentt;
}

Cell * Cell::getParent(){
	return parent;
}

void Cell::printHimself(){
	fprintf(stderr, "[ %i, %i]", i, j);
}