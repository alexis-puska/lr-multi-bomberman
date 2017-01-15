#include "BurnWall.h"

#define nbFrame 4

BurnWall::BurnWall(int posX, int posY, int index, int levelIndex, int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY]) {
	this->indexBurnWall = index;
	this->posX = posX;
	this->posY = posY;
	this->tab = tab;
	this->tabBonus = tabBonus;
	this->levelIndex = levelIndex;
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 6;
	deleteAnimation = false;
}

BurnWall::~BurnWall() {
	tab = NULL;
	tabBonus = NULL;
}

bool BurnWall::canBeDelete() {

	if (deleteAnimation) {
		//remove grid status BurnWall
		if (tab[posX + posY * sizeX] == brickElement) {
			tab[posX + posY * sizeX] = emptyElement;
		}
	}
	return deleteAnimation;
}

void BurnWall::tick(SDL_Surface * surfaceToDraw) {
	SDL_Rect dstRect;
	dstRect.x = posX * 18;
	dstRect.y = posY * 16;
	dstRect.w = 18;
	dstRect.h = 16;
	if (frameCounter > nbFrame) {
		frameCounter = 0;
		offsetSprite++;
		if (offsetSprite >= nbFrameForAnimation) {
			deleteAnimation = true;
			return;
		}
	}
	frameCounter++;
	SDL_BlitSurface(Sprite::Instance().getBurnWall(offsetSprite, levelIndex), NULL, surfaceToDraw, &dstRect);
}