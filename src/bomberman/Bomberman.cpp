#include "Bomberman.h"


Bomberman::Bomberman()
{
	/*init a new buffer like SDL mecanisme, 2 surface for rendering, 
      in C++ code calculate a surface and when it's done, 
      we can call swapBuffer to render It !
    */
    
    image0 = IMG_Load( splash_screen );
    
    Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif
    
    image1 = SDL_CreateRGBSurface(0, 1920, 1080, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(image1, NULL, SDL_MapRGB(image1->format, 0, 204, 255));
    
    currentImage = 0;
    
    //generation d'une grille de 35 sur 21 case
    grid.setSize(35,21);
    copySurfaceToBackRenderer(grid.getGrid(), 15, 72);
}

Bomberman::~Bomberman()
{
	free(image0);
	free(image1);
}

SDL_Surface * Bomberman::getScreen(){
    if(currentImage == 1){
        return image1;
    }else{
        return image0;
    }
}

void Bomberman::swapBuffer(){
    if(currentImage == 0){
        currentImage = 1;
    }else{
        currentImage = 0;
    }
}

void Bomberman::copySurfaceToBackRenderer(SDL_Surface * toCopy){
    fprintf(stderr, "copy grid!\n");
    SDL_BlitSurface(toCopy,NULL,(currentImage == 0 ? image1 : image0) ,NULL);
}

void Bomberman::copySurfaceToBackRenderer(SDL_Surface * toCopy, int x, int y){
    fprintf(stderr, "copy grid!\n");
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = toCopy->w;
    dstrect.h = toCopy->h;
    SDL_BlitSurface(toCopy,NULL,(currentImage == 0 ? image1 : image0), &dstrect);
}