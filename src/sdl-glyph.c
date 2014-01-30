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

#include "SDL.h"
#include "SDL_ttf.h"
#include "physfsrwops.h"
#include "sdl-defines.h"
#include "sdl-extern.h"
#include "sdl-types.h"

#include "angband.h"

#define MIN_FONT_SIZE					4

s32b fontsets_loaded[MAX_TERM_DATA];

s32b sdl_glyph_init_ttf()
{
	if (!TTF_WasInit())
	{
		if (TTF_Init() == -1)
		{
			/* terminate the application */
			sdl_log_error("Could not initialize SDL_ttf");
		}
	}
	else
	{
		sdl_log_info("SDL_ttf already initialized");
	}

   return ERRLVL_NO_ERR;
}

void sdl_glyph_quit_ttf()
{
	if (TTF_WasInit())
	{
		TTF_Quit();
		sdl_log_info("SDL_ttf quit");
	}
}

void sdl_glyph_free_text(SDL_Surface** text)
{
   s32b ch = 0;

   while (ch < MAX_GLYPHS)
   {
		if (text[ch])
		{
			SDL_FreeSurface(text[ch]);
			text[ch] = NULL;
		}

		ch++;
   }
}

void sdl_glyph_load_params(term_data* td, s32b i)
{
	char buf[MAX_LINE_LEN];
	char fontfile[MAX_LINE_LEN];
	char fonttype[4];

	/* assumes ini file exists and is loaded */

	td->vp.id = 			i;
	sprintf(buf, "sdl.term%d", (int) i);

	if (td->vp.id != 0)
		td->vp.visible = 	(flag_used(&window_flag[td->vp.id]) > 0) ? TRUE : FALSE;
	else
		td->vp.visible = 	TRUE;

	td->vp.name = 			string_make(angband_term_name[td->vp.id]);
	td->vp.termsize.w =		esettings_get_int(format("%s.cols", buf), 80);
	td->vp.termsize.h =		esettings_get_int(format("%s.rows", buf), 24);
	td->vp.rect.x = 		esettings_get_int(format("%s.pos.x", buf), 0);
	td->vp.rect.y = 		esettings_get_int(format("%s.pos.y", buf), 0);
	td->vp.z_pos = 			esettings_get_int(format("%s.pos.z", buf), i);
	td->vp.fontset =		esettings_get_int(format("%s.fontset", buf), 0);
	td->vp.bordervis =		esettings_get_int(format("%s.border_on", buf), 0);

	sprintf(buf, "sdl.fonts.set%i", (int) td->vp.fontset);
	snprintf(fontfile, MAX_LINE_LEN, "%s", esettings_get_str(format("%s.name", buf), DEFAULT_FONT));
	td->vp.font =			string_make(fontfile);
	td->vp.fontsize =		esettings_get_int(format("%s.size", buf), DEFAULT_TTF_FONT_SIZE);

	td->vp.rendmode = REND_SOLID;

	sdl_strmid(buf, (char*)td->vp.font, strlen(td->vp.font) - 3, 3);
	sdl_strtoupper(fonttype, buf, 3);

	if (strcmp(fonttype, "TTF") == 0)
	{
		if TST_CFLAG(USE_ALPHA)
		{
			td->vp.rendmode = REND_BLEND;
		}
	}
}

void sdl_glyph_save_parameters()
{
	char buf[MAX_LINE_LEN];
	s32b i = 0;

	esettings_set_int("sdl.screen.width",		screen.width);
	esettings_set_int("sdl.screen.height",		screen.height);
	esettings_set_int("sdl.screen.num_terms",	max_terms);

	esettings_set_int("sdl.common.first_char",		font_info.first_ch);
	esettings_set_int("sdl.common.last_char",		font_info.last_ch);
	esettings_set_int("sdl.common.font.forecolor.r", font_info.fg.r);
	esettings_set_int("sdl.common.font.forecolor.g", font_info.fg.g);
	esettings_set_int("sdl.common.font.forecolor.b", font_info.fg.b);
	esettings_set_int("sdl.common.font.backcolor.r", font_info.bg.r);
	esettings_set_int("sdl.common.font.backcolor.g", font_info.bg.g);
	esettings_set_int("sdl.common.font.backcolor.b", font_info.bg.b);
	esettings_set_int("sdl.common.border.r",		border_color.r);
	esettings_set_int("sdl.common.border.g",		border_color.g);
	esettings_set_int("sdl.common.border.b",		border_color.b);
	esettings_set_int("sdl.graphics.enabled",			gfx.enabled);
	esettings_set_int("sdl.graphics.tile.size",			gfx.tilesize);
	esettings_set_str("sdl.graphics.tile.filename",		gfx.filename);

	while (i < max_terms)
	{
		sprintf(buf, "sdl.term%d", data[i].vp.id);

		esettings_set_int(format("%s.cols", buf),		data[i].vp.termsize.w);
		esettings_set_int(format("%s.rows", buf),		data[i].vp.termsize.h);
		esettings_set_int(format("%s.pos.x", buf),		data[i].vp.rect.x);
		esettings_set_int(format("%s.pos.y", buf),		data[i].vp.rect.y);
		esettings_set_int(format("%s.pos.z", buf),		data[i].vp.z_pos);
		esettings_set_int(format("%s.fontset", buf),	data[i].vp.fontset);
		esettings_set_int(format("%s.border_on", buf),	data[i].vp.bordervis);
		i++;
	}

	i = 0;
	while (i < max_terms)
	{
		sprintf(buf, "sdl.fonts.set%i", (int) data[i].vp.fontset);

		if (strcmp(data[i].vp.font, "") == 0)
		{
			esettings_set_str(format("%s.name", buf), 			DEFAULT_FONT);
			esettings_set_int(format("%s.size", buf),			DEFAULT_TTF_FONT_SIZE);
		}
		else
		{
			esettings_set_str(format("%s.name", buf), 			(char*)data[i].vp.font);
			esettings_set_int(format("%s.size", buf),			data[i].vp.fontsize);
		}

		i++;
	}

	esettings_save();
}

s32b sdl_glyph_make_cursor(term_data* td)
{
	Uint32 color_key;
	SDL_Rect destrect = { 1, 1,
						  td->vp.glyph[font_info.first_ch]->w - 2,
						  td->vp.glyph[font_info.first_ch]->h - 2 };

	/*
	 * Create the text SDL surface and convert it to the same format as the
	 * character glyph. This allows the glyph BMPs to be created with the
	 * proper fore color across platforms.
	 */
	td->vp.cursurf = sdl_utl_create_surface(0,
							td->vp.glyph[font_info.first_ch]->w,
							td->vp.glyph[font_info.first_ch]->h,
							BPP_AUTO,
							td->vp.rendmode);

	if (!td->vp.cursurf)
	{
		/* terminate the application */
		sdl_log_error("[%s] Could not create term cursor", td->vp.name);
	}

	/* alpha off */
	SDL_SetAlpha(td->vp.cursurf, NO_ALPHA, 0);

	/* color the cursor's base, typically yellow*/
	SDL_FillRect(td->vp.cursurf, ENTIRE_SURFACE,
						SDL_MapRGB(td->vp.cursurf->format,
										0xff,
										0xff,
										0));

	/* now color the center area, leaving the outside colored rect */
	SDL_FillRect(td->vp.cursurf, &destrect,
						SDL_MapRGB(td->vp.cursurf->format,
										0xff,
										0,
										0xff));

	/* make sure the center of the cursor is invisible */
	color_key = SDL_MapRGB(td->vp.cursurf->format,
										0xff,
										0,
										0xff);

	SDL_SetColorKey(td->vp.cursurf, SDL_SRCCOLORKEY, color_key);

	return ERRLVL_NO_ERR;
}

void sdl_glyph_get_font_dims(term_data* td)
{
	TTF_Font* h_font = NULL;
	int minx = 0;
	int maxx = 0;
	int miny = 0;
	int maxy = 0;
	int advance = 0;
	Uint16 ch = 0;
	char fontfile[MAX_PATH_LEN];

	path_build(fontfile, 1024, SDL_FONT_DIR, td->vp.font);

	if (sdl_utl_file_verify(fontfile, ERRLVL_CRITICAL) == ERRLVL_CRITICAL)
	{
		/* terminate the application */
		sdl_log_error("Error: Font %s not found", fontfile);
	}

	h_font = TTF_OpenFontRW(PHYSFSRWOPS_openRead(fontfile), 1, td->vp.fontsize);

	if (h_font == NULL)
	{
		sdl_log_error("[%s] Could not open %s for calculating metrics (%s)", td->vp.name, fontfile, SDL_GetError());
	}

	sdl_log_info("[%s] First char: 0x%.2X, last char: 0x%.2X", td->vp.name, font_info.first_ch, font_info.last_ch);

   TTF_SetFontStyle(h_font, TTF_STYLE_NORMAL);

   /* find the widest character and save the value */
   ch = font_info.first_ch;
   td->vp.fontdim.w = 0;
   while (ch <= font_info.last_ch)
   {
      TTF_GlyphMetrics(h_font, ch, &minx, &maxx, &miny, &maxy, &advance);

      if (maxx - minx > td->vp.fontdim.w)
      {
         td->vp.fontdim.w = maxx - minx;
      }
      ch++;
   }

	sdl_log_info("[%s] Widest character: %ipx", td->vp.name, td->vp.fontdim.w);

	/* get the font height */
   td->vp.fontdim.h = TTF_FontHeight(h_font);

	/* set the scaled dimensions as 1:1 */
	td->vp.fontdim.w = td->vp.fontdim.w;
	td->vp.fontdim.h = td->vp.fontdim.h;

   if (h_font)
   {
      TTF_CloseFont(h_font);
      h_font = NULL;
   }
}

void sdl_glyph_render(term_data* td)
{
	TTF_Font* h_font = NULL;
	int minx = 0;
	int maxx = 0;
	int miny = 0;
	int maxy = 0;
	int advance = 0;
	int max_height = 0;
	Sint16 ch = 0;
	bool ch_skip[MAX_GLYPHS];
	SDL_Surface* glyph_surf[MAX_GLYPHS] = { 0 };
	SDL_Rect glyph_rect = {0, 0, 0, 0};
	char fontfile[MAX_PATH_LEN];
	SDL_Color fore_color = {255, 255, 255, 255};

	path_build(fontfile, 1024, SDL_FONT_DIR, td->vp.font);

	if (sdl_utl_file_verify(fontfile, ERRLVL_CRITICAL) == ERRLVL_CRITICAL)
	{
		/* terminate the application */
		sdl_log_error("Error: Font %s not found", fontfile);
	}

   	/* load the font */
	h_font = TTF_OpenFontRW(PHYSFSRWOPS_openRead(fontfile), 1, td->vp.fontsize);

	if (h_font == NULL)
	{
		/* terminate the application */
		sdl_log_error("[%s] Could not open %s for rendering (%s)", td->vp.name, fontfile, SDL_GetError());
	}

	/* set the style */
	TTF_SetFontStyle(h_font, TTF_STYLE_NORMAL);

	/*
	 * render the font's characters into their respective glyph surfaces
	 */
/*
	ch = font_info.first_ch;
	if (ch < FONT_PARAM_FIRST_CH)
	{
		font_info.first_ch = FONT_PARAM_FIRST_CH;
		ch = font_info.first_ch;
	}
*/
	/* continue to make the rest of the character glyphs */

	ch = font_info.first_ch;
	while (ch <= font_info.last_ch)
	{
		ch_skip[ch] = FALSE;

		/* render and check to make sure it completed */
		switch(td->vp.rendmode)
		{
			case REND_SOLID:
				glyph_surf[ch] = TTF_RenderGlyph_Solid(h_font, ch, td->vp.fg);
				break;

			case REND_BLEND:
				glyph_surf[ch] = TTF_RenderGlyph_Blended(h_font, ch, fore_color);
				break;

			default:
				sdl_log_error("[%s] Invalid render mode selected (%i)", td->vp.name, td->vp.rendmode);
				break;
		}

		if (glyph_surf[ch] == ch || glyph_surf[ch] == NULL)
		{
			/*
			 * If the glyph result equals the character, FreeType won't allow rendering.
			 * If the result is NULL, something went wrong.
			 * Instead, just make a blank glyph for that character and log it.
			 */

			sdl_log_warn("[%s] Could not render char %.2x of %s (SDL_ttf Error: %s)", td->vp.name, ch, fontfile, TTF_GetError());

			ch_skip[ch] = TRUE;

			glyph_surf[ch] = sdl_utl_create_surface(0,
													td->vp.fontdim.w,
													td->vp.fontdim.h,
													BPP_AUTO,
													td->vp.rendmode);

			if (glyph_surf[ch] == NULL)
			{
				/* terminate the application */
				sdl_log_error("[%s] Could not render char %.2x of %s (SDL_ttf Error: %s)", ch, td->vp.name, fontfile, TTF_GetError());
			}

			/* no alpha */
			SDL_SetAlpha(glyph_surf[ch], NO_ALPHA, SDL_ALPHA_OPAQUE);

			if (SDL_MUSTLOCK(glyph_surf[ch]))
			{
				SDL_LockSurface(glyph_surf[ch]);
			}

			SDL_FillRect(glyph_surf[ch], ENTIRE_SURFACE,
						 SDL_MapRGB(glyph_surf[ch]->format,
									td->vp.bg.r,
									td->vp.bg.g,
									td->vp.bg.b));

			if (SDL_MUSTLOCK(glyph_surf[ch]))
			{
				SDL_UnlockSurface(glyph_surf[ch]);
			}
		}
		else
		{
			/* The character was rendered properly, determine the max height */

			SDL_SetAlpha(glyph_surf[ch], NO_ALPHA, 0);

			/*
			* get the metrics for this character so that we can determine
			* the maximum height of the rendered character set
			*/
			TTF_GlyphMetrics(h_font, ch ,&minx, &maxx, &miny, &maxy, &advance);

			/* determine the max height of all the characters rendered */
			if ((maxy - miny) > max_height)
			{
				max_height = maxy - miny;
			}
		}

		ch++;
	}

	sdl_log_info("[%s] Tallest character: %ipx", td->vp.name, max_height);

	/*
	 * now center align the glyph and place it into the usable text surface
	 */

	ch = font_info.first_ch;
	while (ch <= font_info.last_ch)
   	{
		if (!ch_skip[ch])
		{
			/*
			* get the metrics for this character so that we can position
			* it correctly on the final SDL surface
			*/
			TTF_GlyphMetrics(h_font, ch ,&minx, &maxx, &miny, &maxy, &advance);

			/* calculate the proper position for the glyph on the new surface */
			glyph_rect.x = (td->vp.fontdim.w >> 1) - ((maxx - minx) >> 1);
			glyph_rect.y = (max_height - maxy) - 1;
			glyph_rect.w = td->vp.fontdim.w;
			glyph_rect.h = td->vp.fontdim.h;

			/*
			* FON files don't require vertical positioning
			* and this will prevent improper vertical alignment for those fonts
			*/

			if (glyph_rect.y < 0)
			{
				glyph_rect.y = 0;
			}
		}
		else
		{
			/* this glyph was not available for FreeType rendering */
			glyph_rect.x = 0;
			glyph_rect.y = 0;
			glyph_rect.w = td->vp.fontdim.w;
			glyph_rect.h = td->vp.fontdim.h;
		}

		/*
		 * Create the text SDL surface
		 */

		if (td->vp.glyph[ch])
		{
			SDL_FreeSurface(td->vp.glyph[ch]);
		}

		td->vp.glyph[ch] = sdl_utl_create_surface(0, glyph_rect.w, glyph_rect.h, BPP_AUTO, td->vp.rendmode);

		/* make the final character glyph */
		if (td->vp.glyph[ch])
		{
			SDL_BlitSurface(glyph_surf[ch], ENTIRE_SURFACE, td->vp.glyph[ch], &glyph_rect);
			SDL_SetAlpha(td->vp.glyph[ch], NO_ALPHA, 0);

			td->vp.fontdim.w = td->vp.glyph[ch]->w;
			td->vp.fontdim.h = td->vp.glyph[ch]->h;

			if (glyph_surf[ch])
			{
				SDL_FreeSurface(glyph_surf[ch]);
				glyph_surf[ch] = NULL;
			}
		}

		ch++;
	}

	sdl_log_info("[%s] Final glyph size: %i x %i", td->vp.name, td->vp.fontdim.w, td->vp.fontdim.h);

	if (h_font)
	{
		TTF_CloseFont(h_font);
		h_font = NULL;
	}
}

#define S_G_CREATE_INIT_SDL_TTF		0
#define S_G_CREATE_GET_FONT_DIMS		1
#define S_G_CREATE_RENDER_GLYPHS		2
#define S_G_CREATE_EXIT					3

/*
 * Each funtion called will set the global errlvl variable
 * before returning
 */

void sdl_glyphs_create(term_data* td)
{
	s32b state = 0;

	sdl_log_info("[%s] Creating glyphs ", td->vp.name);

	while (state != S_G_CREATE_EXIT && errlvl != ERRLVL_CRITICAL)
	{
		switch (state)
		{
			case S_G_CREATE_INIT_SDL_TTF:
				sdl_glyph_init_ttf();
				break;

			case S_G_CREATE_GET_FONT_DIMS:
				sdl_glyph_get_font_dims(td);
				break;

			case S_G_CREATE_RENDER_GLYPHS:
				sdl_glyph_render(td);
				break;

			case S_G_CREATE_EXIT:
				/* we should never get here */
			default:
				/* terminate the application */
				sdl_log_error("Invalid state [%.2x] reached in init_core()", state);
				break;
		}
		state++;
	}

}

#endif /* USE_SDL */
