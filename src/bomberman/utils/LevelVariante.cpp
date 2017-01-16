#include "LevelVariante.h"

LevelVariante::LevelVariante(int number, const char * descriptionLine1, const char * descriptionLine2, bool fillWithBrick, int definition[735], int bonus[13], int fireStrenght, int nbBombe){
	this->number = number;
	strcpy(this->descriptionLine1, descriptionLine1);
	strcpy(this->descriptionLine2, descriptionLine2);
	this->fillWithBrick = fillWithBrick;
	this->fireStrenght = fireStrenght;
	this->nbBombe = nbBombe;
	for(int i=0; i<735; i++){
		this->definition[i] = definition[i];
		reserved[i]=false;
	}
	
	for(int i=0; i<13; i++){
		this->bonus[i] = bonus[i];
	}
	
	for(int i=0; i<16; i++){
		this->start[i] = 0;
	}
	
	int idxStart = 0;
	for(int i=0; i<735; i++){
		if(this->definition[i]=='S' || this->definition[i]=='T'){
			start[idxStart]=i;
			reserved[i]=true;
			reserved[i-1]=true;
			reserved[i+1]=true;
			reserved[i-34]=true;
			reserved[i-35]=true;
			reserved[i-36]=true;
			reserved[i+34]=true;
			reserved[i+35]=true;
			reserved[i+36]=true;
			idxStart++;
		}
	}
}

LevelVariante::~LevelVariante(){
}

char LevelVariante::getDefinition(int index){
	return definition[index];
}

int LevelVariante::getBonus(int index){
	return bonus[index];
}

bool LevelVariante::isFillWithBricks(){
	return fillWithBrick;
}

int LevelVariante::getNumber(){
	return number;
}

char * LevelVariante::getDescriptionLine1(){
	return descriptionLine1;
}

char * LevelVariante::getDescriptionLine2(){
	return descriptionLine2;
}

bool LevelVariante::isReserved(int idx){
	return reserved[idx];
}

int LevelVariante::getStart(int idx){
	return start[idx];	
}