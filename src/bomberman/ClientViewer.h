#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include "utils/Sound.h"
#include "utils/Sprite.h"
#include "utils/GameConfig.h"

#ifndef __MYCLASS_CLIENT_VIEWER
#define __MYCLASS_CLIENT_VIEWER

class ClientViewer {
	public:
		ClientViewer(SDL_Surface * vout_bufLibretro);
		~ClientViewer();
		void decode(char data[512]);
		bool checkKeystate();
	private:
		SDL_Surface * vout_buf;
		//keystate of client player
		bool keychange[16];
		unsigned short previousPlayerKeystate[16];
		void copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y);
};
#endif
