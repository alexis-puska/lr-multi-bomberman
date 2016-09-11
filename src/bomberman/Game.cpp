#include "Game.h"


#define gameTick 20
#define sizeX 35
#define sizeY 21

Uint32 rmask, gmask, bmask, amask;

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
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    playerBombeExplode = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	screenBuffer = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	isThreadAlive = false;
	configured = false;
	requestStopGame = false;
	tab = new int[sizeX * sizeY];
}

Game::Game(int levelIndexInformation, int playerInformation[16][2], int gameOption[4], SDL_Surface *  vout_bufLibretro, unsigned short * in_keystateLibretro){
	//declarativ color mask, used for create a RGB surface
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
	screenBuffer = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	playerBombeExplode = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	isThreadAlive = false;
	configured = true;
	requestStopGame = false;
	tab = new int[sizeX * sizeY];
	
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
	grid = new Grid(levelIndexInformation, tab);
	
	for(int i = 0; i < 16; i++){
		
		int indexTexture = playerInformation[i][1];
		int startX = startPlayer[i][0];
		int startY = startPlayer[i][1];
		Player * player = new Player(&in_keystate[i], false , indexTexture , startX, startY, i, tab);
		players.push_back(player);
	}
	
	
//		for(int i=0;i<myvector.size();i++){
//			if(myvector[i] == 6 || myvector[i] == 2 || myvector[i] == 8){
//				  myvector.erase (myvector.begin()+i);			
//			}
//		}
//		for (unsigned i=0; i<myvector.size(); ++i)
//			fprintf(stderr,"%i", myvector[i]);
//			fprintf(stderr,"\n\n");
//		}
	
	
	mergeScreen();
	startGame();
}

Game::~Game(){
	exitGame();
	free(grid);
	free(tab);
	free(in_keystate);
	players.clear();
	SDL_FreeSurface(vout_buf);
	SDL_FreeSurface(screenBuffer);
	SDL_FreeSurface(playerBombeExplode);
}

void Game::startGame(){
	if(!isThreadAlive){
		isThreadAlive = true;
		mainThread = SDL_CreateThread(metronome, "mainThread", this);
	}
}

void Game::exitGame(){
	stopGame();
}

void Game::stopGame(){
	if(isThreadAlive){
		isThreadAlive = false;
		configured = false;
		requestStopGame = false;
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
	amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
	
	if(in_keystate[0] & keyPadB && !requestStopGame){
		requestStopGame = true;		
	}
	
	SDL_FillRect(playerBombeExplode, NULL, SDL_MapRGBA(playerBombeExplode->format, 0, 0, 0, 0));
	
	for(int i=0;i<players.size();i++){
		players[i] -> doSomething(playerBombeExplode);
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
    SDL_BlitSurface(playerBombeExplode		, &mergeRect, screenBuffer, &mergeRect);
    SDL_BlitSurface(grid->getSkyLayer()		, &mergeRect, screenBuffer, &mergeRect);
    
    copySurfaceToBackRenderer(screenBuffer, vout_buf, 5, 24);
}
