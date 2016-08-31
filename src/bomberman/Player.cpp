#include "Bomberman.h"


Bomberman::Bomberman()
{
	/*init a new buffer like SDL mecanisme, 2 surface for rendering, 
      in C++ code calculate a surface and when it's done, 
      we can call swapBuffer to render It !
    */
    
    image0 = IMG_Load( splash_screen );
    
    Uint32 rmask, gmask, bmask, amask;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;
    
    image1 = SDL_CreateRGBSurface(0, 1920, 1080, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(image1, NULL, SDL_MapRGB(image1->format, 255, 204, 0));
    
    currentImage = 0;
    
    //generation d'une grille de 35 sur 21 case
    grid.configure(35,21,8);
    copySurfaceToBackRenderer(grid.getGrid(), 15, 72);
}

Bomberman::~Bomberman()
{
	free(image0);
	free(image1);
}

void Bomberman::tick(unsigned short * in_keystate, SDL_Surface * vout_buf){
    fprintf(stderr, "tick! ");
    for(int i = 0; i < 16; i++){
        fprintf(stderr, "%u, ", in_keystate[i]);
    }
    fprintf(stderr, "\n vout_buf : %i ", vout_buf->w);
    
    if(currentImage == 1){
        SDL_BlitSurface(image1,NULL,vout_buf ,NULL);
    }else{
        SDL_BlitSurface(image0,NULL,vout_buf ,NULL);
    }
	
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