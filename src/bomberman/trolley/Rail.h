#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include <map>
#include "../utils/Sprite.h"
#include "../utils/network/BomberNetServer.h"

#ifndef __MYCLASS_RAIL
#define __MYCLASS_RAIL

#define sizeX 35
#define sizeY 21

enum {
	RailBumperLeft = 6, RailBumperRight = 7, RailLeftToTop = 5, RailLeftToBottom = 2, RailRightToTop = 3, RailRightToBottom = 0, RailVertical = 4, RailHorizontal = 1
};

class Rail {
	public:
		Rail(int index, int prev, int next, int nextAlt);
		Rail(int index, int prev, int next);
		Rail(int index, int prev);
		Rail(int index);
		~Rail();
		void init(std::map<int, Rail*> rails);
		int getIndex();
		int getPrevIndex();
		int getNext(int prev, int cur);
		void switching();
		bool isBumper();
		bool isSwitching();
		void drawHimself(SDL_Surface * surfaceToDraw);
		void print();
		int getNextIndex();
	private:
		void initImpl(int index, std::map<int, Rail*> rails);
		int index;
		int nextIndex;
		int prevIndex;
		int nextIndexAlt;
};

#endif
