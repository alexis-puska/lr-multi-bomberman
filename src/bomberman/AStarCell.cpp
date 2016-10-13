#include "AStarCell.h"

AStarCell::AStarCell() {
}

void AStarCell::configure(int x, int y, bool origin, int endX, int endY) {
	this->x = x;
	this->y = y;
	finalCost = 0;
	heuristicCost = abs(x - endX) + abs(y - endY);
	this->origin = origin;
	this->parent = NULL;
}

AStarCell::~AStarCell() {
	parent = NULL;
}

void AStarCell::setHeuristicCost(int cost) {
	heuristicCost = cost;
}

int AStarCell::getHeuristicCost() {
	return heuristicCost;
}

void AStarCell::setFinalCost(int cost) {
	finalCost = cost;
}

int AStarCell::getFinalCost() {
	return finalCost;
}

void AStarCell::setParent(AStarCell * parent) {
	this->parent = parent;
}

AStarCell * AStarCell::getParent() {
	return parent;
}

bool AStarCell::isOriginal() {
	return origin;
}

void AStarCell::printHimself() {
	fprintf(stderr, "[%2i,%2i]", x, y);
}

int AStarCell::getX() {
	return x;
}

int AStarCell::getY() {
	return y;
}

int AStarCell::getIndex() {
	return this->x + this->y * sizeX;
}
