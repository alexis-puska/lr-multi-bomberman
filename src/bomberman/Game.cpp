#include "Game.h"


#define gameTick 20


Uint32 rmask, gmask, bmask, amask;
const static char *misc = "./resources/sprite/other/Misc.png";
const static char *fireSprite = "./resources/sprite/other/Fire.png";
const static char *levelSprite = "./resources/sprite/level/AllLevel.png";
const static char *background = "./resources/image/EmptyBackground.png";


const static char *BombermanSprite = "./resources/sprite/characters/AllBomberman.png";
const static char *BombermanSpriteCossak = "./resources/sprite/characters/AllBombermanCossak.png";
const static char *BombermanSpriteBarbar = "./resources/sprite/characters/AllBombermanBarbar.png";
const static char *BombermanSpriteChan = "./resources/sprite/characters/AllBombermanChan.png";
const static char *BombermanSpriteKid = "./resources/sprite/characters/AllBombermanKid.png";
const static char *BombermanSpritePretty = "./resources/sprite/characters/AllBombermanPretty.png";
const static char *BombermanSpritePunk = "./resources/sprite/characters/AllBombermanPunk.png";
const static char *BombermanSpriteMexican = "./resources/sprite/characters/AllBombermanMexican.png";

/*
* 
* TICK generator
* 
*/

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
//            fprintf(stderr, "%i\n", (int)delay);
			SDL_Delay(delay);
		}else{
			fprintf(stderr, "warning\n");
		}
	}
	return 0;
}

/*
* 
* Default constructor
* 
*/

Game::Game(){
	gameState = gameWait;
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
	tabBonus = new int[sizeX * sizeY];
}

/*
* 
* constructor
* 
*/
Game::Game(int levelIndexInformation, int playerInformationParam[16][2], int gameOption[4], SDL_Surface *  vout_bufLibretro, unsigned short * in_keystateLibretro){
	gameState = gameWait;
	
	// Load Font
    fragileBombersFont = TTF_OpenFont( "./resources/font/fragile_bombers.ttf", 36); //this opens a font style and sets a size
	
	// declarativ color mask, used for create a RGB surface
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    // create overlay for pause / exit game
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
    TTF_CloseFont(fragileBombersFont);
    
    // init variable or surface
	screenBuffer = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	playerBombeExplode = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	isThreadAlive = false;
	configured = true;
	requestStopGame = false;
	tab = new int[sizeX * sizeY];
	tabBonus = new int[sizeX * sizeY];
	
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
	if(gameOption[3] != -1){
		nbTickForGame = gameOption[3] * 50 * 60;
		nbTickForGameParam = nbTickForGame;
	}else{
		nbTickForGame = gameOption[3];
		nbTickForGameParam = nbTickForGame;
	}
	
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
	burnWallSprite = new SDL_Surface * [6];
	headerPlayerSprite = new SDL_Surface * [16];
	
	SDL_Surface * tempSurface = IMG_Load(fireSprite);
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
			srcTextureRect.x = i * 18;
		    srcTextureRect.y = j * 16;
		    srcTextureRect.w = 18;
		    srcTextureRect.h = 16;
			explosionSprite[i + (j * 4)] =  SDL_CreateRGBSurface(0, 18, 16, 32, rmask, gmask, bmask, amask);
        	SDL_BlitSurface(tempSurface, &srcTextureRect, explosionSprite[i + (j * 4)], &destTextureRect);
		}
	}
	
	SDL_FreeSurface(tempSurface);
	tempSurface = IMG_Load(misc);
	
	// bonusSprite
	for(i = 0 ; i < 2; i++){
		for(j = 0; j < 6; j++){
			srcTextureRect.x = i * 16;
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
			srcTextureRect.x = (i+2) * 16;
		    srcTextureRect.y = j * 16;
		    srcTextureRect.w = 16;
		    srcTextureRect.h = 16;
			bombeSprite[i + (j * 3)] =  SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
        	SDL_BlitSurface(tempSurface, &srcTextureRect, bombeSprite[i + (j * 3)], &destTextureRect);
		}
	}
	
	// eggsSprite
	for(i = 0 ; i < 2; i++){
		srcTextureRect.x = i * 16;
	    srcTextureRect.y = 4 * 16;
	    srcTextureRect.w = 16;
	    srcTextureRect.h = 16;
		eggsSprite[i] =  SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
       	SDL_BlitSurface(tempSurface, &srcTextureRect, eggsSprite[i], &destTextureRect);
	}
	SDL_FreeSurface(tempSurface);
	
	
	// Load Surface for burn wall animation
	// levelIndexInformation
	tempSurface = IMG_Load(levelSprite);
	
	for(i = 0 ; i < 3; i++){
		srcTextureRect.x = (i + 2) *18;
	    srcTextureRect.y = (levelIndexInformation * 128) + (4 * 16);
	    srcTextureRect.w = 18;
	    srcTextureRect.h = 16;
		burnWallSprite[i] =  SDL_CreateRGBSurface(0, 18, 16, 32, rmask, gmask, bmask, amask);
       	SDL_BlitSurface(tempSurface, &srcTextureRect, burnWallSprite[i], &destTextureRect);
	}
	for(i = 0 ; i < 3; i++){
		srcTextureRect.x = i *18;
	    srcTextureRect.y = (levelIndexInformation * 128) + (5 * 16);
	    srcTextureRect.w = 18;
	    srcTextureRect.h = 16;
		burnWallSprite[i+3] =  SDL_CreateRGBSurface(0, 18, 16, 32, rmask, gmask, bmask, amask);
       	SDL_BlitSurface(tempSurface, &srcTextureRect, burnWallSprite[i+3], &destTextureRect);
	}
	SDL_FreeSurface(tempSurface);
	
	/*
	* PLAYER INFORMATION
	
	
	
	LOAD MINIATURE FOR HEADER SCORE
	*/
	
	
	SDL_Surface *textureBuffer;
	SDL_Rect srcRect;
    srcRect.x = 90;
    srcRect.y = 210;
    srcRect.w = 20;
    srcRect.h = 20;
    SDL_Rect dstRect;
    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = 20;
    dstRect.h = 20;
    
	for(int i = 0; i < 16; i++){
	    switch(playerInformationParam[i][1]){
	    	case 0:
		    	 textureBuffer= IMG_Load(BombermanSprite);
		    	 break;
			case 1:
		    	 textureBuffer= IMG_Load(BombermanSpriteCossak);
		    	 break;
			case 2:
		    	 textureBuffer= IMG_Load(BombermanSpriteBarbar);
		    	 break;
			case 3:
		    	 textureBuffer= IMG_Load(BombermanSpriteChan);
		    	 break;
			case 4:
		    	 textureBuffer= IMG_Load(BombermanSpriteKid);
		    	 break;
			case 5:
		    	 textureBuffer= IMG_Load(BombermanSpritePretty);
		    	 break;
			case 6:
		    	 textureBuffer= IMG_Load(BombermanSpritePunk);
		    	 break;
			case 7:
		    	 textureBuffer= IMG_Load(BombermanSpriteMexican);
		    	 break;
		    default:
		    	textureBuffer= IMG_Load(BombermanSprite);
	    }
    
    	headerPlayerSprite[i] =  SDL_CreateRGBSurface(0, 18, 16, 32, rmask, gmask, bmask, amask);
	    SDL_BlitSurface(textureBuffer, &srcRect, headerPlayerSprite[i], &dstRect);
	   	SDL_FreeSurface(textureBuffer); 
	}
	
	/*
	*	LOAD PLAYER ON GRID
	*/

	for(int i = 0; i < 16; i++){
		int indexTexture = playerInformationParam[i][1];
		float startX = startPlayer[i][0];
		float startY = startPlayer[i][1];
		
		playerType[i] = playerInformationParam[i][0];
		playerIndexTexture[i] = playerInformationParam[i][1];
		playerScore[i] = 0;
		
		Player * player = new Player(&in_keystate[i], false , indexTexture , startX, startY, i, tab, bombeSprite);
		players.push_back(player);
		player = NULL;
	}

    
    //load for time display and generate header
    fragileBombersFont = TTF_OpenFont( "./resources/font/fragile_bombers.ttf", 16); //this opens a font style and sets a size

	generateHeader();
	updateTimeDisplay();
	mergeScreen();
	startGame();
}

/*
* 
* Destructor
* 
*/

Game::~Game(){
	exitGame();
	players.clear();
	bombes.clear();
	explosions.clear();
	burnWalls.clear();
	
	
	
	free(grid);
	free(tab);
	free(tabBonus);
	in_keystate = NULL;

	vout_buf = NULL;
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
	for(int i = 0; i < 6; i++){
		SDL_FreeSurface(burnWallSprite[i]);
	}
	for(int i = 0; i < 16; i++){
		SDL_FreeSurface(headerPlayerSprite[i]);
	}
	
	free(explosionSprite);
	free(bombeSprite);
	free(bonusSprite);
	free(eggsSprite);
	free(burnWallSprite);
	free(headerPlayerSprite);
	
    TTF_CloseFont(fragileBombersFont);
    TTF_Quit();
}

/*
* 
* function that initialise totaly a game
* 
*/
void Game::startGame(){
	if(!isThreadAlive){
		isThreadAlive = true;
		mainThread = SDL_CreateThread(metronome, "mainThread", this);
	}
}

void Game::exitGame(){
	stopGame();
}

/*
* 
* function that kill a Game and the Tick thread
* 
*/
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

/*
* 
* function to copy a furgare inside an other.
* 
*/
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

/*
* 
* function return some information to Bomberman class, for navigation with the previous menu
* 
*/
bool Game::isConfigured(){
	return configured;
}

bool Game::isAlive(){
	return isThreadAlive;
}

bool Game::isRequestStopGame(){
	return requestStopGame;
}

/*
* 
* merge the different layers to generate the game frame
* 
*/
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

/**
	GENERATE HEADER Player Score
*/
void Game::generateHeader(){
	//reset the background
	SDL_Surface * textureBuffer = IMG_Load(background);
	copySurfaceToBackRenderer(textureBuffer, vout_buf, 0, 0);
	SDL_FreeSurface(textureBuffer);
	
	Uint32 rmask, gmask, bmask, amask;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;

	SDL_Surface* surfaceMessage;
	SDL_Color green = {0, 255, 0};
	SDL_Color red = {255, 0, 0};

    SDL_Surface* menu;
	menu =  SDL_CreateRGBSurface(0, 32, 20, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
	
	for(int i = 0; i < 8; i++){
		//shadow rect
		SDL_Rect rect;
	    rect.x = i * 36 + 2;
	    rect.y = 2;
	    rect.w = 32;
	    rect.h = 20;
		SDL_BlitSurface(menu , NULL, vout_buf, &rect);
		
		//copy mini head player
		rect.x = i * 36 + 4;
	    rect.y = 4;
	    rect.w = 20;
	    rect.h = 20;
		SDL_BlitSurface(headerPlayerSprite[i], NULL, vout_buf, &rect);
		
		//wrote number of victory
		char score[3];
    	sprintf(score, "%i", playerScore[i]);
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, score, red);
    	copySurfaceToBackRenderer(surfaceMessage, vout_buf , i * 36 + 22 , 2);
	    SDL_FreeSurface(surfaceMessage);
		
	}
	
	for(int i = 0; i < 8; i++){
		SDL_Rect rect;
	    rect.x = i * 36 + 354;
	    rect.y = 2;
	    rect.w = 32;
	    rect.h = 20;
		SDL_BlitSurface(menu , NULL, vout_buf, &rect);
		
		//copy mini head player
		rect.x = i * 36 + 356;
	    rect.y = 4;
	    rect.w = 20;
	    rect.h = 20;
		SDL_BlitSurface(headerPlayerSprite[i + 8], NULL, vout_buf, &rect);
		
		
		
		//wrote number of victory
		char score[3];
    	sprintf(score, "%i", playerScore[i+8]);
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, score, red);
    	copySurfaceToBackRenderer(surfaceMessage, vout_buf , i * 36 + 374 , 2);
	    SDL_FreeSurface(surfaceMessage);

		
	}
	
	SDL_FreeSurface(menu);
}

/**
* Update the remaining time of a game
* erase a part of the vout_buffer and write remaining time
*/
void Game::updateTimeDisplay(){
	SDL_Rect rect;
    rect.x = 290;
    rect.y = 2;
    rect.w = 60;
    rect.h = 20;
    SDL_FillRect(vout_buf, &rect, 0x000000);
    
    char time[7];
    if(nbTickForGameParam != -1){
    	sprintf(time, "%i", nbTickForGame / 50);
    }else{
    	sprintf(time, "INFINI", nbTickForGame / 50);
    }
    
    SDL_Color green = {0, 255, 0};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, time, green);
	copySurfaceToBackRenderer(surfaceMessage, vout_buf , ((640/2)-(surfaceMessage->w/2)), 2);

    SDL_FreeSurface(surfaceMessage);
}



/*
* 
* Tick : MAIN function of the game, every 20 ms this function is call
* 
* if the game is in pause mode
* 		display the pause message
* else
* 		Check the bombe
*			theck if the bombe remain time is up -> explode
*				generate the explosion
*				generate a wall burn 
*				explode the near bombe
*		Check the explosion
*			if an explosion animation is done -> remove
*		Check the wall burn
* 			if an burning wall animation is done -> remove
*		Check the player 
*			animate player
* 			check if the player request to put a bombe
*/
void Game::tick(){
    
    
    
    switch(gameState){
    
		case gameWait:
			if(in_keystate[0] == 0){
				gameState = gameStart;
			}
			break;
		case gameStart:
			amask = 0xff000000;
		    rmask = 0x00ff0000;
		    gmask = 0x0000ff00;
		    bmask = 0x000000ff;
			if(in_keystate[0] & keyPadStart && !requestStopGame){
				gameState = gamePause;
				break;
			}
			
			if(nbTickForGame == 0){
				for(unsigned int i=0;i<players.size();i++){
	    			if(players[i]->isAlive()){
	    				playerScore[i]++;
	    			}	
	    		}
				gameState = gameEnd;
				break;
			}
			
			if(nbTickForGameParam != -1){
				nbTickForGame--;
			    if(nbTickForGame % 50 == 0){
			        updateTimeDisplay();
			    }	
			}
			
			SDL_FillRect(playerBombeExplode, NULL, SDL_MapRGBA(playerBombeExplode->format, 0, 0, 0, 0));
					
			
			for(unsigned int i=0;i<bombes.size();i++){
				bombes[i] -> tick(playerBombeExplode);
				if(bombes[i]->isExplode()){
					players[bombes[i]->getPlayer()]->ABombeExplode();
					
					/* 
					* generate explosion
					*/
					
					int strenght = bombes[i]->getStrenght();
					int posXBombe = bombes[i]->getPosX();
					int posYBombe = bombes[i]->getPosY();
					
					
					/*
					*DIRECTION
					*/

					bool exitLoop = false;
					int ind = 0;
					
					//CENTER
					explosions.push_back(new Explosion(posXBombe, posYBombe, 0, explosionSprite, tab));
					
					//UP
					for(int j = 1; j < strenght + 1; j++){
						if(exitLoop == true){
							break;	
						}
						
						switch(tab[posXBombe + (posYBombe-j) * sizeX]){
							case 0 :
							case 1 :
								if(j == strenght){
									ind = 1;
								}else{
									ind = 2;
								}
								explosions.push_back(new Explosion(posXBombe, posYBombe - j, ind, explosionSprite, tab));
								break;
							case 2 :
								burnWalls.push_back(new BurnWall(posXBombe, posYBombe - j, ind, burnWallSprite, tab));
								grid->burnAWall(posXBombe, posYBombe - j);
								exitLoop = true;
								break;
							case 3 : 
								for(unsigned int k=0;k<bombes.size();k++){
									if(bombes[k] -> getCase() == posXBombe + (posYBombe - j) * sizeX){
										bombes[k]->explode();
										exitLoop = true;
										break;
									}
								}
								break;
							case 4 :
								exitLoop = true;
								break;
						}
					}
					
					
					//Right
					exitLoop = false;
					for(int j = 1; j < strenght + 1; j++){
						if(exitLoop == true){
							break;	
						}
						
						switch(tab[ (posXBombe + j ) + posYBombe * sizeX]){
							case 0 :
							case 1 :
								if(j == strenght){
									ind = 7;
								}else{
									ind = 8;
								}
								explosions.push_back(new Explosion((posXBombe + j ),posYBombe, ind, explosionSprite, tab));
								break;
							case 2 :
								burnWalls.push_back(new BurnWall((posXBombe + j ), posYBombe, ind, burnWallSprite, tab));
								grid->burnAWall((posXBombe + j ), posYBombe);
								exitLoop = true;
								break;
							case 3 : 
								for(unsigned int k=0;k<bombes.size();k++){
									if(bombes[k] -> getCase() == (posXBombe + j ) + posYBombe * sizeX){
										bombes[k]->explode();
										exitLoop = true;
										break;
									}
								}
								break;
							case 4 :
								exitLoop = true;
								break;	
						}
					}
					
					
					//DOWN
					exitLoop = false;
					for(int j = 1; j < strenght + 1; j++){
						if(exitLoop == true){
							break;	
						}
						
						switch(tab[posXBombe + (posYBombe + j) * sizeX]){
							case 0 :
							case 1 :
								if(j == strenght){
									ind = 5;
								}else{
									ind = 6;
								}
								explosions.push_back(new Explosion(posXBombe, posYBombe + j, ind, explosionSprite, tab));
								break;
							case 2 :
								burnWalls.push_back(new BurnWall(posXBombe, posYBombe + j, ind, burnWallSprite, tab));
								grid->burnAWall(posXBombe, posYBombe + j);
								exitLoop = true;
								break;
							case 3 : 
								for(unsigned int k=0;k<bombes.size();k++){
									if(bombes[k] -> getCase() == posXBombe + (posYBombe + j) * sizeX){
										bombes[k]->explode();
										exitLoop = true;
										break;
									}
								}
								break;
							case 4 :
								exitLoop = true;
								break;
						}
					}
					
					//LEFT
					exitLoop = false;
					for(int j = 1; j < strenght + 1; j++){
						if(exitLoop == true){
							break;	
						}
						
						switch(tab[ (posXBombe - j ) + posYBombe * sizeX]){
							case 0 :
							case 1 :
								if(j == strenght){
									ind = 3;
								}else{
									ind = 4;
								}
								explosions.push_back(new Explosion((posXBombe - j ),posYBombe, ind, explosionSprite, tab));
								break;
							case 2 :
								burnWalls.push_back(new BurnWall((posXBombe - j ), posYBombe, ind, burnWallSprite, tab));
								grid->burnAWall((posXBombe - j ), posYBombe);
								exitLoop = true;
								break;
							case 3 : 
								for(unsigned int k=0;k<bombes.size();k++){
									if(bombes[k] -> getCase() == (posXBombe - j ) + posYBombe * sizeX){
										bombes[k]->explode();
										exitLoop = true;
										break;
									}
								}
								break;
							case 4 :
								exitLoop = true;
								break;	
						}
					}



					bombes.erase(bombes.begin()+i);
				}
			}
			
			for(unsigned int i=0 ; i< explosions.size() ; i++){
				explosions[i] -> tick(playerBombeExplode);
				if(explosions[i]->canBeDelete()){
					explosions.erase(explosions.begin()+i);
				}
			}
			
			for(unsigned int i=0 ; i< burnWalls.size() ; i++){
				burnWalls[i] -> tick(playerBombeExplode);
				if(burnWalls[i]->canBeDelete()){
					burnWalls.erase(burnWalls.begin()+i);
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
			break;
			
			
		case gamePause:
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
				gameState = gameStart;
			}
			if(in_keystate[0] & keyPadB && !requestStopGame){
				requestStopGame = true;
				gameState = gameWait;
			}
		break;
    	case gameEnd:    	
    		//restart game
    		if(in_keystate[0] & keyPadStart){
				gameState = gameWait;
				generateHeader();
				nbTickForGame = nbTickForGameParam;
			
    			players.clear();
    			bombes.clear();
				explosions.clear();
				burnWalls.clear();
				grid -> resetSurface();
				grid -> generateGrid();
				
	    		for(int i = 0; i < 16; i++){
					float startX = startPlayer[i][0];
					float startY = startPlayer[i][1];
					Player * player = new Player(&in_keystate[i], false , playerIndexTexture[i] , startX, startY, i, tab, bombeSprite);
					players.push_back(player);
					player = NULL;
				}
			}
    		break;
	}
}
