#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include <map>
#include "../utils/Sprite.h"

#ifndef __MYCLASS_RAIL
#define __MYCLASS_RAIL

enum{
	RailBumperLeft = 0,
	RailBumperRight = 1,
	RailLeftToTop = 2,
	RailLeftToBottom = 3,
	RailRightToTop = 4,
	RailRightToBottom = 5,
	RailVertical = 6,
	RailHorizontal = 7
};

class Rail {
	public:
		Rail(int index, int prev, int next, int nextAlt);
		Rail(int index, int prev, int next);
		Rail(int index, int prev);
		Rail(int index);
		~Rail();
		void init(std::map<int,Rail*> rails);
		int getIndex();
		int getNext(int previous);
		void switching();
		bool isBumper();
		bool isSwitching();
		void drawHimself(SDL_Surface * surfaceToDraw);
	private:
		void initImpl(int index, std::map<int, Rail*> rails);
		int index;
		int nextIndex;
		int prevIndex;
		int nextIndexAlt;
};

#endif