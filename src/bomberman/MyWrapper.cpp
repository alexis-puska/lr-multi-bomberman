#include "Bomberman.h"
#include "MyWrapper.h"

extern "C" {
	Bomberman* newBomberman() {
		return new Bomberman();
	}
	
	SDL_Surface * bomberman_getScreen(Bomberman* v) {
		return v->getScreen();
	}
	
	void bomberman_swapBuffer(Bomberman* v) {
		return v->swapBuffer();
	}
	
	void deleteBomberman(Bomberman* v) {
		delete v;
	}
	
	void bomberman_tick(Bomberman *v, unsigned short *in_keystate, SDL_Surface * vout_buf){
		v->tick(in_keystate, vout_buf);
	}
}