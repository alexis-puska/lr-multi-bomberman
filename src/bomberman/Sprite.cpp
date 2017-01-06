#include "Sprite.h"
#include <vector>

const static char *splashScreen = "./resources/image/SplashScreen.png";
const static char *menuBackground = "./resources/image/MenuBackground.png";
const static char *spriteBombermanPath = "./resources/sprite/characters/AllBomberman.png";
const static char *spriteBombermanBarbarPath = "./resources/sprite/characters/AllBombermanBarbar.png";
const static char *spriteBombermanChanPath = "./resources/sprite/characters/AllBombermanChan.png";
const static char *spriteBombermanCossakPath = "./resources/sprite/characters/AllBombermanCossak.png";
const static char *spriteBombermanKidPath = "./resources/sprite/characters/AllBombermanKid.png";
const static char *spriteBombermanMexicanPath = "./resources/sprite/characters/AllBombermanMexican.png";
const static char *spriteBombermanPrettyPath = "./resources/sprite/characters/AllBombermanPretty.png";
const static char *spriteBombermanPunkPath = "./resources/sprite/characters/AllBombermanPunk.png";
const static char *spriteLouisPath = "./resources/sprite/characters/AllLouis.png";
const static char *spriteFirePath = "./resources/sprite/other/Fire.png";
const static char *spriteMiscPath = "./resources/sprite/other/Misc.png";
const static char *spriteCursorPath = "./resources/sprite/other/Cursor.png";
const static char *spritePreviewLevelPath = "./resources/sprite/level/LevelView.png";
const static char *spriteLevelPath = "./resources/sprite/level/AllLevel.png";

Sprite Sprite::m_instance = Sprite();

Sprite::Sprite() {
	fprintf(stderr, "Init sprite system\n");
	playerSprite = new SDL_Surface *[nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * nbTypePlayer];
	louisSprite = new SDL_Surface *[nbSpriteLouisX * nbSpriteLouisY * nbTypeLouis];
	fireSprite = new SDL_Surface *[nbFireSpriteX * nbFireSpriteY];
	bombeSprite = new SDL_Surface *[nbBombeSpriteX * nbBombeSpriteY];
	bonusSprite = new SDL_Surface *[nbBonusSpriteX * nbBonusSpriteX];
	previewLevelSprite = new SDL_Surface *[nbLevel];
	levelSprite = new SDL_Surface *[nbLevel * nbSmallSpriteLevelX * nbSmallSpriteLevelY + nbLevel * nbLargeSpriteLevelX * nbLargeSpriteLevelY];
	splashScreenSurface = IMG_Load(splashScreen);
	menuBackgroundSurface = IMG_Load(menuBackground);
	cropSurface();
}

Sprite::~Sprite() {
	fprintf(stderr, "close sprite system\n");
	for (int i = 0; i < nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * nbTypePlayer; i++) {
		SDL_FreeSurface(playerSprite[i]);
	}
	for (int i = 0; i < nbSpriteLouisX * nbSpriteLouisY * nbTypeLouis; i++) {
		SDL_FreeSurface(louisSprite[i]);
	}
	for (int i = 0; i < nbFireSpriteX * nbFireSpriteY; i++) {
		SDL_FreeSurface(fireSprite[i]);
	}
	for (int i = 0; i < nbBombeSpriteX * nbBombeSpriteY; i++) {
		SDL_FreeSurface(bombeSprite[i]);
	}
	for (int i = 0; i < nbBonusSpriteX * nbBonusSpriteX; i++) {
		SDL_FreeSurface(bonusSprite[i]);
	}
	for (int i = 0; i < nbLevel; i++) {
		SDL_FreeSurface(previewLevelSprite[i]);
	}
	for (int i = 0; i < nbLevel * nbSmallSpriteLevelX * nbSmallSpriteLevelY + nbLevel * nbLargeSpriteLevelX * nbLargeSpriteLevelY; i++) {
		SDL_FreeSurface(levelSprite[i]);
	}
	SDL_FreeSurface(splashScreenSurface);
	SDL_FreeSurface(menuBackgroundSurface);
	free(playerSprite);
	free(louisSprite);
	free(fireSprite);
	free(bombeSprite);
	free(bonusSprite);
	free(previewLevelSprite);
	free(levelSprite);
}

Sprite& Sprite::Instance() {
	return m_instance;
}

/***********************************************
 * 
 * 				INFORMATION FUNCTION
 * 
 **********************************************/
void Sprite::getColorInSurface(SDL_Surface* surface) {
	std::vector<int> colors;
	bool found = false;
	//lock the surface for work on it
	if (SDL_MUSTLOCK(surface)) {
		SDL_LockSurface(surface);
	}
	//get pointer on the pixels table
	Uint32 *pixels = (Uint32 *) surface->pixels;
	for (int x = 0; x < surface->w; x++) {
		for (int y = 0; y < surface->h; y++) {
			found = false;
			for (unsigned i = 0; i < colors.size(); i++) {
				if (pixels[y * surface->w + x] == colors[i]) {
					found = true;
					break;
				}
			}
			if (!found) {
				colors.push_back(pixels[y * surface->w + x]);
			}
		}
	}
	fprintf(stderr, "colors in surface : ");
	for (unsigned i = 0; i < colors.size(); i++) {
		fprintf(stderr, "%x, ", colors[i]);
	}
	//unlock the surface after work
	if (SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
}

/***********************************************
 * 
 * 				COLOR FUNCTION
 * 
 **********************************************/
SDL_Surface* Sprite::replaceColor(SDL_Surface* surface, int src, int dest) {
	//lock the surface for work on it
	if (SDL_MUSTLOCK(surface)) {
		SDL_LockSurface(surface);
	}
	//get pointer on the pixels table
	Uint32 *pixels = (Uint32 *) surface->pixels;
	for (int x = 0; x < surface->w; x++) {
		for (int y = 0; y < surface->h; y++) {
			if (pixels[y * surface->w + x] == src) {
				pixels[y * surface->w + x] = dest;
			}
		}
	}
	//unlock the surface after work
	if (SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
	return surface;
}
/*********************************************
 * Change color for player
 * CLASSIQUE
 * 0xfff8f8f8 -> blanc bonnet / corp
 * 0xffa0a0a0 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff0058e8 -> bleu corp
 * 0xff00b018 -> vert slip chapeau veste
 * 0xffe82050 -> gant ponpon chaussure bandeau
 ********************************************/

/*********************************************
 * CLASSIQUE to GREY
 * 0xfff8f8f8 -> blanc bonnet / corp
 * 0xffa0a0a0 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff0058e8 -> bleu corp
 * 0xff00b018 -> vert slip chapeau veste
 * 0xffe82050 -> gant ponpon chaussure bandeau
 *
 * 0xff707070 -> blanc bonnet / corp
 * 0xff303030 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff505050 -> bleu corp
 * 0xffC0C0C0 -> vert slip chapeau veste
 * 0xffE01898 -> gant ponpon chaussure bandeau
 ********************************************/
SDL_Surface* Sprite::upPlayerGreyColor(SDL_Surface* surface) {
	replaceColor(surface, 0xfff8f8f8, 0xff707070);
	replaceColor(surface, 0xffa0a0a0, 0xff303030);
	replaceColor(surface, 0xfff8a020, 0xfff8a020);
	replaceColor(surface, 0xff0058e8, 0xff505050);
	replaceColor(surface, 0xff00b018, 0xffC0C0C0);
	replaceColor(surface, 0xffe82050, 0xffE01898);
	return surface;
}

/*********************************************
 * CLASSIQUE to RED
 * 0xfff8f8f8 -> blanc bonnet / corp
 * 0xffa0a0a0 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff0058e8 -> bleu corp
 * 0xff00b018 -> vert slip chapeau veste
 * 0xffe82050 -> gant ponpon chaussure bandeau
 * 
 * 0xffC00000 -> blanc bonnet / corp
 * 0xff800000 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xffC00000 -> bleu corp
 * 0xffC0C0C0 -> vert slip chapeau veste
 * 0xff30A060 -> gant ponpon chaussure bandeau
 ********************************************/
SDL_Surface* Sprite::upPlayerRedColor(SDL_Surface* surface) {
	replaceColor(surface, 0xfff8f8f8, 0xffC00000);
	replaceColor(surface, 0xffa0a0a0, 0xff800000);
	replaceColor(surface, 0xfff8a020, 0xfff8a020);
	replaceColor(surface, 0xff0058e8, 0xffC00000);
	replaceColor(surface, 0xff00b018, 0xffC0C0C0);
	replaceColor(surface, 0xffe82050, 0xff30A060);
	return surface;
}

/*********************************************
 * CLASSIQUE to BLUE
 * 0xfff8f8f8 -> blanc bonnet / corp
 * 0xffa0a0a0 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff0058e8 -> bleu corp
 * 0xff00b018 -> vert slip chapeau veste
 * 0xffe82050 -> gant ponpon chaussure bandeau
 * 
 * 0xff0058C0 -> blanc bonnet / corp
 * 0xff004080 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff0058C0 -> bleu corp
 * 0xff50C0C0 -> vert slip chapeau veste
 * 0xffE01898 -> gant ponpon chaussure bandeau
 ********************************************/
SDL_Surface* Sprite::upPlayerBlueColor(SDL_Surface* surface) {
	replaceColor(surface, 0xfff8f8f8, 0xff0058C0);
	replaceColor(surface, 0xffa0a0a0, 0xff004080);
	replaceColor(surface, 0xfff8a020, 0xfff8a020);
	replaceColor(surface, 0xff0058e8, 0xff0058C0);
	replaceColor(surface, 0xff00b018, 0xff50C0C0);
	replaceColor(surface, 0xffe82050, 0xffE01898);
	return surface;
}

/*********************************************
 * CLASSIQUE to GREEN
 * 0xfff8f8f8 -> blanc bonnet / corp
 * 0xffa0a0a0 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff0058e8 -> bleu corp
 * 0xff00b018 -> vert slip chapeau veste
 * 0xffe82050 -> gant ponpon chaussure bandeau
 *
 * 0xff10C020 -> blanc bonnet / corp
 * 0xff009010 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff10C020 -> bleu corp
 * 0xffB0D050 -> vert slip chapeau veste
 * 0xffE01898 -> gant ponpon chaussure bandeau
 ********************************************/
SDL_Surface* Sprite::upPlayerGreenColor(SDL_Surface* surface) {
	replaceColor(surface, 0xfff8f8f8, 0xff10C020);
	replaceColor(surface, 0xffa0a0a0, 0xff009010);
	replaceColor(surface, 0xfff8a020, 0xfff8a020);
	replaceColor(surface, 0xff0058e8, 0xff10C020);
	replaceColor(surface, 0xff00b018, 0xffB0D050);
	replaceColor(surface, 0xffe82050, 0xffE01898);
	return surface;
}

/*********************************************
 * CLASSIQUE to GOLD
 * 0xfff8f8f8 -> blanc bonnet / corp
 * 0xffa0a0a0 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff0058e8 -> bleu corp
 * 0xff00b018 -> vert slip chapeau veste
 * 0xffe82050 -> gant ponpon chaussure bandeau
 * 
 * 0xffF8E000 -> blanc bonnet / corp
 * 0xffB0A000 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xffE8B020 -> bleu corp
 * 0xff00B018 -> vert slip chapeau veste
 * 0xffD06800 -> gant ponpon chaussure bandeau
 ********************************************/
SDL_Surface* Sprite::upPlayerGoldColor(SDL_Surface* surface) {
	replaceColor(surface, 0xfff8f8f8, 0xffF8E000);
	replaceColor(surface, 0xffa0a0a0, 0xffB0A000);
	replaceColor(surface, 0xfff8a020, 0xfff8a020);
	replaceColor(surface, 0xff0058e8, 0xffE8B020);
	replaceColor(surface, 0xff00b018, 0xff00B018);
	replaceColor(surface, 0xffe82050, 0xffD06800);
	return surface;
}

/*********************************************
 * CLASSIQUE to BROWN
 * 0xfff8f8f8 -> blanc bonnet / corp
 * 0xffa0a0a0 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xff0058e8 -> bleu corp
 * 0xff00b018 -> vert slip chapeau veste
 * 0xffe82050 -> gant ponpon chaussure bandeau
 *
 * 0xffB05040 -> blanc bonnet / corp
 * 0xff800010 -> gris bonnet
 * 0xfff8a020 -> visage
 * 0xffB0B0B0 -> bleu corp
 * 0xffB0B0B0 -> vert slip chapeau veste
 * 0xffF8B000 -> gant ponpon chaussure bandeau
 ********************************************/
SDL_Surface* Sprite::upPlayerBrownColor(SDL_Surface* surface) {
	replaceColor(surface, 0xfff8f8f8, 0xffB05040);
	replaceColor(surface, 0xffa0a0a0, 0xff800010);
	replaceColor(surface, 0xfff8a020, 0xfff8a020);
	replaceColor(surface, 0xff0058e8, 0xffB0B0B0);
	replaceColor(surface, 0xff00b018, 0xffB0B0B0);
	replaceColor(surface, 0xffe82050, 0xffF8B000);
	return surface;
}

/*********************************************
 * Apply a ramplace set of color to a playe sprite
 * 0 : None
 * 1 : Grey
 * 2 : Red
 * 3 : Blue
 * 4 : Green
 * 5 : Gold
 * 6 : Brown
 ********************************************/
SDL_Surface* Sprite::applyPlayerColor(SDL_Surface * surface, int color) {
	switch (color) {
		case 0:
			return surface;
		case 1:
			return upPlayerGreyColor(surface);
		case 2:
			return upPlayerRedColor(surface);
		case 3:
			return upPlayerBlueColor(surface);
		case 4:
			return upPlayerGreenColor(surface);
		case 5:
			return upPlayerGoldColor(surface);
		case 6:
			return upPlayerBrownColor(surface);
	}
	return surface;
}

/*********************************************
 * CLASSIQUE to GOLD
 * 0xff0051b5 -> dark blue
 * 0xff00a2ff -> light blue
 *
 * 0xffe8b000 -> dark gold
 * 0xfff8d000 -> light gold
 ********************************************/
SDL_Surface* Sprite::upLouisGoldColor(SDL_Surface* surface) {
	replaceColor(surface, 0xff0051b5, 0xffe8b000);
	replaceColor(surface, 0xff00a2ff, 0xfff8d000);
	return surface;
}

/*********************************************
 * CLASSIQUE to GREEN
 * 0xff0051b5 -> dark blue
 * 0xff00a2ff -> light blue
 *
 * 0xffd050d0 -> dark green
 * 0xfff878f8 -> light green
 ********************************************/
SDL_Surface* Sprite::upLouisGreenColor(SDL_Surface* surface) {
	replaceColor(surface, 0xff0051b5, 0xffd050d0);
	replaceColor(surface, 0xff00a2ff, 0xfff878f8);
	return surface;
}

/*********************************************
 * CLASSIQUE to PINK
 * 0xff0051b5 -> dark blue
 * 0xff00a2ff -> light blue
 *
 * 0xff00b010 -> dark pink
 * 0xff00f830 -> light pink
 ********************************************/
SDL_Surface* Sprite::upLouisPinkColor(SDL_Surface* surface) {
	replaceColor(surface, 0xff0051b5, 0xff00b010);
	replaceColor(surface, 0xff00a2ff, 0xff00f830);
	return surface;
}

/*********************************************
 * CLASSIQUE to BROWN
 * 0xff0051b5 -> dark blue
 * 0xff00a2ff -> light blue
 *
 * 0xff684040 -> dark brown
 * 0xff906868 -> light brown
 ********************************************/
SDL_Surface* Sprite::upLouisBrownColor(SDL_Surface* surface) {
	replaceColor(surface, 0xff0051b5, 0xff684040);
	replaceColor(surface, 0xff00a2ff, 0xff906868);
	return surface;
}

/*********************************************
 * Apply a ramplace set of color to a louis sprite
 * 0 : None
 * 1 : Gold
 * 2 : Green
 * 3 : Pink
 * 4 : Brown
 ********************************************/
SDL_Surface* Sprite::applyLouisColor(SDL_Surface * surface, int color) {
	switch (color) {
		case 0:
			return surface;
		case 1:
			return upLouisGoldColor(surface);
		case 2:
			return upLouisGreenColor(surface);
		case 3:
			return upLouisPinkColor(surface);
		case 4:
			return upLouisBrownColor(surface);
	}
	return surface;
}

/********************************************
 * 
 *		CROP SURFACE
 * 
 ********************************************/
void Sprite::cropSurface() {
	cropPlayerSurface(IMG_Load(spriteBombermanPath), nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 0);
	cropPlayerSurface(IMG_Load(spriteBombermanCossakPath), nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 1);
	cropPlayerSurface(IMG_Load(spriteBombermanBarbarPath), nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 2);
	cropPlayerSurface(IMG_Load(spriteBombermanChanPath), nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 3);
	cropPlayerSurface(IMG_Load(spriteBombermanKidPath), nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 4);
	cropPlayerSurface(IMG_Load(spriteBombermanPrettyPath), nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 5);
	cropPlayerSurface(IMG_Load(spriteBombermanPunkPath), nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 6);
	cropPlayerSurface(IMG_Load(spriteBombermanMexicanPath), nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 7);
	cropFireSurface(IMG_Load(spriteFirePath));
	cropBombeSurface(IMG_Load(spriteMiscPath));
	cropBonusSurface(IMG_Load(spriteMiscPath));
	cropPreviewLevelSurface(IMG_Load(spritePreviewLevelPath));
	cropLevelSurface(IMG_Load(spriteLevelPath));
	cropLouisSurface(IMG_Load(spriteLouisPath));
}

void Sprite::cropLevelSurface(SDL_Surface * surface) {
	int index = 0;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	for (int level = 0; level < nbLevel; level++) {
		destTextureRect.x = 0;
		destTextureRect.y = 0;
		destTextureRect.w = smallSpriteLevelSizeWidth;
		destTextureRect.h = smallSpriteLevelSizeHeight;
		for (int i = 0; i < nbSmallSpriteLevelX; i++) {
			for (int j = 0; j < nbSmallSpriteLevelY; j++) {
				srcTextureRect.x = i * smallSpriteLevelSizeWidth;
				srcTextureRect.y = j * smallSpriteLevelSizeHeight;
				srcTextureRect.w = smallSpriteLevelSizeWidth;
				srcTextureRect.h = smallSpriteLevelSizeHeight;
				levelSprite[index] = SDL_CreateRGBSurface(0, smallSpriteLevelSizeWidth, smallSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
				index++;
			}
		}
		destTextureRect.x = 0;
		destTextureRect.y = 0;
		destTextureRect.w = largeSpriteLevelSizeWidth;
		destTextureRect.h = largeSpriteLevelSizeHeight;
		for (int i = 0; i < nbLargeSpriteLevelX; i++) {
			for (int j = 0; j < nbLargeSpriteLevelY; j++) {
				srcTextureRect.x = i * largeSpriteLevelSizeWidth;
				srcTextureRect.y = j * largeSpriteLevelSizeHeight;
				srcTextureRect.w = largeSpriteLevelSizeWidth;
				srcTextureRect.h = largeSpriteLevelSizeHeight;
				levelSprite[index] = SDL_CreateRGBSurface(0, largeSpriteLevelSizeWidth, largeSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
				index++;
			}
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropLouisSurface(SDL_Surface * surface) {
	int index = 0;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = spriteLouisSizeWidth;
	destTextureRect.h = spriteLouisSizeHeight;
	for (int louis = 0; louis < nbTypeLouis; louis++) {
		for (int i = 0; i < nbSpriteLouisX; i++) {
			for (int j = 0; j < nbSpriteLouisY; j++) {
				srcTextureRect.x = i * spriteLouisSizeWidth;
				srcTextureRect.y = j * spriteLouisSizeHeight;
				srcTextureRect.w = spriteLouisSizeWidth;
				srcTextureRect.h = spriteLouisSizeHeight;
				louisSprite[index] = SDL_CreateRGBSurface(0, spriteLouisSizeWidth, spriteLouisSizeHeight, 32, rmask, gmask, bmask, amask);
				index++;
			}
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropPreviewLevelSurface(SDL_Surface * surface) {
	int index = 0;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = levelPreviewSizeWidth;
	destTextureRect.h = levelPreviewSizeHeight;
	for (int i = 0; i < nbLevelPreviewX; i++) {
		for (int j = 0; j < nbLevelPreviewY; j++) {
			srcTextureRect.x = i * levelPreviewSizeWidth;
			srcTextureRect.y = j * levelPreviewSizeHeight;
			srcTextureRect.w = levelPreviewSizeWidth;
			srcTextureRect.h = levelPreviewSizeHeight;
			previewLevelSprite[index] = SDL_CreateRGBSurface(0, levelPreviewSizeWidth, levelPreviewSizeHeight, 32, rmask, gmask, bmask, amask);
			index++;
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropBombeSurface(SDL_Surface * surface) {
	int index = 0;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = defaultSpriteSize;
	destTextureRect.h = defaultSpriteSize;
	for (int i = 0; i < nbBombeSpriteX; i++) {
		for (int j = 0; j < nbBombeSpriteY; j++) {
			srcTextureRect.x = i * defaultSpriteSize;
			srcTextureRect.y = j * defaultSpriteSize;
			srcTextureRect.w = defaultSpriteSize;
			srcTextureRect.h = defaultSpriteSize;
			bombeSprite[index] = SDL_CreateRGBSurface(0, defaultSpriteSize, defaultSpriteSize, 32, rmask, gmask, bmask, amask);
			index++;
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropBonusSurface(SDL_Surface * surface) {
	int index = 0;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = defaultSpriteSize;
	destTextureRect.h = defaultSpriteSize;
	for (int i = 0; i < nbBonusSpriteX; i++) {
		for (int j = 0; j < nbBonusSpriteY; j++) {
			srcTextureRect.x = i * defaultSpriteSize;
			srcTextureRect.y = j * defaultSpriteSize;
			srcTextureRect.w = defaultSpriteSize;
			srcTextureRect.h = defaultSpriteSize;
			bonusSprite[index] = SDL_CreateRGBSurface(0, defaultSpriteSize, defaultSpriteSize, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, bonusSprite[index], &destTextureRect);
			index++;
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropFireSurface(SDL_Surface * surface) {
	int index = 0;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = defaultSpriteSize;
	destTextureRect.h = defaultSpriteSize;
	for (int i = 0; i < nbFireSpriteX; i++) {
		for (int j = 0; j < nbFireSpriteY; j++) {
			srcTextureRect.x = i * defaultSpriteSize;
			srcTextureRect.y = j * defaultSpriteSize;
			srcTextureRect.w = defaultSpriteSize;
			srcTextureRect.h = defaultSpriteSize;
			fireSprite[index] = SDL_CreateRGBSurface(0, defaultSpriteSize, defaultSpriteSize, 32, rmask, gmask, bmask, amask);
			index++;
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropPlayerSurface(SDL_Surface* surface, int offset) {
	int index = 0;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	SDL_Rect srcTextureRect;
	SDL_Rect destTextureRect;
	destTextureRect.x = 0;
	destTextureRect.y = 0;
	destTextureRect.w = spritePlayerSizeWidth;
	destTextureRect.h = spritePlayerSizeHeight;
	for (int type = 0; type < nbTypePlayer; type++) {
		for (int color = 0; color < nbColorPlayer; color++) {
			for (int i = 0; i < nbSpritePlayerX; i++) {
				for (int j = 0; j < nbSpritePlayerY; j++) {
					srcTextureRect.x = i * spritePlayerSizeWidth;
					srcTextureRect.y = j * spritePlayerSizeHeight;
					srcTextureRect.w = spritePlayerSizeWidth;
					srcTextureRect.h = spritePlayerSizeHeight;
					playerSprite[index] = SDL_CreateRGBSurface(0, spritePlayerSizeWidth, spritePlayerSizeHeight, 32, rmask, gmask, bmask, amask);
					SDL_BlitSurface(surface, &srcTextureRect, playerSprite[index], &destTextureRect);
					applyPlayerColor(playerSprite[index], color);
					index++;
				}
			}
		}
	}
	SDL_FreeSurface(surface);
}

/********************************************
 * 
 *		REQUEST FUNCTION
 * 
 ********************************************/

SDL_Surface* Sprite::getSplashScreen() {
	return splashScreenSurface;
}

SDL_Surface* Sprite::getMenuBackground() {
	return menuBackgroundSurface;
}

SDL_Surface* Sprite::players(int type, int color, int mvt, int pos) {
//TODO
	return NULL;
}
