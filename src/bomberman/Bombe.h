#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#else
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>
#endif

#include <stdio.h>
#include "Explosion.h"
#include "Grid.h"

#ifndef __MYCLASS_BOMBE
#define __MYCLASS_BOMBE

#define sizeX 35
#define sizeY 21

class Bombe {

	public:
		Bombe(int strenght, float posX, float posY, int bombeType, int player, int nbTickExplode, SDL_Surface ** animation, int tab[sizeX * sizeY], float * tabPlayerCoord,
		Mix_Chunk *bombeBounceSound);
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
		SDL_Surface ** animation;
		int * tab;
		float * tabPlayerCoord;
		Mix_Chunk *bombeBounceSound;

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
