#include "Button.h"

Button::Button(int index, std::map<int, Trolley *> * trolleys) {
	this->index = index;
	this->pressed = false;
	this->trolleys = trolleys;
	for(int i = 0; i< nbPlayer; i++){
		activate[i] = false;
	}
}

Button::~Button() {
	trolleys = NULL;
}

bool Button::doSomething(SDL_Surface * surfaceToDraw){
	bool switched = false;
	bool oneTrolleyMove;
	for (std::map<int, Trolley*>::iterator it = trolleys->begin(); it != trolleys->end(); ++it) {
		if(it->second->isMove()){
			return false;
		}
	}
	for(int i = 0; i< nbPlayer; i++){
		if(index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false){
			switching(surfaceToDraw);
			switched = true;
			activate[i] = true;
		}else if(index != GameConfig::Instance().getPlayerIndex(i) && activate[i] == true){
			activate[i] = false;
		}
	}
	return switched;
}

void Button::switching(SDL_Surface * surfaceToDraw){
	if(pressed){
		pressed = false;
	}else{
		pressed = true;
	}
	drawHimself(surfaceToDraw);
}

void Button::drawHimself(SDL_Surface * surfaceToDraw) {
	SDL_Rect dstRect;
	dstRect.x = (index % 35) * smallSpriteLevelSizeWidth;
	dstRect.y = ((int) floor(index / 35)) * smallSpriteLevelSizeHeight;
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	if (pressed) {
		SDL_BlitSurface(Sprite::Instance().getButton(0), NULL, surfaceToDraw, &dstRect);
		return;
	} else {
		SDL_BlitSurface(Sprite::Instance().getButton(1), NULL, surfaceToDraw, &dstRect);
		return;
	}
}
