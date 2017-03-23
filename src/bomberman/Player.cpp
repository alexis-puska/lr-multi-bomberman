#include "Player.h"

#define nbFrame 6

#define sprite_sizeW 30 
#define sprite_sizeH 42
#define blockSizeX 18
#define blockSizeY 16

#define nbFrameIncAnimation 5
#define nbFrameUnderWater 60

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
	normal = 0, onLouis = 1, insideTrolley = 2, carryBombe = 3, throwBombe = 4, burning = 5, victory = 6, victoryOnLouis = 7, crying = 8, dead = 9
};

enum nbFrameAnimationEnum {
	animationNormal = 4, animationOnLouis = 4, animationCarryBombe = 4, animationThrowBombe = 2, animationBurning = 7, animationVictory = 4, animationCrying = 4, animationVictoryOnLouis = 3
};

enum louisTypeEnum {
	blueLouis = 0, yellowLouis = 1, pinkLouis = 2, greenLouis = 3, brownLouis = 4
};

enum spaceShipTypeEnum {
	spaceShipFront = 0, spaceShipRight = 1, spaceShipBottom = 2, spaceShipLeft = 3
};

Player::Player(unsigned short * in_keystate, float posX, float posY, int playerNumber, int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY], Grid * gridParam, int indexPlayerForGame, bool isUnderWater,
		std::vector<Bombe *> * bombes, std::vector<BurnLouis *> * louisBurns) {
	srand (time(NULL));grid = gridParam;
	this->indexPlayerForGame = indexPlayerForGame;
	this->posX = posX;
	this->posY = posY;
	this->playerNumber = playerNumber;
	this->tab = tab;
	this->tabBonus = tabBonus;
	this->characterSpriteIndex = GameConfig::Instance().getPlayerSpriteType(playerNumber);
	this->in_keystate = in_keystate;
	this->nbPlayerConfig = GameConfig::Instance().getNbPlayerInGame();;
	this->color = GameConfig::Instance().getPlayerColor(playerNumber);;
	this->louisType = blueLouis;
	this->isUnderWater = isUnderWater;
	this->bombes = bombes;
	this->louisBurns = louisBurns;
	if(isUnderWater) {
		offsetUnderWater = 1;
	} else {
		offsetUnderWater = 0;
	}
	indexUnderWater = 0;

	GameConfig::Instance().updatePlayerPosition(playerNumber, posX, posY);

	playerState = normal;
	previousPlayerState = normal;
	invincibleTime = 0;
	NbBombeMax = GameConfig::Instance().getBombe();
	NBBombeRemaining = GameConfig::Instance().getBombe();
	bombeType = normalBombeType;
	lineOfBombePower = false;
	ghostModePower = false;
	kickPower = false;
	inSuddenDeathTime = false;
	kickIndex = -1;
	kickDirection = -1;
	trolleyDirection = -1;

	flameStrengh = GameConfig::Instance().getStrenghtBombe();
	playerSpeed = 0.1;
	nbTickMalus = -1;
	playerMalus = noMalus;
	previousBombeNumber = 0;
	previousSpeedValue = 0.0;

	frameCounter = 0;
	offsetSprite = 0;
	previousDirection = down;
	closeBombe = false;
	badBomberWantThrowBombe = false;
	closeBombeValue = 1.5;
}

Player::~Player() {
	in_keystate = NULL;
	tab = NULL;
	tabBonus = NULL;
	grid = NULL;
	bombes = NULL;
	louisBurns = NULL;
}

/*
 * DRAWING PART
 */
void Player::drawNormal(SDL_Surface * surfaceToDraw, bool animate, int offsetUnderWater) {
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
	spriteNumber = Sprite::Instance().playerDrawNormal(characterSpriteIndex, color, previousDirection, offsetSpriteAnimation, offsetUnderWater);
	spriteLouisNumber = -1;
	spriteSpaceshipNumber = -1;
	SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawOnLouis(SDL_Surface * surfaceToDraw, bool animate, int offsetUnderWater) {
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
		spriteNumber = Sprite::Instance().drawOnLouis(characterSpriteIndex, color, previousDirection, offsetUnderWater);
		spriteLouisNumber = Sprite::Instance().drawLouis(louisType, previousDirection, offsetSpriteAnimation, offsetUnderWater);
		SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(Sprite::Instance().getLouisSprite(spriteLouisNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	} else {
		spriteNumber = Sprite::Instance().drawOnLouis(characterSpriteIndex, color, previousDirection, offsetUnderWater);
		spriteLouisNumber = Sprite::Instance().drawLouis(louisType, previousDirection, offsetSpriteAnimation, offsetUnderWater);
		SDL_BlitSurface(Sprite::Instance().getLouisSprite(spriteLouisNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	}
	spriteSpaceshipNumber = -1;
	SDL_FreeSurface(louisMergebuffer);
}

void Player::drawBadBomber(SDL_Surface * surfaceToDraw) {
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	nbFrameForAnimation = animationOnLouis;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
	destTextureRect.y = (posY * blockSizeY) - 20;
	destTextureRect.w = sprite_sizeW;
	destTextureRect.h = sprite_sizeH;
	srcTextureRect.x = 0;
	srcTextureRect.y = 0;
	srcTextureRect.w = sprite_sizeW;
	srcTextureRect.h = sprite_sizeH;
	louisMergebuffer = SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
	if (posX == 0.5) {
		spriteNumber = Sprite::Instance().drawOnLouis(characterSpriteIndex, color, right, 0);
		spriteLouisNumber = -1;
		spriteSpaceshipNumber = spaceShipLeft;
		SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(Sprite::Instance().getSpaceShip(spaceShipLeft), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	} else if (posX == 34.5) {
		spriteNumber = Sprite::Instance().drawOnLouis(characterSpriteIndex, color, left, 0);
		spriteLouisNumber = -1;
		spriteSpaceshipNumber = spaceShipRight;
		SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(Sprite::Instance().getSpaceShip(spaceShipRight), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	} else if (posY == 0.5) {
		spriteNumber = Sprite::Instance().drawOnLouis(characterSpriteIndex, color, down, 0);
		spriteLouisNumber = -1;
		spriteSpaceshipNumber = spaceShipFront;
		SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(Sprite::Instance().getSpaceShip(spaceShipFront), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	} else if (posY == 20.5) {
		spriteNumber = Sprite::Instance().drawOnLouis(characterSpriteIndex, color, up, 0);
		spriteLouisNumber = -1;
		spriteSpaceshipNumber = spaceShipBottom;
		SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(Sprite::Instance().getSpaceShip(spaceShipBottom), &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	}
	SDL_FreeSurface(louisMergebuffer);
}

void Player::drawInsideTrolley(SDL_Surface * surfaceToDraw) {
	nbFrameForAnimation = 0;
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
	spriteLouisNumber = -1;
	spriteSpaceshipNumber = -1;
	spriteNumber = Sprite::Instance().playerDrawInsideTrolley(characterSpriteIndex, color, trolleyDirection);
	SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawGhostBombe(SDL_Surface * surface, float x, float y) {
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_Rect dstRect;
	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = defaultSpriteSize;
	dstRect.h = defaultSpriteSize;
	bombeGhost = SDL_CreateRGBSurface(0, defaultSpriteSize, defaultSpriteSize, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(Sprite::Instance().getBombe(0, bombeType), NULL, bombeGhost, NULL);
	SDL_SetSurfaceAlphaMod(bombeGhost, 60);
	SDL_BlitSurface(bombeGhost, NULL, surface, &dstRect);
	SDL_FreeSurface(bombeGhost);
	BomberNetServer::Instance().sendGhost(x, y);
}

void Player::drawWithBombe(SDL_Surface * surfaceToDraw, bool animate, int offsetUnderWater) {
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
	spriteLouisNumber = -1;
	spriteSpaceshipNumber = -1;
	spriteNumber = Sprite::Instance().drawWithBombe(characterSpriteIndex, color, previousDirection, offsetSpriteAnimation, offsetUnderWater);
	SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawThrowBombe(SDL_Surface * surfaceToDraw, bool animate, int offsetUnderWater) {
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
	spriteLouisNumber = -1;
	spriteSpaceshipNumber = -1;
	spriteNumber = Sprite::Instance().drawThrowBombe(characterSpriteIndex, color, previousDirection, offsetSprite, offsetUnderWater);
	SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawBurning(SDL_Surface * surfaceToDraw, bool animate, int offsetUnderWater) {
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
				if (GameConfig::Instance().isBadBomberMode()) {
					NBBombeRemaining = 1;
					initBadBomberPosition();
				} else {
					GameConfig::Instance().updatePlayerPosition(playerNumber, -1.0, -1.0);
				}
				return;
			}
		}
		frameCounter++;
	} else {
		offsetSprite = 0;
	}
	spriteLouisNumber = -1;
	spriteSpaceshipNumber = -1;
	spriteNumber = Sprite::Instance().drawBurning(characterSpriteIndex, color, offsetSprite, offsetUnderWater);
	SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawVictory(SDL_Surface * surfaceToDraw, bool animate, int offsetUnderWater) {
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
	spriteLouisNumber = -1;
	spriteSpaceshipNumber = -1;
	spriteNumber = Sprite::Instance().drawVictory(characterSpriteIndex, color, offsetSpriteAnimation, offsetUnderWater);
	SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::drawVictoryOnLouis(SDL_Surface * surfaceToDraw, bool animate, int offsetUnderWater) {
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
	spriteLouisNumber = Sprite::Instance().drawVictoryOnLouis(louisType, offsetSpriteAnimation, offsetUnderWater);
	spriteNumber = Sprite::Instance().drawPlayerVictoryOnLouis(characterSpriteIndex, color, offsetUnderWater);
	spriteSpaceshipNumber = -1;
	louisMergebuffer = SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
	SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
	SDL_BlitSurface(Sprite::Instance().getLouisSprite(spriteLouisNumber), &srcTextureRect, louisMergebuffer, &srcTextureRect);
	SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	SDL_FreeSurface(louisMergebuffer);
}

void Player::drawCrying(SDL_Surface * surfaceToDraw, bool animate, int offsetUnderWater) {
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
	spriteLouisNumber = -1;
	spriteSpaceshipNumber = -1;
	spriteNumber = Sprite::Instance().drawCrying(characterSpriteIndex, color, offsetSpriteAnimation, offsetUnderWater);
	SDL_BlitSurface(Sprite::Instance().getPlayerSprite(spriteNumber), &srcTextureRect, surfaceToDraw, &destTextureRect);
}

void Player::doSomething(SDL_Surface * surfaceToDraw) {
	unsigned short keystate = *in_keystate;
	bool animate = false;
	louisBurn = false;
	if (playerState != dead || playerState == victory) {
		if (playerState != burning && playerState != insideTrolley) {
			/*
			 * Direction of a player
			 */
			int roundX = floor(posX);
			int roundY = floor(posY);

			//update coordonate in global memory
			posX = GameConfig::Instance().getPlayerPosX(playerNumber);
			posY = GameConfig::Instance().getPlayerPosY(playerNumber);

			if (invincibleTime == 0) {
				if (!inSuddenDeathTime) {
					if (tab[roundX + roundY * sizeX] == explosionElement) {
						if (playerState == onLouis) {
							invincibleTime = 50;
							playerState = normal;
							louisBurn = true;
							Sound::Instance().playPlayerBurnSound();
							if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
								BomberNetServer::Instance().sendSound(2, -1, true);
							}
						} else {
							playerState = burning;
							Sound::Instance().playPlayerBurnSound();
							if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
								BomberNetServer::Instance().sendSound(2, -1, true);
							}
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
							if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
								BomberNetServer::Instance().sendSound(2, -1, true);
							}
						} else {
							playerState = burning;
							Sound::Instance().playPlayerBurnSound();
							if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
								BomberNetServer::Instance().sendSound(2, -1, true);
							}
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
							if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
								BomberNetServer::Instance().sendSound(9, -1, true);
							}
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
							if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
								BomberNetServer::Instance().sendSound(9, -1, true);
							}
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
								if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
									BomberNetServer::Instance().sendSound(9, -1, true);
								}
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
								if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
									BomberNetServer::Instance().sendSound(9, -1, true);
								}
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
					if (NBBombeRemaining > 0 && tab[(int) floor(posX) + ((int) floor(posY) * sizeX)] != bombeElement) {
						if (!walkOnWall()) {
							Bombe * bombe = addBombe();
							if (bombe != NULL) {
								NBBombeRemaining--;
								bombes->push_back(bombe);
							}
						}
					}

					//putABombe = true;
				}
			}
			if (keystate & keyPadB) {
				//trigger Bombe
				if (bombeType == radioBombeType) {
					for (unsigned int j = 0; j < bombes->size(); j++) {
						if (bombes->at(j)->getPlayer() == indexPlayerForGame) {
							bombes->at(j)->explodeNow();
						}
					}
				}
			}
			if (keystate & keyPadX) {
				if (playerMalus != constipationMalus) {
					if (lineOfBombePower) {
						putLineOfBombe();
					}
				}
			}
			if (keystate & keyPadY) {
				//display menu
			}

			if (playerMalus == diarheeMalus) {
				if (NBBombeRemaining > 0 && tab[(int) floor(posX) + ((int) floor(posY) * sizeX)] != bombeElement) {
					Bombe * bombe = addBombe();
					if (bombe != NULL) {
						NBBombeRemaining--;
						bombes->push_back(bombe);
					}
				}
				//putABombe = true;
			}
		} else if (playerState == insideTrolley) {
			posX = GameConfig::Instance().getPlayerPosX(playerNumber);
			posY = GameConfig::Instance().getPlayerPosY(playerNumber);
		}
	} else if (playerState == dead && GameConfig::Instance().isBadBomberMode() && !inSuddenDeathTime) {
		//BAD BOMBER MOVE
		posX = GameConfig::Instance().getPlayerPosX(playerNumber);
		posY = GameConfig::Instance().getPlayerPosY(playerNumber);
		if (keystate & keyPadA && !badBomberWantThrowBombe) {
			badBomberWantThrowBombe = true;
		} else if (keystate & keyPadA && badBomberWantThrowBombe) {
			if (closeBombe) {
				closeBombeValue -= 0.5;
				if (closeBombeValue <= 1.5) {
					closeBombe = false;
				}
			} else {
				closeBombeValue += 0.5;
				if (closeBombeValue >= 11.5) {
					closeBombe = true;
				}
			}
			float x = 0.0;
			float y = 0.0;

			if (posX >= 1.5 && posX <= 33.5) {
				x = posX - 0.5;
				if (posY > 2) {
					y = posY - closeBombeValue - 1.0;
				} else {
					y = posY + closeBombeValue;
				}
			} else if (posY >= 1.5 && posY <= 19.5) {
				y = posY;
				if (posX > 2) {
					x = posX - closeBombeValue - 1.0;
				} else {
					x = posX + closeBombeValue;
				}
			}
			drawGhostBombe(surfaceToDraw, x * 18.0, y * 16.0);
		} else {
			if (badBomberWantThrowBombe) {
				badBomberWantThrowBombe = false;
				if (NBBombeRemaining > 0 && tab[(int) floor(posX) + ((int) floor(posY) * sizeX)] != bombeElement) {
					Bombe * bombe = addBombeBadBomber();
					if (bombe != NULL) {
						NBBombeRemaining--;
						bombes->push_back(bombe);
					}
				}
			}
			float speed = 0.1;
			if (keystate & keyPadL1) {
				speed += 0.1;
			}
			if (keystate & keyPadL2) {
				speed += 0.1;
			}
			if (keystate & keyPadR1) {
				speed += 0.1;
			}
			if (keystate & keyPadR2) {
				speed += 0.1;
			}
			if (speed >= 0.5) {
				speed = 0.5;
			}
			if (keystate & keyPadLeft) {
				if (floor(posY) == 0 || floor(posY) == 20) {
					posY = floor(posY) + 0.5;
					if (posX > 0.5) {
						posX -= speed;
						if (posX <= 0.5) {
							posX = 0.5;
						}
					}
				}
			}
			if (keystate & keyPadUp) {
				if (floor(posX) == 0 || floor(posX) == 34) {
					posX = floor(posX) + 0.5;
					if (posY > 0.5) {
						posY -= speed;
						if (posY <= 0.5) {
							posY = 0.5;
						}
					}
				}
			}
			if (keystate & keyPadRight) {
				if (floor(posY) == 0 || floor(posY) == 20) {
					posY = floor(posY) + 0.5;
					if (posX < 34.5) {
						posX += speed;
						if (posX >= 34.5) {
							posX = 34.5;
						}
					}
				}
			}
			if (keystate & keyPadDown) {
				if (floor(posX) == 0 || floor(posX) == 34) {
					posX = floor(posX) + 0.5;
					if (posY < 20.5) {
						posY += speed;
						if (posY <= 0.5) {
							posY = 0.5;
						}
					}
				}
			}
			GameConfig::Instance().updatePlayerPosition(playerNumber, posX, posY);
		}
	}

	if (isUnderWater) {
		if (indexUnderWater > nbFrameUnderWater) {
			offsetUnderWater++;
		}
		if (offsetUnderWater > nbFrameWater) {
			offsetUnderWater = 1;
		}
		indexUnderWater++;
	}

	if (louisBurn) {
		louisBurns->push_back(new BurnLouis(posX, posY));
	}

	switch (playerState) {
		case normal:
			drawNormal(surfaceToDraw, animate, offsetUnderWater);
			break;
		case onLouis:
			drawOnLouis(surfaceToDraw, animate, offsetUnderWater);
			break;
		case insideTrolley:
			drawInsideTrolley(surfaceToDraw);
			break;
		case carryBombe:
			drawWithBombe(surfaceToDraw, animate, offsetUnderWater);
			break;
		case throwBombe:
			drawThrowBombe(surfaceToDraw, animate, offsetUnderWater);
			break;
		case burning:
			drawBurning(surfaceToDraw, true, offsetUnderWater);
			break;
		case victory:
			drawVictory(surfaceToDraw, true, offsetUnderWater);
			break;
		case victoryOnLouis:
			drawVictoryOnLouis(surfaceToDraw, true, offsetUnderWater);
			break;
		case crying:
			drawCrying(surfaceToDraw, animate, offsetUnderWater);
			break;
		case dead:
			if (GameConfig::Instance().isBadBomberMode() && !inSuddenDeathTime) {
				drawBadBomber(surfaceToDraw);
			}
			break;
	}

	if (playerState == dead && !GameConfig::Instance().isBadBomberMode()) {
		GameConfig::Instance().updatePlayerPosition(playerNumber, -1.0, -1.0);
	} else {
		//malus treatment
		GameConfig::Instance().updatePlayerPosition(playerNumber, posX, posY);

		if (nbTickMalus > 0) {
			nbTickMalus--;
		} else if (nbTickMalus == 0) {
			releaseMalus();
			nbTickMalus--;
		}
	}

//correct value for AStar
	if (floor(GameConfig::Instance().getPlayerPosY(playerNumber)) > sizeY - 1) {
		GameConfig::Instance().updatePlayerPosY(playerNumber, 0);
	}

	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().sendPlayer(posX, posY, spriteNumber, spriteLouisNumber, spriteSpaceshipNumber);
	}
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
	return new Bombe(strenght, floor(posX) + 0.5, floor(posY) + 0.5, bombeType, indexPlayerForGame, time, tab);
}

Bombe * Player::addBombeBadBomber() {
	int x = 0;
	int y = 0;
	if (posX >= 1.5 && posX <= 33.5) {
		x = floor(posX);
		if (posY > 2) {
			y = floor(posY - closeBombeValue - 1);
		} else {
			y = floor(posY + closeBombeValue);
		}
	} else if (posY >= 1.5 && posY <= 19.5) {
		y = floor(posY);
		if (posX > 2) {
			x = floor(posX - closeBombeValue - 1);
		} else {
			x = floor(posX + closeBombeValue);
		}
	}
	fprintf(stderr, "abb %i %i\n", x, y);
	int time = 100;
	int strenght = flameStrengh;
	int bombeType = normalBombeType;

	if (tab[x + y * sizeX] < brickElement) {
		fprintf(stderr, "ajout d'une bombe ");
		return new Bombe(strenght, (float) x + 0.5, (float) y + 0.5, bombeType, indexPlayerForGame, time, tab);
	} else {
		//putABombe = false;
		return NULL;
	}
}

Bombe * Player::addBombe(int x, int y) {
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

	if (tab[((int) floor(posX) + x) + ((int) floor(posY) + y) * sizeX] < brickElement) {
		return new Bombe(strenght, floor(posX) + 0.5 + x, floor(posY) + 0.5 + y, bombeType, indexPlayerForGame, time, tab);
	} else {
		return NULL;
	}
}

int Player::getPlayerNumber() {
	return playerNumber;
}

int Player::getIndexPlayerForGame() {
	return indexPlayerForGame;
}

bool Player::walkOnWall() {
	if (tab[(int) floor(posX) + (int) floor(posY) * sizeX] < brickElement) {
		return false;
	} else if (!isAlive() && GameConfig::Instance().isBadBomberMode()) {
		return false;
	} else {
		return true;
	}
}

void Player::ABombeExplode() {
	if (!isAlive() && GameConfig::Instance().isBadBomberMode() && NBBombeRemaining >= 1) {
		NBBombeRemaining = 0;
	}
	NBBombeRemaining++;
}

bool Player::isAlive() {
	if (playerState == dead || playerState == burning) {
		return false;
	}
	return true;
}

void Player::winTheGame() {
	if (playerState == onLouis) {
		playerState = victoryOnLouis;
	} else {
		playerState = victory;
	}
}

int Player::foundABonus() {

	int roundX = floor(posX);
	int roundY = floor(posY);

	int bonusIndex = -1;

	if (roundX + roundY * sizeX < sizeX * sizeY) {
		if (tabBonus[roundX + roundY * sizeX] != noBonus && tab[roundX + roundY * sizeX] < brickElement) {
			bonusIndex = tabBonus[roundX + roundY * sizeX];
		} else if (tabBonus[roundX + roundY * sizeX] != noBonus && tab[roundX + roundY * sizeX] == bombeElement) {
			bonusIndex = tabBonus[roundX + roundY * sizeX];
		}
	}
	if (bonusIndex != -1) {
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
				if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
					BomberNetServer::Instance().sendSound(10, -1, true);
				}
				break;
			case shieldBonus:
				invincibleTime = 15 * 50;
				break;
			case lineOfBombeBonus:
				lineOfBombePower = true;
				break;

		}
		return roundX + roundY * sizeX;
	} else {
		return -1;
	}
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
			float toSwitchX = floor(GameConfig::Instance().getPlayerPosX(playerToSwitch)) + 0.5;
			float toSwitchY = floor(GameConfig::Instance().getPlayerPosY(playerToSwitch)) + 0.5;
			GameConfig::Instance().updatePlayerPosX(playerToSwitch, floor(posX) + 0.5);
			GameConfig::Instance().updatePlayerPosY(playerToSwitch, floor(posY) + 0.5);
			GameConfig::Instance().updatePlayerPosX(playerNumber, toSwitchX);
			GameConfig::Instance().updatePlayerPosY(playerNumber, toSwitchY);
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
			if (GameConfig::Instance().getPlayerPosX(i) != -1.0) {
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

int Player::getBombeType() {
	return bombeType;
}

void Player::brainPressButton() {
	for (unsigned int j = 0; j < bombes->size(); j++) {
		if (bombes->at(j)->getPlayer() == indexPlayerForGame) {
			bombes->at(j)->explodeNow();
		}
	}
}

int Player::getBombeRemaining() {
	return NBBombeRemaining;
}

int Player::getPreviousDirection() {
	return previousDirection;
}

bool Player::goInsideTrolley() {
	if (playerState == normal || playerState == onLouis) {
		previousPlayerState = playerState;
		playerState = insideTrolley;
		trolleyDirection = 0;
		return true;
	}
	return false;
}

void Player::goOutsideTrolley() {
	playerState = previousPlayerState;
}

void Player::setTrolleyDirection(int direction) {
	trolleyDirection = direction;
}

void Player::initBadBomberPosition() {
	if (floor(posX) < 7) {
		posX = 0.5;
		posY = (floor(posY)) + 0.5;
	} else if (floor(posX) > 28) {
		posX = 34.5;
		posY = (floor(posY)) + 0.5;
	} else if (floor(posY) < 11) {
		posY = 0.5;
		posX = (floor(posX)) + 0.5;
	} else {
		posY = 20.5;
		posX = (floor(posX)) + 0.5;
	}
	GameConfig::Instance().updatePlayerPosition(playerNumber, posX, posY);
}

void Player::putLineOfBombe() {
	if (!walkOnWall()) {
		if (previousDirection == up) {
			int n = NBBombeRemaining;
			for (int j = 0; j < n; j++) {
				Bombe * bombe = addBombe(0, -j);
				if (bombe == NULL) {
					break;
				} else {
					NBBombeRemaining--;
					bombes->push_back(bombe);
				}
			}
		} else if (previousDirection == right) {
			int n = NBBombeRemaining;
			for (int j = 0; j < n; j++) {
				Bombe * bombe = addBombe(j, 0);
				if (bombe == NULL) {
					break;
				} else {
					NBBombeRemaining--;
					bombes->push_back(bombe);
				}
			}
		} else if (previousDirection == down) {
			int n = NBBombeRemaining;
			for (int j = 0; j < n; j++) {
				Bombe * bombe = addBombe(0, j);
				if (bombe == NULL) {
					break;
				} else {
					NBBombeRemaining--;
					bombes->push_back(bombe);
				}
			}
		} else if (previousDirection == left) {
			int n = NBBombeRemaining;
			for (int j = 0; j < n; j++) {
				Bombe * bombe = addBombe(-j, 0);
				if (bombe == NULL) {
					break;
				} else
					NBBombeRemaining--;
				bombes->push_back(bombe);
			}
		}
	}
}
