#include "Grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define blockSizeX 18
#define blockSizeY 16
#define largeBlockSizeX 54
#define largeBlockSizeY 48

const static char *levelSprite = "./resources/sprite/level/AllLevel.png";

Grid::Grid() {
	lvl = 0;
	init();
}

Grid::Grid(int lvl, int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY], SDL_Surface ** bonusSprite) {
	this->bonusSprite = bonusSprite;
	this->lvl = lvl;
	this->tab = tab;
	this->tabBonus = tabBonus;
	init();
	generateGrid();
}

Grid::~Grid() {
	int i;
	for (i = 0; i < 40; i++) {
		SDL_FreeSurface (textures[i]);
	}
	for (i = 0; i < 2; i++) {
		SDL_FreeSurface (skys[i]);
	}
	free (textures);
	free (skys);

	SDL_FreeSurface (ground);
	SDL_FreeSurface (brickShadow);
	SDL_FreeSurface (skyFixe);
	tab = NULL;
	tabBonus = NULL;
	bonusSprite = NULL;
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
	return textures[16];
}

void Grid::configure(int levelNumber) {

	lvl = levelNumber;
	tab = new int[sizeX * sizeY];
	tabBonus = new int[sizeX * sizeY];
	init();
	generateGrid();
}

void Grid::init() {
	//declarativ color mask, used for create a RGB surface
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;

	//buffer for loading textures
	SDL_Surface *textureBuffer = IMG_Load(levelSprite);

	//initialise the surface Tab
	textures = new SDL_Surface*[40];
	skys = new SDL_Surface*[2];

	//rectangle for cropped the texture
	SDL_Rect textureRect;
	SDL_Rect skyRect;
	SDL_Rect destTextureRect;

	int offset = lvl * 128;

	//CROPPING textures level
	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = blockSizeX;
	destTextureRect.h = blockSizeY;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			textureRect.x = i * blockSizeX;
			textureRect.y = offset + (j * blockSizeY);
			textureRect.w = blockSizeX;
			textureRect.h = blockSizeY;
			textures[i + (j * 5)] = SDL_CreateRGBSurface(0, blockSizeX, blockSizeY, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(textureBuffer, &textureRect, textures[i + (j * 5)], &destTextureRect);
		}
	}

	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = 54;
	destTextureRect.h = 48;
	for (int k = 0; k < 2; k++) {
		skyRect.x = 90;
		skyRect.y = offset + (k * 48);
		skyRect.w = 54;
		skyRect.h = 48;
		skys[k] = SDL_CreateRGBSurface(0, 54, 48, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(textureBuffer, &skyRect, skys[k], &destTextureRect);
	}
	ground = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	brickShadow = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	skyFixe = SDL_CreateRGBSurface(0, 630, 336, 32, rmask, gmask, bmask, amask);
	SDL_FreeSurface(textureBuffer);
}

void Grid::resetSurface() {
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_FreeSurface (ground);
	SDL_FreeSurface (brickShadow);
	SDL_FreeSurface (skyFixe);
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
				if(level[lvl][j][i] == 18 || level[lvl][j][i] == 19 || level[lvl][j][i] == 20) {
					tab[i+(j*sizeX)] = emptyElement;
					emptyCase.push_back(i+(j*sizeX));
				}
			} else {
				/* generate secret number between 1 and 3: */
				if((rand() % 7 + 1)>=2) {
					if(reservedSpot[j][i] == 0) {
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
	srcrect.w = blockSizeX;
	srcrect.h = blockSizeY;

	skyRect.x = 0;
	skyRect.y = 0;
	skyRect.w = largeBlockSizeX;
	skyRect.h = largeBlockSizeY;

	for(int i = 0; i < sizeX; i++) {
		for(int j = 0; j < sizeY; j++) {
			dstrect.x = i * blockSizeX;
			dstrect.y = j * blockSizeY;
			dstrect.w = blockSizeX;
			dstrect.h = blockSizeY;
			int textureIndex = level[lvl][j][i];
			SDL_BlitSurface(textures[18], &srcrect, ground, &dstrect);
			if(textureIndex < 40) {
				SDL_BlitSurface(textures[textureIndex], &srcrect, ground, &dstrect);
			} else {
				dstrect.x = (i-1) * blockSizeX;
				dstrect.y = (j-1) * blockSizeY;
				dstrect.w = largeBlockSizeX;
				dstrect.h = largeBlockSizeY;
				SDL_BlitSurface(skys[textureIndex % 40], &skyRect, skyFixe, &dstrect);
			}
			if(tab[i+(j*sizeX)] == brickElement) {
				if(reservedSpot[j][i] == 0) {
					if(textureIndex == 40) {
						dstrect.x = i * blockSizeX;
						dstrect.y = j * blockSizeY;
						dstrect.w = blockSizeX;
						dstrect.h = blockSizeY;
					}
					SDL_BlitSurface(textures[21], &srcrect, brickShadow, &dstrect);
				} else {
					//reservedSpot !
					tab[i+(j*sizeX)] = emptyElement;
				}
			}
		}
	}

	//draw Death bonus for a level
	for(int i = 0; i < bonusByLevel[lvl][0]; i++) {
		int ind = emptyCase[rand() % emptyCase.size() + 1];
		while(reservedSpot[(int)ind / sizeX][(ind % sizeX)] != 0) {
			ind = emptyCase[rand() % emptyCase.size() + 1];
		}
		tabBonus[ind] = deathBonus;
		SDL_Rect dstrect;
		dstrect.x = ((ind % sizeX) * blockSizeX) + 1;
		dstrect.y = floor(ind / sizeX) * blockSizeY;
		dstrect.w = 16;
		dstrect.h = 16;
		SDL_BlitSurface(bonusSprite[0], NULL, brickShadow, &dstrect);
	}

	//draw Death bonus for a level
	for(int y = 1; y < 13; y++) {
		for(int i = 0; i < bonusByLevel[lvl][y]; i++) {
			int ind = notEmptyCase[rand() % notEmptyCase.size()];
			while(tabBonus[ind] != noBonus) {
				ind = notEmptyCase[rand() % notEmptyCase.size()];
			}
			tabBonus[ind] = y;
//			SDL_Rect dstrect;
//			dstrect.x = ((ind % sizeX) * blockSizeX) + 1;
//			dstrect.y = floor(ind / sizeX) * blockSizeY;
//			dstrect.w = 16;
//			dstrect.h = 16;
//			SDL_BlitSurface(bonusSprite[y], NULL, brickShadow, &dstrect);
		}
	}

}

void Grid::burnABrick(int posX, int posY) {
	if (tab[posX + posY * sizeX] == brickElement) {
		SDL_Rect rect;
		rect.x = posX * blockSizeX;
		rect.y = posY * blockSizeY;
		rect.w = blockSizeX;
		rect.h = blockSizeY;
		SDL_FillRect(brickShadow, &rect, 0x000000);
	}

	if (tabBonus[posX + posY * sizeX] != noBonus) {
		SDL_Rect dstrect;
		dstrect.x = posX * blockSizeX + 1;
		dstrect.y = posY * blockSizeY;
		dstrect.w = 16;
		dstrect.h = 16;
		SDL_BlitSurface(bonusSprite[tabBonus[posX + posY * sizeX]], NULL, brickShadow, &dstrect);
	}
}

void Grid::burnBonus(int posX, int posY) {
	if (tabBonus[posX + posY * sizeX] != noBonus) {
		SDL_Rect rect;
		rect.x = posX * blockSizeX;
		rect.y = posY * blockSizeY;
		rect.w = blockSizeX;
		rect.h = blockSizeY;
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
	dstrect.x = ((ind % sizeX) * blockSizeX) + 1;
	dstrect.y = floor(ind / sizeX) * blockSizeY;
	dstrect.w = 16;
	dstrect.h = 16;
	SDL_BlitSurface(bonusSprite[0], NULL, brickShadow, &dstrect);
}

void Grid::placeSuddenDeathWall(int x, int y) {
	SDL_Rect dstrect;
	dstrect.x = x * blockSizeX;
	dstrect.y = y * blockSizeY;
	dstrect.w = 18;
	dstrect.h = 16;
	SDL_BlitSurface(textures[16], NULL, brickShadow, &dstrect);
	tab[x + y * sizeX] = suddenDeathElement;
}
