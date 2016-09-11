#include "Player.h"

#define nbFrame 8
#define nb_imageX 9
#define nb_imageY 7
#define sprite_sizeW 30 
#define sprite_sizeH 42
#define blockSizeX 18
#define blockSizeY 16

#define stepOfPlayerY 0.08
#define stepOfPlayerX 0.08
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
	previousDirection = down;
	Uint32 rmask, gmask, bmask, amask;
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
	
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
    playerSprite = new SDL_Surface * [nb_imageX * nb_imageY];
	
	for(int i = 0 ; i < nb_imageX; i++){
		for(int j = 0; j < nb_imageY; j++){
			srcTextureRect.x = i * sprite_sizeW;
		    srcTextureRect.y = j * sprite_sizeH;
		    srcTextureRect.w = sprite_sizeW;
		    srcTextureRect.h = sprite_sizeH;
			playerSprite[i + (j * nb_imageX)] =  SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
        	SDL_BlitSurface(tempSurface, &srcTextureRect, playerSprite[i + (j * nb_imageX)], &destTextureRect);
		}
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
	for(int i = 0; i < nb_imageX * nb_imageY; i++){
		SDL_FreeSurface(playerSprite[i]);
	}
	free(in_keystate);
	free(tab);
	free(playerSprite);
}

void Player::doSomething(SDL_Surface * surfaceToDraw){
	int indexToDraw = previousDirection * 9;
	if(cpu){
		
	}else{
		unsigned short keystate = *in_keystate;
		
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
		
				
		
		
		if(playerNumber == 0)
		fprintf(stderr, "%i %f %i %f",roundX, posX,roundY, posY);
	}
	
	
	
	
	
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = (posX * blockSizeX) - 6;
    destTextureRect.y = (posY * blockSizeY) - 32;
    destTextureRect.w = sprite_sizeW;
    destTextureRect.h = sprite_sizeH;
	srcTextureRect.x = 0;
	srcTextureRect.y = 0;
	srcTextureRect.w = sprite_sizeW;
	srcTextureRect.h = sprite_sizeH;
	SDL_BlitSurface(playerSprite[indexToDraw], &srcTextureRect, surfaceToDraw, &destTextureRect);
}




