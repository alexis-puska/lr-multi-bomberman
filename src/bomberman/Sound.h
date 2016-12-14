#ifndef IS_OSX
#include <SDL2/SDL_mixer.h>
#else
#include <SDL2_mixer/SDL_mixer.h>
#endif

#include <stdio.h>

#ifndef __MYCLASS_Sound
#define __MYCLASS_Sound

class Sound {
	public:
		static Sound& Instance();
				Sound();
		~Sound();
		
		void startBattleMusique();
		void startMenuMusique();
		void StopMusique();
		void playValidSound();		
		void playCancelSound();
		void playBipSound();
		void playFireSound();
		void playLouisSound();
		void playPlayerBurnSound();
		void playPlayerKickSound();
		void playBombeBounceSound();
		void playEndSound();
		
		
	private:
		Sound& operator= (const Sound&){}
		Sound (const Sound&){}
		static Sound m_instance;
		
		//musique
		Mix_Music *menu;
		Mix_Music *battle;
		//sound menu
		Mix_Chunk *validSound;
		Mix_Chunk *cancelSound;
		Mix_Chunk *bipSound;
		//sound battle
		Mix_Chunk *fireSound;
		Mix_Chunk *louisSound;
		Mix_Chunk *playerBurnSound;
		Mix_Chunk *playerKickSound;
		Mix_Chunk *bombeBounceSound;
		Mix_Chunk *endSound;
};
#endif
