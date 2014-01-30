/* File: music.c */

/* Purpose: Music layer based on SDL_mixer */

/*
 * Copyright (c) 2004 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include "SDL.h"
#include "music.h"
#include "sdl-defines.h"

bool sound_not_available = TRUE;

static void channel_finished(int channel)
{
	Uint32 sound_channel = channel;
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = EVENT_GAME_SOUND_CHANNEL_STOP;
	userevent.data1 = (void*)sound_channel;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
}

static void music_finished()
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = EVENT_GAME_MUSIC_STOP;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
}

void init_music_subsystem(void)
{
	// set this to any of 512,1024,2048,4096 the higher it is, the more FPS shown and CPU needed
	s32b audio_buffers = 512;

	/* Should we do sounds ? */
	esettings_get_int("audio.music.enable", 1);
	esettings_get_int("audio.effects.enable", 1);
	if (!esettings_get_int("audio.enable", 1))
	{
		sound_not_available = TRUE;
		return;
	}

	// initialize SDL_net
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, audio_buffers) == -1)
	{
		sound_not_available = TRUE;
		return;
	}

	Mix_VolumeMusic(SDL_MIX_MAXVOLUME);
	Mix_Volume(-1, SDL_MIX_MAXVOLUME);

#ifdef USE_SDL
	if (!strcmp(ANGBAND_SYS, "sdl"))
	{
		Mix_HookMusicFinished(music_finished);
		Mix_ChannelFinished(channel_finished);
	}
#endif

	sound_not_available = FALSE;
}

Mix_Music *music_load_music(cptr file)
{
	Mix_Music *music;
	SDL_RWops *rops;

	if (!esettings_get_int("audio.music.enable", 1)) return NULL;

	rops = PHYSFSRWOPS_openRead(file);
	if (!rops)
	{
		return NULL;
	}
	music = Mix_LoadMUS_RW(rops);
	return music;
}

bool music_play_music(Mix_Music *music, s32b loop, s32b fade_in_time)
{
	if (!music || !loop) return FALSE;
	return (Mix_FadeInMusic(music, loop, fade_in_time) == -1) ? FALSE : TRUE;
}


Mix_Chunk *music_load_sample(cptr file)
{
	Mix_Chunk *sample;
	SDL_RWops *rops;

	if (!esettings_get_int("audio.effects.enable", 1)) return NULL;

	rops = PHYSFSRWOPS_openRead(file);
	if (!rops)
	{
		return NULL;
	}
	sample = Mix_LoadWAV_RW(rops, TRUE);
	if (sample) Mix_VolumeChunk(sample, SDL_MIX_MAXVOLUME);
	return sample;
}

bool music_play_sample(Mix_Chunk *sample, s32b channel, s32b loop, s32b fade_in_time)
{
	if (!sample) return FALSE;
//	return (Mix_FadeInChannel(channel, sample, loop, fade_in_time) == -1) ? FALSE : TRUE;
	return (Mix_PlayChannel(channel, sample, loop) == -1) ? FALSE : TRUE;
}
