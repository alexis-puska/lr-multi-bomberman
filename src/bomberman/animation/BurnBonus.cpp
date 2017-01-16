#include "BurnBonus.h"

#define nbFrame 4

BurnBonus::BurnBonus(int posX, int posY) {
	this->posX = posX;
	this->posY = posY;
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 6;
	deleteAnimation = false;
}

BurnBonus::~BurnBonus() {
}

bool BurnBonus::canBeDelete() {
	return deleteAnimation;
}

void BurnBonus::tick(SDL_Surface * surfaceToDraw) {
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
	SDL_BlitSurface(Sprite::Instance().getBurnBonus(offsetSprite), NULL, surfaceToDraw, &dstRect);
}
