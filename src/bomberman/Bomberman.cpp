#include "Bomberman.h"

Bomberman::Bomberman()
{
	screen = IMG_Load( background );
	fprintf(stderr, "call C++ %i %i\n", screen->w, screen->h);
	fprintf(stderr, "after call C++ \n");
}

Bomberman::~Bomberman()
{
}

void Bomberman::getScreen(){
	//screen = IMG_Load( background );
}
