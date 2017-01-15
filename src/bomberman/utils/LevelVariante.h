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
		LevelVariante(int number, const char * descriptionLine1, const char * descriptionLine2, bool fillWithBrick, int definition[735], int bonus[13]);
		~LevelVariante();

		char getDefinition(int index);
		int getBonus(int index);
		bool isFillWithBricks();
		int getNumber();
		char * getDescriptionLine1();
		char * getDescriptionLine2();
		bool isReserved(int idx);
		int getStart(int idx);
	private:
		int number;
		char descriptionLine1[32];
		char descriptionLine2[32];
		bool fillWithBrick;
		int definition[735];
		bool reserved[735];
		int start[16];
		int bonus[13];
};
#endif
