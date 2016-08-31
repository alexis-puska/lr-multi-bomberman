#include "Bomberman.h"

#include "SDL_thread.h"
#include "SDL_timer.h"

#include <time.h>


Bomberman::Bomberman()
{
    //Init TTF feature
    TTF_Init();
    
    srand (time(NULL));
    
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
	for(int i=0; i<16; i++){
		playerType[i][0]=1; 
		playerType[i][1]= (rand() % 7);
	}
    playerType[0][0]=0; 
	playerType[0][1]=0;
	
	//init GameOption
	gameOption[0] = 0;
	gameOption[1] = 0;
	gameOption[2] = 1;
	
    //init Sprite for menu
    menuPlayerSprite = new SDL_Surface*[8];
    for(int i=0;i<8;i++){
        menuPlayerSprite[i] =  SDL_CreateRGBSurface(0, 90, 126, 32, rmask, gmask, bmask, amask);
    }
    
    SDL_Rect dstRect;
    dstRect.x = 0;
    dstRect.y = 0;
    dstRect.w = 90;
    dstRect.h = 126;
    

    SDL_Surface *textureBuffer = IMG_Load(BombermanSprite);
    SDL_BlitSurface(textureBuffer, &dstRect, menuPlayerSprite[0], &dstRect);
    
    textureBuffer = IMG_Load(BombermanSpriteCossak);
    SDL_BlitSurface(textureBuffer, &dstRect, menuPlayerSprite[1], &dstRect);
    
    textureBuffer = IMG_Load(BombermanSpriteBarbar);
    SDL_BlitSurface(textureBuffer, &dstRect, menuPlayerSprite[2], &dstRect);
    
    textureBuffer = IMG_Load(BombermanSpriteChan);
    SDL_BlitSurface(textureBuffer, &dstRect, menuPlayerSprite[3], &dstRect);
    
    textureBuffer = IMG_Load(BombermanSpriteKid);
    SDL_BlitSurface(textureBuffer, &dstRect, menuPlayerSprite[4], &dstRect);
    
    textureBuffer = IMG_Load(BombermanSpritePretty);
    SDL_BlitSurface(textureBuffer, &dstRect, menuPlayerSprite[5], &dstRect);
    
    textureBuffer = IMG_Load(BombermanSpritePunk);
    SDL_BlitSurface(textureBuffer, &dstRect, menuPlayerSprite[6], &dstRect);
    
    textureBuffer = IMG_Load(BombermanSpriteMexican);
    SDL_BlitSurface(textureBuffer, &dstRect, menuPlayerSprite[7], &dstRect);
    
    
    //init Level sprite for menu
    
    textureBuffer = IMG_Load(levelView);
    SDL_Rect srcRect;
    dstRect.x = 0;
	dstRect.y = 0;
	dstRect.w = 384;
	dstRect.h = 330;
    
    menuLevelSprite = new SDL_Surface*[9];
    for(int j=0;j<3;j++){
		for(int i=0;i<3;i++){
			srcRect.x = i*384;
    		srcRect.y = j*330;
		    srcRect.w = 384;
		    srcRect.h = 330;
			menuLevelSprite[j*3+i] =  SDL_CreateRGBSurface(0, 384, 330, 32, rmask, gmask, bmask, amask);
			SDL_BlitSurface(textureBuffer, &srcRect, menuLevelSprite[j*3+i], &dstRect);
		}
    }
    
    
    //SDL_FillRect(image1, NULL, SDL_MapRGB(image1->format, 255, 204, 0));
    
    //generation d'une grille de 35 sur 21 case
	//grid.configure(35,21,8);
}




Bomberman::~Bomberman()
{
	SDL_FreeSurface(screenBuffer);
	SDL_FreeSurface(splashScreenSurface);
	SDL_FreeSurface(menuBackgroundSurface);
    for(int i = 0; i < 8; i++){
        SDL_FreeSurface(menuPlayerSprite[i]);
    }
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
			   		for(int i=0; i<16; i++){
						if(playerType[i][0] == 1){
							playerType[i][1]= (rand() % 7);
						}
					}
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
					cursorPosition = levelIndex;
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
			if(refreshBuffer && previousPlayerKeystate[0] & keyPadStart && keychange[0]){
				fprintf(stderr, "Generate Grid\n");
				grid.configure(35,21,levelIndex);
				refreshBuffer = false;
			}
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
		}
		if(previousPlayerKeystate[0] & keyPadLeft && keychange[0]){
			cursorPosition -= 4;
			if(cursorPosition < 0){
				cursorPosition +=16 ;	
			}
		}
		if(previousPlayerKeystate[0] & keyPadDown && keychange[0]){
			cursorPosition++;
			if(cursorPosition>15){
				cursorPosition = 0;	
			}
		}
		if(previousPlayerKeystate[0] & keyPadUp && keychange[0]){
			cursorPosition--;
			if(cursorPosition<0){
				cursorPosition = 15;	
			}
		}
		if(previousPlayerKeystate[0] & keyPadA && keychange[0]){
			playerType[cursorPosition][0]++;
			if(playerType[cursorPosition][0]>2){
				playerType[cursorPosition][0] = 0;	
			}
		}
		if(previousPlayerKeystate[0] & keyPadB && keychange[0]){
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
	
	if(refreshBuffer || keychange[0]){
		Uint32 rmask, gmask, bmask, amask;
	    rmask = 0x00ff0000;
	    gmask = 0x0000ff00;
	    bmask = 0x000000ff;
	    amask = 0xff000000;
		
		SDL_BlitSurface(menuBackgroundSurface,NULL,screenBuffer ,NULL);
		SDL_Surface* menu;
		menu =  SDL_CreateRGBSurface(0, 1720, 80, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
		copySurfaceToBackRenderer(menu, screenBuffer, 100, 450);
		
		menu =  SDL_CreateRGBSurface(0, 1720, 450, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
		copySurfaceToBackRenderer(menu, screenBuffer, 100, 550);
		
		SDL_Color green = {0, 255, 0};
		SDL_Color blue = {0, 140, 255};
		
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "SELECT COSTUME", green);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer, ((1920/2)-(surfaceMessage->w/2)), 462);
	
		//add player sprite
		for(int i=0 ; i<8 ; i++){
			copySurfaceToBackRenderer(menuPlayerSprite[i], screenBuffer, 162+(i*215), 520);	
		}
		
		for(int i=0;i<16;i++){
			if(previousPlayerKeystate[i] & keyPadRight && keychange[i]){
				playerType[i][1] = playerType[i][1] +1;
				if(playerType[i][1]>7){
					playerType[i][1] = 0;
				}
			}
			if(previousPlayerKeystate[i] & keyPadLeft && keychange[i]){
				playerType[i][1] = playerType[i][1] -1;
				if(playerType[i][1] < 0){
					playerType[i][1] = 7;	
				}
			}
		}
	
		int list[8] = {0,0,0,0,0,0,0,0};
		for(int i=0 ; i<16 ; i++){
			if(playerType[i][0] != OFF){
				int index = playerType[i][1];
				char playerName[2];
				SDL_Color playerColor = blue;
				switch(playerType[i][0]){
					case HUMAN : 
						sprintf(playerName, "HUMAN %i", i+1);
						playerColor = blue;
						break;
					case CPU : 
						sprintf(playerName, "CPU %i", i+1);
						playerColor = green;
						break;
					case OFF : 
						break;
				}
				SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, playerName, playerColor);
				copySurfaceToBackRenderer(surfaceMessage, screenBuffer, 200+(playerType[i][1]*216) -(surfaceMessage->w/2), 650+(45*list[index]));
				list[index] = list[index] + 1;
			}
		}
		refreshBuffer = false;	
		copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
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
	if(refreshBuffer || keychange[0]){
		Uint32 rmask, gmask, bmask, amask;
	    rmask = 0x00ff0000;
	    gmask = 0x0000ff00;
	    bmask = 0x000000ff;
	    amask = 0xff000000;
	
		if(previousPlayerKeystate[0] & keyPadRight && keychange[0]){
			
			switch(cursorPosition){
				case suddenDeathOption:
				case badBomberOption:
					if(gameOption[cursorPosition] == 0){
						gameOption[cursorPosition] = 1;
					}else{
						gameOption[cursorPosition] = 0;
					}
					break;
				case cpuLevelOption:
					gameOption[cursorPosition] = gameOption[cursorPosition] +1;
					if(gameOption[cursorPosition] < 1){
						gameOption[cursorPosition] = 3;
					}else if(gameOption[cursorPosition] >3){
						gameOption[cursorPosition] = 1;
					}
					break;
			}
		}
		if(previousPlayerKeystate[0] & keyPadLeft && keychange[0]){
			switch(cursorPosition){
				case suddenDeathOption:
				case badBomberOption:
					if(gameOption[cursorPosition] == 0){
						gameOption[cursorPosition] = 1;
					}else{
						gameOption[cursorPosition] = 0;
					}
					break;
				case cpuLevelOption:
					gameOption[cursorPosition] = gameOption[cursorPosition] -1;
					if(gameOption[cursorPosition] < 1){
						gameOption[cursorPosition] = 3;
					}else if(gameOption[cursorPosition] >3){
						gameOption[cursorPosition] = 1;
					}
					break;
			}
		}
		if(previousPlayerKeystate[0] & keyPadDown && keychange[0]){
			cursorPosition++;
			if(cursorPosition > 2){
				cursorPosition = 0;	
			}
		}
		if(previousPlayerKeystate[0] & keyPadUp && keychange[0]){
			cursorPosition--;
			if(cursorPosition < 0){
				cursorPosition = 2;
			}
		}
		
		SDL_BlitSurface(menuBackgroundSurface,NULL,screenBuffer ,NULL);
		SDL_Surface* menu;
		menu =  SDL_CreateRGBSurface(0, 1720, 80, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
		copySurfaceToBackRenderer(menu, screenBuffer, 100, 450);
		
		menu =  SDL_CreateRGBSurface(0, 1720, 300, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
		copySurfaceToBackRenderer(menu, screenBuffer, 100, 550);
		
		SDL_Color red = {255, 0, 0};
		SDL_Color green = {0, 255, 0};
		SDL_Color blue = {0, 140, 255};
		
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "GAME OPTION", green);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer, ((1920/2)-(surfaceMessage->w/2)), 462);
		
		surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "Sudden Death", green);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer, 600, 562);
		surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "Bad Bomber", green);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer, 600, 622);
		surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "CPU Level", green);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer, 600, 682);
		
		surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, gameOption[0] == 1 ? "ON" : "OFF", gameOption[0] == 1 ? green : red);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer, 1200, 562);
		surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, gameOption[1] == 1 ? "ON" : "OFF", gameOption[1] == 1 ? green : red);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer, 1200, 622);
		
		char CPULevel[2];
		sprintf(CPULevel, "%i", gameOption[2]);
		surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, CPULevel, blue);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer, 1200, 682);
		refreshBuffer = false;
	}
	int cursorPosX = 550;
	int cursorposY = 562 + ( cursorPosition * 60 );	
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
	copySurfaceToBackRenderer(cursor.getCurrentFrame(), vout_buf, cursorPosX, cursorposY);	
}









/*
* Menu for level selection
* 
* 
* 
* 
*/
void Bomberman::drawLevelSelectionMenu(unsigned short * in_keystate, SDL_Surface * vout_buf){
	if(refreshBuffer || keychange[0]){
		Uint32 rmask, gmask, bmask, amask;
	    rmask = 0x00ff0000;
	    gmask = 0x0000ff00;
	    bmask = 0x000000ff;
	    amask = 0xff000000;
		SDL_BlitSurface(menuBackgroundSurface,NULL,screenBuffer ,NULL);
		SDL_Surface* menu;
		menu =  SDL_CreateRGBSurface(0, 1720, 80, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
		copySurfaceToBackRenderer(menu, screenBuffer, 100, 450);
		
		menu =  SDL_CreateRGBSurface(0, 1720, 450, 32, rmask, gmask, bmask, amask);
		SDL_FillRect(menu, NULL, SDL_MapRGBA(menu->format, 0, 0, 0, 120));
		copySurfaceToBackRenderer(menu, screenBuffer, 100, 550);
		
		SDL_Color red = {255, 0, 0};
		SDL_Color green = {0, 255, 0};
		//SDL_Color blue = {0, 140, 255};
		
		//menuLevelSprite
		
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fragileBombersFont, "SELECT THE LEVEL", green);
		copySurfaceToBackRenderer(surfaceMessage, screenBuffer, ((1920/2)-(surfaceMessage->w/2)), 462);
		
		
		
		
		if(previousPlayerKeystate[0] & keyPadRight && keychange[0]){
			cursorPosition++;
			if(cursorPosition>8){
				cursorPosition = 0;	
			}
			
		}
		if(previousPlayerKeystate[0] & keyPadLeft && keychange[0]){
			cursorPosition--;
			if(cursorPosition<0){
				cursorPosition = 8;	
			}
		}
		
		copySurfaceToBackRenderer(menuLevelSprite[cursorPosition], screenBuffer, ((1920/2)-(menuLevelSprite[cursorPosition]->w/2)), 600);
		
		refreshBuffer = false;
		levelIndex = cursorPosition;
	}
	copySurfaceToBackRenderer(screenBuffer, vout_buf, 0, 0);
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
