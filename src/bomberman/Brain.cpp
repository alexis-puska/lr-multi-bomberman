#include "Brain.h"

Brain::Brain(unsigned short * keystateGame, int tabGame[sizeX * sizeY], float tabPlayerCoordGame[], int nbPlayerConfigGame, int playerNumberControleGame, int posXGame, int posYGame){
	keystate = keystateGame;
	tab = tabGame;
	tabPlayerCoord = tabPlayerCoordGame;
	nbPlayerConfig = nbPlayerConfigGame;
	playerNumberControle = playerNumberControleGame;
	posX = posXGame;
	posY = posYGame;
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