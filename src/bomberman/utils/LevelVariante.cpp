#include "LevelVariante.h"

LevelVariante::LevelVariante(int number, const char * descriptionLine1, const char * descriptionLine2, bool fillWithBrick, int definition[735], int bonus[15], int fireStrenght, int nbBombe) {
	this->number = number;
	strcpy(this->descriptionLine1, descriptionLine1);
	strcpy(this->descriptionLine2, descriptionLine2);
	this->fillWithBrick = fillWithBrick;
	this->fireStrenght = fireStrenght;
	this->nbBombe = nbBombe;
	for (int i = 0; i < 735; i++) {
		this->definition[i] = definition[i];
		reserved[i] = false;
	}

	for (int i = 0; i < 15; i++) {
		this->bonus[i] = bonus[i];
	}

	for (int i = 0; i < 16; i++) {
		this->start[i] = 0;
	}

	int idxStart = 0;
	for (int i = 0; i < 735; i++) {
		if (this->definition[i] == 'S' || this->definition[i] == 'T') {
			start[idxStart] = i;
			reserved[i] = true;
			reserved[i - 1] = true;
			reserved[i + 1] = true;
			reserved[i - 34] = true;
			reserved[i - 35] = true;
			reserved[i - 36] = true;
			reserved[i + 34] = true;
			reserved[i + 35] = true;
			reserved[i + 36] = true;
			idxStart++;
		}
	}
}

LevelVariante::~LevelVariante() {
}

char LevelVariante::getDefinition(int index) {
	return definition[index];
}

int LevelVariante::getBonus(int index) {
	return bonus[index];
}

bool LevelVariante::isFillWithBricks() {
	return fillWithBrick;
}

int LevelVariante::getNumber() {
	return number;
}

char * LevelVariante::getDescriptionLine1() {
	return descriptionLine1;
}

char * LevelVariante::getDescriptionLine2() {
	return descriptionLine2;
}

bool LevelVariante::isReserved(int idx) {
	return reserved[idx];
}

int LevelVariante::getStart(int idx) {
	return start[idx];
}

int LevelVariante::getNbBombe() {
	return nbBombe;
}

int LevelVariante::getFireStrenght() {
	return fireStrenght;
}

void LevelVariante::textureIsAWall(int idx) {
	indexDefWall[idx] = idx;
}

void LevelVariante::textureIsAWall(int startIdx, int stopIdx) {
	for (int i = startIdx; i <= stopIdx; i++) {
		indexDefWall[i] = i;
	}
}

void LevelVariante::texturedrawInsky(int idx) {
	indexDefSky[idx] = idx;
}

bool LevelVariante::isAWall(int idx) {
	if (indexDefWall.count(idx)) {
		return true;
	}
	return false;
}

bool LevelVariante::isDrawInSky(int idx) {
	if (indexDefSky.count(idx)) {
		return true;
	}
	return false;
}

void LevelVariante::addRail(int index, RailSwitch * rail) {
	rails[index] = rail;
}

void LevelVariante::addRail(int index) {
	rails[index] = NULL;
}

std::map<int, RailSwitch *> LevelVariante::getRailsIndex() {
	return rails;
}

void LevelVariante::addButton(int index) {
	buttons.push_back(index);
}

std::vector<int> LevelVariante::getButtonsIndex() {
	return buttons;
}






void LevelVariante::addMine(int index){
	mines.push_back(index);
}
std::vector<int> LevelVariante::getMinesIndex(){
	return mines;
}
void LevelVariante::addHole(int index){
	holes.push_back(index);
}
std::vector<int> LevelVariante::getHolesIndex(){
	return holes;
}
void LevelVariante::addTeleporter(int index){
	teleporters.push_back(index);
}
std::vector<int> LevelVariante::getTeleportersIndex(){
	return teleporters;
}
void LevelVariante::addTrolley(int index){
	trolleys.push_back(index);
}
std::vector<int> LevelVariante::getTrolleysIndex(){
	return trolleys;
}