#include "Player.h"


#define nbFrame 6

#define sprite_sizeW 30 
#define sprite_sizeH 42
#define blockSizeX 18
#define blockSizeY 16

#define nbFrameIncAnimation 5

const static char *BombermanSprite = "./resources/sprite/characters/AllBomberman.png";
const static char *BombermanSpriteCossak = "./resources/sprite/characters/AllBombermanCossak.png";
const static char *BombermanSpriteBarbar = "./resources/sprite/characters/AllBombermanBarbar.png";
const static char *BombermanSpriteChan = "./resources/sprite/characters/AllBombermanChan.png";
const static char *BombermanSpriteKid = "./resources/sprite/characters/AllBombermanKid.png";
const static char *BombermanSpritePretty = "./resources/sprite/characters/AllBombermanPretty.png";
const static char *BombermanSpritePunk = "./resources/sprite/characters/AllBombermanPunk.png";
const static char *BombermanSpriteMexican = "./resources/sprite/characters/AllBombermanMexican.png";
const static char *BombermanSpriteLouis = "./resources/sprite/characters/AllLouis.png";

enum playerKey{
	keyPadSelect	= 1,
	keyPadL3		= 2,
	keyPadR3		= 4,
	keyPadStart		= 8,
	keyPadUp		= 16,
	keyPadRight		= 32,
	keyPadDown		= 64,
	keyPadLeft		= 128,
	keyPadL2		= 256,
	keyPadR2		= 512,
	keyPadL1		= 1024,
	keyPadR1		= 2048,
	keyPadX			= 4096,
	keyPadA			= 8192,
	keyPadB			= 16384,
	keyPadY			= 32768	
};

enum playerMove{
	none					=-1,
	down					= 0,
	up						= 1,
	left					= 2,
	right					= 3
};

enum playerSprite{
	bomberman				= 0,	 
	cossak					= 1,
	barbar					= 2,
	chan					= 3,
	kid						= 4,
	pretty					= 5,
	punk					= 6,
	mexican					= 7	
};

enum playerStateEnum{
	normal					= 0,
	onLouis					= 1,
	carryBombe				= 2,
	throwBombe				= 3,
	burning					= 4,
	louisBurning			= 5,
	victory					= 6,
	crying					= 7,
	dead					= 8
};

enum nbFrameAnimationEnum{
	animationNormal			= 4,
	animationOnLouis		= 4,
	animationCarryBombe		= 4,
	animationThrowBombe		= 2,
	animationBurning		= 6,
	animationLouisBurning	= 7,
	animationVictory		= 4,
	animationCrying			= 4
};

enum louisTypeEnum{
	blueLouis				= 0,
	yellowLouis				= 1,
	pinkLouis				= 2,
	greenLouis				= 3,
	brownLouis				= 4
};



Player::Player(unsigned short * in_keystateLibretro, bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY, int playerNumberLibretro, int table[sizeX * sizeY], int tableBonus[sizeX * sizeY], SDL_Surface ** bombeSpriteGame, Grid * gridParam, float * tabPlayerCoordGame, int nbPlayerConfigGame){
	srand(time(NULL));
	grid = gridParam;
	playerState = normal;
	invincibleTime = 0;
	NbBombeMax = 2;
	NBBombeRemaining = 2;
	bombeType = normalBombeType;
	putABombe = false;
	ghostModePower = false;
	triggerBombe = false;
	kickPower = false;
	kickIndex = -1;
	kickDirection = -1;
	
	flameStrengh = 2;
	playerSpeed = 0.1;
	nbTickMalus = -1;
	playerMalus = noMalus;
	previousBombeNumber = 0;
	previousSpeedValue = 0.0;
	tabPlayerCoord = tabPlayerCoordGame;
	nbPlayerConfig = nbPlayerConfigGame;
	
	frameCounter = 0;
	offsetSprite = 0;
	previousDirection = down;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	int i;
	int j;
	
	SDL_Surface * tempSurface;
	switch(indexSprite){
		case bomberman:
			tempSurface = IMG_Load(BombermanSprite);
			break;
		case cossak:
			tempSurface = IMG_Load(BombermanSpriteCossak);
			break;
		case barbar:
			tempSurface = IMG_Load(BombermanSpriteBarbar);
			break;
		case chan:
			tempSurface = IMG_Load(BombermanSpriteChan);
			break;
		case kid:
			tempSurface = IMG_Load(BombermanSpriteKid);
			break;
		case pretty:
			tempSurface = IMG_Load(BombermanSpritePretty);
			break;
		case punk:
			tempSurface = IMG_Load(BombermanSpritePunk);
			break;
		case mexican:
			tempSurface = IMG_Load(BombermanSpriteMexican);
			break;		
		default: 
			tempSurface = IMG_Load(BombermanSpriteMexican);
	}
	
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = sprite_sizeW;
	destTextureRect.h = sprite_sizeH;
	
	playerSpriteWalk = new SDL_Surface * [12];
	playerSpriteWalkBomb= new SDL_Surface * [12];
	playerSpriteThrowBomb= new SDL_Surface * [8];
	playerSpriteOnLouis= new SDL_Surface * [4];
	playerSpriteVictory= new SDL_Surface * [4];
	playerSpriteAngry= new SDL_Surface * [4];
	playerSpriteBurn= new SDL_Surface * [7];
	louisMergebuffer = new SDL_Surface;
	
	louisSprite = new SDL_Surface * [12];
	louisSpriteBurn = new SDL_Surface * [4];
	
	
	//playerSpriteWalk
	for(i = 0 ; i < 3; i++){
		for(j = 0; j < 4; j++){
			srcTextureRect.x = i * sprite_sizeW;
			srcTextureRect.y = j * sprite_sizeH;
			srcTextureRect.w = sprite_sizeW;
			srcTextureRect.h = sprite_sizeH;
			playerSpriteWalk[i + (j * 3)] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(tempSurface, &srcTextureRect, playerSpriteWalk[i + (j * 3)], &destTextureRect);
		}
	}
	
	//playerSpriteWalkBomb
	for(i = 0 ; i < 3; i++){
		for(j = 0; j < 4; j++){
			srcTextureRect.x = (i+3) * sprite_sizeW;
			srcTextureRect.y = j * sprite_sizeH;
			srcTextureRect.w = sprite_sizeW;
			srcTextureRect.h = sprite_sizeH;
			playerSpriteWalkBomb[i + (j * 3)] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(tempSurface, &srcTextureRect, playerSpriteWalkBomb[i + (j * 3)], &destTextureRect);
		}
	}
	
	//playerSpriteThrowBomb
	for(i = 0 ; i < 2; i++){
		for(j = 0; j < 4; j++){
			srcTextureRect.x = (i+6) * sprite_sizeW;
			srcTextureRect.y = j * sprite_sizeH;
			srcTextureRect.w = sprite_sizeW;
			srcTextureRect.h = sprite_sizeH;
			playerSpriteThrowBomb[i + (j * 2)] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(tempSurface, &srcTextureRect, playerSpriteThrowBomb[i + (j * 2)], &destTextureRect);
		}
	}
	
	//playerSpriteOnLouis
	for(j = 0 ; j < 4; j++){
		srcTextureRect.x = 8 * sprite_sizeW;
		srcTextureRect.y = j * sprite_sizeH;
		srcTextureRect.w = sprite_sizeW;
		srcTextureRect.h = sprite_sizeH;
		playerSpriteOnLouis[j] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(tempSurface, &srcTextureRect, playerSpriteOnLouis[j], &destTextureRect);
	}
	
	//playerSpriteVictory
	for(i = 0 ; i < 3; i++){
		srcTextureRect.x = i * sprite_sizeW;
		srcTextureRect.y = 4 * sprite_sizeH;
		srcTextureRect.w = sprite_sizeW;
		srcTextureRect.h = sprite_sizeH;
		playerSpriteVictory[i] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(tempSurface, &srcTextureRect, playerSpriteVictory[i], &destTextureRect);
	}
	
	//playerSpriteAngry
	for(i = 0 ; i < 3; i++){
		srcTextureRect.x = i * sprite_sizeW;
		srcTextureRect.y = 5 * sprite_sizeH;
		srcTextureRect.w = sprite_sizeW;
		srcTextureRect.h = sprite_sizeH;
		playerSpriteAngry[i] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(tempSurface, &srcTextureRect, playerSpriteAngry[i], &destTextureRect);
	}
	
	//playerSpriteBurn
	for(i = 0 ; i < 7; i++){
		srcTextureRect.x = i * sprite_sizeW;
		srcTextureRect.y = 6 * sprite_sizeH;
		srcTextureRect.w = sprite_sizeW;
		srcTextureRect.h = sprite_sizeH;
		playerSpriteBurn[i] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(tempSurface, &srcTextureRect, playerSpriteBurn[i], &destTextureRect);
	}
	SDL_FreeSurface(tempSurface);
	
	
	//LOUIS PART
	tempSurface = IMG_Load(BombermanSpriteLouis);
	
	for(i = 0 ; i < 3; i++){
		for(j = 0; j < 4; j++){
			srcTextureRect.x = i * sprite_sizeW;
			srcTextureRect.y = j * sprite_sizeH;
			srcTextureRect.w = sprite_sizeW;
			srcTextureRect.h = sprite_sizeH;
			louisSprite[i + (j * 3)] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(tempSurface, &srcTextureRect, louisSprite[i + (j * 3)], &destTextureRect);
		}
	}
	
	for(i = 0 ; i < 4; i++){
		srcTextureRect.x = i * sprite_sizeW;
		srcTextureRect.y = 4 * sprite_sizeH;
		srcTextureRect.w = sprite_sizeW;
		srcTextureRect.h = sprite_sizeH;
		louisSpriteBurn[i] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(tempSurface, &srcTextureRect, louisSpriteBurn[i], &destTextureRect);
	}
	
	posX = startPositionX;
	posY = startPositionY;
	
	playerNumber = playerNumberLibretro;
	tabPlayerCoord[playerNumber * 2] = posX;
	tabPlayerCoord[playerNumber * 2 + 1 ] = posY;
	cpu = isACpuPlayer;
	tab = table;
	tabBonus = tableBonus;
	characterSpriteIndex = indexSprite;
	in_keystate = in_keystateLibretro;
	bombeSprite = bombeSpriteGame;
	SDL_FreeSurface(tempSurface);
}


Player::~Player(){
	for(int i = 0; i < 12; i++){
		SDL_FreeSurface(playerSpriteWalk[i]);
	}
	for(int i = 0; i < 12; i++){
		SDL_FreeSurface(playerSpriteWalkBomb[i]);
	}
	for(int i = 0; i < 8; i++){
		SDL_FreeSurface(playerSpriteThrowBomb[i]);
	}
	for(int i = 0; i < 4; i++){
		SDL_FreeSurface(playerSpriteOnLouis[i]);
	}
	for(int i = 0; i < 4; i++){
		SDL_FreeSurface(playerSpriteVictory[i]);
	}
	for(int i = 0; i < 4; i++){
		SDL_FreeSurface(playerSpriteAngry[i]);
	}
	for(int i = 0; i < 7; i++){
		SDL_FreeSurface(playerSpriteBurn[i]);
	}
	for(int i = 0; i < 12; i++){
		SDL_FreeSurface(louisSprite[i]);
	}
	for(int i = 0; i < 4; i++){
		SDL_FreeSurface(louisSpriteBurn[i]);
	}
	
	free(playerSpriteWalk);
	free(playerSpriteWalkBomb);
	free(playerSpriteThrowBomb);
	free(playerSpriteOnLouis);
	free(playerSpriteVictory);
	free(playerSpriteAngry);
	free(playerSpriteBurn);
	free(louisSprite);
	free(louisSpriteBurn);
	
	free(bombeSprite);
	
	free(in_keystate);
	tab = NULL;
	tabBonus = NULL;
	tabPlayerCoord = NULL;
	grid = NULL;
}






/*
* DRAWING PART
*/
void Player::drawNormal(SDL_Surface * surfaceToDraw, bool animate){
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
	if(animate){	
		if(frameCounter > nbFrame){
			frameCounter = 0;
			offsetSprite++;	
			if(offsetSprite >=nbFrameForAnimation){
			offsetSprite = 0;
			}
		}
		frameCounter++;
	}else{
		offsetSprite = 0;
	}	
	int offsetSpriteAnimation = 0;
	switch (offsetSprite){
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
	SDL_BlitSurface(playerSpriteWalk[(previousDirection * 3) + offsetSpriteAnimation], &srcTextureRect, surfaceToDraw, &destTextureRect);
}



void Player::drawOnLouis(SDL_Surface * surfaceToDraw, bool animate){
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
	if(animate){	
		if(frameCounter > nbFrame){
			frameCounter = 0;
			offsetSprite++;	
			if(offsetSprite >=nbFrameForAnimation){
				offsetSprite = 0;
			}
		}
		frameCounter++;
	}else{
		offsetSprite = 0;
	}	
	int offsetSpriteAnimation = 0;
	switch (offsetSprite){
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
	if(previousDirection == down){
		SDL_BlitSurface(playerSpriteOnLouis[previousDirection], &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisSprite[(previousDirection * 3) + offsetSpriteAnimation], &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	}else{
		SDL_BlitSurface(louisSprite[(previousDirection * 3) + offsetSpriteAnimation], &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(playerSpriteOnLouis[previousDirection], &srcTextureRect, louisMergebuffer, &srcTextureRect);
		SDL_BlitSurface(louisMergebuffer, &srcTextureRect, surfaceToDraw, &destTextureRect);
	}
	SDL_FreeSurface(louisMergebuffer);
}



void Player::drawWithBombe(SDL_Surface * surfaceToDraw, bool animate){
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
	if(animate){	
		if(frameCounter > nbFrame){
			frameCounter = 0;
			offsetSprite++;	
			if(offsetSprite >=nbFrameForAnimation){
				offsetSprite = 0;
			}
		}
		frameCounter++;
	}else{
		offsetSprite = 0;
	}	
	int offsetSpriteAnimation = 0;
	switch (offsetSprite){
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
	SDL_BlitSurface(playerSpriteWalkBomb[(previousDirection * 3) + offsetSpriteAnimation], &srcTextureRect, surfaceToDraw, &destTextureRect);
}



void Player::drawThrowBombe(SDL_Surface * surfaceToDraw, bool animate){
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
	if(animate){	
		if(frameCounter > nbFrame){
			frameCounter = 0;
			offsetSprite++;	
			if(offsetSprite >=nbFrameForAnimation){
				offsetSprite = 0;
			}
		}
		frameCounter++;
	}else{
		offsetSprite = 0;
	}
	SDL_BlitSurface(playerSpriteThrowBomb[(previousDirection * 2) + offsetSprite], &srcTextureRect, surfaceToDraw, &destTextureRect);
}



void Player::drawBurning(SDL_Surface * surfaceToDraw, bool animate){
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
	if(animate){	
		if(frameCounter > nbFrame){
			frameCounter = 0;
			offsetSprite++;	
			if(offsetSprite >=nbFrameForAnimation){
				offsetSprite = 0;
				playerState = dead;
				fprintf(stderr,"reset player pos");
				tabPlayerCoord[playerNumber * 2] = -1.0;
				tabPlayerCoord[playerNumber * 2 + 1 ] = -1.0;
			return;
		}
	}
		frameCounter++;
	}else{
		offsetSprite = 0;
	}	
	SDL_BlitSurface(playerSpriteBurn[offsetSprite], &srcTextureRect, surfaceToDraw, &destTextureRect);
}



void Player::drawLouisBurning(SDL_Surface * surfaceToDraw, bool animate){
	nbFrameForAnimation = animationLouisBurning;
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
	if(animate){	
		if(frameCounter > nbFrame){
			frameCounter = 0;
			offsetSprite++;	
			if(offsetSprite >=nbFrameForAnimation){
				offsetSprite = 0;
			}
		}
		frameCounter++;
	}else{
		offsetSprite = 0;
	}
	SDL_BlitSurface(playerSpriteBurn[offsetSprite], &srcTextureRect, surfaceToDraw, &destTextureRect);
}



void Player::drawVictory(SDL_Surface * surfaceToDraw, bool animate){
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
	if(animate){	
		if(frameCounter > nbFrame){
			frameCounter = 0;
			offsetSprite++;	
			if(offsetSprite >=nbFrameForAnimation){
				offsetSprite = 0;
			}
		}
		frameCounter++;
	}else{
		offsetSprite = 0;
	}	
	int offsetSpriteAnimation = 0;
	switch (offsetSprite){
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
	SDL_BlitSurface(playerSpriteVictory[offsetSpriteAnimation], &srcTextureRect, surfaceToDraw, &destTextureRect);
}



void Player::drawCrying(SDL_Surface * surfaceToDraw, bool animate){
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
	
	if(animate){	
		if(frameCounter > nbFrame){
		frameCounter = 0;
		offsetSprite++;	
			if(offsetSprite >=nbFrameForAnimation){
				offsetSprite = 0;
			}
		}
		frameCounter++;
	}else{
		offsetSprite = 0;
	}	
	int offsetSpriteAnimation = 0;
	switch (offsetSprite){
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
	SDL_BlitSurface(playerSpriteAngry[offsetSpriteAnimation], &srcTextureRect, surfaceToDraw, &destTextureRect);
}





void Player::doSomething(SDL_Surface * surfaceToDraw){
	unsigned short keystate = *in_keystate;
	bool animate = false;
	if(playerState != dead || playerState == victory){
		if(playerState != burning){
		/*
		* Direction of a player
		*/
		int roundX = floor(posX);
		int roundY = floor(posY);
		
		//update coordonate in global memory		
		posX = tabPlayerCoord[playerNumber * 2];
		posY = tabPlayerCoord[playerNumber * 2 + 1 ];

		if(tabBonus[roundX + roundY * sizeX] != noBonus && tab[roundX + roundY * sizeX] < brickElement){
			foundABonus(tabBonus[roundX + roundY * sizeX]);
		}
		
		if(invincibleTime == 0){
			if(tab[roundX + roundY * sizeX] == explosionElement){
				if(playerState == onLouis){
					invincibleTime = 50;
					playerState = normal;
				}else{
					playerState = burning;
					animate = true;
				}
			}
		}else{
			invincibleTime--;	
		}
		
		if(roundY > sizeY){
			posY = 0.0;
			roundY = 0;
		}
		
		if(roundY < 0){
			posY = sizeY + 0.9;
			roundY = sizeY;
		}
		
		float margeInf;
		float margeSup;
		
		if(playerSpeed > 0.1){
			margeInf = 0.51 - (playerSpeed/2);
			margeSup = 0.49 + (playerSpeed/2);
		}else{
			margeInf = 0.49 - (playerSpeed/2);
			margeSup = 0.51 + (playerSpeed/2);
		}
		
		
		
		if(posX-(float)roundX >= margeInf && posX-(float)roundX <= margeSup){
			posX = (float)floor(posX)+0.5;
		}
		
		if(posY-(float)roundY >= margeInf && posY-(float)roundY <= margeSup){
			posY = (float)floor(posY) + 0.5;
		}
		
		int canPassBlock = 0;
		if(ghostModePower == true){
			canPassBlock = brickElement;
		}else{
			canPassBlock = explosionElement;
		}
		
		//left
		if(playerMalus != invDirectionMalus ? keystate & keyPadLeft : keystate & keyPadRight){
			if(posX - roundX == 0.5){
				if(!(tab[(roundX - 1) + (roundY * sizeX)] > canPassBlock)){		
					posX = ( posX - playerSpeed );
				}else if(kickPower){
					if(tab[(roundX - 1) + (roundY * sizeX)] == bombeElement){
						kickIndex = (roundX - 1) + (roundY * sizeX);
						kickDirection = kickOnLeft;
					}
				}
			}else{
				if(posY - roundY > 0.5){
					posY = ( posY - playerSpeed );
				}else if(posY - roundY < 0.5){
					posY = ( posY + playerSpeed );
				}else{
					posX = ( posX - playerSpeed );
				}
			}
			previousDirection = left;
			animate = true;
		}
		
		//right
		if(playerMalus != invDirectionMalus ? keystate & keyPadRight : keystate & keyPadLeft){
			if(posX - roundX == 0.5){
				if(!(tab[(roundX + 1) + (roundY * sizeX)] > canPassBlock)){
					posX = ( posX + playerSpeed );
				}else if(kickPower){
					if(tab[(roundX + 1) + (roundY * sizeX)] == bombeElement){
						kickIndex = (roundX + 1) + (roundY * sizeX);
						kickDirection = kickOnRight;
					}				
				}
			}else{
				if(posY - roundY > 0.5){
					posY = ( posY - playerSpeed );
				}else if(posY - roundY < 0.5){
					posY = ( posY + playerSpeed );
				}else{
					posX = ( posX + playerSpeed );	
				}
			}
			previousDirection = right;
			animate = true;
		}
		
		//down
		if(playerMalus != invDirectionMalus ? keystate & keyPadDown : keystate & keyPadUp){
			if(posY - roundY == 0.5){
				if(roundY >= (sizeY-1)){
					posY = ( posY + playerSpeed );
				}else{
					if(!(tab[roundX + ((roundY + 1 ) * sizeX)] > canPassBlock)){
						posY = ( posY + playerSpeed );
					}else if(kickPower){
						if(tab[roundX + ((roundY + 1 ) * sizeX)] == bombeElement){	
							kickIndex = roundX + ((roundY + 1 ) * sizeX);
							kickDirection = kickOnDown;
						}
					}
				}
			}else{
				if(posX - roundX > 0.5){
					posX = ( posX - playerSpeed );
				}else if(posX - roundX < 0.5){
					posX = ( posX + playerSpeed );
				}else{
					posY = ( posY + playerSpeed );
				}
			}
			previousDirection = down;
			animate = true;
		}
		
		//up
		if(playerMalus != invDirectionMalus ? keystate & keyPadUp : keystate & keyPadDown){
			if(posY - roundY == 0.5){
				if(roundY == 0){
					posY = ( posY - playerSpeed );
				}else{
					if(!(tab[roundX + ((roundY - 1 ) * sizeX)] > canPassBlock)){
						posY = ( posY - playerSpeed );
					}else if(kickPower){
						if(tab[roundX + ((roundY - 1 ) * sizeX)] == bombeElement){
							kickIndex = roundX + ((roundY - 1 ) * sizeX);
							kickDirection = kickOnUp;
						}
					}
				}
			}else{
				if(posX - roundX > 0.5){
					posX = ( posX - playerSpeed );
				}else if(posX - roundX < 0.5){
					posX = ( posX + playerSpeed );
				}else{
					posY = ( posY - playerSpeed );
				}
			}
			previousDirection = up;
			animate = true;
		}
		
		
		/*
		* ACTION OF A PLAYER
		*/
		
		if(keystate & keyPadStart){
		//display menu	
		}
		if(keystate & keyPadA){
			if(playerMalus != constipationMalus){
				putABombe = true;
			}
		}
		if(keystate & keyPadB){
			if(bombeType == radioBombeType){
				triggerBombe = true;	
			}
		}
		if(keystate & keyPadX){
		//display menu	
		}
		if(keystate & keyPadY){
		//display menu	
		}	
		
		
		
		if(playerMalus == diarheeMalus){
			putABombe = true;
		}
	}
	
	}
	switch(playerState){
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
		case louisBurning:
			drawLouisBurning(surfaceToDraw, animate);
			break;
		case victory:
			drawVictory(surfaceToDraw, true);
			break;
		case crying:
			drawCrying(surfaceToDraw, animate);
			break;
	}
	
	if(playerState == dead){
		tabPlayerCoord[playerNumber * 2] = -1.0;
		tabPlayerCoord[playerNumber * 2 + 1 ] = -1.0;
	}else{
		//malus treatment
		tabPlayerCoord[playerNumber * 2] = posX;
		tabPlayerCoord[playerNumber * 2 + 1 ] = posY;
		if(nbTickMalus > 0){
			nbTickMalus--;
		}else if(nbTickMalus == 0){
			releaseMalus();
			nbTickMalus--;
		}
	}
}

Bombe * Player::addBombe(){
	int time = 100;
	int strenght = flameStrengh;
	switch(bombeType){
		case radioBombeType:
			time = -1;
			break;
		case powerBombeType:
		case normalBombeType:
		case bubbleBombeType:
			break;		
	}
	switch(playerMalus){
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
	tab[(int)floor(posX) + ((int)floor(posY)*sizeX)] = bombeElement;
	return new Bombe(strenght, floor(posX) + 0.5, floor(posY) + 0.5, bombeType, playerNumber, time, bombeSprite, tab, tabPlayerCoord);
}

int Player::getPlayerNumber(){
	return playerNumber;	
}

bool Player::triggerPowerBombe(){
	return triggerBombe;	
}

void Player::releaseTrigger(){
	triggerBombe = false;
}

bool Player::wantPutBombe(){
	if(isAlive()){
		if(NBBombeRemaining>0 && tab[(int)floor(posX) + ((int)floor(posY)*sizeX)] != bombeElement){
			return putABombe;
		}else{
			putABombe = false;
			return false;
		}
	}
	return false;
}

bool Player::walkOnWall(){
	if(tab[(int)floor(posX) + (int)floor(posY) * sizeX] < brickElement){
		return false;
	}else{
		putABombe = false;
		return true;	
	}
}

void Player::ABombeExplode(){
	triggerBombe = false;
	NBBombeRemaining++;
}

void Player::ABombeIsSet(){
	NBBombeRemaining--;
	putABombe = false;
}

bool Player::isAlive(){
	if(playerState == dead || playerState == burning){
		return false;
	}
	return true;
}

void Player::winTheGame(){
	playerState = victory;	
}

void Player::foundABonus(int bonusIndex){
	int roundX = floor(posX);
	int roundY = floor(posY);
	switch(bonusIndex){
		case deathBonus :
			getAMalusBonus();
			grid->placeNewDeathMalus();
			break;
		case rollerBonus :
			if(playerSpeed < 0.15){
				playerSpeed += 0.02;
			}
			break;
		case fireBonus :
			flameStrengh++;
			break;
		case fireMaxBonus :
			flameStrengh += 10;
			break;
		case bombeBonus :
			NbBombeMax++;
			NBBombeRemaining++;
			break;
		case kickBonus :
			kickPower = true;
			break;
		case gloveBonus :
			break;
		case radioBombeBonus :
			bombeType = radioBombeType;
			break;
		case bubbleBonus :
			bombeType = bubbleBombeType;
			break;
		case powerBombeBonus :
			bombeType = powerBombeType;
			break;
		case getaBonus :
			if(playerSpeed > 0.05){
				playerSpeed -= 0.02;
			}
		break;
		case ghostBonus :
			ghostModePower = true;
			break;
		case eggBonus :
			playerState = onLouis;
			break;
	}
	grid->burnBonus(roundX, roundY);
}


void Player::releaseMalus(){
	switch(playerMalus){
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



void Player::getAMalusBonus(){
	//reset in first Time the previous malus if exist
	if(playerMalus != noMalus){
		releaseMalus();
	}
	
	nbTickMalus = 15 * 50;
	
	/* generate secret number between 0 and 6: */
	int malus = rand() % 9 ;
	switch(malus){
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


int Player::findIndexPlayer(){
	std::vector<int> index;
	for(int i = 0; i < 16; i++){
		if(i != playerNumber){
			if(tabPlayerCoord[i * 2] != -1.0){
				index.push_back(i);
			}
		}
	}
	int idx = rand() % index.size();
	return index[idx];
}

bool Player::hasKickPower(){
	return kickPower;	
}

int Player::isKickingBombe(){
	return kickIndex;
}

int Player::getKickDirection(){
	return kickDirection;
}

void Player::releaseKick(){
	kickDirection = kickNone;
	kickIndex = -1;
}