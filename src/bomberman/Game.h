#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#ifndef __MYCLASS_GAME
#define __MYCLASS_GAME
class Game
{

public:
	Game();
    ~Game();
	bool isAlive();
	void tick();
	void configure();
	void startGame();
	void exitGame();
private:
	//variable
	SDL_Thread *mainThread;
	bool isThreadAlive;
	//function
	
};
#endif