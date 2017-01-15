#ifndef IS_OSX
#include <SDL2/SDL_mixer.h>
#else
#include <SDL2_mixer/SDL_mixer.h>
#endif

#include <stdio.h>

#ifndef __MYCLASS_LevelVariante
#define __MYCLASS_LevelVariante

class LevelVariante {
	public:
		LevelVariante(int number, const char * description, bool fillWithBrick, int definition[735], int bonus[13]);
		~LevelVariante();

		char getDefinition(int index);
		int getBonus(int index);
		bool isFillWithBricks();
		int getNumber();
		char * getDescription();
	private:
		int number;
		char description[256];
		bool fillWithBrick;
		int definition[735];
		int bonus[13];
};
#endif
