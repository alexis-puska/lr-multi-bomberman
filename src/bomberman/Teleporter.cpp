#include "Teleporter.h"

Teleporter::Teleporter(int index) {
	this->index = index;
}

Teleporter::~Teleporter() {
}

bool Teleporter::doSomething(){
//	if(position < nb){
		for(int i = 0; i< nbPlayer; i++){
			if(index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false){
				Sound::Instance().playTeleporterOpenSound();
				activate[i] = true;
			}else if(index != GameConfig::Instance().getPlayerIndex(i) && activate[i] == true){
				activate[i] = false;
			}
		}
//	}
//	if (position == nb){
//		return true;
//	}
	return false;
}

void Teleporter::drawHimself(SDL_Surface * surfaceToDraw) {
	SDL_Rect dstRect;
	dstRect.x = (index % 35) * smallSpriteLevelSizeWidth;
	dstRect.y = ((int) floor(index / 35)) * smallSpriteLevelSizeHeight;
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	SDL_BlitSurface(Sprite::Instance().getTeleporter(0), NULL, surfaceToDraw, &dstRect);
}
