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
	none	=-1,
	down	= 0,
	up		= 1,
	left	= 2,
	right	= 3
};
	
enum playerSprite{
	bomberman	= 0,	 
	cossak		= 1,
	barbar		= 2,
	chan		= 3,
	kid			= 4,
	pretty		= 5,
	punk		= 6,
	mexican		= 7	
};

enum playerStateEnum{
	normal			= 0,
	onLouis			= 1,
	carryBombe		= 2,
	throwBombe		= 3,
	burning			= 4,
	louisBurning	= 5,
	victory			= 6,
	crying			= 7,
	dead			= 8
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
	blueLouis		= 0,
	yellowLouis		= 1,
	pinkLouis		= 2,
	greenLouis		= 3,
	brownLouis		= 4
};



Player::Player(unsigned short * in_keystateLibretro, bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY, int playerNumberLibretro, int table[sizeX * sizeY], SDL_Surface ** bombeSpriteGame)
{
	playerState = normal;
	NbBombeMax = 2;
	NBBombeRemaining = 2;
	bubbleBombePower = false;
	haveGlovePower = false;
	ghostModePower = false;
	powerBombePower = false;
	radioBombePower = false;
	putABombe = false;
	flameStrengh = 2;
	playerSpeed = 0.1;
	
	
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
	cpu = isACpuPlayer;
	tab = table;
	playerNumber = playerNumberLibretro;
	characterSpriteIndex = indexSprite;
	in_keystate = in_keystateLibretro;
	bombeSprite = bombeSpriteGame;
	SDL_FreeSurface(tempSurface);
}

Player::~Player()
{
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
	free(tab);
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
			
			if(tab[roundX + roundY * sizeX] == 1){
				playerState = burning;
				animate = true;
			}
					
			if(roundY > sizeY){
				posY = 0.0;
				roundY = 0;
			}
			
			if(roundY < 0){
				posY = sizeY + 0.9;
				roundY = sizeY;
			}
	
			float margeInf = 0.51 - (playerSpeed/2);
			float margeSup = 0.49 + (playerSpeed/2);
			
			if(posX-(float)roundX >= margeInf && posX-(float)roundX <= margeSup){
				posX = (float)floor(posX)+0.5;
			}
			
			if(posY-(float)roundY >= margeInf && posY-(float)roundY <= margeSup){
				posY = (float)floor(posY) + 0.5;
			}
			
			if(keystate & keyPadLeft){
				if(posX - roundX == 0.5){
					if(!(tab[(roundX - 1) + (roundY * sizeX)] > 1)){		
						posX = ( posX - playerSpeed );
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
			
			if(keystate & keyPadRight){
				if(posX - roundX == 0.5){
					if(!(tab[(roundX + 1) + (roundY * sizeX)] > 1)){
						posX = ( posX + playerSpeed );
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
			
			
			
			
			if(keystate & keyPadDown){
				
				if(posY - roundY == 0.5){
					if((roundY+1) > sizeY){
						if(!(tab[roundX + ((sizeY+1) * sizeX)] > 1)){
							posY = ( posY + playerSpeed );
						}
					}else{
						if(!(tab[roundX + ((roundY + 1 ) * sizeX)] > 1)){
							posY = ( posY + playerSpeed );
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
			
			if(keystate & keyPadUp){
				if(posY - roundY == 0.5){
					if((roundY-1) < 0){
						if(!(tab[roundX + ((sizeY-1) * sizeX)] > 1)){
							posY = ( posY - playerSpeed );
						}
					}else{
						if(!(tab[roundX + ((roundY - 1 ) * sizeX)] > 1)){
							posY = ( posY - playerSpeed );
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
				putABombe = true;	
			}
			if(keystate & keyPadB){
				//display menu	
			}
			if(keystate & keyPadX){
				//display menu	
			}
			if(keystate & keyPadY){
				//display menu	
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
}

Bombe * Player::addBombe(){
	int type = normalBombeType;
	int time = 100	;
	int strenght = flameStrengh;
	
	if(bubbleBombePower){
		type = bubbleBombeType;
	}
	if(radioBombePower){
		type = radioBombeType;
		time = -1;
	}
	if(powerBombePower){
		strenght = 30;
		type = powerBombeType;
	}
	tab[(int)floor(posX) + ((int)floor(posY)*sizeX)] = 3;
	return new Bombe(flameStrengh, (int)floor(posX), (int)floor(posY), type, playerNumber, time, bombeSprite, tab);
}

bool Player::wantPutBombe(){
	if(isAlive()){
		if(NBBombeRemaining>0 && tab[(int)floor(posX) + ((int)floor(posY)*sizeX)] != 3){
			return putABombe;
		}else{
			putABombe = false;
			return false;
		}
	}
	return false;
}

void Player::ABombeExplode(){
	NBBombeRemaining++;
}

void Player::ABombeIsSet(){
	NBBombeRemaining--;
	putABombe = false;
}

void Player::takeAnEgg(){
	playerState = onLouis;
}

void Player::takeGlove(){
	haveGlovePower = true;
}

void Player::ghostMode(){
	ghostModePower = true;
}

void Player::flameUp(){
	flameStrengh++;
}

void Player::speedUp(){
	playerSpeed = playerSpeed + 0.02;
}

void Player::speedDown(){
	playerSpeed = playerSpeed - 0.02;
}

void Player::powerBombe(){
	powerBombePower = true;
}

void Player::radioBombe(){
	radioBombePower = true;
}

void Player::bubleBombe(){
	bubbleBombePower = true;
}

void Player::bombeNbUp(){
	NbBombeMax++;
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