#include "Cell.h"

Cell::Cell() {
}

Cell::Cell(int x, int y, bool origin, int endX, int endY) {
	this->x = x;
	this->y = y;
	finalCost = 0;
	heuristicCost = abs(x - endX) + abs(y - endY);
	if (heuristicCost == 0) {
		fprintf(stderr, "original is here at : %i %i", x, y);
	}
	this->origin = origin;
}

Cell::~Cell(){
	if(parent){
		free(parent);
	}
}

void Cell::setHeuristicCost(int cost) {
	heuristicCost = cost;
}

int Cell::getHeuristicCost() {
	return heuristicCost;
}

void Cell::setFinalCost(int cost) {
	finalCost = cost;
}

int Cell::getFinalCost() {
	return finalCost;
}

void Cell::setParent(Cell * parent) {
	this->parent = parent;
}

Cell * Cell::getParent() {
	return parent;
}

bool Cell::isOriginal() {
	return origin;
}

void Cell::printHimself() {
	fprintf(stderr, "[ %i, %i]", x, y);
}

int Cell::getX() {
	return x;
}

int Cell::getY() {
	return y;
}
