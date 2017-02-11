#include "Mine.h"

Mine::Mine(int index) {
	this->index = index;
	work = false;
}

Mine::~Mine() {
}

bool Mine::doSomething() {
	if (work) {
		count++;
		if (count >= nbCycle) {
			count = 0;
			work = false;
			Sound::Instance().stopMineSound();
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
//	if (position == nb){
//		return true;
//	}
//	return false;
}

void Mine::drawHimself(SDL_Surface * surfaceToDraw) {
	SDL_Rect dstRect;
	dstRect.x = (index % 35) * smallSpriteLevelSizeWidth;
	dstRect.y = ((int) floor(index / 35)) * smallSpriteLevelSizeHeight;
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	SDL_BlitSurface(Sprite::Instance().getMine(0), NULL, surfaceToDraw, &dstRect);
}
