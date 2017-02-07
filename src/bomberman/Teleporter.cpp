//#include "Teleporter.h"
//
//Teleporter::Teleporter(int index) {
//	this->index = index;
//	this->position = 0;
//	for(int i = 0; i< nbPlayer; i++){
//		activate[i] = false;
//	}
//}
//
//Teleporter::~Teleporter() {
//}
//
//bool Teleporter::doSomething(){
//	if(position < nb){
//		for(int i = 0; i< nbPlayer; i++){
//			if(index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false){
//				position++;
//				activate[i] = true;
//			}else if(index != GameConfig::Instance().getPlayerIndex(i) && activate[i] == true){
//				activate[i] = false;
//			}
//		}
//	}
//	if (position == nb){
//		return true;
//	}
//	return false;
//}
//
//void Teleporter::drawHimself(SDL_Surface * surfaceToDraw) {
//	SDL_Rect dstRect;
//	dstRect.x = (index % 35) * smallSpriteLevelSizeWidth;
//	dstRect.y = ((int) floor(index / 35)) * smallSpriteLevelSizeHeight;
//	dstRect.w = smallSpriteLevelSizeWidth;
//	dstRect.h = smallSpriteLevelSizeHeight;
//	if (position != 0 && position <= nb) {
//		SDL_BlitSurface(Sprite::Instance().getTeleporter(0), NULL, surfaceToDraw, &dstRect);
//		return;
//	} else if (position >= nb) {
//		SDL_BlitSurface(Sprite::Instance().getTeleporter(1), NULL, surfaceToDraw, &dstRect);
//		return;
//	}
//}
