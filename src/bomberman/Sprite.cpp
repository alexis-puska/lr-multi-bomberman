#include "Sprite.h"
#include <vector>

const static char *splashScreen = "./resources/image/SplashScreen.png";
const static char *menuBackground = "./resources/image/MenuBackground.png";
const static char *spriteBombermanPath = "./resources/sprite/characters/AllBomberman.png";
const static char *spriteBombermanBarbarPath = "./resources/sprite/characters/AllBombermanBarbar.png";
const static char *spriteBombermanChanPath = "./resources/sprite/characters/AllBombermanChan.png";
const static char *spriteBombermanCossakPath = "./resources/sprite/characters/AllBombermanCossak.png";
const static char *spriteBombermanKidPath = "./resources/sprite/characters/AllBombermanKid.png";
const static char *spriteBombermanMexican = "./resources/sprite/characters/AllBombermanMexican.png";
const static char *spriteBombermanPrettyPath = "./resources/sprite/characters/AllBombermanPretty.png";
const static char *spriteBombermanPunkPath = "./resources/sprite/characters/AllBombermanPunk.png";
const static char *spriteFirePath = "./resources/sprite/other/Fire.png";
const static char *spriteMiscPath = "./resources/sprite/other/Misc.png";
const static char *spriteCursorPath = "./resources/sprite/other/Cursor.png";


Sprite Sprite::m_instance = Sprite();

Sprite::Sprite()
{
    fprintf(stderr,"Init sprite system\n");
    splashScreenSurface = IMG_Load(splashScreen);
    menuBackgroundSurface = IMG_Load(menuBackground);
    spriteBombermanSurface = IMG_Load(spriteBombermanPath);
	spriteBombermanBarbarSurface = IMG_Load(spriteBombermanBarbarPath);
	spriteBombermanChanSurface = IMG_Load(spriteBombermanChanPath);
	spriteBombermanCossakSurface = IMG_Load(spriteBombermanCossakPath);
	spriteBombermanKidSurface = IMG_Load(spriteBombermanKidPath);
	spriteBombermanMexicanSurface = IMG_Load(spriteBombermanMexican);
	spriteBombermanPrettySurface = IMG_Load(spriteBombermanPrettyPath);
	spriteBombermanPunkSurface = IMG_Load(spriteBombermanPunkPath);
	spriteCursorSurface = IMG_Load(spriteFirePath);
	spriteFireSurface = IMG_Load(spriteMiscPath);
	spriteMiscSurface = IMG_Load(spriteCursorPath);
	cropSurface();
}

Sprite::~Sprite()
{
	fprintf(stderr,"close sprite system\n");
   	SDL_FreeSurface (splashScreenSurface);
   	SDL_FreeSurface (menuBackgroundSurface);
   	SDL_FreeSurface (spriteBombermanSurface);
	SDL_FreeSurface (spriteBombermanBarbarSurface);
	SDL_FreeSurface (spriteBombermanChanSurface);
	SDL_FreeSurface (spriteBombermanCossakSurface);
	SDL_FreeSurface (spriteBombermanKidSurface);
	SDL_FreeSurface (spriteBombermanMexicanSurface);
	SDL_FreeSurface (spriteBombermanPrettySurface);
	SDL_FreeSurface (spriteBombermanPunkSurface);
	SDL_FreeSurface (spriteCursorSurface);
	SDL_FreeSurface (spriteFireSurface);
	SDL_FreeSurface (spriteMiscSurface);	
}

Sprite& Sprite::Instance()
{
    return m_instance;
}

/***********************************************
 * 
 * 				INFORMATION FUNCTION
 * 
 **********************************************/
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


/***********************************************
 * 
 * 				COLOR FUNCTION
 * 
 **********************************************/
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
SDL_Surface* Sprite::upPlayerGreyColor(SDL_Surface* surface){
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
SDL_Surface* Sprite::upPlayerRedColor(SDL_Surface* surface){
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
SDL_Surface* Sprite::upPlayerBlueColor(SDL_Surface* surface){
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
SDL_Surface* Sprite::upPlayerGreenColor(SDL_Surface* surface){
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
SDL_Surface* Sprite::upPlayerGoldColor(SDL_Surface* surface){
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
SDL_Surface* Sprite::upPlayerBrownColor(SDL_Surface* surface){
	replaceColor(surface, 0xfff8f8f8, 0xffB05040);
	replaceColor(surface, 0xffa0a0a0, 0xff800010);
	replaceColor(surface, 0xfff8a020, 0xfff8a020);
	replaceColor(surface, 0xff0058e8, 0xffB0B0B0);
	replaceColor(surface, 0xff00b018, 0xffB0B0B0);
	replaceColor(surface, 0xffe82050, 0xffF8B000);
	return surface;
}

/********************************************
 * 
 *		CROP SURFACE
 * 
 ********************************************/
 void Sprite::cropSurface(){
	 
 }
 
 /********************************************
 * 
 *		REQUEST FUNCTION
 * 
 ********************************************/
 
SDL_Surface* Sprite::getSplashScreen(){
	return splashScreenSurface;
}

SDL_Surface* Sprite::getMenuBackground(){
	return menuBackgroundSurface;
}
 
 SDL_Surface* Sprite::players(int type, int color, int mvt, int pos){
	 return null;
}