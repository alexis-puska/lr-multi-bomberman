#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include <vector>
#include <map>

#include "../utils/Sprite.h"
#include "../utils/GameConfig.h"
#include "../Player.h"
#include "rail.h"

#ifndef __MYCLASS_TROLLEY
#define __MYCLASS_TROLLEY

class Trolley {
	public:
		Trolley(int index);
		Trolley(int index, std::vector <Player*> * players, std::map<int, Rail *> * rails);
		~Trolley();
		void doSomething(SDL_Surface * surface);
		void drawHimself(SDL_Surface * surface);
		int getCurrentIndex();
		int getPrevIndex();
		int getDirection();
		int getPlayerInside();
		int getPlayerOutside();

		bool isMove();
	private:
		//case présent
		int index;
		int prevIndex;
		//position en mouvement
		float x;
		float y;
		bool move;
		bool activate[nbPlayer];

		int playerInside;
		int playerOutside;
		int direction;

		std::vector <Player *> * players;
		std::map<int, Rail *> * rails;
};
#endif