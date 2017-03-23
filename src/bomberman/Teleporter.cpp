#include "Teleporter.h"

Teleporter::Teleporter(int index) {
	srand (time(NULL));this->index = index;
	//for animation
	frameCounter = 0;
	offsetSprite = 0;
	nbFrameForAnimation = 7;
	//for teleportation
	playerToTeleporte = -1;
	indexDestination = -1;
}

Teleporter::~Teleporter() {
	destination.clear();
}

bool Teleporter::doSomething(SDL_Surface * surface) {
	if (!animate) {
		for (int i = 0; i < nbPlayer; i++) {
			if (index == GameConfig::Instance().getPlayerIndex(i) && activate[i] == false) {
				Sound::Instance().playTeleporterOpenSound();
				if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
					BomberNetServer::Instance().sendSound(13, -1, true);
				}
				activate[i] = true;
				animate = true;
				playerToTeleporte = i;
				int j = rand() % destination.size();
				indexDestination = destination[j]->getIndex();
				destination[j]->notifyArrival(playerToTeleporte);
			} else if (index != GameConfig::Instance().getPlayerIndex(i) && activate[i] == true) {
				activate[i] = false;
			}
		}
	} else {
		if (frameCounter > nbFrameTeleporter) {
			frameCounter = 0;
			offsetSprite++;
			if (offsetSprite >= nbFrameForAnimation) {
				offsetSprite = 0;
			}
		}
		frameCounter++;
		int offsetSpriteAnimation = 0;
		switch (offsetSprite) {
			case 0:
				offsetSpriteAnimation = 0;
				keepPlayer();
				break;
			case 1:
				offsetSpriteAnimation = 1;
				keepPlayer();
				break;
			case 2:
				offsetSpriteAnimation = 2;
				keepPlayer();
				break;
			case 3:
				offsetSpriteAnimation = 3;
				teleporte();
				break;
			case 4:
				offsetSpriteAnimation = 2;
				break;
			case 5:
				offsetSpriteAnimation = 1;
				break;
			case 6:
				offsetSpriteAnimation = 0;
				offsetSprite = 0;
				frameCounter = 0;
				animate = false;
				break;
		}
		drawHimself(surface, offsetSpriteAnimation);
	}
	return false;
}

//teleporte the player
void Teleporter::teleporte() {
	if (playerToTeleporte != -1) {
		if (destination.size() != 0) {
			float x = (float) (indexDestination % 35) + 0.5;
			float y = (float) (floor(indexDestination / 35)) + 0.5;
			GameConfig::Instance().updatePlayerPosition(playerToTeleporte, x, y);
			playerToTeleporte = -1;
			activate[playerToTeleporte] = false;
			indexDestination = -1;
		}
		Sound::Instance().playTeleporterCloseSound();
		if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
			BomberNetServer::Instance().sendSound(12, -1, true);
		}
	}
}

//draw the teleporter
void Teleporter::drawHimself(SDL_Surface * surfaceToDraw, int frame) {
	SDL_Rect dstRect;
	dstRect.x = (index % 35) * smallSpriteLevelSizeWidth;
	dstRect.y = ((int) floor(index / 35)) * smallSpriteLevelSizeHeight;
	dstRect.w = smallSpriteLevelSizeWidth;
	dstRect.h = smallSpriteLevelSizeHeight;
	SDL_BlitSurface(Sprite::Instance().getTeleporter(frame), NULL, surfaceToDraw, &dstRect);
	if (GameConfig::Instance().getGameModeType() == NET_SERVER) {
		BomberNetServer::Instance().sendTeleporter(index, frame);
	}

}

//add teleporter destination for choose the destination teleporter
void Teleporter::addDestination(Teleporter * teleporter) {
	if (teleporter->getIndex() != this->index) {
		destination.push_back(teleporter);
	}
}

//notification to the destination teleporter for the arrival of player
void Teleporter::notifyArrival(int playerNumber) {
	activate[playerNumber] = true;
	animate = true;
}

//get index case of teleporter
int Teleporter::getIndex() {
	return this->index;
}

//keep player on the case the time before teleportation
void Teleporter::keepPlayer() {
	float x = (float) (index % 35) + 0.5;
	float y = (float) (floor(index / 35)) + 0.5;
	GameConfig::Instance().updatePlayerPosition(playerToTeleporte, x, y);
}
