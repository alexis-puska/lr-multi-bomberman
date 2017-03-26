#include "Explosion.h"

#define nbFrame 4

Explosion::Explosion(int posX, int posY, int index, int tab[sizeX * sizeY]) {
	this->indexExplosion = index;
	this->posX = posX;
	this->posY = posY;
	this->idx = posX + posY * 35;
	this->tab = tab;
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 7;
	deleteAnimation = false;
	//set explosion in the grid
	tab[posX + posY * sizeX] = explosionElement;
	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().sendExplosion(idx, indexExplosion);
	}
}

Explosion::Explosion(int posX, int posY, int index) {
	this->indexExplosion = index;
	this->posX = posX;
	this->posY = posY;
	this->idx = posX + posY * 35;
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 7;
	deleteAnimation = false;
}

Explosion::~Explosion() {
	tab = NULL;
}

bool Explosion::canBeDelete() {
	if (GameConfig::Instance().getGameModeType() != NET_CLIENT) {
		if (deleteAnimation) {
			//remove grid status explosion
			tab[posX + posY * sizeX] = emptyElement;
		}
	}
	return deleteAnimation;
}

void Explosion::tick(SDL_Surface * surfaceToDraw) {
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
	int offsetSpriteAnimation = 0;
	switch (offsetSprite) {
		case 0:
			offsetSpriteAnimation = 3;
			break;
		case 1:
			offsetSpriteAnimation = 2;
			break;
		case 2:
			offsetSpriteAnimation = 1;
			break;
		case 3:
			offsetSpriteAnimation = 0;
			break;
		case 4:
			offsetSpriteAnimation = 1;
			break;
		case 5:
			offsetSpriteAnimation = 2;
			break;
		case 6:
			offsetSpriteAnimation = 3;
			break;
	}
	SDL_BlitSurface(Sprite::Instance().getFire(offsetSpriteAnimation, indexExplosion), NULL, surfaceToDraw, &dstRect);

}
