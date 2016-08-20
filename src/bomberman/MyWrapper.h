#ifndef __MYWRAPPER_H
#define __MYWRAPPER_H
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>



#ifdef __cplusplus
extern "C" {
#endif

typedef struct Bomberman Bomberman;

Bomberman* newBomberman();

SDL_Surface * bomberman_getScreen(Bomberman* v);
void bomberman_swapBuffer(Bomberman* v);
void deleteBomberman(Bomberman* v);

#ifdef __cplusplus
}
#endif
#endif