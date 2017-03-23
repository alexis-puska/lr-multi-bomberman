#include "PopBonus.h"

#define nbFrame 4

PopBonus::PopBonus(int posX, int posY) {
	this->posX = posX;
	this->posY = posY;
	this->idx = posX + posY * 35;
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 4;
	deleteAnimation = false;
}

PopBonus::PopBonus(int index) {
	this->posX = index % sizeX;
	this->posY = floor(index / sizeX);
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 4;
	deleteAnimation = false;
}

PopBonus::~PopBonus() {
}

bool PopBonus::canBeDelete() {
	return deleteAnimation;
}

void PopBonus::tick(SDL_Surface * surfaceToDraw) {
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
	SDL_BlitSurface(Sprite::Instance().getPopBonus(offsetSprite), NULL, surfaceToDraw, &dstRect);
	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().sendPopBonus(idx, offsetSprite);
	}
}
