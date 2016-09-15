#include "Bombe.h"

#define nbFrame 6


Bombe::Bombe(int Strenght, int posXX, int posYY, int type, int numPlayerSetTheBombe, int nbTickBeforeExplostion, SDL_Surface ** miscSheet, int table[sizeX * sizeY]){

	tab = table;
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 4;

	strenght = Strenght;
	posX = posXX;
	posY = posYY;
	player = numPlayerSetTheBombe;
	nbTickExplode = nbTickBeforeExplostion;
	animation = miscSheet;
	bombeType = type;
	
}

Bombe::~Bombe(){
	animation = NULL;
	tab = NULL;
}

bool Bombe::isExplode(){

	if(nbTickExplode <=0){
		tab[posX + posY * sizeX ] = 0;
		return true;
	}else{
		return false;
	} 
}

void Bombe::explode(){
	tab[posX + posY * sizeX ] = 0;
	nbTickExplode = 0;
}

int Bombe::getStrenght(){
	return strenght;
}

int Bombe::getPosX(){
	return posX;
}

int Bombe::getPosY(){
	return posY;
}

int Bombe::getCase(){
	return posX + posY * sizeX;	
}

int Bombe::getPlayer(){
	return player;
}

bool Bombe::isPowerBombe(){
	if(bombeType == 2){
		return true;
	}else{
		return false;
	} 
}

void Bombe::pushBomb(int direction){
	
}

void Bombe::tick(SDL_Surface * surfaceToDraw){
	SDL_Rect dstRect;
	dstRect.x = posX * 18 +1;
	dstRect.y = posY * 16;
	dstRect.w = 16;
	dstRect.h = 16;
	
	
	if(frameCounter > nbFrame){
		frameCounter = 0;
		offsetSprite++;	
		if(offsetSprite >= nbFrameForAnimation){
			offsetSprite = 0;
		}
	}
	frameCounter++;
	int offsetSpriteAnimation = 0;
	switch (offsetSprite){
		case 0:
			offsetSpriteAnimation = 1;
			break;	
		case 1:
			offsetSpriteAnimation = 0;
			break;
		case 2:
			offsetSpriteAnimation = 1;
			break;
		case 3:
			offsetSpriteAnimation = 2;
			break;		
	}
	nbTickExplode--;
	SDL_BlitSurface(animation[offsetSpriteAnimation], NULL, surfaceToDraw, &dstRect);
}
