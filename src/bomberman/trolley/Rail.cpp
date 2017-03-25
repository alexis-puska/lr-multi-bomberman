#include "Rail.h"

Rail::Rail(int index, int prev, int next, int nextAlt) {
	this->index = index;
	this->prevIndex = prev;
	this->nextIndex = next;
	this->nextIndexAlt = nextAlt;
}

Rail::Rail(int index, int prev, int next) {
	this->index = index;
	this->prevIndex = prev;
	this->nextIndex = next;
	this->nextIndexAlt = -1;
}

Rail::Rail(int index) {
	this->index = index;
	this->prevIndex = -1;
	this->nextIndex = -1;
	this->nextIndexAlt = -1;
}

Rail::Rail(int index, int prev) {
	this->index = index;
	this->prevIndex = prev;
	this->nextIndex = -1;
	this->nextIndexAlt = -1;
}

void Rail::init(std::map<int, Rail*> rails) {
	initImpl(this->getIndex() - 35, rails);
	initImpl(this->getIndex() - 1, rails);
	initImpl(this->getIndex() + 1, rails);
	initImpl(this->getIndex() + 35, rails);

}

void Rail::initImpl(int calcIndex, std::map<int, Rail*> rails) {
	std::map<int, Rail *>::iterator it;
	it = rails.find(calcIndex);
	if (it != rails.end()) {
		if (this->prevIndex == -1) {
			prevIndex = calcIndex;
		} else if (this->nextIndex == -1) {
			nextIndex = calcIndex;
		} else if (this->nextIndexAlt == -1) {
			nextIndexAlt = calcIndex;
		}
	}
}

Rail::~Rail() {
}

bool Rail::isBumper() {
	return nextIndex == -1;
}

bool Rail::isSwitching() {
	return nextIndexAlt != -1;
}

int Rail::getIndex() {
	return index;
}

int Rail::getNext(int prv, int cur) {
	if (prv == nextIndex || prv == nextIndexAlt) {
		return prevIndex;
	}
	return nextIndex;
}

int Rail::getNextIndex() {
	return nextIndex;
}

int Rail::getPrevIndex() {
	return prevIndex;
}

void Rail::switching() {
	if (nextIndexAlt != -1) {
		int tmp = nextIndex;
		nextIndex = nextIndexAlt;
		nextIndexAlt = tmp;
	}
}

void Rail::drawHimself(SDL_Surface * surfaceToDraw) {
	SDL_Rect dstRect;
	dstRect.x = (index % 35) * smallSpriteLevelSizeWidth;
	dstRect.y = ((int) floor(index / 35)) * smallSpriteLevelSizeHeight;
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	SDL_FillRect(surfaceToDraw, &dstRect, 0x000000);

	int spriteIndex = 0;

	//Bumper
	if (nextIndex == -1) {
		if ((index - prevIndex) == -1) {
			spriteIndex = RailBumperLeft;
		} else {
			spriteIndex = RailBumperRight;
		}
	} else {
		int res = prevIndex - nextIndex;
		switch (res) {
			case 2:
			case -2:
				spriteIndex = RailHorizontal;
				break;
			case 70:
			case -70:
				spriteIndex = RailVertical;
				break;
			case 34:
				if (prevIndex - index == -1) {
					spriteIndex = RailLeftToTop;
					break;
				} else {
					spriteIndex = RailRightToBottom;
					break;
				}
			case -34:
				if (prevIndex - index == -35) {
					spriteIndex = RailLeftToTop;
					break;
				} else {
					spriteIndex = RailRightToBottom;
					break;
				}
			case 36:
				if (prevIndex - index == 35) {
					spriteIndex = RailLeftToBottom;
					break;
				} else {
					spriteIndex = RailRightToTop;
					break;
				}
			case -36:
				if (prevIndex - index == -1) {
					spriteIndex = RailLeftToBottom;
					break;
				} else {
					spriteIndex = RailRightToTop;
					break;
				}
		}
	}
	SDL_BlitSurface(Sprite::Instance().getRail(spriteIndex), NULL, surfaceToDraw, &dstRect);
	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().sendRail(index, spriteIndex);
	}
}

void Rail::print() {
	fprintf(stderr, "rail %i %i %i %i\n", index, nextIndex, prevIndex, nextIndexAlt);
}
