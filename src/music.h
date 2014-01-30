#ifndef INCLUDED_MUSIC_H
#define INCLUDED_MUSIC_H

#define USE_RWOPS
#include "SDL_mixer.h"


/* music.c */
extern void init_music_subsystem(void);
extern Mix_Music *music_load_music(cptr file);
extern bool music_play_music(Mix_Music *music, s32b loop, s32b fade_in_time);
extern bool sound_not_available;

extern Mix_Chunk *music_load_sample(cptr file);
extern bool music_play_sample(Mix_Chunk *sample, s32b channel, s32b loop, s32b fade_in_time);

#endif
