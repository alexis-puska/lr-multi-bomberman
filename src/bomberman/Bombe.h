#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#else
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#endif

#include <stdio.h>
#include "Explosion.h"
#include "Grid.h"
#include "Sound.h"

#ifndef __MYCLASS_BOMBE
#define __MYCLASS_BOMBE

#define sizeX 35
#define sizeY 21

class Bombe {

	public:
		Bombe(int strenght, float posX, float posY, int bombeType, int player, int nbTickExplode, int tab[sizeX * sizeY], float * tabPlayerCoord);
		~Bombe();
		bool isExplode();
		bool isPowerBombe();

		void explode();
		void explodeNow();
		void pushBomb(int direction);
		void tick(SDL_Surface * surfaceToDraw);

		int getStrenght();
		int getPlayer();
		int getPosX();
		int getPosY();
		int getCase();

	private:
		int * tab;
		float * tabPlayerCoord;

		float posX;
		float posY;

		int direction;
		int nbTickExplode;
		int strenght;
		int player;
		int bombeType;

		//for animation	
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
};
#endif
