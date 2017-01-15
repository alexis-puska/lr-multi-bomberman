#include "Level.h"

Level::Level(int numero, const char* name){
	this->numero = numero;
	strcpy(this->name, name);
}

Level::~Level(){
	variantes.clear();
}

void Level::addVariante(LevelVariante * variante){
	variantes.push_back(variante);
}

char * Level::getName(){
	return name;
}

int Level::getVariantesSize(){
	return variantes.size();
}

LevelVariante * Level::getVariantes(int i){
	return variantes[i];
}