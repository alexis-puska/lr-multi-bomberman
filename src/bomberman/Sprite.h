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

class Sprite {

	public:
		static Sprite& Instance();
		SDL_Surface* getSplashScreen();
		SDL_Surface* getMenuBackground();
		Sprite();
		~Sprite();
		SDL_Surface* players(int type, int color, int mvt, int pos);
	private:
		Sprite& operator= (const Sprite&);
		Sprite (const Sprite&);
		static Sprite m_instance;
		void cropSurface();
		
		/*
		 * INFORMATION FUNCTIN
		 */
		void getColorInSurface(SDL_Surface* surface);
		
		/*
		 * COLOR FUNCTION
		 */
		SDL_Surface* replaceColor(SDL_Surface* surface, int src, int dest);
		SDL_Surface* upPlayerGreyColor(SDL_Surface* surface);
		SDL_Surface* upPlayerRedColor(SDL_Surface* surface);
		SDL_Surface* upPlayerBlueColor(SDL_Surface* surface);
		SDL_Surface* upPlayerGreenColor(SDL_Surface* surface);
		SDL_Surface* upPlayerGoldColor(SDL_Surface* surface);
		SDL_Surface* upPlayerBrownColor(SDL_Surface* surface);
		
		/*
		 * SURFACE
		 */ 
		//SplashScreen
		SDL_Surface *splashScreenSurface;
		//background for menu
		SDL_Surface *menuBackgroundSurface;
		
		SDL_Surface *spriteBombermanSurface;
		SDL_Surface *spriteBombermanBarbarSurface;
		SDL_Surface *spriteBombermanChanSurface;
		SDL_Surface *spriteBombermanCossakSurface;
		SDL_Surface *spriteBombermanKidSurface;
		SDL_Surface *spriteBombermanMexicanSurface;
		SDL_Surface *spriteBombermanPrettySurface;
		SDL_Surface *spriteBombermanPunkSurface;
		SDL_Surface *spriteCursorSurface;
		SDL_Surface *spriteFireSurface;
		SDL_Surface *spriteMiscSurface;
		
		SDL_Surface **playerSprite;
};
#endif
