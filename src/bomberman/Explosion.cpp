#include "Explosion.h"

#define nbFrame 4


Explosion::Explosion(int posXX, int posYY, int index, SDL_Surface ** miscSheet, int table[sizeX * sizeY], int tableBonus[sizeX * sizeY]){
	indexExplosion = index;
	posX = posXX;
	posY = posYY;
	animation = miscSheet;
	tab = table;
	tabBonus = tableBonus;
	frameCounter = 0;
	offsetSprite = 0;
    nbFrameForAnimation = 7;
    deleteAnimation = false;
    //set explosion in the grid
    tab[posX + posY * sizeX] = explosionElement;
}


Explosion::~Explosion(){
	animation = NULL;
	tab = NULL;
	tabBonus = NULL;
}

bool Explosion::canBeDelete(){
	
	if(deleteAnimation){
		//remove grid status explosion
		tab[posX + posY * sizeX] = emptyElement;	
	}
	return deleteAnimation;
}


void Explosion::tick(SDL_Surface * surfaceToDraw){
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
	int offsetSpriteAnimation = 0;
	switch (offsetSprite){
		case 0:
			offsetSpriteAnimation = 3;
			break;	
		case 1:
			offsetSpriteAnimation = 2;
			break;
		case 2:
			offsetSpriteAnimation = 1;
			break;
		case 3:
			offsetSpriteAnimation = 0;
			break;
		case 4:
			offsetSpriteAnimation = 1;
			break;	
		case 5:
			offsetSpriteAnimation = 2;
			break;
		case 6:
			offsetSpriteAnimation = 3;
			break;	
	}
	SDL_BlitSurface(animation[offsetSpriteAnimation + (indexExplosion * 4)], NULL, surfaceToDraw, &dstRect);
}
