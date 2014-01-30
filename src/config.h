/* File: config.h */

/* Purpose: Angband specific configuration stuff */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */


/*
 * Look through the following lines, and where a comment includes the
 * tag "OPTION:", examine the associated "#define" statements, and decide
 * whether you wish to keep, comment, or uncomment them.  You should not
 * have to modify any lines not indicated by "OPTION".
 *
 * Note: Also examine the "system" configuration file "h-config.h"
 * and the variable initialization file "variable.c".  If you change
 * anything in "variable.c", you only need to recompile that file.
 *
 * And finally, remember that the "Makefile" will specify some rather
 * important compile time options, like what visual module to use.
 */


/*
 * OPTION: See the Makefile(s), where several options may be declared.
 *
 * Some popular options include "USE_GCU" (allow use with Unix "curses"),
 * "USE_X11" (allow basic use with Unix X11), "USE_XAW" (allow use with
 * Unix X11 plus the Athena Widget set), and "USE_CAP" (allow use with
 * the "termcap" library, or with hard-coded vt100 terminals).
 *
 * The old "USE_NCU" option has been replaced with "USE_GCU".
 *
 * Several other such options are available for non-unix machines,
 * such as "USE_WINDOWS"
 */


/*
 * OPTION: define "SPECIAL_BSD" for using certain versions of UNIX
 * that use the 4.4BSD Lite version of Curses in "main-gcu.c"
 */
/* #define SPECIAL_BSD */


/*
 * OPTION: Use the POSIX "termios" methods in "main-gcu.c"
 */
/* #define USE_TPOSIX */

/*
 * OPTION: Use the "termio" methods in "main-gcu.c"
 */
/* #define USE_TERMIO */

/*
 * OPTION: Use the icky BSD "tchars" methods in "main-gcu.c"
 */
/* #define USE_TCHARS */


/*
 * OPTION: Include "ncurses.h" instead of "curses.h" in "main-gcu.c"
 */
/* #define USE_NCURSES */



/*
 * OPTION: Verify savefile Checksums (Angband 2.7.0 and up)
 * This option can help prevent "corruption" of savefiles, and also
 * stop intentional modification by amateur users.
 */
#define VERIFY_CHECKSUMS


/*
 * OPTION: Forbid the use of "fiddled" savefiles.  As far as I can tell,
 * a fiddled savefile is one with an internal timestamp different from
 * the actual timestamp.  Thus, turning this option on forbids one from
 * copying a savefile to a different name.  Combined with disabling the
 * ability to save the game without quitting, and with some method of
 * stopping the user from killing the process at the tombstone screen,
 * this should prevent the use of backup savefiles.  It may also stop
 * the use of savefiles from other platforms, so be careful.
 */
/* #define VERIFY_TIMESTAMP */


/*
 * OPTION: Forbid the "savefile over-write" cheat, in which you simply
 * run another copy of the game, loading a previously saved savefile,
 * and let that copy over-write the "dead" savefile later.  This option
 * either locks the savefile, or creates a fake "xxx.lok" file to prevent
 * the use of the savefile until the file is deleted.  Not ready yet.
 */
/* #define VERIFY_SAVEFILE */


/*
 * OPTION: Check version stamp when loading files in lib/data directory,
 * or their source in lib/edit.
 */
/* # #define VERIFY_VERSION_STAMP */



/*
 * OPTION: Hack -- Compile in support for "Cyborg" mode
 */
/*#define ALLOW_BORG*/

/*
 * OPTION: Hack -- Compile in support for "Wizard Commands"
 */
#define ALLOW_WIZARD

/*
 * OPTION: Hack -- Compile in support for "Spoiler Generation"
 */
/* #define ALLOW_SPOILERS */

/*
 * OPTION: Allow "do_cmd_colors" at run-time
 */
#define ALLOW_COLORS

/*
 * OPTION: Allow "do_cmd_visuals" at run-time
 */
#define ALLOW_VISUALS

/*
 * OPTION: Allow "do_cmd_macros" at run-time
 */
#define ALLOW_MACROS

/*
 * OPTION: Allow characteres to be "auto-rolled"
 */
#define ALLOW_AUTOROLLER


/*
 * OPTION: Allow monsters to "flee" when hit hard
 */
#define ALLOW_FEAR

/*
 * OPTION: Allow monsters to "flee" from strong players
 */
#define ALLOW_TERROR


/*
 * OPTION: Allow parsing of the ascii template files in "init.c".
 * This must be defined if you do not have valid binary image files.
 * It should be usually be defined anyway to allow easy "updating".
 */
#define ALLOW_TEMPLATES

/*
 * OPTION: Allow loading of pre-2.7.0 savefiles.  Note that it takes
 * about 15K of code in "save-old.c" to parse the old savefile format.
 * Angband 2.8.0 will ignore a lot of info from pre-2.7.0 savefiles.
 */
#define ALLOW_OLD_SAVEFILES


/*
 * OPTION: Delay the loading of the "f_text" array until it is actually
 * needed, saving ~1K, since "feature" descriptions are unused.
 */
/* #define DELAY_LOAD_F_TEXT */

/*
 * OPTION: Delay the loading of the "k_text" array until it is actually
 * needed, saving ~1K, since "object" descriptions are unused.
 */
/* #define DELAY_LOAD_K_TEXT */

/*
 * OPTION: Delay the loading of the "a_text" array until it is actually
 * needed, saving ~1K, since "artifact" descriptions are unused.
 */
/* #define DELAY_LOAD_A_TEXT */

/*
 * OPTION: Delay the loading of the "e_text" array until it is actually
 * needed, saving ~1K, since "ego-item" descriptions are unused.
 */
#define DELAY_LOAD_E_TEXT

/*
 * OPTION: Delay the loading of the "r_text" array until it is actually
 * needed, saving ~60K, but "simplifying" the "monster" descriptions.
 */
/* #define DELAY_LOAD_R_TEXT */

/*
 * OPTION: Delay the loading of the "v_text" array until it is actually
 * needed, saving ~1K, but "destroying" the "vault" generation.
 */
/* #define DELAY_LOAD_V_TEXT */


/*
 * OPTION: Handle signals
 */
#define HANDLE_SIGNALS


/*
 * Allow "Wizards" to yield "high scores"
 */
/* #define SCORE_WIZARDS */

/*
 * Allow "Borgs" to yield "high scores"
 */
/*#define SCORE_BORGS*/

/*
 * Allow "Cheaters" to yield "high scores"
 */
/* #define SCORE_CHEATERS */


/*
 * OPTION: Allow use of extended spell info	-DRS-
 */
#define DRS_SHOW_SPELL_INFO

/*
 * OPTION: Allow use of the monster health bar	-DRS-
 */
#define DRS_SHOW_HEALTH_BAR



/*
 * OPTION: Enable the "track_follow" and "track_target" options.
 * They let monsters follow the player's foot-prints, or remember
 * the player's recent locations.  This code has been removed from
 * the current version because it is being rewritten by Billy, and
 * until it is ready, it will not work.  Do not define this option.
 */
/* #define WDT_TRACK_OPTIONS */



/*
 * OPTION: Allow the use of "sound" in various places.
 */
#define USE_SOUND

/*
 * OPTION: Allow the use of "graphics" in various places
 */
#define USE_GRAPHICS


/*
 * OPTION: Hack -- Windows stuff
 */
#ifdef USE_WINDOWS

/* Do not handle signals */
# undef HANDLE_SIGNALS

#endif


/*
 * Where to store user data relative to the home directory (as defined
 * by physfs)
 */
#ifndef PRIVATE_USER_PATH
#define PRIVATE_USER_PATH ".tome"
#endif /* PRIVATE_USER_PATH */

/*
 * Name of the settings files
 */
#define SETTINGS_LUA  "tome.cfg"
#define SETTINGS_TOME "settings.cfg"


/*
 * On multiuser systems, add the "uid" to savefile names
 */
#if defined(SET_UID)
# define SAVEFILE_USE_UID
#endif /* SET_UID */


/*
 * OPTION: Check the "time" against "lib/file/hours.txt"
 */
/* #define CHECK_TIME */

/*
 * OPTION: Check the "load" against "lib/file/load.txt"
 * This may require the 'rpcsvs' library
 */
/* #define CHECK_LOAD */


/*
 * OPTION: For some brain-dead computers with no command line interface,
 * namely Macintosh, there has to be some way of "naming" your savefiles.
 * The current "Macintosh" hack is to make it so whenever the character
 * name changes, the savefile is renamed accordingly.  But on normal
 * machines, once you manage to "load" a savefile, it stays that way.
 * Macintosh is particularly weird because you can load savefiles that
 * are not contained in the "lib:save:" folder, and if you change the
 * player's name, it will then save the savefile elsewhere.  Note that
 * this also gives a method of "bypassing" the "VERIFY_TIMESTAMP" code.
 */
#if defined(USE_WINDOWS) || defined(AMIGA)
/* #define SAVEFILE_MUTABLE */
#endif


/*
 * OPTION: Capitalize the "user_name" (for "default" player name)
 * This option is only relevant on SET_UID machines.
 */
#define CAPITALIZE_USER_NAME



/*
 * OPTION: Person to bother if something goes wrong.
 */
#define MAINTAINER      "darkgod@t-o-m-e.net"


/*
 * OPTION: Default font (when using X11).
 */
#define DEFAULT_X11_FONT		"9x15"

/*
 * OPTION: Default fonts (when using X11)
 */
#define DEFAULT_X11_FONT_SCREEN		DEFAULT_X11_FONT
#define DEFAULT_X11_FONT_MIRROR		DEFAULT_X11_FONT
#define DEFAULT_X11_FONT_RECALL		DEFAULT_X11_FONT
#define DEFAULT_X11_FONT_CHOICE		DEFAULT_X11_FONT



/*
 * Hack -- Special "ancient machine" versions
 */
#if defined(USE_286) || defined(ANGBAND_LITE_MAC)
# ifndef ANGBAND_LITE
#  define ANGBAND_LITE
# endif
#endif

/*
 * OPTION: Attempt to minimize the size of the game
 */
#ifndef ANGBAND_LITE
/* #define ANGBAND_LITE */
#endif

/*
 * Hack -- React to the "ANGBAND_LITE" flag
 */
#ifdef ANGBAND_LITE
# undef ALLOW_COLORS
# undef ALLOW_VISUALS
# undef ALLOW_MACROS
# undef WDT_TRACK_OPTIONS
# undef ALLOW_OLD_SAVEFILES
# undef ALLOW_BORG
# undef ALLOW_WIZARD
# undef ALLOW_SPOILERS
# undef ALLOW_TEMPLATES
# undef DELAY_LOAD_R_TEXT
# define DELAY_LOAD_R_TEXT
#endif



/*
 * OPTION: Attempt to prevent all "cheating"
 */
/* #define VERIFY_HONOR */


/*
 * React to the "VERIFY_HONOR" flag
 */
#ifdef VERIFY_HONOR
# define VERIFY_SAVEFILE
# define VERIFY_CHECKSUMS
# define VERIFY_TIMESTAMPS
#endif

/* ToME options: */

/* Should the player know his / her starting life rate? */
/* #define SHOW_LIFE_RATE */

/* Do we want different characters for different races? */
#define VARIABLE_PLAYER_GRAPH

/* Wizard mode testing options: */

/* For testing the vaults */
/* # define FORCE_V_IDX 20 */

/* Testing upkeep */
/* # define TRACK_FRIENDS */

/*
 * OPTION: Repeat last command -- TNB
 */
#define ALLOW_REPEAT

/*
 * OPTION: Make opening and closing things easy -- TNB
 */
#define ALLOW_EASY_OPEN

/*
 * OPTION: Make disarming traps easy -- TNB
 */
#define ALLOW_EASY_DISARM

/*
 * Check the modification time of *_info.raw files
 * (by Keldon Jones)
 */
#define CHECK_MODIFICATION_TIME

/*
 * Using the fast autoroller can be considered as cheating
 */
#define USE_FAST_AUTOROLLER

/*
 * Forbid the use of some race/class combinations
 */
/* #define RESTRICT_COMBINATIONS */

/*
 * Enable the CTRL + L command to quit without saving
 * Only use for debugging purpose, otherwise you are a CHEATER
 */
/* #define ALLOW_QUITTING */

/*
 * Panic saves have a different name
 */
#define SAFER_PANICS

/*
 * Compile lua sources to bytecode when source is newer
 * and then only loads bytecode if source is not newer
 */
/* #define COMPILE_LUA */

/*
 * Allow makefiles to override the default file mode
 */
#ifndef FILE_MODE
#define FILE_MODE 0644
#endif
