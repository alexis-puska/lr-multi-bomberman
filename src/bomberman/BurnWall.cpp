#include "BurnWall.h"

#define nbFrame 4


BurnWall::BurnWall(int posXX, int posYY, int index, SDL_Surface ** miscSheet, int table[sizeX * sizeY], int tableBonus[sizeX * sizeY]){
	indexBurnWall = index;
	posX = posXX;
	posY = posYY;
	animation = miscSheet;
	tab = table;
	tabBonus = tableBonus;
	frameCounter = 0;
	offsetSprite = 0;
    nbFrameForAnimation = 6;
    deleteAnimation = false;
}

BurnWall::~BurnWall(){
	animation = NULL;
	tab = NULL;
	tabBonus = NULL;
}

bool BurnWall::canBeDelete(){
	
	if(deleteAnimation){
		//remove grid status BurnWall
		if(tab[posX + posY * sizeX] == 2){
			tab[posX + posY * sizeX] = 0;	
		}
	}
	return deleteAnimation;
}

void BurnWall::tick(SDL_Surface * surfaceToDraw){
	SDL_Rect dstRect;
	dstRect.x = posX * 18;
	dstRect.y = posY * 16;
	dstRect.w = 18;
	dstRect.h = 16;
	if(frameCounter > nbFrame){
		frameCounter = 0;
		offsetSprite++;	
		if(offsetSprite >= nbFrameForAnimation){
			deleteAnimation = true;
			return;
		}
	}
	frameCounter++;
	SDL_BlitSurface(animation[offsetSprite], NULL, surfaceToDraw, &dstRect);
}
