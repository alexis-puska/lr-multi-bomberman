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



#ifndef __MYCLASS_CURSOR
#define __MYCLASS_CURSOR
class Cursor{

	public:
		Cursor();
		~Cursor();
		
		SDL_Surface * getCurrentFrame();
		bool isAlive();
		int  getPosition();
		void setPosition(int newPosition);
		void startAnimation();
		void stopAnimation();
		
	private:
		bool isThreadAlive;
		int position;
		
		SDL_Thread *cursorThread;
		SDL_Surface **cursor;
};
#endif