/* File: z-term.h */

/*
 * Copyright (c) 1997 Ben Harrison
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.
 */

#ifndef INCLUDED_Z_TERM_H
#define INCLUDED_Z_TERM_H

#include "h-basic.h"

#define IN_MAINWINDOW (Term == term_screen)

/*
 * A term_win is a "window" for a Term
 *
 *	- Cursor Useless/Visible codes
 *	- Cursor Location (see "Useless")
 *
 *	- Array[h] -- Access to the attribute array
 *	- Array[h] -- Access to the character array
 *
 *	- Array[h*w] -- Attribute array
 *	- Array[h*w] -- Character array
 *
 * Note that the attr/char pair at (x,y) is a[y][x]/c[y][x]
 * and that the row of attr/chars at (0,y) is a[y]/c[y]
 */

typedef struct term_win term_win;

struct term_win
{
	bool cu, cv;
	byte cx, cy;

	byte **a;
	char **c;

	byte *va;
	char *vc;

#ifdef USE_TRANSPARENCY
	byte **ta;
	char **tc;

	byte *vta;
	char *vtc;

#ifdef USE_EGO_GRAPHICS
	byte **ea;
	char **ec;

	byte *vea;
	char *vec;
#endif /* USE_EGO_GRAPHICS */

#endif /* USE_TRANSPARENCY */

};



/*
 * An actual "term" structure
 *
 *	- Extra "user" info (used by application)
 *
 *	- Extra "data" info (used by implementation)
 *
 *
 *	- Flag "user_flag"
 *	  An extra "user" flag (used by application)
 *
 *
 *	- Flag "data_flag"
 *	  An extra "data" flag (used by implementation)
 *
 *
 *	- Flag "active_flag"
 *	  This "term" is "active"
 *
 *	- Flag "mapped_flag"
 *	  This "term" is "mapped"
 *
 *	- Flag "total_erase"
 *	  This "term" should be fully erased
 *
 *	- Flag "fixed_shape"
 *	  This "term" is not allowed to resize
 *
 *	- Flag "icky_corner"
 *	  This "term" has an "icky" corner grid
 *
 *	- Flag "soft_cursor"
 *	  This "term" uses a "software" cursor
 *
 *	- Flag "always_pict"
 *	  Use the "Term_pict()" routine for all text
 *
 *	- Flag "higher_pict"
 *	  Use the "Term_pict()" routine for special text
 *
 *	- Flag "always_text"
 *	  Use the "Term_text()" routine for invisible text
 *
 *	- Flag "unused_flag"
 *	  Reserved for future use
 *
 *	- Flag "never_bored"
 *	  Never call the "TERM_XTRA_BORED" action
 *
 *	- Flag "never_frosh"
 *	  Never call the "TERM_XTRA_FROSH" action
 *
 *
 *	- Value "attr_blank"
 *	  Use this "attr" value for "blank" grids
 *
 *	- Value "char_blank"
 *	  Use this "char" value for "blank" grids
 *
 *
 *	- Ignore this pointer
 *
 *	- Keypress Queue -- various data
 *
 *	- Keypress Queue -- pending keys
 *
 *
 *	- Window Width (max 255)
 *	- Window Height (max 255)
 *
 *	- Minimum modified row
 *	- Maximum modified row
 *
 *	- Minimum modified column (per row)
 *	- Maximum modified column (per row)
 *
 *
 *	- Displayed screen image
 *	- Requested screen image
 *
 *	- Temporary screen image
 *	- Memorized screen image
 *
 *
 *	- Hook for init-ing the term
 *	- Hook for nuke-ing the term
 *
 *	- Hook for user actions
 *
 *	- Hook for extra actions
 *
 *	- Hook for placing the cursor
 *
 *	- Hook for drawing some blank spaces
 *
 *	- Hook for drawing a string of chars using an attr
 *
 *	- Hook for drawing a sequence of special attr/char pairs
 */

typedef struct term term;

struct term
{
	vptr user;

	vptr data;

	bool user_flag;

	bool data_flag;

	bool active_flag;
	bool mapped_flag;
	bool total_erase;
	bool fixed_shape;
	bool icky_corner;
	bool soft_cursor;
	bool always_pict;
	bool higher_pict;
	bool always_text;
	bool unused_flag;
	bool never_bored;
	bool never_frosh;

	byte attr_blank;
	char char_blank;

	char *key_queue;

	u16b key_head;
	u16b key_tail;
	u16b key_xtra;
	u16b key_size;

	byte wid;
	byte hgt;

	byte y1;
	byte y2;

	byte *x1;
	byte *x2;

	term_win *old;
	term_win *scr;

	term_win *tmp;
	term_win *mem;

	void (*init_hook)(term *t);
	void (*nuke_hook)(term *t);

	errr (*user_hook)(s32b n);

	errr (*xtra_hook)(s32b n, s32b v);

	errr (*curs_hook)(s32b x, s32b y);

	errr (*wipe_hook)(s32b x, s32b y, s32b n);

	errr (*text_hook)(s32b x, s32b y, s32b n, byte a, cptr s);

	void (*resize_hook)(void);

#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
	errr (*pict_hook)(s32b x, s32b y, s32b n, const byte *ap, const char *cp, const byte *tap, const char *tcp, const byte *eap, const char *ecp);
#else /* USE_EGO_GRAPHICS */
	errr (*pict_hook)(s32b x, s32b y, s32b n, const byte *ap, const char *cp, const byte *tap, const char *tcp);
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
	errr (*pict_hook)(s32b x, s32b y, s32b n, const byte *ap, const char *cp);
#endif /* USE_TRANSPARENCY */

};







/**** Available Constants ****/


/*
 * Definitions for the "actions" of "Term_xtra()"
 *
 * These values may be used as the first parameter of "Term_xtra()",
 * with the second parameter depending on the "action" itself.  Many
 * of the actions shown below are optional on at least one platform.
 *
 * The "TERM_XTRA_EVENT" action uses "v" to "wait" for an event
 * The "TERM_XTRA_SHAPE" action uses "v" to "show" the cursor
 * The "TERM_XTRA_FROSH" action uses "v" for the index of the row
 * The "TERM_XTRA_SOUND" action uses "v" for the index of a sound
 * The "TERM_XTRA_ALIVE" action uses "v" to "activate" (or "close")
 * The "TERM_XTRA_LEVEL" action uses "v" to "resume" (or "suspend")
 * The "TERM_XTRA_DELAY" action uses "v" as a "millisecond" value
 * The "TERM_XTRA_WINVIS" action uses "v" to turn off (0) or on (1) an SDL term
 * The "TERM_XTRA_EFFECTS_DELAY" action uses "v" as a "millisecond" value
 *
 * The other actions do not need a "v" code, so "zero" is used.
 */
#define TERM_XTRA_EVENT	1	/* Process some pending events */
#define TERM_XTRA_FLUSH 2	/* Flush all pending events */
#define TERM_XTRA_CLEAR 3	/* Clear the entire window */
#define TERM_XTRA_SHAPE 4	/* Set cursor shape (optional) */
#define TERM_XTRA_FROSH 5	/* Flush one row (optional) */
#define TERM_XTRA_FRESH 6	/* Flush all rows (optional) */
#define TERM_XTRA_NOISE 7	/* Make a noise (optional) */
#define TERM_XTRA_SOUND 8	/* Make a sound (optional) */
#define TERM_XTRA_BORED 9	/* Handle stuff when bored (optional) */
#define TERM_XTRA_REACT 10	/* React to global changes (optional) */
#define TERM_XTRA_ALIVE 11	/* Change the "hard" level (optional) */
#define TERM_XTRA_LEVEL 12	/* Change the "soft" level (optional) */
#define TERM_XTRA_DELAY 13	/* Delay some milliseconds (optional) */
#define TERM_XTRA_GET_DELAY 14	/* Get the current time in milliseconds (optional) */
/* XXX */
#define TERM_XTRA_RENAME_MAIN_WIN 16 /* Rename the main game window */
#define TERM_XTRA_LOAD_GFX 17 	/* Load the graphics of the game, this is used to notify the main-foo that module is now setup and virtual paths working so laoding gfx will get the right ones  */

#ifdef USE_SDL
#define TERM_XTRA_WINVIS 30 /* Window state change on/off */
#define TERM_XTRA_EFFECTS_DELAY 31	/* additional delay action to avoid problems with TERM_XTRA_DELAY */
#else
/* equate TERM_XTRA_EFFECTS_DELAY to TERM_XTRA_DELAY so as to not break the other interfaces */
#define TERM_XTRA_EFFECTS_DELAY TERM_XTRA_DELAY
#endif

/**** Available Variables ****/

extern term *Term;
extern s32b do_movies;
extern s32b last_paused;


/**** Available Functions ****/

extern errr Term_user(s32b n);
extern errr Term_xtra(s32b n, s32b v);
extern long Term_xtra_long;

#ifdef USE_TRANSPARENCY
#ifdef USE_EGO_GRAPHICS
extern void Term_queue_char(s32b x, s32b y, byte a, char c, byte ta, char tc, byte ea, char ec);

extern void Term_queue_line(s32b x, s32b y, s32b n, byte *a, char *c, byte *ta, char *tc, byte *ea, char *ec);
#else /* USE_EGO_GRAPHICS */
extern void Term_queue_char(s32b x, s32b y, byte a, char c, byte ta, char tc);

extern void Term_queue_line(s32b x, s32b y, s32b n, byte *a, char *c, byte *ta, char *tc);
#endif /* USE_EGO_GRAPHICS */
#else /* USE_TRANSPARENCY */
extern void Term_queue_char(s32b x, s32b y, byte a, char c);

extern void Term_queue_line(s32b x, s32b y, s32b n, byte *a, char *c);
#endif /* USE_TRANSPARENCY */

extern void Term_queue_chars(s32b x, s32b y, s32b n, byte a, cptr s);

extern errr Term_fresh(void);
extern errr Term_set_cursor(s32b v);
extern errr Term_gotoxy(s32b x, s32b y);
extern errr Term_draw(s32b x, s32b y, byte a, char c);
extern errr Term_addch(byte a, char c);
extern errr Term_addstr(s32b n, byte a, cptr s);
extern errr Term_putch(s32b x, s32b y, byte a, char c);
extern errr Term_putstr(s32b x, s32b y, s32b n, byte a, cptr s);
extern errr Term_erase(s32b x, s32b y, s32b n);
extern errr Term_clear(void);
extern errr Term_redraw(void);
extern errr Term_redraw_section(s32b x1, s32b y1, s32b x2, s32b y2);

extern errr Term_get_cursor(s32b *v);
extern errr Term_get_size(s32b *w, s32b *h);
extern errr Term_locate(s32b *x, s32b *y);
extern errr Term_what(s32b x, s32b y, byte *a, char *c);

extern errr Term_flush(void);
extern errr Term_keypress(s32b k);
extern errr Term_key_push(s32b k);
extern errr Term_inkey(char *ch, bool wait, bool take);

extern errr Term_save(void);
extern term_win* Term_save_to(void);
extern errr Term_load(void);
extern errr Term_load_from(term_win *save, bool final);

extern errr Term_exchange(void);

extern errr Term_resize(s32b w, s32b h);

extern errr Term_activate(term *t);

extern errr term_nuke(term *t);
extern errr term_init(term *t, s32b w, s32b h, s32b k);

#endif


