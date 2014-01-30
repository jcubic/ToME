/* File: net.c */

/* Purpose: Network layer based on SDL_net */

/*
 * Copyright (c) 2004 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include "SDL.h"
#include "SDL_net.h"

void init_net_subsystem(void)
{
	/* Initialize SDL */
	if(SDL_Init(0)==-1)
	{
		quit(format("SDL_Init: %s\n", SDL_GetError()));
	}

	// initialize SDL_net
	if(SDLNet_Init()==-1)
	{
		quit(format("SDLNet_Init: %s\n", SDL_GetError()));
	}
}
