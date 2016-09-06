#ifndef IS_OSX
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_thread.h>
#else
    #include <SDL2_image/SDL_image.h>
    #include <SDL2_ttf/SDL_ttf.h>
    #include <SDL2/SDL_thread.h>
#endif


#include <stdio.h>



#ifndef __MYCLASS_BOMBE
#define __MYCLASS_BOMBE
class Bombe
{

public:
	Bombe();
    ~Bombe();
private:
};
#endif