/* Copyright (C) 2006 Eric Stevens <sdltome@gmail.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* THE AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
* AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name(s) of the author(s) shall not be
* used in advertising or otherwise to promote the sale, use or other dealings
* in this Software without prior written authorization from the author(s).
*
*******************************************************************************
*/
#ifdef USE_SDL

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SDL.h"
#include "sdl-types.h"
#include "sdl-defines.h"
#include "sdl-extern.h"

#include "angband.h"

/*
 * Prototypes
 */

s32b sdl_event_handler(SDL_Event* event);

/*
 * Defines
 */

#define VP_STATIC			0x00
#define VP_RESIZE			0x01
#define VP_TRANSLATE		0x02

/*
 * Baseline refresh rate matched to a P3-550 w/512M and a Matrox G200 on SuSE 9.2 w/SDL under X11
 * Decrease this to reduce any choppy movement on faster systems.
 */
#define SDL_EVENT_OPAQUE_REFRESH	40
#define SDL_EVENT_REFRESH_RATE		SDL_EVENT_OPAQUE_REFRESH

#define IN_BOUNDS(pos_x, pos_y, rect)	\
		( (pos_x >= rect.x) && (pos_x <= (rect.x + rect.w)) \
	  && (pos_y >= rect.y) && (pos_y <= (rect.y + rect.h)) )

bool is_overlapping(SDL_Rect r1, SDL_Rect r2)
{
	dim_t r1_half = { r1.w / 2, r1.h / 2 };
	dim_t r2_half = { r2.w / 2, r2.h / 2 };
	pos_t r1_center = { r1.x + r1_half.w, r1.y + r1_half.h };
	pos_t r2_center = { r2.x + r2_half.w, r2.y + r2_half.h };

	if ( ((r1_half.w + r2_half.w) > abs(r1_center.x - r2_center.x))
		&& ((r1_half.h + r2_half.h) > abs(r1_center.y - r2_center.y)) )
	{
		return TRUE;
	}

	return FALSE;
}

void show_coords(s32b row, s32b col, s32b x, s32b y)
{
#ifdef SDL_DEBUG
	char varbuf[16];

	sprintf(varbuf, "x:%d", x);

	if (do_redraw)
	{
		sdl_putrawstr(&data[0],
							data[0].vp.fontdim.w * row,
							data[0].vp.fontdim.h * col,
							strlen(varbuf),
							varbuf);

		sprintf(varbuf, "y:%d", y);
		sdl_putrawstr(&data[0],
							data[0].vp.fontdim.w * (row + poslen + 2),
							data[0].vp.fontdim.h * col,
							strlen(varbuf),
							varbuf);
	}
#endif
}

void show_key_code(s32b row, s32b col, void* code, byte type)
{
	char varbuf[64];
	char codestr[64];
	s32b* key_code;

	sdl_strlcpy(codestr, code, strlen(code));

	if (type)
	{
		snprintf(varbuf, 64, "key code: %s             ", codestr);
	}
	else
	{
		key_code = code;
		snprintf(varbuf, 64, "key code: %i (0x%.2x) ('%c')             ", (unsigned int)*key_code, (unsigned int)*key_code, (char)*key_code);
	}

	sdl_putrawstr(&data[0],
						data[0].vp.fontdim.w * row,
						data[0].vp.fontdim.h * col,
						strlen(varbuf),
						varbuf);

	//printf("%s\n", varbuf);
}

s32b handle_mouse_btn1(SDL_Event* first_event)
{
	SDL_Event*		event = first_event;
	s32b				i = 0;
	s32b				z = 0;
	s32b				vp_op = VP_STATIC;
	bool			done = FALSE;
	term_data*		mod_vp_ptr = NULL;
	pos_t			mouse_pos = { 0, 0 };
	term*			current_term;
	SDL_Rect		delta = { 0, 0, 0, 0 };
	SDL_Rect		vp_rect = { 0, 0, 0, 0 };
	SDL_Rect		orig_rect;
	SDL_Rect		handle_rect;
	SDL_Rect		last_rect;

	while (!done)
	{
		switch (event->type)
		{
			case SDL_MOUSEBUTTONDOWN:
				show_coords(1, 0, event->button.x, event->button.y);

				i = 0;
				while (i < max_terms && vp_op == VP_STATIC)
				{
					z = vp_z_order[i];

					/* note the position of the mouse when we clicked on the viewport */
					mouse_pos.x = event->button.x;
					mouse_pos.y = event->button.y;

					handle_rect.x = data[z].vp.rect.x + data[z].vp.area.handle.x;
					handle_rect.y = data[z].vp.rect.y + data[z].vp.area.handle.y;
					handle_rect.w = data[z].vp.area.handle.w;
					handle_rect.h = data[z].vp.area.handle.h;

					/* we are moving the viewport */
					if (  data[z].vp.visible
										 && IN_BOUNDS(event->button.x, event->button.y, data[z].vp.rect) )
					{
						vp_op = VP_TRANSLATE;
						mod_vp_ptr = &data[z];
						sdl_utl_rect_cp(&orig_rect, &mod_vp_ptr->vp.rect);
						sdl_utl_rect_cp(&last_rect, &mod_vp_ptr->vp.rect);

						sdl_utl_reorder_z_pos(mod_vp_ptr);

						/* bring it to the top */
						REDRAW_VP(z, ENTIRE_SURFACE, &mod_vp_ptr->vp.rect);
						UPDATE_RECT(screen.surface, mod_vp_ptr->vp.rect);

						SET_CFLAG(IN_TERM_DRAW_OP);
					}

					/* we are resizing the viewport */
					if (  data[z].vp.visible
										 && IN_BOUNDS(event->button.x, event->button.y, handle_rect) )
					{
						vp_op = VP_RESIZE;
						mod_vp_ptr = &data[z];
						sdl_utl_rect_cp(&orig_rect, &mod_vp_ptr->vp.rect);
						sdl_utl_rect_cp(&last_rect, &mod_vp_ptr->vp.rect);

						sdl_utl_reorder_z_pos(mod_vp_ptr);

						SET_CFLAG(IN_TERM_DRAW_OP);
					}

					i++;
				}

				/* if none of our criteria was met, leave */
				if (vp_op == VP_STATIC)
				{
					done = TRUE;
				}
				break;

			case SDL_MOUSEMOTION:
				show_coords(1, 1, event->button.x, event->button.y);

				if (mod_vp_ptr && vp_op != VP_STATIC)
				{
					switch (vp_op)
					{
						case VP_TRANSLATE:

							if (tempsurf == NULL)
							{
								// tempsurf freed on SDL_MOUSEBUTTONUP event
								tempsurf = sdl_utl_create_surface(0,
										screen.surface->w,
										screen.surface->h,
										BPP_AUTO,
										mod_vp_ptr->vp.rendmode);

								if (do_redraw)
								{
									/* remove the viewport from the screen */
									sdl_utl_clr_vp_from_screen(mod_vp_ptr);

									/* blit the screen to the temp surface */
									SDL_BlitSurface(screen.surface, ENTIRE_SURFACE, tempsurf, ENTIRE_SURFACE);
								}
							}

							if (do_redraw)
							{
								do_redraw = 0;

								delta.x = event->button.x - mouse_pos.x;
								delta.y = event->button.y - mouse_pos.y;

								/* contain the viewports to the screen */
								if (  mod_vp_ptr->vp.rect.x + delta.x + mod_vp_ptr->vp.rect.w < screen.surface->w
															  && mod_vp_ptr->vp.rect.x + delta.x >= 0
															  && mod_vp_ptr->vp.rect.y + delta.y + mod_vp_ptr->vp.rect.h < screen.surface->h
															  && mod_vp_ptr->vp.rect.y + delta.y >= 0 )
								{
									vp_rect.x = mod_vp_ptr->vp.rect.x + delta.x;
									vp_rect.y = mod_vp_ptr->vp.rect.y + delta.y;
									vp_rect.w = mod_vp_ptr->vp.rect.w;
									vp_rect.h = mod_vp_ptr->vp.rect.h;
								}

								/* reposition the viewport */
								mod_vp_ptr->vp.rect.x = vp_rect.x;
								mod_vp_ptr->vp.rect.y = vp_rect.y;

								/* update the relative mouse position */
								mouse_pos.x = event->button.x;
								mouse_pos.y = event->button.y;

								/*
								* move the viewport
								*/

								/* redraw the background */
								SDL_BlitSurface(tempsurf, &last_rect, screen.surface, &last_rect);

								/* refresh */
								//UPDATE_RECT(screen.surface, last_rect);

								/* draw the viewport in its new position */
								SDL_BlitSurface(mod_vp_ptr->vp.surface, ENTIRE_SURFACE, screen.surface, &mod_vp_ptr->vp.rect);

								/* refresh */
								UPDATE_RECT(screen.surface, last_rect);

								/* save a copy of the repositioned viewport rect metrics */
								sdl_utl_rect_cp(&last_rect, &mod_vp_ptr->vp.rect);

								mod_vp_ptr->vp.refresh = TRUE;
							}
							break;

						case VP_RESIZE:
							delta.x = event->button.x - mouse_pos.x;
							delta.y = event->button.y - mouse_pos.y;

							delta.w = (s32b)(delta.x / mod_vp_ptr->vp.fontdim.w);
							delta.h = (s32b)(delta.y / mod_vp_ptr->vp.fontdim.h);

							show_coords(1, 2, delta.x, delta.y);
							show_coords(1, 3, delta.w, delta.h);

							if (delta.w || delta.h)
							{
								if (tempsurf == NULL)
								{
									tempsurf = sdl_utl_create_surface(0,
											screen.surface->w,
											screen.surface->h,
											BPP_AUTO,
											mod_vp_ptr->vp.rendmode);

									// remove the viewport from the screen
									sdl_utl_clr_vp_from_screen(mod_vp_ptr);

									// blit the screen to the temp surface
									SDL_BlitSurface(screen.surface, ENTIRE_SURFACE, tempsurf, ENTIRE_SURFACE);

									mod_vp_ptr->vp.refresh = TRUE;
								}

								if (do_redraw)
								{
									do_redraw = 0;

									mod_vp_ptr->vp.termsize.w += delta.w;
									mod_vp_ptr->vp.termsize.h += delta.h;

									mouse_pos.x += delta.x;
									mouse_pos.y += delta.y;

									delta.x = 0;
									delta.y = 0;
									delta.w = 0;
									delta.h = 0;

									show_coords(1, 4, mod_vp_ptr->vp.termsize.w, mod_vp_ptr->vp.termsize.h);

									/*
									* make sure the main window (ToME) meets its minimum
									* size requirements
									*/
									if (mod_vp_ptr->vp.id == 0)
									{
										// FIXME: magics

										if (mod_vp_ptr->vp.termsize.w < 80)
											mod_vp_ptr->vp.termsize.w = 80;

										if (mod_vp_ptr->vp.termsize.h < 24)
											mod_vp_ptr->vp.termsize.h = 24;
									}

									// grab the pointer to the current term so we can return to it
									current_term = &mod_vp_ptr->t;
									Term = &mod_vp_ptr->t;

									// restore the background - FIXME: this hurts
									SDL_BlitSurface(tempsurf, ENTIRE_SURFACE, screen.surface, ENTIRE_SURFACE);

									// force the term to resize
									Term_resize(mod_vp_ptr->vp.termsize.w, mod_vp_ptr->vp.termsize.h);

									/* grab the pointer to the resized term */
									Term = &mod_vp_ptr->t;

									// re-set the term area (size constraint check)
									mod_vp_ptr->vp.termsize.w = Term->wid;
									mod_vp_ptr->vp.termsize.h = Term->hgt;

									// resize the viewport
									//sdl_utl_set_vp_size(mod_vp_ptr);

									mod_vp_ptr->vp.rect.w = mod_vp_ptr->vp.termsize.w * mod_vp_ptr->vp.fontdim.w + BORDER * 2;
									mod_vp_ptr->vp.rect.h = mod_vp_ptr->vp.termsize.h * mod_vp_ptr->vp.fontdim.h + BORDER;
									ext_term_init_sdl(&mod_vp_ptr->t);

									Term = current_term;
								}
							}
							break;

						default:
							break;
					}
				}
				break;

			case SDL_MOUSEBUTTONUP:

				CLR_CFLAG(IN_TERM_DRAW_OP);

				if (vp_op == VP_TRANSLATE)
				{
					sdl_utl_calc_vp_coords(mod_vp_ptr, mod_vp_ptr->vp.rect.x, mod_vp_ptr->vp.rect.y);
				}

				if (tempsurf)
				{
					SDL_FreeSurface(tempsurf);
					tempsurf = NULL;
				}

				/* force all visible viewports to refresh */
				sdl_utl_force_refresh_all();

				do_redraw = 0;

				done = TRUE;
				break;

			default:
				break;
		}

		if (!done)
		{
			SDL_WaitEvent(event);
		}
	}

	return done;
}

s32b handle_mouse_btn3(SDL_Event* first_event)
{
	SDL_Event*	event = first_event;
	bool done = FALSE;

	while (!done)
	{
		SDL_PumpEvents();

		switch (event->type)
		{
			case SDL_MOUSEBUTTONDOWN:
				done = TRUE;
				break;

			case SDL_MOUSEBUTTONUP:
				done = TRUE;
				break;

			case SDL_MOUSEMOTION:
				done = TRUE;
				break;

			default:
				done = TRUE;
				break;
		}
	}

	return done;
}

void sdl_handle_mouse(SDL_Event* first_event)
{
	s32b			i = 0;
	s32b 		j = 0;
	s32b			z = 0;
	bool		done = FALSE;
	bool		overlap = FALSE;
	SDL_Event*	event = first_event;
	SDL_Rect	handle_rect;

	while (!done)
	{
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))
		{
			done = handle_mouse_btn1(event);
		}
		else
		{
			if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3))
			{
				done = handle_mouse_btn3(event);
			}
			else
			{
				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2))
					done = TRUE;
			}
		}

		/* deal with the window handles */
		if (!done)
		{
			switch (event->type)
			{
				case SDL_MOUSEMOTION:

					i = 0;
					while (i < max_terms)
					{
						z = vp_z_order[i];

						if (data[z].vp.surface)
						{
							handle_rect.x = data[z].vp.rect.x + data[z].vp.area.handle.x;
							handle_rect.y = data[z].vp.rect.y + data[z].vp.area.handle.y;
							handle_rect.w = data[z].vp.area.handle.w;
							handle_rect.h = data[z].vp.area.handle.h;

							if (data[z].vp.visible)
							{
								if (IN_BOUNDS(event->button.x, event->button.y, handle_rect))
								{
									/* see if another viewport is overlapping the current handle */
									j = i - 1;
									overlap = FALSE;
									while (j >=0 && !overlap)
									{
										if (is_overlapping(data[vp_z_order[j]].vp.rect, handle_rect))
										{
											overlap = TRUE;
										}

										j--;
									}

									if (!overlap)
									{
										/* no overlapping, so show the appropriate handle image */
										data[z].vp.current_handle = TRUE;
										SDL_BlitSurface(handlesurf, ENTIRE_SURFACE, screen.surface, &handle_rect);
										UPDATE_RECT(screen.surface, handle_rect);

										/* look for a previous handle image and remove it */
										j = 0;
										while (j < max_terms)
										{
											if (j != z)
											{
												if (data[j].vp.current_handle)
												{
													SDL_BlitSurface(data[j].vp.surface, &data[j].vp.area.handle,
															screen.surface, &handle_rect);
													UPDATE_RECT(screen.surface, handle_rect);
												}
											}
											j++;
										}
										return;
									}
								}
								else
								{
									if (data[z].vp.current_handle)
									{
										data[z].vp.current_handle = FALSE;
										SDL_BlitSurface(data[z].vp.surface, &data[z].vp.area.handle,
												screen.surface, &handle_rect);
										UPDATE_RECT(screen.surface, handle_rect);
									}
								}
							}
						}
						i++;
					}
					done = TRUE;
					break;

				default:
					break;
			}
		}

		if (!done)
			SDL_WaitEvent(event);

	} /* while (!done) */
}


/*
 * Handle all application events (key presses, mouse, etc)
 *
 * If a keypress contains any modifier keys (shift, ctrl, etc),
 * build a macro string using the modifiers with the key that was
 * just pressed and send it in the following format:
 * (mod-key)-(key name)
 *
 * For example, Ctrl-x would be sent as:
 * ctrl-x
 *
 * pref-sdl2.prf macro examples:
 * If you wanted to run west when you pressed
 * shift-KP4, for example, you would create this
 * entry in the pref-sdl.prf file:
 * A:\\.4
 * P:[shift-[4]]
 *
 * And, for the same command with the arrow key:
 * A:\\.4
 * P:[shift-left]
 *
 * Action/Pattern reference in pref-sdl2.prf:
 * A - The action to take in the game as
 * if you pressed the command key '/' and
 * then the command string (ex. '.' + '4' in
 * this case to run west).
 *
 * P - The pattern to match to perform the previously
 *     defined action (ex. shift-4).
 *
 * Note: All backslashes ('/') should be encoded as "//"
 *
 * See lib/pref/pref-sdl.prf for modifier definitions
 */

#define MAX_MOD_STR_LEN	64

static bool keypad_keysym(s32b key_sym)
{
	if ((key_sym == SDLK_KP1)
		|| (key_sym == SDLK_KP2)
		|| (key_sym == SDLK_KP3)
		|| (key_sym == SDLK_KP4)
		|| (key_sym == SDLK_KP5)
		|| (key_sym == SDLK_KP6)
		|| (key_sym == SDLK_KP7)
		|| (key_sym == SDLK_KP8)
		|| (key_sym == SDLK_KP9))
		return TRUE;

	return FALSE;
}

s32b sdl_event_handler(SDL_Event* event)
{
	s32b done = 0;
	s32b key_code = 0;
	s32b key_sym = 0;
	s32b key_mod = 0;
	char mod_str[MAX_MOD_STR_LEN];
	static char buf[MAX_MOD_STR_LEN];
	Uint32 interval, sound_channel;
	bool shift_down = 0;
	bool numlock_down = 0;	

	switch(event->type)
	{
		case SDL_KEYDOWN:

			key_code = event->key.keysym.unicode;
			key_sym = event->key.keysym.sym;
			key_mod = event->key.keysym.mod;								

			/*
			 * If the key is a normal ASCII key, *not* one of the keypad
			 * numbers, and the user isn't pressing any modifier keys,
			 * then just pass the plain keycode to the engine.
			 */
			if ((key_code > 0) && (key_code <= 127) && !keypad_keysym(key_sym)
				&& !(key_mod & (KMOD_LALT | KMOD_RALT))
				&& !(key_mod & (KMOD_LMETA | KMOD_RMETA)))
			{
				if TST_CFLAG(SHOW_KEYCODES)
				{
					show_key_code(1, 0, (s32b*)&key_code, 0);
				}
				Term_keypress(key_code);
			}
			else
			{
				/*
				 * build key strings for any modded key press, ignoring
				 * keypresses that are only modifier keys
				 */
				if (key_sym != SDLK_NUMLOCK
					&& key_sym != SDLK_CAPSLOCK
					&& key_sym != SDLK_SCROLLOCK
					&& key_sym != SDLK_RSHIFT
					&& key_sym != SDLK_LSHIFT
					&& key_sym != SDLK_RCTRL
					&& key_sym != SDLK_LCTRL
					&& key_sym != SDLK_RALT
					&& key_sym != SDLK_LALT
					&& key_sym != SDLK_RMETA
					&& key_sym != SDLK_LMETA
					&& key_sym != SDLK_LSUPER
					&& key_sym != SDLK_RSUPER
					&& key_sym != SDLK_MODE
					&& key_sym != SDLK_COMPOSE)
				{
					s32b i;

					/* Start the macro trigger string (char value 31) */
					sdl_strlcpy(mod_str, "\037", MAX_MOD_STR_LEN);

					/* Only allow these modifiers if the current engine status allows us to. */
					if (!ignore_numlock)
					{
						/* Put modifiers at front of trigger string. */
						if (key_mod & (KMOD_LCTRL | KMOD_RCTRL))
						{
							sdl_strlcat(mod_str, "N", MAX_MOD_STR_LEN);
						}

						/*
						* Add "shift" modifier if either the player is holding
						* down shift, or if it's a numeric keypad press
						* with num-lock mode enabled.
						*/

						shift_down   = (key_mod & (KMOD_LSHIFT | KMOD_RSHIFT))
									   != 0;
						numlock_down = (key_mod & KMOD_NUM) != 0;
											   											   						
						if (!(keypad_autorun && keypad_keysym(key_sym) && shift_down))	// allow walking with the numpad and shift while autorun is on
						{
							// run if the shift key is used, or if numlock is down, or if autorun and a numkey is pressed
							if (shift_down || (keypad_keysym(key_sym) && numlock_down) || (keypad_autorun && keypad_keysym(key_sym)))
							{
								sdl_strlcat(mod_str, "S", MAX_MOD_STR_LEN);
							}
						}

						if (key_mod & (KMOD_LALT | KMOD_RALT))
						{
							sdl_strlcat(mod_str, "O", MAX_MOD_STR_LEN);
						}

						if (key_mod & (KMOD_LMETA | KMOD_RMETA))
						{
							sdl_strlcat(mod_str, "M", MAX_MOD_STR_LEN);
						}
					}

					/* End modifiers section of trigger. */
					sdl_strlcat(mod_str, "_", MAX_MOD_STR_LEN);

					/*
					 * Add lowercased key name, replacing spaces and
					 * dashes with underscores.
					 */
					sdl_strtolower(buf, SDL_GetKeyName(key_sym), MAX_MOD_STR_LEN);

					for (i = 0; buf[i]; i++)
					{
						if (buf[i] == ' ')
							buf[i] = '_';
					}

					if ( ((char)key_code) != '-')
					{
						for (i = 0; buf[i]; i++)
						{
							if (buf[i] == '-')
								buf[i] = '_';

						}
					}

					sdl_strlcat(mod_str, buf, MAX_MOD_STR_LEN);

					/* End the macro trigger string (char value 13) */
					sdl_strlcat(mod_str, "\015", MAX_MOD_STR_LEN);

					if TST_CFLAG(SHOW_KEYCODES)
					{
						show_key_code(1, 0, (char*)mod_str, 1);
					}

					/* Send the macro trigger string */
					i = 0;
					while (mod_str[i])
					{
						Term_keypress(mod_str[i++]);
					}
				}
			}
			break;

		/* application exit requested via app close button */
		case SDL_QUIT:
			/* Quit */
			quit(NULL);
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEMOTION:
			sdl_handle_mouse(event);
			break;

		case SDL_VIDEORESIZE:
			screen.width = event->resize.w;
			screen.height = event->resize.h;

			sdl_init_term_window();

			if (init_complete)
				do_cmd_redraw();

			/* force all visible viewports to refresh */
			sdl_utl_force_refresh_all();
			break;

		case SDL_USEREVENT:
			switch (event->user.code)
			{
				case EVENT_REFRESH_VP:
					if (rect_changed)
					{
						sdl_utl_redraw_viewports();
					}
					do_redraw = 0;
					break;

				case EVENT_GAME_RT_TIMER:
					interval = (Uint32)event->user.data1;
					if (event->user.data2 != NULL)
					{
						((void (*)())event->user.data2)();
					}
					else
					{
						call_lua("time.rt.exec", "(d)", "", (s32b)interval);
					}					
					break;

				case EVENT_GAME_MUSIC_STOP:
					call_lua("sound.events.music_stop", "()", "");
					break;

				case EVENT_GAME_SOUND_CHANNEL_STOP:
					sound_channel = (Uint32)event->user.data1;
					call_lua("sound.events.channel_stop", "(d)", "", sound_channel);
					break;

				default:
					break;
			}

		default:
			break;
	}

	return done;
}

#endif /* USE_SDL */
