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


enum objectLenght {
	drawScreenRequest = 2,
	playerTypeRequest = 18,
	spriteTypeRequest = 18,
	gameOptionRequest = 6,
	levelInfoRequest = 22,
};

enum screenToDrawEnum{
	playerTypeScreen = 0,
	spriteTypeScreen,
	gameOptionScreen,
	levelInfoScreen,
};

class ClientViewer {
	public:
		ClientViewer(SDL_Surface * vout_bufLibretro);
		~ClientViewer();
		void decode(char data[512]);

	private:
		SDL_Surface * vout_buf;

		void drawPlayerTypeScreen();
		void drawSpriteTypeScreen();
		void drawGameOptionScreen();
		void drawLevelInfoScreen();

		void copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y);
};
#endif
