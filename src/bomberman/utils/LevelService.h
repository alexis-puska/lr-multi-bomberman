#ifndef IS_OSX
#include <SDL2/SDL_mixer.h>
#else
#include <SDL2_mixer/SDL_mixer.h>
#endif

#include <stdio.h>
#include <vector>
#include "Level.h"
#include "LevelVariante.h"
#include "RailSwitch.h"

#ifndef __MYCLASS_LevelService
#define __MYCLASS_LevelService

class LevelService {
	public:
		static LevelService& Instance();
		LevelService();
		~LevelService();

		Level * getLevel(int n);
		int getNumberOfLevels();
		void setCustomBonusValue(int bonus, int value);
		int getCustomBonusValue(int bonus);

	private:
		LevelService& operator=(const LevelService&);
		LevelService(const LevelService&);
		static LevelService m_instance;
		std::vector<Level *> levels;
		void initLevel();
		void createLevel0();
		void createLevel1();
		void createLevel2();
		void createLevel3();
		void createLevel4();
		void createLevel5();
		void createLevel6();
		void createLevel7();
		void createLevel8();
};
#endif
