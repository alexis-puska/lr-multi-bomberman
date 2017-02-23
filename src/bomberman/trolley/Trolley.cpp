#include "Trolley.h"

#define sprite_sizeW 30
#define sprite_sizeH 42
#define blockSizeX 18
#define blockSizeY 16
#define speed 0.2

Trolley::Trolley(int index) {
	this->index = index;
	x = (double) (index % 35) + 0.5;
	y = (double) (floor(index / 35)) + 0.5;
	this->prevIndex = -1;
	this->playerInside = -1;
	this->direction = 0;
	this->playerDirection = 0;
	cur = -1;
	nxt = -1;
	prv = -1;
	move = false;
	placeInMiddle = false;
}

Trolley::Trolley(int index, std::vector<Player*> * players, std::map<int, Rail *> * rails) {
	this->index = index;
	x = (double) (index % 35) + 0.5;
	y = (double) (floor(index / 35)) + 0.5;
	this->prevIndex = -1;
	this->playerInside = -1;
	this->direction = 0;
	this->players = players;
	this->playerDirection = 0;
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
				//TODO
				//chargement dujoueur : Bloquer le joueur de ses mouvements !
				players->at(i)->goInsideTrolley();
				players->at(i)->setTrolleyDirection(playerDirection);
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
				//fprintf(stderr, "idx  %i next %i\n", cur, nxt);
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

		moveTrolley();

		if (!placeInMiddle) {
			if (index != prevIndex) {
				if (!rail->isBumper() || prv == -1) {
					//si sur rail différent d'une fin de voie ou au départ du chariot et si on change de case, on va chercher le prochain rail
					rail = rails->find(nxt)->second;
					prv = cur;
					cur = nxt;
					nxt = rail->getNext(prv, cur);
					//fprintf(stderr, "idx  %i next %i\n", cur, nxt);
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
			double xCalc = x - floor(x);
			double yCalc = y - floor(y);
			if (xCalc > 0.4 && xCalc < 0.6 && yCalc > 0.4 && yCalc < 0.6) {
				//fprintf(stderr, "eject player %i at  %i\n", playerInside, index);
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
	SDL_BlitSurface(Sprite::Instance().getTrolley(direction), NULL, surface, &dstRect);
}

int Trolley::getCurrentIndex() {
	return index;
}

bool Trolley::isMove() {
	return move;
}

int Trolley::getPreviousIndex(){
	return prv;
}

void Trolley::correctValue() {
	//correct double value position double
	double margeInf = 0.55 - (speed / 2);
	double margeSup = 0.45 + (speed / 2);
	double roundX = (double) floor(x);
	double roundY = (double) floor(y);
	if (x - (double) roundX >= margeInf && x - (double) roundX <= margeSup) {
		x = (double) floor(x) + 0.5;
	}
	if (y - (double) roundY >= margeInf && y - (double) roundY <= margeSup) {
		y = (double) floor(y) + 0.5;
	}
}

void Trolley::moveTrolley() {
	/*
	 drawPlayerNorth
	 drawPlayerNorthEast
	 drawPlayerEast
	 drawPlayerSouthEast
	 drawPlayerSouth
	 drawPlayerSouthWest
	 drawPlayerWest
	 drawPlayerNorthWest
	 */
	if (prv == -1) {
		if ((nxt - cur) == 1) {
			x += speed;
			index = (int) floor(x) + (int) floor(y) * 35;
			direction = drawTrolleyHorizontal;
			playerDirection = drawPlayerEast;
		} else if ((nxt - cur) == -1) {
			x -= speed;
			index = (int) floor(x) + (int) floor(y) * 35;
			direction = drawTrolleyHorizontal;
			playerDirection = drawPlayerWest;
		}
	} else if (nxt != -1) {
		switch (nxt - prv) {
			case -36:
				//NC
				//-P
				if (nxt - cur == -1) {
					//Y
					x -= speed;
					if (y - floor(y) > 0.6) {
						y -= speed;
					}
					//N-
					//CP
				} else {
					if (x - floor(x) > 0.6) {
						x -= speed;
					}
					y -= speed;
				}
				direction = drawTrolleyTopLeftBottomRight;
				playerDirection = drawPlayerNorthWest;
				break;
			case -34:
				//CN
				//P-
				if (nxt - cur == 1) {
					x += speed;
					if (y - floor(y) > 0.6) {
						y -= speed;
					}
					//-N
					//PC
				} else {
					if (x - floor(x) < 0.4) {
						x += speed;
					}
					y -= speed;
				}
				direction = drawTrolleyTopRightBottomLeft;
				playerDirection = drawPlayerNorthEast;
				break;
			case -2:
				x -= speed;
				previousMove = 0;
				direction = drawTrolleyHorizontal;
				playerDirection = drawPlayerWest;
				break;
			case 2:
				x += speed;
				previousMove = 1;
				direction = drawTrolleyHorizontal;
				playerDirection = drawPlayerEast;
				break;
			case 70:
				y += speed;
				direction = drawTrolleyVertical;
				playerDirection = drawPlayerSouth;
				break;
			case -70:
				y -= speed;
				direction = drawTrolleyVertical;
				playerDirection = drawPlayerNorth;
				break;
			case 34:
				//CP
				//N-
				if (nxt - cur == 35) {
					if (x - floor(x) > 0.6) {
						x -= speed;
					}
					y += speed;
					//-P
					//NC
				} else {
					x -= speed;
					if (y - floor(y) < 0.4) {
						y += speed;
					}
				}
				direction = drawTrolleyTopRightBottomLeft;
				playerDirection = drawPlayerSouthWest;
				break;
			case 36:
				//PC
				//-N
				//fprintf(stderr, "%f, %f\n", x, y);
				if (nxt - cur == 35) {
					if (x - floor(x) < 0.4) {
						x += speed;
					}
					y += speed;
					//P-
					//CN
				} else {
					x += speed;
					if (y - floor(y) < 0.4) {
						y += speed;
					}
				}
				direction = drawTrolleyTopLeftBottomRight;
				playerDirection = drawPlayerSouthEast;
				break;
		}
		index = (int) floor(x) + (int) floor(y) * 35;
	} else {
		if (previousMove == 0) {
			x -= speed;
			direction = drawTrolleyHorizontal;
			playerDirection = drawPlayerWest;
		} else {
			x += speed;
			direction = drawTrolleyHorizontal;
			playerDirection = drawPlayerEast;
		}
	}
	players->at(playerInside)->setTrolleyDirection(playerDirection);
	GameConfig::Instance().updatePlayerPosition(playerInside, x, y);
}
