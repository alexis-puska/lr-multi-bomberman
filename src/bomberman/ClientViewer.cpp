#include "ClientViewer.h"

ClientViewer::ClientViewer(SDL_Surface * vout_bufLibretro) {
	fprintf(stderr, "INIT VIEWER\n");
	this->vout_buf = vout_bufLibretro;
	for(int i=0;i<16;i++){
		keychange[i] = false;
		previousPlayerKeystate[i] = false;
	}
	//anyPlayerkeychange = false;
	copySurfaceToBackRenderer(Sprite::Instance().getBackground(), vout_buf, 0, 0);
}

ClientViewer::~ClientViewer() {
	fprintf(stderr, "DESTROY VIEWER\n");
	this->vout_buf = NULL;
}

void ClientViewer::decode(char data[512]) {
	fprintf(stderr, "decode request\n");
}

bool ClientViewer::checkKeystate() {
	bool anyPlayerkeychange = false;
	for (int i = 0; i < GameConfig::Instance().getNbPlayerOfClient(); i++) {
		if (previousPlayerKeystate[i] != GameConfig::Instance().getKeystate(i)) {
			keychange[i] = true;
			anyPlayerkeychange = true;
			previousPlayerKeystate[i] = GameConfig::Instance().getKeystate(i);
		} else {
			keychange[i] = false;
		}
	}
	return anyPlayerkeychange;
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
