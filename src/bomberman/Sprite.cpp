#include "Sprite.h"

const static char *splashScreen = "./resources/image/SplashScreen.png";
const static char *menuBackground = "./resources/image/MenuBackground.png";


Sprite Sprite::m_instance = Sprite();

Sprite::Sprite()
{
    fprintf(stderr,"Init sprite system\n");
    splashScreenSurface = IMG_Load(splashScreen);
    menuBackgroundSurface = IMG_Load(menuBackground);
}

Sprite::~Sprite()
{
	fprintf(stderr,"close sprite system\n");
   	SDL_FreeSurface (splashScreenSurface);
   	SDL_FreeSurface (menuBackgroundSurface);
}

Sprite& Sprite::Instance()
{
    return m_instance;
}

SDL_Surface* Sprite::getSplashScreen(){
	return splashScreenSurface;
}

SDL_Surface* Sprite::getMenuBackground(){
	return menuBackgroundSurface;
}