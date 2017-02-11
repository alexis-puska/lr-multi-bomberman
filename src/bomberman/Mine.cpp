#include "Mine.h"

Mine::Mine(int index) {
	this->index = index;
	this->type = straight;
	srand (time(NULL));

	//for animation
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 8;
	work = false;
}

Mine::~Mine() {
	Sound::Instance().stopMineSound();
}

bool Mine::doSomething(SDL_Surface * surface) {
	if (work) {
		count++;
		frameCounter++;
		if (frameCounter > nbFrameMine) {
			frameCounter = 0;
			offsetSprite++;
			if (offsetSprite >= nbFrameForAnimation) {
				offsetSprite = 0;
			}
		}
		frameCounter++;
		drawHimself(surface, offsetSprite);
		if (count > nbCycle) {
			work = false;
			count = 0;
			Sound::Instance().stopMineSound();
			drawHimself(surface, 12);
		}
	} else {
		for (int i = 0; i < nbPlayer; i++) {
			if (index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false) {
				Sound::Instance().startMineSound();
				activate[i] = true;
				if (!work) {
					work = true;
				}
			} else if (index != GameConfig::Instance().getPlayerIndex(i) && activate[i] == true) {
				activate[i] = false;
			}
		}
	}
	return false;
}

void Mine::drawHimself(SDL_Surface * surfaceToDraw, int offsetSpriteAnimation) {
	SDL_Rect dstRect;
	dstRect.x = (index % 35) * smallSpriteLevelSizeWidth;
	dstRect.y = ((int) floor(index / 35)) * smallSpriteLevelSizeHeight;
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	SDL_FillRect(surfaceToDraw, &dstRect, 0x000000);
	SDL_BlitSurface(Sprite::Instance().getMine(offsetSpriteAnimation), NULL, surfaceToDraw, &dstRect);
}
