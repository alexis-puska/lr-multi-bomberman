#include "Trolley.h"

#define sprite_sizeW 30
#define sprite_sizeH 42
#define blockSizeX 18
#define blockSizeY 16

Trolley::Trolley(int index) {
	this->index = index;
	this->prevIndex = -1;
	this->playerInside = -1;
	this->playerOutside = -1;
	this->direction = 0;
	move = false;
}

Trolley::Trolley(int index, std::vector<Player*> * players, std::map<int, Rail *> * rails) {
	this->index = index;
	this->prevIndex = -1;
	this->playerInside = -1;
	this->playerOutside = -1;
	this->direction = 0;
	this->players = players;
	this->rails = rails;

	move = false;
}

Trolley::~Trolley() {
	this->players = NULL;
	this->rails = NULL;
}

void Trolley::doSomething(SDL_Surface * surface) {
	for (int i = 0; i < nbPlayer; i++) {
		if (index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false) {
			activate[i] = true;
			players->at(i)->goInsideTrolley();
			players->at(i)->setTrolleyDirection(0);
			playerInside = i;
			playerOutside = -1;

			Rail * rail = rails->find(index)->second;

			int cur = index;
			int nxt = rail->getNextIndex();
			int prv = -1;
			fprintf(stderr, "start idx %i next %i\n", cur, nxt);

			while (true) {

				if (!rail->isBumper()) {
					prv = cur;
					cur = nxt;

					nxt = rail->getNext(prv, cur);

					if (nxt == -1) {
						fprintf(stderr, "aiguillage en mauvaise position\n");
						break;
					}
					rail = rails->find(nxt)->second;

					fprintf(stderr, "idx  %i next %i\n", cur, nxt);

				} else {
					fprintf(stderr, "break");
					break;
				}
			}
			//fprintf(stderr,"break2");

			break;
		} else if (index != GameConfig::Instance().getPlayerIndex(i) && activate[i] == true) {
			activate[i] = false;
			if (playerInside != -1) {
				players->at(playerInside)->goOutsideTrolley();
				playerOutside = playerInside;
				playerInside = -1;
			}
		}
	}
}

void Trolley::drawHimself(SDL_Surface * surface) {
	SDL_Rect dstRect;
	float posX = (float) (index % 35) + 0.5;
	float posY = (float) (floor(index / 35)) + 0.5;
	dstRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
	dstRect.y = (posY * blockSizeY) - (sprite_sizeH - 10);
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	SDL_BlitSurface(Sprite::Instance().getTrolley(0), NULL, surface, &dstRect);
}

int Trolley::getCurrentIndex() {
	return index;
}
int Trolley::getPrevIndex() {
	return prevIndex;
}

bool Trolley::isMove() {
	return move;
}

int Trolley::getPlayerInside() {
	return playerInside;
}

int Trolley::getPlayerOutside() {
	int tmp = playerOutside;
	playerOutside = -1;
	return tmp;;
}

int Trolley::getDirection() {
	return direction;
}
