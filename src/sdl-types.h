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

#ifndef __SDL_TYPES_H
#define __SDL_TYPES_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "sdl-defines.h"

#include "angband.h"

typedef struct term_data term_data;

typedef struct
{
	SDL_Surface*			surface;
	SDL_Surface*			bgsurf;
	s32b						width;
	s32b						height;
} screen_t;

// font data type
typedef struct
{
   Uint16 first_ch;					// first character in our font
   Uint16 last_ch;					// last character in our font
   SDL_Color fg;						// foreground color
   SDL_Color bg;						// background color
} font_param_t;

typedef struct
{
	SDL_Color color;
	Uint8 tag;
} color_t;

typedef struct
{
	Sint16	x;
	Sint16	y;
} pos_t;

typedef struct
{
	Uint16		w;
	Uint16		h;
} dim_t;

typedef struct
{
	Uint16	left;
	Uint16	right;
	Uint16	top;
	Uint16	bottom;
} resize_frame_t;

typedef struct
{
	s32b	id;
	char	path[MAX_PATH_LEN];
} crit_files_t;

typedef struct
{
	SDL_Rect		content;			// content area
	SDL_Rect		titlebar;		// titlebar area
	SDL_Rect		titletext;		// titlebar text area
	SDL_Rect		handle;			// grab handle area
} area_t;

typedef struct
{
	bool	enabled;
	bool	turn_off;
	Sint8	tilesize;
	cptr	filename;
	dim_t	tile;
} gfx_t;

typedef struct
{
	uint			id;				// id (Term-n)
	dim_t			termsize;		// terminal size (cols, rows)
	s32b				z_pos;			// z-order
	SDL_Rect		rect;			// surface position and text area (pixels)
	area_t			area;			// various viewport areas
	bool			visible;		// visible status
	bool			bordervis;		// border visibility (on, off)
	s32b				fontset;		// fontset
	cptr			font;			// font file name
	s32b				rendmode;		// font rendering mode
	s32b				fontsize;		// fontsize (for TTF Fonts)
	dim_t			fontdim;		// base font size (pixels)
	resize_frame_t	rframe;			// resize frame area
	bool			in_edit;		// being modified status
	bool			refresh;		// ready for refresh flag
	bool			current_handle;	// handle visibility flag
	cptr			name;			// name
	cptr			altname;		// user defined alternate name
	char*			ccpy;			// term char copy
	char*			acpy;			// term attr copy
	SDL_Color		fg;				// character fore color
	SDL_Color		bg;				// character back color
	SDL_Surface*	surface;		// content surface
	SDL_Surface*	textsurf;		// text scratch pad
	SDL_Surface*	colorsurf;		// color pad
	SDL_Surface*	cursurf;		// cursor glyph
	SDL_Surface*	glyph[255];		// character glyphs
} viewport_t;

/*
 * Extra data to associate with each "window"
 *
 * Each "window" is represented by a "term_data" structure, which
 * contains a "term" structure, which contains a pointer (t->data)
 * back to the term_data structure.
 */
struct term_data
{
	term t;

	viewport_t		vp;
};

typedef struct
{
	s32b				id;			// fontset id
	char			filename;	// font filename
	SDL_Surface*	glyph[256];	// glyph with width and height info
} t_fontset;

typedef struct
{
	SDL_Rect	dim;
	Sint8		BarHeight;
	SDL_Color 	BorderColor;
	SDL_Color 	BarColor;
	SDL_Color 	WindowColor;
} dialog_t;

#endif // __SDL_TYPES_H

