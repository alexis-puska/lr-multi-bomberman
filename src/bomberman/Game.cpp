#include "Game.h"

#define gameTick 20

Uint32 rmask, gmask, bmask, amask;

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
			fprintf(stderr, "%i\n", (int) delay);
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
Game::Game(int levelIndex, int playerInformationParam[16][2], int gameOption[4], SDL_Surface * vout_buf, unsigned short * in_keystate, int nbPlayerConfig) {
	srand (time(NULL));this->nbPlayerConfig = nbPlayerConfig;
	this->vout_buf = vout_buf;
	this->in_keystate = in_keystate;
	this->levelIndex = levelIndex;

	gameState = gameWait;
	nbPlayerInGame = 0;

	// declarativ color mask, used for create a RGB surface
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	// create overlay for pause / exit game
	overlay = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(overlay, NULL, SDL_MapRGBA(overlay->format, 0, 0, 0, 120));

	Sprite::Instance().drawText(overlay, (640 / 2), 114, "--- PAUSE ---", green, true);
	Sprite::Instance().drawText(overlay, (640 / 2), 154, "Press X/B for exit game", green, true);
	Sprite::Instance().drawText(overlay, (640 / 2), 194, "Press Select for exit pause", green, true);

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
	grid = new Grid(levelIndex, tab, tabBonus);
	

	/*
	 *	LOAD PLAYER ON GRID
	 */

	//in_keystate = in_keystateLibretro;
	in_keystate_cpu = new unsigned short[16];
	for (int i = 0; i < 16; i++) {
		in_keystate_cpu[i] = 0;
	}

	int indexLibretro = 0;
	int index = 0;
	int indexPlayerForGame = 0;
	Player * player;
	Brain * brain;

	//init color
	for (int i = 0; i < 16; i++) {
		playerInformation[i][2] = rand() % nbColorPlayer;
	}

	for (int i = 0; i < 16; i++) {
		float startX = startPlayer[i][0];
		float startY = startPlayer[i][1];

		playerInformation[i][0] = playerInformationParam[i][0];
		playerInformation[i][1] = playerInformationParam[i][1];
		//alive
		playerInformation[i][3] = 1;
		//score null
		playerInformation[i][4] = 0;

		switch (playerInformation[i][0]) {
			case HUMAN:

			// if a human link the next keystate of libretro, else link a empty value
			player = new Player(&in_keystate[indexLibretro], false, playerInformation[i][1], startX, startY, i, tab, tabBonus, grid, tabPlayerCoord, nbPlayerConfig, indexPlayerForGame, playerInformation[i][2]);
			players.push_back(player);
			player = NULL;
			indexLibretro++;
			nbPlayerAlive++;
			indexPlayerForGame++;
			break;
			case CPU:
			player = new Player(&in_keystate_cpu[index], true, playerInformation[i][1], startX, startY, i, tab, tabBonus, grid, tabPlayerCoord, nbPlayerConfig, indexPlayerForGame, playerInformation[i][2]);
			players.push_back(player);
			player = NULL;

			brain = new Brain(&in_keystate_cpu[index], tab, tabPlayerCoord, nbPlayerConfig, i, cpuLevel);
			brains.push_back(brain);
			brain = NULL;

			in_keystate[index] = 0;
			index++;
			nbPlayerAlive++;
			indexPlayerForGame++;
			break;
			case OFF:
			break;
		}

	}

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

	free(grid);
	free(tab);
	free(tabBonus);
	free(tabPlayerCoord);
	in_keystate = NULL;
	free(in_keystate_cpu);

	vout_buf = NULL;
	SDL_FreeSurface(overlay);
	SDL_FreeSurface(overlayResult);
	SDL_FreeSurface(screenBuffer);
	SDL_FreeSurface(playerBombeExplode);

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
	char mess[18];
	int x = 25;
	int y = 20;
	for (unsigned int i = 0; i < players.size(); i++) {
		if (players[i]->isAlive()) {
			sprintf(mess, "Player %i : ALIVE", i + 1);
			Sprite::Instance().drawText(overlayResult, x, y, mess, green, false);
		} else {
			sprintf(mess, "Player %i : DEAD", i + 1);
			Sprite::Instance().drawText(overlayResult, x, y, mess, red, false);
		}
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
	//reset background
	copySurfaceToBackRenderer(Sprite::Instance().getBackground(), vout_buf, 0, 0);

	int offsetShadow = 2;
	int offsetHeadPlayer = 4;
	int offsetScore = 22;
	
	for (int i = 0; i < 16; i++) {
		if(i==8){
			offsetShadow += 64;
			offsetHeadPlayer += 64;
			offsetScore += 64;
		}
		//shadow rect
		SDL_Rect rect;
		rect.x = i * 36 + offsetShadow;
		rect.y = 2;
		rect.w = 32;
		rect.h = 20;
		SDL_BlitSurface(Sprite::Instance().getShadowArea(3), NULL, vout_buf, &rect);

		//copy mini head player
		rect.x = i * 36 + offsetHeadPlayer;
		rect.y = 4;
		rect.w = 20;
		rect.h = 20;
		
		if(playerInformation[i][0] != 2){
			//FOR HUMAN PLAYER OR CPU
			SDL_BlitSurface(Sprite::Instance().getHappySprite(playerInformation[i][1], playerInformation[i][2]), NULL, vout_buf, &rect);
			//wrote number of victory
			char score[3];
			sprintf(score, "%i", playerInformation[i][4]);
			Sprite::Instance().drawText(vout_buf, i * 36 + offsetScore, 2, score, red, false);
		}else{
			//NO PLAYER
			char score[4];
			sprintf(score, "XXX");
			Sprite::Instance().drawText(vout_buf, i * 36 + offsetScore, 2, score, red, false);
		}
	}
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
	Sprite::Instance().drawText(vout_buf, (640/2), 2, time, green, true);
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
	int posYcalc = 0;

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
						//update score
						playerInformation[players[i]->getPlayerNumber()][4]++;
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

					Sound::Instance().playFireSound();

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
					explosions.push_back(new Explosion(posXBombe, posYBombe, 0, tab, tabBonus));
					if (tabBonus[posXBombe + posYBombe * sizeX] != -1) {
						grid->burnBonus(posXBombe, posYBombe);
					}

					//UP
					for (int j = 1; j < strenght + 1; j++) {
						if (exitLoop == true) {
							break;
						}
						posYcalc = posYBombe - j;
						if (posYcalc < 0) {
							posYcalc = sizeY + posYcalc;
						}

						//explostion and wall
						switch (tab[posXBombe + posYcalc * sizeX]) {
							case emptyElement:
							case explosionElement:
								if (j == strenght) {
									ind = 1;
								} else {
									ind = 2;
								}
								explosions.push_back(new Explosion(posXBombe, posYcalc, ind, tab, tabBonus));
								break;
							case brickElement:
								burnWalls.push_back(new BurnWall(posXBombe, posYcalc, ind, levelIndex, tab, tabBonus));
								grid->burnABrick(posXBombe, posYcalc);
								if (!isAPowerBombe) {
									exitLoop = true;
								}
								aWallHasBurn = true;
								break;
							case bombeElement:
								for (unsigned int k = 0; k < bombes.size(); k++) {
									if (bombes[k]->getCase() == posXBombe + posYcalc * sizeX) {
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
							if (tabBonus[posXBombe + posYcalc * sizeX] != -1) {
								grid->burnBonus(posXBombe, posYcalc);
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
								explosions.push_back(new Explosion((posXBombe + j), posYBombe, ind, tab, tabBonus));
								break;
							case brickElement:
								burnWalls.push_back(new BurnWall((posXBombe + j), posYBombe, ind, levelIndex, tab, tabBonus));
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

						posYcalc = posYBombe + j;
						if (posYcalc >= sizeY) {
							posYcalc = posYcalc - sizeY;
						}

						switch (tab[posXBombe + posYcalc * sizeX]) {
							case emptyElement:
							case explosionElement:
								if (j == strenght) {
									ind = 5;
								} else {
									ind = 6;
								}
								explosions.push_back(new Explosion(posXBombe, posYcalc, ind, tab, tabBonus));
								break;
							case brickElement:
								burnWalls.push_back(new BurnWall(posXBombe, posYcalc, ind, levelIndex, tab, tabBonus));
								grid->burnABrick(posXBombe, posYcalc);
								if (!isAPowerBombe) {
									exitLoop = true;
								}
								aWallHasBurn = true;
								break;
							case bombeElement:
								for (unsigned int k = 0; k < bombes.size(); k++) {
									if (bombes[k]->getCase() == posXBombe + posYcalc * sizeX) {
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
							if (tabBonus[posXBombe + posYBombe * sizeX] != -1) {
								grid->burnBonus(posXBombe, posYBombe);
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
								explosions.push_back(new Explosion((posXBombe - j), posYBombe, ind, tab, tabBonus));
								break;
							case brickElement:
								burnWalls.push_back(new BurnWall((posXBombe - j), posYBombe, ind, levelIndex, tab, tabBonus));
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
				}else{
					if(playerInformation[players[i]->getPlayerNumber()][3] ==1){
						playerInformation[players[i]->getPlayerNumber()][3] = 0;
						updateHeaderPlayer(players[i]->getIndexPlayerForGame(), players[i]->getPlayerNumber());
					}
				}
				if (!players[i]->walkOnWall()) {
					if (players[i]->wantPutBombe()) {
						bombes.push_back(players[i]->addBombe());
						players[i]->ABombeIsSet();
					}
				}
				if (players[i]->triggerPowerBombe()) {
					for (unsigned int j = 0; j < bombes.size(); j++) {
						if (bombes[j]->getPlayer() == players[i]->getIndexPlayerForGame()) {
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
						//update score
						playerInformation[players[i]->getPlayerNumber()][4]++;
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
						SuddenDeathAnimation * suddenDeathAnimation = new SuddenDeathAnimation(suddenDeathX, suddenDeathY, levelIndex, tab, grid);
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
			Sound::Instance().playEndSound();
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
				int indexPlayerForGame = 0;

				Player * player;
				Brain * brain;

				
				for (int i = 0; i < 16; i++) {

					float startX = startPlayer[i][0];
					float startY = startPlayer[i][1];
					playerInformation[i][3] = 1;
					switch (playerInformation[i][0]) {
						case HUMAN:
							// if a human link the next keystate of libretro, else link a empty value
							player = new Player(&in_keystate[indexLibretro], false, playerInformation[i][1], startX, startY, i, tab, tabBonus, grid, tabPlayerCoord, nbPlayerConfig,
									indexPlayerForGame, playerInformation[i][2]);
							players.push_back(player);
							player = NULL;
							indexLibretro++;
							nbPlayerInGame++;
							indexPlayerForGame++;
							break;
						case CPU:
							player = new Player(&in_keystate_cpu[index], true, playerInformation[i][1], startX, startY, i, tab, tabBonus, grid, tabPlayerCoord, nbPlayerConfig,
									indexPlayerForGame, playerInformation[i][2]);
							players.push_back(player);
							brain = new Brain(&in_keystate_cpu[index], tab, tabPlayerCoord, nbPlayerConfig, i, cpuLevel);
							brains.push_back(brain);
							brain = NULL;

							player = NULL;
							in_keystate[index] = 0;
							index++;
							nbPlayerInGame++;
							indexPlayerForGame++;
							break;
						case OFF:
							break;
					}
				}
			}
			break;
	}
}



void Game::updateHeaderPlayer(int i, int playerNumber){
	int offsetShadow = 2;
	int offsetHeadPlayer = 4;
	int offsetScore = 22;
	if(playerNumber>=8){
		offsetShadow += 64;
		offsetHeadPlayer += 64;
		offsetScore += 64;
	}
	//shadow rect
	SDL_Rect rect;
	rect.x = playerNumber * 36 + offsetShadow;
	rect.y = 2;
	rect.w = 32;
	rect.h = 20;
	
	SDL_BlitSurface(Sprite::Instance().getBackground(), &rect, vout_buf, &rect);
	
	SDL_BlitSurface(Sprite::Instance().getShadowArea(3), NULL, vout_buf, &rect);
	//copy mini head player
	rect.x = playerNumber * 36 + offsetHeadPlayer;
	rect.y = 4;
	rect.w = 20;
	rect.h = 20;
	//FOR HUMAN PLAYER OR CPU
	SDL_BlitSurface(Sprite::Instance().getCryingSprite(playerInformation[playerNumber][1], playerInformation[playerNumber][2]), NULL, vout_buf, &rect);
	//wrote number of victory
	char score[3];
	sprintf(score, "%i", playerInformation[playerNumber][4]);
	Sprite::Instance().drawText(vout_buf, playerNumber * 36 + offsetScore, 2, score, red, false);
}
