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

void Grid::reset() {
	emptyCase.clear();
	notEmptyCase.clear();
	for (int j = 0; j < sizeY; j++) {
		for (int i = 0; i < sizeX; i++) {
			tab[i+j*sizeX] = emptyElement;
			tabBonus[i+j*sizeX] = noBonus;
		}
	}
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

bool Grid::somethingInSky(){
	return somethingDrawInSky;
}

void Grid::generateGrid() {
	somethingDrawInSky = false;
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

	for(int j=0;j<sizeY;j++) {
		for(int i=0;i<sizeX;i++) {
			tab[i+j*sizeX] = emptyElement;
			tabBonus[i+j*sizeX] = noBonus;
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
				emptyCase[i] = i;
				dstrect.x = (i-1) * smallSpriteLevelSizeWidth;
				dstrect.y = (j-1) * smallSpriteLevelSizeHeight;
				dstrect.w = largeSpriteLevelSizeWidth;
				dstrect.h = largeSpriteLevelSizeHeight;
				SDL_BlitSurface(Sprite::Instance().getLevel(skyStartSpriteIndex, lvl), &skyRect, skyFixe, &dstrect);
				somethingDrawInSky = true;
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
								notEmptyCase[i+(j*sizeX)] = i+(j*sizeX);
								dstrect.x = i * smallSpriteLevelSizeWidth;
								dstrect.y = j * smallSpriteLevelSizeHeight;
								dstrect.w = smallSpriteLevelSizeWidth;
								dstrect.h = smallSpriteLevelSizeHeight;
								SDL_BlitSurface(Sprite::Instance().getLevel(21, lvl), &srcrect, brickShadow, &dstrect);
							}
						} else {
							emptyCase[i+(j*sizeX)] = i+(j*sizeX);
						}
					}
				} else {
					emptyCase[i+(j*sizeX)];
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
		int ind = getEmptyCaseAlea();
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
				int ind = getNotEmptyCaseAlea();
				tabBonus[ind] = y;
			}
		}
	}
}

void Grid::burnABrick(int posX, int posY) {
	if (tab[posX + posY * sizeX] == brickElement) {
		eraseArea(posX, posY);
	}
	notEmptyCase.erase(posX + (posY * sizeX));
}

void Grid::burnBonus(int posX, int posY) {
	eraseArea(posX, posY);
	if (tabBonus[posX + posY * sizeX] != noBonus) {
		if (tabBonus[posX + posY * sizeX] == deathBonus) {
			placeNewDeathMalus();
		}
		tabBonus[posX + posY * sizeX] = noBonus;
	}
}

void Grid::eraseArea(int posX, int posY) {
	SDL_Rect rect;
	rect.x = posX * smallSpriteLevelSizeWidth;
	rect.y = posY * smallSpriteLevelSizeHeight;
	rect.w = smallSpriteLevelSizeWidth;
	rect.h = smallSpriteLevelSizeHeight;
	SDL_FillRect(brickShadow, &rect, 0x000000);
}

void Grid::drawBonus(int index) {
	drawBonus(index % sizeX, floor(index / 35));
}

void Grid::drawBonus(int posX, int posY) {
	if (tabBonus[posX + posY * sizeX] != noBonus) {
		SDL_Rect dstrect;
		dstrect.x = posX * smallSpriteLevelSizeWidth + 1;
		dstrect.y = posY * smallSpriteLevelSizeHeight;
		dstrect.w = defaultSpriteSize;
		dstrect.h = defaultSpriteSize;
		SDL_BlitSurface(Sprite::Instance().getBonus(tabBonus[posX + posY * sizeX]), NULL, brickShadow, &dstrect);
	}
}

void Grid::placeNewDeathMalus() {
	int ind = getEmptyCaseAlea();
	tabBonus[ind] = deathBonus;
	int posX = (ind % sizeX);
	int posY = floor(ind / sizeX);
	drawBonus(posX, posY);
}

void Grid::placeSuddenDeathWall(int x, int y) {
	SDL_Rect dstrect;
	dstrect.x = x * smallSpriteLevelSizeWidth;
	dstrect.y = y * smallSpriteLevelSizeHeight;
	dstrect.w = smallSpriteLevelSizeWidth;
	dstrect.h = smallSpriteLevelSizeHeight;
	SDL_BlitSurface(Sprite::Instance().getLevel(suddenDeathWallSpriteIndex, lvl), NULL, brickShadow, &dstrect);
	tab[x + y * sizeX] = suddenDeathElement;
	emptyCase.erase(x + (y * sizeX));
}

int Grid::playerDeadNeedBonus(int bonusIndex) {
	int ind = getEmptyCaseAlea();
	int nbTry = 0;
	while (tabBonus[ind] != noBonus) {
		if (nbTry > 4)
			break;
		ind = getEmptyCaseAlea();
		nbTry++;
	}
	tabBonus[ind] = bonusIndex;
	int posX = (ind % sizeX);
	int posY = floor(ind / sizeX);
	drawBonus(posX, posY);
	return ind;
}

int Grid::getEmptyCaseAlea() {
	std::map<int, int>::iterator it = emptyCase.begin();
	std::advance(it, rand() % emptyCase.size());
	int ind = it->first;
	while (tabBonus[ind] != noBonus) {
		it = emptyCase.begin();
		std::advance(it, rand() % emptyCase.size() + 1);
		ind = it->first;
	}
	return ind;
}
int Grid::getNotEmptyCaseAlea() {
	std::map<int, int>::iterator it = notEmptyCase.begin();
	std::advance(it, rand() % notEmptyCase.size());
	int ind = it->first;
	while (tabBonus[ind] != noBonus) {
		it = notEmptyCase.begin();
		std::advance(it, rand() % notEmptyCase.size() + 1);
		ind = it->first;
	}
	return ind;
}

//std::map <int,int> test;
//	test[3] = 1;
//	test [12] = 2;
//	test [10] = 2;
//	test [5] = 2;
//	std::map<int,int>::iterator it = test.begin();
//	fprintf(stderr, "%i\n", it->first);
//	std::advance(it,2);
//	fprintf(stderr, "%i\n", it->first);
//	fprintf(stderr, "%i\n", test[3]);
//	fprintf(stderr, "%i\n", test.size());