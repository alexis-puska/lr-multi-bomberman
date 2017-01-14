#ifndef IS_OSX
#include <SDL2/SDL_mixer.h>
#else
#include <SDL2_mixer/SDL_mixer.h>
#endif

#include <stdio.h>

#ifndef __MYCLASS_LevelService
#define __MYCLASS_LevelService

class LevelService {
	public:
		static LevelService& Instance();
		LevelService();
		~LevelService();

	private:
		LevelService& operator=(const LevelService&);
		LevelService(const LevelService&);
		static LevelService m_instance;

};
#endif
