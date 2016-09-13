#include "Bomberman.h"
#include "MyWrapper.h"

extern "C" {
	Bomberman* newBomberman(SDL_Surface * vout_buf) {
		return new Bomberman(vout_buf);
	}
	
	void deleteBomberman(Bomberman* v) {
		delete v;
	}
	
	void bomberman_tick(Bomberman *v, unsigned short in_keystate[16]){
		v->tick(in_keystate);
	}
}