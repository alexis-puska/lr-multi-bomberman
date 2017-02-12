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
const static char *hole1SoundPath = "./resources/sound/hole1.wav";
const static char *hole2SoundPath = "./resources/sound/hole2.wav";
const static char *hole3SoundPath = "./resources/sound/hole3.wav";
const static char *teleporterOpenSoundPath = "./resources/sound/teleporter_open.wav";
const static char *teleporterCloseSoundPath = "./resources/sound/teleporter_close.wav";
const static char *mineSoundPath = "./resources/sound/mine.wav";

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
	hole1Sound = Mix_LoadWAV(hole1SoundPath);
	hole2Sound = Mix_LoadWAV(hole2SoundPath);
	hole3Sound = Mix_LoadWAV(hole3SoundPath);
	teleporterOpenSound = Mix_LoadWAV(teleporterOpenSoundPath);
	teleporterCloseSound = Mix_LoadWAV(teleporterCloseSoundPath);
	mineSound = Mix_LoadWAV(mineSoundPath);

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

	Mix_PlayChannel(channel, mineSound, -1);
}

void Sound::stopMineSound(int channel) {
	Mix_HaltChannel(channel);
}