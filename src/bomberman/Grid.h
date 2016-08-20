#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "../include/image_ressource.h"

class Grid
{

public:
    Grid();
	Grid(int x, int y, int levelIndex);
    ~Grid();
    void setSize(int x, int y);
    SDL_Surface * getGrid();
    
private:
	SDL_Surface *theGrid;
    SDL_Surface *wall;
    SDL_Surface *brick;
    SDL_Surface *ground;
    int * tab;
    int sizeX;
    int sizeY;
    int lvl;
    
    void init();
    void generateGrid();
	
};
