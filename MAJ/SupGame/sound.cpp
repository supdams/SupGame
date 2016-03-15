#include "sound.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
 
#include "SDL.h"
#include "SDL_mixer.h"
 

 
sound::sound(){}
sound::~sound(){}
 
void sound::playWAV(const char *wav){
    Mix_Chunk *music;
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    music = Mix_LoadWAV(wav);
    Mix_PlayChannel(1,music,0);
}
 
void sound::playMP3(const char *mp3){
    Mix_Music *music;
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,1024);
    music = Mix_LoadMUS(mp3);
    Mix_VolumeMusic(100);
    Mix_PlayMusic(music,-1);
 
}








	
	/*
// Set Volume
Mix_VolumeMusic(100);


if (music == 0)
cout << "Mix_LoadMuS ERROR: " << Mix_GetError() << endl;
else
cout << "Mix_LoadMus SUCCESS" << endl;

// Start Playback
if (Mix_PlayMusic(music, 1) != 0)
cout << "Mix_PlayMusic ERROR: " << Mix_GetError() << endl;
else
cout << "Mix_PlayMusic SUCCESS" << endl;
int startTime = SDL_GetTicks();


// Wait
while (Mix_PlayingMusic()) {
SDL_Delay(100000);
cout << "Time: " << (SDL_GetTicks() - startTime) / 100000 << endl;
break;
} 
*/

					//fin