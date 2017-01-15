#ifndef IS_OSX
#include <SDL2/SDL_mixer.h>
#else
#include <SDL2_mixer/SDL_mixer.h>
#endif

#include <stdio.h>
#include <string.h>
#include <vector>
#include "LevelVariante.h"

#ifndef __MYCLASS_Level
#define __MYCLASS_Level

class Level {
	public:
		Level(int numero, const char* name);
		~Level();
		void addVariante(LevelVariante * variante);
		int getVariantesSize();
		LevelVariante * getVariantes(int i);

		char * getName();
	private:
		int numero;
		char name[20];
		std::vector <LevelVariante *> variantes;
};
#endif
