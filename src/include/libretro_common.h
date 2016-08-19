#include "../include/pad.h"
#include "../include/image_ressource.h"
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#define VOUT_MAX_WIDTH 1920
#define VOUT_MAX_HEIGHT 1080
#define VOUT_WIDTH 1920
#define VOUT_HEIGHT 1080

// libretro pointer for video/audio/input


static SDL_Surface *screen;

static unsigned short in_keystate[16];
static int in_type[16] =  { 
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD,
	PSE_PAD_TYPE_STANDARD, PSE_PAD_TYPE_STANDARD
};


