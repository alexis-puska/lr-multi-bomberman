#ifndef __MYWRAPPER_H
#define __MYWRAPPER_H
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Bomberman Bomberman;

Bomberman* _newBomberman();

void _bomberman_getScreen(Bomberman* v);

void _deleteBomberman(Bomberman* v);

#ifdef __cplusplus
}
#endif
#endif