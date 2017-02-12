#include "Sprite.h"
#include <vector>

const static char *splashScreen = "./resources/image/SplashScreen.png";
const static char *menuBackground = "./resources/image/MenuBackground.png";
const static char *background = "./resources/image/EmptyBackground.png";
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
const static char *spritePreviewLevelPath = "./resources/sprite/level/LevelView.png";
const static char *spriteLevelPath = "./resources/sprite/level/AllLevel.png";
const static char *spriteTrolleyPath = "./resources/sprite/other/trolley.png";
const static char *spriteSpaceShipPath = "./resources/sprite/other/spaceShip.png";

const static char *fontPath = "./resources/font/fragile_bombers.ttf";
const static SDL_Color greenColor = { 0, 255, 0 };
const static SDL_Color redColor = { 255, 0, 0 };
const static SDL_Color blueColor = { 0, 140, 255 };
const static SDL_Color goldColor = { 255, 255, 0 };

Sprite Sprite::m_instance = Sprite();

Sprite::Sprite() {
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	fprintf(stderr, "Init sprite system\n");
	shadowAreaSprite = new SDL_Surface *[nbShadowAreaSprite];
	playerSprite = new SDL_Surface *[nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * nbTypePlayer * (nbFrameWater + 1)];
	louisSprite = new SDL_Surface *[nbSpriteLouisX * nbSpriteLouisY * nbTypeLouis * (nbFrameWater + 1)];
	fireSprite = new SDL_Surface *[nbFireSpriteX * nbFireSpriteY];
	bombeSprite = new SDL_Surface *[nbBombeSpriteX * nbBombeSpriteY];
	bonusSprite = new SDL_Surface *[nbBonusSpriteX * nbBonusSpriteY + nbPopBonusSpriteX * nbPopBonusSpriteY + nbBurnBonusSpriteX * nbBurnBonusSpriteY];
	previewLevelSprite = new SDL_Surface *[nbLevel];
	levelSprite = new SDL_Surface *[nbLevel * nbSmallSpriteLevelX * nbSmallSpriteLevelY + nbLevel * nbLargeSpriteLevelX * nbLargeSpriteLevelY];
	spaceShipSprite = new SDL_Surface *[nbSpaceShipSpriteX * nbSpaceShipSpriteY];
	trolleySprite = new SDL_Surface *[nbTrolleySpriteX * nbTrolleySpriteY];
	mine = new SDL_Surface *[nbSpriteMineX * nbSpriteMineY];
	teleporter = new SDL_Surface *[nbTeleporterSpriteX * nbTeleporterSpriteY];
	hole = new SDL_Surface *[nbHoleX * nbHoleY];
	waterOverlay = new SDL_Surface *[nbWaterOverlayX * nbWaterOverlayY];
	rail = new SDL_Surface *[nbRailX * nbRailY];
	button = new SDL_Surface *[nbButtonX * nbButtonY];
	splashScreenSurface = IMG_Load(splashScreen);
	menuBackgroundSurface = IMG_Load(menuBackground);
	backgroundSurface = IMG_Load(background);
	cropSurface();
	fprintf(stderr, "init font text system\n");
	font = TTF_OpenFont(fontPath, 16);
}

Sprite::~Sprite() {
	fprintf(stderr, "close sprite system\n");
	for (int i = 0; i < nbShadowAreaSprite; i++) {
		SDL_FreeSurface(shadowAreaSprite[i]);
	}
	for (int i = 0; i < nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * nbTypePlayer * (nbFrameWater + 1); i++) {
		SDL_FreeSurface(playerSprite[i]);
	}
	for (int i = 0; i < nbSpriteLouisX * nbSpriteLouisY * nbTypeLouis * (nbFrameWater + 1); i++) {
		SDL_FreeSurface(louisSprite[i]);
	}
	for (int i = 0; i < nbFireSpriteX * nbFireSpriteY; i++) {
		SDL_FreeSurface(fireSprite[i]);
	}
	for (int i = 0; i < nbBombeSpriteX * nbBombeSpriteY; i++) {
		SDL_FreeSurface(bombeSprite[i]);
	}
	for (int i = 0; i < nbBonusSpriteX * nbBonusSpriteX + nbPopBonusSpriteX * nbPopBonusSpriteY + nbBurnBonusSpriteX * nbBurnBonusSpriteY; i++) {
		SDL_FreeSurface(bonusSprite[i]);
	}
	for (int i = 0; i < nbLevel; i++) {
		SDL_FreeSurface(previewLevelSprite[i]);
	}
	for (int i = 0; i < nbLevel * nbSmallSpriteLevelX * nbSmallSpriteLevelY + nbLevel * nbLargeSpriteLevelX * nbLargeSpriteLevelY; i++) {
		SDL_FreeSurface(levelSprite[i]);
	}
	for (int i = 0; i < nbTrolleySpriteX * nbTrolleySpriteY; i++) {
		SDL_FreeSurface(trolleySprite[i]);
	}
	for (int i = 0; i < nbSpaceShipSpriteX * nbSpaceShipSpriteY; i++) {
		SDL_FreeSurface(spaceShipSprite[i]);
	}

	for (int i = 0; i < nbSpriteMineX * nbSpriteMineY; i++) {
		SDL_FreeSurface(mine[i]);
	}

	for (int i = 0; i < nbTeleporterSpriteX * nbTeleporterSpriteY; i++) {
		SDL_FreeSurface(teleporter[i]);
	}

	for (int i = 0; i < nbHoleX * nbHoleY; i++) {
		SDL_FreeSurface(hole[i]);
	}

	for (int i = 0; i < nbWaterOverlayX * nbWaterOverlayY; i++) {
		SDL_FreeSurface(waterOverlay[i]);
	}

	for (int i = 0; i < nbRailX * nbRailY; i++) {
		SDL_FreeSurface(rail[i]);
	}

	for (int i = 0; i < nbButtonX * nbButtonY; i++) {
		SDL_FreeSurface(button[i]);
	}

	SDL_FreeSurface(splashScreenSurface);
	SDL_FreeSurface(menuBackgroundSurface);
	SDL_FreeSurface(backgroundSurface);
	delete shadowAreaSprite;
	delete playerSprite;
	delete louisSprite;
	delete fireSprite;
	delete bombeSprite;
	delete bonusSprite;
	delete previewLevelSprite;
	delete levelSprite;
	delete trolleySprite;
	delete spaceShipSprite;
	delete mine;
	delete teleporter;
	delete hole;
	delete waterOverlay;
	delete rail;
	delete button;
	fprintf(stderr, "close font text system\n");
	TTF_CloseFont(font);
	IMG_Quit();
	TTF_Quit();
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
	std::vector<unsigned int> colors;
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
SDL_Surface* Sprite::replaceColor(SDL_Surface* surface, unsigned int src, unsigned int dest) {
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
	createShadowArea();

	SDL_Surface * misc = IMG_Load(spriteMiscPath);
	cropBombeSurface(misc);
	cropBonusSurface(misc);
	cropMineSurface(misc);
	cropTeleporterSurface(misc);
	cropHoleSurface(misc);
	cropWaterOverlaySurface(misc);
	cropRailSurface(misc);
	cropButtonSurface(misc);
	SDL_FreeSurface(misc);

	for (int i = 0; i < (nbFrameWater + 1); i++) {
		int idx = nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * nbTypePlayer * i;
		cropPlayerSurface(IMG_Load(spriteBombermanPath), idx + (nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 0), (i - 1));
		cropPlayerSurface(IMG_Load(spriteBombermanCossakPath), idx + (nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 1), (i - 1));
		cropPlayerSurface(IMG_Load(spriteBombermanBarbarPath), idx + (nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 2), (i - 1));
		cropPlayerSurface(IMG_Load(spriteBombermanChanPath), idx + (nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 3), (i - 1));
		cropPlayerSurface(IMG_Load(spriteBombermanKidPath), idx + (nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 4), (i - 1));
		cropPlayerSurface(IMG_Load(spriteBombermanPrettyPath), idx + (nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 5), (i - 1));
		cropPlayerSurface(IMG_Load(spriteBombermanPunkPath), idx + (nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 6), (i - 1));
		cropPlayerSurface(IMG_Load(spriteBombermanMexicanPath), idx + (nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * 7), (i - 1));
	}
	for (int i = 0; i < (nbFrameWater + 1); i++) {
		int offset = nbTypeLouis * nbSpriteLouisY * nbSpriteLouisX * i;
		cropLouisSurface(IMG_Load(spriteLouisPath), offset, (i - 1));
	}

	cropFireSurface(IMG_Load(spriteFirePath));
	cropPreviewLevelSurface(IMG_Load(spritePreviewLevelPath));
	cropLevelSurface(IMG_Load(spriteLevelPath));
	cropTrolleySurface(IMG_Load(spriteTrolleyPath));
	cropSpaceShipSurface(IMG_Load(spriteSpaceShipPath));
}

void Sprite::createShadowArea() {
	Uint32 rmask, gmask, bmask, amask;
	amask = 0xff000000;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	//menu shadow surface
	shadowAreaSprite[0] = SDL_CreateRGBSurface(0, 574, 27, 32, rmask, gmask, bmask, amask);
	shadowAreaSprite[1] = SDL_CreateRGBSurface(0, 574, 100, 32, rmask, gmask, bmask, amask);
	shadowAreaSprite[2] = SDL_CreateRGBSurface(0, 574, 150, 32, rmask, gmask, bmask, amask);
	//header of game
	shadowAreaSprite[3] = SDL_CreateRGBSurface(0, 32, 20, 32, rmask, gmask, bmask, amask);
	for (int i = 0; i < nbShadowAreaSprite; i++) {
		SDL_FillRect(shadowAreaSprite[i], NULL, SDL_MapRGBA(shadowAreaSprite[i]->format, 0, 0, 0, 120));
	}
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

		for (int j = 0; j < nbSmallSpriteLevelY; j++) {
			for (int i = 0; i < nbSmallSpriteLevelX; i++) {
				srcTextureRect.x = i * smallSpriteLevelSizeWidth;
				srcTextureRect.y = j * smallSpriteLevelSizeHeight + (smallSpriteLevelSizeHeight * nbSmallSpriteLevelY * level);
				srcTextureRect.w = smallSpriteLevelSizeWidth;
				srcTextureRect.h = smallSpriteLevelSizeHeight;
				levelSprite[index] = SDL_CreateRGBSurface(0, smallSpriteLevelSizeWidth, smallSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
				SDL_BlitSurface(surface, &srcTextureRect, levelSprite[index], &destTextureRect);
				index++;
			}
		}
		destTextureRect.x = 0;
		destTextureRect.y = 0;
		destTextureRect.w = largeSpriteLevelSizeWidth;
		destTextureRect.h = largeSpriteLevelSizeHeight;

		for (int j = 0; j < nbLargeSpriteLevelY; j++) {
			for (int i = 0; i < nbLargeSpriteLevelX; i++) {
				srcTextureRect.x = i * largeSpriteLevelSizeWidth + (smallSpriteLevelSizeWidth * nbSmallSpriteLevelX);
				srcTextureRect.y = j * largeSpriteLevelSizeHeight + (smallSpriteLevelSizeHeight * nbSmallSpriteLevelY * level);
				srcTextureRect.w = largeSpriteLevelSizeWidth;
				srcTextureRect.h = largeSpriteLevelSizeHeight;
				levelSprite[index] = SDL_CreateRGBSurface(0, largeSpriteLevelSizeWidth, largeSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
				SDL_BlitSurface(surface, &srcTextureRect, levelSprite[index], &destTextureRect);
				index++;
			}
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropLouisSurface(SDL_Surface * surface, int offset, int offsetUnderWater) {
	int index = offset;
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
		for (int j = 0; j < nbSpriteLouisY; j++) {
			for (int i = 0; i < nbSpriteLouisX; i++) {
				srcTextureRect.x = i * spriteLouisSizeWidth;
				srcTextureRect.y = j * spriteLouisSizeHeight;
				srcTextureRect.w = spriteLouisSizeWidth;
				srcTextureRect.h = spriteLouisSizeHeight;
				louisSprite[index] = SDL_CreateRGBSurface(0, spriteLouisSizeWidth, spriteLouisSizeHeight, 32, rmask, gmask, bmask, amask);
				SDL_BlitSurface(surface, &srcTextureRect, louisSprite[index], &destTextureRect);
				applyLouisColor(louisSprite[index], louis);
				if (offsetUnderWater >= 0) {
					applyUnderwaterOverlay(louisSprite[index], offsetUnderWater);
				}
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

	for (int j = 0; j < nbLevelPreviewY; j++) {
		for (int i = 0; i < nbLevelPreviewX; i++) {
			srcTextureRect.x = i * levelPreviewSizeWidth;
			srcTextureRect.y = j * levelPreviewSizeHeight;
			srcTextureRect.w = levelPreviewSizeWidth;
			srcTextureRect.h = levelPreviewSizeHeight;
			previewLevelSprite[index] = SDL_CreateRGBSurface(0, levelPreviewSizeWidth, levelPreviewSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, previewLevelSprite[index], &destTextureRect);
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
	for (int j = 0; j < nbBombeSpriteY; j++) {
		for (int i = 0; i < nbBombeSpriteX; i++) {
			srcTextureRect.x = (i + 2) * defaultSpriteSize;
			srcTextureRect.y = j * defaultSpriteSize;
			srcTextureRect.w = defaultSpriteSize;
			srcTextureRect.h = defaultSpriteSize;
			bombeSprite[index] = SDL_CreateRGBSurface(0, defaultSpriteSize, defaultSpriteSize, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, bombeSprite[index], &destTextureRect);
			index++;
		}
	}
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
	for (int j = 0; j < nbBonusSpriteY; j++) {
		for (int i = 0; i < nbBonusSpriteX; i++) {
			srcTextureRect.x = i * defaultSpriteSize;
			srcTextureRect.y = j * defaultSpriteSize;
			srcTextureRect.w = defaultSpriteSize;
			srcTextureRect.h = defaultSpriteSize;
			bonusSprite[index] = SDL_CreateRGBSurface(0, defaultSpriteSize, defaultSpriteSize, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, bonusSprite[index], &destTextureRect);
			index++;
		}
	}
	for (int j = 0; j < nbBurnBonusSpriteY; j++) {
		for (int i = 0; i < nbBurnBonusSpriteX; i++) {
			srcTextureRect.x = i * burnBonusSpritewidth + (defaultSpriteSize * nbBonusSpriteX) + ( nbBombeSpriteX * defaultSpriteSize);
			srcTextureRect.y = j * burnBonusSpriteHeight;
			srcTextureRect.w = burnBonusSpritewidth;
			srcTextureRect.h = burnBonusSpriteHeight;
			bonusSprite[index] = SDL_CreateRGBSurface(0, burnBonusSpritewidth, burnBonusSpriteHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, bonusSprite[index], &destTextureRect);
			index++;
		}
	}
	for (int j = 0; j < nbPopBonusSpriteY; j++) {
		for (int i = 0; i < nbPopBonusSpriteX; i++) {
			srcTextureRect.x = i * popBonusSpriteWidth + (defaultSpriteSize * nbBonusSpriteX) + ( nbBombeSpriteX * defaultSpriteSize);
			srcTextureRect.y = j * popBonusSpriteHeight + (burnBonusSpriteHeight * nbBurnBonusSpriteY);
			srcTextureRect.w = popBonusSpriteWidth;
			srcTextureRect.h = popBonusSpriteHeight;
			bonusSprite[index] = SDL_CreateRGBSurface(0, popBonusSpriteWidth, popBonusSpriteHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, bonusSprite[index], &destTextureRect);
			index++;
		}
	}
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
	destTextureRect.w = smallSpriteFireSizeWidth;
	destTextureRect.h = smallSpriteFireSizeHeight;

	for (int j = 0; j < nbFireSpriteY; j++) {
		for (int i = 0; i < nbFireSpriteX; i++) {
			srcTextureRect.x = i * smallSpriteFireSizeWidth;
			srcTextureRect.y = j * smallSpriteFireSizeHeight;
			srcTextureRect.w = smallSpriteFireSizeWidth;
			srcTextureRect.h = smallSpriteFireSizeHeight;
			fireSprite[index] = SDL_CreateRGBSurface(0, smallSpriteFireSizeWidth, smallSpriteFireSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, fireSprite[index], &destTextureRect);
			index++;
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropTrolleySurface(SDL_Surface * surface) {
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
	destTextureRect.w = trolleySpriteSizeWidth;
	destTextureRect.h = trolleySpriteSizeHeight;

	for (int j = 0; j < nbTrolleySpriteY; j++) {
		for (int i = 0; i < nbTrolleySpriteX; i++) {
			srcTextureRect.x = i * trolleySpriteSizeWidth;
			srcTextureRect.y = j * trolleySpriteSizeHeight;
			srcTextureRect.w = trolleySpriteSizeWidth;
			srcTextureRect.h = trolleySpriteSizeHeight;
			trolleySprite[index] = SDL_CreateRGBSurface(0, trolleySpriteSizeWidth, trolleySpriteSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, trolleySprite[index], &destTextureRect);
			index++;
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropSpaceShipSurface(SDL_Surface * surface) {
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
	destTextureRect.w = spaceShipSpriteSizeWidth;
	destTextureRect.h = spaceShipSpriteSizeHeight;

	for (int j = 0; j < nbSpaceShipSpriteY; j++) {
		for (int i = 0; i < nbSpaceShipSpriteX; i++) {
			srcTextureRect.x = i * spaceShipSpriteSizeWidth;
			srcTextureRect.y = j * spaceShipSpriteSizeHeight;
			srcTextureRect.w = spaceShipSpriteSizeWidth;
			srcTextureRect.h = spaceShipSpriteSizeHeight;
			spaceShipSprite[index] = SDL_CreateRGBSurface(0, spaceShipSpriteSizeWidth, spaceShipSpriteSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, spaceShipSprite[index], &destTextureRect);
			index++;
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropPlayerSurface(SDL_Surface* surface, int offset, int offsetUnderWater) {
	int index = offset;
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
	for (int color = 0; color < nbColorPlayer; color++) {
		for (int j = 0; j < nbSpritePlayerY; j++) {
			for (int i = 0; i < nbSpritePlayerX; i++) {
				srcTextureRect.x = i * spritePlayerSizeWidth;
				srcTextureRect.y = j * spritePlayerSizeHeight;
				srcTextureRect.w = spritePlayerSizeWidth;
				srcTextureRect.h = spritePlayerSizeHeight;
				playerSprite[index] = SDL_CreateRGBSurface(0, spritePlayerSizeWidth, spritePlayerSizeHeight, 32, rmask, gmask, bmask, amask);
				SDL_BlitSurface(surface, &srcTextureRect, playerSprite[index], &destTextureRect);
				applyPlayerColor(playerSprite[index], color);
				if (offsetUnderWater >= 0) {
					applyUnderwaterOverlay(playerSprite[index], offsetUnderWater);
				}
				index++;
			}
		}
	}
	SDL_FreeSurface(surface);
}

void Sprite::cropMineSurface(SDL_Surface * surface) {
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
	destTextureRect.w = smallSpriteLevelSizeWidth;
	destTextureRect.h = smallSpriteLevelSizeHeight;
	for (int j = 0; j < nbSpriteMineY; j++) {
		for (int i = 0; i < nbSpriteMineX; i++) {
			srcTextureRect.x = i * smallSpriteLevelSizeWidth + (defaultSpriteSize * nbBonusSpriteX) + ( nbBombeSpriteX * defaultSpriteSize);
			srcTextureRect.y = j * smallSpriteLevelSizeHeight + (defaultSpriteSize * 4);
			srcTextureRect.w = smallSpriteLevelSizeWidth;
			srcTextureRect.h = smallSpriteLevelSizeHeight;
			mine[index] = SDL_CreateRGBSurface(0, smallSpriteLevelSizeWidth, smallSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, mine[index], &destTextureRect);
			index++;
		}
	}
}
void Sprite::cropTeleporterSurface(SDL_Surface * surface) {
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
	destTextureRect.w = smallSpriteLevelSizeWidth;
	destTextureRect.h = smallSpriteLevelSizeHeight;
	for (int j = 0; j < nbTeleporterSpriteY; j++) {
		for (int i = 0; i < nbTeleporterSpriteX; i++) {
			srcTextureRect.x = i * smallSpriteLevelSizeWidth + (defaultSpriteSize * nbBonusSpriteX) + ( nbBombeSpriteX * defaultSpriteSize);
			srcTextureRect.y = j * smallSpriteLevelSizeHeight + (defaultSpriteSize * 7);
			srcTextureRect.w = smallSpriteLevelSizeWidth;
			srcTextureRect.h = smallSpriteLevelSizeHeight;
			teleporter[index] = SDL_CreateRGBSurface(0, smallSpriteLevelSizeWidth, smallSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, teleporter[index], &destTextureRect);
			index++;
		}
	}
}
void Sprite::cropHoleSurface(SDL_Surface * surface) {
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
	destTextureRect.w = smallSpriteLevelSizeWidth;
	destTextureRect.h = smallSpriteLevelSizeHeight;
	for (int j = 0; j < nbHoleY; j++) {
		for (int i = 0; i < nbHoleX; i++) {
			srcTextureRect.x = i * smallSpriteLevelSizeWidth + (defaultSpriteSize * nbBonusSpriteX) + ( nbBombeSpriteX * defaultSpriteSize);
			srcTextureRect.y = j * smallSpriteLevelSizeHeight + (defaultSpriteSize * 6);
			srcTextureRect.w = smallSpriteLevelSizeWidth;
			srcTextureRect.h = smallSpriteLevelSizeHeight;
			hole[index] = SDL_CreateRGBSurface(0, smallSpriteLevelSizeWidth, smallSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, hole[index], &destTextureRect);
			index++;
		}
	}
}
void Sprite::cropWaterOverlaySurface(SDL_Surface * surface) {
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
	destTextureRect.w = smallSpriteLevelSizeWidth;
	destTextureRect.h = smallSpriteLevelSizeHeight;
	for (int j = 0; j < nbWaterOverlayY; j++) {
		for (int i = 0; i < nbWaterOverlayX; i++) {
			srcTextureRect.x = i * smallSpriteLevelSizeWidth + (defaultSpriteSize * nbBonusSpriteX) + ( nbBombeSpriteX * defaultSpriteSize) + (2 * smallSpriteLevelSizeWidth);
			srcTextureRect.y = j * smallSpriteLevelSizeHeight + (defaultSpriteSize * 6);
			srcTextureRect.w = smallSpriteLevelSizeWidth;
			srcTextureRect.h = smallSpriteLevelSizeHeight;
			waterOverlay[index] = SDL_CreateRGBSurface(0, smallSpriteLevelSizeWidth, smallSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, waterOverlay[index], &destTextureRect);
			index++;
		}
	}
}
void Sprite::cropRailSurface(SDL_Surface * surface) {
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
	destTextureRect.w = smallSpriteLevelSizeWidth;
	destTextureRect.h = smallSpriteLevelSizeHeight;
	for (int j = 0; j < nbRailY; j++) {
		for (int i = 0; i < nbRailX; i++) {
			srcTextureRect.x = i * smallSpriteLevelSizeWidth + (defaultSpriteSize * nbBonusSpriteX) + ( nbBombeSpriteX * defaultSpriteSize);
			srcTextureRect.y = j * smallSpriteLevelSizeHeight + (defaultSpriteSize * 5);
			srcTextureRect.w = smallSpriteLevelSizeWidth;
			srcTextureRect.h = smallSpriteLevelSizeHeight;
			rail[index] = SDL_CreateRGBSurface(0, smallSpriteLevelSizeWidth, smallSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, rail[index], &destTextureRect);
			index++;
		}
	}
}
void Sprite::cropButtonSurface(SDL_Surface * surface) {
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
	destTextureRect.w = smallSpriteLevelSizeWidth;
	destTextureRect.h = smallSpriteLevelSizeHeight;
	for (int j = 0; j < nbButtonY; j++) {
		for (int i = 0; i < nbButtonX; i++) {
			srcTextureRect.x = i * smallSpriteLevelSizeWidth + (defaultSpriteSize * nbBonusSpriteX) + ( nbBombeSpriteX * defaultSpriteSize) + (8 * smallSpriteLevelSizeWidth);
			srcTextureRect.y = j * smallSpriteLevelSizeHeight + (defaultSpriteSize * 5);
			srcTextureRect.w = smallSpriteLevelSizeWidth;
			srcTextureRect.h = smallSpriteLevelSizeHeight;
			button[index] = SDL_CreateRGBSurface(0, smallSpriteLevelSizeWidth, smallSpriteLevelSizeHeight, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(surface, &srcTextureRect, button[index], &destTextureRect);
			index++;
		}
	}
}

/********************************************
 * 
 *		DRAW TEXT FUNCTION
 * 
 ********************************************/
void Sprite::drawText(SDL_Surface* surfaceToDraw, int x, int y, const char* text, int color, bool alignCenter) {
	SDL_Color colorSelected = getSDL_Color(color);
	SDL_Surface *text_surface = text_surface = TTF_RenderText_Solid(font, text, colorSelected);
	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = text_surface->w;
	srcRect.h = text_surface->h;
	SDL_Rect dstRect;
	if (alignCenter) {
		dstRect.x = x - (text_surface->w / 2);
	} else {
		dstRect.x = x;
	}
	dstRect.y = y;
	dstRect.w = text_surface->w;
	dstRect.h = text_surface->h;
	SDL_BlitSurface(text_surface, &srcRect, surfaceToDraw, &dstRect);
	SDL_FreeSurface(text_surface);
}

SDL_Color Sprite::getSDL_Color(int color) {
	switch (color) {
		case red:
			return redColor;
			break;
		case blue:
			return blueColor;
			break;
		case green:
			return greenColor;
			break;
		case gold:
			return goldColor;
			break;
	}
	return greenColor;
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

SDL_Surface* Sprite::getBackground() {
	return backgroundSurface;
}

SDL_Surface* Sprite::getCursor(int pos) {
	if (pos > 2) {
		return bombeSprite[pos - 2];
	}
	return bombeSprite[pos];
}

SDL_Surface* Sprite::getFire(int x, int y) {
	return fireSprite[y * nbFireSpriteX + x];
}

SDL_Surface* Sprite::getBonus(int bonusNumber) {
	return bonusSprite[bonusNumber];
}

SDL_Surface* Sprite::getBurnBonus(int pos) {
	return bonusSprite[(nbBonusSpriteX * nbBonusSpriteY) + pos];
}

SDL_Surface* Sprite::getPopBonus(int pos) {
	return bonusSprite[(nbBonusSpriteX * nbBonusSpriteY) + (nbBurnBonusSpriteX * nbBurnBonusSpriteY) + pos];
}

SDL_Surface* Sprite::getBombe(int x, int y) {
	return bombeSprite[y * nbBombeSpriteX + x];
}

SDL_Surface* Sprite::getLevelPreview(int pos) {
	return previewLevelSprite[pos];
}

SDL_Surface* Sprite::getBurnWall(int pos, int levelIndex) {
	int spriteNumber = ((nbSmallSpriteLevelX * nbSmallSpriteLevelY) + (nbLargeSpriteLevelX * nbLargeSpriteLevelY)) * levelIndex + burnWallStartSprite + pos;
	return levelSprite[spriteNumber];
}

SDL_Surface* Sprite::getLevel(int pos, int levelIndex) {
	int spriteNumber = ((nbSmallSpriteLevelX * nbSmallSpriteLevelY) + (nbLargeSpriteLevelX * nbLargeSpriteLevelY)) * levelIndex + pos;
	return levelSprite[spriteNumber];
}
/****************
 *	PLAYER
 ****************/

int Sprite::calcStartIndexPlayer(int type, int color, int offsetUnderWater) {
	return ((nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * nbTypePlayer) * offsetUnderWater) + (nbSpritePlayerX * nbSpritePlayerY * nbColorPlayer * type)
			+ ((nbSpritePlayerX * nbSpritePlayerY) * color);
}

SDL_Surface* Sprite::playerDrawNormal(int type, int color, int move, int pos, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * move) + pos];
}

SDL_Surface* Sprite::drawOnLouis(int type, int color, int move, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * move) + 8];
}

SDL_Surface* Sprite::drawWithBombe(int type, int color, int move, int pos, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * move) + 3 + pos];
}

SDL_Surface* Sprite::drawThrowBombe(int type, int color, int move, int pos, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * move) + 6 + pos];
}

SDL_Surface* Sprite::drawBurning(int type, int color, int pos, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * 6) + pos];
}

SDL_Surface* Sprite::drawVictory(int type, int color, int pos, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * 4) + pos];
}

SDL_Surface* Sprite::drawPlayerVictoryOnLouis(int type, int color, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * 4) + 4];
}

SDL_Surface* Sprite::drawCrying(int type, int color, int pos, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * 5) + pos];
}

SDL_Surface* Sprite::getHappySprite(int type, int color, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * 5) + 3];
}

SDL_Surface* Sprite::getCryingSprite(int type, int color, int offsetUnderWater) {
	return playerSprite[calcStartIndexPlayer(type, color, offsetUnderWater) + (nbSpritePlayerX * 5) + 4];
}
/****************
 *	LOUIS
 ****************/
SDL_Surface* Sprite::drawLouis(int louisType, int move, int pos, int offsetUnderWater) {
	return louisSprite[(nbSpriteLouisX * nbSpriteLouisY * nbTypeLouis) * offsetUnderWater + (nbSpriteLouisX * nbSpriteLouisY * louisType) + (move * nbSpriteLouisX) + pos];
}

SDL_Surface* Sprite::drawLouisBurning(int type, int pos, int offsetUnderWater) {
	return louisSprite[(nbSpriteLouisX * nbSpriteLouisY * nbTypeLouis) * offsetUnderWater + (nbSpriteLouisX * nbSpriteLouisY * type) + (4 * nbSpriteLouisX) + pos];
}

SDL_Surface* Sprite::drawVictoryOnLouis(int type, int pos, int offsetUnderWater) {
	return louisSprite[(nbSpriteLouisX * nbSpriteLouisY * nbTypeLouis) * offsetUnderWater + (nbSpriteLouisX * nbSpriteLouisY * type) + (4 * nbSpriteLouisX) + 4 + pos];
}
/****************
 * menu /header shadow area
 ****************/
SDL_Surface* Sprite::getShadowArea(int number) {
	return shadowAreaSprite[number];
}
/****************
 * misc area
 ****************/
SDL_Surface* Sprite::getButton(int idx) {
	return button[idx];
}
SDL_Surface* Sprite::getMine(int idx) {
	return mine[idx];
}
SDL_Surface* Sprite::getWaterOverlay(int idx) {
	return waterOverlay[idx];
}
SDL_Surface* Sprite::getTeleporter(int idx) {
	return teleporter[idx];
}
SDL_Surface* Sprite::getHole(int idx) {
	return hole[idx];
}
SDL_Surface* Sprite::getRail(int idx) {
	return rail[idx];
}
SDL_Surface* Sprite::getTrolley(int idx) {
	return trolleySprite[idx];
}

void Sprite::applyUnderwaterOverlay(SDL_Surface * surface, int idx) {
	SDL_Rect destTextureRectTmp;
	SDL_Rect srcTextureRectTmp;
	destTextureRectTmp.x = 6;
	destTextureRectTmp.y = 26;
	destTextureRectTmp.w = smallSpriteLevelSizeWidth;
	destTextureRectTmp.h = smallSpriteLevelSizeHeight;
	srcTextureRectTmp.x = 0;
	srcTextureRectTmp.y = 0;
	srcTextureRectTmp.w = smallSpriteLevelSizeWidth;
	srcTextureRectTmp.h = smallSpriteLevelSizeHeight;
	SDL_BlitSurface(waterOverlay[idx], &srcTextureRectTmp, surface, &destTextureRectTmp);
	if (SDL_MUSTLOCK(surface)) {
		SDL_LockSurface(surface);
	}
	Uint32 *pixels = (Uint32 *) surface->pixels;
	for (int x = 0; x < surface->w; x++) {
		for (int y = 0; y < surface->h; y++) {
			if (pixels[y * surface->w + x] == 0x95003346) {
				pixels[y * surface->w + x] = 0x00000000;
			}else if (pixels[y * surface->w + x] == 0x97003447) {
				pixels[y * surface->w + x] = 0x00000000;
			}
		}
	}
	if (SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
}
