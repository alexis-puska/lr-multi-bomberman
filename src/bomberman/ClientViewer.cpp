#include "ClientViewer.h"

ClientViewer::ClientViewer(SDL_Surface * vout_bufLibretro) {
	fprintf(stderr, "INIT VIEWER\n");
	this->vout_buf = vout_bufLibretro;
	copySurfaceToBackRenderer(Sprite::Instance().getBackground(), vout_buf, 0, 0);
}

ClientViewer::~ClientViewer() {
	fprintf(stderr, "DESTROY VIEWER\n");
	this->vout_buf = NULL;
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

void ClientViewer::decode(char data[512]) {
	fprintf(stderr, "decode request\n");
	fprintf(stderr, "%i %i %i\n", data[5], data[6], data[7]);
	int positionObjectType = 6;
	for (int i = 0; i < data[5]; i++) {
		switch (data[positionObjectType]) {
			//draw screen command
			case 1:

				switch (data[positionObjectType + 1]) {
					case 1:
						drawPlayerTypeScreen();
						break;
					case 2:
						drawSpriteTypeScreen();
						break;
					case 3:
						drawGameOptionScreen();
						break;
					case 4:
						drawLevelInfoScreen();
						break;
				}
				positionObjectType += drawScreenRequest;
				break;
			case 2:
				switch (data[positionObjectType + 1]) {
					case 1:
						positionObjectType += playerTypeRequest;
						break;
					case 2:
						positionObjectType += spriteTypeRequest;
						break;
					case 3:
						positionObjectType += gameOptionRequest;
						break;
					case 4:
						positionObjectType += levelInfoRequest;
						break;
					case 5:
						positionObjectType += levelInfoRequest;
						break;
				}
		}
	}
}

void ClientViewer::drawPlayerTypeScreen() {
	fprintf(stderr, "drawPlayerTypeScreen();\n");
}
void ClientViewer::drawSpriteTypeScreen() {
	fprintf(stderr, "drawSpriteTypeScreen();\n");
}
void ClientViewer::drawGameOptionScreen() {
	fprintf(stderr, "drawGameOptionScreen();\n");
}
void ClientViewer::drawLevelInfoScreen() {
	fprintf(stderr, "drawLevelInfoScreen();\n");
}
