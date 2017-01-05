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

#define nbSpritePlayer		53
#define nbColorPlayer		7
#define nbPlayer			16
#define nbTypePlayer		8

#define nbSpriteLouis		27
#define nbTypeLouis 		5

#define nbLevel			9
#define nbFireSprite		36
#define nbMiscSprite		40
#define nbCursorSprite		4

#define nbSmallSpriteLevel	40
#define nbLargeSpriteLevel	2

#define spritePlayerSizeWidth	30
#define spritePlayerSizeHeight	42

#define spriteCursorSize	16

class Sprite {

	public:
		static Sprite& Instance();
		SDL_Surface* getSplashScreen();
		SDL_Surface* getMenuBackground();
		Sprite();
		~Sprite();
		SDL_Surface* players(int type, int color, int mvt, int pos);
	private:
		Sprite& operator=(const Sprite&);
		Sprite(const Sprite&);
		static Sprite m_instance;
		void cropSurface();
		void cropPlayerSurface(SDL_Surface*, int offset);
		void cropPreviewLevelSurface(SDL_Surface * surface);
		void cropMiscSurface(SDL_Surface * surface);
		void cropFireSurface(SDL_Surface * surface);
		void cropCursorSurface(SDL_Surface * surface);
		void cropLevelSurface(SDL_Surface * surface);
		void cropLouisSurface(SDL_Surface * surface);

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
		SDL_Surface **miscSprite;
		SDL_Surface **cursorSprite;
		SDL_Surface **previewLevelSprite;
		SDL_Surface **levelSprite;
};
#endif
