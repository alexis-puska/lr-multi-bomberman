#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#else
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#endif

#include <stdio.h>

#ifndef __MYCLASS_SPRITE
#define __MYCLASS_SPRITE

#define nbShadowAreaSprite		4

#define nbFrameWater			2
#define nbSpritePlayerX			9
#define nbSpritePlayerY			7
#define spritePlayerSizeWidth		30
#define spritePlayerSizeHeight	42
#define nbColorPlayer			7
//#define nbPlayer				16
#define nbTypePlayer			8

#define nbSpriteLouisX			5
#define nbSpriteLouisY			7
#define spriteLouisSizeWidth		30
#define spriteLouisSizeHeight		42
#define nbTypeLouis 			5

#define nbLevel				9
#define defaultSpriteSize		16

#define nbFireSpriteX			4
#define nbFireSpriteY			9
#define smallSpriteFireSizeWidth	18
#define smallSpriteFireSizeHeight	16

#define nbBombeSpriteX			3
#define nbBombeSpriteY			4
#define nbBonusSpriteX			2
#define nbBonusSpriteY			8

#define nbPopBonusSpriteX			4
#define nbPopBonusSpriteY			1
#define popBonusSpriteWidth			18
#define popBonusSpriteHeight		16

#define nbBurnBonusSpriteX			8
#define nbBurnBonusSpriteY			1
#define burnBonusSpritewidth		30
#define burnBonusSpriteHeight		42

#define levelPreviewSizeWidth		128
#define levelPreviewSizeHeight	110
#define nbLevelPreviewX			3
#define nbLevelPreviewY			3


#define nbSmallSpriteLevelX		5
#define nbSmallSpriteLevelY		8
#define nbLargeSpriteLevelX		1
#define nbLargeSpriteLevelY		2
#define smallSpriteLevelSizeWidth	18
#define smallSpriteLevelSizeHeight	16
#define largeSpriteLevelSizeWidth	54
#define largeSpriteLevelSizeHeight	48
#define burnWallStartSprite 22
#define suddenDeathWallSpriteIndex	16
#define wallSpriteIndex				16
#define skyStartSpriteIndex			40


#define nbTrolleySpriteX		4
#define nbTrolleySpriteY		1
#define trolleySpriteSizeWidth	30
#define trolleySpriteSizeHeight	42

#define nbSpaceShipSpriteX		2
#define nbSpaceShipSpriteY		4
#define spaceShipSpriteSizeWidth	30
#define spaceShipSpriteSizeHeight	42

#define nbSpriteMineX			13
#define nbSpriteMineY			1
#define nbTeleporterSpriteX		4
#define nbTeleporterSpriteY		1
#define nbHoleX				2
#define nbHoleY				1
#define nbWaterOverlayX			2
#define nbWaterOverlayY			1
#define nbRailX				8
#define nbRailY				1
#define nbButtonX				2
#define nbButtonY				1

enum playerMove {
	none = -1, down = 0, up = 1, left = 2, right = 3
};

enum textColor {
	red = 0, blue = 1, green = 2, gold = 3
};

class Sprite {

	public:
		static Sprite& Instance();
		SDL_Surface* getSplashScreen();
		SDL_Surface* getMenuBackground();
		SDL_Surface* getBackground();
		Sprite();
		~Sprite();
		SDL_Surface* getShadowArea(int number);
		SDL_Surface* getCursor(int pos);
		SDL_Surface* getLevelPreview(int pos);
		SDL_Surface* getBonus(int bonusNumber);
		SDL_Surface* getFire(int x, int y);
		SDL_Surface* getBombe(int x, int y);
		SDL_Surface* getBurnWall(int pos, int levelIndex);
		SDL_Surface* getLevel(int pos, int levelIndex);
		SDL_Surface* getBurnBonus(int pos);
		SDL_Surface* getPopBonus(int pos);
		SDL_Surface* getButton(int idx);
		SDL_Surface* getMine(int idx);
		SDL_Surface* getWaterOverlay(int idx);
		SDL_Surface* getTeleporter(int idx);
		SDL_Surface* getHole(int idx);
		SDL_Surface* getRail(int idx);
		SDL_Surface* getTrolley(int idx);
		/*
		 * DRAW PLAYER FUNCTION
		 */
		SDL_Surface* playerDrawNormal(int type, int color, int move, int pos, int offsetUnderWater);
		SDL_Surface* drawOnLouis(int type, int color, int move, int offsetUnderWater);
		SDL_Surface* drawWithBombe(int type, int color, int move, int pos, int offsetUnderWater);
		SDL_Surface* drawThrowBombe(int type, int color, int move, int pos, int offsetUnderWater);
		SDL_Surface* drawBurning(int type, int color, int pos, int offsetUnderWater);
		SDL_Surface* drawLouisBurning(int type, int pos, int offsetUnderWater);
		SDL_Surface* drawVictory(int type, int color, int pos, int offsetUnderWater);
		SDL_Surface* drawPlayerVictoryOnLouis(int type, int color, int offsetUnderWater);
		SDL_Surface* drawCrying(int type, int color, int pos, int offsetUnderWater);
		SDL_Surface* drawLouis(int louisType, int move, int pos, int offsetUnderWater);
		SDL_Surface* drawVictoryOnLouis(int type, int pos, int offsetUnderWater);
		SDL_Surface* getHappySprite(int type, int color, int offsetUnderWater);
		SDL_Surface* getCryingSprite(int type, int color, int offsetUnderWater);
		/*
		 * DRAW TEXT FUNCTION
		 */
		void drawText(SDL_Surface* surfaceToDraw, int x, int y, const char* text, int color, bool alignCenter);
		SDL_Color getSDL_Color(int color);
		
	private:
		Sprite& operator=(const Sprite&);
		Sprite(const Sprite&);
		static Sprite m_instance;
		void cropSurface();
		void createShadowArea();
		void cropPlayerSurface(SDL_Surface*, int offset, int offsetUnderWater);
		void cropPreviewLevelSurface(SDL_Surface * surface);
		void cropBombeSurface(SDL_Surface * surface);
		void cropFireSurface(SDL_Surface * surface);
		void cropBonusSurface(SDL_Surface * surface);
		void cropLevelSurface(SDL_Surface * surface);
		void cropLouisSurface(SDL_Surface * surface, int offset, int offsetUnderWater);
		void cropTrolleySurface(SDL_Surface * surface);
		void cropSpaceShipSurface(SDL_Surface * surface);
		void cropMineSurface(SDL_Surface * surface);
		void cropTeleporterSurface(SDL_Surface * surface);
		void cropHoleSurface(SDL_Surface * surface);
		void cropWaterOverlaySurface(SDL_Surface * surface);
		void cropRailSurface(SDL_Surface * surface);
		void cropButtonSurface(SDL_Surface * surface);
		int calcStartIndexPlayer(int type, int color, int offsetUnderWater);
		void applyUnderwaterOverlay(SDL_Surface * surface, int idx);


		/*
		 * INFORMATION FUNCTION
		 */
		void getColorInSurface(SDL_Surface* surface);
		
		/*
		 * COLOR FUNCTION
		 */
		SDL_Surface* applyPlayerColor(SDL_Surface * surface, int color);
		SDL_Surface* replaceColor(SDL_Surface* surface, unsigned int src, unsigned int dest);
		SDL_Surface* upPlayerGreyColor(SDL_Surface* surface);
		SDL_Surface* upPlayerRedColor(SDL_Surface* surface);
		SDL_Surface* upPlayerBlueColor(SDL_Surface* surface);
		SDL_Surface* upPlayerGreenColor(SDL_Surface* surface);
		SDL_Surface* upPlayerGoldColor(SDL_Surface* surface);
		SDL_Surface* upPlayerBrownColor(SDL_Surface* surface);
		SDL_Surface* applyLouisColor(SDL_Surface * surface, int color);
		SDL_Surface* upLouisGoldColor(SDL_Surface* surface);
		SDL_Surface* upLouisGreenColor(SDL_Surface* surface);
		SDL_Surface* upLouisPinkColor(SDL_Surface* surface);
		SDL_Surface* upLouisBrownColor(SDL_Surface* surface);

		/*
		 * SURFACE
		 */
		//SplashScreen
		SDL_Surface *splashScreenSurface;
		//background for menu
		SDL_Surface *menuBackgroundSurface;
		//background for game
		SDL_Surface *backgroundSurface;
		
		SDL_Surface **shadowAreaSprite;
		SDL_Surface **playerSprite;
		SDL_Surface **louisSprite;
		SDL_Surface **fireSprite;
		SDL_Surface **bonusSprite;
		SDL_Surface **bombeSprite;
		SDL_Surface **previewLevelSprite;
		SDL_Surface **levelSprite;
		SDL_Surface **trolleySprite;
		SDL_Surface **spaceShipSprite;
		
		SDL_Surface **mine;
		SDL_Surface **teleporter;
		SDL_Surface **hole;
		SDL_Surface **waterOverlay;
		SDL_Surface **rail;
		SDL_Surface **button;

		/*
		 *	FONT
		 */
		 TTF_Font* font;
};
#endif
