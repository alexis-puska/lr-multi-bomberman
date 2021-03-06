#include "BurnWall.h"

#define nbFrame 4

BurnWall::BurnWall(int posX, int posY, int index, int tab[sizeX * sizeY]) {
	this->posX = posX;
	this->posY = posY;
	this->idx = posX + posY * 35;
	this->tab = tab;
	this->levelIndex = GameConfig::Instance().getLevel();
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 6;
	deleteAnimation = false;
	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().sendNewEmptyElement(idx);
		BomberNetServer::Instance().sendburnWall(idx);
	}
}


BurnWall::BurnWall(int posX, int posY, int index) {
	this->posX = posX;
	this->posY = posY;
	this->idx = posX + posY * 35;
	this->levelIndex = index;
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 6;
	deleteAnimation = false;
}


BurnWall::~BurnWall() {
	tab = NULL;
}

bool BurnWall::canBeDelete() {
	if (GameConfig::Instance().getGameModeType() != NET_CLIENT) {
		if (deleteAnimation) {
			//remove grid status BurnWall
			if (tab[posX + posY * sizeX] == brickElement) {
				tab[posX + posY * sizeX] = emptyElement;
			}
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
