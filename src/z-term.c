/* File: z-term.c */

/*
 * Copyright (c) 1997 Ben Harrison
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.
 */

/* Purpose: a generic, efficient, terminal window package -BEN- */

#include "angband.h"

#include "z-term.h"

#include "z-virt.h"


/*
 * This file provides a generic, efficient, terminal window package,
 * which can be used not only on standard terminal environments such
 * as dumb terminals connected to a Unix box, but also in more modern
 * "graphic" environments, such as the Macintosh or Unix/X11.
 *
 * Each "window" works like a standard "dumb terminal", that is, it
 * can display a two dimensional array of grids containing colored
 * textual symbols, plus an optional cursor, and it can be used to
 * get keypress events from the user.
 *
 * In fact, this package can simply be used, if desired, to support
 * programs which will look the same on a dumb terminal as they do
 * on a graphic platform such as the Macintosh.
 *
 * This package was designed to help port the game "Angband" to a wide
 * variety of different platforms.  Angband, like many other games in
 * the "rogue-like" heirarchy, requires, at the minimum, the ability
 * to display "colored textual symbols" in a standard 80x24 "window",
 * such as that provided by most dumb terminals, and many old personal
 * computers, and to check for "keypresses" from the user.  The major
 * concerns were thus portability and efficiency, so Angband could be
 * easily ported to many different systems, with minimal effort, and
 * yet would run quickly on each of these systems, no matter what kind
 * of underlying hardware/software support was being used.
 *
 * It is important to understand the differences between the older
 * "dumb terminals" and the newer "graphic interface" machines, since
 * this package was designed to work with both types of systems.
 *
 * New machines:
 *   waiting for a keypress is complex
 *   checking for a keypress is often cheap
 *   changing "colors" may be expensive
 *   the "color" of a "blank" is rarely important
 *   moving the "cursor" is relatively cheap
 *   use a "software" cursor (only moves when requested)
 *   drawing characters normally will not erase old ones
 *   drawing a character on the cursor often erases it
 *   may have fast routines for "clear a region"
 *   the bottom right corner is usually not special
 *
 * Old machines:
 *   waiting for a keypress is simple
 *   checking for a keypress is often expensive
 *   changing "colors" is usually cheap
 *   the "color" of a "blank" may be important
 *   moving the "cursor" may be expensive
 *   use a "hardware" cursor (moves during screen updates)
 *   drawing new symbols automatically erases old ones
 *   characters may only be drawn at the cursor location
 *   drawing a character on the cursor will move the cursor
 *   may have fast routines for "clear entire window"
 *   may have fast routines for "clear to end of line"
 *   the bottom right corner is often dangerous
 *
 *
 * This package provides support for multiple windows, each of an
 * arbitrary size (up to 255x255), each with its own set of flags,
 * and its own hooks to handle several low-level procedures which
 * differ from platform to platform.  Then the main program simply
 * creates one or more "term" structures, setting the various flags
 * and hooks in a manner appropriate for the current platform, and
 * then it can use the various "term" structures without worrying
 * about the underlying platform.
 *
 *
 * This package allows each "grid" in each window to hold an attr/char
 * pair, with each ranging from 0 to 255, and makes very few assumptions
 * about the meaning of any attr/char values.  Normally, we assume that
 * "attr 0" is "black", with the semantics that "black" text should be
 * sent to "Term_wipe()" instead of "Term_text()", but this sematics is
 * modified if either the "always_pict" or the "always_text" flags are
 * set.  We assume that "char 0" is "dangerous", since placing such a
 * "char" in the middle of a string "terminates" the string, and usually
 * we prevent its use.
 *
 * Finally, we use a special attr/char pair, defaulting to "attr 0" and
 * "char 32", also known as "black space", when we "erase" or "clear"
 * any window, but this pair can be redefined to any pair, including
 * the standard "white space", or the bizarre "emptiness" ("attr 0"
 * and "char 0"), as long as various obscure restrictions are met.
 *
 *
 * This package provides several functions which allow a program to
 * interact with the "term" structures.  Most of the functions allow
 * the program to "request" certain changes to the current "term",
 * such as moving the cursor, drawing an attr/char pair, erasing a
 * region of grids, hiding the cursor, etc.  Then there is a special
 * function which causes all of the "pending" requests to be performed
 * in an efficient manner.  There is another set of functions which
 * allow the program to query the "requested state" of the current
 * "term", such as asking for the cursor location, or what attr/char
 * is at a given location, etc.  There is another set of functions
 * dealing with "keypress" events, which allows the program to ask if
 * the user has pressed any keys, or to forget any keys the user pressed.
 * There is a pair of functions to allow this package to memorize the
 * contents of the current "term", and to restore these contents at
 * a later time.  There is a special function which allows the program
 * to specify which "term" structure should be the "current" one.  At
 * the lowest level, there is a set of functions which allow a new
 * "term" to be initialized or destroyed, and which allow this package,
 * or a program, to access the special "hooks" defined for the current
 * "term", and a set of functions which those "hooks" can use to inform
 * this package of the results of certain occurances, for example, one
 * such function allows this package to learn about user keypresses,
 * detected by one of the special "hooks".
 *
 * We provide, among other things, the functions "Term_keypress()"
 * to "react" to keypress events, and "Term_redraw()" to redraw the
 * entire window, plus "Term_resize()" to note a new size.
 *
 *
 * Note that the current "term" contains two "window images".  One of
 * these images represents the "requested" contents of the "term", and
 * the other represents the "actual" contents of the "term", at the time
 * of the last performance of pending requests.  This package uses these
 * two images to determine the "minimal" amount of work needed to make
 * the "actual" contents of the "term" match the "requested" contents of
 * the "term".  This method is not perfect, but it often reduces the
 * amount of work needed to perform the pending requests, which thus
 * increases the speed of the program itself.  This package promises
 * that the requested changes will appear to occur either "all at once"
 * or in a "top to bottom" order.  In addition, a "cursor" is maintained,
 * and this cursor is updated along with the actual window contents.
 *
 * Currently, the "Term_fresh()" routine attempts to perform the "minimum"
 * number of physical updates, in terms of total "work" done by the hooks
 * Term_wipe(), Term_text(), and Term_pict(), making use of the fact that
 * adjacent characters of the same color can both be drawn together using
 * the "Term_text()" hook, and that "black" text can often be sent to the
 * "Term_wipe()" hook instead of the "Term_text()" hook, and if something
 * is already displayed in a window, then it is not necessary to display
 * it again.  Unfortunately, this may induce slightly non-optimal results
 * in some cases, in particular, those in which, say, a string of ten
 * characters needs to be written, but the fifth character has already
 * been displayed.  Currently, this will cause the "Term_text()" routine
 * to be called once for each half of the string, instead of once for the
 * whole string, which, on some machines, may be non-optimal behavior.
 *
 * The new formalism includes a "displayed" screen image (old) which
 * is actually seen by the user, a "requested" screen image (scr)
 * which is being prepared for display, a "memorized" screen image
 * (mem) which is used to save and restore screen images, and a
 * "temporary" screen image (tmp) which is currently unused.
 *
 *
 * Several "flags" are available in each "term" to allow the underlying
 * visual system (which initializes the "term" structure) to "optimize"
 * the performance of this package for the given system, or to request
 * certain behavior which is helpful/required for the given system.
 *
 * The "soft_cursor" flag indicates the use of a "soft" cursor, which
 * only moves when explicitly requested,and which is "erased" when
 * any characters are drawn on top of it.  This flag is used for all
 * "graphic" systems which handle the cursor by "drawing" it.
 *
 * The "icky_corner" flag indicates that the bottom right "corner"
 * of the windows are "icky", and "printing" anything there may
 * induce "messy" behavior, such as "scrolling".  This flag is used
 * for most old "dumb terminal" systems.
 *
 *
 * The "term" structure contains the following function "hooks":
 *
 *   Term->init_hook = Init the term
 *   Term->nuke_hook = Nuke the term
 *   Term->user_hook = Perform user actions
 *   Term->xtra_hook = Perform extra actions
 *   Term->curs_hook = Draw (or Move) the cursor
 *   Term->wipe_hook = Draw some blank spaces
 *   Term->text_hook = Draw some text in the window
 *   Term->pict_hook = Draw some attr/chars in the window
 *
 * The "Term->user_hook" hook provides a simple hook to an implementation
 * defined function, with application defined semantics.  It is available
 * to the program via the "Term_user()" function.
 *
 * The "Term->xtra_hook" hook provides a variety of different functions,
 * based on the first parameter (which should be taken from the various
 * TERM_XTRA_* defines) and the second parameter (which may make sense
 * only for some first parameters).  It is available to the program via
 * the "Term_xtra()" function, though some first parameters are only
 * "legal" when called from inside this package.
 *
 * The "Term->curs_hook" hook provides this package with a simple way
 * to "move" or "draw" the cursor to the grid "x,y", depending on the
 * setting of the "soft_cursor" flag.  Note that the cursor is never
 * redrawn if "nothing" has happened to the screen (even temporarily).
 * This hook is required.
 *
 * The "Term->wipe_hook" hook provides this package with a simple way
 * to "erase", starting at "x,y", the next "n" grids.  This hook assumes
 * that the input is valid.  This hook is required, unless the setting
 * of the "always_pict" or "always_text" flags makes it optional.
 *
 * The "Term->text_hook" hook provides this package with a simple way
 * to "draw", starting at "x,y", the "n" chars contained in "cp", using
 * the attr "a".  This hook assumes that the input is valid, and that
 * "n" is between 1 and 256 inclusive, but it should NOT assume that
 * the contents of "cp" are null-terminated.  This hook is required,
 * unless the setting of the "always_pict" flag makes it optional.
 *
 * The "Term->pict_hook" hook provides this package with a simple way
 * to "draw", starting at "x,y", the "n" attr/char pairs contained in
 * the arrays "ap" and "cp".  This hook assumes that the input is valid,
 * and that "n" is between 1 and 256 inclusive, but it should NOT assume
 * that the contents of "cp" are null-terminated.  This hook is optional,
 * unless the setting of the "always_pict" or "higher_pict" flags make
 * it required.  Note that recently, this hook was changed from taking
 * a byte "a" and a char "c" to taking a length "n", an array of bytes
 * "ap" and an array of chars "cp".  Old implementations of this hook
 * should now iterate over all "n" attr/char pairs.
 *
 *
 * The game "Angband" uses a set of files called "main-xxx.c", for
 * various "xxx" suffixes.  Most of these contain a function called
 * "init_xxx()", that will prepare the underlying visual system for
 * use with Angband, and then create one or more "term" structures,
 * using flags and hooks appropriate to the given platform, so that
 * the "main()" function can call one (or more) of the "init_xxx()"
 * functions, as appropriate, to prepare the required "term" structs
 * (one for each desired sub-window), and these "init_xxx()" functions
 * are called from a centralized "main()" function in "main.c".  Other
 * "main-xxx.c" systems contain their own "main()" function which, in
 * addition to doing everything needed to initialize the actual program,
 * also does everything that the normal "init_xxx()" functions would do.
 *
 * The game "Angband" defines, in addition to "attr 0", all of the
 * attr codes from 1 to 15, using definitions in "defines.h", and
 * thus the "main-xxx.c" files used by Angband must handle these
 * attr values correctly.  Also, they must handle all other attr
 * values, though they may do so in any way they wish, for example,
 * by always taking every attr code mod 16.  Many of the "main-xxx.c"
 * files use "white space" ("attr 1" / "char 32") to "erase" or "clear"
 * any window, for efficiency.
 *
 * The game "Angband" uses the "Term_user" hook to allow any of the
 * "main-xxx.c" files to interact with the user, by calling this hook
 * whenever the user presses the "!" key when the game is waiting for
 * a new command.  This could be used, for example, to provide "unix
 * shell commands" to the Unix versions of the game.
 *
 * See "main-xxx.c" for a simple skeleton file which can be used to
 * create a "visual system" for a new platform when porting Angband.
 */



/*
 * The current "term"
 */
term *Term = NULL;

/* File handler for saving movies */
PHYSFS_file *movfile = NULL;
s32b do_movies = 0;  /* Later set this as a global */
/* set to 1 if you want movies made */
time_t lastc;
s32b last_paused = 0;
s32b cmovie_get_msecond(void);

/* Record cmovies with millisecond frame rate */
long cmov_last_time_msec;
long cmov_delta_time_msec;


/*** Local routines ***/


/*
 * Nuke a term_win (see below)
 */
static errr term_win_nuke(term_win *s, s32b w, s32b h)
{
	/* Free the window access arrays */
	C_KILL(s->a, h, byte*);
	C_KILL(s->c, h, char*);

	/* Free the window content arrays */
	C_KILL(s->va, h * w, byte);
	C_KILL(s->vc, h * w, char);

#ifdef USE_TRANSPARENCY

	/* Free the terrain access arrays */
	C_KILL(s->ta, h, byte*);
	C_KILL(s->tc, h, char*);

	/* Free the terrain content arrays */
	C_KILL(s->vta, h * w, byte);
	C_KILL(s->vtc, h * w, char);

#ifdef USE_EGO_GRAPHICS

	/* Free the ego graphics access arrays */
	C_KILL(s->ea, h, byte*);
	C_KILL(s->ec, h, char*);

	/* Free the ego graphics content arrays */
	C_KILL(s->vea, h * w, byte);
	C_KILL(s->vec, h * w, char);

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

	/* Success */
	return (0);
}


/*
 * Initialize a "term_win" (using the given window size)
 */
static errr term_win_init(term_win *s, s32b w, s32b h)
{
	s32b y;

	/* Make the window access arrays */
	C_MAKE(s->a, h, byte*);
	C_MAKE(s->c, h, char*);

	/* Make the window content arrays */
	C_MAKE(s->va, h * w, byte);
	C_MAKE(s->vc, h * w, char);

#ifdef USE_TRANSPARENCY

	/* Make the terrain access arrays */
	C_MAKE(s->ta, h, byte*);
	C_MAKE(s->tc, h, char*);

	/* Make the terrain content arrays */
	C_MAKE(s->vta, h * w, byte);
	C_MAKE(s->vtc, h * w, char);

#ifdef USE_EGO_GRAPHICS

	/* Make the ego graphics access arrays */
	C_MAKE(s->ea, h, byte*);
	C_MAKE(s->ec, h, char*);

	/* Make the ego graphics content arrays */
	C_MAKE(s->vea, h * w, byte);
	C_MAKE(s->vec, h * w, char);

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */


	/* Prepare the window access arrays */
	for (y = 0; y < h; y++)
	{
		s->a[y] = s->va + w * y;
		s->c[y] = s->vc + w * y;

#ifdef USE_TRANSPARENCY

		s->ta[y] = s->vta + w * y;
		s->tc[y] = s->vtc + w * y;

#ifdef USE_EGO_GRAPHICS

		s->ea[y] = s->vea + w * y;
		s->ec[y] = s->vec + w * y;

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

	}

	/* Success */
	return (0);
}


/*
 * Copy a "term_win" from another
 */
static errr term_win_copy(term_win *s, term_win *f, s32b w, s32b h)
{
	s32b x, y;

	/* Copy contents */
	for (y = 0; y < h; y++)
	{
		byte *f_aa = f->a[y];
		char *f_cc = f->c[y];

		byte *s_aa = s->a[y];
		char *s_cc = s->c[y];

#ifdef USE_TRANSPARENCY

		byte *f_taa = f->ta[y];
		char *f_tcc = f->tc[y];

		byte *s_taa = s->ta[y];
		char *s_tcc = s->tc[y];

#ifdef USE_EGO_GRAPHICS

		byte *f_eaa = f->ea[y];
		char *f_ecc = f->ec[y];

		byte *s_eaa = s->ea[y];
		char *s_ecc = s->ec[y];

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

		for (x = 0; x < w; x++)
		{
			*s_aa++ = *f_aa++;
			*s_cc++ = *f_cc++;

#ifdef USE_TRANSPARENCY
			*s_taa++ = *f_taa++;
			*s_tcc++ = *f_tcc++;

#ifdef USE_EGO_GRAPHICS
			*s_eaa++ = *f_eaa++;
			*s_ecc++ = *f_ecc++;
#endif /* USE_EGO_GRAPHICS */
#endif /* USE_TRANSPARENCY */

		}
	}

	/* Copy cursor */
	s->cx = f->cx;
	s->cy = f->cy;
	s->cu = f->cu;
	s->cv = f->cv;

	/* Success */
	return (0);
}



/*** External hooks ***/


/*
 * Execute the "Term->user_hook" hook, if available (see above).
 */
errr Term_user(s32b n)
{
	/* Verify the hook */
	if (!Term->user_hook) return ( -1);

	/* Call the hook */
	return ((*Term->user_hook)(n));
}

/*
 * Execute the "Term->xtra_hook" hook, if available (see above).
 * And *hacky* get a return code
 */
long Term_xtra_long;
errr Term_xtra(s32b n, s32b v)
{
	/* Verify the hook */
	if (!Term->xtra_hook) return ( -1);

	/* Call the hook */
	return ((*Term->xtra_hook)(n, v));
}



/*** Fake hooks ***/


/*
 * Hack -- fake hook for "Term_curs()" (see above)
 */
static errr Term_curs_hack(s32b x, s32b y)
{
	/* Compiler silliness */
	if (x || y) return ( -2);

	/* Oops */
	return ( -1);
}

/*
 * Hack -- fake hook for "Term_wipe()" (see above)
 */
static errr Term_wipe_hack(s32b x, s32b y, s32b n)
{
	/* Compiler silliness */
	if (x || y || n) return ( -2);

	/* Oops */
	return ( -1);
}

/*
 * Hack -- fake hook for "Term_text()" (see above)
 */
static errr Term_text_hack(s32b x, s32b y, s32b n, byte a, const char *cp)
{
	/* Compiler silliness */
	if (x || y || n || a || cp) return ( -2);

	/* Oops */
	return ( -1);
}

/*
 * Hack -- fake hook for "Term_pict()" (see above)
 */
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
static errr Term_pict_hack(s32b x, s32b y, s32b n, const byte *ap, const char *cp, const byte *tap, const char *tcp, const byte *eap, const char *ecp)
#else /* USE_EGO_GRAPHICS */
static errr Term_pict_hack(s32b x, s32b y, s32b n, const byte *ap, const char *cp, const byte *tap, const char *tcp)
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
static errr Term_pict_hack(s32b x, s32b y, s32b n, const byte *ap, const char *cp)
#endif /* USE_TRANSPARENCY */
{
	/* Compiler silliness */
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
	if (x || y || n || ap || cp || tap || tcp || eap || ecp) return ( -2);
#else /* USE_EGO_GRAPHICS */
if (x || y || n || ap || cp || tap || tcp) return ( -2);
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
	if (x || y || n || ap || cp) return ( -2);
#endif /* USE_TRANSPARENCY */

	/* Oops */
	return ( -1);
}



/*** Efficient routines ***/


/*
 * Mentally draw an attr/char at a given location
 *
 * Assumes given location and values are valid.
 */
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
void Term_queue_char(s32b x, s32b y, byte a, char c, byte ta, char tc, byte ea, char ec)
#else /* USE_EGO_GRAPHICS */
void Term_queue_char(s32b x, s32b y, byte a, char c, byte ta, char tc)
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
void Term_queue_char(s32b x, s32b y, byte a, char c)
#endif /* USE_TRANSPARENCY */
{
	term_win *scrn = Term->scr;

	byte *scr_aa = &scrn->a[y][x];
	char *scr_cc = &scrn->c[y][x];

#ifdef USE_TRANSPARENCY

	byte *scr_taa = &scrn->ta[y][x];
	char *scr_tcc = &scrn->tc[y][x];

#ifdef USE_EGO_GRAPHICS

	byte *scr_eaa = &scrn->ea[y][x];
	char *scr_ecc = &scrn->ec[y][x];

	/* Hack -- Ignore non-changes */
	if ((*scr_aa == a) && (*scr_cc == c) &&
	                (*scr_taa == ta) && (*scr_tcc == tc) &&
	                (*scr_eaa == ea) && (*scr_ecc == ec)) return;

#else /* USE_EGO_GRAPHICS */

	/* Hack -- Ignore non-changes */
	if ((*scr_aa == a) && (*scr_cc == c) &&
	                (*scr_taa == ta) && (*scr_tcc == tc)) return;

#endif /* USE_EGO_GRAPHICS */

#else /* USE_TRANSPARENCY */

	/* Hack -- Ignore non-changes */
	if ((*scr_aa == a) && (*scr_cc == c)) return;

#endif /* USE_TRANSPARENCY */

	/* Save the "literal" information */
	*scr_aa = a;
	*scr_cc = c;

#ifdef USE_TRANSPARENCY

	*scr_taa = ta;
	*scr_tcc = tc;

#ifdef USE_EGO_GRAPHICS

	*scr_eaa = ea;
	*scr_ecc = ec;

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

	/* Check for new min/max row info */
	if (y < Term->y1) Term->y1 = y;
	if (y > Term->y2) Term->y2 = y;

	/* Check for new min/max col info for this row */
	if (x < Term->x1[y]) Term->x1[y] = x;
	if (x > Term->x2[y]) Term->x2[y] = x;
}


/*
 * Mentally draw a string of attr/chars at a given location
 *
 * Assumes given location and values are valid.
 *
 * This function is designed to be fast, with no consistancy checking.
 * It is used to update the map in the game.
 */
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
void Term_queue_line(s32b x, s32b y, s32b n, byte *a, char *c, byte *ta, char *tc, byte *ea, char *ec)
#else /* USE_EGO_GRAPHICS */
void Term_queue_line(s32b x, s32b y, s32b n, byte *a, char *c, byte *ta, char *tc)
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
void Term_queue_line(s32b x, s32b y, s32b n, byte *a, char *c)
#endif /* USE_TRANSPARENCY */
{
	term_win *scrn = Term->scr;

	s32b x1 = -1;
	s32b x2 = -1;

	byte *scr_aa = &scrn->a[y][x];
	char *scr_cc = &scrn->c[y][x];

#ifdef USE_TRANSPARENCY

	byte *scr_taa = &scrn->ta[y][x];
	char *scr_tcc = &scrn->tc[y][x];

#ifdef USE_EGO_GRAPHICS

	byte *scr_eaa = &scrn->ea[y][x];
	char *scr_ecc = &scrn->ec[y][x];

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

	while (n--)
	{

#ifdef USE_TRANSPARENCY

#ifdef USE_EGO_GRAPHICS
		/* Hack -- Ignore non-changes */
		if ((*scr_aa == *a) && (*scr_cc == *c) &&
		                (*scr_taa == *ta) && (*scr_tcc == *tc) &&
		                (*scr_eaa == *ea) && (*scr_ecc == *ec))
#else /* USE_EGO_GRAPHICS */
		/* Hack -- Ignore non-changes */
		if ((*scr_aa == *a) && (*scr_cc == *c) &&
		                (*scr_taa == *ta) && (*scr_tcc == *tc))
#endif  /* USE_EGO_GRAPHICS */
		{
			x++;
			a++;
			c++;
			ta++;
			tc++;
#ifdef USE_EGO_GRAPHICS
			ea++;
			ec++;
#endif /* USE_EGO_GRAPHICS */
			scr_aa++;
			scr_cc++;
			scr_taa++;
			scr_tcc++;
#ifdef USE_EGO_GRAPHICS
			scr_eaa++;
			scr_ecc++;
#endif /* USE_EGO_GRAPHICS */
			continue;
		}

		/* Save the "literal" information */
		*scr_taa++ = *ta++;
		*scr_tcc++ = *tc++;

#ifdef USE_EGO_GRAPHICS
		/* Save the "literal" information */
		*scr_eaa++ = *ea++;
		*scr_ecc++ = *ec++;
#endif /* USE_EGO_GRAPHICS */

#else /* USE_TRANSPARENCY */

		/* Hack -- Ignore non-changes */
		if ((*scr_aa == *a) && (*scr_cc == *c))
		{
			x++;
			a++;
			c++;
			scr_aa++;
			scr_cc++;
			continue;
		}

#endif /* USE_TRANSPARENCY */

		/* Save the "literal" information */
		*scr_aa++ = *a++;
		*scr_cc++ = *c++;

		/* Track minimum changed column */
		if (x1 < 0) x1 = x;

		/* Track maximum changed column */
		x2 = x;

		x++;
	}

	/* Expand the "change area" as needed */
	if (x1 >= 0)
	{
		/* Check for new min/max row info */
		if (y < Term->y1) Term->y1 = y;
		if (y > Term->y2) Term->y2 = y;

		/* Check for new min/max col info in this row */
		if (x1 < Term->x1[y]) Term->x1[y] = x1;
		if (x2 > Term->x2[y]) Term->x2[y] = x2;
	}
}



/*
 * Mentally draw some attr/chars at a given location
 *
 * Assumes that (x,y) is a valid location, that the first "n" characters
 * of the string "s" are all valid (non-zero), and that (x+n-1,y) is also
 * a valid location, so the first "n" characters of "s" can all be added
 * starting at (x,y) without causing any illegal operations.
 */
void Term_queue_chars(s32b x, s32b y, s32b n, byte a, cptr s)
{
	s32b x1 = -1, x2 = -1;

	byte *scr_aa = Term->scr->a[y];
	char *scr_cc = Term->scr->c[y];

#ifdef USE_TRANSPARENCY

	byte *scr_taa = Term->scr->ta[y];
	char *scr_tcc = Term->scr->tc[y];

#ifdef USE_EGO_GRAPHICS

	byte *scr_eaa = Term->scr->ea[y];
	char *scr_ecc = Term->scr->ec[y];

#endif /* USE_EGO_GRAPHICS */
#endif /* USE_TRANSPARENCY */

	/* Queue the attr/chars */
	for ( ; n; x++, s++, n--)
	{
		s32b oa = scr_aa[x];
		s32b oc = scr_cc[x];

#ifdef USE_TRANSPARENCY

		s32b ota = scr_taa[x];
		s32b otc = scr_tcc[x];

#ifdef USE_EGO_GRAPHICS

		s32b oea = scr_eaa[x];
		s32b oec = scr_ecc[x];

		/* Hack -- Ignore non-changes */
		if ((oa == a) && (oc == *s) &&
		                (ota == 0) && (otc == 0) &&
		                (oea == 0) && (oec == 0)) continue;

#else /* USE_EGO_GRAPHICS */

		/* Hack -- Ignore non-changes */
		if ((oa == a) && (oc == *s) && (ota == 0) && (otc == 0)) continue;

#endif /* USE_EGO_GRAPHICS */

#else /* USE_TRANSPARENCY */

		/* Hack -- Ignore non-changes */
		if ((oa == a) && (oc == *s)) continue;

#endif /* USE_TRANSPARENCY */

		/* Save the "literal" information */
		scr_aa[x] = a;
		scr_cc[x] = *s;

#ifdef USE_TRANSPARENCY

		scr_taa[x] = 0;
		scr_tcc[x] = 0;

#ifdef USE_EGO_GRAPHICS

		scr_taa[x] = 0;
		scr_tcc[x] = 0;

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

		/* Note the "range" of window updates */
		if (x1 < 0) x1 = x;
		x2 = x;
	}

	/* Expand the "change area" as needed */
	if (x1 >= 0)
	{
		/* Check for new min/max row info */
		if (y < Term->y1) Term->y1 = y;
		if (y > Term->y2) Term->y2 = y;

		/* Check for new min/max col info in this row */
		if (x1 < Term->x1[y]) Term->x1[y] = x1;
		if (x2 > Term->x2[y]) Term->x2[y] = x2;
	}
}



/*** Refresh routines ***/


/*
 * Flush a row of the current window (see "Term_fresh")
 *
 * Display text using "Term_pict()"
 */
static void Term_fresh_row_pict(s32b y, s32b x1, s32b x2)
{
	s32b x;

	byte *old_aa = Term->old->a[y];
	char *old_cc = Term->old->c[y];

	byte *scr_aa = Term->scr->a[y];
	char *scr_cc = Term->scr->c[y];

#ifdef USE_TRANSPARENCY

	byte *old_taa = Term->old->ta[y];
	char *old_tcc = Term->old->tc[y];

	byte *scr_taa = Term->scr->ta[y];
	char *scr_tcc = Term->scr->tc[y];

	byte ota;
	char otc;

	byte nta;
	char ntc;

#ifdef USE_EGO_GRAPHICS

	byte *old_eaa = Term->old->ea[y];
	char *old_ecc = Term->old->ec[y];

	byte *scr_eaa = Term->scr->ea[y];
	char *scr_ecc = Term->scr->ec[y];

	byte oea;
	char oec;

	byte nea;
	char nec;

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */


	/* Pending length */
	s32b fn = 0;

	/* Pending start */
	s32b fx = 0;

	byte oa;
	char oc;

	byte na;
	char nc;

	/* Scan "modified" columns */
	for (x = x1; x <= x2; x++)
	{
		/* See what is currently here */
		oa = old_aa[x];
		oc = old_cc[x];

		/* See what is desired there */
		na = scr_aa[x];
		nc = scr_cc[x];

#ifdef USE_TRANSPARENCY

		ota = old_taa[x];
		otc = old_tcc[x];

		nta = scr_taa[x];
		ntc = scr_tcc[x];

#ifdef USE_EGO_GRAPHICS

		oea = old_eaa[x];
		oec = old_ecc[x];

		nea = scr_eaa[x];
		nec = scr_ecc[x];

		/* Handle unchanged grids */
		if ((na == oa) && (nc == oc) &&
		                (nta == ota) && (ntc == otc) &&
		                (nea == oea) && (nec == oec))

#else /* USE_EGO_GRAPHICS */

		/* Handle unchanged grids */
		if ((na == oa) && (nc == oc) && (nta == ota) && (ntc == otc))

#endif /* USE_EGO_GRAPHICS */

#else /* USE_TRANSPARENCY */

		/* Handle unchanged grids */
		if ((na == oa) && (nc == oc))

#endif /* USE_TRANSPARENCY */
		{
			/* Flush */
			if (fn)
			{
				/* Draw pending attr/char pairs */
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
				(void)((*Term->pict_hook)(fx, y, fn,
				                          &scr_aa[fx], &scr_cc[fx],
				                          &scr_taa[fx], &scr_tcc[fx],
				                          &scr_eaa[fx], &scr_ecc[fx]));
#else /* USE_EGO_GRAPHICS */
				(void)((*Term->pict_hook)(fx, y, fn,
				                          &scr_aa[fx], &scr_cc[fx], &scr_taa[fx], &scr_tcc[fx]));
#endif
#else /* USE_TRANSPARENCY */
				(void)((*Term->pict_hook)(fx, y, fn, &scr_aa[fx], &scr_cc[fx]));
#endif /* USE_TRANSPARENCY */

				/* Forget */
				fn = 0;
			}

			/* Skip */
			continue;
		}
		/* Save new contents */
		old_aa[x] = na;
		old_cc[x] = nc;

#ifdef USE_TRANSPARENCY
		old_taa[x] = nta;
		old_tcc[x] = ntc;

#ifdef USE_EGO_GRAPHICS
		old_eaa[x] = nea;
		old_ecc[x] = nec;
#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

		/* Restart and Advance */
		if (fn++ == 0) fx = x;
	}

	/* Flush */
	if (fn)
	{
		/* Draw pending attr/char pairs */
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
		(void)((*Term->pict_hook)(fx, y, fn,
		                          &scr_aa[fx], &scr_cc[fx],
		                          &scr_taa[fx], &scr_tcc[fx],
		                          &scr_eaa[fx], &scr_ecc[fx]));
#else /* USE_EGO_GRAPHICS */
		(void)((*Term->pict_hook)(fx, y, fn,
		                          &scr_aa[fx], &scr_cc[fx], &scr_taa[fx], &scr_tcc[fx]));
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
		(void)((*Term->pict_hook)(fx, y, fn, &scr_aa[fx], &scr_cc[fx]));
#endif /* USE_TRANSPARENCY */
	}
}



/*
 * Flush a row of the current window (see "Term_fresh")
 *
 * Display text using "Term_text()" and "Term_wipe()",
 * but use "Term_pict()" for high-bit attr/char pairs
 */
static void Term_fresh_row_both(s32b y, s32b x1, s32b x2)
{
	s32b x;

	byte *old_aa = Term->old->a[y];
	char *old_cc = Term->old->c[y];

	byte *scr_aa = Term->scr->a[y];
	char *scr_cc = Term->scr->c[y];

#ifdef USE_TRANSPARENCY
	byte *old_taa = Term->old->ta[y];
	char *old_tcc = Term->old->tc[y];
	byte *scr_taa = Term->scr->ta[y];
	char *scr_tcc = Term->scr->tc[y];

	byte ota;
	char otc;
	byte nta;
	char ntc;

#ifdef USE_EGO_GRAPHICS
	byte *old_eaa = Term->old->ea[y];
	char *old_ecc = Term->old->ec[y];
	byte *scr_eaa = Term->scr->ea[y];
	char *scr_ecc = Term->scr->ec[y];

	byte oea;
	char oec;
	byte nea;
	char nec;
#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

	/* The "always_text" flag */
	s32b always_text = Term->always_text;

	/* Pending length */
	s32b fn = 0;

	/* Pending start */
	s32b fx = 0;

	/* Pending attr */
	byte fa = Term->attr_blank;

	byte oa;
	char oc;

	byte na;
	char nc;

	/* Scan "modified" columns */
	for (x = x1; x <= x2; x++)
	{
		/* See what is currently here */
		oa = old_aa[x];
		oc = old_cc[x];

		/* See what is desired there */
		na = scr_aa[x];
		nc = scr_cc[x];

#ifdef USE_TRANSPARENCY

		ota = old_taa[x];
		otc = old_tcc[x];

		nta = scr_taa[x];
		ntc = scr_tcc[x];

#ifdef USE_EGO_GRAPHICS
		oea = old_eaa[x];
		oec = old_ecc[x];

		nea = scr_eaa[x];
		nec = scr_ecc[x];

		/* Handle unchanged grids */
		if ((na == oa) && (nc == oc) &&
		                (nta == ota) && (ntc == otc) &&
		                (nea == oea) && (nec == oec))

#else /* USE_EGO_GRAPHICS */

/* Handle unchanged grids */
		if ((na == oa) && (nc == oc) && (nta == ota) && (ntc == otc))

#endif /* USE_EGO_GRAPHICS */

#else /* USE_TRANSPARENCY */

		/* Handle unchanged grids */
		if ((na == oa) && (nc == oc))

#endif /* USE_TRANSPARENCY */

		{
			/* Flush */
			if (fn)
			{
				/* Draw pending chars (normal) */
				if (fa || always_text)
				{
					(void)((*Term->text_hook)(fx, y, fn, fa, &scr_cc[fx]));
				}
				/* Draw pending chars (black) */
				else
				{
					(void)((*Term->wipe_hook)(fx, y, fn));
				}
				/* Forget */
				fn = 0;
			}

			/* Skip */
			continue;
		}

		/* Save new contents */
		old_aa[x] = na;
		old_cc[x] = nc;

#ifdef USE_TRANSPARENCY

		old_taa[x] = nta;
		old_tcc[x] = ntc;

#ifdef USE_EGO_GRAPHICS

		old_eaa[x] = nea;
		old_ecc[x] = nec;

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

		/* 2nd byte of bigtile */
		if (na == 255) continue;

		/* Handle high-bit attr/chars */
		if (na & 0x80)
		{
			/* Flush */
			if (fn)
			{
				/* Draw pending chars (normal) */
				if (fa || always_text)
				{
					(void)((*Term->text_hook)(fx, y, fn, fa, &scr_cc[fx]));
				}
				/* Draw pending chars (black) */
				else
				{
					(void)((*Term->wipe_hook)(fx, y, fn));
				}
				/* Forget */
				fn = 0;
			}

#ifdef USE_TRANSPARENCY

#ifdef USE_EGO_GRAPHICS

			/* Hack -- Draw the special attr/char pair */
			(void)((*Term->pict_hook)(x, y, 1, &na, &nc, &nta, &ntc, &nea, &nec));

#else /* USE_EGO_GRAPHICS */

/* Hack -- Draw the special attr/char pair */
			(void)((*Term->pict_hook)(x, y, 1, &na, &nc, &nta, &ntc));

#endif /* USE_EGO_GRAPHICS */

#else /* USE_TRANSPARENCY */

			/* Hack -- Draw the special attr/char pair */
			(void)((*Term->pict_hook)(x, y, 1, &na, &nc));

#endif /* USE_TRANSPARENCY */

			/* Skip */
			continue;
		}

		/* Notice new color */
		if (fa != na)
		{
			/* Flush */
			if (fn)
			{
				/* Draw the pending chars */
				if (fa || always_text)
				{
					(void)((*Term->text_hook)(fx, y, fn, fa, &scr_cc[fx]));
				}
				/* Hack -- Erase "leading" spaces */
				else
				{
					(void)((*Term->wipe_hook)(fx, y, fn));
				}
				/* Forget */
				fn = 0;
			}

			/* Save the new color */
			fa = na;
		}

		/* Restart and Advance */
		if (fn++ == 0) fx = x;
	}

	/* Flush */
	if (fn)
	{
		/* Draw pending chars (normal) */
		if (fa || always_text)
		{
			(void)((*Term->text_hook)(fx, y, fn, fa, &scr_cc[fx]));
		}
		/* Draw pending chars (black) */
		else
		{
			(void)((*Term->wipe_hook)(fx, y, fn));
		}
	}
}


/*
 * Flush a row of the current window (see "Term_fresh")
 *
 * Display text using "Term_text()" and "Term_wipe()"
 */
static void Term_fresh_row_text(s32b y, s32b x1, s32b x2)
{
	s32b x;

	byte *old_aa = Term->old->a[y];
	char *old_cc = Term->old->c[y];

	byte *scr_aa = Term->scr->a[y];
	char *scr_cc = Term->scr->c[y];

	/* The "always_text" flag */
	s32b always_text = Term->always_text;

	/* Pending length */
	s32b fn = 0;

	/* Pending start */
	s32b fx = 0;

	/* Pending attr */
	byte fa = Term->attr_blank;

	byte oa;
	char oc;

	byte na;
	char nc;

	/* Scan "modified" columns */
	for (x = x1; x <= x2; x++)
	{
		/* See what is currently here */
		oa = old_aa[x];
		oc = old_cc[x];

		/* See what is desired there */
		na = scr_aa[x];
		nc = scr_cc[x];

		/* Handle unchanged grids */
		if ((na == oa) && (nc == oc))
		{
			/* Flush */
			if (fn)
			{
				/* Draw pending chars (normal) */
				if (fa || always_text)
				{
					(void)((*Term->text_hook)(fx, y, fn, fa, &scr_cc[fx]));
				}

				/* Draw pending chars (black) */
				else
				{
					(void)((*Term->wipe_hook)(fx, y, fn));
				}

				/* Forget */
				fn = 0;
			}

			/* Skip */
			continue;
		}

		/* Save new contents */
		old_aa[x] = na;
		old_cc[x] = nc;

		/* Notice new color */
		if (fa != na)
		{
			/* Flush */
			if (fn)
			{
				/* Draw the pending chars */
				if (fa || always_text)
				{
					(void)((*Term->text_hook)(fx, y, fn, fa, &scr_cc[fx]));
				}

				/* Hack -- Erase "leading" spaces */
				else
				{
					(void)((*Term->wipe_hook)(fx, y, fn));
				}

				/* Forget */
				fn = 0;
			}

			/* Save the new color */
			fa = na;
		}

		/* Restart and Advance */
		if (fn++ == 0) fx = x;
	}

	/* Flush */
	if (fn)
	{
		/* Draw pending chars (normal) */
		if (fa || always_text)
		{
			(void)((*Term->text_hook)(fx, y, fn, fa, &scr_cc[fx]));
		}

		/* Draw pending chars (black) */
		else
		{
			(void)((*Term->wipe_hook)(fx, y, fn));
		}
	}
}





/*
 * Actually perform all requested changes to the window
 *
 * If absolutely nothing has changed, not even temporarily, or if the
 * current "Term" is not mapped, then this function will return 1 and
 * do absolutely nothing.
 *
 * Note that when "soft_cursor" is true, we erase the cursor (if needed)
 * whenever anything has changed, and redraw it (if needed) after all of
 * the screen updates are complete.  This will induce a small amount of
 * "cursor flicker" but only when the screen has been updated.  If the
 * screen is updated and then restored, you may still get this flicker.
 *
 * When "soft_cursor" is not true, we make the cursor invisible before
 * doing anything else if it is supposed to be invisible by the time we
 * are done, and we make it visible after moving it to its final location
 * after all of the screen updates are complete.
 *
 * Note that "Term_xtra(TERM_XTRA_CLEAR,0)" must erase the entire screen,
 * including the cursor, if needed, and may place the cursor anywhere.
 *
 * Note that "Term_xtra(TERM_XTRA_FROSH,y)" will be always be called
 * after any row "y" has been "flushed", unless the "Term->never_frosh"
 * flag is set, and "Term_xtra(TERM_XTRA_FRESH,0)" will be called after
 * all of the rows have been "flushed".
 *
 * Note the use of three different functions to handle the actual flush,
 * based on the settings of the "Term->always_pict" and "Term->higher_pict"
 * flags (see below).
 *
 * The three helper functions (above) work by collecting similar adjacent
 * grids into stripes, and then sending each stripe to "Term->pict_hook",
 * "Term->text_hook", or "Term->wipe_hook", based on the settings of the
 * "Term->always_pict" and "Term->higher_pict" flags, which select which
 * of the helper functions to call to flush each row.
 *
 * The helper functions currently "skip" any grids which already contain
 * the desired contents.  This may or may not be the best method, especially
 * when the desired content fits nicely into the current stripe.  For example,
 * it might be better to go ahead and queue them while allowed, but keep a
 * count of the "trailing skipables", then, when time to flush, or when a
 * "non skippable" is found, force a flush if there are too many skippables.
 *
 * Perhaps an "initialization" stage, where the "text" (and "attr")
 * buffers are "filled" with information, converting "blanks" into
 * a convenient representation, and marking "skips" with "zero chars",
 * and then some "processing" is done to determine which chars to skip.
 *
 * Currently, the helper functions are optimal for systems which prefer
 * to "print a char + move a char + print a char" to "print three chars",
 * and for applications that do a lot of "detailed" color printing.
 *
 * In the two "queue" functions, total "non-changes" are "pre-skipped".
 * The helper functions must also handle situations in which the contents
 * of a grid are changed, but then changed back to the original value,
 * and situations in which two grids in the same row are changed, but
 * the grids between them are unchanged.
 *
 * If the "Term->always_pict" flag is set, then "Term_fresh_row_pict()"
 * will be used instead of "Term_fresh_row_text()".  This allows all the
 * modified grids to be collected into stripes of attr/char pairs, which
 * are then sent to the "Term->pict_hook" hook, which can draw these pairs
 * in whatever way it would like.
 *
 * If the "Term->higher_pict" flag is set, then "Term_fresh_row_both()"
 * will be used instead of "Term_fresh_row_text()".  This allows all the
 * "special" attr/char pairs (in which both the attr and char have the
 * high-bit set) to be sent (one pair at a time) to the "Term->pict_hook"
 * hook, which can draw these pairs in whatever way it would like.
 *
 * Normally, the "Term_wipe()" function is used only to display "blanks"
 * that were induced by "Term_clear()" or "Term_erase()", and then only
 * if the "attr_blank" and "char_blank" fields have not been redefined
 * to use "white space" instead of the default "black space".  Actually,
 * the "Term_wipe()" function is used to display all "black" text, such
 * as the default "spaces" created by "Term_clear()" and "Term_erase()".
 *
 * Note that the "Term->always_text" flag will disable the use of the
 * "Term_wipe()" function hook entirely, and force all text, even text
 * drawn in the color "black", to be explicitly drawn.  This is useful
 * for machines which implement "Term_wipe()" by just drawing spaces.
 *
 * Note that the "Term->always_pict" flag will disable the use of the
 * "Term_wipe()" function entirely, and force everything, even text
 * drawn in the attr "black", to be explicitly drawn.
 *
 * Note that if no "black" text is ever drawn, and if "attr_blank" is
 * not "zero", then the "Term_wipe" hook will never be used, even if
 * the "Term->always_text" flag is not set.
 *
 * This function does nothing unless the "Term" is "mapped", which allows
 * certain systems to optimize the handling of "closed" windows.
 *
 * On systems with a "soft" cursor, we must explicitly erase the cursor
 * before flushing the output, if needed, to prevent a "jumpy" refresh.
 * The actual method for this is horrible, but there is very little that
 * we can do to simplify it efficiently.  XXX XXX XXX
 *
 * On systems with a "hard" cursor, we will "hide" the cursor before
 * flushing the output, if needed, to avoid a "flickery" refresh.  It
 * would be nice to *always* hide the cursor during the refresh, but
 * this might be expensive (and/or ugly) on some machines.
 *
 * The "Term->icky_corner" flag is used to avoid calling "Term_wipe()"
 * or "Term_pict()" or "Term_text()" on the bottom right corner of the
 * window, which might induce "scrolling" or other nasty stuff on old
 * dumb terminals.  This flag is handled very efficiently.  We assume
 * that the "Term_curs()" call will prevent placing the cursor in the
 * corner, if needed, though I doubt such placement is ever a problem.
 * Currently, the use of "Term->icky_corner" and "Term->soft_cursor"
 * together may result in undefined behavior.
 */
errr Term_fresh(void)
{
	s32b x, y;

	s32b w = Term->wid;
	s32b h = Term->hgt;

	s32b y1 = Term->y1;
	s32b y2 = Term->y2;

	term_win *old = Term->old;
	term_win *scr = Term->scr;


	/* Do nothing unless "mapped" */
	if (!Term->mapped_flag) return (1);


	/* Trivial Refresh */
	if ((y1 > y2) &&
	                (scr->cu == old->cu) &&
	                (scr->cv == old->cv) &&
	                (scr->cx == old->cx) &&
	                (scr->cy == old->cy) &&
	                !(Term->total_erase))
	{
		/* Nothing */
		return (1);
	}


	/* Paranoia -- use "fake" hooks to prevent core dumps */
	if (!Term->curs_hook) Term->curs_hook = Term_curs_hack;
	if (!Term->wipe_hook) Term->wipe_hook = Term_wipe_hack;
	if (!Term->text_hook) Term->text_hook = Term_text_hack;
	if (!Term->pict_hook) Term->pict_hook = Term_pict_hack;


	/* Handle "total erase" */
	if (Term->total_erase)
	{
		byte na = Term->attr_blank;
		char nc = Term->char_blank;

		if ((do_movies == 1) && IN_MAINWINDOW)
		{
			if (!cmovie_get_msecond())
			{
				fprintf(movfile, "S:%ld:\n", cmov_delta_time_msec);
			}
			fprintf(movfile, "C:\n");
			last_paused = 0;
		}

		/* Physically erase the entire window */
		Term_xtra(TERM_XTRA_CLEAR, 0);

		/* Hack -- clear all "cursor" data */
		old->cv = old->cu = old->cx = old->cy = 0;

		/* Wipe each row */
		for (y = 0; y < h; y++)
		{
			byte *aa = old->a[y];
			char *cc = old->c[y];

#ifdef USE_TRANSPARENCY

			byte *taa = old->ta[y];
			char *tcc = old->tc[y];

#ifdef USE_EGO_GRAPHICS

			byte *eaa = old->ea[y];
			char *ecc = old->ec[y];

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */


			/* Wipe each column */
			for (x = 0; x < w; x++)
			{
				/* Wipe each grid */
				*aa++ = na;
				*cc++ = nc;

#ifdef USE_TRANSPARENCY

				*taa++ = na;
				*tcc++ = nc;

#ifdef USE_EGO_GRAPHICS

				*eaa++ = na;
				*ecc++ = nc;

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

			}
		}

		/* Redraw every row */
		Term->y1 = y1 = 0;
		Term->y2 = y2 = h - 1;

		/* Redraw every column */
		for (y = 0; y < h; y++)
		{
			Term->x1[y] = 0;
			Term->x2[y] = w - 1;
		}

		/* Forget "total erase" */
		Term->total_erase = FALSE;
	}


	/* Cursor update -- Erase old Cursor */
	if (Term->soft_cursor)
	{
		/* Cursor was visible */
		if (!old->cu && old->cv)
		{
			s32b tx = old->cx;
			s32b ty = old->cy;

			byte *old_aa = old->a[ty];
			char *old_cc = old->c[ty];

			byte oa = old_aa[tx];
			char oc = old_cc[tx];

#ifdef USE_TRANSPARENCY

			byte *old_taa = old->ta[ty];
			char *old_tcc = old->tc[ty];

			byte ota = old_taa[tx];
			char otc = old_tcc[tx];

#ifdef USE_EGO_GRAPHICS

			byte *old_eaa = old->ea[ty];
			char *old_ecc = old->ec[ty];

			byte oea = old_eaa[tx];
			char oec = old_ecc[tx];

#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

			/* Hack -- use "Term_pict()" always */
			if (Term->always_pict)
			{
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
				(void)((*Term->pict_hook)(tx, ty, 1, &oa, &oc, &ota, &otc, &oea, &oec));
#else /* USE_EGO_GRAPHICS */
				(void)((*Term->pict_hook)(tx, ty, 1, &oa, &oc, &ota, &otc));
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
				(void)((*Term->pict_hook)(tx, ty, 1, &oa, &oc));
#endif /* USE_TRANSPARENCY */
			}

			/* Hack -- use "Term_pict()" sometimes */
			else if (Term->higher_pict && (oa & 0x80))
			{
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
				(void)((*Term->pict_hook)(tx, ty, 1, &oa, &oc, &ota, &otc, &oea, &oec));
#else /* USE_EGO_GRAPHICS */
(void)((*Term->pict_hook)(tx, ty, 1, &oa, &oc, &ota, &otc));
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
				(void)((*Term->pict_hook)(tx, ty, 1, &oa, &oc));
#endif /* USE_TRANSPARENCY */
			}

			/* Hack -- restore the actual character */
			else if (oa || Term->always_text)
			{
				(void)((*Term->text_hook)(tx, ty, 1, oa, &oc));
			}

			/* Hack -- erase the grid */
			else
			{
				(void)((*Term->wipe_hook)(tx, ty, 1));
			}
		}
	}

	/* Cursor Update -- Erase old Cursor */
	else
	{
		/* Cursor will be invisible */
		if (scr->cu || !scr->cv)
		{
			/* Make the cursor invisible */
			Term_xtra(TERM_XTRA_SHAPE, 0);
		}
	}


	/* Something to update */
	if (y1 <= y2)
	{
		/* Handle "icky corner" */
		if (Term->icky_corner)
		{
			/* Avoid the corner */
			if (y2 >= h - 1)
			{
				/* Avoid the corner */
				if (Term->x2[h - 1] > w - 2)
				{
					/* Avoid the corner */
					Term->x2[h - 1] = w - 2;
				}
			}
		}


		/* Scan the "modified" rows */
		for (y = y1; y <= y2; ++y)
		{
			s32b x1 = Term->x1[y];
			s32b x2 = Term->x2[y];

			/* Flush each "modified" row */
			if (x1 <= x2)
			{
				if ((do_movies == 1) && IN_MAINWINDOW)
				{
					/* Most magic happens here */
					cmovie_record_line(y);
					last_paused = 0;
				}

				/* Always use "Term_pict()" */
				if (Term->always_pict)
				{
					/* Flush the row */
					Term_fresh_row_pict(y, x1, x2);
				}

				/* Sometimes use "Term_pict()" */
				else if (Term->higher_pict)
				{
					/* Flush the row */
					Term_fresh_row_both(y, x1, x2);
				}

				/* Never use "Term_pict()" */
				else
				{
					/* Flush the row */
					Term_fresh_row_text(y, x1, x2);
				}

				/* This row is all done */
				Term->x1[y] = w;
				Term->x2[y] = 0;

				/* Hack -- Flush that row (if allowed) */
				if (!Term->never_frosh) Term_xtra(TERM_XTRA_FROSH, y);
			}
		}

		/* No rows are invalid */
		Term->y1 = h;
		Term->y2 = 0;
	}


	/* Cursor update -- Show new Cursor */
	if (Term->soft_cursor)
	{
		/* Draw the cursor */
		if (!scr->cu && scr->cv)
		{
			/* Call the cursor display routine */
			(void)((*Term->curs_hook)(scr->cx, scr->cy));
		}
	}

	/* Cursor Update -- Show new Cursor */
	else
	{
		/* The cursor is useless, hide it */
		if (scr->cu)
		{
			/* Paranoia -- Put the cursor NEAR where it belongs */
			(void)((*Term->curs_hook)(w - 1, scr->cy));

			/* Make the cursor invisible */
			/* Term_xtra(TERM_XTRA_SHAPE, 0); */
		}

		/* The cursor is invisible, hide it */
		else if (!scr->cv)
		{
			/* Paranoia -- Put the cursor where it belongs */
			(void)((*Term->curs_hook)(scr->cx, scr->cy));

			/* Make the cursor invisible */
			/* Term_xtra(TERM_XTRA_SHAPE, 0); */
		}

		/* The cursor is visible, display it correctly */
		else
		{
			/* Put the cursor where it belongs */
			(void)((*Term->curs_hook)(scr->cx, scr->cy));

			/* Make the cursor visible */
			Term_xtra(TERM_XTRA_SHAPE, 1);
		}
	}


	/* Save the "cursor state" */
	old->cu = scr->cu;
	old->cv = scr->cv;
	old->cx = scr->cx;
	old->cy = scr->cy;


	/* Actually flush the output */
	Term_xtra(TERM_XTRA_FRESH, 0);


	/* Success */
	return (0);
}



/*** Output routines ***/


/*
 * Set the cursor visibility
 */
errr Term_set_cursor(s32b v)
{
	/* Already done */
	if (Term->scr->cv == v) return (1);

	/* Change */
	Term->scr->cv = v;

	/* Success */
	return (0);
}


/*
 * Place the cursor at a given location
 *
 * Note -- "illegal" requests do not move the cursor.
 */
errr Term_gotoxy(s32b x, s32b y)
{
	s32b w = Term->wid;
	s32b h = Term->hgt;

	/* Verify */
	if ((x < 0) || (x >= w)) return ( -1);
	if ((y < 0) || (y >= h)) return ( -1);

	/* Remember the cursor */
	Term->scr->cx = x;
	Term->scr->cy = y;

	/* The cursor is not useless */
	Term->scr->cu = 0;

	/* Success */
	return (0);
}


/*
 * At a given location, place an attr/char
 * Do not change the cursor position
 * No visual changes until "Term_fresh()".
 */
errr Term_draw(s32b x, s32b y, byte a, char c)
{
	s32b w = Term->wid;
	s32b h = Term->hgt;

	/* Verify location */
	if ((x < 0) || (x >= w)) return ( -1);
	if ((y < 0) || (y >= h)) return ( -1);

	/* Paranoia -- illegal char */
	if (!c) return ( -2);

	/* Queue it for later */
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
	Term_queue_char(x, y, a, c, 0, 0, 0, 0);
#else /* USE_EGO_GRAPHICS */
Term_queue_char(x, y, a, c, 0, 0);
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
	Term_queue_char(x, y, a, c);
#endif /* USE_TRANSPARENCY */

	/* Success */
	return (0);
}


/*
 * Using the given attr, add the given char at the cursor.
 *
 * We return "-2" if the character is "illegal". XXX XXX
 *
 * We return "-1" if the cursor is currently unusable.
 *
 * We queue the given attr/char for display at the current
 * cursor location, and advance the cursor to the right,
 * marking it as unuable and returning "1" if it leaves
 * the screen, and otherwise returning "0".
 *
 * So when this function, or the following one, return a
 * positive value, future calls to either function will
 * return negative ones.
 */
errr Term_addch(byte a, char c)
{
	s32b w = Term->wid;

	/* Handle "unusable" cursor */
	if (Term->scr->cu) return ( -1);

	/* Paranoia -- no illegal chars */
	if (!c) return ( -2);

	/* Queue the given character for display */
#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
	Term_queue_char(Term->scr->cx, Term->scr->cy, a, c, 0, 0, 0, 0);
#else /* USE_EGO_GRAPHICS */
	Term_queue_char(Term->scr->cx, Term->scr->cy, a, c, 0, 0);
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
	Term_queue_char(Term->scr->cx, Term->scr->cy, a, c);
#endif /* USE_TRANSPARENCY */

	/* Advance the cursor */
	Term->scr->cx++;

	/* Success */
	if (Term->scr->cx < w) return (0);

	/* Note "Useless" cursor */
	Term->scr->cu = 1;

	/* Note "Useless" cursor */
	return (1);
}


/*
 * At the current location, using an attr, add a string
 *
 * We also take a length "n", using negative values to imply
 * the largest possible value, and then we use the minimum of
 * this length and the "actual" length of the string as the
 * actual number of characters to attempt to display, never
 * displaying more characters than will actually fit, since
 * we do NOT attempt to "wrap" the cursor at the screen edge.
 *
 * We return "-1" if the cursor is currently unusable.
 * We return "N" if we were "only" able to write "N" chars,
 * even if all of the given characters fit on the screen,
 * and mark the cursor as unusable for future attempts.
 *
 * So when this function, or the preceding one, return a
 * positive value, future calls to either function will
 * return negative ones.
 */
errr Term_addstr(s32b n, byte a, cptr s)
{
	s32b k;

	s32b w = Term->wid;

	errr res = 0;

	/* Handle "unusable" cursor */
	if (Term->scr->cu) return ( -1);

	/* Obtain maximal length */
	k = (n < 0) ? (w + 1) : n;

	/* Obtain the usable string length */
	for (n = 0; (n < k) && s[n]; n++) /* loop */;

	/* React to reaching the edge of the screen */
	if (Term->scr->cx + n >= w) res = n = w - Term->scr->cx;

	/* Queue the first "n" characters for display */
	Term_queue_chars(Term->scr->cx, Term->scr->cy, n, a, s);

	/* Advance the cursor */
	Term->scr->cx += n;

	/* Hack -- Notice "Useless" cursor */
	if (res) Term->scr->cu = 1;

	/* Success (usually) */
	return (res);
}


/*
 * Move to a location and, using an attr, add a char
 */
errr Term_putch(s32b x, s32b y, byte a, char c)
{
	errr res;

	/* Move first */
	if ((res = Term_gotoxy(x, y)) != 0) return (res);

	/* Then add the char */
	if ((res = Term_addch(a, c)) != 0) return (res);

	/* Success */
	return (0);
}


/*
 * Move to a location and, using an attr, add a string
 */
errr Term_putstr(s32b x, s32b y, s32b n, byte a, cptr s)
{
	errr res;

	/* Move first */
	if ((res = Term_gotoxy(x, y)) != 0) return (res);

	/* Then add the string */
	if ((res = Term_addstr(n, a, s)) != 0) return (res);

	/* Success */
	return (0);
}



/*
 * Place cursor at (x,y), and clear the next "n" chars
 */
errr Term_erase(s32b x, s32b y, s32b n)
{
	s32b i;

	s32b w = Term->wid;
	/* s32b h = Term->hgt; */

	s32b x1 = -1;
	s32b x2 = -1;

	s32b na = Term->attr_blank;
	s32b nc = Term->char_blank;

	byte *scr_aa;
	char *scr_cc;

#ifdef USE_TRANSPARENCY
	byte *scr_taa;
	char *scr_tcc;

#ifdef USE_EGO_GRAPHICS
	byte *scr_eaa;
	char *scr_ecc;
#endif /* USE_EGO_GRAPHICS */
#endif /* USE_TRANSPARENCY */

	/* Place cursor */
	if (Term_gotoxy(x, y)) return ( -1);

	/* Force legal size */
	if (x + n > w) n = w - x;

	/* Fast access */
	scr_aa = Term->scr->a[y];
	scr_cc = Term->scr->c[y];

#ifdef USE_TRANSPARENCY
	scr_taa = Term->scr->ta[y];
	scr_tcc = Term->scr->tc[y];

#ifdef USE_EGO_GRAPHICS
	scr_eaa = Term->scr->ea[y];
	scr_ecc = Term->scr->ec[y];
#endif
#endif /* USE_TRANSPARENCY */

	if (n > 0 && (byte)scr_cc[x] == 255 && scr_aa[x] == 255)
	{
		x--;
		n++;
	}

	/* Scan every column */
	for (i = 0; i < n; i++, x++)
	{
		s32b oa = scr_aa[x];
		s32b oc = scr_cc[x];

		/* Hack -- Ignore "non-changes" */
		if ((oa == na) && (oc == nc)) continue;

		/* Save the "literal" information */
		scr_aa[x] = na;
		scr_cc[x] = nc;

#ifdef USE_TRANSPARENCY
		scr_taa[x] = 0;
		scr_tcc[x] = 0;

#ifdef USE_EGO_GRAPHICS
		scr_eaa[x] = 0;
		scr_ecc[x] = 0;
#endif /* USE_EGO_GRAPHICS */
#endif /* USE_TRANSPARENCY */

		/* Track minimum changed column */
		if (x1 < 0) x1 = x;

		/* Track maximum changed column */
		x2 = x;
	}

	/* Expand the "change area" as needed */
	if (x1 >= 0)
	{
		/* Check for new min/max row info */
		if (y < Term->y1) Term->y1 = y;
		if (y > Term->y2) Term->y2 = y;

		/* Check for new min/max col info in this row */
		if (x1 < Term->x1[y]) Term->x1[y] = x1;
		if (x2 > Term->x2[y]) Term->x2[y] = x2;
	}

	/* Success */
	return (0);
}


/*
 * Clear the entire window, and move to the top left corner
 *
 * Note the use of the special "total_erase" code
 */
errr Term_clear(void)
{
	s32b x, y;

	s32b w = Term->wid;
	s32b h = Term->hgt;

	byte na = Term->attr_blank;
	char nc = Term->char_blank;

	/* Cursor usable */
	Term->scr->cu = 0;

	/* Cursor to the top left */
	Term->scr->cx = Term->scr->cy = 0;

	/* Wipe each row */
	for (y = 0; y < h; y++)
	{
		byte *scr_aa = Term->scr->a[y];
		char *scr_cc = Term->scr->c[y];

#ifdef USE_TRANSPARENCY
		byte *scr_taa = Term->scr->ta[y];
		char *scr_tcc = Term->scr->tc[y];

#ifdef USE_EGO_GRAPHICS
		byte *scr_eaa = Term->scr->ea[y];
		char *scr_ecc = Term->scr->ec[y];
#endif /* USE_EGO_GRAPHICS */
#endif /* USE_TRANSPARENCY */

		/* Wipe each column */
		for (x = 0; x < w; x++)
		{
			scr_aa[x] = na;
			scr_cc[x] = nc;

#ifdef USE_TRANSPARENCY
			scr_taa[x] = 0;
			scr_tcc[x] = 0;

#ifdef USE_EGO_GRAPHICS
			scr_eaa[x] = 0;
			scr_ecc[x] = 0;
#endif /* USE_EGO_GRAPHICS */
#endif /* USE_TRANSPARENCY */
		}

		/* This row has changed */
		Term->x1[y] = 0;
		Term->x2[y] = w - 1;
	}

	/* Every row has changed */
	Term->y1 = 0;
	Term->y2 = h - 1;

	/* Force "total erase" */
	Term->total_erase = TRUE;

	/* Success */
	return (0);
}





/*
 * Redraw (and refresh) the whole window.
 */
errr Term_redraw(void)
{
	/* Pat */
	if ((do_movies == 1) && IN_MAINWINDOW)
	{
		if (!cmovie_get_msecond())
		{
			fprintf(movfile, "S:%ld:\n", cmov_delta_time_msec);
		}
		last_paused = 1;
	}
	/* Endpat */

	/* Force "total erase" */
	Term->total_erase = TRUE;

	/* Hack -- Refresh */
	Term_fresh();

	/* Success */
	return (0);
}


/*
 * Redraw part of a window.
 */
errr Term_redraw_section(s32b x1, s32b y1, s32b x2, s32b y2)
{
	s32b i, j;

	char *c_ptr;

	/* Bounds checking */
	if (y2 >= Term->hgt) y2 = Term->hgt - 1;
	if (x2 >= Term->wid) x2 = Term->wid - 1;
	if (y1 < 0) y1 = 0;
	if (x1 < 0) x1 = 0;

	/* Set y limits */
	Term->y1 = y1;
	Term->y2 = y2;

	/* Set the x limits */
	for (i = Term->y1; i <= Term->y2; i++)
	{
		Term->x1[i] = x1;
		Term->x2[i] = x2;

		c_ptr = Term->old->c[i];

		/* Clear the section so it is redrawn */
		for (j = x1; j <= x2; j++)
		{
			/* Hack - set the old character to "none" */
			c_ptr[j] = 0;
		}
	}

	/* Hack -- Refresh */
	Term_fresh();

	/* Success */
	return (0);
}



/*** Access routines ***/


/*
 * Extract the cursor visibility
 */
errr Term_get_cursor(s32b *v)
{
	/* Extract visibility */
	(*v) = Term->scr->cv;

	/* Success */
	return (0);
}


/*
 * Extract the current window size
 */
errr Term_get_size(s32b *w, s32b *h)
{
	/* Access the cursor */
	(*w) = Term->wid;
	(*h) = Term->hgt;

	/* Success */
	return (0);
}


/*
 * Extract the current cursor location
 */
errr Term_locate(s32b *x, s32b *y)
{
	/* Access the cursor */
	(*x) = Term->scr->cx;
	(*y) = Term->scr->cy;

	/* Warn about "useless" cursor */
	if (Term->scr->cu) return (1);

	/* Success */
	return (0);
}


/*
 * At a given location, determine the "current" attr and char
 * Note that this refers to what will be on the window after the
 * next call to "Term_fresh()".  It may or may not already be there.
 */
errr Term_what(s32b x, s32b y, byte *a, char *c)
{
	s32b w = Term->wid;
	s32b h = Term->hgt;

	/* Verify location */
	if ((x < 0) || (x >= w)) return ( -1);
	if ((y < 0) || (y >= h)) return ( -1);

	/* Direct access */
	(*a) = Term->scr->a[y][x];
	(*c) = Term->scr->c[y][x];

	/* Success */
	return (0);
}



/*** Input routines ***/


/*
 * Flush and forget the input
 */
errr Term_flush(void)
{
	/* Hack -- Flush all events */
	Term_xtra(TERM_XTRA_FLUSH, 0);

	/* Forget all keypresses */
	Term->key_head = Term->key_tail = 0;

	/* Success */
	return (0);
}



/*
 * Add a keypress to the "queue"
 */
errr Term_keypress(s32b k)
{
	/* Hack -- Refuse to enqueue non-keys */
	if (!k) return ( -1);

	/* Store the char, advance the queue */
	Term->key_queue[Term->key_head++] = k;

	/* Circular queue, handle wrap */
	if (Term->key_head == Term->key_size) Term->key_head = 0;

	/* Success (unless overflow) */
	if (Term->key_head != Term->key_tail) return (0);

#if 0
	/* Hack -- Forget the oldest key */
	if (++Term->key_tail == Term->key_size) Term->key_tail = 0;
#endif

	/* Problem */
	return (1);
}


/*
 * Add a keypress to the FRONT of the "queue"
 */
errr Term_key_push(s32b k)
{
	/* Hack -- Refuse to enqueue non-keys */
	if (!k) return ( -1);

	/* Hack -- Overflow may induce circular queue */
	if (Term->key_tail == 0) Term->key_tail = Term->key_size;

	/* Back up, Store the char */
	Term->key_queue[--Term->key_tail] = k;

	/* Success (unless overflow) */
	if (Term->key_head != Term->key_tail) return (0);

#if 0
	/* Hack -- Forget the oldest key */
	if (++Term->key_tail == Term->key_size) Term->key_tail = 0;
#endif

	/* Problem */
	return (1);
}





/*
 * Check for a pending keypress on the key queue.
 *
 * Store the keypress, if any, in "ch", and return "0".
 * Otherwise store "zero" in "ch", and return "1".
 *
 * Wait for a keypress if "wait" is true.
 *
 * Remove the keypress if "take" is true.
 */
errr Term_inkey(char *ch, bool wait, bool take)
{
	/* Assume no key */
	(*ch) = '\0';

	/* Hack -- get bored */
	if (!Term->never_bored)
	{
		/* Process random events */
		Term_xtra(TERM_XTRA_BORED, 0);
	}

	/* PatN */
	if ((do_movies == 1) && (last_paused == 0) && (!cmovie_get_msecond()))
	{
		fprintf(movfile, "S:%ld:\n", cmov_delta_time_msec);
		last_paused = 1;
	}
	/* PatNEnd */

	/* Wait */
	if (wait)
	{
		/* Process pending events while necessary */
		while (Term->key_head == Term->key_tail)
		{
			/* Process events (wait for one) */
			Term_xtra(TERM_XTRA_EVENT, TRUE);
		}
	}

	/* Do not Wait */
	else
	{
		/* Process pending events if necessary */
		if (Term->key_head == Term->key_tail)
		{
			/* Process events (do not wait) */
			Term_xtra(TERM_XTRA_EVENT, FALSE);
		}
	}

	/* No keys are ready */
	if (Term->key_head == Term->key_tail) return (1);

	/* Extract the next keypress */
	(*ch) = Term->key_queue[Term->key_tail];

	/* If requested, advance the queue, wrap around if necessary */
	if (take && (++Term->key_tail == Term->key_size)) Term->key_tail = 0;

	/* Success */
	return (0);
}



/*** Extra routines ***/


/*
 * Save the "requested" screen into the "memorized" screen
 *
 * Every "Term_save()" should match exactly one "Term_load()"
 */
errr Term_save(void)
{
	s32b w = Term->wid;
	s32b h = Term->hgt;

	/* Create */
	if (!Term->mem)
	{
		/* Allocate window */
		MAKE(Term->mem, term_win);

		/* Initialize window */
		term_win_init(Term->mem, w, h);
	}

	/* Grab */
	term_win_copy(Term->mem, Term->scr, w, h);

	/* Success */
	return (0);
}

/*
 * Same as before but can save more than once
 */
term_win* Term_save_to(void)
{
	s32b w = Term->wid;
	s32b h = Term->hgt;
	term_win *save;

	/* Allocate window */
	MAKE(save, term_win);

	/* Initialize window */
	term_win_init(save, w, h);

	/* Grab */
	term_win_copy(save, Term->scr, w, h);

	/* Success */
	return (save);
}

/*
 * Restore the "requested" contents (see above).
 *
 * Every "Term_save()" should match exactly one "Term_load()"
 */
errr Term_load(void)
{
	s32b y;

	s32b w = Term->wid;
	s32b h = Term->hgt;

	/* Create */
	if (!Term->mem)
	{
		/* Allocate window */
		MAKE(Term->mem, term_win);

		/* Initialize window */
		term_win_init(Term->mem, w, h);
	}

	/* Load */
	term_win_copy(Term->scr, Term->mem, w, h);

	/* Assume change */
	for (y = 0; y < h; y++)
	{
		/* Assume change */
		Term->x1[y] = 0;
		Term->x2[y] = w - 1;
	}

	/* Assume change */
	Term->y1 = 0;
	Term->y2 = h - 1;

	/* Success */
	return (0);
}

/*
 * Same as previous but allow to save more than one
 */
errr Term_load_from(term_win *save, bool final)
{
	s32b y;

	s32b w = Term->wid;
	s32b h = Term->hgt;

	/* Create */
	if (!save)
	{
		return (1);
	}

	/* Load */
	term_win_copy(Term->scr, save, w, h);

	/* Assume change */
	for (y = 0; y < h; y++)
	{
		/* Assume change */
		Term->x1[y] = 0;
		Term->x2[y] = w - 1;
	}

	/* Assume change */
	Term->y1 = 0;
	Term->y2 = h - 1;

	/* Free is requested */
	if (final)
	{
		term_win_nuke(save, Term->wid, Term->hgt);
	}

	/* Success */
	return (0);
}

/*
 * Exchange the "requested" screen with the "tmp" screen
 */
errr Term_exchange(void)
{
	s32b y;

	s32b w = Term->wid;
	s32b h = Term->hgt;

	term_win *exchanger;


	/* Create */
	if (!Term->tmp)
	{
		/* Allocate window */
		MAKE(Term->tmp, term_win);

		/* Initialize window */
		term_win_init(Term->tmp, w, h);
	}

	/* Swap */
	exchanger = Term->scr;
	Term->scr = Term->tmp;
	Term->tmp = exchanger;

	/* Assume change */
	for (y = 0; y < h; y++)
	{
		/* Assume change */
		Term->x1[y] = 0;
		Term->x2[y] = w - 1;
	}

	/* Assume change */
	Term->y1 = 0;
	Term->y2 = h - 1;

	/* Success */
	return (0);
}

/*
 * React to a new physical window size.
 */
errr Term_resize(s32b w, s32b h)
{
	s32b i;

	s32b wid, hgt;

	byte *hold_x1;
	byte *hold_x2;

	term_win *hold_old;
	term_win *hold_scr;
	term_win *hold_mem;
	term_win *hold_tmp;

	/* Resizing is forbidden */
	if (Term->fixed_shape) return ( -1);

	/* Ignore illegal changes */
	if ((w < 1) || (h < 1)) return ( -1);


	/* Ignore non-changes */
	if ((Term->wid == w) && (Term->hgt == h) && (arg_bigtile == use_bigtile)) return (1);
	use_bigtile = arg_bigtile;

	/* Minimum dimensions */
	wid = MIN(Term->wid, w);
	hgt = MIN(Term->hgt, h);

	/* Save scanners */
	hold_x1 = Term->x1;
	hold_x2 = Term->x2;

	/* Save old window */
	hold_old = Term->old;

	/* Save old window */
	hold_scr = Term->scr;

	/* Save old window */
	hold_mem = Term->mem;

	/* Save old window */
	hold_tmp = Term->tmp;

	/* Create new scanners */
	C_MAKE(Term->x1, h, byte);
	C_MAKE(Term->x2, h, byte);

	/* Create new window */
	MAKE(Term->old, term_win);

	/* Initialize new window */
	term_win_init(Term->old, w, h);

	/* Save the contents */
	term_win_copy(Term->old, hold_old, wid, hgt);

	/* Create new window */
	MAKE(Term->scr, term_win);

	/* Initialize new window */
	term_win_init(Term->scr, w, h);

	/* Save the contents */
	term_win_copy(Term->scr, hold_scr, wid, hgt);

	/* If needed */
	if (hold_mem)
	{
		/* Create new window */
		MAKE(Term->mem, term_win);

		/* Initialize new window */
		term_win_init(Term->mem, w, h);

		/* Save the contents */
		term_win_copy(Term->mem, hold_mem, wid, hgt);
	}

	/* If needed */
	if (hold_tmp)
	{
		/* Create new window */
		MAKE(Term->tmp, term_win);

		/* Initialize new window */
		term_win_init(Term->tmp, w, h);

		/* Save the contents */
		term_win_copy(Term->tmp, hold_tmp, wid, hgt);
	}

	/* Free some arrays */
	C_KILL(hold_x1, Term->hgt, byte);
	C_KILL(hold_x2, Term->hgt, byte);

	/* Nuke */
	term_win_nuke(hold_old, Term->wid, Term->hgt);

	/* Kill */
	KILL(hold_old, term_win);

	/* Illegal cursor */
	if (Term->old->cx >= w) Term->old->cu = 1;
	if (Term->old->cy >= h) Term->old->cu = 1;

	/* Nuke */
	term_win_nuke(hold_scr, Term->wid, Term->hgt);

	/* Kill */
	KILL(hold_scr, term_win);

	/* Illegal cursor */
	if (Term->scr->cx >= w) Term->scr->cu = 1;
	if (Term->scr->cy >= h) Term->scr->cu = 1;

	/* If needed */
	if (hold_mem)
	{
		/* Nuke */
		term_win_nuke(hold_mem, Term->wid, Term->hgt);

		/* Kill */
		KILL(hold_mem, term_win);

		/* Illegal cursor */
		if (Term->mem->cx >= w) Term->mem->cu = 1;
		if (Term->mem->cy >= h) Term->mem->cu = 1;
	}

	/* If needed */
	if (hold_tmp)
	{
		/* Nuke */
		term_win_nuke(hold_tmp, Term->wid, Term->hgt);

		/* Kill */
		KILL(hold_tmp, term_win);

		/* Illegal cursor */
		if (Term->tmp->cx >= w) Term->tmp->cu = 1;
		if (Term->tmp->cy >= h) Term->tmp->cu = 1;
	}

	/* Save new size */
	Term->wid = w;
	Term->hgt = h;

	/* Force "total erase" */
	Term->total_erase = TRUE;

	/* Assume change */
	for (i = 0; i < h; i++)
	{
		/* Assume change */
		Term->x1[i] = 0;
		Term->x2[i] = w - 1;
	}

	/* Assume change */
	Term->y1 = 0;
	Term->y2 = h - 1;

	/* Execute the "resize_hook" hook, if available */
	if (Term->resize_hook)
	{
		Term->resize_hook();
	}

	/* Success */
	return (0);
}



/*
 * Activate a new Term (and deactivate the current Term)
 *
 * This function is extremely important, and also somewhat bizarre.
 * It is the only function that should "modify" the value of "Term".
 *
 * To "create" a valid "term", one should do "term_init(t)", then
 * set the various flags and hooks, and then do "Term_activate(t)".
 */
errr Term_activate(term *t)
{
	/* Hack -- already done */
	if (Term == t) return (1);

	/* Deactivate the old Term */
	if (Term) Term_xtra(TERM_XTRA_LEVEL, 0);

	/* Hack -- Call the special "init" hook */
	if (t && !t->active_flag)
	{
		/* Call the "init" hook */
		if (t->init_hook) (*t->init_hook)(t);

		/* Remember */
		t->active_flag = TRUE;

		/* Assume mapped */
		t->mapped_flag = TRUE;
	}

	/* Remember the Term */
	Term = t;

	/* Activate the new Term */
	if (Term) Term_xtra(TERM_XTRA_LEVEL, 1);

	/* Success */
	return (0);
}



/*
 * Nuke a term
 */
errr term_nuke(term *t)
{
	s32b w = t->wid;
	s32b h = t->hgt;

	/* Hack -- Call the special "nuke" hook */
	if (t->active_flag)
	{
		/* Call the "nuke" hook */
		if (t->nuke_hook) (*t->nuke_hook)(t);

		/* Remember */
		t->active_flag = FALSE;

		/* Assume not mapped */
		t->mapped_flag = FALSE;
	}


	/* Nuke "displayed" */
	term_win_nuke(t->old, w, h);

	/* Kill "displayed" */
	KILL(t->old, term_win);

	/* Nuke "requested" */
	term_win_nuke(t->scr, w, h);

	/* Kill "requested" */
	KILL(t->scr, term_win);

	/* If needed */
	if (t->mem)
	{
		/* Nuke "memorized" */
		term_win_nuke(t->mem, w, h);

		/* Kill "memorized" */
		KILL(t->mem, term_win);
	}

	/* If needed */
	if (t->tmp)
	{
		/* Nuke "temporary" */
		term_win_nuke(t->tmp, w, h);

		/* Kill "temporary" */
		KILL(t->tmp, term_win);
	}

	/* Free some arrays */
	C_KILL(t->x1, h, byte);
	C_KILL(t->x2, h, byte);

	/* Free the input queue */
	C_KILL(t->key_queue, t->key_size, char);

	/* Success */
	return (0);
}


/*
 * Initialize a term, using a window of the given size.
 * Also prepare the "input queue" for "k" keypresses
 * By default, the cursor starts out "invisible"
 * By default, we "erase" using "black spaces"
 */
errr term_init(term *t, s32b w, s32b h, s32b k)
{
	s32b y;

	/* Wipe it */
	(void)WIPE(t, term);


	/* Prepare the input queue */
	t->key_head = t->key_tail = 0;

	/* Determine the input queue size */
	t->key_size = k;

	/* Allocate the input queue */
	C_MAKE(t->key_queue, t->key_size, char);


	/* Save the size */
	t->wid = w;
	t->hgt = h;

	/* Allocate change arrays */
	C_MAKE(t->x1, h, byte);
	C_MAKE(t->x2, h, byte);


	/* Allocate "displayed" */
	MAKE(t->old, term_win);

	/* Initialize "displayed" */
	term_win_init(t->old, w, h);


	/* Allocate "requested" */
	MAKE(t->scr, term_win);

	/* Initialize "requested" */
	term_win_init(t->scr, w, h);


	/* Assume change */
	for (y = 0; y < h; y++)
	{
		/* Assume change */
		t->x1[y] = 0;
		t->x2[y] = w - 1;
	}

	/* Assume change */
	t->y1 = 0;
	t->y2 = h - 1;

	/* Force "total erase" */
	t->total_erase = TRUE;


	/* Default "blank" */
	t->attr_blank = 0;
	t->char_blank = ' ';


	/* Success */
	return (0);
}

/*
 * Determine if we are called in the same second as the last time?
 * This *ASSUMES* that time_t is seconds past something. Is this portable?
 */
s32b cmovie_get_msecond(void)
{
#ifndef USE_PRECISE_CMOVIE
	/* Not very precise, but portable */
	time_t thisc;

	thisc = time(NULL);

	cmov_delta_time_msec = 300;

	if (thisc == lastc)
	{
		return 1;
	}
	return 0;
#else /* Very precise but needs main-foo.c to define TERM_XTRA_GET_DELAY */
Term_xtra(TERM_XTRA_GET_DELAY, 0);

	cmov_delta_time_msec = Term_xtra_long - cmov_last_time_msec;
	cmov_last_time_msec = Term_xtra_long;
	return 0;
#endif
}

void cmovie_init_second()
{
#ifndef USE_PRECISE_CMOVIE
	/* Not very precise, but portable */
	cmov_last_time_msec = 0;
#else /* Precise but need main-foo.c to have TERM_XTRA_GET_DELAY */
	Term_xtra(TERM_XTRA_GET_DELAY, 0);
	cmov_last_time_msec = Term_xtra_long;
#endif
}
