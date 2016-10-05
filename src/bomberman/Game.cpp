#include "Game.h"

#define gameTick 20

Uint32 rmask, gmask, bmask, amask;
const static char *misc = "./resources/sprite/other/Misc.png";
const static char *fireSprite = "./resources/sprite/other/Fire.png";
const static char *levelSprite = "./resources/sprite/level/AllLevel.png";
const static char *background = "./resources/image/EmptyBackground.png";

const static char *fireSoundPath = "./resources/sound/fire.wav";
const static char *LouisSoundPath = "./resources/sound/louis.wav";
const static char *bounceSoundPath = "./resources/sound/bounce.wav";
const static char *playerBurnSoundPath = "./resources/sound/burn.wav";
const static char *playerKickSoundPath = "./resources/sound/kick.wav";
const static char *endSoundPath = "./resources/sound/end.wav";

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

static int metronome(void* data) {
	Game *game = ((Game *) data);
	struct timeval t1, t2;

	long delay;
	while (game->isAlive()) {
		gettimeofday(&t1, NULL);
		game->tick();
		gettimeofday(&t2, NULL);

		long milliseconds = (t2.tv_usec - t1.tv_usec) / 1000;
		if (milliseconds < 0) {
			milliseconds += 1000;
		}

		delay = gameTick - milliseconds;

		if (delay > 0) {
			//fprintf(stderr, "%i\n", (int)delay);
			SDL_Delay(delay);
		} else {
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

Game::Game() {
	gameState = gameWait;
	//declarativ color mask, used for create a RGB surface
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	playerBombeExplode = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	screenBuffer = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	overlay = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	overlayResult = SDL_CreateRGBSurface(0, 250, 200, 32, rmask, gmask, bmask, amask);
	isThreadAlive = false;
	configured = false;
	requestStopGame = false;
	tab = new int[sizeX * sizeY];
	tabBonus = new int[sizeX * sizeY];
	tabPlayerCoord = new float[16 * 2];
}

/*
 * 
 * constructor
 * 
 */
Game::Game(int levelIndexInformation, int playerInformationParam[16][2], int gameOption[4], SDL_Surface * vout_bufLibretro, unsigned short * in_keystateLibretro, int nbPlayerConfigGame) {
	gameState = gameWait;
	nbPlayerInGame = 0;
	nbPlayerConfig = nbPlayerConfigGame;
	// Load Font
	fragileBombersFont = TTF_OpenFont("./resources/font/fragile_bombers.ttf", 36); //this opens a font style and sets a size

	// declarativ color mask, used for create a RGB surface
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	// create overlay for pause / exit game
	overlay = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(overlay, NULL, SDL_MapRGBA(overlay->format, 0, 0, 0, 120));
	SDL_Color green = { 0, 255, 0 };
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "PAUSE", green);
	copySurfaceToBackRenderer(surfaceMessage, overlay, ((640 / 2) - (surfaceMessage->w / 2)), 114);
	SDL_FreeSurface(surfaceMessage);
	surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "Press X/B for exit game", green);
	copySurfaceToBackRenderer(surfaceMessage, overlay, ((640 / 2) - (surfaceMessage->w / 2)), 154);
	SDL_FreeSurface(surfaceMessage);
	surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "Press Select for exit pause", green);
	copySurfaceToBackRenderer(surfaceMessage, overlay, ((640 / 2) - (surfaceMessage->w / 2)), 194);
	SDL_FreeSurface(surfaceMessage);
	TTF_CloseFont (fragileBombersFont);

	fireSound = Mix_LoadWAV(fireSoundPath);
	louisSound = Mix_LoadWAV(LouisSoundPath);
	bombeBounceSound = Mix_LoadWAV(bounceSoundPath);
	playerBurnSound = Mix_LoadWAV(playerBurnSoundPath);
	playerKickSound = Mix_LoadWAV(playerKickSoundPath);
	endSound = Mix_LoadWAV(endSoundPath);

	Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
	Mix_AllocateChannels(6);

	// init variable or surface
	screenBuffer = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	playerBombeExplode = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	overlayResult = SDL_CreateRGBSurface(0, 250, 200, 32, rmask, gmask, bmask, amask);

	isThreadAlive = false;
	configured = true;
	requestStopGame = false;
	tab = new int[sizeX * sizeY];
	tabBonus = new int[sizeX * sizeY];
	tabPlayerCoord = new float[16 * 2];
	for (int i = 0; i < sizeX * sizeY; i++) {
		tab[i] = emptyElement;
		tabBonus[i] = -1;
	}
	for (int i = 0; i < 16 * 2; i++) {
		tabPlayerCoord[i] = -1.0;
	}

	levelIndex = levelIndexInformation;
	if (gameOption[0] == 1) {
		suddenDeath = true;
	} else {
		suddenDeath = false;
	}

	if (gameOption[1] == 1) {
		badBomber = true;
	} else {
		badBomber = false;
	}

	cpuLevel = gameOption[2];
	if (gameOption[3] != -1) {
		nbTickForGame = gameOption[3] * 50 * 60;
		nbTickForGameParam = nbTickForGame;
	} else {
		nbTickForGame = gameOption[3];
		nbTickForGameParam = nbTickForGame;
	}

	vout_buf = vout_bufLibretro;

	/*
	 * LOAD MISC IMAGE : Bombe animation, explosion animation, bonus image, eggs image
	 */

	explosionSprite = new SDL_Surface *[36];
	bombeSprite = new SDL_Surface *[12];
	bonusSprite = new SDL_Surface *[14];
	eggsSprite = new SDL_Surface *[2];
	burnWallSprite = new SDL_Surface *[6];
	headerPlayerSprite = new SDL_Surface *[16];

	SDL_Surface * tempSurface = IMG_Load(fireSprite);
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = 16;
	destTextureRect.h = 16;

	int i = 0;
	int j = 0;
	// explosionSprite
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 9; j++) {
			srcTextureRect.x = i * 18;
			srcTextureRect.y = j * 16;
			srcTextureRect.w = 18;
			srcTextureRect.h = 16;
			explosionSprite[i + (j * 4)] = SDL_CreateRGBSurface(0, 18, 16, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(tempSurface, &srcTextureRect, explosionSprite[i + (j * 4)], &destTextureRect);
		}
	}

	SDL_FreeSurface(tempSurface);
	tempSurface = IMG_Load(misc);

	// bonusSprite
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 7; j++) {
			srcTextureRect.x = i * 16;
			srcTextureRect.y = j * 16;
			srcTextureRect.w = 16;
			srcTextureRect.h = 16;
			bonusSprite[i + (j * 2)] = SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(tempSurface, &srcTextureRect, bonusSprite[i + (j * 2)], &destTextureRect);
		}
	}

	grid = new Grid(levelIndexInformation, tab, tabBonus, bonusSprite);

	// bombeSprite
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 4; j++) {
			srcTextureRect.x = (i + 2) * 16;
			srcTextureRect.y = j * 16;
			srcTextureRect.w = 16;
			srcTextureRect.h = 16;
			bombeSprite[i + (j * 3)] = SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(tempSurface, &srcTextureRect, bombeSprite[i + (j * 3)], &destTextureRect);
		}
	}

	// eggsSprite
	for (i = 0; i < 2; i++) {
		srcTextureRect.x = i * 16;
		srcTextureRect.y = 4 * 16;
		srcTextureRect.w = 16;
		srcTextureRect.h = 16;
		eggsSprite[i] = SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(tempSurface, &srcTextureRect, eggsSprite[i], &destTextureRect);
	}
	SDL_FreeSurface(tempSurface);

	// Load Surface for burn wall animation
	// levelIndexInformation
	tempSurface = IMG_Load(levelSprite);

	for (i = 0; i < 3; i++) {
		srcTextureRect.x = (i + 2) * 18;
		srcTextureRect.y = (levelIndexInformation * 128) + (4 * 16);
		srcTextureRect.w = 18;
		srcTextureRect.h = 16;
		burnWallSprite[i] = SDL_CreateRGBSurface(0, 18, 16, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(tempSurface, &srcTextureRect, burnWallSprite[i], &destTextureRect);
	}
	for (i = 0; i < 3; i++) {
		srcTextureRect.x = i * 18;
		srcTextureRect.y = (levelIndexInformation * 128) + (5 * 16);
		srcTextureRect.w = 18;
		srcTextureRect.h = 16;
		burnWallSprite[i + 3] = SDL_CreateRGBSurface(0, 18, 16, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(tempSurface, &srcTextureRect, burnWallSprite[i + 3], &destTextureRect);
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

	for (int i = 0; i < 16; i++) {
		switch (playerInformationParam[i][1]) {
			case 0:
				textureBuffer = IMG_Load(BombermanSprite);
				break;
			case 1:
				textureBuffer = IMG_Load(BombermanSpriteCossak);
				break;
			case 2:
				textureBuffer = IMG_Load(BombermanSpriteBarbar);
				break;
			case 3:
				textureBuffer = IMG_Load(BombermanSpriteChan);
				break;
			case 4:
				textureBuffer = IMG_Load(BombermanSpriteKid);
				break;
			case 5:
				textureBuffer = IMG_Load(BombermanSpritePretty);
				break;
			case 6:
				textureBuffer = IMG_Load(BombermanSpritePunk);
				break;
			case 7:
				textureBuffer = IMG_Load(BombermanSpriteMexican);
				break;
			default:
				textureBuffer = IMG_Load(BombermanSprite);
		}

		headerPlayerSprite[i] = SDL_CreateRGBSurface(0, 18, 16, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(textureBuffer, &srcRect, headerPlayerSprite[i], &dstRect);
		SDL_FreeSurface(textureBuffer);
	}

	/*
	 *	LOAD PLAYER ON GRID
	 */

	//in_keystate = in_keystateLibretro;
	in_keystate_cpu = new unsigned short[16];
	for (int i = 0; i < 16; i++) {
		in_keystate_cpu[i] = 0;
	}
	in_keystate = in_keystateLibretro;
	int indexLibretro = 0;
	int index = 0;
	Player * player;
	Brain * brain;

	for (int i = 0; i < 16; i++) {

		int indexTexture = playerInformationParam[i][1];
		float startX = startPlayer[i][0];
		float startY = startPlayer[i][1];

		playerType[i] = playerInformationParam[i][0];
		playerIndexTexture[i] = playerInformationParam[i][1];
		playerScore[i] = 0;

		switch (playerType[i]) {
			case HUMAN:

				// if a human link the next keystate of libretro, else link a empty value
				player = new Player(&in_keystate[indexLibretro], false, indexTexture, startX, startY, i, tab, tabBonus, bombeSprite, grid, tabPlayerCoord, nbPlayerConfig, louisSound,
						playerKickSound, playerBurnSound, bombeBounceSound, index);
				players.push_back(player);
				player = NULL;
				indexLibretro++;
				nbPlayerAlive++;
				index++;
				break;
			case CPU:
				player = new Player(&in_keystate_cpu[index], true, indexTexture, startX, startY, i, tab, tabBonus, bombeSprite, grid, tabPlayerCoord, nbPlayerConfig, louisSound,
						playerKickSound, playerBurnSound, bombeBounceSound, index);
				players.push_back(player);
				player = NULL;
				
				brain = new Brain(&in_keystate_cpu[index], tab, tabPlayerCoord, nbPlayerConfig, i, floor(startX), floor(startX));
				brains.push_back(brain);
				brain = NULL;
				
				in_keystate[index] = 0;
				index++;
				nbPlayerAlive++;
				index++;
				break;
			case OFF:
				break;
		}


	}

	//load for time display and generate header
	fragileBombersFont = TTF_OpenFont("./resources/font/fragile_bombers.ttf", 16); //this opens a font style and sets a size

	suddenDeathCase = false;
	generateHeader();
	updateTimeDisplay();
	mergeScreen(false);
	startGame();
}

/*
 * 
 * Destructor
 * 
 */

Game::~Game() {
	exitGame();
	players.clear();
	bombes.clear();
	explosions.clear();
	burnWalls.clear();
	brains.clear();

	free (grid);
	free (tab);
	free (tabBonus);
	free (tabPlayerCoord);
	in_keystate = NULL;
	free (in_keystate_cpu);

	Mix_FreeChunk (fireSound);
	Mix_FreeChunk (louisSound);
	Mix_FreeChunk (bombeBounceSound);
	Mix_FreeChunk (playerBurnSound);
	Mix_FreeChunk (playerKickSound);
	Mix_FreeChunk (endSound);

	vout_buf = NULL;
	SDL_FreeSurface (overlay);
	SDL_FreeSurface (overlayResult);
	SDL_FreeSurface (screenBuffer);
	SDL_FreeSurface (playerBombeExplode);

	for (int i = 0; i < 36; i++) {
		SDL_FreeSurface (explosionSprite[i]);
	}
	for (int i = 0; i < 12; i++) {
		SDL_FreeSurface (bombeSprite[i]);
	}
	for (int i = 0; i < 12; i++) {
		SDL_FreeSurface (bonusSprite[i]);
	}
	for (int i = 0; i < 2; i++) {
		SDL_FreeSurface (eggsSprite[i]);
	}
	for (int i = 0; i < 6; i++) {
		SDL_FreeSurface (burnWallSprite[i]);
	}
	for (int i = 0; i < 16; i++) {
		SDL_FreeSurface (headerPlayerSprite[i]);
	}

	free (explosionSprite);
	free (bombeSprite);
	free (bonusSprite);
	free (eggsSprite);
	free (burnWallSprite);
	free (headerPlayerSprite);

	TTF_CloseFont (fragileBombersFont);
	TTF_Quit();
}

/*
 * 
 * function that initialise totaly a game
 * 
 */
void Game::startGame() {
	if (!isThreadAlive) {
		isThreadAlive = true;
		mainThread = SDL_CreateThread(metronome, "mainThread", this);
	}
}

void Game::exitGame() {
	stopGame();
}

/*
 * 
 * function that kill a Game and the Tick thread
 * 
 */
void Game::stopGame() {
	if (isThreadAlive) {
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
void Game::copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y) {
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
bool Game::isConfigured() {
	return configured;
}

bool Game::isAlive() {
	return isThreadAlive;
}

bool Game::isRequestStopGame() {
	return requestStopGame;
}

/*
 * 
 * merge the different layers to generate the game frame
 * 
 */
void Game::mergeScreen(bool mergeResult) {
	SDL_Rect mergeRect;
	mergeRect.x = 0;
	mergeRect.y = 0;
	mergeRect.w = 630;
	mergeRect.h = 336;
	SDL_BlitSurface(grid->getGroundLayer(), &mergeRect, screenBuffer, &mergeRect);
	SDL_BlitSurface(grid->getBricksLayer(), &mergeRect, screenBuffer, &mergeRect);
	SDL_BlitSurface(playerBombeExplode, &mergeRect, screenBuffer, &mergeRect);
	SDL_BlitSurface(grid->getSkyLayer(), &mergeRect, screenBuffer, &mergeRect);
	if (mergeResult) {
		mergeRect.x = (630 / 2) - (overlayResult->w / 2);
		mergeRect.y = (360 / 2) - (overlayResult->h / 2);
		mergeRect.w = overlayResult->w;
		mergeRect.h = overlayResult->h;
		SDL_BlitSurface(overlayResult, NULL, screenBuffer, &mergeRect);
	}
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 5, 24);
}

/**
 * 
 * DRAW RESULT TABLE OVER THE GAME
 *
 */

void Game::drawResultOfGame() {
	SDL_FillRect(overlayResult, NULL, SDL_MapRGBA(overlayResult->format, 0, 0, 0, 120));
	SDL_Surface* surfaceMessage;
	SDL_Color green = { 0, 255, 0 };
	SDL_Color red = { 255, 0, 0 };
	char mess[18];
	int x = 25;
	int y = 20;
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i]->isAlive()) {
			sprintf(mess, "Player %i : ALIVE", i + 1);
			surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, mess, green);
		} else {
			sprintf(mess, "Player %i : DEAD", i + 1);
			surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, mess, red);
		}

		copySurfaceToBackRenderer(surfaceMessage, overlayResult, x, y);
		SDL_FreeSurface(surfaceMessage);
		y = y + 20;
		if (y > 170) {
			y = 20;
			x = 145;
		}
	}
}

/**
 GENERATE HEADER Player Score
 */
void Game::generateHeader() {
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
	SDL_Color red = { 255, 0, 0 };

	SDL_Surface* menu;
	menu = SDL_CreateRGBSurface(0, 32, 20, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));

	for (int i = 0; i < 8; i++) {
		//shadow rect
		SDL_Rect rect;
		rect.x = i * 36 + 2;
		rect.y = 2;
		rect.w = 32;
		rect.h = 20;
		SDL_BlitSurface(menu, NULL, vout_buf, &rect);

		//copy mini head player
		rect.x = i * 36 + 4;
		rect.y = 4;
		rect.w = 20;
		rect.h = 20;
		SDL_BlitSurface(headerPlayerSprite[i], NULL, vout_buf, &rect);

		//wrote number of victory
		char score[3];
		sprintf(score, "%i", playerScore[i]);
		surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, score, red);
		copySurfaceToBackRenderer(surfaceMessage, vout_buf, i * 36 + 22, 2);
		SDL_FreeSurface(surfaceMessage);

	}

	for (int i = 0; i < 8; i++) {
		SDL_Rect rect;
		rect.x = i * 36 + 354;
		rect.y = 2;
		rect.w = 32;
		rect.h = 20;
		SDL_BlitSurface(menu, NULL, vout_buf, &rect);

		//copy mini head player
		rect.x = i * 36 + 356;
		rect.y = 4;
		rect.w = 20;
		rect.h = 20;
		SDL_BlitSurface(headerPlayerSprite[i + 8], NULL, vout_buf, &rect);

		//wrote number of victory
		char score[3];
		sprintf(score, "%i", playerScore[i + 8]);
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, score, red);
		copySurfaceToBackRenderer(surfaceMessage, vout_buf, i * 36 + 374, 2);
		SDL_FreeSurface(surfaceMessage);

	}

	SDL_FreeSurface(menu);
}

/**
 * Update the remaining time of a game
 * erase a part of the vout_buffer and write remaining time
 */
void Game::updateTimeDisplay() {
	SDL_Rect rect;
	rect.x = 290;
	rect.y = 2;
	rect.w = 60;
	rect.h = 20;
	SDL_FillRect(vout_buf, &rect, 0x000000);

	char time[7];
	if (nbTickForGameParam != -1) {
		sprintf(time, "%i", nbTickForGame / 50);
	} else {
		sprintf(time, "INFINI");
	}

	SDL_Color green = { 0, 255, 0 };
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, time, green);
	copySurfaceToBackRenderer(surfaceMessage, vout_buf, ((640 / 2) - (surfaceMessage->w / 2)), 2);

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
void Game::tick() {

	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;

	switch (gameState) {

		case gameWait:
			if (in_keystate[0] == 0) {
				gameState = gameStart;
			}
			break;
		case gameStart:

			nbPlayerAlive = 0;
			if (in_keystate[0] & keyPadStart && !requestStopGame) {
				gameState = gamePause;
				break;
			}

			if (nbTickForGame == 0) {
				for (unsigned int i = 0; i < players.size(); i++) {
					if (players[i]->isAlive()) {
						playerScore[i]++;
						players[i]->winTheGame();
					}
				}
				gameState = generateResult;
				break;
			}

			/*
			 *
			 *	UPDATE TIME DISPLAY
			 *
			 */
			if (nbTickForGameParam != -1) {
				nbTickForGame--;
				if (nbTickForGame % 50 == 0) {
					updateTimeDisplay();
				}
			}
			SDL_FillRect(playerBombeExplode, NULL, SDL_MapRGBA(playerBombeExplode->format, 0, 0, 0, 0));

			/*
			 *
			 *	GAME PART : BOMBE
			 *
			 */
			for (unsigned int i = 0; i < bombes.size(); i++) {
				bombes[i]->tick(playerBombeExplode);
				if (bombes[i]->isExplode()) {

					Mix_PlayChannel(0, fireSound, 0);

					players[bombes[i]->getPlayer()]->ABombeExplode();

					/* 
					 * generate explosion
					 */

					int strenght = bombes[i]->getStrenght();
					int posXBombe = bombes[i]->getPosX();
					int posYBombe = bombes[i]->getPosY();
					bool isAPowerBombe = bombes[i]->isPowerBombe();

					/*
					 *DIRECTION
					 */

					bool exitLoop = false;
					bool aWallHasBurn = false;
					int ind = 0;

					//CENTER
					explosions.push_back(new Explosion(posXBombe, posYBombe, 0, explosionSprite, tab, tabBonus));
					if (tabBonus[posXBombe + posYBombe * sizeX] != -1) {
						grid->burnBonus(posXBombe, posYBombe);
					}

					//UP
					for (int j = 1; j < strenght + 1; j++) {
						if (exitLoop == true) {
							break;
						}

						//explostion and wall
						switch (tab[posXBombe + (posYBombe - j) * sizeX]) {
							case emptyElement:
							case explosionElement:
								if (j == strenght) {
									ind = 1;
								} else {
									ind = 2;
								}
								explosions.push_back(new Explosion(posXBombe, posYBombe - j, ind, explosionSprite, tab, tabBonus));
								break;
							case brickElement:
								burnWalls.push_back(new BurnWall(posXBombe, posYBombe - j, ind, burnWallSprite, tab, tabBonus));
								grid->burnABrick(posXBombe, posYBombe - j);
								if (!isAPowerBombe) {
									exitLoop = true;
								}
								aWallHasBurn = true;
								break;
							case bombeElement:
								for (unsigned int k = 0; k < bombes.size(); k++) {
									if (bombes[k]->getCase() == posXBombe + (posYBombe - j) * sizeX) {
										bombes[k]->explode();
										if (!isAPowerBombe) {
											exitLoop = true;
										}
										break;
									}
								}
								break;
							case wallElement:
								exitLoop = true;
								break;
						}
						if (!aWallHasBurn) {
							// if we don't have burn a wall, we can have a bonus in the case of table. we remove it !
							if (tabBonus[posXBombe + (posYBombe - j) * sizeX] != -1) {
								grid->burnBonus(posXBombe, posYBombe - j);
							}
						}
					}

					//Right
					exitLoop = false;
					aWallHasBurn = false;
					for (int j = 1; j < strenght + 1; j++) {
						if (exitLoop == true) {
							break;
						}

						switch (tab[(posXBombe + j) + posYBombe * sizeX]) {
							case emptyElement:
							case explosionElement:
								if (j == strenght) {
									ind = 7;
								} else {
									ind = 8;
								}
								explosions.push_back(new Explosion((posXBombe + j), posYBombe, ind, explosionSprite, tab, tabBonus));
								break;
							case brickElement:
								burnWalls.push_back(new BurnWall((posXBombe + j), posYBombe, ind, burnWallSprite, tab, tabBonus));
								grid->burnABrick((posXBombe + j), posYBombe);
								if (!isAPowerBombe) {
									exitLoop = true;
								}
								aWallHasBurn = true;
								break;
							case bombeElement:
								for (unsigned int k = 0; k < bombes.size(); k++) {
									if (bombes[k]->getCase() == (posXBombe + j) + posYBombe * sizeX) {
										bombes[k]->explode();
										if (!isAPowerBombe) {
											exitLoop = true;
										}
										break;
									}
								}
								break;
							case wallElement:
								exitLoop = true;
								break;
						}
						if (!aWallHasBurn) {
							// if we don't have burn a wall, we can have a bonus in the case of table. we remove it !
							if (tabBonus[(posXBombe + j) + posYBombe * sizeX] != -1) {
								grid->burnBonus((posXBombe + j), posYBombe);
							}
						}
					}

					//DOWN
					exitLoop = false;
					aWallHasBurn = false;
					for (int j = 1; j < strenght + 1; j++) {
						if (exitLoop == true) {
							break;
						}

						switch (tab[posXBombe + (posYBombe + j) * sizeX]) {
							case emptyElement:
							case explosionElement:
								if (j == strenght) {
									ind = 5;
								} else {
									ind = 6;
								}
								explosions.push_back(new Explosion(posXBombe, posYBombe + j, ind, explosionSprite, tab, tabBonus));
								break;
							case brickElement:
								burnWalls.push_back(new BurnWall(posXBombe, posYBombe + j, ind, burnWallSprite, tab, tabBonus));
								grid->burnABrick(posXBombe, posYBombe + j);
								if (!isAPowerBombe) {
									exitLoop = true;
								}
								aWallHasBurn = true;
								break;
							case bombeElement:
								for (unsigned int k = 0; k < bombes.size(); k++) {
									if (bombes[k]->getCase() == posXBombe + (posYBombe + j) * sizeX) {
										bombes[k]->explode();
										if (!isAPowerBombe) {
											exitLoop = true;
										}
										break;
									}
								}
								break;
							case wallElement:
								exitLoop = true;
								break;
						}
						if (!aWallHasBurn) {
							// if we don't have burn a wall, we can have a bonus in the case of table. we remove it !
							if (tabBonus[posXBombe + (posYBombe + j) * sizeX] != -1) {
								grid->burnBonus(posXBombe, posYBombe + j);
							}
						}

					}

					//LEFT
					exitLoop = false;
					aWallHasBurn = false;
					for (int j = 1; j < strenght + 1; j++) {
						if (exitLoop == true) {
							break;
						}

						switch (tab[(posXBombe - j) + posYBombe * sizeX]) {
							case emptyElement:
							case explosionElement:
								if (j == strenght) {
									ind = 3;
								} else {
									ind = 4;
								}
								explosions.push_back(new Explosion((posXBombe - j), posYBombe, ind, explosionSprite, tab, tabBonus));
								break;
							case brickElement:
								burnWalls.push_back(new BurnWall((posXBombe - j), posYBombe, ind, burnWallSprite, tab, tabBonus));
								grid->burnABrick((posXBombe - j), posYBombe);
								if (!isAPowerBombe) {
									exitLoop = true;
								}
								break;
							case bombeElement:
								for (unsigned int k = 0; k < bombes.size(); k++) {
									if (bombes[k]->getCase() == (posXBombe - j) + posYBombe * sizeX) {
										bombes[k]->explode();
										if (!isAPowerBombe) {
											exitLoop = true;
										}
										aWallHasBurn = true;
										break;
									}
								}
								break;
							case wallElement:
								exitLoop = true;
								break;
						}
						if (!aWallHasBurn) {
							// if we don't have burn a wall, we can have a bonus in the case of table. we remove it !
							if (tabBonus[(posXBombe - j) + posYBombe * sizeX] != -1) {
								grid->burnBonus((posXBombe - j), posYBombe);
							}
						}
					}

					bombes.erase(bombes.begin() + i);
				}
			}
			/*
			 *
			 *	GAME PART : EXPLOSION
			 *
			 */
			for (unsigned int i = 0; i < explosions.size(); i++) {
				explosions[i]->tick(playerBombeExplode);
				if (explosions[i]->canBeDelete()) {
					explosions.erase(explosions.begin() + i);
				}
			}

			/*
			 *
			 *	GAME PART : BURNING WALL
			 *
			 */
			for (unsigned int i = 0; i < burnWalls.size(); i++) {
				burnWalls[i]->tick(playerBombeExplode);
				if (burnWalls[i]->canBeDelete()) {
					burnWalls.erase(burnWalls.begin() + i);
				}
			}

			/*
			*
			*	BRAIN THINK'S
			*
			*/
			for (unsigned int i = 0; i < brains.size(); i++) {
				brains[i]->think();
			}


			/*
			 *
			 *	GAME PART : PLAYERS
			 *
			 */
			for (unsigned int i = 0; i < players.size(); i++) {
				players[i]->doSomething(playerBombeExplode);
				if (players[i]->isAlive()) {
					nbPlayerAlive++;
				}
				if (!players[i]->walkOnWall()) {
					if (players[i]->wantPutBombe()) {
						bombes.push_back(players[i]->addBombe());
						players[i]->ABombeIsSet();
					}
				}
				if (players[i]->triggerPowerBombe()) {
					for (unsigned int j = 0; j < bombes.size(); j++) {
						if (bombes[j]->getPlayer() == players[i]->getPlayerNumber()) {
							bombes[j]->explodeNow();
						}
					}
					players[i]->releaseTrigger();
				}

				//kick treatment				
				if (players[i]->hasKickPower() && players[i]->isKickingBombe() != -1) {
					for (unsigned int j = 0; j < bombes.size(); j++) {
						if (bombes[j]->getCase() == players[i]->isKickingBombe()) {
							bombes[j]->pushBomb(players[i]->getKickDirection());
							players[i]->releaseKick();
						}
					}
				}
			}

			if (nbPlayerAlive <= 1) {
				for (unsigned int i = 0; i < players.size(); i++) {
					if (players[i]->isAlive()) {
						playerScore[i]++;
						players[i]->winTheGame();
					}
				}
				gameState = generateResult;
			}

			/*
			 *
			 *	SUDDEN DEATH PART !
			 *
			 */
			if (!suddenDeathCase) {
				if (suddenDeath && (nbTickForGame == (4 * (sizeX - 2) * (sizeY - 2)) + (34 * 1))) {
					fprintf(stderr, "init sudden death");
					suddenDeathCase = true;
					for (unsigned int i = 0; i < players.size(); i++) {
						//notify to players that the suddenDeath
						players[i]->itSuddenDeathTime();
					}
					suddenDeathMinX = 1;
					suddenDeathMaxX = 33;
					suddenDeathMinY = 1;
					suddenDeathMaxY = 19;
					suddenDeathX = 1;
					suddenDeathY = 1;
					suddenDeathDirection = suddenDeathRight;
				}
			} else {
				if (nbTickForGame % 4 == 0) {
					//add animation of falling wall
					if (tab[suddenDeathX + suddenDeathY * sizeX] < wallElement) {
						SuddenDeathAnimation * suddenDeathAnimation = new SuddenDeathAnimation(suddenDeathX, suddenDeathY, grid->getWallSprite(), tab, grid);
						suddenDeathAnimations.push_back(suddenDeathAnimation);
						suddenDeathAnimation = NULL;
					}

					switch (suddenDeathDirection) {
						case suddenDeathRight:
							if (suddenDeathX < suddenDeathMaxX) {
								suddenDeathX++;
							} else {
								suddenDeathMinY++;
								suddenDeathY++;
								suddenDeathDirection = suddenDeathDown;
							}
							break;

						case suddenDeathDown:
							if (suddenDeathY < suddenDeathMaxY) {
								suddenDeathY++;
							} else {
								suddenDeathMaxX--;
								suddenDeathX--;
								suddenDeathDirection = suddenDeathLeft;
							}
							break;

						case suddenDeathLeft:
							if (suddenDeathX > suddenDeathMinX) {
								suddenDeathX--;
							} else {
								suddenDeathMaxY--;
								suddenDeathY--;
								suddenDeathDirection = suddenDeathUp;
							}
							break;

						case suddenDeathUp:
							if (suddenDeathY > suddenDeathMinY) {
								suddenDeathY--;
							} else {
								suddenDeathMinX++;
								suddenDeathX++;
								suddenDeathDirection = suddenDeathRight;
							}
							break;

					}

				}
//				purge old animation
				for (unsigned int i = 0; i < suddenDeathAnimations.size(); i++) {
					if (suddenDeathAnimations[i]->canBeDeleted()) {
						suddenDeathAnimations.erase(suddenDeathAnimations.begin() + i);
					}
					suddenDeathAnimations[i]->tick(playerBombeExplode);
				}
			}

			mergeScreen(false);

			break;

		case gamePause:
			SDL_Rect mergeRect;
			mergeRect.x = 0;
			mergeRect.y = 0;
			mergeRect.w = 630;
			mergeRect.h = 336;
			SDL_BlitSurface(grid->getGroundLayer(), &mergeRect, screenBuffer, &mergeRect);
			SDL_BlitSurface(grid->getBricksLayer(), &mergeRect, screenBuffer, &mergeRect);
			SDL_BlitSurface(playerBombeExplode, &mergeRect, screenBuffer, &mergeRect);
			SDL_BlitSurface(grid->getSkyLayer(), &mergeRect, screenBuffer, &mergeRect);
			SDL_BlitSurface(overlay, &mergeRect, screenBuffer, &mergeRect);
			copySurfaceToBackRenderer(screenBuffer, vout_buf, 5, 24);
			if (in_keystate[0] & keyPadSelect && !requestStopGame) {
				gameState = gameStart;
			}
			if (in_keystate[0] & keyPadB && !requestStopGame) {
				requestStopGame = true;
				gameState = gameWait;
			}
			break;
		case generateResult:
			Mix_PlayChannel(5, endSound, 0);
			drawResultOfGame();
			gameState = gameEnd;
		case gameEnd:

			SDL_FillRect(playerBombeExplode, NULL, SDL_MapRGBA(playerBombeExplode->format, 0, 0, 0, 0));
			for (unsigned int i = 0; i < players.size(); i++) {
				players[i]->doSomething(playerBombeExplode);
			}
			mergeScreen(true);

			//restart game
			if (in_keystate[0] & keyPadStart) {
				for (int i = 0; i < 16 * 2; i++) {
					tabPlayerCoord[i] = -1.0;
				}

				gameState = gameWait;
				generateHeader();
				nbTickForGame = nbTickForGameParam;
				suddenDeathCase = false;
				players.clear();
				bombes.clear();
				explosions.clear();
				burnWalls.clear();
				suddenDeathAnimations.clear();
				brains.clear();
				grid->resetSurface();
				grid->generateGrid();

				int indexLibretro = 0;
				int index = 0;
				nbPlayerInGame = 0;

				Player * player;
				Brain * brain;

				for (int i = 0; i < 16; i++) {
					float startX = startPlayer[i][0];
					float startY = startPlayer[i][1];
					switch (playerType[i]) {
						case HUMAN:
							// if a human link the next keystate of libretro, else link a empty value
							player = new Player(&in_keystate[indexLibretro], false, playerIndexTexture[i], startX, startY, i, tab, tabBonus, bombeSprite, grid, tabPlayerCoord,
									nbPlayerConfig, louisSound, playerKickSound, playerBurnSound, bombeBounceSound, index);
							players.push_back(player);
							player = NULL;
							indexLibretro++;
							nbPlayerInGame++;
							index++;
							break;
						case CPU:
							player = new Player(&in_keystate_cpu[index], true, playerIndexTexture[i], startX, startY, i, tab, tabBonus, bombeSprite, grid, tabPlayerCoord, nbPlayerConfig,
									louisSound, playerKickSound, playerBurnSound, bombeBounceSound, index);
							players.push_back(player);
							brain = new Brain(&in_keystate_cpu[index], tab, tabPlayerCoord, nbPlayerConfig, i, floor(startX), floor(startX));
							brains.push_back(brain);
							brain = NULL;
							
							player = NULL;
							in_keystate[index] = 0;
							index++;
							nbPlayerInGame++;
							index++;
							break;
						case OFF:
							break;
					}
				}
			}
			break;
	}
}

