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
            fprintf(stderr, "game tick pause : %i \n", (int)delay);
			SDL_Delay(delay);
		}else{
			fprintf(stderr, "game tick time tooooo long !!!!\n");
		}
	}
	return 0;
}

Game::Game(){
	isThreadAlive = false;
	configured = false;
	requestStopGame = false;
}

Game::Game(int levelIndexInformation, int playerInformation[16][2], int gameOption[4], SDL_Surface *  vout_bufLibretro, unsigned short * in_keystateLibretro){
	isThreadAlive = false;
	configured = true;
	requestStopGame = false;
	
	int levelIndex = levelIndexInformation;
	if(gameOption[0] == 1){
		bool suddenDeath = true;
	}else{
		bool suddenDeath = false;
	}
	
	if(gameOption[1] == 1){
		bool badBomber = true;
	}else{
		bool badBomber = false;
	}
		
	int cpuLevel = gameOption[2];
	int time = gameOption[3];
	
	
	in_keystate = in_keystateLibretro;
	vout_buf = vout_bufLibretro;
	
	fprintf(stderr, "generate one grid\n");
	grid = new Grid(levelIndexInformation);
	
	copySurfaceToBackRenderer(grid->getGrid(),vout_buf, 5, 24);
	startGame();
}

Game::~Game(){
	exitGame();	
}

void Game::startGame(){
	if(!isThreadAlive){
		isThreadAlive = true;
		mainThread = SDL_CreateThread(metronome, "mainThread", this);
	}
}

void Game::exitGame(){
	stopGame();
	free(grid);
}

void Game::stopGame(){
	if(isThreadAlive){
		isThreadAlive = false;
		int treadResult;
		SDL_WaitThread(mainThread, &treadResult);
		fprintf(stderr, "result stop thread %i\n", treadResult);
	}
}

void Game::copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y){
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = src->w;
    dstRect.h = src->h;
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = src->w;
    srcRect.h = src->h;
    SDL_BlitSurface(src, &srcRect, dest, &dstRect);
}

bool Game::isConfigured(){
	return configured;
}

bool Game::isAlive(){
	return isThreadAlive;
}

bool Game::isRequestStopGame(){
	return requestStopGame;
}

void Game::tick(){
	if(in_keystate[0] & keyPadB && !requestStopGame){
		requestStopGame = true;		
	}

	copySurfaceToBackRenderer(grid->getGrid(),vout_buf, 5, 24);
}