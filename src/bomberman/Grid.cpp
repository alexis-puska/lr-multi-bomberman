#include "Grid.h"

Grid::Grid() {
	lvl = 0;
	var = 0;
	init();
}

Grid::Grid(int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY]) {
	lvl = GameConfig::Instance().getLevel();
	var = GameConfig::Instance().getVariante();
	this->tab = tab;
	this->tabBonus = tabBonus;
	init();
	generateGrid();
}

Grid::~Grid() {
	SDL_FreeSurface(ground);
	SDL_FreeSurface(brickShadow);
	SDL_FreeSurface(skyFixe);
	tab = NULL;
	tabBonus = NULL;
}

SDL_Surface * Grid::getGroundLayer() {
	return ground;
}

SDL_Surface * Grid::getSkyLayer() {
	return skyFixe;
}

SDL_Surface * Grid::getBricksLayer() {
	return brickShadow;
}

SDL_Surface * Grid::getWallSprite() {
	return Sprite::Instance().getLevel(wallSpriteIndex, lvl);
}

void Grid::init() {
	//declarativ color mask, used for create a RGB surface
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	ground = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	brickShadow = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	skyFixe = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
}

void Grid::resetSurface() {
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_FreeSurface(ground);
	SDL_FreeSurface(brickShadow);
	SDL_FreeSurface(skyFixe);
	ground = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	brickShadow = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	skyFixe = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
}

void Grid::generateGrid() {
	srand (time(NULL));SDL_Rect dstrect;
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

	initRails();
	initButtons();

	for(int j=0;j<sizeY;j++) {
		for(int i=0;i<sizeX;i++) {
			tab[i] = emptyElement;
			tabBonus[i] = -1;
			dstrect.x = i * smallSpriteLevelSizeWidth;
			dstrect.y = j * smallSpriteLevelSizeHeight;
			dstrect.w = smallSpriteLevelSizeWidth;
			dstrect.h = smallSpriteLevelSizeHeight;
			SDL_BlitSurface(Sprite::Instance().getLevel(18, lvl), &srcrect, ground, &dstrect);
			if(LevelService::Instance().getLevel(lvl)->getVariantes(var)->isAWall(LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j*sizeX+i))) {
				tab[i+(j*sizeX)] = wallElement;

				int textureIndex = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j*sizeX+i);
				if(textureIndex != 'W') {
					if(textureIndex < 40) {
						dstrect.x = i * smallSpriteLevelSizeWidth;
						dstrect.y = j * smallSpriteLevelSizeHeight;
						dstrect.w = smallSpriteLevelSizeWidth;
						dstrect.h = smallSpriteLevelSizeHeight;
						SDL_BlitSurface(Sprite::Instance().getLevel(textureIndex, lvl), &srcrect, ground, &dstrect);
					}
				}
			} else if(LevelService::Instance().getLevel(lvl)->getVariantes(var)->isDrawInSky(LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j*sizeX+i))) {
				tab[i+(j*sizeX)] = emptyElement;
				emptyCase.push_back(i);
				dstrect.x = (i-1) * smallSpriteLevelSizeWidth;
				dstrect.y = (j-1) * smallSpriteLevelSizeHeight;
				dstrect.w = largeSpriteLevelSizeWidth;
				dstrect.h = largeSpriteLevelSizeHeight;
				SDL_BlitSurface(Sprite::Instance().getLevel(skyStartSpriteIndex, lvl), &skyRect, skyFixe, &dstrect);
			} else {
				tab[i+(j*sizeX)] = emptyElement;
				dstrect.x = i * smallSpriteLevelSizeWidth;
				dstrect.y = j * smallSpriteLevelSizeHeight;
				dstrect.w = smallSpriteLevelSizeWidth;
				dstrect.h = smallSpriteLevelSizeHeight;
				int textureIndex = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j*sizeX+i);
				if(textureIndex == 'S' || textureIndex == 'T' || textureIndex == 'U') {
					textureIndex -=65;
				}
				SDL_BlitSurface(Sprite::Instance().getLevel(textureIndex, lvl), &srcrect, ground, &dstrect);
				if(LevelService::Instance().getLevel(lvl)->getVariantes(var)->isFillWithBricks()) {
					if(j>0 && j<sizeY-1) {
						if((rand() % 9 + 1)>=2) {
							if(LevelService::Instance().getLevel(lvl)->getVariantes(var)->isReserved(j*sizeX+i) == 0) {
								tab[i+(j*sizeX)] = brickElement;
								notEmptyCase.push_back(i+(j*sizeX));
								dstrect.x = i * smallSpriteLevelSizeWidth;
								dstrect.y = j * smallSpriteLevelSizeHeight;
								dstrect.w = smallSpriteLevelSizeWidth;
								dstrect.h = smallSpriteLevelSizeHeight;
								SDL_BlitSurface(Sprite::Instance().getLevel(21, lvl), &srcrect, brickShadow, &dstrect);
							} else {
								redrawRail(i+(j*sizeX));
								redrawButton(i+(j*sizeX));
							}
						} else {
							emptyCase.push_back(i+(j*sizeX));
							redrawRail(i+(j*sizeX));
							redrawButton(i+(j*sizeX));
						}
					}
				} else {
					for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
						it->second->drawHimself(brickShadow);
					}
					emptyCase.push_back(i+(j*sizeX));
				}
			}
		}
	}
	for(int j=0;j<sizeY;j++) {
		for(int i=0;i<sizeX;i++) {
			if(LevelService::Instance().getLevel(lvl)->getVariantes(var)->isAWall(LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j*sizeX+i))) {
				tab[i+(j*sizeX)] = wallElement;
				int textureIndex = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getDefinition(j*sizeX+i);
				if(textureIndex != 'W') {
					if(textureIndex >= 40) {
						dstrect.x = (i-1) * smallSpriteLevelSizeWidth;
						dstrect.y = (j-1) * smallSpriteLevelSizeHeight;
						dstrect.w = largeSpriteLevelSizeWidth;
						dstrect.h = largeSpriteLevelSizeHeight;
						SDL_BlitSurface(Sprite::Instance().getLevel(textureIndex, lvl), &skyRect, ground, &dstrect);
					}
				}
			}
		}
	}

	int nbDeathBonus = 0;
	if(GameConfig::Instance().isCustomBonus()) {
		nbDeathBonus = GameConfig::Instance().getBonus(0);
	} else {
		nbDeathBonus = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getBonus(0);
	}

	//draw Death bonus for a level
	for(int i = 0; i < nbDeathBonus; i++) {
		int ind = emptyCase[rand() % emptyCase.size() + 1];
		while(LevelService::Instance().getLevel(lvl)->getVariantes(var)->isReserved(ind) != 0) {
			ind = emptyCase[rand() % emptyCase.size() + 1];
		}
		tabBonus[ind] = deathBonus;
		SDL_Rect dstrect;
		dstrect.x = ((ind % sizeX) * smallSpriteLevelSizeWidth) + 1;
		dstrect.y = floor(ind / sizeX) * smallSpriteLevelSizeHeight;
		dstrect.w = defaultSpriteSize;
		dstrect.h = defaultSpriteSize;
		SDL_BlitSurface(Sprite::Instance().getBonus(0), NULL, brickShadow, &dstrect);
	}

	if(LevelService::Instance().getLevel(lvl)->getVariantes(var)->isFillWithBricks()) {
		//draw Death bonus for a level
		for(int y = 1; y < 15; y++) {
			int nbBonusType = 0;
			if(GameConfig::Instance().isCustomBonus()) {
				nbBonusType = GameConfig::Instance().getBonus(y);
			} else {
				nbBonusType = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getBonus(y);
			}

			for(int i = 0; i < nbBonusType; i++) {
				int ind = notEmptyCase[rand() % notEmptyCase.size()];
				while(tabBonus[ind] != noBonus) {
					ind = notEmptyCase[rand() % notEmptyCase.size()];
				}
				tabBonus[ind] = y;
			}
		}
	}
}

void Grid::redrawRail(int index) {
	std::map<int, Rail*>::iterator it = rails.find(index);
	if (it != rails.end()) {
		it->second->drawHimself(brickShadow);
	}
	if (tabBonus[index] != noBonus) {
		SDL_Rect dstrect;
		dstrect.x = (index % 35) * smallSpriteLevelSizeWidth + 1;
		dstrect.y = ((int) floor(index / 35)) * smallSpriteLevelSizeHeight;
		dstrect.w = defaultSpriteSize;
		dstrect.h = defaultSpriteSize;
		SDL_BlitSurface(Sprite::Instance().getBonus(tabBonus[index]), NULL, brickShadow, &dstrect);
	}
}

void Grid::redrawButton(int index) {
	std::map<int, Button*>::iterator it = buttons.find(index);
	if (it != buttons.end()) {
		it->second->drawHimself(brickShadow);
	}
}

void Grid::burnABrick(int posX, int posY) {
	if (tab[posX + posY * sizeX] == brickElement) {
		SDL_Rect rect;
		rect.x = posX * smallSpriteLevelSizeWidth;
		rect.y = posY * smallSpriteLevelSizeHeight;
		rect.w = smallSpriteLevelSizeWidth;
		rect.h = smallSpriteLevelSizeHeight;
		SDL_FillRect(brickShadow, &rect, 0x000000);
	}
	redrawRail(posX + (posY * 35));
	redrawButton(posX + (posY * 35));
	if (tabBonus[posX + posY * sizeX] != noBonus) {
		SDL_Rect dstrect;
		dstrect.x = posX * smallSpriteLevelSizeWidth + 1;
		dstrect.y = posY * smallSpriteLevelSizeHeight;
		dstrect.w = defaultSpriteSize;
		dstrect.h = defaultSpriteSize;
		SDL_BlitSurface(Sprite::Instance().getBonus(tabBonus[posX + posY * sizeX]), NULL, brickShadow, &dstrect);
	}
	for (unsigned int i = 0; i < notEmptyCase.size(); i++) {
		if (notEmptyCase[i] == (posX + (posY * sizeX))) {
			//fprintf(stderr, "remove %i ", notEmptyCase[i]);
			notEmptyCase.erase(notEmptyCase.begin() + i);
			emptyCase.push_back(posX + (posY * sizeX));
			break;
		}
	}
}

void Grid::burnBonus(int posX, int posY) {
	if (tabBonus[posX + posY * sizeX] != noBonus) {
		SDL_Rect rect;
		rect.x = posX * smallSpriteLevelSizeWidth;
		rect.y = posY * smallSpriteLevelSizeHeight;
		rect.w = smallSpriteLevelSizeWidth;
		rect.h = smallSpriteLevelSizeHeight;
		SDL_FillRect(brickShadow, &rect, 0x000000);
		if (tabBonus[posX + posY * sizeX] == deathBonus) {
			placeNewDeathMalus();
		}
		tabBonus[posX + posY * sizeX] = noBonus;
	}
	redrawRail(posX + (posY * 35));
	redrawButton(posX + (posY * 35));
}

void Grid::placeNewDeathMalus() {
	int ind = emptyCase[rand() % emptyCase.size() + 1];
	while (tabBonus[ind] != noBonus) {
		ind = emptyCase[rand() % emptyCase.size() + 1];
	}
	tabBonus[ind] = deathBonus;
	SDL_Rect dstrect;
	dstrect.x = ((ind % sizeX) * smallSpriteLevelSizeWidth) + 1;
	dstrect.y = floor(ind / sizeX) * smallSpriteLevelSizeHeight;
	dstrect.w = defaultSpriteSize;
	dstrect.h = defaultSpriteSize;
	SDL_BlitSurface(Sprite::Instance().getBonus(0), NULL, brickShadow, &dstrect);
}

void Grid::placeSuddenDeathWall(int x, int y) {
	SDL_Rect dstrect;
	dstrect.x = x * smallSpriteLevelSizeWidth;
	dstrect.y = y * smallSpriteLevelSizeHeight;
	dstrect.w = smallSpriteLevelSizeWidth;
	dstrect.h = smallSpriteLevelSizeHeight;
	SDL_BlitSurface(Sprite::Instance().getLevel(suddenDeathWallSpriteIndex, lvl), NULL, brickShadow, &dstrect);
	tab[x + y * sizeX] = suddenDeathElement;
	for (unsigned int i = 0; i < emptyCase.size(); i++) {
		if (emptyCase[i] == (x + (y * sizeX))) {
			//fprintf(stderr, "remove %i ", emptyCase[i]);
			emptyCase.erase(emptyCase.begin() + i);
			break;
		}
	}
}

int Grid::playerDeadNeedBonus(int bonusIndex) {
	int ind = emptyCase[rand() % emptyCase.size()];
	int nbTry = 0;
	while (tabBonus[ind] != noBonus) {
		if (nbTry > 4)
			break;
		ind = emptyCase[rand() % emptyCase.size()];
		nbTry++;
	}
	tabBonus[ind] = bonusIndex;
	SDL_Rect dstrect;
	dstrect.x = ((ind % sizeX) * smallSpriteLevelSizeWidth) + 1;
	dstrect.y = floor(ind / sizeX) * smallSpriteLevelSizeHeight;
	dstrect.w = defaultSpriteSize;
	dstrect.h = defaultSpriteSize;
	SDL_BlitSurface(Sprite::Instance().getBonus(tabBonus[ind]), NULL, brickShadow, &dstrect);
	return ind;
}

void Grid::initRails() {
	std::map<int, RailSwitch *> railsIndex = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getRailsIndex();
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
		int index = 0;
		if (rails.size() != 0) {

			for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {

				it->second->init(rails);
			}
			for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
				if (it->second->isBumper()) {
					index = it->second->getIndex();
					break;
				}
			}
//			Rail * rail = rails.find(index)->second;
//			fprintf(stderr, "rail %i start, next %i", rail->getIndex(), rail->getNext(index));
//			rail = rails.find(rail->getNext(index))->second;
//			while (true) {
//				if (!rail->isBumper()) {
//					rail = rails.find(rail->getNext(index))->second;
//					fprintf(stderr, "rail %i start", rail->getIndex());
//				} else {
//					break;
//				}
//			}
//			rails.find(112)->second->switching();
//
//			for (std::map<int, Rail*>::iterator it = rails.begin(); it != rails.end(); ++it) {
//				if (it->second->isBumper()) {
//					index = it->second->getIndex();
//					break;
//				}
//			}
//			fprintf(stderr, "%i index found\n", index);
//			rail = rails.find(index)->second;
//			fprintf(stderr, "rail %i start, next %i", rail->getIndex(), rail->getNext(index));
//			rail = rails.find(rail->getNext(index))->second;
//			while (true) {
//				if (!rail->isBumper()) {
//					rail = rails.find(rail->getNext(index))->second;
//					fprintf(stderr, "rail %i start", rail->getIndex());
//				} else {
//					break;
//				}
//			}
		}

	}
}

void Grid::initButtons() {
	std::vector<int> buttonsIndex = LevelService::Instance().getLevel(lvl)->getVariantes(var)->getButtonsIndex();
	if (buttonsIndex.size() != 0) {
		for (int i = 0; i < buttonsIndex.size(); i++) {
			int indexButton = buttonsIndex[i];
			Button * button = new Button(indexButton);
			buttons[indexButton] = button;
		}
	}
}

void Grid::buttonDoSomething() {
	for (std::map<int, Button*>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
		if (it->second->doSomething(brickShadow)) {
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
