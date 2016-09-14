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

enum bombeTypeEnum{
	normalBombeType	= 0,
	radioBombeType	= 1,
	powerBombeType	= 2,
	bubbleBombeType	= 3,
};







class Bombe
{

public:
	Bombe(int Strenght, int posX, int posY, int type, int numPlayerSetTheBombe, int nbTickBeforeExplostion, SDL_Surface ** miscSheet);
    ~Bombe();
    bool isExplode();
    void explode();
    int getStrenght();
    bool isPowerBombe();
    void pushBomb(int direction);
	void tick();


private:
	int nbTickExplode;
	bool hasExplode;
	float posX;
	float posY;
	int roundX;
	int roundY;
};
#endif