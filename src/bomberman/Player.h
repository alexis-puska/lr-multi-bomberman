#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>
#include <time.h>

#include "Bombe.h"
#include "Grid.h"
#include "utils/Sprite.h"
#include "animation/BurnLouis.h"

#define sizeX 35
#define sizeY 21

#ifndef __MYCLASS_PLAYER
#define __MYCLASS_PLAYER
class Player {
	public:
		Player(unsigned short * in_keystate, float startPositionX, float startPositionY, int playerNumber, int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY],
				Grid * gridParam, int indexPlayerForGame);
		~Player();
		void doSomething(SDL_Surface * surfaceToDraw);

		/*
		 * Game Function
		 */
		Bombe * addBombe();
		BurnLouis* louisBurnAnimation();

		bool wantPutBombe();
		bool triggerPowerBombe();
		bool walkOnWall();
		bool isAlive();
		bool hasKickPower();
		bool isLouisBurn();

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

		int getBombeType();

		void brainPressButton();

	private:
		/*
		 * VAR
		 */
		//Object variable
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
		int color;
		int louisType;
		bool louisBurn;

		//pointer to the grid element;
		int * tab;
		int * tabBonus;
		Grid * grid;

		//image of differente sprite of player
		SDL_Surface *louisMergebuffer;

		/*
		 * FUNCTION For draw player
		 */
		void drawNormal(SDL_Surface * surfaceToDraw, bool animate);
		void drawOnLouis(SDL_Surface * surfaceToDraw, bool animate);
		void drawWithBombe(SDL_Surface * surfaceToDraw, bool animate);
		void drawThrowBombe(SDL_Surface * surfaceToDraw, bool animate);
		void drawBurning(SDL_Surface * surfaceToDraw, bool animate);
		void drawVictory(SDL_Surface * surfaceToDraw, bool animate);
		void drawCrying(SDL_Surface * surfaceToDraw, bool animate);
		void drawVictoryOnLouis(SDL_Surface * surfaceToDraw, bool animate);

		//malus function
		void getAMalusBonus();
		void releaseMalus();
		int findIndexPlayer();
		void foundABonus(int bonusIndex);
};
#endif
