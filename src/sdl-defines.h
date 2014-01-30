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
* Hard tabs at 4
*******************************************************************************
*/

#ifndef __SDL_DEFINES_H
#define __SDL_DEFINES_H

/*
 * Number of "term_data" structures to support XXX XXX XXX
 *
 * You MUST support at least one "term_data" structure, and the
 * game will currently use up to eight "term_data" structures if
 * they are available.
 *
 * If only one "term_data" structure is supported, then a lot of
 * the things that would normally go into a "term_data" structure
 * could be made into global variables instead.
 */
#define MAX_TERM_DATA					8

#define SDL_OK							0

#ifndef TRUE
#define TRUE							-1
#endif

#ifndef FALSE
#define FALSE							0
#endif

#define DEFAULT_REFRESH_RATE			16	/* in ms - 16ms = 62.5 frames per second */
#define DEFAULT_KEY_INTERVAL			25

/* window constants */
#define BORDER							1
#define INSIDE_BORDER					2

/* default grab handle size */
#define GRAB_HANDLE_WIDTH				20
#define GRAB_HANDLE_HEIGHT				20

/* maximum command line argument length for each argument */
#define MAX_ARG_LEN						80

/* files and directories */
#define GLYPH_DIR						"glyph"
#define FONT_DIR						"font"
#define ICON_DIR						"icon"
#define SDL_LOG_FILE					"sdl-log.txt"
#define SDL_INI_FILE					"sdl-tome.ini"
#define SDL_FONTSET_INI_FILE			"sdl-fonts.ini"
#define MOD_INI_FILE					"sdl-mod.ini"
#define MOD_FONTSET_FILE				"sdl-mod_fonts.ini"
#define GLYPH_FILE_FMT					"set%i-%.2x.bmp"
#define SDL_HANDLE_FILE					"handle.bmp"

#define REND_UNKNOWN					0
#define REND_SOLID						1
#define REND_BLEND						2

#define NO_ALPHA						0

#define MAX_COLORS						256

/* ini file setting size limits - these will go away eventually */
#define MAX_SECTION_LEN		32
#define MAX_KEY_LEN			32
#define MAX_VALUE_LEN		256
#define MAX_LINE_LEN		MAX_SECTION_LEN + MAX_KEY_LEN + MAX_VALUE_LEN + 1

/*
 * default font parameters (font_param)
 * note that max_h, max_w, vert_advance and horz_advance are set after
 * the font is loaded
 */
#define DEFAULT_FONT					"VeraMono.ttf"
#define DEFAULT_FONT_WIDTH				8
#define DEFAULT_FONT_HEIGHT				13
#define DEFAULT_TTF_FONT_SIZE			10
#define FONT_PARAM_FIRST_CH				0
#define FONT_PARAM_LAST_CH				0x7f
#define FONT_PARAM_FG_R					255
#define FONT_PARAM_FG_G					255
#define FONT_PARAM_FG_B					255
#define FONT_PARAM_BG_R					0
#define FONT_PARAM_BG_G					0
#define FONT_PARAM_BG_B					0

#define BORDER_COLOR_R					64
#define BORDER_COLOR_G					64
#define BORDER_COLOR_B					64

/* error levels and app states */
#define ERRLVL_NO_ERR					(1<<0)
#define ERRLVL_CRITICAL					(1<<1)
#define ERRLVL_WARNING					(1<<2)
#define LOG_START						(1<<3)
#define LOG_STOP						(1<<4)

/* file constants */
#define MAX_PATH_LEN					512

/* glyph constants */
#define MAX_GLYPHS						255

/* SDL constants */
#define ENTIRE_SURFACE					NULL
#define BPP_AUTO						0

/* client flags (up to 32 flags) */
#define NET_INIT_DONE					(1<<0)
#define FLASH_ENABLE					(1<<1)
#define FLASH_STATE						(1<<2)
#define UPDATE							(1<<3)
#define WAIT_KEY_UP						(1<<4)
#define WAIT_KEY_DOWN					(1<<5)
#define GET_RESPONSE					(1<<6)
#define HAVE_RESPONSE					(1<<7)
#define USE_CURSOR						(1<<8)
#define PARSE_COLORS					(1<<9)
#define LOG_ALL							(1<<10)
#define DISPLAY_ERROR_MSG				(1<<11)
#define SHOW_KEYCODES					(1<<12)
#define USE_ALPHA						(1<<13)
#define DO_FULLSCREEN					(1<<14)
#define IN_TERM_DRAW_OP					(1<<15)

/* custom events */
#define EVENT_REFRESH_VP				(1<<0)
#define EVENT_GAME_RT_TIMER				(1<<1)
#define EVENT_GAME_SOUND_CHANNEL_STOP	(1<<2)
#define EVENT_GAME_MUSIC_STOP			(1<<3)

/* macros for client_flags */
#define SET_CFLAG(n)					(client_flags |= n)
#define CLR_CFLAG(n)					(client_flags &= ~n)
#define TST_CFLAG(n)					(client_flags & n)

/* various macros */
#define UPDATE_RECT(surf, rect)			SDL_UpdateRects(surf, 1, &rect)
#define UPDATE_VIEWPORT(surf)			SDL_UpdateRect(surf, 0, 0, 0, 0)
#define UPDATE_SCREEN					UPDATE_VIEWPORT(screen.surface)
#define REDRAW_VP(i,src,dest)			SDL_BlitSurface(data[i].vp.surface, src, screen.surface, dest)


#endif	// __SDL_DEFINES_H


