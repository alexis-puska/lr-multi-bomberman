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
enum viewerStateEnum {
	menu= -1, gameViewerStart = 0, gameViewerPause = 1, gameViewerWait = 2, gameViewerEnd = 3, generateViewerResult = 4,
};

class ClientViewer {
	public:
		ClientViewer(SDL_Surface * vout_bufLibretro);
		~ClientViewer();
		void decode(char data[2048]);

	private:


		void drawServerWaitForClient();
		void drawPlayerTypeScreen();
		void drawSpriteTypeScreen();
		void drawGameOptionScreen();
		void drawLevelInfoScreen();
		void drawGameScreen();

		void copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y);
		void playSound(int sound, int channel, int active);

		void generateGround();


		int nbConnected[2];
		int playerType[16];
		int playerSprite[16];
		int playerState[16];
		int gameOption[4];
		int levelInfo[19];
		int gameState;
		int tickRemaining;
		bool newCycle;

		SDL_Surface *louisMergebuffer;
		Uint32 rmask, gmask, bmask, amask;

		bool somethingDrawInSky;
		char tab[735];
		char tabBonus[735];
		SDL_Surface * vout_buf;
		SDL_Surface * screenBuffer;
		SDL_Surface * ground;
		SDL_Surface * brickShadow;
		SDL_Surface * skyFixe;
		SDL_Surface * playerBombeExplode;
		SDL_Surface * overlay;
		SDL_Surface * overlayResult;

		void mergeScreen(bool mergeResult);

		void updateGameInfo(int tickRemaining, bool newCycle, int gameState);
		void updateTab(int idx, int val);
		void updateTabBonus(int idx, int val);
		void drawPlayer(float posX, float posY, int sprite, int louisSprite, int spaceShipSprite, bool inverse);
		void updatePlayerState(int idx, int val);
		void clearArea(int idx);
		void drawRail(int idx, int sprite);
		void drawTrolley(float posX, float posY, int sprite);
		void drawButton(int idx, int sprite);
		void drawBurnLouis(float posX, float posY, int sprite);
		void drawBurnBonus(int idx, int sprite);
		void drawBurnWall(int idx, int sprite);
		void drawExplosion(int idx, int type, int sprite);
		void drawPopBonus(int idx, int sprite);
		void drawSuddentDeath(float posX, float posY, int sprite);
		void drawBombe(float posX, float posY, int type, int sprite);
		void drawHole(int idx, int sprite);
		void drawMine(int idx, int sprite);
		void drawTeleporter(int idx, int sprite);
		void drawGhost(float posX, float posY);
		void playMusique(int musique, bool start);
		void drawBonus(int idx, int type);
		void eraseBonus(int idx);

};
#endif
