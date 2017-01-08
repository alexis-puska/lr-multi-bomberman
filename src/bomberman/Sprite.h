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
#define nbBonusSpriteY			7

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


#define nbTrolleySpriteX		4
#define nbTrolleySpriteY		1
#define trolleySpriteSizeWidth	30
#define trolleySpriteSizeHeight	42

#define nbSpaceShipSpriteX		2
#define nbSpaceShipSpriteY		4
#define spaceShipSpriteSizeWidth	30
#define spaceShipSpriteSizeHeight	42

class Sprite {

	public:
		static Sprite& Instance();
		SDL_Surface* getSplashScreen();
		SDL_Surface* getMenuBackground();
		Sprite();
		~Sprite();
		SDL_Surface* players(int type, int color, int mvt, int pos);
		SDL_Surface* getCursor(int pos);
		SDL_Surface* getLevelPreview(int pos);
		SDL_Surface* getFire(int x, int y);
		SDL_Surface* getBombe(int x, int y);
		SDL_Surface* getBurnWall(int pos, int levelIndex);
	private:
		Sprite& operator=(const Sprite&);
		Sprite(const Sprite&);
		static Sprite m_instance;
		void cropSurface();
		void cropPlayerSurface(SDL_Surface*, int offset);
		void cropPreviewLevelSurface(SDL_Surface * surface);
		void cropBombeSurface(SDL_Surface * surface);
		void cropFireSurface(SDL_Surface * surface);
		void cropBonusSurface(SDL_Surface * surface);
		void cropLevelSurface(SDL_Surface * surface);
		void cropLouisSurface(SDL_Surface * surface);
		void cropTrolleySurface(SDL_Surface * surface);
		void cropSpaceShipSurface(SDL_Surface * surface);


		/*
		 * INFORMATION FUNCTIN
		 */
		void getColorInSurface(SDL_Surface* surface);

		/*
		 * COLOR FUNCTION
		 */
		SDL_Surface* applyPlayerColor(SDL_Surface * surface, int color);
		SDL_Surface* replaceColor(SDL_Surface* surface, int src, int dest);
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

		SDL_Surface **playerSprite;
		SDL_Surface **louisSprite;
		SDL_Surface **fireSprite;
		SDL_Surface **bonusSprite;
		SDL_Surface **bombeSprite;
		SDL_Surface **previewLevelSprite;
		SDL_Surface **levelSprite;
		SDL_Surface **trolleySprite;
		SDL_Surface **spaceShipSprite;
};
#endif
