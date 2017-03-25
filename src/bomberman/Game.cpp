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
	long warningCount = 0l;
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
			if (delay < 5) {
				warningCount++;
				fprintf(stderr, "%i ms %li\n", (int) delay, warningCount);
			}
			SDL_Delay(delay);
		} else {
			warningCount++;
			fprintf(stderr, "warning %li\n", warningCount);
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
	isThreadAlive = false;
	configured = false;
	requestStopGame = false;
}

/*
 * 
 * constructor
 * 
 */
Game::Game(SDL_Surface * vout_buf, unsigned short * in_keystate, unsigned short * in_keystate_over_net) {
	srand (time(NULL));

this	->vout_buf = vout_buf;
	this->in_keystate = in_keystate;
	this->in_keystate_over_net = in_keystate_over_net;

	levelIndex = GameConfig::Instance().getLevel();
	variante = GameConfig::Instance().getVariante();

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
	for (int i = 0; i < sizeX * sizeY; i++) {
		tab[i] = emptyElement;
		tabBonus[i] = -1;
	}
	GameConfig::Instance().resetPlayerCord();

	suddenDeath = GameConfig::Instance().isSuddentDeathMode();
	badBomber = GameConfig::Instance().isBadBomberMode();

	if (GameConfig::Instance().getTimeOfGame() != -1) {
		nbTickForGame = GameConfig::Instance().getTimeOfGame() * 50 * 60;
		nbTickForGameParam = nbTickForGame;
	} else {
		nbTickForGame = GameConfig::Instance().getTimeOfGame();
		nbTickForGameParam = nbTickForGame;
	}
	grid = new Grid(tab, tabBonus);

	/*
	 *	LOAD PLAYER ON GRID
	 */
	this->gameInitElement = false;
	initRails();
	initButtons();
	initHole();
	initMine();
	initTrolley();
	initTeleporter();

	//in_keystate = in_keystateLibretro;
	in_keystate_cpu = new unsigned short[16];
	for (int i = 0; i < 16; i++) {
		in_keystate_cpu[i] = 0;
	}

	int indexLibretro = 0;
	int indexOverInternet = 0;
	int index = 0;
	int indexPlayerForGame = 0;
	Player * player;
	Brain * brain;

	GameConfig::Instance().generateColorPlayer();
	GameConfig::Instance().resetPlayerScore();

	for (int i = 0; i < 16; i++) {
		int startCase = LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->getStart(i);

		float startX = (startCase%35)+0.5;
		float startY = (startCase/35)+0.5;

		//alive
		GameConfig::Instance().setPlayerAlive(i);

		switch (GameConfig::Instance().getPlayerType(i)) {
			case HUMAN:

			// if a human link the next keystate of libretro, else link a empty value
			player = new Player(&in_keystate[indexLibretro], startX, startY, i, tab, tabBonus, grid, indexPlayerForGame, LevelService::Instance().getLevel(levelIndex)->isWaterOverlayMode(), &bombes, &louisBurns);
			players.push_back(player);
			player = NULL;
			indexLibretro++;
			nbPlayerAlive++;
			indexPlayerForGame++;
			break;
			case CPU:
			player = new Player(&in_keystate_cpu[index], startX, startY, i, tab, tabBonus, grid, indexPlayerForGame, LevelService::Instance().getLevel(levelIndex)->isWaterOverlayMode(), &bombes, &louisBurns);
			players.push_back(player);
			player = NULL;

			brain = new Brain(&in_keystate_cpu[index], tab, i, players[indexPlayerForGame]);
			brains.push_back(brain);
			brain = NULL;

			in_keystate[index] = 0;
			index++;
			nbPlayerAlive++;
			indexPlayerForGame++;
			break;
			case NET:

			// if a human link the next keystate of libretro, else link a empty value
			player = new Player(&in_keystate_over_net[indexOverInternet], startX, startY, i, tab, tabBonus, grid, indexPlayerForGame, LevelService::Instance().getLevel(levelIndex)->isWaterOverlayMode(), &bombes, &louisBurns);
			players.push_back(player);
			player = NULL;
			indexOverInternet++;
			nbPlayerAlive++;
			indexPlayerForGame++;
			break;

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
	for (std::map<int, Trolley*>::iterator it = trolleys.begin(); it != trolleys.end(); ++it) {
		delete it->second;
	}
	for (std::map<int, Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
		delete it->second;
	}
	for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
		delete it->second;
	}
	for (std::map<int, Mine*>::iterator it = mines.begin(); it != mines.end(); ++it) {
		delete it->second;
	}
	for (std::map<int, Hole*>::iterator it = holes.begin(); it != holes.end(); ++it) {
		delete it->second;
	}
	for (std::map<int, Teleporter*>::iterator it = teleporters.begin(); it != teleporters.end(); ++it) {
		delete it->second;
	}
	for (unsigned int i = 0; i < bombes.size(); i++) {
		delete bombes[i];
	}
	for (unsigned int i = 0; i < brains.size(); i++) {
		delete brains[i];
	}
	for (unsigned int i = 0; i < players.size(); i++) {
		delete players[i];
	}
	for (unsigned int i = 0; i < explosions.size(); i++) {
		delete explosions[i];
	}
	for (unsigned int i = 0; i < burnWalls.size(); i++) {
		delete burnWalls[i];
	}
	for (unsigned int i = 0; i < suddenDeathAnimations.size(); i++) {
		delete suddenDeathAnimations[i];
	}
	for (unsigned int i = 0; i < louisBurns.size(); i++) {
		delete louisBurns[i];
	}
	for (unsigned int i = 0; i < PopBonusList.size(); i++) {
		delete PopBonusList[i];
	}
	for (unsigned int i = 0; i < BurnBonusList.size(); i++) {
		delete BurnBonusList[i];
	}

	brains.clear();
	players.clear();
	bombes.clear();
	explosions.clear();
	burnWalls.clear();
	suddenDeathAnimations.clear();
	louisBurns.clear();
	PopBonusList.clear();
	BurnBonusList.clear();
	rails.clear();
	buttons.clear();
	trolleys.clear();
	mines.clear();
	holes.clear();
	teleporters.clear();

	delete grid;
	delete tab;
	delete tabBonus;

	in_keystate = NULL;
	delete in_keystate_cpu;

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
		if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
			BomberNetServer::Instance().sendChangeScreenCommand(6);
		}
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
	if (grid->somethingInSky()) {
		SDL_BlitSurface(grid->getSkyLayer(), &mergeRect, screenBuffer, &mergeRect);
	}
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
		if (i == 8) {
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

		if (GameConfig::Instance().getPlayerType(i) != 2) {
			//FOR HUMAN PLAYER OR CPU
			int idx = Sprite::Instance().getHappySprite(GameConfig::Instance().getPlayerSpriteType(i), GameConfig::Instance().getPlayerColor(i), 0);
			SDL_BlitSurface(Sprite::Instance().getPlayerSprite(idx), NULL, vout_buf, &rect);
			//wrote number of victory
			char score[3];
			sprintf(score, "%i", GameConfig::Instance().getPlayerScore(i));
			Sprite::Instance().drawText(vout_buf, i * 36 + offsetScore, 2, score, green, false);
		} else {
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
	Sprite::Instance().drawText(vout_buf, (640 / 2), 2, time, green, true);
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

	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().initBuffer();
		BomberNetServer::Instance().sendGameInfo(nbTickForGame, true, gameState);
	}

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
						GameConfig::Instance().incPlayerScore(players[i]->getPlayerNumber());
						;
						players[i]->winTheGame();
					}
				}
				gameState = generateResult;
				break;
			}

			if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
				BomberNetServer::Instance().sendPlayerState();
				if (nbTickForGame % 500 == 0) {
					BomberNetServer::Instance().sendBuffer();
					BomberNetServer::Instance().initBuffer();
					BomberNetServer::Instance().sendTab(tab);
					BomberNetServer::Instance().sendTabBonus(tabBonus);
					BomberNetServer::Instance().sendBuffer();
					BomberNetServer::Instance().initBuffer();
				}
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

			if (!gameInitElement) {
				InitElementOfGame();
				gameInitElement = true;
			}

			/*
			 *
			 * GAME PART : BUTTON
			 *
			 */

			buttonDoSomething();
			holeDoSomething();
			mineDoSomething();
			teleporterDoSomething();
			trolleyDoSomething();

			/*
			 *
			 *	GAME PART : BOMBE
			 *
			 */

			for (unsigned int i = 0; i < bombes.size(); i++) {
				bombes[i]->tick(playerBombeExplode);
				if (bombes[i]->isExplode()) {

					Sound::Instance().playFireSound();
					if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
						BomberNetServer::Instance().sendSound(5, -1, true);
					}

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
						redrawElement(posXBombe, posYBombe);
						BurnBonusList.push_back(new BurnBonus(posXBombe, posYBombe));
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
								burnWalls.push_back(new BurnWall(posXBombe, posYcalc, ind, tab, tabBonus));
								grid->burnABrick(posXBombe, posYcalc);
								redrawElement(posXBombe, posYcalc);
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
								redrawElement(posXBombe, posYBombe);
								BurnBonusList.push_back(new BurnBonus(posXBombe, posYcalc));
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
								burnWalls.push_back(new BurnWall((posXBombe + j), posYBombe, ind, tab, tabBonus));
								grid->burnABrick((posXBombe + j), posYBombe);
								redrawElement((posXBombe + j), posYBombe);
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
								redrawElement((posXBombe + j), posYBombe);
								BurnBonusList.push_back(new BurnBonus(posXBombe + j, posYBombe));
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
								burnWalls.push_back(new BurnWall(posXBombe, posYcalc, ind, tab, tabBonus));
								grid->burnABrick(posXBombe, posYcalc);
								redrawElement(posXBombe, posYcalc);
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
								redrawElement(posXBombe, posYcalc);
								BurnBonusList.push_back(new BurnBonus(posXBombe, posYcalc));
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
								burnWalls.push_back(new BurnWall((posXBombe - j), posYBombe, ind, tab, tabBonus));
								grid->burnABrick((posXBombe - j), posYBombe);
								redrawElement((posXBombe - j), posYBombe);
								if (!isAPowerBombe) {
									exitLoop = true;
								}
								aWallHasBurn = true;
								break;
							case bombeElement:
								for (unsigned int k = 0; k < bombes.size(); k++) {
									if (bombes[k]->getCase() == (posXBombe - j) + posYBombe * sizeX) {
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
							if (tabBonus[(posXBombe - j) + posYBombe * sizeX] != -1) {
								grid->burnBonus((posXBombe - j), posYBombe);
								redrawElement((posXBombe - j), posYBombe);
								BurnBonusList.push_back(new BurnBonus(posXBombe - j, posYBombe));
							}
						}
					}
					delete bombes[i];
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
					delete explosions[i];
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
					delete burnWalls[i];
					burnWalls.erase(burnWalls.begin() + i);
				}
			}

			/*
			 *
			 *	GAME PART : BURN BONUS
			 *
			 */
			for (unsigned int i = 0; i < BurnBonusList.size(); i++) {
				BurnBonusList[i]->tick(playerBombeExplode);
				if (BurnBonusList[i]->canBeDelete()) {
					delete BurnBonusList[i];
					BurnBonusList.erase(BurnBonusList.begin() + i);
				}
			}

			/*
			 *
			 *	GAME PART : POP BONUS
			 *
			 */
			for (unsigned int i = 0; i < PopBonusList.size(); i++) {
				PopBonusList[i]->tick(playerBombeExplode);
				if (PopBonusList[i]->canBeDelete()) {
					delete PopBonusList[i];
					PopBonusList.erase(PopBonusList.begin() + i);
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
				int caseToRedraw = players[i]->foundABonus();
				if (caseToRedraw != -1) {
					grid->burnBonus(caseToRedraw % sizeX, round(caseToRedraw / sizeX));
					redrawElement(caseToRedraw % sizeX, round(caseToRedraw / sizeX));
				}
				players[i]->doSomething(playerBombeExplode);
				if (players[i]->isAlive()) {
					nbPlayerAlive++;
				} else {
					if (GameConfig::Instance().isPlayerAlive(players[i]->getPlayerNumber())) {
						GameConfig::Instance().setPlayerDead(players[i]->getPlayerNumber());

						if (!LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->isFillWithBricks()) {
							for (int y = 1; y < 15; y++) {
								int nbBonusType = 0;
								if (GameConfig::Instance().isCustomBonus()) {
									nbBonusType = GameConfig::Instance().getBonus(y);
								} else {
									nbBonusType = LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->getBonus(y);
								}

								for (int i = 0; i < nbBonusType; i++) {
									PopBonusList.push_back(new PopBonus(grid->playerDeadNeedBonus(y)));
								}
							}
						}

						updateHeaderPlayer(players[i]->getIndexPlayerForGame(), players[i]->getPlayerNumber());
					}
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
						GameConfig::Instance().incPlayerScore(players[i]->getPlayerNumber());
						players[i]->winTheGame();
					}
				}
				gameState = generateResult;
			}

			/*
			 *
			 *	GAME PART : BURNIN LOUIS ANIMATION
			 *
			 */
			for (unsigned int i = 0; i < louisBurns.size(); i++) {
				louisBurns[i]->tick(playerBombeExplode);
				if (louisBurns[i]->canBeDelete()) {
					delete louisBurns[i];
					louisBurns.erase(louisBurns.begin() + i);
				}
			}

			/*
			 *
			 *	SUDDEN DEATH PART !
			 *
			 */
			if (!suddenDeathCase) {
				if (GameConfig::Instance().isSuddentDeathMode() && (nbTickForGame == (4 * (sizeX - 2) * (sizeY - 2)) + (34 * 1))) {
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
						SuddenDeathAnimation * suddenDeathAnimation = new SuddenDeathAnimation(suddenDeathX, suddenDeathY, tab, grid);
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
						delete suddenDeathAnimations[i];
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
			if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
				BomberNetServer::Instance().sendSound(4, -1, true);
			}
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
				GameConfig::Instance().resetPlayerCord();
				gameState = gameWait;
				generateHeader();
				nbTickForGame = nbTickForGameParam;
				suddenDeathCase = false;

				for (std::map<int, Trolley*>::iterator it = trolleys.begin(); it != trolleys.end(); ++it) {
					delete it->second;
				}
				for (std::map<int, Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
					delete it->second;
				}
				for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
					delete it->second;
				}
				for (std::map<int, Mine*>::iterator it = mines.begin(); it != mines.end(); ++it) {
					delete it->second;
				}
				for (std::map<int, Hole*>::iterator it = holes.begin(); it != holes.end(); ++it) {
					delete it->second;
				}
				for (std::map<int, Teleporter*>::iterator it = teleporters.begin(); it != teleporters.end(); ++it) {
					delete it->second;
				}
				for (unsigned int i = 0; i < bombes.size(); i++) {
					delete bombes[i];
				}
				for (unsigned int i = 0; i < brains.size(); i++) {
					delete brains[i];
				}
				for (unsigned int i = 0; i < players.size(); i++) {
					delete players[i];
				}
				for (unsigned int i = 0; i < explosions.size(); i++) {
					delete explosions[i];
				}
				for (unsigned int i = 0; i < burnWalls.size(); i++) {
					delete burnWalls[i];
				}
				for (unsigned int i = 0; i < suddenDeathAnimations.size(); i++) {
					delete suddenDeathAnimations[i];
				}
				for (unsigned int i = 0; i < louisBurns.size(); i++) {
					delete louisBurns[i];
				}
				for (unsigned int i = 0; i < PopBonusList.size(); i++) {
					delete PopBonusList[i];
				}
				for (unsigned int i = 0; i < BurnBonusList.size(); i++) {
					delete BurnBonusList[i];
				}
				brains.clear();
				players.clear();
				bombes.clear();
				explosions.clear();
				burnWalls.clear();
				suddenDeathAnimations.clear();
				louisBurns.clear();
				PopBonusList.clear();
				BurnBonusList.clear();
				rails.clear();
				buttons.clear();
				trolleys.clear();
				mines.clear();
				holes.clear();
				teleporters.clear();

				grid->resetSurface();
				grid->reset();
				grid->generateGrid();

				int indexLibretro = 0;
				int indexOverNet = 0;
				int index = 0;
				nbPlayerInGame = 0;
				int indexPlayerForGame = 0;
				SDL_FillRect(playerBombeExplode, NULL, SDL_MapRGBA(playerBombeExplode->format, 0, 0, 0, 0));

				Player * player;
				Brain * brain;

				this->gameInitElement = false;
				initRails();
				initButtons();
				initHole();
				initMine();
				initTrolley();
				initTeleporter();

				for (int i = 0; i < 16; i++) {

					int startCase = LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->getStart(i);

					float startX = (startCase % 35) + 0.5;
					float startY = (startCase / 35) + 0.5;
					GameConfig::Instance().setPlayerAlive(i);
					switch (GameConfig::Instance().getPlayerType(i)) {
						case HUMAN:
							// if a human link the next keystate of libretro, else link a empty value
							player = new Player(&in_keystate[indexLibretro], startX, startY, i, tab, tabBonus, grid, indexPlayerForGame,
									LevelService::Instance().getLevel(levelIndex)->isWaterOverlayMode(), &bombes, &louisBurns);
							players.push_back(player);
							player = NULL;
							indexLibretro++;
							nbPlayerInGame++;
							indexPlayerForGame++;
							break;
						case CPU:
							player = new Player(&in_keystate_cpu[index], startX, startY, i, tab, tabBonus, grid, indexPlayerForGame,
									LevelService::Instance().getLevel(levelIndex)->isWaterOverlayMode(), &bombes, &louisBurns);
							players.push_back(player);
							brain = new Brain(&in_keystate_cpu[index], tab, i, players[indexPlayerForGame]);
							brains.push_back(brain);
							brain = NULL;

							player = NULL;
							in_keystate[index] = 0;
							index++;
							nbPlayerInGame++;
							indexPlayerForGame++;
							break;
						case NET:
							// if a human link the next keystate of libretro, else link a empty value
							player = new Player(&in_keystate_over_net[indexOverNet], startX, startY, i, tab, tabBonus, grid, indexPlayerForGame,
									LevelService::Instance().getLevel(levelIndex)->isWaterOverlayMode(), &bombes, &louisBurns);
							players.push_back(player);
							player = NULL;
							indexOverNet++;
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
	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().sendBuffer();
	}
}

void Game::updateHeaderPlayer(int i, int playerNumber) {
	int offsetShadow = 2;
	int offsetHeadPlayer = 4;
	int offsetScore = 22;
	if (playerNumber >= 8) {
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
	int idx = Sprite::Instance().getCryingSprite(GameConfig::Instance().getPlayerSpriteType(i), GameConfig::Instance().getPlayerColor(i), 0);
	SDL_BlitSurface(Sprite::Instance().getPlayerSprite(idx), NULL, vout_buf, &rect);
	//wrote number of victory
	char score[3];
	sprintf(score, "%i", GameConfig::Instance().getPlayerScore(playerNumber));
	Sprite::Instance().drawText(vout_buf, playerNumber * 36 + offsetScore, 2, score, red, false);
}

void Game::initRails() {
	std::map<int, RailSwitch *> railsIndex = LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->getRailsIndex();
	if (railsIndex.size() != 0) {
		for (std::map<int, RailSwitch *>::iterator it1 = railsIndex.begin(); it1 != railsIndex.end(); ++it1) {
			if (it1->second == NULL) {
				Rail * rail = new Rail(it1->first);
				rails[it1->first] = rail;
			} else {
				Rail * rail = new Rail(it1->first, it1->second->getPrevIndex(), it1->second->getNextIndex(), it1->second->getNextIndexAlt());
				rails[it1->first] = rail;
			}
		}
		if (rails.size() != 0) {
			for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
				it->second->init(rails);
			}
		}
	}
}

void Game::initButtons() {
	std::vector<int> buttonsIndex = LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->getButtonsIndex();
	if (buttonsIndex.size() != 0) {
		for (unsigned int i = 0; i < buttonsIndex.size(); i++) {
			int indexButton = buttonsIndex[i];
			Button * button = new Button(indexButton, &trolleys);
			buttons[indexButton] = button;
		}
	}
}

void Game::initHole() {
	std::vector<int> holesIndex = LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->getHolesIndex();
	if (holesIndex.size() != 0) {
		for (unsigned int i = 0; i < holesIndex.size(); i++) {
			int indexHole = holesIndex[i];
			Hole * hole = new Hole(indexHole);
			holes[indexHole] = hole;
		}
	}
}

void Game::initTrolley() {
	std::vector<int> trolleysIndex = LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->getTrolleysIndex();
	if (trolleysIndex.size() != 0) {
		for (unsigned int i = 0; i < trolleysIndex.size(); i++) {
			int indexTrolley = trolleysIndex[i];
			Trolley * trolley = new Trolley(indexTrolley, &players, &rails);
			trolleys[indexTrolley] = trolley;
		}
	}
}

void Game::initMine() {
	std::vector<int> minesIndex = LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->getMinesIndex();
	if (minesIndex.size() != 0) {
		for (unsigned int i = 0; i < minesIndex.size(); i++) {
			int indexMine = minesIndex[i];
			Mine * mine = new Mine(indexMine);
			mines[indexMine] = mine;
		}
	}
}

void Game::initTeleporter() {
	std::vector<int> teleportersIndex = LevelService::Instance().getLevel(levelIndex)->getVariantes(variante)->getTeleportersIndex();
	if (teleportersIndex.size() != 0) {
		for (unsigned int i = 0; i < teleportersIndex.size(); i++) {
			int indexTeleporter = teleportersIndex[i];
			Teleporter * teleporter = new Teleporter(indexTeleporter);
			teleporters[indexTeleporter] = teleporter;
		}
		for (std::map<int, Teleporter*>::iterator it = teleporters.begin(); it != teleporters.end(); ++it) {
			for (std::map<int, Teleporter*>::iterator it2 = teleporters.begin(); it2 != teleporters.end(); ++it2) {
				it->second->addDestination(it2->second);
			}
		}
	}
}

void Game::buttonDoSomething() {
	for (std::map<int, Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
		if (it->second->doSomething(grid->getBricksLayer())) {
			for (std::map<int, Rail*>::iterator it1 = rails.begin(); it1 != rails.end(); ++it1) {
				if (tab[it1->second->getIndex()] != brickElement) {
					it1->second->switching();
					redrawRail(it1->second->getIndex());

				} else {
					it1->second->switching();
				}
			}
		}
	}
}

void Game::mineDoSomething() {
	for (std::map<int, Mine*>::iterator it = mines.begin(); it != mines.end(); ++it) {
		if (it->second->doSomething(grid->getBricksLayer())) {
			GenerateMineExplosion(it->second->getIndex(), it->second->getFirstDirection());
			GenerateMineExplosion(it->second->getIndex(), it->second->getSecondDirection());
		}
	}
}

void Game::holeDoSomething() {
	for (std::map<int, Hole*>::iterator it = holes.begin(); it != holes.end(); ++it) {
		bool makeWall = it->second->doSomething(grid->getBricksLayer());
		int index = it->second->getIndex();
		if (tabBonus[index] != noBonus && tab[index] < brickElement) {
			grid->drawBonus(index);
		}
		if (makeWall) {
			tab[index] = wallElement;
		}
	}
}

void Game::teleporterDoSomething() {
	for (std::map<int, Teleporter*>::iterator it = teleporters.begin(); it != teleporters.end(); ++it) {
		if (it->second->doSomething(grid->getBricksLayer())) {
			grid->drawBonus(it->second->getIndex());
		}
	}
}

void Game::trolleyDoSomething() {
	for (std::map<int, Trolley*>::iterator it = trolleys.begin(); it != trolleys.end(); ++it) {
		it->second->doSomething(playerBombeExplode);
		if (it->second->isMove()) {
			int cur = it->second->getCurrentIndex();
			//fprintf(stderr, "cur : %i\n", cur);
			int prv = it->second->getPreviousIndex();
			int curX = cur % sizeX;
			int curY = floor(cur / sizeX);
			if (tab[cur] == brickElement) {
				grid->burnABrick(curX, curY);
				redrawRail(cur);
				redrawRail(prv);
				burnWalls.push_back(new BurnWall(curX, curY, 0, tab, tabBonus));
			}
			for (unsigned int k = 0; k < bombes.size(); k++) {
				if (bombes[k]->getCase() == cur) {
					bombes[k]->explode();
					break;
				}
			}
			grid->burnBonus(curX, curY);
			tab[cur] = explosionElement;
			if (prv != -1) {
				redrawRail(prv);
				tab[prv] = emptyElement;
			}
		} else {
			tab[it->second->getCurrentIndex()] = emptyElement;
		}
	}
}

//redessine quand on gille un bonus ou lorsque l'on grille une brick, dessine l'element puis le bonus dans tout les cas
void Game::redrawElement(int x, int y) {
	int index = x + (sizeX * y);
	redrawRail(index);
	redrawButton(index);
	redrawHole(index);
	redrawMine(index);
	redrawTeleporter(index);
	redrawTrolley(index);
	grid->drawBonus(x, y);
}

//redessine l'element que si la fonction et appeller, redessine le bonus que si il n'y a pas de brick au dessus.
void Game::redrawRail(int index) {
	std::map<int, Rail*>::iterator it = rails.find(index);
	if (it != rails.end()) {
		it->second->drawHimself(grid->getBricksLayer());
		if (tabBonus[index] != noBonus && tab[index] < brickElement) {
			grid->drawBonus(index);
		}
	}

}

void Game::redrawButton(int index) {
	std::map<int, Button*>::iterator it = buttons.find(index);
	if (it != buttons.end()) {
		it->second->drawHimself(grid->getBricksLayer());
		if (tabBonus[index] != noBonus && tab[index] < brickElement) {
			grid->drawBonus(index);
		}
	}

}
void Game::redrawHole(int index) {
	std::map<int, Hole*>::iterator it = holes.find(index);
	if (it != holes.end()) {
		it->second->drawHimself(grid->getBricksLayer());
		if (tabBonus[index] != noBonus && tab[index] < brickElement) {
			grid->drawBonus(index);
		}
	}

}
void Game::redrawMine(int index) {
	std::map<int, Mine*>::iterator it = mines.find(index);
	if (it != mines.end()) {
		it->second->drawHimself(grid->getBricksLayer(), 12);
		if (tabBonus[index] != noBonus && tab[index] < brickElement) {
			grid->drawBonus(index);
		}
	}

}
void Game::redrawTeleporter(int index) {
	std::map<int, Teleporter*>::iterator it = teleporters.find(index);
	if (it != teleporters.end()) {
		it->second->drawHimself(grid->getBricksLayer(), 0);
		if (tabBonus[index] != noBonus && tab[index] < brickElement) {
			grid->drawBonus(index);
		}
	}
}

void Game::redrawTrolley(int index) {
	std::map<int, Trolley*>::iterator it = trolleys.find(index);
	if (it != trolleys.end()) {
		it->second->drawHimself(playerBombeExplode);
		if (tabBonus[index] != noBonus && tab[index] < brickElement) {
			grid->drawBonus(index);
		}
	}
}

void Game::InitElementOfGame() {
	for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
		if (tab[it->first] < brickElement || tab[it->first] == bombeElement) {
			it->second->drawHimself(grid->getBricksLayer());
		}
	}
	for (std::map<int, Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
		if (tab[it->first] < brickElement || tab[it->first] == bombeElement) {
			it->second->drawHimself(grid->getBricksLayer());
		}
	}
	for (std::map<int, Mine*>::iterator it = mines.begin(); it != mines.end(); ++it) {
		if (tab[it->first] < brickElement || tab[it->first] == bombeElement) {
			it->second->drawHimself(grid->getBricksLayer(), 12);
		}
	}
	for (std::map<int, Hole*>::iterator it = holes.begin(); it != holes.end(); ++it) {
		if (tab[it->first] < brickElement || tab[it->first] == bombeElement) {
			it->second->drawHimself(grid->getBricksLayer());
		}
	}
	for (std::map<int, Teleporter*>::iterator it = teleporters.begin(); it != teleporters.end(); ++it) {
		if (tab[it->first] < brickElement || tab[it->first] == bombeElement) {
			it->second->drawHimself(grid->getBricksLayer(), 0);
		}
	}
	for (std::map<int, Trolley*>::iterator it = trolleys.begin(); it != trolleys.end(); ++it) {
		if (tab[it->first] < brickElement || tab[it->first] == bombeElement) {
			it->second->drawHimself(playerBombeExplode);
		}
	}
}

void Game::GenerateMineExplosion(int index, int direction) {
	Sound::Instance().playFireSound();
	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().sendSound(5, -1, true);
	}
	int strenght = 20;
	int posXMine = index % 35;
	int posYMine = floor(index / 35);
	int posYcalc = 0;
	bool exitLoop = false;
	bool aWallHasBurn = false;
	int ind = 0;

	//UP
	if (direction == mineUp) {
		for (int j = 1; j < strenght + 1; j++) {
			if (exitLoop == true) {
				break;
			}
			posYcalc = posYMine - j;
			if (posYcalc < 0) {
				posYcalc = sizeY + posYcalc;
			}

			//explostion and wall
			switch (tab[posXMine + posYcalc * sizeX]) {
				case emptyElement:
				case explosionElement:
					if (j == strenght) {
						ind = 1;
					} else if (j == 1) {
						ind = 5;
					} else {
						ind = 2;
					}
					explosions.push_back(new Explosion(posXMine, posYcalc, ind, tab, tabBonus));
					break;
				case brickElement:
					burnWalls.push_back(new BurnWall(posXMine, posYcalc, ind, tab, tabBonus));
					grid->burnABrick(posXMine, posYcalc);
					redrawElement(posXMine, posYcalc);

					exitLoop = true;

					aWallHasBurn = true;
					break;
				case bombeElement:
					for (unsigned int k = 0; k < bombes.size(); k++) {
						if (bombes[k]->getCase() == posXMine + posYcalc * sizeX) {
							bombes[k]->explode();

							exitLoop = true;

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
				if (tabBonus[posXMine + posYcalc * sizeX] != -1) {
					grid->burnBonus(posXMine, posYcalc);
					redrawElement(posXMine, posYMine);
					BurnBonusList.push_back(new BurnBonus(posXMine, posYcalc));
				}
			}
		}
	}

	if (direction == mineRight) {
		exitLoop = false;
		aWallHasBurn = false;
		for (int j = 1; j < strenght + 1; j++) {
			if (exitLoop == true) {
				break;
			}

			switch (tab[(posXMine + j) + posYMine * sizeX]) {
				case emptyElement:
				case explosionElement:
					if (j == strenght) {
						ind = 7;
					} else if (j == 1) {
						ind = 3;
					} else {
						ind = 8;
					}
					explosions.push_back(new Explosion((posXMine + j), posYMine, ind, tab, tabBonus));
					break;
				case brickElement:
					burnWalls.push_back(new BurnWall((posXMine + j), posYMine, ind, tab, tabBonus));
					grid->burnABrick((posXMine + j), posYMine);
					redrawElement((posXMine + j), posYMine);

					exitLoop = true;

					aWallHasBurn = true;
					break;
				case bombeElement:
					for (unsigned int k = 0; k < bombes.size(); k++) {
						if (bombes[k]->getCase() == (posXMine + j) + posYMine * sizeX) {
							bombes[k]->explode();

							exitLoop = true;

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
				if (tabBonus[(posXMine + j) + posYMine * sizeX] != -1) {
					grid->burnBonus((posXMine + j), posYMine);
					redrawElement((posXMine + j), posYMine);
					BurnBonusList.push_back(new BurnBonus(posXMine + j, posYMine));
				}
			}
		}
	}

	if (direction == mineDown) {
		exitLoop = false;
		aWallHasBurn = false;
		for (int j = 1; j < strenght + 1; j++) {
			if (exitLoop == true) {
				break;
			}

			posYcalc = posYMine + j;
			if (posYcalc >= sizeY) {
				posYcalc = posYcalc - sizeY;
			}

			switch (tab[posXMine + posYcalc * sizeX]) {
				case emptyElement:
				case explosionElement:
					if (j == strenght) {
						ind = 5;
					} else if (j == 1) {
						ind = 1;
					} else {
						ind = 6;
					}
					explosions.push_back(new Explosion(posXMine, posYcalc, ind, tab, tabBonus));
					break;
				case brickElement:
					burnWalls.push_back(new BurnWall(posXMine, posYcalc, ind, tab, tabBonus));
					grid->burnABrick(posXMine, posYcalc);
					redrawElement(posXMine, posYcalc);

					exitLoop = true;

					aWallHasBurn = true;
					break;
				case bombeElement:
					for (unsigned int k = 0; k < bombes.size(); k++) {
						if (bombes[k]->getCase() == posXMine + posYcalc * sizeX) {
							bombes[k]->explode();

							exitLoop = true;

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
				if (tabBonus[posXMine + posYcalc * sizeX] != -1) {
					grid->burnBonus(posXMine, posYcalc);
					redrawElement(posXMine, posYcalc);
					BurnBonusList.push_back(new BurnBonus(posXMine, posYcalc));
				}
			}

		}
	}

	if (direction == mineLeft) {
		exitLoop = false;
		aWallHasBurn = false;
		for (int j = 1; j < strenght + 1; j++) {
			if (exitLoop == true) {
				break;
			}

			switch (tab[(posXMine - j) + posYMine * sizeX]) {
				case emptyElement:
				case explosionElement:
					if (j == strenght) {
						ind = 3;
					} else if (j == 1) {
						ind = 7;
					} else {
						ind = 4;
					}
					explosions.push_back(new Explosion((posXMine - j), posYMine, ind, tab, tabBonus));
					break;
				case brickElement:
					burnWalls.push_back(new BurnWall((posXMine - j), posYMine, ind, tab, tabBonus));
					grid->burnABrick((posXMine - j), posYMine);
					redrawElement((posXMine - j), posYMine);
					exitLoop = true;
					aWallHasBurn = true;
					break;
				case bombeElement:
					for (unsigned int k = 0; k < bombes.size(); k++) {
						if (bombes[k]->getCase() == (posXMine - j) + posYMine * sizeX) {
							bombes[k]->explode();
							exitLoop = true;
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
				if (tabBonus[(posXMine - j) + posYMine * sizeX] != -1) {
					grid->burnBonus((posXMine - j), posYMine);
					redrawElement((posXMine - j), posYMine);
					BurnBonusList.push_back(new BurnBonus(posXMine - j, posYMine));
				}
			}
		}
	}
}
