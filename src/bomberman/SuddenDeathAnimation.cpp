#include "SuddenDeathAnimation.h"

#define spriteSizeX 18
#define spriteSizeY 16
#define nbTickAnimation 1

SuddenDeathAnimation::SuddenDeathAnimation(int posXX, int posYY, SDL_Surface * wallSprite, int table[sizeX * sizeY], Grid * gridGame) {
	posX = posXX;
	posY = posYY;
	canDelete = false;
	sprite = wallSprite;
	tab = table;
	grid = gridGame;
	offset = 340;
	frameCounter = 0;
}
SuddenDeathAnimation::~SuddenDeathAnimation() {
	sprite = NULL;
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
	dstRect.x = posX * spriteSizeX;
	dstRect.y = posY * spriteSizeY - offset;
	dstRect.w = 18;
	dstRect.h = 16;
	SDL_BlitSurface(sprite, NULL, surfaceToDraw, &dstRect);
}

bool SuddenDeathAnimation::canBeDeleted() {
	return canDelete;
}
