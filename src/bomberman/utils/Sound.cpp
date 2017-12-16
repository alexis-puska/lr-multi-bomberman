#include "Sound.h"

//musique
#include "resources/music_menu.h"
#include "resources/music_battle.h"
//sound
#include "resources/sound_bip.h"
#include "resources/sound_bounce.h"
#include "resources/sound_burn.h"
#include "resources/sound_cancel.h"
#include "resources/sound_end.h"
#include "resources/sound_fire.h"
#include "resources/sound_hole1.h"
#include "resources/sound_hole2.h"
#include "resources/sound_hole3.h"
#include "resources/sound_kick.h"
#include "resources/sound_louis.h"
#include "resources/sound_mine.h"
#include "resources/sound_teleporter_close.h"
#include "resources/sound_teleporter_open.h"
#include "resources/sound_valide.h"



Sound::Sound() {
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
		printf("%s", Mix_GetError());
	}
	for(int i = 0; i < nbChannelSound; i++){
		activeChannel[i] = false;
	}
	menu = Mix_LoadMUSType_RW(SDL_RWFromMem(music_menu_mp3, music_menu_mp3_len), MUS_MP3, 0);
	battle = Mix_LoadMUSType_RW(SDL_RWFromMem(music_battle_mp3, music_battle_mp3_len), MUS_MP3, 0);

	bipSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_bip_wav, sound_bip_wav_len), 0);
	bombeBounceSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_bounce_wav, sound_bounce_wav_len), 0);
	playerBurnSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_burn_wav, sound_burn_wav_len), 0);
	cancelSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_cancel_wav, sound_cancel_wav_len), 0);
	endSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_end_wav, sound_end_wav_len), 0);
	fireSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_fire_wav, sound_fire_wav_len), 0);
	hole1Sound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_hole1_wav, sound_hole1_wav_len), 0);
	hole2Sound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_hole2_wav, sound_hole2_wav_len), 0);
	hole3Sound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_hole3_wav, sound_hole3_wav_len), 0);
	playerKickSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_kick_wav, sound_kick_wav_len), 0);
	louisSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_louis_wav, sound_louis_wav_len), 0);
	mineSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_mine_wav, sound_mine_wav_len), 0);
	teleporterCloseSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_teleporter_close_wav, sound_teleporter_close_wav_len), 0);
	teleporterOpenSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_teleporter_open_wav, sound_teleporter_open_wav_len), 0);
	validSound = Mix_LoadWAV_RW(SDL_RWFromMem(sound_valide_wav, sound_valide_wav_len), 0);

	Mix_PlayMusic(menu, -1);
	Mix_VolumeMusic (MIX_MAX_VOLUME);
	Mix_AllocateChannels(nbChannelSound);
	Mix_Volume(0, MIX_MAX_VOLUME / 2);
	Mix_Volume(3, MIX_MAX_VOLUME / 2);

	mineOffsetChannel = 14;
}

Sound::~Sound() {
	fprintf(stderr, "close sound system\n");
	Mix_CloseAudio();
	Mix_FreeMusic(menu);
	Mix_FreeMusic(battle);
	Mix_FreeChunk(validSound);
	Mix_FreeChunk(cancelSound);
	Mix_FreeChunk(bipSound);
	Mix_FreeChunk(fireSound);
	Mix_FreeChunk(louisSound);
	Mix_FreeChunk(bombeBounceSound);
	Mix_FreeChunk(playerBurnSound);
	Mix_FreeChunk(playerKickSound);
	Mix_FreeChunk(endSound);
	Mix_FreeChunk(hole1Sound);
	Mix_FreeChunk(hole2Sound);
	Mix_FreeChunk(hole3Sound);
	Mix_FreeChunk(teleporterOpenSound);
	Mix_FreeChunk(teleporterCloseSound);
	Mix_FreeChunk(mineSound);
	Mix_CloseAudio();
}

Sound& Sound::Instance() {
    static Sound m_instance;
    return m_instance;
}


void Sound::startBattleMusique() {
	Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
	Mix_PlayMusic(battle, -1);
}

void Sound::startMenuMusique() {
	Mix_VolumeMusic (MIX_MAX_VOLUME);
	Mix_PlayMusic(menu, -1);
}

void Sound::stopMusique() {
	Mix_HaltMusic();
}

void Sound::playValidSound() {
	Mix_PlayChannel(0, validSound, 0);
}

void Sound::playCancelSound() {
	Mix_PlayChannel(1, cancelSound, 0);
}

void Sound::playBipSound() {
	Mix_PlayChannel(2, bipSound, 0);
}

void Sound::playFireSound() {
	Mix_PlayChannel(3, fireSound, 0);
}

void Sound::playLouisSound() {
	Mix_PlayChannel(4, louisSound, 0);
}

void Sound::playPlayerBurnSound() {
	Mix_PlayChannel(5, playerBurnSound, 0);
}

void Sound::playPlayerKickSound() {
	Mix_PlayChannel(6, playerKickSound, 0);
}

void Sound::playBombeBounceSound() {
	Mix_PlayChannel(7, bombeBounceSound, 0);
}

void Sound::playEndSound() {
	Mix_PlayChannel(8, endSound, 0);
}

void Sound::playHole1Sound() {
	Mix_PlayChannel(9, hole1Sound, 0);
}

void Sound::playHole2Sound() {
	Mix_PlayChannel(10, hole2Sound, 0);
}

void Sound::playHole3Sound() {
	Mix_PlayChannel(11, hole3Sound, 0);
}

void Sound::playTeleporterCloseSound() {
	Mix_PlayChannel(12, teleporterCloseSound, 0);
}

void Sound::playTeleporterOpenSound() {
	Mix_PlayChannel(13, teleporterOpenSound, 0);
}

int Sound::getNextMineOffsetChannel(){
	mineOffsetChannel++;
	if(mineOffsetChannel >= 20){
		mineOffsetChannel = 14;
	}
	return mineOffsetChannel - 1;
}

void Sound::startMineSound(int channel) {
	activeChannel[channel] = true;
	Mix_PlayChannel(channel, mineSound, -1);
}

void Sound::stopMineSound(int channel) {
	activeChannel[channel] = false;
	Mix_HaltChannel(channel);
}

void Sound::stopAllMineChannel() {
	for(int i = 14;i < nbChannelSound; i++){
		if(activeChannel[i]){
			Mix_HaltChannel(i);
		}
	}
}