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
	cur = -1;
	nxt = -1;
	prv = -1;
	move = false;
	placeInMiddle = false;
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
	cur = -1;
	nxt = -1;
	prv = -1;
	move = false;
	placeInMiddle = false;
}

Trolley::~Trolley() {
	this->players = NULL;
	this->rails = NULL;
	this->rail = NULL;
}

void Trolley::doSomething(SDL_Surface * surface) {
	correctValue();

	//si pas de mouvement on attend qu'un joueur passe sur le chariot
	if (!move) {
		for (int i = 0; i < nbPlayer; i++) {
			//a player is on the trolley start move, init the chained
			if (index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false) {
				activate[i] = true;
				move = true;
				//chargement du joueur : Bloquer le joueur de ses mouvements !
				players->at(i)->goInsideTrolley();
				players->at(i)->setTrolleyDirection(0);
				playerInside = i;
				//debut navigation
				rail = rails->find(index)->second;
				cur = index;
				prevIndex = index;
				nxt = rail->getNextIndex();
				prv = -1;
				if (nxt == -1) {
					nxt = rail->getPrevIndex();
				}
				fprintf(stderr, "idx  %i next %i\n", cur, nxt);
				break;
			} else if (index != GameConfig::Instance().getPlayerIndex(i) && activate[i] == true) {
				activate[i] = false;
				move = false;
			}
		}
	} else {
		/*
		 * deplacement du chariot
		 */

		x += speed;
		index = (int) floor(x) + (int) floor(y) * 35;


		if (!placeInMiddle) {
			if (index != prevIndex) {
				if (!rail->isBumper() || prv == -1) {
					//si sur rail différent d'une fin de voie ou au départ du chariot et si on change de case, on va chercher le prochain rail
					rail = rails->find(nxt)->second;
					prv = cur;
					cur = nxt;
					nxt = rail->getNext(prv, cur);
					fprintf(stderr, "idx  %i next %i\n", cur, nxt);
				}
				prevIndex = index;
			} else if (rail->isBumper() && prv != -1) {
				// si sur un rail de fin de voie et non pas au départ du chariot
				if (playerInside != -1) {
					placeInMiddle = true;
				}
			}
		}

		//On va arrêter le chariot au milieu d'une case
		if (placeInMiddle) {
			float xCalc = x - floor(x);
			float yCalc = y - floor(y);
			if (xCalc > 0.4 && xCalc < 0.6 && yCalc > 0.4 && yCalc < 0.6) {
				fprintf(stderr, "eject player %i at  %i\n", playerInside, index);
				players->at(playerInside)->goOutsideTrolley();
				playerInside = -1;
				move = false;
				placeInMiddle = false;
				rail = NULL;
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

void Trolley::correctValue() {
	//correct float value position float
	float margeInf = 0.55 - (speed / 2);
	float margeSup = 0.45 + (speed / 2);
	float roundX = (float) floor(x);
	float roundY = (float) floor(y);
	if (x - (float) roundX >= margeInf && x - (float) roundX <= margeSup) {
		x = (float) floor(x) + 0.5;
	}
	if (y - (float) roundY >= margeInf && y - (float) roundY <= margeSup) {
		y = (float) floor(y) + 0.5;
	}
}
