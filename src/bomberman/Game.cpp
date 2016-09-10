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
	//declarativ color mask, used for create a RGB surface
    Uint32 rmask, gmask, bmask, amask;
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
	screenBuffer = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	isThreadAlive = false;
	configured = false;
	requestStopGame = false;
}

Game::Game(int levelIndexInformation, int playerInformation[16][2], int gameOption[4], SDL_Surface *  vout_bufLibretro, unsigned short * in_keystateLibretro){
	//declarativ color mask, used for create a RGB surface
    Uint32 rmask, gmask, bmask, amask;
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
	screenBuffer = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	isThreadAlive = false;
	configured = true;
	requestStopGame = false;
	
	levelIndex = levelIndexInformation;
	if(gameOption[0] == 1){
		suddenDeath = true;
	}else{
		suddenDeath = false;
	}
	
	if(gameOption[1] == 1){
		badBomber = true;
	}else{
		badBomber = false;
	}
		
	cpuLevel = gameOption[2];
	time = gameOption[3];
	
	
	in_keystate = in_keystateLibretro;
	vout_buf = vout_bufLibretro;
	
	fprintf(stderr, "generate one grid\n");
	grid = new Grid(levelIndexInformation);
	
	mergeScreen();
	startGame();
}

Game::~Game(){
	exitGame();
	SDL_FreeSurface(screenBuffer);
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
		int treadResult = 0;
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
	mergeScreen();
}


    
void Game::mergeScreen(){
	SDL_Rect mergeRect;
    mergeRect.x = 0;
    mergeRect.y = 0;
    mergeRect.w = 630;
    mergeRect.h = 336;
    SDL_BlitSurface(grid->getGroundLayer()	, &mergeRect, screenBuffer, &mergeRect);
    SDL_BlitSurface(grid->getBricksLayer()	, &mergeRect, screenBuffer, &mergeRect);
    SDL_BlitSurface(grid->getSkyLayer()		, &mergeRect, screenBuffer, &mergeRect);
    
    copySurfaceToBackRenderer(screenBuffer, vout_buf, 5, 24);
}
