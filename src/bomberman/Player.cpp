#include "Player.h"

#define nbFrame 8


Player::Player()
{
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


