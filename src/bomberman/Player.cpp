#include "Player.h"

#define nbFrame 8



//const static char *BombermanSpriteCossak = "./resources/sprite/characters/AllBombermanCossak.png";
//const static char *BombermanSprite = "./resources/sprite/characters/AllBomberman.png";
//const static char *BombermanSpriteBarbar = "./resources/sprite/characters/AllBombermanBarbar.png";
//const static char *BombermanSpriteChan = "./resources/sprite/characters/AllBombermanChan.png";
//const static char *BombermanSpriteKid = "./resources/sprite/characters/AllBombermanKid.png";
//const static char *BombermanSpritePretty = "./resources/sprite/characters/AllBombermanPretty.png";
//const static char *BombermanSpritePunk = "./resources/sprite/characters/AllBombermanPunk.png";
//const static char *BombermanSpriteMexican = "./resources/sprite/characters/AllBombermanMexican.png";

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

#define stepOfPlayer 0.625
#define nbFrameIncAnimation 5

Player::Player(unsigned short * in_keystateLibretro, bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY, int playerNumberLibretro)
{
	posX = startPositionX;
	posY = startPositionY;
	cpu = isACpuPlayer;
	playerNumber = playerNumberLibretro;
	characterSpriteIndex = indexSprite;
	in_keystate = in_keystateLibretro;
	playerSprite = new SDL_Surface*[nbFrame];
}

Player::~Player()
{
	for(int i = 0; i < nbFrame; i++){
		SDL_FreeSurface(playerSprite[i]);
	}
}

void Player::doSomething(){
	if(cpu){
		
	}else{
		unsigned short keystate = in_keystate[playerNumber];
		if(keystate & keyPadLeft){
			posX = posX - stepOfPlayer;
			fprintf(stderr, "push left for player : %i", playerNumber);	
		}
	}
}


