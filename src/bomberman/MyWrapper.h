#ifndef __MYWRAPPER_H
#define __MYWRAPPER_H
#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Bomberman Bomberman;

Bomberman* newBomberman(SDL_Surface * vout_buf);
void bomberman_tick(Bomberman *v, unsigned short in_keystate[16]);

//SDL_Surface * bomberman_getScreen(Bomberman* v);
void bomberman_swapBuffer(Bomberman* v);
void deleteBomberman(Bomberman* v);

#ifdef __cplusplus
}
#endif
#endif
