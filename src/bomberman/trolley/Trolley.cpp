#include "Trolley.h"

#define sprite_sizeW 30
#define sprite_sizeH 42
#define blockSizeX 18
#define blockSizeY 16

Trolley::Trolley(int index) {
	this->index = index;
	this->prevIndex = -1;
}

Trolley::~Trolley() {

}

void Trolley::doSomething(SDL_Surface * surface) {

}

void Trolley::drawHimself(SDL_Surface * surface) {
	SDL_Rect dstRect;
	int posX = (index % 35);
	int posY = ((int) floor(index / 35));
	dstRect.x = 1;//(posX * blockSizeX) - ((sprite_sizeW - blockSizeX) / 2);
	dstRect.y = 1;//(posY * blockSizeY) - ((sprite_sizeH - blockSizeY) );
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	SDL_BlitSurface(Sprite::Instance().getTrolley(0), NULL, surface, &dstRect);
}

int Trolley::getCurrentIndex() {
	return index;
}
int Trolley::getPrevIndex() {
	return prevIndex;
}
