#include "Cell.h"

Cell::Cell() {
}

void Cell::configure(int x, int y, bool origin, int endX, int endY) {
	this->x = x;
	this->y = y;
	finalCost = 0;
	heuristicCost = abs(x - endX) + abs(y - endY);
	this->origin = origin;
	this->parent = NULL;
}

Cell::~Cell() {
	parent = NULL;
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
	fprintf(stderr, "[%2i,%2i]", x, y);
}

int Cell::getX() {
	return x;
}

int Cell::getY() {
	return y;
}

int Cell::getIndex() {
	return this->x + this->y * sizeX;
}
