#include <SDL2/SDL.h>

#ifndef IS_OSX
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
#else
    #include <SDL2_image/SDL_image.h>
    #include <SDL2_ttf/SDL_ttf.h>
#endif

#include <stdio.h>
#include "Grid.h"
#include "Cursor.h"


enum bombermanStepEnum{
		home = 0,					//spashscreen
		PlayerTypeMenu, 		//type of Player CPU / HUMAN / OFF
		PlayerSpriteMenu,
		gameOptionMenu,			//Sudden death / Bad Bomber / CPU Level
		levelSelectionMenu,		// Select the grid design
		game
	};
	
	
enum keyPad{
	keyPadSelect	= 1,
	keyPadL3		= 2,
	keyPadR3		= 4,
	keyPadStart		= 8,
	keyPadUp		= 16,
	keyPadRight		= 32,
	keyPadDown		= 64,
	keyPadLeft		= 128,
	keyPadL2		= 256,
	keyPadR2		= 512,
	keyPadL1		= 1024,
	keyPadR1		= 2048,
	keyPadX			= 4096,
	keyPadA			= 8192,
	keyPadB			= 16384,
	keyPadY			= 32768	
	};
	
enum playerTypeEnum{
	HUMAN	= 0,
	CPU 	= 1,
	OFF		= 2	
};

enum gameOptionEnum{
	suddenDeathOption		= 0,
	badBomberOption			= 1,
	cpuLevelOption			= 2
};


#ifndef __MYCLASS_H
#define __MYCLASS_H
class Bomberman
{

public:
	Bomberman();
    ~Bomberman();
    
    //function call for every frame
    void tick(unsigned short *in_keystate, SDL_Surface * vout_buf);
    
    //function to copy some surface into an other
    void copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y);
private:

	void drawPlayerTypeMenu(unsigned short * in_keystate, SDL_Surface * vout_buf);
	void drawPlayerSpriteMenu(unsigned short * in_keystate, SDL_Surface * vout_buf);
	void drawGameOptionMenu(unsigned short * in_keystate, SDL_Surface * vout_buf);;
	void drawLevelSelectionMenu(unsigned short * in_keystate, SDL_Surface * vout_buf);
    //function to evoid key repetition
    void keyPressed(unsigned short * in_keystate);

    //position index for the cursor
	int cursorPosition;
    //the bomb cursor
	Cursor cursor;
	
    //refreshbuffer, for menu, if need to be full redraw
	bool refreshBuffer;
	
	//table for memorise player type
    //index 0 : 0 Human, 1 CPU, 2 OFF
    //index 1 : sprite used for player
	int playerType[16][2];
	/*
	* 0 : SuddenDeath	0 off, 1 on
	* 1 : Bad Bomber	0 off, 1 on
	* 2 : CPU Level		[0-3]
	*/
	int gameOption[3];
    
	//buffer for draw
	SDL_Surface *screenBuffer;
    
    //image of differente sprite of player
    SDL_Surface **menuPlayerSprite;
    
    //image of the different level available
    SDL_Surface **menuLevelSprite;
    
    //SplashScreen
    SDL_Surface *splashScreenSurface;
    
    //background for menu
    SDL_Surface *menuBackgroundSurface;    
    
    //Font for menu and Game
    TTF_Font* fragileBombersFont;
    
    //currentStep index to know wich menu is display
    bombermanStepEnum currentStep;
    
    //boolean set to true when a keystate change
	bool keychange[16];
    unsigned short previousPlayerKeystate[16];
    
    
    //Game Grid
    Grid grid;
    
    int levelIndex;
    
};
#endif