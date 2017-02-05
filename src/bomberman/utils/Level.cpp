#include "Level.h"

Level::Level(int numero, const char* name, bool waterOverlayMode){
	this->numero = numero;
	this->waterOverlayMode = waterOverlayMode;
	strcpy(this->name, name);
}

Level::~Level(){
	variantes.clear();
}

void Level::addVariante(LevelVariante * variante){
	variante->initRails();
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

bool Level::isWaterOverlayMode(){
	return waterOverlayMode;
}