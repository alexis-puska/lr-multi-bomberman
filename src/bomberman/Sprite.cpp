#include "Sprite.h"
#include <vector>

const static char *splashScreen = "./resources/image/SplashScreen.png";
const static char *menuBackground = "./resources/image/MenuBackground.png";
const static char *allBomberman = "./resources/sprite/characters/AllBombermanBarbar.png";


Sprite Sprite::m_instance = Sprite();

Sprite::Sprite()
{
    fprintf(stderr,"Init sprite system\n");
    splashScreenSurface = IMG_Load(splashScreen);
    menuBackgroundSurface = IMG_Load(menuBackground);
    allBombermanSurface = IMG_Load(allBomberman);
    
}

Sprite::~Sprite()
{
	fprintf(stderr,"close sprite system\n");
   	SDL_FreeSurface (splashScreenSurface);
   	SDL_FreeSurface (menuBackgroundSurface);
   	SDL_FreeSurface (allBombermanSurface);
}

Sprite& Sprite::Instance()
{
    return m_instance;
}

SDL_Surface* Sprite::getSplashScreen(){
	return splashScreenSurface;
}

SDL_Surface* Sprite::getMenuBackground(){
	//0xfff8f8f8 -> blanc bonnet
	//0xffa0a0a0 -> gris bonnet
	//0xfff8a020 -> visage
	//0xff0058e8 -> bleu
	//0xff00b018 -> vert
	//0xffe82050 -> gant ponpon chaussure bandeau
	replaceColor(allBombermanSurface, 0xfff8f8f8, 0xffc00000);
	replaceColor(allBombermanSurface, 0xffa0a0a0, 0xff800000);
	//replaceColor(allBombermanSurface, 0xfff8a020, 0xFFAAAA00);
	replaceColor(allBombermanSurface, 0xff0058e8, 0xffc0c0c0);
	replaceColor(allBombermanSurface, 0xff00b018, 0xffc0c0c0);
	replaceColor(allBombermanSurface, 0xffe82050, 0xff30a060);
	
	return menuBackgroundSurface;
}

SDL_Surface* Sprite::replaceColor(SDL_Surface* surface, int src, int dest){
	//lock the surface for work on it
	if(SDL_MUSTLOCK(surface)){
        SDL_LockSurface(surface);
    }
    //get pointer on the pixels table
    Uint32 *pixels = (Uint32 *)surface->pixels;
	for(int x=0;x<surface->w;x++){
		for(int y=0;y<surface->h;y++){
			if(pixels[y*surface->w+x] == src){
				pixels[y*surface->w+x] = dest;
			}
		}
	}
	//unlock the surface after work
	if(SDL_MUSTLOCK(surface)){
        SDL_UnlockSurface( surface );
    }
	return surface;
}

void Sprite::getColorInSurface(SDL_Surface* surface){
	std::vector<int> colors;
	bool found = false;
	//lock the surface for work on it
	if(SDL_MUSTLOCK(surface)){
        SDL_LockSurface(surface);
    }
    //get pointer on the pixels table
    Uint32 *pixels = (Uint32 *)surface->pixels;
	for(int x=0;x<surface->w;x++){
		for(int y=0;y<surface->h;y++){
			found = false;
			for(unsigned i = 0 ; i < colors.size();i++){
				if(pixels[y*surface->w+x] == colors[i]){
					found = true;
					break;	
				}
			}
			if(!found){
				colors.push_back(pixels[y*surface->w+x]);
			}
		}
	}
	fprintf(stderr,"colors in surface : ");
	for(unsigned i = 0 ; i < colors.size();i++){
		fprintf(stderr,"%x, ", colors[i]);
	}
	//unlock the surface after work
	if(SDL_MUSTLOCK(surface)){
        SDL_UnlockSurface( surface );
    }
}