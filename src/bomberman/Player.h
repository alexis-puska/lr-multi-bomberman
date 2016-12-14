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
#include <time.h>

#include "Bombe.h"
#include "Grid.h"

#define sizeX 35
#define sizeY 21

#ifndef __MYCLASS_PLAYER
#define __MYCLASS_PLAYER
class Player {
	public:
		Player(unsigned short * in_keystate, bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY, int playerNumber, int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY],
				SDL_Surface ** bombeSprite, Grid * gridParam, float * tabPlayerCoord, int nbPlayerConfig, int indexPlayerForGame);
		~Player();
		void doSomething(SDL_Surface * surfaceToDraw);

		/*
		 * Game Function
		 */
		Bombe * addBombe();

		bool wantPutBombe();
		bool triggerPowerBombe();
		bool walkOnWall();
		bool isAlive();
		bool hasKickPower();

		void winTheGame();
		void ABombeExplode();
		void releaseTrigger();
		void ABombeIsSet();

		//kick
		int isKickingBombe();
		int getKickDirection();
		void releaseKick();

		int getPlayerNumber();
		void itSuddenDeathTime();
		int getIndexPlayerForGame(); 

	private:
		/*
		 * VAR
		 */

		//Object variable
		bool cpu;
		int playerNumber;
		int indexPlayerForGame;
		int nbPlayerConfig;
		int characterSpriteIndex;

		//Game variable
		float posX;
		float posY;
		float playerSpeed;
		int playerState;
		int nbTickMalus;
		int playerMalus;
		float previousSpeedValue;
		int previousBombeNumber;

		bool ghostModePower;
		bool kickPower;
		bool inSuddenDeathTime;
		int kickIndex;
		int kickDirection;

		//bombe variable
		bool triggerBombe;
		bool putABombe;
		int flameStrengh;
		int bombeType;
		int NbBombeMax;
		int NBBombeRemaining;

		//louis bunring time
		int invincibleTime;

		//direction variable
		unsigned short * in_keystate;
		int previousDirection;

		//animation part
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
		int louisType;

		//pointer to the grid element;
		int * tab;
		int * tabBonus;
		float * tabPlayerCoord;
		SDL_Surface ** bombeSprite;
		Grid * grid;

		//image of differente sprite of player
		SDL_Surface **playerSpriteWalk;
		SDL_Surface **playerSpriteWalkBomb;
		SDL_Surface **playerSpriteThrowBomb;
		SDL_Surface **playerSpriteOnLouis;
		SDL_Surface **playerSpriteVictory;
		SDL_Surface **playerSpriteAngry;
		SDL_Surface **playerSpriteBurn;
		SDL_Surface **louisSprite;
		SDL_Surface **louisSpriteBurn;
		SDL_Surface *louisMergebuffer;

		/*
		 * FUNCTION For draw player
		 */
		void drawNormal(SDL_Surface * surfaceToDraw, bool animate);
		void drawOnLouis(SDL_Surface * surfaceToDraw, bool animate);
		void drawWithBombe(SDL_Surface * surfaceToDraw, bool animate);
		void drawThrowBombe(SDL_Surface * surfaceToDraw, bool animate);
		void drawBurning(SDL_Surface * surfaceToDraw, bool animate);
		void drawLouisBurning(SDL_Surface * surfaceToDraw, bool animate);
		void drawVictory(SDL_Surface * surfaceToDraw, bool animate);
		void drawCrying(SDL_Surface * surfaceToDraw, bool animate);

		//malus function
		void getAMalusBonus();
		void releaseMalus();
		int findIndexPlayer();
		void foundABonus(int bonusIndex);
};
#endif
