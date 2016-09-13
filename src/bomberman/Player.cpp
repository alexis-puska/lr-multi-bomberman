#include "Player.h"

#define nbFrame 6

#define sprite_sizeW 30 
#define sprite_sizeH 42
#define blockSizeX 18
#define blockSizeY 16

#define stepOfPlayerY 0.1
#define stepOfPlayerX 0.1
#define nbFrameIncAnimation 5

const static char *BombermanSprite = "./resources/sprite/characters/AllBomberman.png";
const static char *BombermanSpriteCossak = "./resources/sprite/characters/AllBombermanCossak.png";
const static char *BombermanSpriteBarbar = "./resources/sprite/characters/AllBombermanBarbar.png";
const static char *BombermanSpriteChan = "./resources/sprite/characters/AllBombermanChan.png";
const static char *BombermanSpriteKid = "./resources/sprite/characters/AllBombermanKid.png";
const static char *BombermanSpritePretty = "./resources/sprite/characters/AllBombermanPretty.png";
const static char *BombermanSpritePunk = "./resources/sprite/characters/AllBombermanPunk.png";
const static char *BombermanSpriteMexican = "./resources/sprite/characters/AllBombermanMexican.png";

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


Player::Player(unsigned short * in_keystateLibretro, bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY, int playerNumberLibretro, int table[sizeX * sizeY])
{
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
		srcTextureRect.x = 7 * sprite_sizeW;
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
	
	posX = startPositionX;
	posY = startPositionY;
	cpu = isACpuPlayer;
	tab = table;
	playerNumber = playerNumberLibretro;
	characterSpriteIndex = indexSprite;
	in_keystate = in_keystateLibretro;
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
	
	free(playerSpriteWalk);
	free(playerSpriteWalkBomb);
	free(playerSpriteThrowBomb);
	free(playerSpriteOnLouis);
	free(playerSpriteVictory);
	free(playerSpriteAngry);
	free(playerSpriteBurn);
	
	free(in_keystate);
	free(tab);
}

void Player::doSomething(SDL_Surface * surfaceToDraw){
	int indexToDraw = previousDirection * 3;
	
	unsigned short keystate = *in_keystate;
	
	if(cpu){
		
	}else{
		int roundX = 0;
		int roundY = 0;
		if((posX - floor(posX)) <= 0.5){
			roundX = floor(posX);
		}else{
			roundX = ceil(posX);
		}
		if((posY - floor(posY)) <= 0.5){
			roundY = floor(posY);
		}else{
			roundY = ceil(posY);
		}
		
		
		if(keystate & keyPadLeft){
			posX = ( posX - stepOfPlayerX );
			previousDirection = left;
		}
		
		if(keystate & keyPadRight){
			posX = ( posX + stepOfPlayerX );
			previousDirection = right;
		}
		
		
		if(keystate & keyPadDown){
			posY = ( posY + stepOfPlayerY );
			previousDirection = down;
		}
		
		if(keystate & keyPadUp){
			posY = ( posY - stepOfPlayerY );
			previousDirection = up;
		}

//		if(playerNumber == 0)
//		fprintf(stderr, "%i %f %i %f",roundX, posX,roundY, posY);
	}
	
	
	
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
	if(keystate != 0){	
		if(frameCounter > nbFrame){
			frameCounter = 0;
			offsetSprite++;	
			if(offsetSprite >=4){
				offsetSprite = 0;
			}
		}
		frameCounter++;
	}else{
		offsetSprite = 0;
	}
	if(playerNumber == 0){
		fprintf(stderr, "%u %i\n",keystate, offsetSprite);
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

	SDL_BlitSurface(playerSpriteWalk[indexToDraw + offsetSpriteAnimation], &srcTextureRect, surfaceToDraw, &destTextureRect);
}




