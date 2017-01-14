#include "SuddenDeathAnimation.h"

#define nbTickAnimation 1

SuddenDeathAnimation::SuddenDeathAnimation(int posX, int posY, int levelIndex, int tab[sizeX * sizeY], Grid * grid) {
	this->posX = posX;
	this->posY = posY;
	this->tab = tab;
	this->grid = grid;
	this->levelIndex = levelIndex;
	offset = 340;
	frameCounter = 0;
	canDelete = false;
}
SuddenDeathAnimation::~SuddenDeathAnimation() {
	tab = NULL;
}
void SuddenDeathAnimation::tick(SDL_Surface * surfaceToDraw) {
	if (offset == 0) {
		grid->placeSuddenDeathWall(posX, posY);
		canDelete = true;
		return;
	} else {
		if (frameCounter == nbTickAnimation) {
			offset = offset - 10;
			frameCounter = 0;
		}
	}
	frameCounter++;

	SDL_Rect dstRect;
	dstRect.x = posX * smallSpriteLevelSizeWidth;
	dstRect.y = posY * smallSpriteLevelSizeHeight - offset;
	dstRect.w = 18;
	dstRect.h = 16;
	SDL_BlitSurface(Sprite::Instance().getLevel(suddenDeathWallSpriteIndex, levelIndex), NULL, surfaceToDraw, &dstRect);
}

bool SuddenDeathAnimation::canBeDeleted() {
	return canDelete;
}
