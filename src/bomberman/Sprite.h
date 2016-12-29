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
	private:
		Sprite& operator= (const Sprite&);
		Sprite (const Sprite&);
		SDL_Surface* replaceColor(SDL_Surface* surface, int src, int dest);
		void getColorInSurface(SDL_Surface* surface);
		static Sprite m_instance;
		
		
		
		
		//SplashScreen
		SDL_Surface *splashScreenSurface;
		//background for menu
		SDL_Surface *menuBackgroundSurface;
		
		SDL_Surface *allBombermanSurface;
		
	
};
#endif
