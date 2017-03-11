#include "Bomberman.h"

#include <time.h>

Bomberman::Bomberman(SDL_Surface * vout_bufLibretro) {
	Sprite::Instance();
	LevelService::Instance();
	GameConfig::Instance();
	Sound::Instance();
	Sound::Instance().startMenuMusique();

//
//	BomberNetClient::Instance();
//	BomberNetClient::Instance().connectClient();

//color mask
	Uint32 rmask, gmask, bmask, amask;
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

	game = NULL;
}

Bomberman::~Bomberman() {
	//BomberNet::Instance().stopServer();
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

void Bomberman::keyPressed() {
	int index = 0;
	anyPlayerkeychange = false;
	for (int i = 0; i < 16; i++) {
		if (GameConfig::Instance().getPlayerType(i) == 0) {
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

void Bomberman::tick(unsigned short in_keystateLibretro[16]) {
	//fprintf(stderr, "%u\n", in_keystate[0]);
	//color mask
	for (int i = 0; i < 16; i++) {
		in_keystate[i] = in_keystateLibretro[i];
	}

	if (currentStep != gameStep) {
		keyPressed();

		//spash screen and start pressed !
		if (previousPlayerKeystate[0] & keyPadStart && keychange[0]) {
			refreshBuffer = true;
			Sound::Instance().playValidSound();
			switch (currentStep) {
				case home:
					cursorPosition = 0;
					currentStep = gameMode;
					break;
				case gameMode:
					cursorPosition = 0;
					switch (GameConfig::Instance().getGameModeType()) {
						case LOCAL:
							currentStep = PlayerTypeMenu;
							break;
						case NET_SERVER:
							currentStep = serverNumberOfClient;
							break;
						case NET_CLIENT:
							currentStep = clientNumberPlayerName;
							break;
					}
					break;
				case serverNumberOfClient:
					if (BomberNetServer::Instance().createTcpServer()) {
						BomberNetServer::Instance().startServer();
						cursorPosition = 0;
						currentStep = PlayerTypeMenu;
					}
					break;
				case clientNumberPlayerName:
					cursorPosition = 0;
					currentStep = clientIpPort;
					break;
				case clientIpPort:
					BomberNetClient::Instance().createTcpClient();
					if (BomberNetClient::Instance().connectClient()) {
						cursorPosition = 0;
						currentStep = PlayerTypeMenu;
					}
					break;
				case PlayerTypeMenu:
					GameConfig::Instance().generatePlayerSpriteTypeforCPU();
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
					game = new Game(vout_buf, in_keystate);
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
				case gameMode:
					cursorPosition = 0;
					currentStep = home;
					break;
				case serverNumberOfClient:
					cursorPosition = 0;
					currentStep = gameMode;
					break;
				case clientNumberPlayerName:
					cursorPosition = 0;
					currentStep = gameMode;
					break;
				case clientIpPort:
					cursorPosition = 0;
					currentStep = clientNumberPlayerName;
					break;
				case PlayerTypeMenu:
					cursorPosition = 0;
					switch (GameConfig::Instance().getGameModeType()) {
						case LOCAL:
							currentStep = gameMode;
							break;
						case NET_SERVER:
							BomberNetClient::Instance().disconnectClient();
							BomberNetServer::Instance().stopServer();
							currentStep = serverNumberOfClient;
							break;
						case NET_CLIENT:
							currentStep = clientIpPort;
							BomberNetClient::Instance().disconnectClient();
							break;
					}
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
			case gameMode:
				drawSelectGameModeMenu();
				break;
			case serverNumberOfClient:
				drawServerConfigurationMenu();
				break;
			case clientNumberPlayerName:
				drawClientConfigurationMenu();
				break;
			case clientIpPort:
				drawClientIpMenu();
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
			previousPlayerKeystate[0] = in_keystate[0];
			game->exitGame();
			delete game;
			game = NULL;
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
			Sound::Instance().playBipSound();
			GameConfig::Instance().incPlayerType(cursorPosition);
		}
		if (previousPlayerKeystate[0] & keyPadB && keychange[0]) {
			Sound::Instance().playBipSound();
			GameConfig::Instance().decPlayerType(cursorPosition);
		}

		//player 0 always Human

		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				char playerName[15] = "Joueur ";
				char playerTypeName[6];
				sprintf(playerName, "Joueur %2i : ", (j * 4 + i) + 1);

				int playerColor = blue;
				switch (GameConfig::Instance().getPlayerType(j * 4 + i)) {
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
			copySurfaceToBackRenderer(Sprite::Instance().playerDrawNormal(i, 0, 0, 0, 0), screenBuffer, 54 + (i * 72), 174);
		}

		for (int i = 0; i < 16; i++) {
			if (previousPlayerKeystate[i] & keyPadRight && keychange[i]) {
				GameConfig::Instance().incPlayerSpriteType(i);
				Sound::Instance().playBipSound();
			}
			if (previousPlayerKeystate[i] & keyPadLeft && keychange[i]) {
				GameConfig::Instance().decPlayerSpriteType(i);
				Sound::Instance().playBipSound();
			}
		}

		int list[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < 16; i++) {
			if (GameConfig::Instance().getPlayerType(i) != OFF) {
				int index = GameConfig::Instance().getPlayerSpriteType(i);
				char playerName[10];
				int playerColor = blue;
				switch (GameConfig::Instance().getPlayerType(i)) {
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
				Sprite::Instance().drawText(screenBuffer, 70 + (GameConfig::Instance().getPlayerSpriteType(i) * 72), 216 + (15 * list[index]), playerName, playerColor, true);
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
					GameConfig::Instance().switchSuddenDeathMode();
					break;
				case badBomberOption:
					GameConfig::Instance().switchBadBomberMode();
					break;
				case cpuLevelOption:
					GameConfig::Instance().incIALevel();
					break;
				case timeLevelOption:
					GameConfig::Instance().incTimeOfGame();
					break;
			}
		}
		if (previousPlayerKeystate[0] & keyPadB && keychange[0]) {
			Sound::Instance().playBipSound();
			switch (cursorPosition) {
				case suddenDeathOption:
					GameConfig::Instance().switchSuddenDeathMode();
					break;
				case badBomberOption:
					GameConfig::Instance().switchBadBomberMode();
					break;
				case cpuLevelOption:
					GameConfig::Instance().decIALevel();
					break;
				case timeLevelOption:
					GameConfig::Instance().decTimeOfGame();
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
		sprintf(CPULevel, "%i", GameConfig::Instance().getIALevel());
		char timeOfLevel[7];
		if (GameConfig::Instance().getTimeOfGame() != -1) {
			sprintf(timeOfLevel, "%i", GameConfig::Instance().getTimeOfGame());
		} else {
			sprintf(timeOfLevel, "infini");
		}
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(1), screenBuffer, 33, 183);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "GAME OPTION", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);
		Sprite::Instance().drawText(screenBuffer, 200, 187, "Sudden Death", green, false);
		Sprite::Instance().drawText(screenBuffer, 200, 207, "Bad Bomber", green, false);
		Sprite::Instance().drawText(screenBuffer, 200, 227, "CPU Level", green, false);
		Sprite::Instance().drawText(screenBuffer, 200, 247, "Time", green, false);
		if (GameConfig::Instance().isSuddentDeathMode()) {
			Sprite::Instance().drawText(screenBuffer, 400, 187, "ON", green, false);
		} else {
			Sprite::Instance().drawText(screenBuffer, 400, 187, "OFF", red, false);
		}
		if (GameConfig::Instance().isBadBomberMode()) {
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
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "SELECT THE LEVEL", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);

		if (previousPlayerKeystate[0] & keyPadDown && keychange[0]) {
			Sound::Instance().playBipSound();
			switch (cursorPosition) {
				case 0:
				case 1:
				case 2:
					cursorPosition += 2;
					break;
				case 3:
				case 4:
					cursorPosition++;
					break;
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
					cursorPosition = 0;
					break;
			}
			if (cursorPosition > 19) {
				cursorPosition = 0;
			}
		}

		if (previousPlayerKeystate[0] & keyPadRight && keychange[0]) {
			Sound::Instance().playBipSound();
			cursorPosition++;
			if (cursorPosition > 19) {
				cursorPosition = 0;
			}
		}
		if (previousPlayerKeystate[0] & keyPadUp && keychange[0]) {
			Sound::Instance().playBipSound();
			switch (cursorPosition) {
				case 0:
				case 1:
					cursorPosition = 5;
					break;
				case 2:
				case 3:
				case 4:
					cursorPosition -= 2;
					break;
				case 5:
					cursorPosition--;
					break;
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
					cursorPosition = 4;
					break;
			}
		}
		if (previousPlayerKeystate[0] & keyPadLeft && keychange[0]) {
			Sound::Instance().playBipSound();
			cursorPosition--;
			if (cursorPosition < 0) {
				cursorPosition = 19;
			}
		}

		if (previousPlayerKeystate[0] & keyPadA && keychange[0]) {
			Sound::Instance().playBipSound();
			switch (cursorPosition) {
				case 0:
					GameConfig::Instance().incLevel();
					break;
				case 1:
					GameConfig::Instance().incBombe();
					break;
				case 2:
					GameConfig::Instance().incVariante();
					break;
				case 3:
					GameConfig::Instance().incStrengthBombe();
					break;
				case 4:
					GameConfig::Instance().switchCustomBonus();
					break;
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
					GameConfig::Instance().incBonus((cursorPosition - 5));
					break;
			}
		}
		if (previousPlayerKeystate[0] & keyPadB && keychange[0]) {
			Sound::Instance().playBipSound();
			switch (cursorPosition) {
				case 0:
					GameConfig::Instance().decLevel();
					break;
				case 1:
					GameConfig::Instance().decBombe();
					break;
				case 2:
					GameConfig::Instance().decVariante();
					break;
				case 3:
					GameConfig::Instance().decStrengthBombe();
					break;
				case 4:
					GameConfig::Instance().switchCustomBonus();
					break;
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
					GameConfig::Instance().decBonus((cursorPosition - 5));
					break;
			}
		}

		if (previousPlayerKeystate[0] & keyPadX && keychange[0]) {
			Sound::Instance().playBipSound();
			switch (cursorPosition) {
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
					GameConfig::Instance().inc5Bonus((cursorPosition - 5));
					break;
			}
		}
		if (previousPlayerKeystate[0] & keyPadY && keychange[0]) {
			Sound::Instance().playBipSound();
			switch (cursorPosition) {
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
					GameConfig::Instance().dec5Bonus((cursorPosition - 5));
					break;
			}
		}

		//display information
		copySurfaceToBackRenderer(Sprite::Instance().getLevelPreview(GameConfig::Instance().getLevel()), screenBuffer, 50, 200);
		Level * level = LevelService::Instance().getLevel(GameConfig::Instance().getLevel());
		char num[3];
		sprintf(num, "%i", level->getVariantes(GameConfig::Instance().getVariante())->getNumber());
		char numBombe[3];
		sprintf(numBombe, "%i", GameConfig::Instance().getBombe());
		char strenght[3];
		sprintf(strenght, "%i", GameConfig::Instance().getStrenghtBombe());
		Sprite::Instance().drawText(screenBuffer, 224, 184, "Name : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 474, 184, "Bombe : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 224, 202, "Variante : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 474, 202, "Strenght : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 224, 220, "Description : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 224, 256, "Bonus : ", green, false);
		Sprite::Instance().drawText(screenBuffer, 327, 184, level->getName(), blue, false);
		Sprite::Instance().drawText(screenBuffer, 550, 184, numBombe, blue, false);
		Sprite::Instance().drawText(screenBuffer, 327, 202, num, blue, false);
		Sprite::Instance().drawText(screenBuffer, 550, 202, strenght, blue, false);
		Sprite::Instance().drawText(screenBuffer, 327, 220, level->getVariantes(GameConfig::Instance().getVariante())->getDescriptionLine1(), green, false);
		Sprite::Instance().drawText(screenBuffer, 327, 238, level->getVariantes(GameConfig::Instance().getVariante())->getDescriptionLine2(), green, false);
		if (GameConfig::Instance().isCustomBonus()) {
			Sprite::Instance().drawText(screenBuffer, 327, 256, "Custom", blue, false);
		} else {
			Sprite::Instance().drawText(screenBuffer, 327, 256, "Default", blue, false);
		}
		for (int i = 0; i < nbTypeBonus; i++) {
			sprintf(num, "%i", GameConfig::Instance().getBonus(i));
			copySurfaceToBackRenderer(Sprite::Instance().getBonus(i), screenBuffer, 224 + i * 26, 292);
			Sprite::Instance().drawText(screenBuffer, 232 + i * 26, 306, num, green, true);
		}
		level = NULL;
		refreshBuffer = false;

	}
	int cursorPosX = 0;
	int cursorposY = 0;

	switch (cursorPosition) {
		case 0:
			cursorPosX = 200;
			cursorposY = 184;
			break;
		case 1:
			cursorPosX = 450;
			cursorposY = 184;
			break;
		case 2:
			cursorPosX = 200;
			cursorposY = 202;
			break;
		case 3:
			cursorPosX = 450;
			cursorposY = 202;
			break;
		case 4:
			cursorPosX = 200;
			cursorposY = 256;
			break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
			cursorPosX = 224 + ((cursorPosition - 5) * 26);
			cursorposY = 274;
			break;
	}
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
	copySurfaceToBackRenderer(cursor.getCurrentFrame(), vout_buf, cursorPosX, cursorposY);
}

void Bomberman::drawSelectGameModeMenu() {
	if (refreshBuffer || keychange[0]) {
		SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "Select your game Mode", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - Use LEFT / RIGHT to change the mode - -", gold, true);

		if (previousPlayerKeystate[0] & keyPadRight && keychange[0]) {
			Sound::Instance().playBipSound();
			GameConfig::Instance().incGameModeType();
		} else if (previousPlayerKeystate[0] & keyPadLeft && keychange[0]) {
			Sound::Instance().playBipSound();
			GameConfig::Instance().decGameModeType();
		}

		switch (GameConfig::Instance().getGameModeType()) {
			case LOCAL:
				Sprite::Instance().drawText(screenBuffer, (640 / 2), 254, "LOCAL", green, true);
				break;
			case NET_SERVER:
				Sprite::Instance().drawText(screenBuffer, (640 / 2), 254, "SERVER", red, true);
				break;
			case NET_CLIENT:
				Sprite::Instance().drawText(screenBuffer, (640 / 2), 254, "CLIENT", gold, true);
				break;
		}
		copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
	}
}

void Bomberman::drawServerConfigurationMenu() {
	if (refreshBuffer || keychange[0]) {
		SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "SERVER CONFIGURATION", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);

		int cursorPosX = 36 + (((cursorPosition - (cursorPosition % 4)) / 4) * 133);
		int cursorposY = 187 + ((cursorPosition % 4) * 20);
		copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
		copySurfaceToBackRenderer(cursor.getCurrentFrame(), vout_buf, cursorPosX, cursorposY);
	}
}

void Bomberman::drawClientConfigurationMenu() {
	if (refreshBuffer || keychange[0]) {
		SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "CLIENT CONFIGURATION", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);

		int cursorPosX = 36 + (((cursorPosition - (cursorPosition % 4)) / 4) * 133);
		int cursorposY = 187 + ((cursorPosition % 4) * 20);
		copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
		copySurfaceToBackRenderer(cursor.getCurrentFrame(), vout_buf, cursorPosX, cursorposY);
	}
}

void Bomberman::drawClientIpMenu() {
	if (refreshBuffer || keychange[0]) {
		SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
		copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "CLIENT CONFIGURATION", green, true);
		Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);

		int cursorPosX = 36 + (((cursorPosition - (cursorPosition % 4)) / 4) * 133);
		int cursorposY = 187 + ((cursorPosition % 4) * 20);
		copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
		copySurfaceToBackRenderer(cursor.getCurrentFrame(), vout_buf, cursorPosX, cursorposY);
	}
}
