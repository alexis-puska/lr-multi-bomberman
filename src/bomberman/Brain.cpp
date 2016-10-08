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
	astar = new AStar(tab);
}

Brain::~Brain(){
	free(astar);
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
	
if(playerNumberControle == 1){
		astar -> init(0,0,5,5);
		astar -> solve();
		if(astar -> isSolved()) {
			fprintf(stderr,"Brain N° %i chemin vers cible : ", playerNumberControle);
			Cell * current = astar->getEnd();
			current ->printHimself();
			while(true) {
				if(!current->isOriginal()) {
					fprintf(stderr," ->");
					current->getParent()->printHimself();
					current = current-> getParent();} else {break;}
			}
			fprintf(stderr,"\n");
		} else {
			fprintf(stderr,"pas de chemin possible vers cible \n");
		}
}

	
}