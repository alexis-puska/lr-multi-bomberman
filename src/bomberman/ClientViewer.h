#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#else
#include <SDL2_net/SDL_net.h>
#include <SDL2_image/SDL_image.h>
#endif
#include "utils/Sound.h"
#include "utils/Sprite.h"
#include "utils/GameConfig.h"
#include "Grid.h"

#ifndef __MYCLASS_CLIENT_VIEWER
#define __MYCLASS_CLIENT_VIEWER

enum objectLenght {
	nbConnectedRequest = 4, drawScreenRequest = 2, playerTypeRequest = 18, spriteTypeRequest = 18, gameOptionRequest = 6, levelInfoRequest = 22,
};

enum screenToDrawEnum {
	nbConnectedScreen = 1, playerTypeScreen, spriteTypeScreen, gameOptionScreen, levelInfoScreen, gameScreen
};



class ClientViewer {
	public:
		ClientViewer(SDL_Surface * vout_bufLibretro);
		~ClientViewer();
		void decode(char data[2048]);

	private:
		SDL_Surface * vout_buf;
		SDL_Surface *screenBuffer;

		void drawServerWaitForClient();
		void drawPlayerTypeScreen();
		void drawSpriteTypeScreen();
		void drawGameOptionScreen();
		void drawLevelInfoScreen();

		void copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y);
		void playSound(int sound, int channel, int active);


		void generateGround();

		int nbConnected[2];
		int playerType[16];
		int playerSprite[16];
		int gameOption[4];
		int levelInfo[19];

		char tab[735];
		SDL_Surface *ground;
		SDL_Surface *brickShadow;
		SDL_Surface *skyFixe;
};
#endif
