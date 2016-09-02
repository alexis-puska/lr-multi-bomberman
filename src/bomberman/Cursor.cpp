#include "Cursor.h"


Cursor::Cursor(){
	//declarativ color mask, used for create a RGB surface
    Uint32 rmask, gmask, bmask, amask;
    amask = 0xff000000;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    
    cursor = new SDL_Surface*[4];
    
    SDL_Surface *cursorBuffer = IMG_Load(cursorSprite);
    SDL_Rect src;
    SDL_Rect dst;
	
	dst.x = 0;
	dst.y = 0;
	dst.w = 16;
	dst.h = 16;
    for(int i = 0; i < 4; i++){
    	src.x = i * 16;
	    src.y = 0;
	    src.w = 16;
	   	src.h = 16;
	    cursor[i] =  SDL_CreateRGBSurface(0, 16, 16, 32, rmask, gmask, bmask, amask);
	    SDL_BlitSurface(cursorBuffer, &src, cursor[i], &dst);
    }
	SDL_FreeSurface(cursorBuffer);
}

Cursor::~Cursor(){
	for(int i = 0; i < 4; i++){
        SDL_FreeSurface(cursor[i]);
    }
}

SDL_Surface * Cursor::getCurrentFrame(){
	if(frame % 15 == 0){
		position++;
	}
	if(frame == 61){
		frame = 1;
	}
	frame++;
	if(position > 3){
		position = 0;	
	}
	return cursor[position];
}
