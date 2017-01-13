#include "Player.h"

#define nbFrame 6

#define sprite_sizeW 30 
#define sprite_sizeH 42
#define blockSizeX 18
#define blockSizeY 16

#define nbFrameIncAnimation 5

enum playerKey {
	keyPadSelect = 1,
	keyPadL3 = 2,
	keyPadR3 = 4,
	keyPadStart = 8,
	keyPadUp = 16,
	keyPadRight = 32,
	keyPadDown = 64,
	keyPadLeft = 128,
	keyPadL2 = 256,
	keyPadR2 = 512,
	keyPadL1 = 1024,
	keyPadR1 = 2048,
	keyPadX = 4096,
	keyPadA = 8192,
	keyPadB = 16384,
	keyPadY = 32768
};

enum playerSprite {
	bomberman = 0, cossak = 1, barbar = 2, chan = 3, kid = 4, pretty = 5, punk = 6, mexican = 7
};

enum playerStateEnum {
	normal = 0, onLouis = 1, carryBombe = 2, throwBombe = 3, burning = 4, victory = 5, crying = 6, dead = 7
};

enum nbFrameAnimationEnum {
	animationNormal = 4, animationOnLouis = 4, animationCarryBombe = 4, animationThrowBombe = 2, animationBurning = 7, animationVictory = 4, animationCrying = 4
};

enum louisTypeEnum {
	blueLouis = 0, yellowLouis = 1, pinkLouis = 2, greenLouis = 3, brownLouis = 4
};

Player::Player(unsigned short * in_keystate, bool isACpuPlayer, int indexSprite, float posX, float posY, int playerNumber, int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY], Grid * gridParam,
		float * tabPlayerCoord, int nbPlayerConfig, int indexPlayerForGame, int color) {
	srand (time(NULL));grid = gridParam;
	this->indexPlayerForGame = indexPlayerForGame;
	this->posX = posX;
	this->posY = posY;
	this->playerNumber = playerNumber;
	this->cpu = isACpuPlayer;
	this->tab = tab;
	this->tabBonus = tabBonus;
	this->characterSpriteIndex = indexSprite;
	this->in_keystate = in_keystate;
	this->tabPlayerCoord = tabPlayerCoord;
	this->nbPlayerConfig = nbPlayerConfig;
	this->color = color;
	this->louisType = blueLouis;

	tabPlayerCoord[playerNumber * 2] = posX;
	tabPlayerCoord[playerNumber * 2 + 1 ] = posY;
	playerState = normal;
	invincibleTime = 0;
	NbBombeMax = 2;
	NBBombeRemaining = 2;
	bombeType = normalBombeType;
	putABombe = false;
	ghostModePower = false;
	triggerBombe = false;
	kickPower = false;
	inSuddenDeathTime = false;
	kickIndex = -1;
	kickDirection = -1;

	flameStrengh = 2;
	playerSpeed = 0.1;
	nbTickMalus = -1;
	playerMalus = noMalus;
	previousBombeNumber = 0;
	previousSpeedValue = 0.0;

	frameCounter = 0;
	offsetSprite = 0;
	previousDirection = down;
}

Player::~Player() {
	delete in_keystate;
	tab = NULL;
	tabBonus = NULL;
	tabPlayerCoord = NULL;
	grid = NULL;
}

/*
 * DRAWING PART
 */
void Player::drawNormal(SDL_Surface * surfaceToDraw, bool animate) {
	nbFrameForAnimation = animationNormal;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
	destTextureRect.y = (posY * blockSizeY) - (sprite_sizeH - 7);
	destTextureRect.w = sprite_sizeW;
	destTextureRect.h = sprite_sizeH;
	srcTextureRect.x = 0;
	srcTextureRect.y = 0;
	srcTextureRect.w = sprite_sizeW;
	srcTextureRect.h = sprite_sizeH;
	if (animate) {
		if (frameCounter > nbFrame) {
			frameCounter = 0;
			offsetSprite++;
			if (offsetSprite >= nbFrameForAnimation) {
				offsetSprite = 0;
			}
		}
		frameCounter++;
	} else {
		offsetSprite = 0;
	}
	int offsetSpriteAnimation = 0;
	switch (offsetSprite) {
		case 0:
			offsetSpriteAnimation = 0;
			break;
		case 1:
			offsetSpriteAnimation = 1;
			break;
		case 2:
			offsetSpriteAnimation = 0;
			break;
		case 3:
			offsetSpriteAnimation = 2;
			break;
	}
	SDL_BlitSurface(Sprite::Instance().playerDrawNormal(characterSpriteIndex, color, previousDirection, offsetSpriteAnimation), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawOnLouis(SDL_Surface * surfaceToDraw, bool animate) {
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	nbFrameForAnimation = animationOnLouis;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
	destTextureRect.y = (posY * blockSizeY) - (sprite_sizeH - 7);
	destTextureRect.w = sprite_sizeW;
	destTextureRect.h = sprite_sizeH;
	srcTextureRect.x = 0;
	srcTextureRect.y = 0;
	srcTextureRect.w = sprite_sizeW;
	srcTextureRect.h = sprite_sizeH;
	if (animate) {
		if (frameCounter > nbFrame) {
			frameCounter = 0;
			offsetSprite++;
			if (offsetSprite >= nbFrameForAnimation) {
				offsetSprite = 0;
			}
		}
		frameCounter++;
	} else {
		offsetSprite = 0;
	}
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
	louisMergebuffer = SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
	if (previousDirection == down) {
		SDL_BlitSurface(Sprite::Instance().drawOnLouis(characterSpriteIndex, color, previousDirection), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(Sprite::Instance().drawLouis(louisType, previousDirection, offsetSpriteAnimation), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	} else {
		SDL_BlitSurface(Sprite::Instance().drawLouis(louisType, previousDirection, offsetSpriteAnimation), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(Sprite::Instance().drawOnLouis(characterSpriteIndex, color, previousDirection), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	}
	SDL_FreeSurface(louisMergebuffer);
}

void Player::drawWithBombe(SDL_Surface * surfaceToDraw, bool animate) {
	nbFrameForAnimation = animationCarryBombe;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
	destTextureRect.y = (posY * blockSizeY) - (sprite_sizeH - 7);
	destTextureRect.w = sprite_sizeW;
	destTextureRect.h = sprite_sizeH;
	srcTextureRect.x = 0;
	srcTextureRect.y = 0;
	srcTextureRect.w = sprite_sizeW;
	srcTextureRect.h = sprite_sizeH;
	if (animate) {
		if (frameCounter > nbFrame) {
			frameCounter = 0;
			offsetSprite++;
			if (offsetSprite >= nbFrameForAnimation) {
				offsetSprite = 0;
			}
		}
		frameCounter++;
	} else {
		offsetSprite = 0;
	}
	int offsetSpriteAnimation = 0;
	switch (offsetSprite) {
		case 0:
			offsetSpriteAnimation = 0;
			break;
		case 1:
			offsetSpriteAnimation = 1;
			break;
		case 2:
			offsetSpriteAnimation = 0;
			break;
		case 3:
			offsetSpriteAnimation = 2;
			break;
	}
	SDL_BlitSurface(Sprite::Instance().drawWithBombe(characterSpriteIndex, color, previousDirection, offsetSpriteAnimation), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawThrowBombe(SDL_Surface * surfaceToDraw, bool animate) {
	nbFrameForAnimation = animationThrowBombe;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
	destTextureRect.y = (posY * blockSizeY) - (sprite_sizeH - 7);
	destTextureRect.w = sprite_sizeW;
	destTextureRect.h = sprite_sizeH;
	srcTextureRect.x = 0;
	srcTextureRect.y = 0;
	srcTextureRect.w = sprite_sizeW;
	srcTextureRect.h = sprite_sizeH;
	if (animate) {
		if (frameCounter > nbFrame) {
			frameCounter = 0;
			offsetSprite++;
			if (offsetSprite >= nbFrameForAnimation) {
				offsetSprite = 0;
			}
		}
		frameCounter++;
	} else {
		offsetSprite = 0;
	}
	SDL_BlitSurface(Sprite::Instance().drawThrowBombe(characterSpriteIndex, color, previousDirection, offsetSprite), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawBurning(SDL_Surface * surfaceToDraw, bool animate) {
	nbFrameForAnimation = animationBurning;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
	destTextureRect.y = (posY * blockSizeY) - (sprite_sizeH - 7);
	destTextureRect.w = sprite_sizeW;
	destTextureRect.h = sprite_sizeH;
	srcTextureRect.x = 0;
	srcTextureRect.y = 0;
	srcTextureRect.w = sprite_sizeW;
	srcTextureRect.h = sprite_sizeH;
	if (animate) {
		if (frameCounter > nbFrame) {
			frameCounter = 0;
			offsetSprite++;
			if (offsetSprite >= nbFrameForAnimation) {
				offsetSprite = 0;
				playerState = dead;
				tabPlayerCoord[playerNumber * 2] = -1.0;
				tabPlayerCoord[playerNumber * 2 + 1] = -1.0;
				return;
			}
		}
		frameCounter++;
	} else {
		offsetSprite = 0;
	}
	SDL_BlitSurface(Sprite::Instance().drawBurning(characterSpriteIndex, color, offsetSprite), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawVictory(SDL_Surface * surfaceToDraw, bool animate) {
	nbFrameForAnimation = animationVictory;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
	destTextureRect.y = (posY * blockSizeY) - (sprite_sizeH - 7);
	destTextureRect.w = sprite_sizeW;
	destTextureRect.h = sprite_sizeH;
	srcTextureRect.x = 0;
	srcTextureRect.y = 0;
	srcTextureRect.w = sprite_sizeW;
	srcTextureRect.h = sprite_sizeH;
	if (animate) {
		if (frameCounter > nbFrame) {
			frameCounter = 0;
			offsetSprite++;
			if (offsetSprite >= nbFrameForAnimation) {
				offsetSprite = 0;
			}
		}
		frameCounter++;
	} else {
		offsetSprite = 0;
	}
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
	SDL_BlitSurface(Sprite::Instance().drawVictory(characterSpriteIndex, color, offsetSpriteAnimation), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawCrying(SDL_Surface * surfaceToDraw, bool animate) {
	nbFrameForAnimation = animationCrying;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
	destTextureRect.y = (posY * blockSizeY) - (sprite_sizeH - 7);
	destTextureRect.w = sprite_sizeW;
	destTextureRect.h = sprite_sizeH;
	srcTextureRect.x = 0;
	srcTextureRect.y = 0;
	srcTextureRect.w = sprite_sizeW;
	srcTextureRect.h = sprite_sizeH;

	if (animate) {
		if (frameCounter > nbFrame) {
			frameCounter = 0;
			offsetSprite++;
			if (offsetSprite >= nbFrameForAnimation) {
				offsetSprite = 0;
			}
		}
		frameCounter++;
	} else {
		offsetSprite = 0;
	}
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
	SDL_BlitSurface(Sprite::Instance().drawCrying(characterSpriteIndex, color, offsetSpriteAnimation), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::doSomething(SDL_Surface * surfaceToDraw) {
	unsigned short keystate = *in_keystate;
	bool animate = false;
	louisBurn = false;
	if (playerState != dead || playerState == victory) {
		if (playerState != burning) {
			/*
			 * Direction of a player
			 */
			int roundX = floor(posX);
			int roundY = floor(posY);

			//update coordonate in global memory		
			posX = tabPlayerCoord[playerNumber * 2];
			posY = tabPlayerCoord[playerNumber * 2 + 1];

			if (roundX + roundY * sizeX < sizeX * sizeY) {
				if (tabBonus[roundX + roundY * sizeX] != noBonus && tab[roundX + roundY * sizeX] < brickElement) {
					foundABonus(tabBonus[roundX + roundY * sizeX]);
				}
			}

			if (invincibleTime == 0) {
				if (!inSuddenDeathTime) {
					if (tab[roundX + roundY * sizeX] == explosionElement) {
						if (playerState == onLouis) {
							invincibleTime = 50;
							playerState = normal;
							louisBurn = true;
							Sound::Instance().playPlayerBurnSound();
						} else {
							playerState = burning;
							Sound::Instance().playPlayerBurnSound();
							animate = true;
						}
					}
				} else {
					if (tab[roundX + roundY * sizeX] == explosionElement || tab[roundX + roundY * sizeX] == suddenDeathElement) {
						if (playerState == onLouis) {
							invincibleTime = 50;
							playerState = normal;
							louisBurn = true;
							Sound::Instance().playPlayerBurnSound();
						} else {
							playerState = burning;
							Sound::Instance().playPlayerBurnSound();
							animate = true;
						}
					}
				}
			} else {
				invincibleTime--;
			}

			if (roundY > sizeY - 1) {
				posY = 0.0;
				roundY = 0;
			}

			if (roundY < 0) {
				posY = sizeY - 0.1;
				roundY = sizeY - 1;
			}

			float margeInf;
			float margeSup;

			if (playerSpeed > 0.1) {
				margeInf = 0.51 - (playerSpeed / 2);
				margeSup = 0.49 + (playerSpeed / 2);
			} else {
				margeInf = 0.49 - (playerSpeed / 2);
				margeSup = 0.51 + (playerSpeed / 2);
			}

			if (posX - (float) roundX >= margeInf && posX - (float) roundX <= margeSup) {
				posX = (float) floor(posX) + 0.5;
			}

			if (posY - (float) roundY >= margeInf && posY - (float) roundY <= margeSup) {
				posY = (float) floor(posY) + 0.5;
			}

			int canPassBlock = 0;
			if (ghostModePower == true) {
				canPassBlock = brickElement;
			} else {
				canPassBlock = explosionElement;
			}

			//left
			if (playerMalus != invDirectionMalus ? keystate & keyPadLeft : keystate & keyPadRight) {
				if (posX - roundX == 0.5) {
					if (!(tab[(roundX - 1) + (roundY * sizeX)] > canPassBlock)) {
						posX = (posX - playerSpeed);
					} else if (kickPower) {
						if (tab[(roundX - 1) + (roundY * sizeX)] == bombeElement) {
							kickIndex = (roundX - 1) + (roundY * sizeX);
							Sound::Instance().playPlayerKickSound();
							kickDirection = kickOnLeft;
						}
					}
				} else {
					if (posY - roundY > 0.5) {
						posY = (posY - playerSpeed);
					} else if (posY - roundY < 0.5) {
						posY = (posY + playerSpeed);
					} else {
						posX = (posX - playerSpeed);
					}
				}
				previousDirection = left;
				animate = true;
			}

			//right
			if (playerMalus != invDirectionMalus ? keystate & keyPadRight : keystate & keyPadLeft) {
				if (posX - roundX == 0.5) {
					if (!(tab[(roundX + 1) + (roundY * sizeX)] > canPassBlock)) {
						posX = (posX + playerSpeed);
					} else if (kickPower) {
						if (tab[(roundX + 1) + (roundY * sizeX)] == bombeElement) {
							kickIndex = (roundX + 1) + (roundY * sizeX);
							Sound::Instance().playPlayerKickSound();
							kickDirection = kickOnRight;
						}
					}
				} else {
					if (posY - roundY > 0.5) {
						posY = (posY - playerSpeed);
					} else if (posY - roundY < 0.5) {
						posY = (posY + playerSpeed);
					} else {
						posX = (posX + playerSpeed);
					}
				}
				previousDirection = right;
				animate = true;
			}

			//down
			if (playerMalus != invDirectionMalus ? keystate & keyPadDown : keystate & keyPadUp) {
				if (posY - roundY == 0.5) {
					if (roundY >= (sizeY - 1)) {
						posY = (posY + playerSpeed);
					} else {
						if (!(tab[roundX + ((roundY + 1) * sizeX)] > canPassBlock)) {
							posY = (posY + playerSpeed);
						} else if (kickPower) {
							if (tab[roundX + ((roundY + 1) * sizeX)] == bombeElement) {
								kickIndex = roundX + ((roundY + 1) * sizeX);
								Sound::Instance().playPlayerKickSound();
								kickDirection = kickOnDown;
							}
						}
					}
				} else {
					if (posX - roundX > 0.5) {
						posX = (posX - playerSpeed);
					} else if (posX - roundX < 0.5) {
						posX = (posX + playerSpeed);
					} else {
						posY = (posY + playerSpeed);
					}
				}
				previousDirection = down;
				animate = true;
			}

			//up
			if (playerMalus != invDirectionMalus ? keystate & keyPadUp : keystate & keyPadDown) {
				if (posY - roundY == 0.5) {
					if (roundY == 0) {
						posY = (posY - playerSpeed);
					} else {
						if (!(tab[roundX + ((roundY - 1) * sizeX)] > canPassBlock)) {
							posY = (posY - playerSpeed);
						} else if (kickPower) {
							if (tab[roundX + ((roundY - 1) * sizeX)] == bombeElement) {
								kickIndex = roundX + ((roundY - 1) * sizeX);
								Sound::Instance().playPlayerKickSound();
								kickDirection = kickOnUp;
							}
						}
					}
				} else {
					if (posX - roundX > 0.5) {
						posX = (posX - playerSpeed);
					} else if (posX - roundX < 0.5) {
						posX = (posX + playerSpeed);
					} else {
						posY = (posY - playerSpeed);
					}
				}
				previousDirection = up;
				animate = true;
			}

			/*
			 * ACTION OF A PLAYER
			 */

			if (keystate & keyPadStart) {
				//display menu	
			}
			if (keystate & keyPadA) {
				if (playerMalus != constipationMalus) {
					putABombe = true;
				}
			}
			if (keystate & keyPadB) {
				if (bombeType == radioBombeType) {
					triggerBombe = true;
				}
			}
			if (keystate & keyPadX) {
				//display menu	
			}
			if (keystate & keyPadY) {
				//display menu	
			}

			if (playerMalus == diarheeMalus) {
				putABombe = true;
			}
		}

	}
	switch (playerState) {
		case normal:
			drawNormal(surfaceToDraw, animate);
			break;
		case onLouis:
			drawOnLouis(surfaceToDraw, animate);
			break;
		case carryBombe:
			drawWithBombe(surfaceToDraw, animate);
			break;
		case throwBombe:
			drawThrowBombe(surfaceToDraw, animate);
			break;
		case burning:
			drawBurning(surfaceToDraw, true);
			break;
		case victory:
			drawVictory(surfaceToDraw, true);
			break;
		case crying:
			drawCrying(surfaceToDraw, animate);
			break;
	}

	if (playerState == dead) {
		tabPlayerCoord[playerNumber * 2] = -1.0;
		tabPlayerCoord[playerNumber * 2 + 1] = -1.0;
	} else {
		//malus treatment
		tabPlayerCoord[playerNumber * 2] = posX;
		tabPlayerCoord[playerNumber * 2 + 1] = posY;
		if (nbTickMalus > 0) {
			nbTickMalus--;
		} else if (nbTickMalus == 0) {
			releaseMalus();
			nbTickMalus--;
		}
	}

	//correct value for AStar
	if (floor(tabPlayerCoord[playerNumber * 2 + 1]) > sizeY - 1) {
		tabPlayerCoord[playerNumber * 2 + 1] = 0;
	}
}

BurnLouis* Player::louisBurnAnimation(){
	return new BurnLouis((int) floor(posX), (int) floor(posY));
}

bool Player::isLouisBurn(){
	return louisBurn;
}

Bombe * Player::addBombe() {
	int time = 100;
	int strenght = flameStrengh;
	switch (bombeType) {
		case radioBombeType:
			time = -1;
			break;
		case powerBombeType:
		case normalBombeType:
		case bubbleBombeType:
			break;
	}
	switch (playerMalus) {
		case speedBombeMalus:
			time = 50;
			break;
		case slowBombeMalus:
			time = 300;
			break;
		case miniBombeMalus:
			strenght = 1;
			break;
	}
	tab[(int) floor(posX) + ((int) floor(posY) * sizeX)] = bombeElement;
	return new Bombe(strenght, floor(posX) + 0.5, floor(posY) + 0.5, bombeType, indexPlayerForGame, time, tab, tabPlayerCoord);
}

int Player::getPlayerNumber() {
	return playerNumber;
}

int Player::getIndexPlayerForGame() {
	return indexPlayerForGame;
}

bool Player::triggerPowerBombe() {
	return triggerBombe;
}

void Player::releaseTrigger() {
	triggerBombe = false;
}

bool Player::wantPutBombe() {
	if (isAlive()) {
		if (NBBombeRemaining > 0 && tab[(int) floor(posX) + ((int) floor(posY) * sizeX)] != bombeElement) {
			return putABombe;
		} else {
			putABombe = false;
			return false;
		}
	}
	return false;
}

bool Player::walkOnWall() {
	if (tab[(int) floor(posX) + (int) floor(posY) * sizeX] < brickElement) {
		return false;
	} else {
		putABombe = false;
		return true;
	}
}

void Player::ABombeExplode() {
	triggerBombe = false;
	NBBombeRemaining++;
}

void Player::ABombeIsSet() {
	NBBombeRemaining--;
	putABombe = false;
}

bool Player::isAlive() {
	if (playerState == dead || playerState == burning) {
		return false;
	}
	return true;
}

void Player::winTheGame() {
	playerState = victory;
}

void Player::foundABonus(int bonusIndex) {
	int roundX = floor(posX);
	int roundY = floor(posY);
	switch (bonusIndex) {
		case deathBonus:
			getAMalusBonus();
			break;
		case rollerBonus:
			if (playerSpeed < 0.15) {
				playerSpeed += 0.02;
			}
			break;
		case fireBonus:
			flameStrengh++;
			break;
		case fireMaxBonus:
			flameStrengh += 10;
			break;
		case bombeBonus:
			NbBombeMax++;
			NBBombeRemaining++;
			break;
		case kickBonus:
			kickPower = true;
			break;
		case gloveBonus:
			break;
		case radioBombeBonus:
			bombeType = radioBombeType;
			break;
		case bubbleBonus:
			bombeType = bubbleBombeType;
			break;
		case powerBombeBonus:
			bombeType = powerBombeType;
			break;
		case getaBonus:
			if (playerSpeed > 0.05) {
				playerSpeed -= 0.02;
			}
			break;
		case ghostBonus:
			ghostModePower = true;
			break;
		case eggBonus:
			louisType = rand() % nbTypeLouis;
			playerState = onLouis;
			Sound::Instance().playLouisSound();
			break;
	}
	grid->burnBonus(roundX, roundY);
}

void Player::releaseMalus() {
	switch (playerMalus) {
		case constipationMalus:
			NbBombeMax = previousBombeNumber;
			NBBombeRemaining = previousBombeNumber;
			break;
		case slowDownMalus:
		case speedUpMalus:
			playerSpeed = previousSpeedValue;
			break;
	}
	playerMalus = noMalus;
}

void Player::getAMalusBonus() {
	//reset in first Time the previous malus if exist
	if (playerMalus != noMalus) {
		releaseMalus();
	}

	nbTickMalus = 15 * 50;

	/* generate secret number between 0 and 6: */
	int malus = rand() % 9;
	switch (malus) {
		case diarheeMalus:
		case speedBombeMalus:
		case slowBombeMalus:
			nbTickMalus = 15 * 50;
			break;
		case constipationMalus:
			previousBombeNumber = NbBombeMax;
			NBBombeRemaining -= NbBombeMax;
			nbTickMalus = 15 * 50;
			break;
		case slowDownMalus:
			previousSpeedValue = playerSpeed;
			playerSpeed = 0.04;
			nbTickMalus = 15 * 50;
			break;
		case speedUpMalus:
			previousSpeedValue = playerSpeed;
			playerSpeed = 0.16;
			nbTickMalus = 15 * 50;
			break;
		case switchPlayerMalus:
			int playerToSwitch = findIndexPlayer();
			float toSwitchX = floor(tabPlayerCoord[playerToSwitch * 2]) + 0.5;
			float toSwitchY = floor(tabPlayerCoord[playerToSwitch * 2 + 1]) + 0.5;
			tabPlayerCoord[playerToSwitch * 2] = floor(tabPlayerCoord[playerNumber * 2]) + 0.5;
			tabPlayerCoord[playerToSwitch * 2 + 1] = floor(tabPlayerCoord[playerNumber * 2 + 1]) + 0.5;
			tabPlayerCoord[playerNumber * 2] = toSwitchX;
			tabPlayerCoord[playerNumber * 2 + 1] = toSwitchY;
			posX = toSwitchX;
			posY = toSwitchY;
			break;
	}
	playerMalus = malus;
}

int Player::findIndexPlayer() {
	std::vector<int> index;
	for (int i = 0; i < 16; i++) {
		if (i != playerNumber) {
			if (tabPlayerCoord[i * 2] != -1.0) {
				index.push_back(i);
			}
		}
	}
	int idx = rand() % index.size();
	return index[idx];
}

bool Player::hasKickPower() {
	return kickPower;
}

int Player::isKickingBombe() {
	return kickIndex;
}

int Player::getKickDirection() {
	return kickDirection;
}

void Player::releaseKick() {
	kickDirection = kickNone;
	kickIndex = -1;
}

void Player::itSuddenDeathTime() {
	inSuddenDeathTime = true;
}
