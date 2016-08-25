#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "../include/image_ressource.h"
#include "../include/level.h"


class Grid
{

public:
    Grid();
	Grid(int x, int y, int levelIndex);
    ~Grid();
    
    //configure the grid and build it
    void configure(int x, int y, int levelNumber);
    
    //get Image to render in retroarch
    SDL_Surface * getGrid();
    
private:
    
    /*Texture for the level
     Load from AllLevel.png file
     each level is a block of 432 x 384 px
     Texture is on the left in 270 x 384 px block
     Sky texture is on the right block size 162 * 288 px
     Each texture size is 54 x 48 px
     */
    SDL_Surface **textures;
	SDL_Surface **skys;
    
    //Final image to renderer
    SDL_Surface *theGrid;
    
    //Layers to draw the game
    SDL_Surface *ground;
    SDL_Surface *brickShadow;
    SDL_Surface *playersBombes;
    SDL_Surface *skyFixe;
    SDL_Surface *skyMove;
    
    //table of free/occuped part of the grid
    int * tab;
    
    //size of the grid
    int sizeX;
    int sizeY;
    
    //level index to load the texture and level template
    int lvl;
    
    
    
    
    void init();
    void generateGrid();
	
};
