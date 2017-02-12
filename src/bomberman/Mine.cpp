#include "Mine.h"

Mine::Mine(int index) {
	this->index = index;
	this->type = straight;
	this->first = mineUp;
	this->second = mineDown;
	srand (time(NULL));

	//for animation
frameCounter	= 0;
	offsetSprite = 0;
	nbFrameForAnimation = 8;
	work = false;
	channelSound = 0;
}

Mine::~Mine() {
	Sound::Instance().stopMineSound(channelSound);
}

int Mine::getFirstDirection() {
	return first;
}
int Mine::getSecondDirection() {
	return second;
}
int Mine::getIndex() {
	return index;
}

bool Mine::doSomething(SDL_Surface * surface) {
	if (work) {
		count++;
		if (count < nbCycle) {
			frameCounter++;
			if (frameCounter > nbFrameMine) {
				frameCounter = 0;
				offsetSprite++;
				if (offsetSprite >= (type == straight ? straightNbFrame : crookedNbframe)) {
					offsetSprite = 0;
				}
			}
			frameCounter++;
			drawHimself(surface, (type == straight ? offsetSprite + 8 : offsetSprite));
		}
		if (count == nbCycle) {
			Sound::Instance().stopMineSound(channelSound);
			if (type == straight) {
				switch (offsetSprite) {
					case 0:
						offsetSprite++;
					case 1:
						first = mineUp;
						second = mineDown;
						break;
					case 2:
						offsetSprite++;
					case 3:
						first = mineLeft;
						second = mineRight;
						break;
				}
			} else {
				switch (offsetSprite) {
					case 0:
						offsetSprite++;
					case 1:
						first = mineUp;
						second = mineRight;
						break;
					case 2:
						offsetSprite++;
					case 3:
						first = mineRight;
						second = mineDown;
						break;
					case 4:
						offsetSprite++;
					case 5:
						first = mineDown;
						second = mineLeft;
						break;
					case 6:
						offsetSprite++;
					case 7:
						first = mineLeft;
						second = mineUp;
						break;
				}
			}
			drawHimself(surface, (type == straight ? offsetSprite + 8 : offsetSprite));
		}
		if (count > nbCycleEnd) {
			work = false;
			count = 0;
			drawHimself(surface, 12);
			return true;
		}
	} else {
		for (int i = 0; i < nbPlayer; i++) {
			if (index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false) {
				channelSound = Sound::Instance().getNextMineOffsetChannel();
				Sound::Instance().startMineSound(channelSound);
				activate[i] = true;
				if (!work) {
					type = rand() % 2;
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
