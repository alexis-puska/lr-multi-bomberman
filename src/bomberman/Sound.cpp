#include "Sound.h"

//musique
const static char *musiquePath = "./resources/musique/menu.mp3";
const static char *battlePath = "./resources/musique/battle.mp3";
//sound
const static char *valideSoundPath = "./resources/sound/valide.wav";
const static char *cancelSoundPath = "./resources/sound/cancel.wav";
const static char *bipSoundPath = "./resources/sound/bip.wav";
const static char *fireSoundPath = "./resources/sound/fire.wav";
const static char *LouisSoundPath = "./resources/sound/louis.wav";
const static char *bounceSoundPath = "./resources/sound/bounce.wav";
const static char *playerBurnSoundPath = "./resources/sound/burn.wav";
const static char *playerKickSoundPath = "./resources/sound/kick.wav";
const static char *endSoundPath = "./resources/sound/end.wav";

Sound Sound::m_instance = Sound();

Sound::Sound() {
	fprintf(stderr, "Init sound system\n");
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
		printf("%s", Mix_GetError());
	}
	menu = Mix_LoadMUS(musiquePath);
	battle = Mix_LoadMUS(battlePath);

	validSound = Mix_LoadWAV(valideSoundPath);
	cancelSound = Mix_LoadWAV(cancelSoundPath);
	bipSound = Mix_LoadWAV(bipSoundPath);
	fireSound = Mix_LoadWAV(fireSoundPath);
	louisSound = Mix_LoadWAV(LouisSoundPath);
	playerBurnSound = Mix_LoadWAV(playerBurnSoundPath);
	playerKickSound = Mix_LoadWAV(playerKickSoundPath);
	bombeBounceSound = Mix_LoadWAV(bounceSoundPath);
	endSound = Mix_LoadWAV(endSoundPath);

	Mix_PlayMusic(menu, -1);
	Mix_VolumeMusic (MIX_MAX_VOLUME);
	Mix_AllocateChannels(9);
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
	Mix_CloseAudio();
}

Sound& Sound::Instance() {
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
