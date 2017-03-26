#include "BurnLouis.h"

#define nbFrame 4

BurnLouis::BurnLouis(float posX, float posY) {
	this->posX = posX;
	this->posY = posY;
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 4;
	deleteAnimation = false;
	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().sendBurnLouis(posX, posY);
	}
}

BurnLouis::~BurnLouis() {
}

bool BurnLouis::canBeDelete() {
	return deleteAnimation;
}

void BurnLouis::tick(SDL_Surface * surfaceToDraw) {
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * smallSpriteLevelSizeWidth) - (spriteLouisSizeWidth / 2);
	destTextureRect.y = (posY * smallSpriteLevelSizeHeight) - (spriteLouisSizeHeight - 7);
	destTextureRect.w = spriteLouisSizeWidth;
	destTextureRect.h = spriteLouisSizeHeight;
	if (frameCounter > nbFrame) {
		frameCounter = 0;
		offsetSprite++;
		if (offsetSprite >= nbFrameForAnimation) {
			deleteAnimation = true;
			return;
		}
	}
	frameCounter++;
	int idx = Sprite::Instance().drawLouisBurning(0, offsetSprite, 0);
	SDL_BlitSurface(Sprite::Instance().getLouisSprite(idx), NULL, surfaceToDraw, &destTextureRect);
}
