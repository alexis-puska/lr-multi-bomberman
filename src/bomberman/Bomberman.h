#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

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


	int cursorPosition;
	Cursor cursor;
	
	bool refreshBuffer;
	

	//table for memorise player type

	int playerType[16][2];

	//buffer for draw
	SDL_Surface *screenBuffer;
    
    //SplashScreen
    SDL_Surface *splashScreenSurface;
    
    //background for menu
    SDL_Surface *menuBackgroundSurface;    
    
    //Font for menu and Game
    TTF_Font* fragileBombersFont;
    
    //Game Grid
    Grid grid;
        
    bombermanStepEnum currentStep;
    
	void keyPressed(unsigned short * in_keystate);
    
	bool keychange[16];
    unsigned short previousPlayerKeystate[16];
};
#endif