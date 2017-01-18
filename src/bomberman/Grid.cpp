#include "Grid.h"


Grid::Grid() {
	lvl = 0;
	init();
}

Grid::Grid(int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY]) {
	lvl = GameConfig::Instance().getLevel();
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
	srand (time(NULL));

for	(int i = 0; i < sizeX * sizeY; i++) {
		tab[i] = emptyElement;
		tabBonus[i] = -1;
	}

	for(int i=0;i<sizeX;i++) {
		for(int j=0;j<sizeY;j++) {
			if( j == 0 || j == (sizeY-1) || i == 0 || i == (sizeX-1)|| (j%2 == 0 && i%2 == 0)) {
				//murs
				tab[i+(j*sizeX)] = wallElement;
				if(LevelService::Instance().getLevel(lvl)->getVariantes(0)->getDefinition(j*sizeX+i) == 18 || LevelService::Instance().getLevel(lvl)->getVariantes(0)->getDefinition(j*sizeX+i) == 19 || LevelService::Instance().getLevel(lvl)->getVariantes(0)->getDefinition(j*sizeX+i) == 20) {
					tab[i+(j*sizeX)] = emptyElement;
					emptyCase.push_back(i+(j*sizeX));
				}
			} else {
				/* generate secret number between 1 and 3: */
				if((rand() % 9 + 1)>=2) {
					if(LevelService::Instance().getLevel(lvl)->getVariantes(0)->isReserved(j*sizeX+i) == 0) {
						tab[i+(j*sizeX)] = brickElement;
						notEmptyCase.push_back(i+(j*sizeX));
					}
				} else {
					tab[i+(j*sizeX)] = emptyElement;
					emptyCase.push_back(i+(j*sizeX));
				}
			}
		}
	}
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

	for(int i = 0; i < sizeX; i++) {
		for(int j = 0; j < sizeY; j++) {
			dstrect.x = i * smallSpriteLevelSizeWidth;
			dstrect.y = j * smallSpriteLevelSizeHeight;
			dstrect.w = smallSpriteLevelSizeWidth;
			dstrect.h = smallSpriteLevelSizeHeight;
			int textureIndex = LevelService::Instance().getLevel(lvl)->getVariantes(0)->getDefinition(j*sizeX+i);
			if(textureIndex == 'S' || textureIndex == 'T'){
				textureIndex -= 65;	
			}
			SDL_BlitSurface(Sprite::Instance().getLevel(18, lvl), &srcrect, ground, &dstrect);
			if(textureIndex < 40) {
				SDL_BlitSurface(Sprite::Instance().getLevel(textureIndex, lvl), &srcrect, ground, &dstrect);
			} else {
				dstrect.x = (i-1) * smallSpriteLevelSizeWidth;
				dstrect.y = (j-1) * smallSpriteLevelSizeHeight;
				dstrect.w = largeSpriteLevelSizeWidth;
				dstrect.h = largeSpriteLevelSizeHeight;
				SDL_BlitSurface(Sprite::Instance().getLevel(skyStartSpriteIndex, lvl), &skyRect, skyFixe, &dstrect);
			}
			if(tab[i+(j*sizeX)] == brickElement) {
				if(LevelService::Instance().getLevel(lvl)->getVariantes(0)->isReserved(j*sizeX+i) == 0) {
					if(textureIndex == 40) {
						dstrect.x = i * smallSpriteLevelSizeWidth;
						dstrect.y = j * smallSpriteLevelSizeHeight;
						dstrect.w = smallSpriteLevelSizeWidth;
						dstrect.h = smallSpriteLevelSizeHeight;
					}
					SDL_BlitSurface(Sprite::Instance().getLevel(21, lvl), &srcrect, brickShadow, &dstrect);
				} else {
					//reservedSpot !
					tab[i+(j*sizeX)] = emptyElement;
				}
			}
		}
	}
	
	

	//draw Death bonus for a level
	for(int i = 0; i < LevelService::Instance().getLevel(lvl)->getVariantes(0)->getBonus(0); i++) {
		int ind = emptyCase[rand() % emptyCase.size() + 1];
		while(LevelService::Instance().getLevel(lvl)->getVariantes(0)->isReserved(ind) != 0) {
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

	//draw Death bonus for a level
	for(int y = 1; y < 13; y++) {
		for(int i = 0; i < LevelService::Instance().getLevel(lvl)->getVariantes(0)->getBonus(y); i++) {
			int ind = notEmptyCase[rand() % notEmptyCase.size()];
			while(tabBonus[ind] != noBonus) {
				ind = notEmptyCase[rand() % notEmptyCase.size()];
			}
			tabBonus[ind] = y;
		}
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

	if (tabBonus[posX + posY * sizeX] != noBonus) {
		SDL_Rect dstrect;
		dstrect.x = posX * smallSpriteLevelSizeWidth + 1;
		dstrect.y = posY * smallSpriteLevelSizeHeight;
		dstrect.w = defaultSpriteSize;
		dstrect.h = defaultSpriteSize;
		SDL_BlitSurface(Sprite::Instance().getBonus(tabBonus[posX + posY * sizeX]), NULL, brickShadow, &dstrect);
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
}
