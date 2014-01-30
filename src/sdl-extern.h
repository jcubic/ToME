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

#ifndef __SDL_EXTERN_H
#define __SDL_EXTERN_H

#include "SDL.h"
#include "sdl-types.h"
#include "sdl-defines.h"
#include "angband.h"

/*
 * Global Vars
 */

extern screen_t			screen;
extern font_param_t		font_info;
extern SDL_Color		color_table[MAX_COLORS];
extern SDL_Color		border_color;
extern Uint8			errlvl;
extern s32b				client_flags;
extern s32b				max_terms;
extern SDL_Surface*		handlesurf;
extern SDL_Surface*		backsurf;
extern SDL_Surface*		tempsurf;
extern unsigned short	do_redraw;
extern s32b				num_terms;
extern s32b				vp_z_order[MAX_TERM_DATA];
extern s32b				refresh_rate;
extern void*			vp_update_params;
extern bool 			rect_changed;
extern bool				init_complete;
extern gfx_t			gfx;
extern SDL_TimerID		refresh_timer_id;
extern SDL_Rect			refresh_area[MAX_TERM_DATA];
extern SDL_Rect			refresh_area_max[MAX_TERM_DATA];
extern s32b				draw_immediate;
extern s32b				draw_immediate_delay;
extern SDL_Surface*		tilesurf;         // tile image map surface

/* sdl-glyph.c */
extern s32b 				fontsets_loaded[MAX_TERM_DATA];

/*
 * An array of "term_data" structures, one for each "sub-window"
 */
extern term_data data[MAX_TERM_DATA];

/* core files and directories */
extern cptr SDL_LOG_DIR;				// <userdir>/.tome/log/
extern cptr SDL_ICON_FILE;   			// $LIBDIR/game/media/graf/icon.png
extern cptr SDL_FONT_DIR;				// $LIBDIR/game/media/font/

/*
 * Prototypes
 */

/* sdl-glyph.c */
extern s32b				sdl_glyph_init_ttf(void);
extern void				sdl_glyph_quit_ttf(void);
extern void				sdl_glyph_free_text(SDL_Surface** text);
extern void				sdl_glyph_load_params(term_data* td, s32b i);
extern s32b				sdl_glyph_load_surfaces(term_data* td);
extern s32b				sdl_glyphs_verify(term_data* td);
extern s32b 				sdl_glyph_make_cursor(term_data* td);
extern void				sdl_glyphs_create(term_data* td);
extern void				sdl_glyph_save_parameters(void);

/* sdl-log.c */
extern void				sdl_log_error(char* fmt, ...);
extern void				sdl_log_warn(char* fmt, ...);
extern void				sdl_log_info(char* fmt, ...);
extern void				sdl_log_start(char* fmt, ...);
extern void				sdl_log_stop(char* fmt, ...);

/* sdl-utility.c */
extern s32b				sdl_strlcpy(char* dest, char* src, s32b len);
extern s32b				sdl_strlcat(char* dest, char* src, s32b len);
extern void				sdl_strtrim(char* buf, char* line);
extern void				sdl_strmid(char* buf, char* line, s32b start, s32b len);
extern void				sdl_strtoupper(char* result, char* src, s32b len);
extern void				sdl_strtolower(char* result, char* src, s32b len);
extern void				sdl_utl_make_vp_borders(void);
extern void				sdl_utl_make_vp_border(term_data* td);
extern void				sdl_utl_find_files(char* dir, char* ftype);
extern errr				sdl_utl_dir_verify(const char* dir, s32b error_lvl);
extern errr				sdl_utl_file_verify(const char* filename, s32b error_lvl);
extern void				sdl_utl_z_pos_sort(void);
extern void				sdl_utl_reorder_z_pos(term_data* td);
extern void				sdl_utl_redraw_viewports(void);
extern void				sdl_utl_add_rect(s32b vp_id, SDL_Rect* source);
extern void				sdl_utl_force_refresh_all(void);
extern void				sdl_utl_clear_rect(term_data* td, s32b x, s32b y, s32b n);
extern void				sdl_utl_clr_vp_from_screen(term_data* td);
extern void				sdl_utl_fill_rect(term_data* td, SDL_Rect* destrect, Uint8 r, Uint8 g, Uint8 b);
extern void				sdl_utl_rect_cp(SDL_Rect* new_rect, SDL_Rect* orig_rect);
extern SDL_Surface*		resize_tiles(SDL_Surface* img_surf, s32b ox, s32b oy);
extern SDL_Surface*		sdl_utl_create_surface(Uint32 flags, s32b width, s32b height, s32b bpp, s32b rendmode);
extern void				sdl_putstr(term_data* td, s32b x, s32b y, s32b n, byte a, const char* cp);
extern void				sdl_set_title(term_data* td, const char* title);
extern void 			sdl_utl_update_row(term_data* td, s32b y);
extern void				sdl_utl_color_copy(SDL_Color* dest, SDL_Color* src);
extern void				sdl_utl_color_set(Uint8 tag, Uint8 r, Uint8 g, Uint8 b);
extern void				sdl_utl_clear_content(term_data* td);
extern void				sdl_utl_refresh_term(SDL_Surface* dst_surface);
extern void  			sdl_utl_get_datetime(char* datetime);
extern void				sdl_utl_refresh_viewport(term_data* td);
extern void				sdl_utl_update_screen(void);
extern void				sdl_utl_update_rect(SDL_Surface* dst_surface, SDL_Rect* dest_rect);
extern void				sdl_utl_calc_vp_coords(term_data* td, s32b origin_x, s32b origin_y);
extern void				sdl_utl_make_vp_from_text_copy(term_data* td, SDL_Surface** destsurf);
extern void				sdl_utl_save_term(term_data* td);
extern void 			sdl_putrawstr(term_data* td, s32b x, s32b y, s32b n, const char* cp);

/* sdl-events.c */
extern s32b				sdl_event_handler(SDL_Event* event);

/* main-sdl.c */
extern void				sdl_init_term_window(void);
extern void				ext_term_init_sdl(term* t);
extern SDL_TimerID		sdl_create_timer(Uint32 interval, SDL_NewTimerCallback callback_func, void* update_params);
extern void 			sdl_remove_timer(SDL_TimerID* timer);
extern Uint32			refresh_callback(Uint32 interval, void *param);
extern Uint32			vp_refresh_callback(Uint32 interval, void *param);

#endif // __SDL_EXTERN_H
