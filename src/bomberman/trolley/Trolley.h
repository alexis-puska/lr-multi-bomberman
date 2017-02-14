#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include "../utils/Sprite.h"
#include "../utils/GameConfig.h"

#ifndef __MYCLASS_TROLLEY
#define __MYCLASS_TROLLEY

class Trolley {
	public:
		Trolley(int index);
		~Trolley();
		void doSomething(SDL_Surface * surface);
		void drawHimself(SDL_Surface * surface);
		int getCurrentIndex();
		int getPrevIndex();

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

};
#endif