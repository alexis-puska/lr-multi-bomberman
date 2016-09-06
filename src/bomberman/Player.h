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



#ifndef __MYCLASS_PLAYER
#define __MYCLASS_PLAYER
class Player
{
public:
	Player(bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY);
    ~Player();
    void doSomething();
private:
	/*
	* VAR
	*/
	bool cpu;
	int characterSpriteIndex;
	int posX;
	int posY;
	//image of differente sprite of player
    SDL_Surface **playerSprite;
    
    
	/*
	* FUNCTION
	*/
    
};
#endif