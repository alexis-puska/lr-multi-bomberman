#include "Bomberman.h"

Bomberman::Bomberman()
{
	image = IMG_Load( background );
	fprintf(stderr, "call C++ %i %i\n", image->w, image->h);
	fprintf(stderr, "after call C++ \n");
}

Bomberman::~Bomberman()
{
}

SDL_Surface * Bomberman::getScreen(){
	//SDL_Surface * img = IMG_Load( background );
	return image;
}
