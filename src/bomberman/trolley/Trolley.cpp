#include "Trolley.h"

#define sprite_sizeW 30
#define sprite_sizeH 42
#define blockSizeX 18
#define blockSizeY 16
#define speed 0.2

Trolley::Trolley(int index) {
	this->index = index;
	x = (float) (index % 35) + 0.5;
	y = (float) (floor(index / 35)) + 0.5;
	this->prevIndex = -1;
	this->playerInside = -1;
	this->direction = 0;
	move = false;
}

Trolley::Trolley(int index, std::vector<Player*> * players, std::map<int, Rail *> * rails) {
	this->index = index;
	x = (float) (index % 35) + 0.5;
	y = (float) (floor(index / 35)) + 0.5;
	this->prevIndex = -1;
	this->playerInside = -1;
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
	//correct float value position float
	float margeInf = 0.51 - (speed / 2);
	float margeSup = 0.49 + (speed / 2);
	int roundX = (int) floor(x);
	int roundY = (int) floor(y);
	if (x - (float) roundX >= margeInf && x - (float) roundX <= margeSup) {
		x = (float) floor(x) + 0.5;
	}
	if (y - (float) roundY >= margeInf && y - (float) roundY <= margeSup) {
		y = (float) floor(y) + 0.5;
	}

	//si pas de mouvement on attend qu'un joueur passe sur le chariot
	if (!move) {
		for (int i = 0; i < nbPlayer; i++) {
			if (index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false) {
				activate[i] = true;
				move = true;
				players->at(i)->goInsideTrolley();
				players->at(i)->setTrolleyDirection(0);
				playerInside = i;
				break;
			} else if (index != GameConfig::Instance().getPlayerIndex(i) && activate[i] == true) {
				activate[i] = false;
				move = false;
			}
		}
	} else {
		//debut navigation
		Rail * rail = rails->find(index)->second;
		int cur = index;
		int nxt = rail->getNextIndex();
		int prv = -1;
		if (nxt == -1) {
			fprintf(stderr, "-1 detected");
			nxt = rail->getPrevIndex();
		}
		fprintf(stderr, "start idx %i next %i\n", cur, nxt);
		while (true) {
			if (!rail->isBumper() || prv == -1) {
				rail = rails->find(nxt)->second;
				prv = cur;
				cur = nxt;
				nxt = rail->getNext(prv, cur);
				fprintf(stderr, "idx  %i next %i\n", cur, nxt);
			} else {
				if (playerInside != -1) {
					fprintf(stderr, "eject player %i at  %i\n", playerInside, cur);
					players->at(playerInside)->goOutsideTrolley();
					playerInside = -1;
					move = false;
				}
				break;
			}
		}

	}
	drawHimself(surface);
}

void Trolley::drawHimself(SDL_Surface * surface) {
	SDL_Rect dstRect;
	dstRect.x = (x * blockSizeX) - (sprite_sizeW / 2);
	dstRect.y = (y * blockSizeY) - (sprite_sizeH - 10);
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	SDL_BlitSurface(Sprite::Instance().getTrolley(0), NULL, surface, &dstRect);
}

int Trolley::getCurrentIndex() {
	return index;
}

bool Trolley::isMove() {
	return move;
}
