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

void LevelVariante::addRail(int index) {
	fprintf(stderr, "add index : %i", index);
	Rail * rail = new Rail(index);
	rails[index] = rail;
}

std::map<int, Rail *> LevelVariante::getRails() {
	return rails;
}

void LevelVariante::initRails() {
	int index = 0;
	if (rails.size() != 0) {
		fprintf(stderr, "init rails !\n");
		for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
			fprintf(stderr, "rail index : %i\n", it->second->getIndex());
			it->second->init(rails);
		}
		for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
			if (it->second->isBumper()) {
				index = it->second->getIndex();
				break;
			}
		}
		fprintf(stderr, "%i index found\n", index);
		Rail * rail = rails.find(index)->second;
		fprintf(stderr, "rail %i start, next %i", rail->getIndex(), rail->getNext(index));
		rail = rails.find(rail->getNext(index))->second;
		while (true) {
			if (!rail->isBumper()) {
				rail = rails.find(rail->getNext(index))->second;
				fprintf(stderr, "rail %i start", rail->getIndex());
			} else {
				break;
			}
		}
		rails.find(112)->second->switching();

		for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
					if (it->second->isBumper()) {
						index = it->second->getIndex();
						break;
					}
				}
				fprintf(stderr, "%i index found\n", index);
				rail = rails.find(index)->second;
				fprintf(stderr, "rail %i start, next %i", rail->getIndex(), rail->getNext(index));
				rail = rails.find(rail->getNext(index))->second;
				while (true) {
					if (!rail->isBumper()) {
						rail = rails.find(rail->getNext(index))->second;
						fprintf(stderr, "rail %i start", rail->getIndex());
					} else {
						break;
					}
				}
	}

}
