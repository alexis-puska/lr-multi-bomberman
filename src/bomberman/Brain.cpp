#include "Brain.h"

Brain::Brain(unsigned short * keystate, int tab[sizeX * sizeY], float * tabPlayerCoord, int nbPlayerConfig, int playerNumberControle, int posX, int posY){
	this->keystate = keystate;
	this->tab = tab;
	this->tabPlayerCoord = tabPlayerCoord;
	this->nbPlayerConfig = nbPlayerConfig;
	this->playerNumberControle = playerNumberControle;
	this->posX = posX;
	this->posY = posY;
	nbTick = 0;
}

Brain::~Brain(){
	keystate = NULL;
	tab = NULL;
	tabPlayerCoord = NULL;
}

void Brain::think(){
	*keystate = (short)0;
	if(nbTick>20){
		nbTick = 0;
	}
	if(nbTick<10){
		*keystate += (short)brainKeyRight;
	}else{
		*keystate += (short)brainKeyLeft;
	}
	nbTick++;
}