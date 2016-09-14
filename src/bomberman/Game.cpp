#include "Game.h"


#define gameTick 20
#define sizeX 35
#define sizeY 21

Uint32 rmask, gmask, bmask, amask;
const static char *misc = "./resources/sprite/other/Misc.png";


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
	gameStart = false;
	//Load Font
    fragileBombersFont = TTF_OpenFont( "./resources/font/fragile_bombers.ttf", 36); //this opens a font style and sets a size
	//declarativ color mask, used for create a RGB surface
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    playerBombeExplode = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	screenBuffer = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	overlay = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	isThreadAlive = false;
	configured = false;
	requestStopGame = false;
	tab = new int[sizeX * sizeY];
}

Game::Game(int levelIndexInformation, int playerInformation[16][2], int gameOption[4], SDL_Surface *  vout_bufLibretro, unsigned short * in_keystateLibretro){
	gameStart = false;
	//Load Font
    fragileBombersFont = TTF_OpenFont( "./resources/font/fragile_bombers.ttf", 36); //this opens a font style and sets a size
	//declarativ color mask, used for create a RGB surface
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    //create overlay for pause / exit game
    overlay = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(overlay, NULL, SDL_MapRGBA(overlay->format, 0, 0, 0, 120));
	SDL_Color green = {0, 255, 0};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "PAUSE", green);
    copySurfaceToBackRenderer(surfaceMessage, overlay , ((640/2)-(surfaceMessage->w/2)), 114);
    SDL_FreeSurface(surfaceMessage);
    surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "Press X/B for exit game", green);
    copySurfaceToBackRenderer(surfaceMessage, overlay , ((640/2)-(surfaceMessage->w/2)), 154);
    SDL_FreeSurface(surfaceMessage);
    surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "Press Select for exit pause", green);
    copySurfaceToBackRenderer(surfaceMessage, overlay , ((640/2)-(surfaceMessage->w/2)), 194);
    SDL_FreeSurface(surfaceMessage);
    
    gamePause = false;
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
	
	
	/*
	* LOAD MISC IMAGE : Bombe animation, explosion animation, bonus image, eggs image
	*/
	
	explosionSprite = new SDL_Surface * [36];
	bombeSprite = new SDL_Surface * [12];
	bonusSprite = new SDL_Surface * [12];
	eggsSprite = new SDL_Surface * [2];
	
	SDL_Surface * tempSurface = IMG_Load(misc);
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = 0;
    destTextureRect.y = 0;
    destTextureRect.w = 16;
    destTextureRect.h = 16;

	int i=0;
	int j = 0;
	// explosionSprite
	for(i = 0 ; i < 4; i++){
		for(j = 0; j < 9; j++){
			srcTextureRect.x = i * 16;
		    srcTextureRect.y = j * 16;
		    srcTextureRect.w = 16;
		    srcTextureRect.h = 16;
			explosionSprite[i + (j * 4)] =  SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
        	SDL_BlitSurface(tempSurface, &srcTextureRect, explosionSprite[i + (j * 4)], &destTextureRect);
		}
	}
	
	// bonusSprite
	for(i = 0 ; i < 2; i++){
		for(j = 0; j < 6; j++){
			srcTextureRect.x = (i + 4) * 16;
		    srcTextureRect.y = j * 16;
		    srcTextureRect.w = 16;
		    srcTextureRect.h = 16;
			bonusSprite[i + (j * 2)] =  SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
        	SDL_BlitSurface(tempSurface, &srcTextureRect, bonusSprite[i + (j * 2)], &destTextureRect);
		}
	}
	
	// bombeSprite
	for(i = 0 ; i < 3; i++){
		for(j = 0; j < 4; j++){
			srcTextureRect.x = (i+6) * 16;
		    srcTextureRect.y = j * 16;
		    srcTextureRect.w = 16;
		    srcTextureRect.h = 16;
			bombeSprite[i + (j * 3)] =  SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
        	SDL_BlitSurface(tempSurface, &srcTextureRect, bombeSprite[i + (j * 3)], &destTextureRect);
		}
	}
	
	// eggsSprite
	for(i = 0 ; i < 2; i++){
		srcTextureRect.x = (i+4) * 16;
	    srcTextureRect.y = 4 * 16;
	    srcTextureRect.w = 16;
	    srcTextureRect.h = 16;
		eggsSprite[i] =  SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
       	SDL_BlitSurface(tempSurface, &srcTextureRect, eggsSprite[i], &destTextureRect);
	}

	SDL_FreeSurface(tempSurface);
	
	
	/*
	*	LOAD PLAYER ON GRID
	*/
	for(int i = 0; i < 16; i++){
		int indexTexture = playerInformation[i][1];
		float startX = startPlayer[i][0];
		float startY = startPlayer[i][1];
		Player * player = new Player(&in_keystate[i], false , indexTexture , startX, startY, i, tab, bombeSprite);
		players.push_back(player);
		player = NULL;
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
	SDL_FreeSurface(overlay);
	SDL_FreeSurface(screenBuffer);
	SDL_FreeSurface(playerBombeExplode);
	
	for(int i = 0; i < 36; i++){
		SDL_FreeSurface(explosionSprite[i]);
	}
	for(int i = 0; i < 12; i++){
		SDL_FreeSurface(bombeSprite[i]);
	}
	for(int i = 0; i < 12; i++){
		SDL_FreeSurface(bonusSprite[i]);
	}
	for(int i = 0; i < 2; i++){
		SDL_FreeSurface(eggsSprite[i]);
	}
	free(explosionSprite);
	free(bombeSprite);
	free(bonusSprite);
	free(eggsSprite);
	
	TTF_CloseFont(fragileBombersFont);
    TTF_Quit();
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

void Game::tick(){
	if(!gameStart){
		if(in_keystate[0] == 0){
			gameStart = true;
		}
	}else if(gameStart){
		if(gamePause){
			SDL_Rect mergeRect;
		    mergeRect.x = 0;
		    mergeRect.y = 0;
		    mergeRect.w = 630;
		    mergeRect.h = 336;
		    SDL_BlitSurface(grid->getGroundLayer()	, &mergeRect, screenBuffer, &mergeRect);
		    SDL_BlitSurface(grid->getBricksLayer()	, &mergeRect, screenBuffer, &mergeRect);
		    SDL_BlitSurface(playerBombeExplode		, &mergeRect, screenBuffer, &mergeRect);
		    SDL_BlitSurface(grid->getSkyLayer()		, &mergeRect, screenBuffer, &mergeRect);
			SDL_BlitSurface(overlay					, &mergeRect, screenBuffer, &mergeRect);
	    	copySurfaceToBackRenderer(screenBuffer, vout_buf, 5, 24);
			if(in_keystate[0] & keyPadSelect && !requestStopGame){
				gamePause = false;
				gameStart = false;
			}
			if(in_keystate[0] & keyPadB && !requestStopGame){
				requestStopGame = true;
				gamePause = false;
			}
		}else{
			amask = 0xff000000;
		    rmask = 0x00ff0000;
		    gmask = 0x0000ff00;
		    bmask = 0x000000ff;
			if(in_keystate[0] & keyPadStart && !requestStopGame){
				gamePause = true;		
			}
			
			
			
			SDL_FillRect(playerBombeExplode, NULL, SDL_MapRGBA(playerBombeExplode->format, 0, 0, 0, 0));
			//	Bombe
			//	decompte explosion
			//	si explostion
			//		notification grille suppresion bombe
			//			si bonus à la coordonnée, ajout du sprite pour le bonus
			//explosion
			//	animation
			//		fin d'animation
			//
			//joueur
			//	doSomething
			//		check si dans explosion
			//	check pose bombe
			//		ajout nouvelle bombe
			//for each bonus
			//	check position joueur pour récupérer le bonus
			//Ajout bonus aléatoire
		
			
			for(unsigned int i=0;i<bombes.size();i++){
				bombes[i] -> tick(playerBombeExplode);
				if(bombes[i]->isExplode()){
					players[bombes[i]->getPlayer()]->ABombeExplode();
					bombes.erase(bombes.begin()+i);
				}
			}
		
		
			for(unsigned int i=0;i<players.size();i++){
				players[i] -> doSomething(playerBombeExplode);
				if(players[i] -> wantPutBombe()){
					bombes.push_back(players[i] -> addBombe());
					players[i] -> ABombeIsSet();
				}
			}
			mergeScreen();
		}
	}
}
