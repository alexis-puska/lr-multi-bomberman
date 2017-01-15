#include "LevelVariante.h"

LevelVariante::LevelVariante(int number, const char * description, bool fillWithBrick, int definition[735], int bonus[13]){
	this->number = number;
	strcpy(this->description, description);
	this->fillWithBrick = fillWithBrick;
	for(int i=0; i<735; i++){
		this->definition[i] = definition[i];
	}
	for(int i=0; i<13; i++){
		this->bonus[i] = bonus[i];
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

char * LevelVariante::getDescription(){
	return description;
}