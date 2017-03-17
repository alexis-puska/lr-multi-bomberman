#include "ClientViewer.h"

ClientViewer::ClientViewer(SDL_Surface * vout_bufLibretro) {
	fprintf(stderr, "INIT VIEWER\n");
	Uint32 rmask, gmask, bmask, amask;
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
	screenBuffer = SDL_CreateRGBSurface(0, 640, 360, 32, rmask, gmask, bmask, amask);
	drawServerWaitForClient();
}

ClientViewer::~ClientViewer() {
	fprintf(stderr, "DESTROY VIEWER\n");
	this->vout_buf = NULL;
	SDL_FreeSurface(screenBuffer);
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

void ClientViewer::decode(char data[512]) {
	fprintf(stderr, "decode request\n");
	fprintf(stderr, "%i %i %i\n", data[5], data[6], data[7]);
	int positionObjectType = 6;
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
						positionObjectType += levelInfoRequest;
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
		}
	}
}

/***********************************
 *
 * MENU PART DRAW
 *
 ***********************************/

void ClientViewer::drawServerWaitForClient() {

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
	Sound::Instance().playBipSound();
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
	Sound::Instance().playBipSound();
	SDL_BlitSurface(Sprite::Instance().getMenuBackground(), NULL, screenBuffer, NULL);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(0), screenBuffer, 33, 150);
	copySurfaceToBackRenderer(Sprite::Instance().getShadowArea(2), screenBuffer, 33, 183);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 154, "SELECT COSTUME", green, true);
	Sprite::Instance().drawText(screenBuffer, (640 / 2), 335, "- - change with Left / Right - -", gold, true);
	//add player sprite
	for (int i = 0; i < 8; i++) {
		copySurfaceToBackRenderer(Sprite::Instance().playerDrawNormal(i, 0, 0, 0, 0), screenBuffer, 54 + (i * 72), 174);
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
		sprintf(num, "%i", levelInfo[5+i]);
		copySurfaceToBackRenderer(Sprite::Instance().getBonus(i), screenBuffer, 224 + i * 26, 292);
		Sprite::Instance().drawText(screenBuffer, 232 + i * 26, 306, num, green, true);
	}
	level = NULL;

	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);

}



/***********************************
 *
 * GAME PART DRAW
 *
 ***********************************/
