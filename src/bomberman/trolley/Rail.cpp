#include "rail.h"

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
	fprintf(stderr, "init rail %i\n", this->getIndex());

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
		} else {
			nextIndexAlt = calcIndex;
		}
	}
	fprintf(stderr, "init calcindex : %i, %i, %i, %i, %i\n", calcIndex, index, prevIndex, nextIndex, nextIndexAlt);
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

int Rail::getNext(int prev) {
	if (prev == nextIndex) {
		return prevIndex;
	} else {
		if(nextIndex != -1){
			return nextIndex;
		}else{
			return prevIndex;
		}
	}
}

void Rail::switching() {
	fprintf(stderr, "switching %i <-> %i", nextIndex, nextIndexAlt);
	int tmp = nextIndex;
	nextIndex = nextIndexAlt;
	nextIndexAlt = tmp;
	fprintf(stderr, "switching %i <-> %i", nextIndex, nextIndexAlt);
}

void Rail::drawHimself(SDL_Surface * surfaceToDraw) {

	SDL_Rect dstRect;
	dstRect.x = index % 35;
	dstRect.y = (int) floor(index / 35);
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;

	//Bumper
	if (prevIndex == -1) {
		if ((index - prevIndex) == -1) {
			SDL_BlitSurface(Sprite::Instance().getRail(RailBumperRight), NULL, surfaceToDraw, &dstRect);
			return;
		} else {
			SDL_BlitSurface(Sprite::Instance().getRail(RailBumperLeft), NULL, surfaceToDraw, &dstRect);
			return;
		}
	} else {
		int res = prevIndex - nextIndex;
		switch (res) {
			case 2:
			case -2:
				SDL_BlitSurface(Sprite::Instance().getRail(RailHorizontal), NULL, surfaceToDraw, &dstRect);
				break;
			case 70:
			case -70:
				SDL_BlitSurface(Sprite::Instance().getRail(RailVertical), NULL, surfaceToDraw, &dstRect);
				break;
			case 34:
				if (prevIndex - index == -1) {
					SDL_BlitSurface(Sprite::Instance().getRail(RailLeftToTop), NULL, surfaceToDraw, &dstRect);
					break;
				} else {
					SDL_BlitSurface(Sprite::Instance().getRail(RailRightToBottom), NULL, surfaceToDraw, &dstRect);
					break;
				}
			case -34:
				if (prevIndex - index == -35) {
					SDL_BlitSurface(Sprite::Instance().getRail(RailLeftToTop), NULL, surfaceToDraw, &dstRect);
					break;
				} else {
					SDL_BlitSurface(Sprite::Instance().getRail(RailRightToBottom), NULL, surfaceToDraw, &dstRect);
					break;
				}
			case 36:
				if (prevIndex - index == 35) {
					SDL_BlitSurface(Sprite::Instance().getRail(RailLeftToBottom), NULL, surfaceToDraw, &dstRect);
					break;
				} else {
					SDL_BlitSurface(Sprite::Instance().getRail(RailRightToTop), NULL, surfaceToDraw, &dstRect);
					break;
				}
			case -36:
				if (prevIndex - index == -1) {
					SDL_BlitSurface(Sprite::Instance().getRail(RailLeftToBottom), NULL, surfaceToDraw, &dstRect);
					break;
				} else {
					SDL_BlitSurface(Sprite::Instance().getRail(RailRightToTop), NULL, surfaceToDraw, &dstRect);
					break;
				}
		}
	}
}
