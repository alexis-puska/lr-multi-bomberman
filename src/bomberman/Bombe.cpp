#include "Bombe.h"

#define nbFrame 6
#define bombeMoveSpeed 0.2

Bombe::Bombe(int strenght, float posX, float posY, int bombeType, int player, int nbTickExplode, int tab[sizeX * sizeY], float * tabPlayerCoord) {
	this->strenght = strenght;
	this->posX = posX;
	this->posY = posY;
	this->player = player;
	this->nbTickExplode = nbTickExplode;
	this->bombeType = bombeType;
	this->tabPlayerCoord = tabPlayerCoord;
	this->tab = tab;
	direction = -1;
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 4;
}

Bombe::~Bombe() {
	tab = NULL;
	tabPlayerCoord = NULL;
}

bool Bombe::isExplode() {
	if (nbTickExplode == 0) {
		tab[(int) floor(posX) + (int) floor(posY) * sizeX] = emptyElement;
		return true;
	} else {
		return false;
	}
}

int Bombe::getStrenght() {
	return strenght;
}

int Bombe::getPosX() {
	return posX;
}

int Bombe::getPosY() {
	return posY;
}

int Bombe::getCase() {
	return (int) floor(posX) + (int) floor(posY) * sizeX;
}

int Bombe::getPlayer() {
	return player;
}

bool Bombe::isPowerBombe() {
	if (bombeType == powerBombeType) {
		return true;
	} else {
		return false;
	}
}

void Bombe::explodeNow() {
	tab[(int) floor(posX) + (int) floor(posY) * sizeX] = emptyElement;
	nbTickExplode = 0;
}

void Bombe::explode() {
	tab[(int) floor(posX) + (int) floor(posY) * sizeX] = emptyElement;
	nbTickExplode = 1;
}

void Bombe::pushBomb(int directionPlayer) {
	direction = directionPlayer;
}

void Bombe::tick(SDL_Surface * surfaceToDraw) {
	SDL_Rect dstRect;
	dstRect.x = (posX * 18) - 8;
	dstRect.y = (posY * 16) - 8;
	dstRect.w = 16;
	dstRect.h = 16;

	//correct value of position
	float margeInf = 0.51 - (bombeMoveSpeed / 2);
	float margeSup = 0.49 + (bombeMoveSpeed / 2);
	int roundX = (int) floor(posX);
	int roundY = (int) floor(posY);

	if (posX - (float) roundX >= margeInf && posX - (float) roundX <= margeSup) {
		posX = (float) floor(posX) + 0.5;
	}
	if (posY - (float) roundY >= margeInf && posY - (float) roundY <= margeSup) {
		posY = (float) floor(posY) + 0.5;
	}

	if (posY == (float) sizeY + 0.5) {
		if (bombeType == bubbleBombeType) {
			direction = kickOnUp;
		} else {
			direction = -1;
		}
	}

	if (posY == 0.5) {
		if (bombeType == bubbleBombeType) {
			direction = kickOnDown;
		} else {
			direction = -1;
		}
	}

	if (direction != -1) {
		if (tab[(int) floor(posX) + (int) floor(posY) * sizeX] == explosionElement) {
			explode();
		} else {
			//bombe moved
			tab[(int) floor(posX) + (int) floor(posY) * sizeX] = emptyElement;
			bool nextCaseIsPlayer = false;
			switch (direction) {
				case kickOnRight:
					if (posX - roundX == 0.5) {
						for (int i = 0; i < 16; i++) {
							if ((int) floor(tabPlayerCoord[2 * i]) == roundX + 1 && (int) floor(tabPlayerCoord[2 * i + 1]) == roundY) {
								nextCaseIsPlayer = true;
							}
						}
						if (!(tab[(roundX + 1) + (roundY * sizeX)] >= brickElement || nextCaseIsPlayer)) {
							posX += bombeMoveSpeed;
						} else {
							if (bombeType == bubbleBombeType) {
								Sound::Instance().playBombeBounceSound();
								direction = kickOnLeft;
							} else {
								direction = -1;
							}
						}
					} else {
						posX += bombeMoveSpeed;
					}
					break;
				case kickOnLeft:
					if (posX - roundX == 0.5) {
						for (int i = 0; i < 16; i++) {
							if ((int) floor(tabPlayerCoord[2 * i]) == roundX - 1 && (int) floor(tabPlayerCoord[2 * i + 1]) == roundY) {
								nextCaseIsPlayer = true;
							}
						}
						if (!(tab[(roundX - 1) + (roundY * sizeX)] >= brickElement || nextCaseIsPlayer)) {
							posX -= bombeMoveSpeed;
						} else {
							if (bombeType == bubbleBombeType) {
								Sound::Instance().playBombeBounceSound();
								direction = kickOnRight;
							} else {
								direction = -1;
							}
						}
					} else {
						posX -= bombeMoveSpeed;
					}
					break;
				case kickOnUp:
					if (posY - roundY == 0.5) {
						for (int i = 0; i < 16; i++) {
							if ((int) floor(tabPlayerCoord[2 * i]) == roundX && (int) floor(tabPlayerCoord[2 * i + 1]) == roundY - 1) {
								nextCaseIsPlayer = true;
							}
						}
						if (!(tab[roundX + (roundY - 1) * sizeX] >= brickElement || nextCaseIsPlayer)) {
							posY -= bombeMoveSpeed;
						} else {
							if (bombeType == bubbleBombeType) {
								Sound::Instance().playBombeBounceSound();
								direction = kickOnDown;
							} else {
								direction = -1;
							}
						}
					} else {
						posY -= bombeMoveSpeed;
					}
					break;
				case kickOnDown:
					if (posY - roundY == 0.5) {
						for (int i = 0; i < 16; i++) {
							if ((int) floor(tabPlayerCoord[2 * i]) == roundX && (int) floor(tabPlayerCoord[2 * i + 1]) == roundY + 1) {
								nextCaseIsPlayer = true;
							}
						}
						if (!(tab[roundX + (roundY + 1) * sizeX] >= brickElement || nextCaseIsPlayer)) {
							posY += bombeMoveSpeed;
						} else {
							if (bombeType == bubbleBombeType) {
								Sound::Instance().playBombeBounceSound();
								direction = kickOnUp;
							} else {
								direction = -1;
							}
						}
					} else {
						posY += bombeMoveSpeed;
					}
					break;
			}
			tab[(int) floor(posX) + (int) floor(posY) * sizeX] = bombeElement;
		}
	}

	//Animation
	if (frameCounter > nbFrame) {
		frameCounter = 0;
		offsetSprite++;
		if (offsetSprite >= nbFrameForAnimation) {
			offsetSprite = 0;
		}
	}
	frameCounter++;
	int offsetSpriteAnimation = 0;
	switch (offsetSprite) {
		case 0:
			offsetSpriteAnimation = 1;
			break;
		case 1:
			offsetSpriteAnimation = 0;
			break;
		case 2:
			offsetSpriteAnimation = 1;
			break;
		case 3:
			offsetSpriteAnimation = 2;
			break;
	}

	switch (bombeType) {
		case normalBombeType:
		case powerBombeType:
		case bubbleBombeType:
			nbTickExplode--;
			break;
		case radioBombeType:
			if (nbTickExplode == 1) {
				nbTickExplode--;
			}
			break;
	}
	SDL_BlitSurface(Sprite::Instance().getBombe(offsetSpriteAnimation,bombeType), NULL, surfaceToDraw, &dstRect);
}
