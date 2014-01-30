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

#include "angband.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <SDL_ttf.h>

#include <stdlib.h>

#include <memory.h>
#include <string.h>
#include "physfsrwops.h"
#include "sdl-types.h"
#include "sdl-defines.h"
#include "sdl-extern.h"

#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN					1024
#endif

#define DEFAULT_COLS					80
#define DEFAULT_ROWS					24
#define DEFAULT_POS_X					0
#define DEFAULT_POS_Y					0
#define DEFAULT_VISIBLE					0
#define DEFAULT_FONT_FILE				DEFAULT_FONT
#define DEFAULT_TTF_FONT_SIZE			10

/*
 * SDL relevant directory structures
 **************************************
 * .tome/
 *  - log                    :sdl-log.txt
 *  - <n.n>/<module>/conf    :sdl-mod.ini, sdl-modfontsets.ini
 *
 * $LIBDIR/
 * - game/media/graf         :handle.bmp, icon.png

 * - game/media/font         :various tome fonts
 */

/* core files and directories */
cptr SDL_LOG_DIR;				// <userdir>/.tome/log/
cptr SDL_ICON_FILE;   			// $LIBDIR/game/media/graf/icon.png
cptr SDL_FONT_DIR;				// $LIBDIR/game/media/font/


/*
 * An array of "term_data" structures, one for each "sub-window"
 */
term_data data[MAX_TERM_DATA];


/*
 * Globals
 */
screen_t			screen;
SDL_Color			sdl_fg;
SDL_Color			sdl_bg;
font_param_t		font_info;
SDL_Color			color_table[MAX_COLORS];
SDL_Color			border_color;
Uint8				errlvl;
s32b				client_flags;
s32b				max_terms;
SDL_Rect			destrect;
SDL_Surface*		handlesurf;
SDL_Surface*		backsurf;
SDL_Surface*		tempsurf;
s32b				num_terms;
s32b				vp_z_order[MAX_TERM_DATA];
s32b				refresh_rate;
void*				vp_update_params;
s32b				key_interval;
bool				init_complete;
gfx_t				gfx;
SDL_TimerID			refresh_timer_id;
unsigned short		do_redraw;
s32b				tile_size;
SDL_Rect			refresh_area[MAX_TERM_DATA];
SDL_Rect			refresh_area_max[MAX_TERM_DATA];
SDL_Surface*		tilesurf = NULL;         // tile image map surface
SDL_Surface**		tilesurf_features = NULL;

/* prototypes */

void sdl_init_colors();


/*** Function hooks needed by "Term" ***/


/*
 * Init a new "term"
 *
 * This function should do whatever is necessary to prepare a new "term"
 * for use by the "term.c" package.  This may include clearing the window,
 * preparing the cursor, setting the font/colors, etc.  Usually, this
 * function does nothing, and the "init_rlt()" function does it all.
 */
static void Term_init_sdl(term* t)
{

	term_data* td = (term_data*)(t->data);

	if (!td->vp.visible)
		return;

	/*
	 * Create the terminal (viewport) surfaces with a
	 * predefined border. The text content area will be inside
	 * the surface's borders. The borders will increase the
	 * size of the surface by border size * 2 + one glyph width
	 * (equal borders on each of the surface edges and the
	 * content inside the borders, hence the +one glyph width)
	 */

	/* create the main surface */
	if (td->vp.surface)
	{
		SDL_FreeSurface(td->vp.surface);
	}

	td->vp.surface = sdl_utl_create_surface(0,
											td->vp.termsize.w * td->vp.fontdim.w + td->vp.fontdim.w * 2 + BORDER * 2,
											td->vp.termsize.h * td->vp.fontdim.h + td->vp.fontdim.h * 2 + BORDER * 2,
											BPP_AUTO,
											td->vp.rendmode);

	/* no alpha on the main viewport surface */
	SDL_SetAlpha(td->vp.surface, 0, 0);

	/* create the text scratch pad surface */
	if (td->vp.textsurf)
	{
		SDL_FreeSurface(td->vp.textsurf);
	}

	td->vp.textsurf = sdl_utl_create_surface(0,
										td->vp.fontdim.w,
										td->vp.fontdim.h,
										BPP_AUTO,
										td->vp.rendmode);

	if (td->vp.rendmode == REND_SOLID)
	{
		/* render solid path */

		/* set the color key */
		SDL_SetColorKey(td->vp.textsurf, SDL_SRCCOLORKEY,
							 SDL_MapRGBA(td->vp.textsurf->format,
											 font_info.fg.r,
											 font_info.fg.g,
											 font_info.fg.b,
											 SDL_ALPHA_OPAQUE));

		SDL_SetAlpha(td->vp.textsurf, NO_ALPHA, SDL_ALPHA_OPAQUE);
	}
	else
	{
		/* alpha */
		SDL_SetAlpha(td->vp.textsurf, SDL_SRCALPHA, 0);
	}

	/* create the coloring surface */
	if (td->vp.colorsurf)
	{
		SDL_FreeSurface(td->vp.colorsurf);
	}

	if (td->vp.rendmode == REND_SOLID)
	{
		td->vp.colorsurf = sdl_utl_create_surface(0,
										td->vp.fontdim.w,
										td->vp.fontdim.h,
										BPP_AUTO,
										td->vp.rendmode);

		/* no alpha */
		SDL_SetAlpha(td->vp.colorsurf, NO_ALPHA, SDL_ALPHA_OPAQUE);
	}
	else
	{
		td->vp.colorsurf = sdl_utl_create_surface(0,
										td->vp.fontdim.w,
										td->vp.fontdim.h,
										BPP_AUTO,
										td->vp.rendmode);
	}

	/* calculate all the various terminal areas */
	sdl_utl_calc_vp_coords(td, td->vp.rect.x, td->vp.rect.y);

	/* make the terminal borders */
	sdl_utl_make_vp_border(td);
	sdl_set_title(td, td->vp.name);

	REDRAW_VP(td->vp.id, ENTIRE_SURFACE, &td->vp.rect);
}

/*
 * give Term_init_sdl() a externally callable function
 */

void ext_term_init_sdl(term* t)
{
	Term_init_sdl(t);
}


/*
 * Nuke an old "term"
 *
 * This function is called when an old "term" is no longer needed.  It should
 * do whatever is needed to clean up before the program exits, such as wiping
 * the screen, restoring the cursor, fixing the font, etc.  Often this function
 * does nothing and lets the operating system clean up when the program quits.
 */
static void Term_nuke_sdl(term *t)
{
	term_data *td = (term_data*)(t->data);

	if (td->vp.surface)
		SDL_FreeSurface(td->vp.surface);

	if (td->vp.textsurf)
		SDL_FreeSurface(td->vp.textsurf);

	if (td->vp.colorsurf)
		SDL_FreeSurface(td->vp.colorsurf);

	if (td->vp.cursurf)
		SDL_FreeSurface(td->vp.cursurf);

	td->vp.surface = 0;
	td->vp.textsurf = 0;
	td->vp.colorsurf = 0;
	td->vp.cursurf = 0;
}



/*
 * Do a "user action" on the current "term"
 *
 * This function allows the visual module to do implementation defined
 * things when the user activates the "system defined command" command.
 *
 * This function is normally not used.
 *
 * In general, this function should return zero if the action is successfully
 * handled, and non-zero if the action is unknown or incorrectly handled.
 */
static errr Term_user_sdl(s32b n)
{
	term_data *td = (term_data*)(Term->data);

	/* eliminate build warnings */
	td = td;

	/* XXX XXX XXX */

	/* Unknown */
	return (1);
}


/*
 * Display the cursor
 *
 * This routine should display the cursor at the given location
 * (x,y) in some manner.  On some machines this involves actually
 * moving the physical cursor, on others it involves drawing a fake
 * cursor in some form of graphics mode.  Note the "soft_cursor"
 * flag which tells "z-term.c" to treat the "cursor" as a "visual"
 * thing and not as a "hardware" cursor.
 *
 * You may assume "valid" input if the window is properly sized.
 *
 * You may use the "Term_grab(x, y, &a, &c)" function, if needed,
 * to determine what attr/char should be "under" the new cursor,
 * for "inverting" purposes or whatever.
 */

static errr Term_curs_sdl(s32b x, s32b y)
{
	/* we must use a soft cursor for this to work */

	term_data* td = (term_data*)(Term->data);

	if (td->vp.visible)
	{
		/* draw it to the vp surface */
		SDL_Rect destrect = { td->vp.area.content.x + td->vp.fontdim.w * x,
							  td->vp.area.content.y + td->vp.fontdim.h * y,
							  td->vp.fontdim.w,
							  td->vp.fontdim.h };

		/* place the current cursor */
		SDL_BlitSurface(td->vp.cursurf, ENTIRE_SURFACE, td->vp.surface, &destrect);
		sdl_utl_add_rect(td->vp.id, &destrect);
		td->vp.refresh = TRUE;
	}

	/* Success */
	return (0);
}


/*
 * Erase some characters
 *
 * This function should erase "n" characters starting at (x,y).
 *
 * You may assume "valid" input if the window is properly sized.
 */
static errr Term_wipe_sdl(s32b x, s32b y, s32b n)
{
	term_data* td = (term_data*)(Term->data);

	if (td->vp.visible)
	{
		sdl_utl_clear_rect(td, x, y, n);
	}

	/* Success */
	return (0);
}


/*
 * Draw some text on the screen
 *
 * This function should actually display an array of characters
 * starting at the given location, using the given "attribute",
 * and using the given string of characters, which contains
 * exactly "n" characters and which is NOT null-terminated.
 *
 * You may assume "valid" input if the window is properly sized.
 *
 * You must be sure that the string, when written, erases anything
 * (including any visual cursor) that used to be where the text is
 * drawn.  On many machines this happens automatically, on others,
 * you must first call "Term_wipe_rlt()" to clear the area.
 *
 * In color environments, you should activate the color contained
 * in "color_data[a & 0x0F]", if needed, before drawing anything.
 *
 * You may ignore the "attribute" if you are only supporting a
 * monochrome environment, since this routine is normally never
 * called to display "black" (invisible) text, including the
 * default "spaces", and all other colors should be drawn in
 * the "normal" color in a monochrome environment.
 *
 * Note that if you have changed the "attr_blank" to something
 * which is not black, then this function must be able to draw
 * the resulting "blank" correctly.
 *
 * Note that this function must correctly handle "black" text if
 * the "always_text" flag is set, if this flag is not set, all the
 * "black" text will be handled by the "Term_wipe_rlt()" hook.
 */
static errr Term_text_sdl(s32b x, s32b y, s32b n, byte a, const char* cp)
{
	term_data* td = (term_data*)(Term->data);

	if (td->vp.visible)
	{
		sdl_putstr(td, x, y, n, a, cp);
	}

	/* Success */
	return (0);
}


/*
 * Draw some attr/char pairs on the screen
 *
 * This routine should display the given "n" attr/char pairs at
 * the given location (x,y).  This function is only used if one
 * of the flags "always_pict" or "higher_pict" is defined.
 *
 * You must be sure that the attr/char pairs, when displayed, will
 * erase anything (including any visual cursor) that used to be at
 * the given location.  On many machines this is automatic, but on
 * others, you must first call "Term_wipe_rlt(x, y, 1)".
 *
 * With the "higher_pict" flag, this function can be used to allow
 * the display of "pseudo-graphic" pictures, for example, by using
 * the attr/char pair as an encoded index into a pixmap of special
 * "pictures".
 *
 * With the "always_pict" flag, this function can be used to force
 * every attr/char pair to be drawn by this function, which can be
 * very useful if this file can optimize its own display calls.
 *
 * This function is often associated with the "arg_graphics" flag.
 *
 * This function is only used if one of the "higher_pict" and/or
 * "always_pict" flags are set.
 */
static errr Term_pict_sdl(s32b x, s32b y, s32b n, const byte *ap, const char *cp)
{
	s32b i, x1, y1;

	SDL_Rect destrect, srcrect;

	byte a;
	char c;

	term_data *td = (term_data*)(Term->data);

	destrect.x = td->vp.area.content.x + x * td->vp.fontdim.w;
	destrect.y = td->vp.area.content.y + y * td->vp.fontdim.h;
	destrect.w = td->vp.fontdim.w;
	destrect.h = td->vp.fontdim.h;

	if (destrect.x < 0)
		destrect.x = 0;

	for (i = 0; i < n; ++i)
	{
		a = *ap++;
		c = *cp++;

		/* For extra speed - cache these values */
		x1 = (c & 0x7F) * td->vp.fontdim.w;
		y1 = (a & 0x7F) * td->vp.fontdim.h;

		srcrect.x = x1;
		srcrect.y = y1;
		srcrect.w = td->vp.fontdim.w;
		srcrect.h = td->vp.fontdim.h;

		SDL_BlitSurface(tilesurf, &srcrect, td->vp.surface, &destrect);

		destrect.x += td->vp.fontdim.w;
	}

	destrect.x = td->vp.area.content.x + x * td->vp.fontdim.w;
	destrect.y = td->vp.area.content.y + y * td->vp.fontdim.h;
	destrect.w = td->vp.area.content.x + x * td->vp.fontdim.w * n;

	sdl_utl_add_rect(td->vp.id, &destrect);

	return 0;
}

/*
 * Do a "special thing" to the current "term"
 *
 * This function must react to a large number of possible arguments, each
 * corresponding to a different "action request" by the "z-term.c" package,
 * or by the application itself.
 *
 * The "action type" is specified by the first argument, which must be a
 * constant of the form "TERM_XTRA_*" as given in "term.h", and the second
 * argument specifies the "information" for that argument, if any, and will
 * vary according to the first argument.
 *
 * In general, this function should return zero if the action is successfully
 * handled, and non-zero if the action is unknown or incorrectly handled.
 */
static errr Term_xtra_sdl(s32b n, s32b v)
{
	term_data* td = (term_data*)(Term->data);
	SDL_Event event;
	s32b i;

	/* Analyze */
	switch (n)
	{
		case TERM_XTRA_EVENT:
		{
			/*
			 * Process some pending events XXX XXX XXX
			 *
			 * Wait for at least one event if "v" is non-zero
			 * otherwise, if no events are ready, return at once.
			 * When "keypress" events are encountered, the "ascii"
			 * value corresponding to the key should be sent to the
			 * "Term_keypress()" function.  Certain "bizarre" keys,
			 * such as function keys or arrow keys, may send special
			 * sequences of characters, such as control-underscore,
			 * plus letters corresponding to modifier keys, plus an
			 * underscore, plus carriage return, which can be used by
			 * the main program for "macro" triggers.  This action
			 * should handle as many events as is efficiently possible
			 * but is only required to handle a single event, and then
			 * only if one is ready or "v" is true.
			 *
			 * This action is required.
			 */

			if (v)
			{
				/* Perform event checking with blocking */
				SDL_WaitEvent(&event);
				sdl_event_handler(&event);
			}
			else
			{
				/* Perform event checking without blocking */
				if (SDL_PollEvent(&event))
				{
					sdl_event_handler(&event);
				}
			}

			return (0);
		}

		case TERM_XTRA_FLUSH:
		{
			/*
			 * Flush all pending events XXX XXX XXX
			 *
			 * This action should handle all events waiting on the
			 * queue, optionally discarding all "keypress" events,
			 * since they will be discarded anyway in "z-term.c".
			 *
			 * This action is required, but may not be "essential".
			 */

			while (SDL_PollEvent(&event))
			{
				sdl_event_handler(&event);
			}

			return (0);
		}

		case TERM_XTRA_CLEAR:
		{
			/*
			 * Clear the entire window XXX XXX XXX
			 *
			 * This action should clear the entire window, and redraw
			 * any "borders" or other "graphic" aspects of the window.
			 *
			 * This action is required.
			 */

			/* make the terminal borders */
			sdl_utl_clear_content(td);

			td->vp.refresh = TRUE;

			return (0);
		}

		case TERM_XTRA_SHAPE:
		{
			/*
			 * Set the cursor visibility XXX XXX XXX
			 *
			 * This action should change the visibility of the cursor,
			 * if possible, to the requested value (0=off, 1=on)
			 *
			 * This action is optional, but can improve both the
			 * efficiency (and attractiveness) of the program.
			 */

			return (0);
		}

		case TERM_XTRA_FROSH:
		{
			/*
			 * Flush a row of output XXX XXX XXX
			 *
			 * This action should make sure that row "v" of the "output"
			 * to the window will actually appear on the window.
			 *
			 * This action is optional, assuming that "Term_text_xxx()"
			 * (and similar functions) draw directly to the screen, or
			 * that the "TERM_XTRA_FRESH" entry below takes care of any
			 * necessary flushing issues.
			 */

			return (0);
		}

		case TERM_XTRA_FRESH:
		{
			/*
			 * Flush output XXX XXX XXX
			 *
			 * This action should make sure that all "output" to the
			 * window will actually appear on the window.
			 *
			 * This action is optional, assuming that "Term_text_xxx()"
			 * (and similar functions) draw directly to the screen, or
			 * that the "TERM_XTRA_FROSH" entry above takes care of any
			 * necessary flushing issues.
			 */

			if (!init_complete)
			{
				i = 0;
				while (i < max_terms)
				{
					if (data[i].vp.visible)
						data[i].vp.refresh = TRUE;

					i++;
				}
				sdl_utl_redraw_viewports();
			}

			return (0);
		}

		case TERM_XTRA_NOISE:
		{
			/*
			 * Make a noise XXX XXX XXX
			 *
			 * This action should produce a "beep" noise.
			 *
			 * This action is optional, but convenient.
			 */

			return (0);
		}

		case TERM_XTRA_SOUND:
		{
			/*
			 * Make a sound XXX XXX XXX
			 *
			 * This action should produce sound number "v", where the
			 * "name" of that sound is "sound_names[v]".  This method
			 * is still under construction.
			 *
			 * This action is optional, and not very important.
			 */

			return (0);
		}

		case TERM_XTRA_BORED:
		{
			/*
			 * Handle random events when bored XXX XXX XXX
			 *
			 * This action is optional, and normally not important
			 */

			return (0);
		}

		case TERM_XTRA_REACT:
		{
			/*
			 * React to global changes XXX XXX XXX
			 *
			 * For example, this action can be used to react to
			 * changes in the global "color_table[256][4]" array.
			 *
			 * This action is optional, but can be very useful for
			 * handling "color changes" and the "arg_sound" and/or
			 * "arg_graphics" options.
			 */

			sdl_init_colors();

			return (0);
		}

		case TERM_XTRA_ALIVE:
		{
			/*
			 * Change the "hard" level XXX XXX XXX
			 *
			 * This action is used if the program changes "aliveness"
			 * by being either "suspended" (v=0) or "resumed" (v=1)
			 * This action is optional, unless the computer uses the
			 * same "physical screen" for multiple programs, in which
			 * case this action should clean up to let other programs
			 * use the screen, or resume from such a cleaned up state.
			 *
			 * This action is currently only used by "main-gcu.c",
			 * on UNIX machines, to allow proper "suspending".
			 */

			return (0);
		}

		case TERM_XTRA_LEVEL:
		{
			/*
			 * Change the "soft" level XXX XXX XXX
			 *
			 * This action is used when the term window changes "activation"
			 * either by becoming "inactive" (v=0) or "active" (v=1)
			 *
			 * This action can be used to do things like activate the proper
			 * font / drawing mode for the newly active term window.  This
			 * action should NOT change which window has the "focus", which
			 * window is "raised", or anything like that.
			 *
			 * This action is optional if all the other things which depend
			 * on what term is active handle activation themself, or if only
			 * one "term_data" structure is supported by this file.
			 */

			return (0);
		}

		case TERM_XTRA_WINVIS:
		{
			/*
			 * Change the window visibility
			 *
			 * This action is used when a window visibility option is changed
			 * in Angband Options -> Window Flags in game. The window visibility
			 * state is either on (v=1) or off (v=0).
			 */

			if (td->t.active_flag)
			{
				if (!v)
				{
					if (td->vp.visible)
					{
						sdl_utl_clr_vp_from_screen(td);
						td->vp.visible = v;
						td->vp.refresh = TRUE;
					}
				}
				else
				{
					if (!td->vp.visible)
					{
						td->vp.visible = v;
						Term_init_sdl(&td->t);

						sdl_utl_reorder_z_pos(td);

						td->vp.refresh = TRUE;
					}
				}

				if (td->vp.refresh)
				{
					do_cmd_redraw();

					sdl_utl_force_refresh_all();
					td->vp.refresh = FALSE;
				}
			}

			return (0);
		}

		case TERM_XTRA_DELAY:
		{
			/*
			 * Delay for some milliseconds XXX XXX XXX
			 *
			 * This action is useful for proper "timing" of certain
			 * visual effects, such as breath attacks.
			 *
			 * This action is optional, but may be required by this file,
			 * especially if special "macro sequences" must be supported.
			 */

			//SDL_Delay(v);

			return (0);
		}

		case TERM_XTRA_EFFECTS_DELAY:
		{
			/*
			* Delay for some milliseconds XXX XXX XXX
			*
			* This action is useful for proper "timing" of certain
			* visual effects, such as breath attacks.
			*
			* This action is optional, but may be required by this file,
			* especially if special "macro sequences" must be supported.
			*/

			SDL_Delay(v);

			return (0);
		}

		case TERM_XTRA_GET_DELAY:
		{
			/*
          * Get Delay of some milliseconds XXX XXX XXX
          * place the result in Term_xtra_long
			 *
			 * This action is useful for proper "timing" of certain
			 * visual effects, such as recording cmovies.
			 *
          * This action is optional, but cmovies wont perform
          * good without it
			 */

			return (0);
		}

		/* Rename main window */
		case TERM_XTRA_RENAME_MAIN_WIN:
			SDL_WM_SetCaption(angband_term_name[0], NULL);
			return (0);

		case TERM_XTRA_LOAD_GFX:
		{
#ifdef USE_GRAPHICS
			char filename[MAX_PATH_LEN];

			if (!arg_graphics && !gfx.enabled)
				return (0);

			use_graphics = FALSE;

			/*
			 * Load the gfx tiles with any associated masks
			 * and re-init the term to properly fit the tiles.
			 */

			/* set up the graphics if command line arguments are given */
			if (tile_size)
			{
				/* verify the file exists */
				switch (tile_size)
				{
					case GRAPHICS_8X8:
						gfx.filename	= string_make("8x8.bmp");
						gfx.tile.w		= 8;
						gfx.tile.h		= 8;
						TOME_GRAPHICS 	= "8";
						printf("loading 8x8 graphics\n");
						break;

					case GRAPHICS_16X16:
						gfx.filename	= string_make("16x16.bmp");
						gfx.tile.w		= 16;
						gfx.tile.h		= 16;
						TOME_GRAPHICS	= "16";
						printf("loading 16x16 graphics\n");
						break;

					case GRAPHICS_32X32:
						gfx.filename	= string_make("32x32.bmp");
						gfx.tile.w		= 32;
						gfx.tile.h		= 32;
						TOME_GRAPHICS	= "32";
						printf("loading 32x32 graphics\n");
						break;

					case GRAPHICS_NONE:
					default:
						sdl_log_warn("Improper tile size argument. Defaulting to text mode");
						return (0);
				}
			}

			/* Build the path and check it exists */
			if (gfx.filename)
				path_build(filename, 1024, TENGINE_DIR_GRAF, gfx.filename);
			else
			{
				sdl_log_warn("Graphics tile file (%s) not defined. Defaulting to text mode.", gfx.filename);
				use_graphics = FALSE;
				arg_graphics = FALSE;
				return (0);
			}

			if (PHYSFS_exists(filename))
			{
				s32b i;

				use_graphics = TRUE;
				arg_graphics = TRUE;

				C_MAKE(tilesurf_features, max_f_idx, SDL_Surface*);

				/* save the gfx parameters */
				gfx.enabled = 1;
				gfx.tilesize = gfx.tile.w;

				/* Initialize the windows */
				for (i = 0; i < max_terms; i++)
				{
					term_data *td = &data[i];

					term *t = &td->t;

					t->pict_hook = Term_pict_sdl;

					t->higher_pict = TRUE;
				}
			}
			else
			{
				sdl_log_warn("%s not found. Defaulting to text mode.", filename);
			}

#endif /* USE_GRAPHICS */
			return (0);
		}
	}

	/* Unknown or Unhandled action */
	return (1);
}


/*
 * Instantiate a "term_data" structure
 *
 * This is one way to prepare the "term_data" structures and to
 * "link" the various informational pieces together.
 *
 * This function assumes that every window should be 80x24 in size
 * (the standard size) and should be able to queue 256 characters.
 * Technically, only the "main screen window" needs to queue any
 * characters, but this method is simple.  One way to allow some
 * variation is to add fields to the "term_data" structure listing
 * parameters for that window, initialize them in the "init_rlt()"
 * function, and then use them in the code below.
 *
 * Note that "activation" calls the "Term_init_rlt()" hook for
 * the "term" structure, if needed.
 */
static void term_data_link()
{
	term_data* td;
	term* t;
	s32b i = 0;
	char tilefile[MAX_LINE_LEN];

	max_terms = MAX_TERM_DATA;

	/*
	 * Set up the terminals
	 */

	max_terms = esettings_get_int("sdl.screen.num_terms", MAX_TERM_DATA);
	if (max_terms > MAX_TERM_DATA)
	{
		max_terms = MAX_TERM_DATA;
	}

	if (max_terms == 0)
	{
		max_terms = 1;
	}

	/* load in the data common to all terminals */
	font_info.first_ch = esettings_get_int("sdl.common.first_char", FONT_PARAM_FIRST_CH);
	if (font_info.first_ch < 0x20)
	{
		font_info.first_ch = 0x20;
	}

	font_info.last_ch =			esettings_get_int("sdl.common.last_char", FONT_PARAM_LAST_CH);
	font_info.fg.r =			esettings_get_int("sdl.common.font.forecolor.r", FONT_PARAM_FG_R);
	font_info.fg.g =			esettings_get_int("sdl.common.font.forecolor.g", FONT_PARAM_FG_G);
	font_info.fg.b =			esettings_get_int("sdl.common.font.forecolor.b", FONT_PARAM_FG_B);
	font_info.bg.r =			esettings_get_int("sdl.common.font.backcolor.r", FONT_PARAM_BG_R);
	font_info.bg.g =			esettings_get_int("sdl.common.font.backcolor.g", FONT_PARAM_BG_G);
	font_info.bg.b =			esettings_get_int("sdl.common.font.backcolor.b", FONT_PARAM_BG_B);
	border_color.r =			esettings_get_int("sdl.common.border.r", BORDER_COLOR_R);
	border_color.g =			esettings_get_int("sdl.common.border.g", BORDER_COLOR_G);
	border_color.b =			esettings_get_int("sdl.common.border.b", BORDER_COLOR_B);
	gfx.enabled =				esettings_get_int("sdl.graphics.enabled", 0);
	gfx.tilesize =				esettings_get_int("sdl.graphics.tile.size", 0);
	snprintf(tilefile, MAX_LINE_LEN, "%s",  esettings_get_str("sdl.graphics.tile.filename", ""));
	gfx.filename = 				string_make(tilefile);

	if (gfx.turn_off)
		gfx.enabled = 0;

	if (gfx.enabled && !arg_graphics)
	{
		gfx.tile.w = gfx.tilesize;
		gfx.tile.h = gfx.tilesize;

		switch (gfx.tilesize)
		{
			case 8:
				TOME_GRAPHICS	= "8";
				break;

			case 16:
				TOME_GRAPHICS	= "16";
				break;

			case 32:
				TOME_GRAPHICS	= "32";
				break;

			default:
				gfx.enabled = 0;
				sdl_log_warn("Improper tile size argument. Defaulting to text mode");
				break;
		}
	}

	/* set the default game colors */
	sdl_utl_color_copy(&sdl_fg, &font_info.fg);
	sdl_utl_color_copy(&sdl_bg, &font_info.bg);

	/* initialize all the terms, regardless of max_terms */
	i = max_terms;
	while (--i >= 0)
	{
		td = &data[i];
		t = &td->t;

		(void) WIPE(td, term_data);

		/* only handle valid terminals */
		if (i < max_terms)
		{
			/* load the terminal data from ini values */
			sdl_glyph_load_params(td, i);

			/* set the terminal's base colors */
			sdl_utl_color_copy(&td->vp.fg, &font_info.fg);
			sdl_utl_color_copy(&td->vp.bg, &font_info.bg);

			sdl_glyphs_create(td);

			/* turn on viewport borders */
			td->vp.bordervis = 1;

			/* grab handles are off */
			td->vp.current_handle = FALSE;

			/* make the terminal's cursor */
			sdl_glyph_make_cursor(td);
		}

		/* initialize the terminal */
		term_init(t, td->vp.termsize.w, td->vp.termsize.h, 256);

		/* clear the backup term pointers */
		td->vp.ccpy = NULL;
		td->vp.acpy = NULL;

		/* clear the refresh flags */
		td->vp.refresh = FALSE;

		/* Set the hooks */
		t->init_hook = Term_init_sdl;
		t->nuke_hook = Term_nuke_sdl;
		t->xtra_hook = Term_xtra_sdl;
		t->curs_hook = Term_curs_sdl;
		t->wipe_hook = Term_wipe_sdl;
		t->text_hook = Term_text_sdl;

		/* junk */
		t->user_hook = Term_user_sdl;
		t->pict_hook = Term_pict_sdl;

		/* use a soft cursor */
		t->soft_cursor = TRUE;

		/* Picture routine flags */
		t->always_pict = FALSE;
		t->higher_pict = TRUE;
		t->always_text = FALSE;

		/* erase with "dark" white space */
		t->attr_blank = TERM_DARK;
		t->char_blank = ' ';

		/* Save the data */
		t->data = td;

		/* Activate it */
		Term_activate(t);

		/* Save the global terminal */
		angband_term[i] = Term;

		refresh_area[i].x = td->vp.rect.x + td->vp.rect.w;
		refresh_area[i].y = td->vp.rect.y + td->vp.rect.h;
		refresh_area[i].w = 0;
		refresh_area[i].h = 0;

		refresh_area_max[i].x = td->vp.rect.x + td->vp.rect.w;
		refresh_area_max[i].y = td->vp.rect.y + td->vp.rect.h;
		refresh_area_max[i].w = 0;
		refresh_area_max[i].h = 0;
	}

	sdl_glyph_save_parameters();
}

void sdl_load_ini_files()
{
	/* load in the main window parameters */
	if (screen.width == 0)
	{
		screen.width =	esettings_get_int("sdl.screen.width", (DEFAULT_COLS + 2) * DEFAULT_FONT_WIDTH + BORDER * 2);
	}

	if (screen.height == 0)
	{
		screen.height = esettings_get_int("sdl.screen.height", DEFAULT_ROWS * (DEFAULT_FONT_HEIGHT + 1) + BORDER);
	}
}

void sdl_init_colors()
{
	s32b i;

	/* Prepare normal colors */
	for (i = 0; i < 256; ++i)
	{
		sdl_utl_color_set(i, angband_color_table[i][1], angband_color_table[i][2], angband_color_table[i][3]);
	}
}

/*
 * Timer functions
 */

SDL_TimerID sdl_create_timer(Uint32 interval, SDL_NewTimerCallback callback_func, void* update_params)
{
	SDL_TimerID id = 0;

	id = SDL_AddTimer(interval, callback_func, update_params);

	if (!id)
	{
		/* terminate the application */
		sdl_log_error("Could not start timer: %s", SDL_GetError());
	}

	return id;
}

void sdl_remove_timer(SDL_TimerID* timer_id)
{
	if (*timer_id)
	{
		if (SDL_RemoveTimer(*timer_id))
		{
			*timer_id = 0;
		}
	}
}

/*
 * WARNING: No SDL operations should occur in a timer callback under
 *          X11. This can cause a lock-up or crash. This is due to
 *          calling a SDL function from a thread different from the thread
 *          that created the screen surface. The SDL maintainers are
 *			aware of the problem in X11.
 *
 *			Google this for more info: +site:www.libsdl.org Xlib: unexpected async reply
 */

Uint32 vp_refresh_callback(Uint32 interval, void *param)
{
	SDL_Event event;

	if (refresh_timer_id && !do_redraw)
	{
		event.type = SDL_USEREVENT;
		event.user.code = EVENT_REFRESH_VP;
		event.user.data1 = 0;
		event.user.data2 = 0;

		do_redraw = 1;

		SDL_PushEvent(&event);
	}

	// returning a non-zero keeps the timer running
	return interval;
}

void sdl_load_handle()
{
	char handle_file[MAX_PATH_LEN];

	path_build(handle_file, MAX_PATH_LEN, TENGINE_DIR_GRAF, SDL_HANDLE_FILE);

	if (PHYSFS_exists(handle_file))
	{
		handlesurf = IMG_Load_RW(PHYSFSRWOPS_openRead(handle_file), 1);
   	}

	if (handlesurf == NULL)
	{
		sdl_log_warn("Could not load window handle (%s).", handle_file);

		handlesurf = sdl_utl_create_surface(0, GRAB_HANDLE_WIDTH, GRAB_HANDLE_HEIGHT, BPP_AUTO, REND_BLEND);

		if (handlesurf)
		{
			SDL_FillRect(handlesurf, ENTIRE_SURFACE,
							 SDL_MapRGBA(handlesurf->format,
											 0x00,
											 0x80,
											 0x00,
											 SDL_ALPHA_OPAQUE));
		}
		else
		{
			sdl_log_warn("Could not create alternate window handle");
		}

		return;
	}

	SDL_SetAlpha(handlesurf, NO_ALPHA, 0);
	SDL_SetColorKey(handlesurf, SDL_SRCCOLORKEY,
						 SDL_MapRGBA(handlesurf->format,
										 0xff,
										 0,
										 0xff,
										 SDL_ALPHA_OPAQUE));
}

void sdl_init_term_window()
{
	Uint32 flags = SDL_DOUBLEBUF | SDL_VIDEORESIZE;
	SDL_Surface* icon = NULL;
	char icon_file[MAX_PATH_LEN];
	const SDL_VideoInfo* video_info;

	if (TST_CFLAG(DO_FULLSCREEN))
	{
		flags |= SDL_FULLSCREEN;
	}
	else
	{
		if (esettings_get_int("sdl.screen.fullscreen", 0))
			flags |= SDL_FULLSCREEN;
		else
			flags &= ~SDL_FULLSCREEN;
	}

	video_info = SDL_GetVideoInfo();

	if (video_info->blit_hw_CC && video_info->blit_fill)
	{
		flags |= SDL_HWSURFACE;
	}
	else
	{
		flags |= SDL_SWSURFACE | SDL_ANYFORMAT;
	}

	/* set the window icon */
	path_build(icon_file, MAX_PATH_LEN, TENGINE_DIR_GRAF, "tome-icon.png");

	if (PHYSFS_exists(icon_file))
	{
		icon = IMG_Load_RW(PHYSFSRWOPS_openRead(icon_file), 1);

		if (icon == NULL)
		{
			sdl_log_warn("Could not load window icon (%s).", icon_file);
		}
		else
		{
			SDL_WM_SetIcon(icon, NULL);
		}
   }

	SDL_WM_SetCaption("ToME", NULL);

	if (screen.surface)
		SDL_FreeSurface(screen.surface);

	if ((flags & SDL_HWSURFACE) == SDL_HWSURFACE)
	{
		/* Direct hardware blitting without double-buffering causes really bad flickering. */
		if (video_info->video_mem * 1024 > (screen.width * screen.height * 32 / 8))
		{
			flags |= SDL_DOUBLEBUF;
		}
		else
		{
			flags &= ~SDL_HWSURFACE;
		}
	}

   /* create the primary window surface */
	screen.surface = SDL_SetVideoMode(screen.width,
									  screen.height,
         							  BPP_AUTO,
         							  flags);

	if (icon)
		SDL_FreeSurface(icon);

	if(screen.surface->flags & SDL_HWSURFACE)
		sdl_log_info("Using hardware surfaces");
	else
		sdl_log_info("Using software surfaces");

	if(screen.surface->flags & SDL_DOUBLEBUF)
		sdl_log_info("Using Double Buffering");
	else
		sdl_log_info("Using a single buffer");

	if (SDL_MUSTLOCK(screen.surface))
		sdl_log_info("Must lock screen surface");
	else
		sdl_log_info("Screen surface locking not required");

	if (NULL == screen.surface)
	{
		/* terminate the application */
		sdl_log_error("Could not set video mode");
	}

	//SDL_SetClipRect(screen.surface, NULL);

   sdl_log_info("SDL term initialized");
}

void sdl_init_sdl_components()
{
	sdl_log_info("Initializing SDL video");
	if (SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO)
	{
		 sdl_log_info("SDL already initialized");
		return;
	}

/* Initialize SDL */

/*
 * SDL_INIT_EVENTTHREAD will most likely cause Win32 systems to crash
 *
 * See http://www.libsdl.org/cgi/docwiki.cgi/Programming_20FAQ for details
 */
#ifdef WIN32
#define SDL_INIT_FLAGS (SDL_INIT_EVERYTHING & ~SDL_INIT_EVENTTHREAD)
#else
#define SDL_INIT_FLAGS SDL_INIT_EVERYTHING
#endif

	if (SDL_Init(SDL_INIT_FLAGS) == -1)
	{
		/* terminate the application */
		sdl_log_error("Could not initialize SDL (%s)", SDL_GetError());
	}

	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(500, key_interval);

	sdl_log_info("SDL initialized");
}

#define MIN_NUM_OF_ARGS		1

void sdl_process_args(s32b argc, char** argv)
{
	s32b i = 1;
	bool done = FALSE;
	char buf[MAX_PATH_LEN];

	while (i < argc && !done)
	{
		/* make sure the options are properly noted */
		if (argv[i][0] != '-')
		{
			snprintf(buf, MAX_ARG_LEN, "Improper/unknown argument found (%s).", argv[i]);
			quit(buf);
		}

		/* check for any arguments that require a value */
		if ((argv[i][1] == 'x' || argv[i][1] == 'X'
			|| argv[i][1] == 'y' || argv[i][1] == 'Y'
			|| argv[i][1] == 'g' || argv[i][1] == 'G')
			&& argv[i + 1] == 0)
		{
			snprintf(buf, MAX_ARG_LEN, "Missing argument for option %s", argv[i]);
			quit(buf);
		}

		/* Analyze option */
		switch (argv[i][1])
		{
			case 'x':
			case 'X':
				/* set the terminal x dim */
				screen.width = atoi(argv[i + 1]);
				i++;
				break;

			case 'y':
			case 'Y':
				/* set the terminal y dim */
				screen.height = atoi(argv[i + 1]);
				i++;
				break;

			case 'l':
			case 'L':
				SET_CFLAG(LOG_ALL);
				break;

			case 'k':
			case 'K':
				SET_CFLAG(SHOW_KEYCODES);
				break;

			case 'a':
			case 'A':
				SET_CFLAG(USE_ALPHA);
				break;

			case 'f':
			case 'F':
				SET_CFLAG(DO_FULLSCREEN);
 				break;

			case 'r':
			case 'R':
				refresh_rate =  atoi(argv[i + 1]);

				if (refresh_rate < DEFAULT_REFRESH_RATE)
					refresh_rate = DEFAULT_REFRESH_RATE;

				i++;
 				break;

			case 'i':
				key_interval =  atoi(argv[i + 1]);

				if (key_interval < DEFAULT_KEY_INTERVAL)
					key_interval = DEFAULT_KEY_INTERVAL;

				i++;
				break;

			case 'g':
		case 'G':
				if (!strcmp(argv[i + 1], "0"))
				{
					gfx.turn_off = 1;
					use_graphics = FALSE;
					arg_graphics = FALSE;
				}
				else if (!strcmp(argv[i + 1], "8"))
				{
					tile_size = GRAPHICS_8X8;
				}
				else if (!strcmp(argv[i + 1], "16"))
				{
					tile_size = GRAPHICS_16X16;
					use_graphics = TRUE;
					arg_graphics = TRUE;
				}
				else if (!strcmp(argv[i + 1], "32"))
				{
					tile_size = GRAPHICS_32X32;
				}
				else
				{
					tile_size = GRAPHICS_NONE;
				}
				i++;
				break;

			case 'u':
			case 'U':
				keypad_autorun = 1;
				break;

			default:
				snprintf(buf, MAX_ARG_LEN, "Unknown argument found [-- %s]", argv[i]);
    			quit(buf);
				break;
		}
		i++;
	}
}

void sdl_cleanup(void)
{
	s32b i = 0;
	sdl_glyph_quit_ttf();

	if (handlesurf)
	{
		SDL_FreeSurface(handlesurf);
		handlesurf = NULL;
	}

	if (tempsurf)
	{
		SDL_FreeSurface(tempsurf);
		tempsurf = NULL;
	}

	if (backsurf)
	{
		SDL_FreeSurface(backsurf);
		backsurf = NULL;
	}

	if (tilesurf)
	{
		SDL_FreeSurface(tilesurf);
		tilesurf = NULL;
	}

	/* release the terminal surfaces */
	while (i < MAX_TERM_DATA)
	{
		sdl_glyph_free_text(data[i].vp.glyph);

		if (data[i].vp.ccpy)
		{
			free(data[i].vp.ccpy);
			data[i].vp.ccpy = 0;
		}

		if (data[i].vp.acpy)
		{
			free(data[i].vp.acpy);
			data[i].vp.acpy = 0;
		}

		if (data[i].vp.surface)
		{
			SDL_FreeSurface(data[i].vp.surface);
			data[i].vp.surface = 0;
		}

		if (data[i].vp.textsurf)
		{
			SDL_FreeSurface(data[i].vp.textsurf);
			data[i].vp.textsurf = 0;
		}

		if (data[i].vp.colorsurf)
		{
			SDL_FreeSurface(data[i].vp.colorsurf);
			data[i].vp.colorsurf = 0;
		}

		if (data[i].vp.cursurf)
		{
			SDL_FreeSurface(data[i].vp.cursurf);
			data[i].vp.cursurf = 0;
		}

		i++;
	}

	sdl_log_stop("Quitting SDL ToME");
}

#define APPEND_PATH		TRUE
#define PREPEND_PATH	FALSE

void sdl_init_tome_dirs()
{
	cptr base_dir;
	cptr prev_write_dir;

	/*
	 * make the game's file paths
	 */

	/*
	 * Create the log, conf and glyph directory for the tome core
	 * if needed and set the write path. These are independant from
	 * the individual module configurations.
	 */

	prev_write_dir = string_make(PHYSFS_getWriteDir());

	get_lua_var("TOME_WRITE_ROOT", 's', &base_dir);
	PHYSFS_setWriteDir(base_dir);
	PHYSFS_addToSearchPath(base_dir, FALSE);

	PHYSFS_mkdir(PRIVATE_USER_PATH "/log/");

	SDL_LOG_DIR = PHYSFS_getDependentPath(PRIVATE_USER_PATH "/log/");

	PHYSFS_addToSearchPath(SDL_LOG_DIR, APPEND_PATH);

	PHYSFS_setWriteDir(prev_write_dir);
	PHYSFS_removeFromSearchPath(base_dir);

	/* Build the font path */
	SDL_FONT_DIR = string_make(TENGINE_DIR_FONT);
}

#define S_INIT_TENGINE_DIRS				0
#define S_INIT_SDL						1
#define S_INIT_COLORS					2
#define S_INIT_LOAD_INI_FILES			3
#define S_INIT_CREATE_SCREEN			4
#define S_INIT_LOAD_HANDLE				5
#define S_INIT_DATA_LINK				6
#define S_INIT_EXIT						7

#define S_INIT_FIRST_STATE				S_INIT_TENGINE_DIRS

void sdl_init_core(char* path)
{
	s32b state = S_INIT_FIRST_STATE;

	/* set up the key entry parameters */
	SET_CFLAG(WAIT_KEY_DOWN);

	while (state != S_INIT_EXIT && errlvl != ERRLVL_CRITICAL)
	{
		switch (state)
		{
			case S_INIT_TENGINE_DIRS:
				sdl_init_tome_dirs(path);
				break;

			case S_INIT_SDL:
				sdl_log_start("Starting SDL-ToME");
				sdl_init_sdl_components();
				break;

			case S_INIT_COLORS:
				sdl_init_colors();
				break;

			case S_INIT_LOAD_INI_FILES:
				sdl_load_ini_files();
				break;

			case S_INIT_CREATE_SCREEN:
				sdl_init_term_window();
				break;

			case S_INIT_LOAD_HANDLE:
				sdl_load_handle();
				break;

			case S_INIT_DATA_LINK:
				term_data_link();
				sdl_utl_z_pos_sort();
				break;

			case S_INIT_EXIT:
				/* we should never get here */
			default:
				/* terminate the application */
				sdl_log_error("Invalid state [%.2x] reached in init_core()", state);
				break;
		}
		state++;
	}
}

#define S_PROC_ARGS			1
#define S_INIT_CORE			2
#define S_TERM_LOOP			3
#define S_CLEANUP			4
#define S_EXIT				5

s32b sdl_start(s32b argc, char** argv)
{
	s32b state = S_PROC_ARGS;

	errlvl = ERRLVL_NO_ERR;
	screen.width = 0;
	screen.height = 0;
	num_terms = 1;
	refresh_rate = DEFAULT_REFRESH_RATE;
	key_interval = DEFAULT_KEY_INTERVAL;
	init_complete = FALSE;
	draw_immediate = FALSE;
	gfx.turn_off = FALSE;
	keypad_autorun = 0;

	while (state != S_EXIT && errlvl != ERRLVL_CRITICAL)
	{
		errlvl = ERRLVL_NO_ERR;

		switch (state)
		{
			case S_PROC_ARGS:
				sdl_process_args(argc, argv);
#ifdef __APPLE__
				// XXX Hack Hack Hack
				// Until TTF_RenderGlyph_Solid is fixed on my system, we
				// need to force alpha -- Neil

				SET_CFLAG(USE_ALPHA);
#endif
				break;

			case S_INIT_CORE:
				sdl_init_core(argv[0]);
				state = S_EXIT;
				continue;

			case S_EXIT:
				/* we should never get here */
			default:
				/* terminate the application */
				sdl_log_error("Invalid state [%.2x] reached in main()", state);
				break;
		}
		state++;
	}

	if (errlvl == ERRLVL_CRITICAL)
	{
		sdl_cleanup();
		return (1);
	}

	atexit(sdl_glyph_save_parameters);
	atexit(sdl_cleanup);
	atexit(SDL_Quit);

	return (0);
}

#endif /* USE_SDL */
