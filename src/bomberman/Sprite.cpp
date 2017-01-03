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
	playerSprite = new SDL_Surface *[nbSpritePlayer * nbColorPlayer * nbTypePlayer];
	louisSprite = new SDL_Surface *[nbSpriteLouis * nbTypeLouis];
	fireSprite = new SDL_Surface *[nbFireSprite];
	miscSprite = new SDL_Surface *[nbMiscSprite];
	cursorSprite = new SDL_Surface *[nbCursorSprite];
	previewLevelSprite = new SDL_Surface *[nbLevel];
	levelSprite = new SDL_Surface *[(nbLevel * nbSmallSpriteLevel) + (nbLevel * nbLargeSpriteLevel)];
	splashScreenSurface = IMG_Load(splashScreen);
	menuBackgroundSurface = IMG_Load(menuBackground);
	cropSurface();
}

Sprite::~Sprite() {
	fprintf(stderr, "close sprite system\n");
	for (int i = 0; i < nbSpritePlayer * nbColorPlayer * nbTypePlayer; i++) {
		SDL_FreeSurface(playerSprite[i]);
	}
	for (int i = 0; i < nbSpriteLouis * nbTypeLouis; i++) {
		SDL_FreeSurface(louisSprite[i]);
	}
	for (int i = 0; i < nbFireSprite; i++) {
		SDL_FreeSurface(fireSprite[i]);
	}
	for (int i = 0; i < nbMiscSprite; i++) {
		SDL_FreeSurface(miscSprite[i]);
	}
	for (int i = 0; i < nbCursorSprite; i++) {
		SDL_FreeSurface(cursorSprite[i]);
	}
	for (int i = 0; i < nbLevel; i++) {
		SDL_FreeSurface(previewLevelSprite[i]);
	}
	for (int i = 0; i < nbLevel * nbSmallSpriteLevel + nbLevel * nbLargeSpriteLevel; i++) {
		SDL_FreeSurface(levelSprite[i]);
	}
	SDL_FreeSurface(splashScreenSurface);
	SDL_FreeSurface(menuBackgroundSurface);
	free(playerSprite);
	free(louisSprite);
	free(fireSprite);
	free(cursorSprite);
	free(miscSprite);
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
SDL_Surface* Sprite::applyColor(SDL_Surface * surface, int color) {
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
}

/********************************************
 * 
 *		CROP SURFACE
 * 
 ********************************************/
void Sprite::cropSurface() {
	cropPlayerSurface(IMG_Load(spriteBombermanPath), nbSpritePlayer * nbColorPlayer * 0);
	cropPlayerSurface(IMG_Load(spriteBombermanCossakPath), nbSpritePlayer * nbColorPlayer * 1);
	cropPlayerSurface(IMG_Load(spriteBombermanBarbarPath), nbSpritePlayer * nbColorPlayer * 2);
	cropPlayerSurface(IMG_Load(spriteBombermanChanPath), nbSpritePlayer * nbColorPlayer * 3);
	cropPlayerSurface(IMG_Load(spriteBombermanKidPath), nbSpritePlayer * nbColorPlayer * 4);
	cropPlayerSurface(IMG_Load(spriteBombermanPrettyPath), nbSpritePlayer * nbColorPlayer * 5);
	cropPlayerSurface(IMG_Load(spriteBombermanPunkPath), nbSpritePlayer * nbColorPlayer * 6);
	cropPlayerSurface(IMG_Load(spriteBombermanMexicanPath), nbSpritePlayer * nbColorPlayer * 7);
	cropFireSurface(IMG_Load(spriteFirePath));
	cropMiscSurface(IMG_Load(spriteMiscPath));
	cropCursorSurface(IMG_Load(spriteCursorPath));
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
	for (int level = 0; level < nbLevel; level++) {
		for (int i = 0; i < nbSmallSpriteLevel; i++) {
			levelSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
			index++;
		}
		for (int i = 0; i < nbLargeSpriteLevel; i++) {
			levelSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
			index++;
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
	for (int louis = 0; louis < nbTypeLouis; louis++) {
		for (int i = 0; i < nbSpriteLouis; i++) {
			louisSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
			index++;
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
	for (int i = 0; i < nbLevel; i++) {
		previewLevelSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
		index++;
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropMiscSurface(SDL_Surface * surface) {
	int index = 0;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	for (int i = 0; i < nbMiscSprite; i++) {
		miscSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
		index++;
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
	for (int i = 0; i < nbFireSprite; i++) {
		fireSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
		index++;
	}
	SDL_FreeSurface(surface);
}
void Sprite::cropCursorSurface(SDL_Surface * surface) {
	int index = 0;
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	for (int i = 0; i < nbCursorSprite; i++) {
		cursorSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
		index++;
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
	destTextureRect.w = spriteSizeWidth;
	destTextureRect.h = spriteSizeHeight;
	for (int type = 0; type < nbTypePlayer; type++) {
		for (int color = 0; color < nbColorPlayer; color++) {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 4; j++) {
					srcTextureRect.x = i * spriteSizeWidth;
					srcTextureRect.y = j * spriteSizeHeight;
					srcTextureRect.w = spriteSizeWidth;
					srcTextureRect.h = spriteSizeHeight;
					playerSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
					SDL_BlitSurface(surface, &srcTextureRect, playerSprite[index], &destTextureRect);
					applyColor(playerSprite[index], color);
					index++;
				}
			}
			for (int i = 0; i < 5; i++) {
				for (int j = 4; j < 6; j++) {
					srcTextureRect.x = i * spriteSizeWidth;
					srcTextureRect.y = j * spriteSizeHeight;
					srcTextureRect.w = spriteSizeWidth;
					srcTextureRect.h = spriteSizeHeight;
					playerSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
					SDL_BlitSurface(surface, &srcTextureRect, playerSprite[index], &destTextureRect);
					applyColor(playerSprite[index], color);
					index++;
				}
			}
			for (int i = 0; i < 7; i++) {
				srcTextureRect.x = i * spriteSizeWidth;
				srcTextureRect.y = 6 * spriteSizeHeight;
				srcTextureRect.w = spriteSizeWidth;
				srcTextureRect.h = spriteSizeHeight;
				playerSprite[index] = SDL_CreateRGBSurface(0, spriteSizeWidth, spriteSizeHeight, 32, rmask, gmask, bmask, amask);
				SDL_BlitSurface(surface, &srcTextureRect, playerSprite[index], &destTextureRect);
				applyColor(playerSprite[index], color);
				index++;
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
