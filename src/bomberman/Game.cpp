#include "Game.h"


#define gameTick 20


static int metronome(void* data)
{
	Game *game = ((Game *)data);
	struct timeval t1, t2;

	long delay;
	while(game->isAlive()){


		gettimeofday (&t1, NULL);
		game->tick();
		gettimeofday (&t2, NULL);
	
		
		long milliseconds = (t2.tv_usec - t1.tv_usec) / 1000;
		if(milliseconds < 0){
			milliseconds +=1000;	
		}
  
		delay = gameTick - milliseconds;

		if(delay > 0){
            fprintf(stderr, "game tick pause : %03ul \n", delay);
			SDL_Delay(delay);
		}else{
			fprintf(stderr, "game tick time tooooo long !!!!\n");
		}
	}
	return 0;
}


Game::Game(){
	isThreadAlive = false;
}

Game::~Game(){
	exitGame();
}

void Game::configure(){
	
}

void Game::tick(){
	
    SDL_Delay(15);
    
}

bool Game::isAlive(){
	return isThreadAlive;
}

void Game::startGame(){
	if(!isThreadAlive){
		isThreadAlive = true;
		mainThread = SDL_CreateThread(metronome, "mainThread", this);
	}
}

void Game::exitGame(){
	if(isThreadAlive){
		isThreadAlive = false;
		int treadResult;
		SDL_WaitThread(mainThread, &treadResult);
	}
}