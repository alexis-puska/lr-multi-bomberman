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
#include "Rail.h"
#include "../utils/network/BomberNetServer.h"

#ifndef __MYCLASS_TROLLEY
#define __MYCLASS_TROLLEY

enum drawDirection {
	drawTrolleyHorizontal = 0, drawTrolleyTopLeftBottomRight = 2, drawTrolleyVertical = 1, drawTrolleyTopRightBottomLeft = 3
};

enum drawPlayerDirection {
	drawPlayerNorth = 5, drawPlayerNorthEast = 7, drawPlayerEast = 0, drawPlayerSouthEast = 2, drawPlayerSouth = 1, drawPlayerSouthWest = 3, drawPlayerWest = 4, drawPlayerNorthWest = 6
};

class Trolley {
	public:
		Trolley(int index);
		Trolley(int index, std::vector<Player*> * players, std::map<int, Rail *> * rails);
		~Trolley();
		void doSomething(SDL_Surface * surface);
		void drawHimself(SDL_Surface * surface);
		int getCurrentIndex();
		int getPreviousIndex();
		bool isMove();
	private:
		//case actuelle
		int index;
		int prevIndex;
		int previousMove;
		//position en mouvement
		double x;
		double y;
		//en déplacement
		bool move;
		bool activate[nbPlayer];
		bool placeInMiddle;

		int cur;
		int nxt;
		int prv;
		Rail * rail;

		int playerInside;
		int direction;
		int playerDirection;

		std::vector<Player *> * players;
		std::map<int, Rail *> * rails;
		void correctValue();
		void moveTrolley();
};
#endif
