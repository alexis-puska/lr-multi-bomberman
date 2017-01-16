#include "Bomberman.h"

#include <time.h>

Bomberman::Bomberman(SDL_Surface * vout_bufLibretro) {
	Sprite::Instance();
	LevelService::Instance();
	Sound::Instance();
	Sound::Instance().startMenuMusique();
	srand (time(NULL));

	//color mask
Uint32 	rmask, gmask, bmask, amask;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	amask = 0xff000000;
	refreshBuffer = true;
	vout_buf = vout_bufLibretro;

	//init all surface
	screenBuffer = SDL_CreateRGBSurface(0, 640, 360, 32, rmask, gmask, bmask, amask);
	copySurfaceToBackRenderer(Sprite::Instance().getSplashScreen(), screenBuffer, 0, 0);
	currentStep = home;

	//init default player type
	for (int i = 0; i < 16; i++) {
		playerType[i][0] = 1;
		playerType[i][1] = (rand() % 7);
	}
	playerType[0][0] = 0;
	playerType[0][1] = 0;

	//init GameOption
	gameOption[0] = 0;
	gameOption[1] = 0;
	gameOption[2] = 1;
	gameOption[3] = 3;
	game = NULL;
}

Bomberman::~Bomberman() {
	SDL_FreeSurface(screenBuffer);
	if (game) {
		if (game->isConfigured()) {
			if (game->isAlive()) {
				game->exitGame();
			}
		}
		delete game;
	}
}

void Bomberman::tick(unsigned short in_keystateLibretro[16]) {
	//fprintf(stderr, "%u\n", in_keystate[0]);
	//color mask
	for (int i = 0; i < 16; i++) {
		in_keystate[i] = in_keystateLibretro[i];
	}

	if (currentStep != gameStep) {
		keyPressed();

		int nbPlayerConfig = 0;
		//spash screen and start pressed !
		if (previousPlayerKeystate[0] & keyPadStart && keychange[0]) {
			refreshBuffer = true;
			Sound::Instance().playValidSound();
			switch (currentStep) {
				case home:
					cursorPosition = 0;
					currentStep = PlayerTypeMenu;
					break;
				case PlayerTypeMenu:
					for (int i = 0; i < 16; i++) {
						if (playerType[i][0] == 1) {
							playerType[i][1] = (rand() % 7);
						}
					}
					cursorPosition = 0;
					currentStep = PlayerSpriteMenu;
					break;
				case PlayerSpriteMenu:
					cursorPosition = 0;
					currentStep = gameOptionMenu;
					break;
				case gameOptionMenu:
					cursorPosition = 0;
					currentStep = levelSelectionMenu;
					break;
				case levelSelectionMenu:
					cursorPosition = 0;
					nbPlayerConfig = 0;
					for (int i = 0; i < 16; i++) {
						if (playerType[i][0] <= 1) {
							nbPlayerConfig++;
						}
					}
					game = new Game(levelIndex, playerType, gameOption, vout_buf, in_keystate, nbPlayerConfig);
					currentStep = gameStep;

					Sound::Instance().stopMusique();
					Sound::Instance().startBattleMusique();
					//TODO

					break;
				case gameStep:
					break;
			}
		} else if (previousPlayerKeystate[0] & keyPadSelect && keychange[0]) {
			Sound::Instance().playCancelSound();
			refreshBuffer = true;
			switch (currentStep) {
				case home:
					cursorPosition = 0;
					break;
				case PlayerTypeMenu:
					cursorPosition = 0;
					currentStep = home;
					break;
				case PlayerSpriteMenu:
					cursorPosition = 0;
					currentStep = PlayerTypeMenu;
					break;
				case gameOptionMenu:
					cursorPosition = 0;
					currentStep = PlayerSpriteMenu;
					break;
				case levelSelectionMenu:
					cursorPosition = 0;
					currentStep = gameOptionMenu;
					break;
				case gameStep:
					cursorPosition = levelIndex;
					currentStep = levelSelectionMenu;
					break;
			}
		}

		switch (currentStep) {
			case home:
				if (refreshBuffer) {
					cursor.stopAnimation();
					SDL_BlitSurface(Sprite::Instance().getSplashScreen(), NULL, vout_buf, NULL);
					refreshBuffer = false;
				}
				break;
			case PlayerTypeMenu:
				cursor.startAnimation();
				drawPlayerTypeMenu();
				break;
			case PlayerSpriteMenu:
				cursor.stopAnimation();
				drawPlayerSpriteMenu();
				break;
			case gameOptionMenu:
				cursor.startAnimation();
				drawGameOptionMenu();
				break;
			case levelSelectionMenu:
				cursor.startAnimation();
				drawLevelSelectionMenu();
				break;
			case gameStep:
				cursor.stopAnimation();
				if (refreshBuffer && previousPlayerKeystate[0] & keyPadStart && keychange[0]) {
					refreshBuffer = false;
				}
				break;
		}
	} else {
		if (game->isRequestStopGame()) {
			game->exitGame();
			delete game;
			game = NULL;
			cursorPosition = levelIndex;
			currentStep = levelSelectionMenu;
			Sound::Instance().stopMusique();
			Sound::Instance().startMenuMusique();
		}
	}
}

void Bomberman::copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y) {
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

/**********************************************************
 *
 * Select the type of player
 *
 **********************************************************/
void Bomberman::drawPlayerTypeMenu() {
	if (refreshBuffer || keychange[0]) {
		SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "CONFIGURATION JOUEUR", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);

		if (previousPlayerKeystate[0] & keyPadRight && keychange[0]) {
			cursorPosition += 4;
			Sound::Instance().playBipSound();
			if (cursorPosition > 15) {
				cursorPosition = cursorPosition % 4;
			}
		}
		if (previousPlayerKeystate[0] & keyPadLeft && keychange[0]) {
			cursorPosition -= 4;
			Sound::Instance().playBipSound();
			if (cursorPosition < 0) {
				cursorPosition += 16;
			}
		}
		if (previousPlayerKeystate[0] & keyPadDown && keychange[0]) {
			cursorPosition++;
			Sound::Instance().playBipSound();
			if (cursorPosition > 15) {
				cursorPosition = 0;
			}
		}
		if (previousPlayerKeystate[0] & keyPadUp && keychange[0]) {
			cursorPosition--;
			Sound::Instance().playBipSound();
			if (cursorPosition < 0) {
				cursorPosition = 15;
			}
		}
		if (previousPlayerKeystate[0] & keyPadA && keychange[0]) {
			playerType[cursorPosition][0]++;
			Sound::Instance().playBipSound();
			if (playerType[cursorPosition][0] > 2) {
				playerType[cursorPosition][0] = 0;
			}
		}
		if (previousPlayerKeystate[0] & keyPadB && keychange[0]) {
			playerType[cursorPosition][0]--;
			Sound::Instance().playBipSound();
			if (playerType[cursorPosition][0] < 0) {
				playerType[cursorPosition][0] = 2;
			}
		}

		//player 0 always Human
		playerType[0][0] = 0;
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				char playerName[15] = "Joueur ";
				char playerTypeName[6];
				sprintf(playerName, "Joueur %2i : ", (j * 4 + i) + 1);

				int playerColor = blue;
				switch (playerType[j * 4 + i][0]) {
					case HUMAN:
						sprintf(playerTypeName, "HUMAN");
						playerColor = blue;
						break;
					case CPU:
						sprintf(playerTypeName, "CPU");
						playerColor = green;
						break;
					case OFF:
						sprintf(playerTypeName, "OFF");
						playerColor = red;
						break;
				}
				Sprite::Instance().drawText(screenBuffer, 123 + (j * 133), 187 + (i * 20), playerTypeName, playerColor, false);
				Sprite::Instance().drawText(screenBuffer, 56 + (j * 133), 187 + (i * 20), playerName, green, false);
			}
		}
		refreshBuffer = false;
	}
	int cursorPosX = 36 + (((cursorPosition - (cursorPosition % 4)) / 4) * 133);
	int cursorposY = 187 + ((cursorPosition % 4) * 20);
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
	copySurfaceToBackRenderer(cursor.getCurrentFrame(), vout_buf, cursorPosX, cursorposY);
}

/**********************************************************
 *
 * Menu for selecting the sprite of a player
 *
 **********************************************************/
void Bomberman::drawPlayerSpriteMenu() {

	if (refreshBuffer || anyPlayerkeychange) {

		SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "SELECT COSTUME", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - change with Left / Right - -", gold, true);
		//add player sprite
		for (int i = 0; i < 8; i++) {
			copySurfaceToBackRenderer(Sprite::Instance().playerDrawNormal(i, 0, 0, 0), screenBuffer, 54 + (i * 72), 174);
		}

		for (int i = 0; i < 16; i++) {
			if (previousPlayerKeystate[i] & keyPadRight && keychange[i]) {
				playerType[i][1] = playerType[i][1] + 1;
				Sound::Instance().playBipSound();
				if (playerType[i][1] > 7) {
					playerType[i][1] = 0;
				}
			}
			if (previousPlayerKeystate[i] & keyPadLeft && keychange[i]) {
				playerType[i][1] = playerType[i][1] - 1;
				Sound::Instance().playBipSound();
				if (playerType[i][1] < 0) {
					playerType[i][1] = 7;
				}
			}
		}

		int list[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < 16; i++) {
			if (playerType[i][0] != OFF) {
				int index = playerType[i][1];
				char playerName[10];
				int playerColor = blue;
				switch (playerType[i][0]) {
					case HUMAN:
						sprintf(playerName, "HUMAN %i", i + 1);
						playerColor = blue;
						break;
					case CPU:
						sprintf(playerName, "CPU %i", i + 1);
						playerColor = green;
						break;
					case OFF:
						break;
				}
				Sprite::Instance().drawText(screenBuffer, 70 + (playerType[i][1] * 72), 216 + (15 * list[index]), playerName, playerColor, true);
				list[index] = list[index] + 1;
			}
		}
		refreshBuffer = false;
		copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
	}
}

/**********************************************************
 *
 * Menu for gameStep option
 *
 **********************************************************/
void Bomberman::drawGameOptionMenu() {
	if (refreshBuffer || keychange[0]) {

		if (previousPlayerKeystate[0] & keyPadA && keychange[0]) {
			Sound::Instance().playBipSound();
			switch (cursorPosition) {
				case suddenDeathOption:
				case badBomberOption:
					if (gameOption[cursorPosition] == 0) {
						gameOption[cursorPosition] = 1;
					} else {
						gameOption[cursorPosition] = 0;
					}
					break;
				case cpuLevelOption:
					gameOption[cursorPosition] = gameOption[cursorPosition] + 1;
					if (gameOption[cursorPosition] < 1) {
						gameOption[cursorPosition] = 5;
					} else if (gameOption[cursorPosition] > 5) {
						gameOption[cursorPosition] = 1;
					}
					break;
				case timeLevelOption:
					gameOption[cursorPosition] = gameOption[cursorPosition] + 1;
					if (gameOption[cursorPosition] > 6) {
						gameOption[cursorPosition] = -1;
					} else if (gameOption[cursorPosition] == 0) {
						gameOption[cursorPosition] = 2;
					}
					break;
			}
		}
		if (previousPlayerKeystate[0] & keyPadB && keychange[0]) {
			Sound::Instance().playBipSound();
			switch (cursorPosition) {
				case suddenDeathOption:
				case badBomberOption:
					if (gameOption[cursorPosition] == 0) {
						gameOption[cursorPosition] = 1;
					} else {
						gameOption[cursorPosition] = 0;
					}
					break;
				case cpuLevelOption:
					gameOption[cursorPosition] = gameOption[cursorPosition] - 1;
					if (gameOption[cursorPosition] < 1) {
						gameOption[cursorPosition] = 5;
					} else if (gameOption[cursorPosition] > 5) {
						gameOption[cursorPosition] = 1;
					}
					break;
				case timeLevelOption:
					gameOption[cursorPosition] = gameOption[cursorPosition] - 1;
					if (gameOption[cursorPosition] == 1) {
						gameOption[cursorPosition] = -1;
					} else if (gameOption[cursorPosition] == -2) {
						gameOption[cursorPosition] = 6;
					}
					break;

			}
		}
		if (previousPlayerKeystate[0] & keyPadDown && keychange[0]) {
			Sound::Instance().playBipSound();
			cursorPosition++;
			if (cursorPosition > 3) {
				cursorPosition = 0;
			}
		}
		if (previousPlayerKeystate[0] & keyPadUp && keychange[0]) {
			Sound::Instance().playBipSound();
			cursorPosition--;
			if (cursorPosition < 0) {
				cursorPosition = 3;
			}
		}

		SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);

		char CPULevel[2];
		sprintf(CPULevel, "%i", gameOption[2]);
		char timeOfLevel[7];
		if (gameOption[3] != -1) {
			sprintf(timeOfLevel, "%i", gameOption[3]);
		} else {
			sprintf(timeOfLevel, "infini");
		}
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(1), screenBuffer, 33, 183);
		Sprite::Instance().drawText(screenBuffer, (640/2), 154, "GAME OPTION", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);
		Sprite::Instance().drawText(screenBuffer, 200, 187, "Sudden Death", green, false);
		Sprite::Instance().drawText(screenBuffer, 200, 207, "Bad Bomber", green, false);
		Sprite::Instance().drawText(screenBuffer, 200, 227, "CPU Level", green, false);
		Sprite::Instance().drawText(screenBuffer, 200, 247, "Time", green, false);
		if (gameOption[0] == 1) {
			Sprite::Instance().drawText(screenBuffer, 400, 187, "ON", green, false);
		} else {
			Sprite::Instance().drawText(screenBuffer, 400, 187, "OFF", red, false);
		}
		if (gameOption[1] == 1) {
			Sprite::Instance().drawText(screenBuffer, 400, 207, "ON", green, false);
		} else {
			Sprite::Instance().drawText(screenBuffer, 400, 207, "OFF", red, false);
		}
		Sprite::Instance().drawText(screenBuffer, 400, 227, CPULevel, blue, false);
		Sprite::Instance().drawText(screenBuffer, 400, 247, timeOfLevel, blue, false);
		refreshBuffer = false;
	}
	int cursorPosX = 183;
	int cursorposY = 187 + (cursorPosition * 20);
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
	copySurfaceToBackRenderer(cursor.getCurrentFrame(), vout_buf, cursorPosX, cursorposY);
}

/**********************************************************
 *
 * Menu for level selection
 * 
 **********************************************************/
void Bomberman::drawLevelSelectionMenu() {
	if (refreshBuffer || keychange[0]) {
		SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
		Sprite::Instance().drawText(screenBuffer, (640/2), 154, "SELECT THE LEVEL", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);
		if (previousPlayerKeystate[0] & keyPadA && keychange[0]) {
			Sound::Instance().playBipSound();
			cursorPosition++;
			if (cursorPosition > nbLevel - 1) {
				cursorPosition = 0;
			}
		}
		if (previousPlayerKeystate[0] & keyPadB && keychange[0]) {
			Sound::Instance().playBipSound();
			cursorPosition--;
			if (cursorPosition < 0) {
				cursorPosition = nbLevel - 1;
			}
		}
		copySurfaceToBackRenderer(Sprite::Instance().getLevelPreview(cursorPosition), screenBuffer, 50, 200);
		Level * level = LevelService::Instance().getLevel(cursorPosition);

		char num[3];
		sprintf(num, "%i", level->getVariantes(0)->getNumber());
		Sprite::Instance().drawText(screenBuffer, 224, 184, "Name : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 224, 204, "Variante : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 224, 224, "Description : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 224, 264, "Bonus : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 327, 184, level->getName(), green, false);
		Sprite::Instance().drawText(screenBuffer, 327, 204, num, green, false);
		Sprite::Instance().drawText(screenBuffer, 327, 224, level->getVariantes(0)->getDescriptionLine1(), green, false);
		Sprite::Instance().drawText(screenBuffer, 327, 244, level->getVariantes(0)->getDescriptionLine2(), green, false);
		Sprite::Instance().drawText(screenBuffer, 327, 264, "default", green, false);
		for(int i=0 ; i<13 ; i++){
			sprintf(num, "%i", level->getVariantes(0)->getBonus(i));
			copySurfaceToBackRenderer(Sprite::Instance().getBonus(i), screenBuffer, 224 + i * 30, 288);
			Sprite::Instance().drawText(screenBuffer, 232 + i * 30, 302, num, green, true);
		}
		level = NULL;
		refreshBuffer = false;
		levelIndex = cursorPosition;
	}
	int cursorPosX = 200;
	int cursorposY = 186;

	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
	copySurfaceToBackRenderer(cursor.getCurrentFrame(), vout_buf, cursorPosX, cursorposY);
}

void Bomberman::keyPressed() {
	int index = 0;
	anyPlayerkeychange = false;
	for (int i = 0; i < 16; i++) {
		if (playerType[i][0] == 0) {
			if (previousPlayerKeystate[i] != in_keystate[index]) {
				keychange[i] = true;
				anyPlayerkeychange = true;
				previousPlayerKeystate[i] = in_keystate[index];
			} else {
				keychange[i] = false;
			}
			index++;
		} else {
			keychange[i] = false;
		}
	}
}
