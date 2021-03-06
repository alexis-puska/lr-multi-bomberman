#include "Hole.h"

Hole::Hole(int index) {
	this->index = index;
	this->position = 0;
	for (int i = 0; i < nbPlayer; i++) {
		activate[i] = false;
	}
}

Hole::~Hole() {
}

bool Hole::doSomething(SDL_Surface * surfaceToDraw) {
	if (position < nbWalkOn) {
		for (int i = 0; i < nbPlayer; i++) {
			if (index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false) {
				position++;
				drawHimself(surfaceToDraw);
				activate[i] = true;
			} else if (index != GameConfig::Instance().getPlayerIndex(i) && activate[i] == true) {
				activate[i] = false;
			}
		}
	}
	if (position == nbWalkOn) {
		return true;
	}
	return false;
}

void Hole::drawHimself(SDL_Surface * surfaceToDraw) {
	SDL_Rect dstRect;
	dstRect.x = (index % 35) * smallSpriteLevelSizeWidth;
	dstRect.y = ((int) floor(index / 35)) * smallSpriteLevelSizeHeight;
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	if (position != 0 && position < nbWalkOn) {
		if (position == 1) {
			Sound::Instance().playHole1Sound();
			if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
				BomberNetServer::Instance().sendSound(6, -1, true);
			}
		} else {
			Sound::Instance().playHole2Sound();
			if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
				BomberNetServer::Instance().sendSound(7, -1, true);
			}
		}
		SDL_BlitSurface(Sprite::Instance().getHole(0), NULL, surfaceToDraw, &dstRect);
		if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
			BomberNetServer::Instance().sendHole(index, 0);
		}
		return;
	} else if (position >= nbWalkOn) {
		Sound::Instance().playHole3Sound();
		SDL_BlitSurface(Sprite::Instance().getHole(1), NULL, surfaceToDraw, &dstRect);
		if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
			BomberNetServer::Instance().sendSound(8, -1, true);
			BomberNetServer::Instance().sendHole(index, 1);
		}
		return;
	}
}

int Hole::getIndex() {
	return index;
}
