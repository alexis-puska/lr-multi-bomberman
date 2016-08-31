#include "Bomberman.h"

#include "SDL_thread.h"
#include "SDL_timer.h"


Bomberman::Bomberman()
{
    //Init TTF feature
    TTF_Init();
    
    //color mask
    Uint32 rmask, gmask, bmask, amask;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;
    refreshBuffer = true;
    
    //Load Font
    fragileBombersFont = TTF_OpenFont( "./resources/font/fragile_bombers.ttf", 48); //this opens a font style and sets a size
    
    //init all surface
    screenBuffer = SDL_CreateRGBSurface(0, 1920, 1080, 32, rmask, gmask, bmask, amask);
    splashScreenSurface = IMG_Load(splashScreen);
    menuBackgroundSurface = IMG_Load(menuBackground);
 	
 	copySurfaceToBackRenderer(splashScreenSurface, screenBuffer, 0, 0);
 	
 	
	currentStep = home;
	
	//init default player type
	playerType[0][0]=0; 
	playerType[0][1]=0;
	for(int i=1; i<16; i++){
		playerType[i][0]=1; 
		playerType[i][1]=i % 8;
	}
       
    //    image1 = SDL_CreateRGBSurface(0, 1920, 1080, 32, rmask, gmask, bmask, amask);
    //    SDL_FillRect(image1, NULL, SDL_MapRGB(image1->format, 255, 204, 0));
    
        //generation d'une grille de 35 sur 21 case
	grid.configure(35,21,8);
}

Bomberman::~Bomberman()
{
	SDL_FreeSurface(screenBuffer);
	SDL_FreeSurface(splashScreenSurface);
	SDL_FreeSurface(menuBackgroundSurface);

	TTF_CloseFont(fragileBombersFont);
    TTF_Quit();
}

void Bomberman::tick(unsigned short * in_keystate, SDL_Surface * vout_buf){
	//fprintf(stderr, "%u\n", in_keystate[0]);
	 //color mask
    Uint32 rmask, gmask, bmask, amask;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;
	
	keyPressed(in_keystate);
	
	
	//spash screen and start pressed !
    	if(previousPlayerKeystate[0] & keyPadStart && keychange[0]){
			refreshBuffer = true;
	    	switch(currentStep){
			   	case home:
			   		cursorPosition = 0;
				   	currentStep = PlayerTypeMenu;
					break;
			   	case PlayerTypeMenu:
			   		cursorPosition = 0;
					currentStep = PlayerSpriteMenu;
					break;
				case PlayerSpriteMenu:
					cursorPosition = 0;
					currentStep = gameOptionMenu;
					break;
			   	case gameOptionMenu:
			   		cursorPosition = 0;
					currentStep = levelSelectionMenu;
					break;
			   	case levelSelectionMenu:
			   		cursorPosition = 0;
					currentStep = game;
					break; 
				case game:
					break;
	    	}
    	}else if(previousPlayerKeystate[0] & keyPadSelect && keychange[0]){
			refreshBuffer = true;
    		switch(currentStep){
    			case home:
    				cursorPosition = 0;
    				break;
			   	case PlayerTypeMenu:
			   		cursorPosition = 0;
					currentStep = home;
					break;
				case PlayerSpriteMenu:
					cursorPosition = 0;
					currentStep = PlayerTypeMenu;
					break;
			   	case gameOptionMenu:
			   		cursorPosition = 0;
					currentStep = PlayerSpriteMenu;
					break;
			   	case levelSelectionMenu:
			   		cursorPosition = 0;
					currentStep = gameOptionMenu;
					break; 
				case game : 
					cursorPosition = 0;
					currentStep = levelSelectionMenu;
					break;
	    	}
    	}
    
	
    switch(currentStep){
	   	case home:
		   	if(refreshBuffer){
		   		
				SDL_BlitSurface(splashScreenSurface,NULL,vout_buf ,NULL);
				refreshBuffer = false;
		   	}
			break;
	   	case PlayerTypeMenu:
			drawPlayerTypeMenu(in_keystate, vout_buf);
			break;
		case PlayerSpriteMenu:
			drawPlayerSpriteMenu(in_keystate, vout_buf);
			break;
	   	case gameOptionMenu:
			drawGameOptionMenu(in_keystate, vout_buf);
			break;
	   	case levelSelectionMenu:
			drawLevelSelectionMenu(in_keystate, vout_buf);
			break; 
		case game:
			SDL_BlitSurface(grid.getGrid(),NULL,vout_buf ,NULL);
			break;
    }
}

void Bomberman::copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y){
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = src->w;
    dstRect.h = src->h;
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = src->w;
    srcRect.h = src->h;
    SDL_BlitSurface(src, &srcRect, dest, &dstRect);
}




/*
* Select the type of player
* 
* 
* 
* 
*/
void Bomberman::drawPlayerTypeMenu(unsigned short * in_keystate, SDL_Surface * vout_buf){
	Uint32 rmask, gmask, bmask, amask;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;
	
	//fprintf(stderr, "%d %d\n", refreshBuffer , keychange[0]);
	
	if(refreshBuffer || keychange[0]){
	
	
		SDL_BlitSurface(menuBackgroundSurface,NULL,screenBuffer  ,NULL);
		SDL_Surface* menu;
		menu =  SDL_CreateRGBSurface(0, 1720, 80, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
		copySurfaceToBackRenderer(menu, screenBuffer , 100, 450);
		
		menu =  SDL_CreateRGBSurface(0, 1720, 300, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
		copySurfaceToBackRenderer(menu, screenBuffer , 100, 550);
		
		SDL_Color red = {255, 0, 0};
		SDL_Color green = {0, 255, 0};
		SDL_Color blue = {0, 140, 255};
		
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "Configuration joueur", green);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer , ((1920/2)-(surfaceMessage->w/2)), 462);
		
		if(previousPlayerKeystate[0] & keyPadRight && keychange[0]){
			cursorPosition += 4;
			if(cursorPosition>15){
				cursorPosition = cursorPosition % 4;	
			}
		}if(previousPlayerKeystate[0] & keyPadLeft && keychange[0]){
			cursorPosition -= 4;
			if(cursorPosition < 0){
				cursorPosition +=16 ;	
			}
		} if(previousPlayerKeystate[0] & keyPadDown && keychange[0]){
			cursorPosition++;
			if(cursorPosition>15){
				cursorPosition = 0;	
			}
		} if(previousPlayerKeystate[0] & keyPadUp && keychange[0]){
			cursorPosition--;
			if(cursorPosition<0){
				cursorPosition = 15;	
			}
		} if(previousPlayerKeystate[0] & keyPadA && keychange[0]){
			playerType[cursorPosition][0]++;
			if(playerType[cursorPosition][0]>2){
				playerType[cursorPosition][0] = 0;	
			}
		} if(previousPlayerKeystate[0] & keyPadB && keychange[0]){
			playerType[cursorPosition][0]--;
			if(playerType[cursorPosition][0]<0){
				playerType[cursorPosition][0] = 2;	
			}
		}

		for(int j=0 ; j<4 ; j++){
			for(int i=0 ; i<4 ; i++){
				char playerName[12] = "Joueur ";
				char playerTypeName[6];
				sprintf(playerName, "Joueur %2i : ", (j*4+i)+1);
				
				SDL_Color playerColor = blue;
				switch(playerType[j*4+i][0]){
					case HUMAN : 
						sprintf(playerTypeName, "HUMAN");
						playerColor = blue;
						break;
					case CPU : 
						sprintf(playerTypeName, "CPU");
						playerColor = green;
						break;
					case OFF : 
						sprintf(playerTypeName, "OFF");
						playerColor = red;
						break;
				}
				SDL_Surface* surfaceMessage1 = TTF_RenderText_Solid(fragileBombersFont, playerTypeName, playerColor);
				SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(fragileBombersFont, playerName, green);
				copySurfaceToBackRenderer(surfaceMessage1, screenBuffer, 370+(j*400), 562+(i*60));
				copySurfaceToBackRenderer(surfaceMessage2, screenBuffer, 170+(j*400), 562+(i*60));	
			}
		}
		refreshBuffer = false;
		
	}
	int cursorPosX = 110 + (((cursorPosition - (cursorPosition % 4))/4)*400);
	int cursorposY = 562 + ((cursorPosition % 4)*60);	
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
	copySurfaceToBackRenderer(cursor.getCurrentFrame(), vout_buf, cursorPosX, cursorposY);
}

/*
* Menu for selecting the sprite of a player
* 
* 
* 
* 
*/
void Bomberman::drawPlayerSpriteMenu(unsigned short * in_keystate, SDL_Surface * vout_buf){
	Uint32 rmask, gmask, bmask, amask;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;
	
	SDL_BlitSurface(menuBackgroundSurface,NULL,vout_buf ,NULL);
	SDL_Surface* menu;
	menu =  SDL_CreateRGBSurface(0, 1720, 80, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
	copySurfaceToBackRenderer(menu, vout_buf, 100, 450);
	
	menu =  SDL_CreateRGBSurface(0, 1720, 300, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
	copySurfaceToBackRenderer(menu, vout_buf, 100, 550);
	
	SDL_Color red = {255, 0, 0};
	SDL_Color green = {0, 255, 0};
	//SDL_Color blue = {0, 140, 255};
	
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "SELECT COSTUME", green);
	copySurfaceToBackRenderer(surfaceMessage, vout_buf, ((1920/2)-(surfaceMessage->w/2)), 462);




	for(int j=0 ; j<4 ; j++){
		for(int i=0 ; i<4 ; i++){
			SDL_Surface* surfaceMessage1 = TTF_RenderText_Solid(fragileBombersFont, "CPU", red);
			SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(fragileBombersFont, "Player : ", green);
			copySurfaceToBackRenderer(surfaceMessage1, vout_buf, 350, 562);
			copySurfaceToBackRenderer(surfaceMessage2, vout_buf, 150, 562);	
		}
	}
}

/*
* Menu for game option
* 
* 
* 
* 
*/
void Bomberman::drawGameOptionMenu(unsigned short * in_keystate, SDL_Surface * vout_buf){
	Uint32 rmask, gmask, bmask, amask;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;
	
	SDL_BlitSurface(menuBackgroundSurface,NULL,vout_buf ,NULL);
	SDL_Surface* menu;
	menu =  SDL_CreateRGBSurface(0, 1720, 80, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
	copySurfaceToBackRenderer(menu, vout_buf, 100, 450);
	
	menu =  SDL_CreateRGBSurface(0, 1720, 300, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
	copySurfaceToBackRenderer(menu, vout_buf, 100, 550);
	
	SDL_Color red = {255, 0, 0};
	SDL_Color green = {0, 255, 0};
	//SDL_Color blue = {0, 140, 255};
	
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "GAME OPTION", green);
	SDL_Surface* surfaceMessage1 = TTF_RenderText_Solid(fragileBombersFont, "CPU", red);
	SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(fragileBombersFont, "Player 01 :", green);
	
	copySurfaceToBackRenderer(surfaceMessage, vout_buf, ((1920/2)-(surfaceMessage->w/2)), 462);
	
	copySurfaceToBackRenderer(surfaceMessage1, vout_buf, 350, 562);
	copySurfaceToBackRenderer(surfaceMessage2, vout_buf, 150, 562);	
}

/*
* Menu for level selection
* 
* 
* 
* 
*/
void Bomberman::drawLevelSelectionMenu(unsigned short * in_keystate, SDL_Surface * vout_buf){
	Uint32 rmask, gmask, bmask, amask;
    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;
	
	SDL_BlitSurface(menuBackgroundSurface,NULL,vout_buf ,NULL);
	SDL_Surface* menu;
	menu =  SDL_CreateRGBSurface(0, 1720, 80, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
	copySurfaceToBackRenderer(menu, vout_buf, 100, 450);
	
	menu =  SDL_CreateRGBSurface(0, 1720, 300, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
	copySurfaceToBackRenderer(menu, vout_buf, 100, 550);
	
	SDL_Color red = {255, 0, 0};
	SDL_Color green = {0, 255, 0};
	//SDL_Color blue = {0, 140, 255};
	
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "SELECT THE LEVEL", green);
	SDL_Surface* surfaceMessage1 = TTF_RenderText_Solid(fragileBombersFont, "CPU", red);
	SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(fragileBombersFont, "Player 01 :", green);
	
	copySurfaceToBackRenderer(surfaceMessage, vout_buf, ((1920/2)-(surfaceMessage->w/2)), 462);
	
	copySurfaceToBackRenderer(surfaceMessage1, vout_buf, 350, 562);
	copySurfaceToBackRenderer(surfaceMessage2, vout_buf, 150, 562);	
}


void Bomberman::keyPressed(unsigned short * in_keystate){
	for(int i = 0; i < 16 ; i++){
		if(previousPlayerKeystate[i] != in_keystate[i]){
			keychange[i] = true;
			previousPlayerKeystate[i] = in_keystate[i];
		}else{
			keychange[i] = false;	
		}
	}
}
