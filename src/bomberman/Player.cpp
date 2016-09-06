#include "Player.h"

#define nbFrame 8


Player::Player(bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY)
{
	posX = startPositionX;
	posY = startPositionY;
	cpu = isACpuPlayer;
	
	playerSprite = new SDL_Surface*[nbFrame];
	
	
}

Player::~Player()
{
	for(int i = 0; i < nbFrame; i++){
		SDL_FreeSurface(playerSprite[i]);
	}
}

void Player::doSomething(){
	
}


