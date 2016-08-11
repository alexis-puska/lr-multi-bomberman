#include <cstdlib>
#include <iostream>


#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>




#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>

#else
#ifdef _WIN32
	#include <windows.h>
#endif
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

#include "../include/libretro_common.h"
#include "../include/pad.h"

class Bomberman {
private:
public:
    void init();
    void getScreen();
};

void Bomberman::getScreen(){

 glPushMatrix(); //Start phase
    
    glOrtho(0,720,480,0,-1,1); //Set the matrix
    
    char *image_path = "./resource/test.png";

    
    SDL_Surface *image = IMG_Load( image_path );
    
    float x = 450.0f;
    float y = 150.0f;
    float width = 1081.0f;
    float height = 486.0f;
    
    float iheight = 1081.0f;
    float iwidth = 486.0f;
    
    
    glEnable(GL_TEXTURE_2D);
    
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint textures;
    glGenTextures(1, &textures); //Number of textures stored in array name specified
    
    glBindTexture(GL_TEXTURE_2D, textures);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
//    // Map the surface to the texture in video memory
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_INT_8_8_8_8, image->pixels); //GL_BITMAP
    SDL_FreeSurface ( image );
    
    
    glBindTexture(GL_TEXTURE_2D, textures);
    
    //Render texture quad
    glBegin( GL_QUADS );
    glTexCoord2f( 0.f, 0.f ); glVertex2f(x, y); //Bottom left
    glTexCoord2f( 1.f, 0.f ); glVertex2f(x + iwidth, y); //Bottom right
    glTexCoord2f( 1.f, 1.f ); glVertex2f(x + iwidth, y + iheight); //Top right
    glTexCoord2f( 0.f, 1.f ); glVertex2f(x, y + iheight); //Top left
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    
    glPopMatrix(); //End rendering phase

    
}

void Bomberman::init(){
    
}

std::ios_base::Init::Init(){}

std::ios_base::Init::~Init(){}

