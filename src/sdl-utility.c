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

#include "physfs.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <SDL_ttf.h>
#include <SDL_endian.h>

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include "sdl-types.h"
#include "sdl-defines.h"
#include "sdl-extern.h"

s32b	draw_immediate;
s32b	draw_immediate_delay;

#ifdef USE_SDL

#ifdef WIN32
#define snprintf	_snprintf
#endif

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define SWAP16(X)    (X)
#define SWAP32(X)    (X)
#else
#define SWAP16(X)    SDL_Swap16(X)
#define SWAP32(X)    SDL_Swap32(X)
#endif

bool rect_changed;

/*
 * String routines
 */

/* safe strcpy and strcat ops */

s32b sdl_strlcpy(char* dest, char* src, s32b len)
{
	s32b idx = 0;
	s32b srclen = strlen(src);
	char* srcaddr = src;
	char* dstaddr = dest;

	/* always leave room for the terminating null */
	if (srclen >= len)
		srclen = len - 1;

	while (*srcaddr && idx < srclen)
	{
		*dstaddr++ = *srcaddr++;
		idx++;
	}
	*dstaddr = '\0';

	return idx;
}

s32b sdl_strlcat(char* dest, char* src, s32b len)
{
	s32b idx = 0;
	s32b l = strlen(src) + strlen(dest);
	char* srcaddr = src;
	char* dstaddr = dest;

	/* always leave room for the terminating null */
	if (l >= len)
		l = len - 1;

	while (l && *dstaddr)
	{
		l--;
		dstaddr++;
	}

	while (*srcaddr && idx <= l && l)
	{
		*dstaddr++ = *srcaddr++;
		idx++;
	}
	*dstaddr = '\0';

	return idx;
}

void sdl_strmid(char* buf, char* line, s32b start, s32b len)
{
	s32b i = start;
	s32b line_len = (s32b)strlen(line);
	s32b idx = 0;

	/* clear the buffer in case other parameters are invalid */
	buf[0] = '\0';

	if (start + len > line_len)
	{
		/* invalid parameter found */
		return;
	}

	/* set the value */
	while (idx < len)
	{
		buf[idx++] = line[i++];
	}

	buf[idx] = '\0';
}

void sdl_strtrim(char* buf, char* line)
{
	s32b i = (s32b)strlen(line);

	while (line[i] == ' '
			  || line[i] == '\r'
			  || line[i] == '\n'
			  || line[i] == '\0')
	{
		i--;
	}

	sdl_strmid(buf, line, 0, i + 1);
}

void sdl_strtoupper(char* result, char* src, s32b len)
{
	s32b idx = 0;

	while (*src && idx < len)
	{
		if (*src >= 'a' && *src <= 'z')
		{
			*result++ = *src - 32;
		}
		else
		{
			*result++ = *src;
		}
		src++;
		idx++;
	}
	*result = '\0';
}

void sdl_strtolower(char* result, char* src, s32b len)
{
	s32b idx = 0;

	while (*src && idx < len)
	{
		if (*src >= 'A' && *src <= 'Z')
		{
			*result++ = *src + 32;
		}
		else
		{
			*result++ = *src;
		}
		src++;
		idx++;
	}
	*result = '\0';
}

/*
 * File and directory routines
 */

errr sdl_utl_dir_verify(const char* dir, s32b error_lvl)
{
	 if (PHYSFS_isDirectory(dir))
	 {
		  return ERRLVL_NO_ERR;
	 }

	 return error_lvl;
}

errr sdl_utl_file_verify(const char* filename, s32b error_lvl)
{
	 if (PHYSFS_exists(filename))
	 {
		  return ERRLVL_NO_ERR;
	 }

	 return error_lvl;
}

/*
 * Date and time routines
 */

void sdl_utl_get_datetime(char* datetime)
{
	time_t timeval;
	struct tm *structtime;

	if (datetime)
	{
		(void)time(&timeval);
		structtime = localtime(&timeval);

		sprintf(datetime, "%.2i-%.2i-%i %.2i:%.2i:%.2i",
				  structtime->tm_mday,
				  structtime->tm_mon + 1,
				  structtime->tm_year + 1900,
				  structtime->tm_hour,
				  structtime->tm_min,
				  structtime->tm_sec);
	}
}

/*
 * Color routines
 */

void sdl_utl_color_set(Uint8 tag, Uint8 r, Uint8 g, Uint8 b)
{
	color_table[tag].r = r;
	color_table[tag].g = g;
	color_table[tag].b = b;
	color_table[tag].unused = 0xff;
}

void sdl_utl_color_copy(SDL_Color* dest, SDL_Color* src)
{
	if (dest && src)
	{
		dest->r = src->r;
		dest->g = src->g;
		dest->b = src->b;
		dest->unused = src->unused;
	}
	else
	{
		sdl_log_warn("utl_color_copy(...) called with NULL pointer");
		errlvl = ERRLVL_NO_ERR;
	}
}

/*
 * Make a single rect area to refresh
 */

void sdl_utl_add_rect(s32b vp_id, SDL_Rect* source)
{
	if (source->x < refresh_area[vp_id].x)
		refresh_area[vp_id].x = source->x;

	if (source->y < refresh_area[vp_id].y)
		refresh_area[vp_id].y = source->y;

	if (source->x > refresh_area_max[vp_id].x)
	{
		refresh_area_max[vp_id].x = source->x;
		refresh_area_max[vp_id].w = source->w;
	}

	if (source->y > refresh_area_max[vp_id].y)
	{
		refresh_area_max[vp_id].y = source->y;
		refresh_area_max[vp_id].h = source->h;
	}

	refresh_area[vp_id].w = (refresh_area_max[vp_id].x + refresh_area_max[vp_id].w) - refresh_area[vp_id].x;
	refresh_area[vp_id].h = (refresh_area_max[vp_id].y - refresh_area_max[vp_id].h) - refresh_area[vp_id].y;

	rect_changed = TRUE;
	data[vp_id].vp.refresh = TRUE;

	/*
	 * Handle fast redraws and
	 * restrict them to the main viewport
	 */
	if (draw_immediate && vp_id == 0 && !TST_CFLAG(IN_TERM_DRAW_OP))
	{
		sdl_utl_redraw_viewports();
	}
}

/* the following routine must be callable by outside interfaces */

void sdl_utl_fast_draw(bool enable)
{
	if (enable)
		draw_immediate = TRUE;
	else
		draw_immediate = FALSE;
}

/*
 * Viewport routines
 */

void sdl_utl_set_vp_size(term_data *td)
{
	td->vp.rect.w = td->vp.termsize.w * td->vp.fontdim.w;
	td->vp.rect.h = td->vp.termsize.h * td->vp.fontdim.h;
}

void sdl_utl_calc_vp_coords(term_data* td, s32b origin_x, s32b origin_y)
{
	/* set the position on the screen and the total viewport size */
	td->vp.rect.x = origin_x;
	td->vp.rect.y = origin_y;
	td->vp.rect.w = td->vp.surface->w;
	td->vp.rect.h = td->vp.surface->h;

	/* the following have zero based offsets */

	td->vp.area.titlebar.x = 0;
	td->vp.area.titlebar.y = 0;
	td->vp.area.titlebar.w = td->vp.surface->w;
	td->vp.area.titlebar.h = td->vp.fontdim.h + 2;

	td->vp.area.content.x = BORDER + td->vp.fontdim.w;
	td->vp.area.content.y = td->vp.area.titlebar.h + 2;
	td->vp.area.content.w = td->vp.termsize.w * td->vp.fontdim.w;
	td->vp.area.content.h = td->vp.termsize.h * td->vp.fontdim.h;

	td->vp.area.titletext.x = td->vp.area.content.x;
	td->vp.area.titletext.y = td->vp.area.titlebar.y + 1;
	td->vp.area.titletext.w = td->vp.area.content.w;
	td->vp.area.titletext.h = td->vp.fontdim.h;

	td->vp.area.handle.x = td->vp.rect.w - (handlesurf->w + BORDER);
	td->vp.area.handle.y = td->vp.rect.h - (handlesurf->h + BORDER);
	td->vp.area.handle.w = handlesurf->w;
	td->vp.area.handle.h = handlesurf->h;
}

void sdl_utl_rect_cp(SDL_Rect* new_rect, SDL_Rect* orig_rect)
{
	new_rect->x = orig_rect->x;
	new_rect->y = orig_rect->y;
	new_rect->w = orig_rect->w;
	new_rect->h = orig_rect->h;
}

/* clear a rect in the viewport and screen content area */
void sdl_utl_clear_rect(term_data* td, s32b x, s32b y, s32b n)
{
	SDL_Rect vp_rect;
	SDL_Rect scr_rect;

	if (td->vp.surface)
	{
		vp_rect.x = td->vp.area.content.x + td->vp.fontdim.w * x;
		vp_rect.y = td->vp.area.content.y + td->vp.fontdim.h * y;
		vp_rect.w = td->vp.fontdim.w * n;
		vp_rect.h = td->vp.fontdim.h;

		if (SDL_MUSTLOCK(td->vp.surface))
		{
			SDL_LockSurface(td->vp.surface);
		}

		SDL_FillRect(td->vp.surface, &vp_rect,
						 SDL_MapRGB(td->vp.surface->format,
										 td->vp.bg.r,
										 td->vp.bg.g,
										 td->vp.bg.b));

		if (SDL_MUSTLOCK(td->vp.surface))
		{
			SDL_UnlockSurface(td->vp.surface);
		}

		scr_rect.x = vp_rect.x + td->vp.rect.x;
		scr_rect.y = vp_rect.y + td->vp.rect.y;
		scr_rect.w = vp_rect.w;
		scr_rect.h = vp_rect.h;

		sdl_utl_add_rect(td->vp.id, &scr_rect);

	}
}

/* clear the content area of viewport */
void sdl_utl_clear_content(term_data* td)
{
	SDL_Rect win_rect;

	win_rect.x = BORDER;
	win_rect.y = td->vp.area.titlebar.h;
	win_rect.w = td->vp.termsize.w * td->vp.fontdim.w + td->vp.fontdim.w;
	win_rect.h = td->vp.termsize.h * td->vp.fontdim.h + td->vp.fontdim.h - 2;

	if (td->vp.surface)
	{
		if (SDL_MUSTLOCK(td->vp.surface))
		{
			SDL_LockSurface(td->vp.surface);
		}

		SDL_FillRect(td->vp.surface, &win_rect,
						 SDL_MapRGB(td->vp.surface->format,
										 td->vp.bg.r,
										 td->vp.bg.g,
										 td->vp.bg.b));

		if (SDL_MUSTLOCK(td->vp.surface))
		{
			SDL_UnlockSurface(td->vp.surface);
		}

		//td->vp.refresh = TRUE;
	}
}

/* clear an entire viewport from the screen */
void sdl_utl_clr_vp_from_screen(term_data* td)
{
	if (td->vp.visible)
	{
		if (SDL_MUSTLOCK(screen.surface))
		{
			SDL_LockSurface(screen.surface);
		}

		SDL_FillRect(screen.surface, &td->vp.rect,
						 SDL_MapRGB(screen.surface->format,
										 td->vp.bg.r,
										 td->vp.bg.g,
										 td->vp.bg.b));

		if (SDL_MUSTLOCK(screen.surface))
		{
			SDL_UnlockSurface(screen.surface);
		}

		UPDATE_SCREEN;
	}
}

/*
 * Make a border around a viewport with the proper
 * border color based on it's in-edit status.
 *
 * Fills are only 1px wide for speed
 */

void sdl_utl_make_vp_border(term_data* td)
{
	SDL_Rect destrect;
	SDL_Color color;

	if (td->vp.surface == NULL)
	{
		return;
	}

	if (td->vp.in_edit)
	{
		sdl_utl_color_copy(&color, &color_table[TERM_YELLOW]);
	}
	else
	{
		sdl_utl_color_copy(&color, &border_color);
	}

	if (SDL_MUSTLOCK(td->vp.surface))
	{
		SDL_LockSurface(td->vp.surface);
	}

	/* top */
	SDL_FillRect(td->vp.surface, &td->vp.area.titlebar,
						SDL_MapRGB(td->vp.surface->format,
										 border_color.r,
										 border_color.g,
										 border_color.b));

	/* bottom */
	destrect.x = 0;
	destrect.y = td->vp.rect.h - 1;
	destrect.w = td->vp.rect.w;
	destrect.h = 1;
	SDL_FillRect(td->vp.surface, &destrect,
						SDL_MapRGB(td->vp.surface->format,
										color.r,
										color.g,
										color.b));

	/* left */
	destrect.y = 0;
	destrect.w = 1;
	destrect.h = td->vp.rect.h - 1;
	SDL_FillRect(td->vp.surface, &destrect,
						SDL_MapRGB(td->vp.surface->format,
										color.r,
										color.g,
										color.b));

	/* right */
	destrect.x = td->vp.rect.w - 1;
	SDL_FillRect(td->vp.surface, &destrect,
						SDL_MapRGB(td->vp.surface->format,
										color.r,
										color.g,
										color.b));

	if (SDL_MUSTLOCK(td->vp.surface))
	{
		SDL_UnlockSurface(td->vp.surface);
	}

	td->vp.refresh = TRUE;
}

void sdl_utl_make_vp_borders()
{
	s32b i = 0;

	while (i < max_terms)
	{
		if (data[i].vp.visible && data[i].vp.bordervis)
		{
			sdl_utl_make_vp_border(&data[i]);
		}
		i++;
	}
}

/*
 * z-ordering code
 */

/*
 * Reset the z-position of the viewports
 */

void sdl_utl_z_pos_reset()
{
	s32b i = max_terms;

	while (--i >= 0)
	{
		vp_z_order[i] = i;
	}
}

/*
 * Pre-sort the term order for fast redraws
 *
 * This creates an ordered viewport id list based on
 * the assigned z-position from the ini file.
 */

void sdl_utl_z_pos_sort()
{
	s32b i;

	for (i=0; i<max_terms; i++)
		vp_z_order[i] = data[i].vp.z_pos;
}

/*
 * Reorder the viewports, pulling the given viewport
 * id to the top
 */

void sdl_utl_reorder_z_pos(term_data* td)
{
	s32b i;
	s32b j;

	for (i=0; i < max_terms; i++)
	{
		if (td->vp.id == vp_z_order[i])
		{
			for (j=i; j>0; j--)
			{
				vp_z_order[j] = vp_z_order[j - 1];
				data[j].vp.z_pos = vp_z_order[j];
			}

			vp_z_order[0] = td->vp.id;
			data[0].vp.z_pos = vp_z_order[0];
			sdl_utl_redraw_viewports();

			return;
		}
	}
}

/*
 * Redraw the viewports based on their z-position order
 */

void sdl_utl_redraw_viewports()
{
	s32b 		i = 0;
	s32b 		idx = 0;
	SDL_Rect	handle_rect;
	SDL_Rect	srcrect;

	for (i=max_terms-1; i>=0; i--)
	{
		if (init_complete)
			idx = vp_z_order[i];
		else
			idx = i;

		if (data[idx].vp.visible && data[idx].vp.refresh)
		{
			if (init_complete)
			{
				srcrect.x = refresh_area[idx].x - data[idx].vp.rect.x;
				srcrect.y = refresh_area[idx].y - data[idx].vp.rect.y;
				srcrect.w = refresh_area[idx].w;
				srcrect.h = refresh_area[idx].h;

				REDRAW_VP(idx, &srcrect, &refresh_area[idx]);
			}
			else
			{
				REDRAW_VP(idx, ENTIRE_SURFACE, &data[idx].vp.rect);
			}

			/* deal with a resize handle that may have been erased by the preceeding blit call */
			if (data[idx].vp.current_handle)
			{
				handle_rect.x = data[idx].vp.rect.x + data[idx].vp.area.handle.x;
				handle_rect.y = data[idx].vp.rect.y + data[idx].vp.area.handle.y;
				handle_rect.w = data[idx].vp.area.handle.w;
				handle_rect.h = data[idx].vp.area.handle.h;

				SDL_BlitSurface(handlesurf, ENTIRE_SURFACE, screen.surface, &handle_rect);
			}

			data[idx].vp.refresh = FALSE;
		}
	}

	if (init_complete)
		SDL_UpdateRects(screen.surface, MAX_TERM_DATA, refresh_area);
	else
		UPDATE_SCREEN;

	i = 0;
	while (i < max_terms)
	{
		refresh_area[i].x = data[i].vp.rect.x + data[i].vp.rect.w;
		refresh_area[i].y = data[i].vp.rect.y + data[i].vp.rect.h;
		refresh_area[i].w = 0;
		refresh_area[i].h = 0;

		refresh_area_max[i].x = data[i].vp.rect.x + data[i].vp.rect.w;
		refresh_area_max[i].y = data[i].vp.rect.y + data[i].vp.rect.h;
		refresh_area_max[i].w = 0;
		refresh_area_max[i].h = 0;

		i++;
	}
}

void sdl_utl_force_refresh_all()
{
	s32b i, idx;

	/* force all visible viewports to refresh */
	for (i=max_terms-1; i>=0; i--)
	{
		idx = vp_z_order[i];

		if (data[idx].vp.visible)
			REDRAW_VP(idx, ENTIRE_SURFACE, &data[idx].vp.rect);
	}
	UPDATE_SCREEN;
}

/* pixel processing */

Uint32 getpixel(SDL_Surface *surface, s32b x, s32b y)
{
	s32b bpp = surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void putpixel(SDL_Surface *surface, s32b x, s32b y, Uint32 pixel)
{
	s32b bpp = surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}


/*
 * rough image resizing - code ripped from maid-x11.c
 */
SDL_Surface* resize_tiles(SDL_Surface* img_surf, s32b ox, s32b oy)
{
	s32b width1, height1, width2, height2;
	s32b ix, iy;
	s32b x1, x2, y1, y2, Tx, Ty;
	s32b *px1, *px2, *dx1, *dx2;
	s32b *py1, *py2, *dy1, *dy2;

	SDL_Surface* tile_temp;
	SDL_Surface* tile_srf;

	/*
	if (smoothRescaling && (ix != ox || iy != oy) &&
		   visual->class == TrueColor)
	{
		return ResizeImageSmooth(dpy, Im, ix, iy, ox, oy);
	}
	*/

	ix = gfx.tile.w;
	iy = gfx.tile.h;

	width1 = img_surf->w;
	height1 = img_surf->h;

	width2 = ox * width1 / ix;
	height2 = oy * height1 / iy;
	tile_srf = sdl_utl_create_surface(0, width2, height2, img_surf->format->BitsPerPixel, 7);

	if (tile_srf)
	{
		tile_temp = SDL_ConvertSurface(tile_srf, img_surf->format, img_surf->flags);

		SDL_FreeSurface(tile_srf);
	}

	if (!tile_srf || !tile_temp)
		return NULL;

	if (ix > ox)
	{
		px1 = &x1;
		px2 = &x2;
		dx1 = &ix;
		dx2 = &ox;
	}
	else
	{
		px1 = &x2;
		px2 = &x1;
		dx1 = &ox;
		dx2 = &ix;
	}

	if (iy > oy)
	{
		py1 = &y1;
		py2 = &y2;
		dy1 = &iy;
		dy2 = &oy;
	}
	else
	{
		py1 = &y2;
		py2 = &y1;
		dy1 = &oy;
		dy2 = &iy;
	}

	if (SDL_MUSTLOCK(tile_temp))
	{
		SDL_LockSurface(tile_temp);
	}

	Ty = *dy1 / 2;

	for (y1 = 0, y2 = 0; (y1 < height1) && (y2 < height2); )
	{
		Tx = *dx1 / 2;

		for (x1 = 0, x2 = 0; (x1 < width1) && (x2 < width2); )
		{
			putpixel(tile_temp, x2, y2, getpixel(img_surf, x1, y1));

			(*px1)++;

			Tx -= *dx2;
			if (Tx < 0)
			{
				Tx += *dx1;
				(*px2)++;
			}
		}

		(*py1)++;

		Ty -= *dy2;
		if (Ty < 0)
		{
			Ty += *dy1;
			(*py2)++;
		}
	}

	if (SDL_MUSTLOCK(tile_temp))
	{
		SDL_UnlockSurface(tile_temp);
	}

	return tile_temp;
}

SDL_Surface* sdl_utl_create_surface(Uint32 flags, s32b width, s32b height, s32b bpp, s32b rendmode)
{
	const SDL_VideoInfo* video_info;
	SDL_Surface* surface;
	Uint8 surf_bpp;

	if (rendmode == REND_SOLID)
	{
		surface = SDL_CreateRGBSurface(screen.surface->flags,
												width, height,
												screen.surface->format->BitsPerPixel,
												screen.surface->format->Rmask,
												screen.surface->format->Gmask,
												screen.surface->format->Bmask,
												screen.surface->format->Amask);
	}
	else
	{
		video_info = SDL_GetVideoInfo();

		if (video_info->hw_available)
		{
			flags |= SDL_HWSURFACE;
		}
		else
		{
			flags |= SDL_SWSURFACE;
		}

		if (bpp == BPP_AUTO)
		{
			surf_bpp = screen.surface->format->BitsPerPixel;
		}
		else
		{
			surf_bpp = bpp;
		}

		switch (surf_bpp)
		{
			case 8:
				surface = SDL_CreateRGBSurface(flags, width, height, 8, 0xc0, 0x30, 0x0c, 0x03);
				break;

			case 16:
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				surface = SDL_CreateRGBSurface(flags, width, height, 16, 0xf000, 0x0f00, 0x00f0, 0x000f);
#else
				surface = SDL_CreateRGBSurface(flags, width, height, 16, 0x000f, 0x00f0, 0x0f00, 0xf000);
#endif
				break;

			case 24:
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				surface = SDL_CreateRGBSurface(flags, width, height, 24, 0xfc0000, 0x03f000, 0x000fc0, 0x00003f);
#else
				surface = SDL_CreateRGBSurface(flags, width, height, 24, 0x00003f, 0x000fc0, 0x03f000, 0xfc0000);
#endif
				break;

			case 32:
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				surface = SDL_CreateRGBSurface(flags, width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
				surface = SDL_CreateRGBSurface(flags, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
				break;

			default:
				surface = NULL;
				break;
		}
	}

	return surface;
}

void sdl_set_title(term_data* td, const char* title)
{
	SDL_Rect		destrect;
	SDL_Rect		scratch_area;
   s32b				i = 0;
	s32b				len;
	char			last_ch = 0;
	s32b				color;
	char			vp_title[32];

	snprintf(vp_title, 32, " %s ", title);
	len = strlen(vp_title);

	if (td->vp.surface == NULL)
	{
		/* terminate the application */
		sdl_log_error("sdl_set_title() called when no surface is available");
	}

   	sdl_utl_color_copy(&td->vp.fg, &color_table[TERM_RED]);

	color = SDL_MapRGBA(td->vp.colorsurf->format,
							 td->vp.fg.r,
							 td->vp.fg.g,
							 td->vp.fg.b,
							 SDL_ALPHA_OPAQUE);

	/*
	 * point to the first character position on
	 * the final vp surface
	 */
	destrect.x = td->vp.area.titletext.x;
   	destrect.y = td->vp.area.titletext.y;
   	destrect.w = td->vp.fontdim.w;
   	destrect.h = td->vp.fontdim.h;

	/*
	 * set the destination area on the text and
	 * color surfaces.
	 *
	 * NOTE: this exists in case the text and color
	 * surfaces ever become larger than 1 char in size
	 */
	scratch_area.x = 0;
	scratch_area.y = 0;
	scratch_area.w = destrect.w;
	scratch_area.h = destrect.h;

	if (td->vp.rendmode == REND_BLEND)
	{
		/* we want to fill the entire area */
		destrect.w = td->vp.fontdim.w * len;

		/* clear the destination text area */
		if (SDL_MUSTLOCK(td->vp.surface))
		{
			SDL_LockSurface(td->vp.surface);
		}

		SDL_FillRect(td->vp.surface, &destrect,
						 SDL_MapRGBA(td->vp.surface->format,
										 td->vp.bg.r,
										 td->vp.bg.g,
										 td->vp.bg.b,
										 SDL_ALPHA_OPAQUE));

		if (SDL_MUSTLOCK(td->vp.surface))
		{
			SDL_UnlockSurface(td->vp.surface);
		}

		/* set the fore color mask for the text area */
		SDL_FillRect(td->vp.colorsurf, &scratch_area, color);

		destrect.w = td->vp.fontdim.w;
	}

   /* write the text to the text surface */
   while (i < len)
   {
      if (vp_title[i] > font_info.last_ch || vp_title[i] < font_info.first_ch)
      {
         /*
			 * invalid char found, put the cursor glyph in its place
			 * for lack of a better option at this point
			 */
         SDL_BlitSurface(td->vp.cursurf, ENTIRE_SURFACE, td->vp.surface, &scratch_area);
      }
      else
      {
			if (td->vp.rendmode == REND_SOLID)
			{
				if (vp_title[i] != last_ch)
				{
					/* set the fore color mask for the text area */
					SDL_FillRect(td->vp.colorsurf, &scratch_area, color);

					/* copy the glyph to the working text surface */
					SDL_BlitSurface(td->vp.glyph[(int) vp_title[i]],
									ENTIRE_SURFACE, td->vp.textsurf,
									&scratch_area);

					/* copy the text surface to the colored surface utilizing the fore color key */
					SDL_BlitSurface(td->vp.textsurf,	&scratch_area, td->vp.colorsurf, &scratch_area);
				}

				/* copy the colored glyph to the destination surface */
				SDL_BlitSurface(td->vp.colorsurf, &scratch_area, td->vp.surface, &destrect);
			}
			else
			{
				if (vp_title[i] != last_ch)
				{
					/* copy the glyph to the working text surface */
					SDL_BlitSurface(td->vp.glyph[(int) vp_title[i]],
									ENTIRE_SURFACE, td->vp.textsurf,
									&scratch_area);

					/* color the text */
					SDL_BlitSurface(td->vp.colorsurf, &scratch_area, td->vp.textsurf, &scratch_area);
				}

				/* copy the colored glyph to the destination surface */
				SDL_BlitSurface(td->vp.textsurf,	&scratch_area, td->vp.surface, &destrect);
			}
      }

      destrect.x += td->vp.fontdim.w;

		last_ch = vp_title[i];
		i++;
   }

	destrect.x = td->vp.rect.x + td->vp.area.titletext.x;
	destrect.y = td->vp.rect.y + td->vp.area.titletext.y;
	destrect.w = scratch_area.w;
	destrect.h = scratch_area.h;

	/* copy the title area to the screen */
	SDL_BlitSurface(td->vp.surface, &scratch_area, screen.surface, &destrect);
	UPDATE_RECT(screen.surface, destrect);
}

void sdl_putstr(term_data* td, s32b x, s32b y, s32b n, byte a, const char* cp)
{
	SDL_Rect		destrect;
	SDL_Rect		scratch_area;
	s32b				i;
	char			last_ch = 0;
	s32b				color;

	if (screen.surface == NULL)
	{
		/* terminate the application */
		sdl_log_error("putstr() called when no surface is available");
	}

	sdl_utl_color_copy(&td->vp.fg, &color_table[a]);

	color = SDL_MapRGBA(td->vp.colorsurf->format,
							 td->vp.fg.r,
							 td->vp.fg.g,
							 td->vp.fg.b,
							 SDL_ALPHA_OPAQUE);

	/*
	 * point to the first character position on
	 * the final vp surface
	 */
	destrect.x = td->vp.area.content.x + td->vp.fontdim.w * x;
	destrect.y = td->vp.area.content.y + td->vp.fontdim.h * y;
	destrect.w = td->vp.fontdim.w;
	destrect.h = td->vp.fontdim.h;

	/*
	 * set the destination area on the text and
	 * color surfaces.
	 *
	 * NOTE: this exists in case the text and color
	 * surfaces ever become larger than 1 char in size
	 */
	scratch_area.x = 0;
	scratch_area.y = 0;
	scratch_area.w = destrect.w;
	scratch_area.h = destrect.h;

	if (td->vp.rendmode == REND_BLEND)
	{
		/* clear the destination text area */
		sdl_utl_clear_rect(td, x, y, n);

		/* set the fore color mask for the text area */
		SDL_FillRect(td->vp.colorsurf, ENTIRE_SURFACE, color);
	}

	/* write the text to the text surface */
	i = n;
	while (i--)
	{
		if (*cp > font_info.last_ch || *cp < font_info.first_ch)
		{
			/*
				* invalid char found, put the cursor glyph in its place
				* for lack of a better option at this point
				*/
			SDL_BlitSurface(td->vp.cursurf, ENTIRE_SURFACE, td->vp.surface, &destrect);
		}
		else
		{
			if (td->vp.rendmode == REND_SOLID)
			{
				if (*cp != last_ch)
				{
					// set the fore color mask for the text area
					SDL_FillRect(td->vp.colorsurf, &scratch_area, color);

					// copy the glyph to the working text surface
					SDL_BlitSurface(td->vp.glyph[(s32b)*cp], ENTIRE_SURFACE, td->vp.textsurf, &scratch_area);

					// copy the text surface to the colored surface utilizing the fore color key
					SDL_BlitSurface(td->vp.textsurf,	&scratch_area, td->vp.colorsurf, &scratch_area);
				}

				// copy the colored glyph to the screen
				SDL_BlitSurface(td->vp.colorsurf, &scratch_area, td->vp.surface, &destrect);
			}
			else
			{
				if (*cp != last_ch)
				{
					// copy the glyph to the working text surface
					SDL_BlitSurface(td->vp.glyph[(s32b)*cp], ENTIRE_SURFACE, td->vp.textsurf, &scratch_area);

					// color the text
					SDL_BlitSurface(td->vp.colorsurf, &scratch_area, td->vp.textsurf, &scratch_area);
				}

				// copy the colored glyph to the destination surface
				SDL_BlitSurface(td->vp.textsurf,	&scratch_area, td->vp.surface, &destrect);
			}
		}

		destrect.x += td->vp.fontdim.w;

		last_ch = *cp;
		cp++;
   	}

	scratch_area.x = td->vp.area.content.x + td->vp.fontdim.w * x;
	scratch_area.y = td->vp.area.content.y + td->vp.fontdim.h * y;
	scratch_area.w = td->vp.fontdim.w * n;
	scratch_area.h = td->vp.fontdim.h;

	destrect.x = scratch_area.x + td->vp.rect.x;
	destrect.y = scratch_area.y + td->vp.rect.y;
	destrect.w = scratch_area.w;
	destrect.h = scratch_area.h;

	sdl_utl_add_rect(td->vp.id, &destrect);
	/*
	// debug
	if (draw_immediate && td->vp.id == 0)
	{
		for (i=0; i<n; i++)
			printf("'%c'", src[i]);

		printf("\n");
	}
	*/
}

void sdl_putrawstr(term_data* td, s32b x, s32b y, s32b n, const char* cp)
{
   	SDL_Rect		destrect;
	SDL_Rect		scratch_area;
	SDL_Rect		clear_area;
   	s32b				i;
	char			last_ch = 0;
	s32b				color;

	if (screen.surface == NULL)
	{
		/* terminate the application */
		sdl_log_error("sdl_putrawstr() called when no surface is available");
	}

	color = SDL_MapRGB(screen.surface->format,
								255,
								255,
								255);

	/*
	 * point to the first character position on
	 * the final vp surface
	 */
	destrect.x = x;
   	destrect.y = y;
   	destrect.w = td->vp.fontdim.w;
   	destrect.h = td->vp.fontdim.h;

	/*
	 * set the destination area on the text and
	 * color surfaces.
	 *
	 * NOTE: this exists in case the text and color
	 * surfaces ever become larger than 1 char in size
	 */
	scratch_area.x = 0;
	scratch_area.y = 0;
	scratch_area.w = destrect.w;
	scratch_area.h = destrect.h;

	clear_area.x = x;
	clear_area.y = y;
	clear_area.w = destrect.w * 16;
	clear_area.h = td->vp.fontdim.h;

	/* clear the destination text area */
	if (SDL_MUSTLOCK(screen.surface))
	{
		SDL_LockSurface(screen.surface);
	}

	SDL_FillRect(screen.surface, &clear_area,
					 SDL_MapRGB(screen.surface->format,
									 0,
									 0,
									 0));

	if (SDL_MUSTLOCK(screen.surface))
	{
		SDL_UnlockSurface(screen.surface);
	}

	SDL_UpdateRect(screen.surface, clear_area.x, clear_area.y, clear_area.w, clear_area.h);

   /* write the text to the text surface */
   i = n;
	while (i--)
	{
		if (*cp > font_info.last_ch || *cp < font_info.first_ch)
		{
			/*
				* invalid char found, put the cursor glyph in its place
				* for lack of a better option at this point
				*/
			SDL_BlitSurface(td->vp.cursurf, ENTIRE_SURFACE, screen.surface, &destrect);
		}
		else
		{
			if (td->vp.rendmode == REND_SOLID)
			{
				if (*cp != last_ch)
				{
					/* set the fore color mask for the text area */
					SDL_FillRect(td->vp.colorsurf, &scratch_area, color);

					/* copy the glyph to the working text surface */
					SDL_BlitSurface(td->vp.glyph[(s32b)*cp], ENTIRE_SURFACE, td->vp.textsurf, &scratch_area);

					/* copy the text surface to the colored surface utilizing the fore color key */
					SDL_BlitSurface(td->vp.textsurf, &scratch_area, td->vp.colorsurf, &scratch_area);
				}

				/* copy the colored glyph to the screen */
				SDL_BlitSurface(td->vp.colorsurf, &scratch_area, screen.surface, &destrect);
			}
			else
			{
				if (*cp != last_ch)
				{
					/* copy the glyph to the working text surface */
					SDL_BlitSurface(td->vp.glyph[(s32b)*cp], ENTIRE_SURFACE, td->vp.textsurf, &scratch_area);

					/* color the text */
					SDL_BlitSurface(td->vp.colorsurf, &scratch_area, td->vp.textsurf, &scratch_area);
				}

				/* copy the colored glyph to the destination surface */
				SDL_BlitSurface(td->vp.textsurf, &scratch_area, screen.surface, &destrect);
			}
		}

		destrect.x += td->vp.fontdim.w;

		last_ch = *cp;
		cp++;
	}

	destrect.x = scratch_area.x + td->vp.rect.x;
   	destrect.y = scratch_area.y + td->vp.rect.y;
	destrect.w = td->vp.fontdim.w * n;
	destrect.h = td->vp.fontdim.h;

	/* update the destination surface immediately - no refresh delay */
	UPDATE_RECT(screen.surface, destrect);
}


#endif /* USE_SDL */
