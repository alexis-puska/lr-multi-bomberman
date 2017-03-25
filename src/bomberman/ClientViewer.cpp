#include "ClientViewer.h"

ClientViewer::ClientViewer(SDL_Surface * vout_bufLibretro) {
	fprintf(stderr, "INIT VIEWER\n");
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	amask = 0xff000000;
	this->vout_buf = vout_bufLibretro;
	copySurfaceToBackRenderer(Sprite::Instance().getBackground(), vout_buf, 0, 0);
	for (int i = 0; i < 16; i++) {
		playerType[i] = 0;
		playerSprite[i] = 0;
	}
	for (int i = 0; i < 16; i++) {
		gameOption[i] = 0;
	}
	for (int i = 0; i < 16; i++) {
		levelInfo[i] = 0;
	}
	nbConnected[0] = 0;
	nbConnected[1] = 0;
	somethingDrawInSky = false;
	overlayResult = SDL_CreateRGBSurface(0, 250, 200, 32, rmask, gmask, bmask, amask);
	overlay = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	screenBuffer = SDL_CreateRGBSurface(0, 640, 360, 32, rmask, gmask, bmask, amask);
	playerBombeExplode = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	ground = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	brickShadow = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	skyFixe = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	gameState = menu;
	drawServerWaitForClient();

}

ClientViewer::~ClientViewer() {
	fprintf(stderr, "DESTROY VIEWER\n");
	this->vout_buf = NULL;
	SDL_FreeSurface(overlayResult);
	SDL_FreeSurface(overlay);
	SDL_FreeSurface(screenBuffer);
	SDL_FreeSurface(playerBombeExplode);
	SDL_FreeSurface(ground);
	SDL_FreeSurface(brickShadow);
	SDL_FreeSurface(skyFixe);
}

void ClientViewer::copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y) {
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

/***********************************
 *
 * DECODE
 *
 ***********************************/

void ClientViewer::decode(char data[2048]) {
	int positionObjectType = 6;
	SDL_FillRect(playerBombeExplode, NULL, SDL_MapRGBA(playerBombeExplode->format, 0, 0, 0, 0));
	for (int i = 0; i < data[5]; i++) {
		switch (data[positionObjectType]) {
			//draw screen command
			case 1:

				switch (data[positionObjectType + 1]) {
					case nbConnectedScreen:
						drawServerWaitForClient();
						break;
					case playerTypeScreen:
						drawPlayerTypeScreen();
						break;
					case spriteTypeScreen:
						drawSpriteTypeScreen();
						break;
					case gameOptionScreen:
						drawGameOptionScreen();
						break;
					case levelInfoScreen:
						drawLevelInfoScreen();
						break;
					case gameScreen:
						drawGameScreen();
						//positionObjectType += levelInfoRequest;
						break;
				}
				positionObjectType += drawScreenRequest;
				break;
			case 2:
				switch (data[positionObjectType + 1]) {
					case 1:
						nbConnected[0] = data[positionObjectType + 2];
						nbConnected[1] = data[positionObjectType + 3];
						drawServerWaitForClient();
						positionObjectType += nbConnectedRequest;
						break;
					case 2:
						for (int j = 0; j < 16; j++) {
							playerType[j] = data[positionObjectType + 2 + j];
						}
						drawPlayerTypeScreen();
						positionObjectType += playerTypeRequest;
						break;
					case 3:
						for (int j = 0; j < 16; j++) {
							playerSprite[j] = data[positionObjectType + 2 + j];
						}
						drawSpriteTypeScreen();
						positionObjectType += spriteTypeRequest;
						break;
					case 4:
						for (int j = 0; j < 4; j++) {
							gameOption[j] = data[positionObjectType + 2 + j];
						}
						drawGameOptionScreen();
						positionObjectType += gameOptionRequest;
						break;
					case 5:
						for (int j = 0; j < 19; j++) {
							levelInfo[j] = data[positionObjectType + 2 + j];
						}
						drawLevelInfoScreen();
						positionObjectType += levelInfoRequest;
						break;
				}
				break;
			case 3:
				updateGameInfo(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3] == 1 ? true : false, data[positionObjectType + 4]);
				positionObjectType += 5;
				break;
			case 4:
				//SDL_FillRect(brickShadow, NULL, SDL_MapRGBA(brickShadow->format, 0, 0, 0, 0));
				for (int i = 0; i < 735; i++) {
					updateTab(i, data[i + positionObjectType + 1]);
				}
				positionObjectType += 736;
				break;
			case 5:
				for (int i = 0; i < 735; i++) {
					updateTabBonus(i, data[i + positionObjectType + 1]);
				}
				positionObjectType += 736;
				break;
			case 6:
//				fprintf(stderr, "%f %f %i %i %i\n", (float) SDLNet_Read16(data + positionObjectType + 1) / 100.0, (float) SDLNet_Read16(data + positionObjectType + 3) / 100.0,
//						SDLNet_Read16(data + positionObjectType + 5), (Sint16) SDLNet_Read16(data + positionObjectType + 7), data[positionObjectType + 9]);
				drawPlayer((float) SDLNet_Read16(data + positionObjectType + 1) / 100.0, (float) SDLNet_Read16(data + positionObjectType + 3) / 100.0,
						SDLNet_Read16(data + positionObjectType + 5), (Sint16) SDLNet_Read16(data + positionObjectType + 7), data[positionObjectType + 9],
						data[positionObjectType + 10] == 1 ? true : false);
				positionObjectType += 11;
				break;

			case 7:
				for (int i = 0; i < 16; i++) {
					updatePlayerState(i, SDLNet_Read16(data + positionObjectType + (i * 2) + 1));
				}
				positionObjectType += 17;
				break;
			case 8:
				clearArea(SDLNet_Read16(data + positionObjectType + 1));
				positionObjectType += 3;
				break;
			case 9:
				drawRail(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 10:
				drawTrolley((float) SDLNet_Read16(data + positionObjectType + 1) / 100.0, (float) SDLNet_Read16(data + positionObjectType + 3) / 100.0, data[positionObjectType + 5]);
				positionObjectType += 6;
				break;
			case 11:
				drawButton(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 12:
				drawBurnLouis((float) SDLNet_Read16(data + positionObjectType + 1) / 100.0, (float) SDLNet_Read16(data + positionObjectType + 3) / 100.0, data[positionObjectType + 5]);
				positionObjectType += 6;
				break;
			case 13:
				drawBurnBonus(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 14:
				drawBurnWall(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 15:
				drawExplosion(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3], data[positionObjectType + 4]);
				positionObjectType += 5;
				break;
			case 16:
				drawPopBonus(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 17:
				drawSuddentDeath(SDLNet_Read16(data + positionObjectType + 1), SDLNet_Read16(data + positionObjectType + 3), data[positionObjectType + 5]);
				positionObjectType += 6;
				break;
			case 18:
				drawBombe((float) SDLNet_Read16(data + positionObjectType + 1) / 100.0, (float) SDLNet_Read16(data + positionObjectType + 3) / 100.0, data[positionObjectType + 5],
						data[positionObjectType + 6]);
				positionObjectType += 7;
				break;
			case 19:
				drawHole(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 20:
				drawMine(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 21:
				drawTeleporter(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 22:
				drawGhost((float) SDLNet_Read16(data + positionObjectType + 1) / 100.0, (float) SDLNet_Read16(data + positionObjectType + 3) / 100.0);
				positionObjectType += 5;
				break;
			case 23:
				playMusique(data[positionObjectType + 1], data[positionObjectType + 2] == 1 ? true : false);
				positionObjectType += 3;
				break;
			case 24:
				playSound(data[positionObjectType + 1], data[positionObjectType + 2], data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 25:
				drawBonus(SDLNet_Read16(data + positionObjectType + 1), data[positionObjectType + 3]);
				positionObjectType += 4;
				break;
			case 26:
				eraseBonus(SDLNet_Read16(data + positionObjectType + 1));
				positionObjectType += 3;
				break;
		}
	}
	switch (gameState) {
		case menu:
			break;
		case gameViewerStart:
		case gameViewerPause:
		case gameViewerWait:
		case gameViewerEnd:
			mergeScreen(false);
			break;
		case generateViewerResult:
			mergeScreen(true);
			break;
	}

}

/***********************************
 *
 * MENU PART DRAW
 *
 ***********************************/

void ClientViewer::drawServerWaitForClient() {
	this->gameState = -1;
	SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "WAIT FOR GAME CLIENT", green, true);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - just wait for other player - -", gold, true);

	char num[3];
	sprintf(num, "%i", nbConnected[0]);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 214, "Number of bomber net client connecter", green, true);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 234, num, blue, true);
	sprintf(num, "%i", nbConnected[1]);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 254, "Number of HUMAN player", green, true);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 274, num, blue, true);
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
}

void ClientViewer::drawPlayerTypeScreen() {
	this->gameState = -1;
	SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "CONFIGURATION JOUEUR", green, true);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			char playerName[15] = "Joueur ";
			char playerTypeName[6];
			sprintf(playerName, "Joueur %2i : ", (j * 4 + i) + 1);
			int playerColor = blue;
			switch (playerType[j * 4 + i]) {
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
				case NET:
					sprintf(playerTypeName, "NET");
					playerColor = gold;
					break;
			}
			Sprite::Instance().drawText(screenBuffer, 123 + (j * 133), 187 + (i * 20), playerTypeName, playerColor, false);
			Sprite::Instance().drawText(screenBuffer, 56 + (j * 133), 187 + (i * 20), playerName, green, false);
		}
	}
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
}

void ClientViewer::drawSpriteTypeScreen() {
	this->gameState = -1;
	SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "SELECT COSTUME", green, true);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - change with Left / Right - -", gold, true);
	//add player sprite
	for (int i = 0; i < 8; i++) {
		copySurfaceToBackRenderer(Sprite::Instance().getPlayerSprite(Sprite::Instance().playerDrawNormal(i, 0, 0, 0, 0)), screenBuffer, 54 + (i * 72), 174);
	}

	int list[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 16; i++) {
		if (playerType[i] != OFF) {
			int index = playerSprite[i];
			char playerName[10];
			int playerColor = blue;
			switch (playerType[i]) {
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
				case NET:
					sprintf(playerName, "NET %i", i + 1);
					playerColor = gold;
					break;
			}
			Sprite::Instance().drawText(screenBuffer, 70 + (playerSprite[i] * 72), 216 + (15 * list[index]), playerName, playerColor, true);
			list[index] = list[index] + 1;
		}
	}
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
}

void ClientViewer::drawGameOptionScreen() {
	this->gameState = -1;
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
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "GAME OPTION", green, true);
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
	if (gameOption[1]) {
		Sprite::Instance().drawText(screenBuffer, 400, 207, "ON", green, false);
	} else {
		Sprite::Instance().drawText(screenBuffer, 400, 207, "OFF", red, false);
	}
	Sprite::Instance().drawText(screenBuffer, 400, 227, CPULevel, blue, false);
	Sprite::Instance().drawText(screenBuffer, 400, 247, timeOfLevel, blue, false);
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
}

void ClientViewer::drawLevelInfoScreen() {
	this->gameState = -1;
	SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "SELECT THE LEVEL", green, true);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - move cursor with arrow and change with A / B - -", gold, true);
	//display information
	copySurfaceToBackRenderer(Sprite::Instance().getLevelPreview(levelInfo[0]), screenBuffer, 50, 200);
	Level * level = LevelService::Instance().getLevel(levelInfo[0]);
	char num[3];
	sprintf(num, "%i", level->getVariantes(levelInfo[1])->getNumber());
	char numBombe[3];
	sprintf(numBombe, "%i", levelInfo[2]);
	char strenght[3];
	sprintf(strenght, "%i", levelInfo[3]);
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
	Sprite::Instance().drawText(screenBuffer, 327, 220, level->getVariantes(levelInfo[1])->getDescriptionLine1(), green, false);
	Sprite::Instance().drawText(screenBuffer, 327, 238, level->getVariantes(levelInfo[1])->getDescriptionLine2(), green, false);
	if (levelInfo[4] == 1) {
		Sprite::Instance().drawText(screenBuffer, 327, 256, "Custom", blue, false);
	} else {
		Sprite::Instance().drawText(screenBuffer, 327, 256, "Default", blue, false);
	}
	for (int i = 0; i < nbTypeBonus; i++) {
		sprintf(num, "%i", levelInfo[5 + i]);
		copySurfaceToBackRenderer(Sprite::Instance().getBonus(i), screenBuffer, 224 + i * 26, 292);
		Sprite::Instance().drawText(screenBuffer, 232 + i * 26, 306, num, green, true);
	}
	level = NULL;
	for (int i = startMineOffsetChannel; i < nbChannelSound; i++) {
		Sound::Instance().stopMineSound(i);
	}

	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);

}

void ClientViewer::drawGameScreen() {
	fprintf(stderr, "draw game screen");
	SDL_BlitSurface(Sprite::Instance().getBackground(), NULL, vout_buf, NULL);
	generateGround();
}

/***********************************
 *
 * GAME PART DRAW
 *
 ***********************************/

void ClientViewer::generateGround() {
	somethingDrawInSky = false;
	int lvl = levelInfo[0];
	int var = levelInfo[1];
	SDL_Rect dstrect;
	SDL_Rect srcrect;
	SDL_Rect skyRect;
	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = smallSpriteLevelSizeWidth;
	srcrect.h = smallSpriteLevelSizeHeight;

	skyRect.x = 0;
	skyRect.y = 0;
	skyRect.w = largeSpriteLevelSizeWidth;
	skyRect.h = largeSpriteLevelSizeHeight;

	for (int j = 0; j < sizeY; j++) {
		for (int i = 0; i < sizeX; i++) {
			tab[i + j * sizeX] = emptyElement;
			dstrect.x = i * smallSpriteLevelSizeWidth;
			dstrect.y = j * smallSpriteLevelSizeHeight;
			dstrect.w = smallSpriteLevelSizeWidth;
			dstrect.h = smallSpriteLevelSizeHeight;
			SDL_BlitSurface(Sprite::Instance().getLevel(18, lvl), &srcrect, ground, &dstrect);
			if (LevelService::Instance().getLevel(lvl)->getVariantes(var)->isAWall(LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j * sizeX + i))) {
				tab[i + (j * sizeX)] = wallElement;

				int textureIndex = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j * sizeX + i);
				if (textureIndex != 'W') {
					if (textureIndex < 40) {
						dstrect.x = i * smallSpriteLevelSizeWidth;
						dstrect.y = j * smallSpriteLevelSizeHeight;
						dstrect.w = smallSpriteLevelSizeWidth;
						dstrect.h = smallSpriteLevelSizeHeight;
						SDL_BlitSurface(Sprite::Instance().getLevel(textureIndex, lvl), &srcrect, ground, &dstrect);
					}
				}
			} else if (LevelService::Instance().getLevel(lvl)->getVariantes(var)->isDrawInSky(LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j * sizeX + i))) {
				tab[i + (j * sizeX)] = emptyElement;
				dstrect.x = (i - 1) * smallSpriteLevelSizeWidth;
				dstrect.y = (j - 1) * smallSpriteLevelSizeHeight;
				dstrect.w = largeSpriteLevelSizeWidth;
				dstrect.h = largeSpriteLevelSizeHeight;
				SDL_BlitSurface(Sprite::Instance().getLevel(skyStartSpriteIndex, lvl), &skyRect, skyFixe, &dstrect);
			} else {
				somethingDrawInSky = true;
				tab[i + (j * sizeX)] = emptyElement;
				dstrect.x = i * smallSpriteLevelSizeWidth;
				dstrect.y = j * smallSpriteLevelSizeHeight;
				dstrect.w = smallSpriteLevelSizeWidth;
				dstrect.h = smallSpriteLevelSizeHeight;
				int textureIndex = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j * sizeX + i);
				if (textureIndex == 'S' || textureIndex == 'T' || textureIndex == 'U') {
					textureIndex -= 65;
				}
				SDL_BlitSurface(Sprite::Instance().getLevel(textureIndex, lvl), &srcrect, ground, &dstrect);
			}
		}
	}
	for (int j = 0; j < sizeY; j++) {
		for (int i = 0; i < sizeX; i++) {
			if (LevelService::Instance().getLevel(lvl)->getVariantes(var)->isAWall(LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j * sizeX + i))) {
				tab[i + (j * sizeX)] = wallElement;
				int textureIndex = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j * sizeX + i);
				if (textureIndex != 'W') {
					if (textureIndex >= 40) {
						dstrect.x = (i - 1) * smallSpriteLevelSizeWidth;
						dstrect.y = (j - 1) * smallSpriteLevelSizeHeight;
						dstrect.w = largeSpriteLevelSizeWidth;
						dstrect.h = largeSpriteLevelSizeHeight;
						SDL_BlitSurface(Sprite::Instance().getLevel(textureIndex, lvl), &skyRect, ground, &dstrect);
					}
				}
			}
		}
	}
}

/*
 *
 * merge the different layers to generate the game frame
 *
 */
void ClientViewer::mergeScreen(bool mergeResult) {
	SDL_Rect mergeRect;
	mergeRect.x = 0;
	mergeRect.y = 0;
	mergeRect.w = 630;
	mergeRect.h = 336;
	SDL_BlitSurface(ground, &mergeRect, screenBuffer, &mergeRect);
	SDL_BlitSurface(brickShadow, &mergeRect, screenBuffer, &mergeRect);
	SDL_BlitSurface(playerBombeExplode, &mergeRect, screenBuffer, &mergeRect);
	if (somethingDrawInSky) {
		SDL_BlitSurface(skyFixe, &mergeRect, screenBuffer, &mergeRect);
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

void ClientViewer::updateGameInfo(int tickRemaining, bool newCycle, int gameState) {
	this->gameState = gameState;
	this->tickRemaining = tickRemaining;
	this->newCycle = newCycle;
}

void ClientViewer::updateTab(int idx, int val) {
	SDL_Rect dstrect;
	SDL_Rect srcrect;
	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = smallSpriteLevelSizeWidth;
	srcrect.h = smallSpriteLevelSizeHeight;
	if (tab[idx] != val) {
		tab[idx] = val;
		if (tab[idx] == brickElement) {
			dstrect.x = idx % 35 * smallSpriteLevelSizeWidth;
			dstrect.y = ((int) floor(idx / 35)) * smallSpriteLevelSizeHeight;
			dstrect.w = smallSpriteLevelSizeWidth;
			dstrect.h = smallSpriteLevelSizeHeight;
			SDL_BlitSurface(Sprite::Instance().getLevel(21, levelInfo[0]), &srcrect, brickShadow, &dstrect);
		}else if (tab[idx] == emptyElement) {
			SDL_FillRect(brickShadow, &dstrect, SDL_MapRGBA(brickShadow->format, 0, 0, 0, 0));
		}
	}
}

void ClientViewer::updateTabBonus(int idx, int val) {
	if (tabBonus[idx] != val) {
		tabBonus[idx] = val;
		if (tab[idx] != emptyElement) {

		}
	}

}

void ClientViewer::drawPlayer(float posX, float posY, int sprite, int louisSprite, int spaceShipSprite, bool inverse) {
	if (sprite != -1) {
		int sprite_sizeW = 30;
		int sprite_sizeH = 42;
		int blockSizeX = 18;
		int blockSizeY = 16;
		SDL_Rect srcTextureRect;
		SDL_Rect destTextureRect;
		destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
		destTextureRect.y = (posY * blockSizeY) - (sprite_sizeH - 7);
		destTextureRect.w = sprite_sizeW;
		destTextureRect.h = sprite_sizeH;
		srcTextureRect.x = 0;
		srcTextureRect.y = 0;
		srcTextureRect.w = sprite_sizeW;
		srcTextureRect.h = sprite_sizeH;
		if (louisSprite != -1) {
			if (inverse) {
				louisMergebuffer = SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
				SDL_BlitSurface(Sprite::Instance().getPlayerSprite(sprite), &srcTextureRect, louisMergebuffer, &srcTextureRect);
				SDL_BlitSurface(Sprite::Instance().getLouisSprite(louisSprite), &srcTextureRect, louisMergebuffer, &srcTextureRect);
				SDL_BlitSurface(louisMergebuffer, &srcTextureRect, playerBombeExplode, &destTextureRect);
				SDL_FreeSurface(louisMergebuffer);
			} else {
				louisMergebuffer = SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
				SDL_BlitSurface(Sprite::Instance().getLouisSprite(louisSprite), &srcTextureRect, louisMergebuffer, &srcTextureRect);
				SDL_BlitSurface(Sprite::Instance().getPlayerSprite(sprite), &srcTextureRect, louisMergebuffer, &srcTextureRect);
				SDL_BlitSurface(louisMergebuffer, &srcTextureRect, playerBombeExplode, &destTextureRect);
				SDL_FreeSurface(louisMergebuffer);
			}
		} else if (spaceShipSprite != -1) {
			destTextureRect.x = (posX * blockSizeX) - (sprite_sizeW / 2);
			destTextureRect.y = (posY * blockSizeY) - 20;
			louisMergebuffer = SDL_CreateRGBSurface(0, sprite_sizeW, sprite_sizeH, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(Sprite::Instance().getPlayerSprite(sprite), &srcTextureRect, louisMergebuffer, &srcTextureRect);
			SDL_BlitSurface(Sprite::Instance().getSpaceShip(spaceShipSprite), &srcTextureRect, louisMergebuffer, &srcTextureRect);
			SDL_BlitSurface(louisMergebuffer, &srcTextureRect, playerBombeExplode, &destTextureRect);
			SDL_FreeSurface(louisMergebuffer);
		} else {
			SDL_BlitSurface(Sprite::Instance().getPlayerSprite(sprite), &srcTextureRect, playerBombeExplode, &destTextureRect);
		}
	}
}

void ClientViewer::updatePlayerState(int idx, int val) {
	playerState[idx] = val;
}

void ClientViewer::clearArea(int idx) {
	SDL_Rect destTextureRect;
	destTextureRect.x = idx % 35 * 18;
	destTextureRect.y = ((int) floor(idx / 35)) * 16;
	destTextureRect.w = 18;
	destTextureRect.h = 16;
	SDL_FillRect(brickShadow, &destTextureRect, SDL_MapRGBA(brickShadow->format, 0, 0, 0, 0));
}

void ClientViewer::drawRail(int idx, int sprite) {
	SDL_Rect dstRect;
	dstRect.x = (idx % 35) * 18;
	dstRect.y = ((int) floor(idx / 35)) * 16;
	dstRect.w = 18;
	dstRect.h = 16;
	SDL_FillRect(brickShadow, &dstRect, 0x000000);
	SDL_BlitSurface(Sprite::Instance().getRail(sprite), NULL, brickShadow, &dstRect);
}

void ClientViewer::drawTrolley(float posX, float posY, int sprite) {
	SDL_Rect dstRect;
	dstRect.x = (posX * 18) - (30 / 2);
	dstRect.y = (posY * 16) - (42 - 10);
	dstRect.w = 18;
	dstRect.h = 16;
	SDL_BlitSurface(Sprite::Instance().getTrolley(sprite), NULL, playerBombeExplode, &dstRect);
}

void ClientViewer::drawButton(int idx, int sprite) {
}

void ClientViewer::drawBurnLouis(float posX, float posY, int sprite) {
}

void ClientViewer::drawBurnBonus(int idx, int sprite) {
}

void ClientViewer::drawBurnWall(int idx, int sprite) {
	int lvl = levelInfo[0];
	SDL_Rect dstRect;
	dstRect.x = idx % 35 * 18;
	dstRect.y = ((int) floor(idx / 35)) * 16;
	dstRect.w = 18;
	dstRect.h = 16;
	SDL_BlitSurface(Sprite::Instance().getBurnWall(sprite, lvl), NULL, playerBombeExplode, &dstRect);
}

void ClientViewer::drawExplosion(int idx, int type, int sprite) {
	SDL_Rect dstRect;
	dstRect.x = idx % 35 * 18;
	dstRect.y = ((int) floor(idx / 35)) * 16;
	dstRect.w = 18;
	dstRect.h = 16;
	SDL_BlitSurface(Sprite::Instance().getFire(sprite, type), NULL, playerBombeExplode, &dstRect);

}

void ClientViewer::drawPopBonus(int idx, int sprite) {
}

void ClientViewer::drawSuddentDeath(float posX, float posY, int sprite) {
}

void ClientViewer::drawBombe(float posX, float posY, int type, int sprite) {
	SDL_Rect dstRect;
	dstRect.x = (posX * 18) - 8;
	dstRect.y = (posY * 16) - 8;
	dstRect.w = 16;
	dstRect.h = 16;
	SDL_BlitSurface(Sprite::Instance().getBombe(sprite, type), NULL, playerBombeExplode, &dstRect);
}

void ClientViewer::drawHole(int idx, int sprite) {
}

void ClientViewer::drawMine(int idx, int sprite) {
}

void ClientViewer::drawTeleporter(int idx, int sprite) {
}

void ClientViewer::drawGhost(float posX, float posY) {
}

void ClientViewer::playMusique(int musique, bool start) {
}

void ClientViewer::playSound(int sound, int channel, int active) {
	switch (sound) {
		case 0:
			Sound::Instance().playBipSound();
			break;
		case 1:
			Sound::Instance().playBombeBounceSound();
			break;
		case 2:
			Sound::Instance().playPlayerBurnSound();
			break;
		case 3:
			Sound::Instance().playCancelSound();
			break;
		case 4:
			Sound::Instance().playEndSound();
			break;
		case 5:
			Sound::Instance().playFireSound();
			break;
		case 6:
			Sound::Instance().playHole1Sound();
			break;
		case 7:
			Sound::Instance().playHole2Sound();
			break;
		case 8:
			Sound::Instance().playHole3Sound();
			break;
		case 9:
			Sound::Instance().playPlayerKickSound();
			break;
		case 10:
			Sound::Instance().playLouisSound();
			break;
		case 11:
			if (active == 1) {
				Sound::Instance().startMineSound(channel);
			} else {
				Sound::Instance().stopMineSound(channel);
			}
			break;
		case 12:
			Sound::Instance().playTeleporterCloseSound();
			break;
		case 13:
			Sound::Instance().playTeleporterOpenSound();
			break;
		case 14:
			Sound::Instance().playValidSound();
			break;
	}
}

void ClientViewer::drawBonus(int idx, int type) {

}

void ClientViewer::eraseBonus(int idx) {

}