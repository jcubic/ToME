/* File: main.c */

/*
 * Copyright (c) 1997 Ben Harrison, and others
 *
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.
 */

#include "angband.h"

#include <SDL.h>
#include <SDL_net.h>
#include <SDL_mixer.h>

#ifdef USE_SDL
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#endif

/* We need to tell the compiler what it should do */
extern bool tome_compile_lua_stripping;
extern bool tome_compile_lua_optimizing;
extern bool tome_compile_lua_ok;


/*
 * Some machines have a "main()" function in their "main-xxx.c" file,
 * all the others use this file for their "main()" function.
 */


#if !defined(USE_WINDOWS)


/*
 * A hook for "quit()".
 *
 * Close down, then fall back into "quit()".
 */
static void quit_hook(cptr s)
{
	s32b j;

	/* Scan windows */
	for (j = 8 - 1; j >= 0; j--)
	{
		/* Unused */
		if (!angband_term[j]) continue;

		/* Nuke it */
		term_nuke(angband_term[j]);
	}

	/* Cleanup stuff done in init2.c (mainly C_MAKEs) */
	cleanup_angband();
}



/*
 * Set the stack size (for the Amiga)
 */
#ifdef AMIGA
# include <dos.h>
__near long __stack = 32768L;
#endif


/*
 * Set the stack size and overlay buffer (see main-286.c")
 */
#ifdef USE_286
# include <dos.h>
extern unsigned _stklen = 32768U;
extern unsigned _ovrbuffer = 0x1500;
#endif

/*
 * Initialize and verify the file paths, and the score file.
 */
static void init_stuff(char *argv0)
{
	/* Initialize */
	init_file_paths(argv0);
}

/*
 * Handle a "-d<what>=<path>" option
 *
 * The "<what>" can be any string starting with the same letter as the
 * name of a subdirectory of the "lib" folder (i.e. "i" or "info").
 *
 * The "<path>" can be any legal path for the given system, and should
 * not end in any special path separator (i.e. "/tmp" or "~/.ang-info").
 */
static void change_path(cptr info)
{
	cptr s;

	/* Find equal sign */
	s = strchr(info, '=');

	/* Verify equal sign */
	if (!s) quit_fmt("Try '-d<what>=<path>' not '-d%s'", info);

	/* Analyze */
	switch (tolower(info[0]))
	{
	case 'a':
		{
			string_free(TENGINE_DIR_SCORES);
			TENGINE_DIR_SCORES = string_make(s + 1);
			break;
		}

	case 'f':
		{
			string_free(TENGINE_DIR_FILE);
			TENGINE_DIR_FILE = string_make(s + 1);
			break;
		}

	case 'h':
		{
			string_free(TENGINE_DIR_HELP);
			TENGINE_DIR_HELP = string_make(s + 1);
			break;
		}

	case 'i':
		{
			string_free(TENGINE_DIR_INFO);
			TENGINE_DIR_INFO = string_make(s + 1);
			break;
		}

	case 'u':
		{
			string_free(TENGINE_DIR_USER);
			TENGINE_DIR_USER = string_make(s + 1);
			break;
		}

	case 'x':
		{
			string_free(TENGINE_DIR_XTRA);
			TENGINE_DIR_XTRA = string_make(s + 1);
			break;
		}

#ifdef VERIFY_SAVEFILE

	case 'b':
	case 'd':
	case 'e':
	case 's':
		{
			quit_fmt("Restricted option '-d%s'", info);
		}

#else /* VERIFY_SAVEFILE */

	case 'b':
		{
			string_free(TENGINE_DIR_BONE);
			TENGINE_DIR_BONE = string_make(s + 1);
			break;
		}

	case 'd':
		{
			string_free(TENGINE_DIR_RAW);
			TENGINE_DIR_RAW = string_make(s + 1);
			break;
		}

	case 'e':
		{
			string_free(TENGINE_DIR_DATA);
			TENGINE_DIR_DATA = string_make(s + 1);
			break;
		}

	case 's':
		{
			string_free(TENGINE_DIR_SAVE);
			TENGINE_DIR_SAVE = string_make(s + 1);
			break;
		}

#endif /* VERIFY_SAVEFILE */

	default:
		{
			quit_fmt("Bad semantics in '-d%s'", info);
		}
	}
}

/* Let certain platforms piggyback on this main without it being main() */
#ifndef USE_TOME_MAIN
#define ToME_main main
#endif

/*
 * Simple "main" function for multiple platforms.
 *
 * Note the special "--" option which terminates the processing of
 * standard options.  All non-standard options (if any) are passed
 * directly to the "init_xxx()" function.
 */
int ToME_main(int argc, char *argv[])
{
	s32b i;

	bool done = FALSE;

	bool new_game = FALSE;

	s32b show_score = 0;

	cptr mstr = NULL;

	bool args = TRUE;

#ifdef CHECK_MEMORY_LEAKS
	GC_find_leak = 1;
#endif /* CHECK_MEMORY_LEAKS */


	/* Save the "program name" XXX XXX XXX */
	argv0 = argv[0];


#ifdef USE_286
	/* Attempt to use XMS (or EMS) memory for swap space */
	if (_OvrInitExt(0L, 0L))
	{
		_OvrInitEms(0, 0, 64);
	}
#endif


#ifdef SET_UID

	/* Default permissions on files */
	(void)umask(022);

	/* Please note that the game is still running in the game's permission */

	/* Initialize the "time" checker */
	if (check_time_init() || check_time())
	{
		quit("The gates to Angband are closed (bad time).");
	}

	/* Initialize the "load" checker */
	if (check_load_init() || check_load())
	{
		quit("The gates to Angband are closed (bad load).");
	}

	/* Acquire the "user name" as a default player name */
	user_name(player_name, player_uid);

#endif /* SET_UID */

	/* Get the file paths */
	init_stuff(argv[0]);

	/* Process the command line arguments */
	for (i = 1; args && (i < argc); i++)
	{
		/* Require proper options */
		if (argv[i][0] != '-') goto usage;

		/* Analyze option */
#ifdef COMPILE_LUA
		if (!strcmp(argv[i], "--dont-optimize"))
			tome_compile_lua_optimizing = FALSE;
		else if (!strcmp(argv[i], "--dont-strip"))
			tome_compile_lua_stripping = FALSE;
		else if (!strcmp(argv[i], "--no-lua-bytecode"))
			tome_compile_lua_ok = FALSE;
		else
#endif
		if (!strcmp(argv[i], "--lua-exec"))
		{
			/* If any extra lua commands have been defined on command line, use them */
			exec_lua(argv[++i]);
		}
		else
		{
			switch (argv[i][1])
			{
			case 'N':
			case 'n':
				{
					new_game = TRUE;
					break;
				}

			case 'F':
			case 'f':
				{
					arg_fiddle = TRUE;
					break;
				}

			case 'W':
			case 'w':
				{
					arg_wizard = TRUE;
					break;
				}

			case 'V':
			case 'v':
				{
					arg_sound = TRUE;
					break;
				}

			case 'G':
			case 'g':
				{
					arg_graphics = TRUE;
					break;
				}

			case 'R':
			case 'r':
				{
					arg_force_roguelike = TRUE;
					break;
				}

			case 'O':
			case 'o':
				{
					arg_force_original = TRUE;
					break;
				}

			case 'S':
			case 's':
				{
					show_score = atoi(&argv[i][2]);
					if (show_score <= 0) show_score = 10;
					break;
				}

			case 'u':
			case 'U':
				{
					if (!argv[i][2]) goto usage;
					strcpy(player_name, &argv[i][2]);
					strcpy(player_base, &argv[i][2]);
					no_begin_screen = TRUE;
					break;
				}

			case 'm':
				{
					if (!argv[i][2]) goto usage;
					mstr = &argv[i][2];
					break;
				}

			case 'M':
				{
					if (!argv[i][2]) goto usage;
					force_module = string_make(&argv[i][2]);
					break;
				}

			case 'h':
				{
					goto usage;
					break;
				}

			case 'H':
				{
					char *s;
					s32b j;

					init_lua();
					for (j = i + 1; j < argc; j++)
					{
						s = argv[j];

						while (*s != '.') s++;
						*s = '\0';
						s++;
						txt_to_html("head.aux", "foot.aux", argv[j], s, FALSE, FALSE);
					}

					return 0;
				}

			case 'c':
			case 'C':
				{
					chg_to_txt(argv[i + 1], argv[i + 2]);

					return 0;
				}

			case 'd':
			case 'D':
				{
					change_path(&argv[i][2]);
					break;
				}

			case 'l':
				{
					printf("\nSDL libraries used:\n");
					printf("=========================\n");
#ifdef SDL_VERSION
					/* we can't have the other SDL libraries without the main SDL library */
					SDL_version sdl_ver;

					SDL_VERSION(&sdl_ver);
					printf("SDL\t\t%d.%d.%d\n", sdl_ver.major, sdl_ver.minor, sdl_ver.patch);

#ifdef SDL_NET_VERSION
					SDL_NET_VERSION(&sdl_ver);
					printf("SDL_net\t\t%d.%d.%d\n", sdl_ver.major, sdl_ver.minor, sdl_ver.patch);
#endif

#ifdef SDL_MIXER_VERSION
					SDL_MIXER_VERSION(&sdl_ver);
					printf("SDL_mixer\t%d.%d.%d\n", sdl_ver.major, sdl_ver.minor, sdl_ver.patch);
#endif

#ifdef SDL_TTF_VERSION
					SDL_TTF_VERSION(&sdl_ver);
					printf("SDL_ttf\t\t%d.%d.%d\n", sdl_ver.major, sdl_ver.minor, sdl_ver.patch);
#endif

#ifdef SDL_GFXPRIMITIVES_MAJOR
					printf("SDL_gfx\t\t%d.%d.%d\n", SDL_GFXPRIMITIVES_MAJOR, SDL_GFXPRIMITIVES_MINOR, SDL_GFXPRIMITIVES_MICRO);
#endif
#else
					printf("No SDL library found\n");
#endif /* SDL_VERSION */
					printf("\n");
					return 0;
				}

			case '-':
				{
					if (argv[i][2] == 'h' && !strcmp((argv[i] + 2), "help"))
						goto usage;
					else
					{
						argv[i] = argv[0];
						argc = argc - i;
						argv = argv + i;
						args = FALSE;
						break;
					}
				}

			default:
			usage:
				{
					s32b j;

					/* Dump usage information */
					for (j = 0; j < argc; j++) printf("%s ", argv[j]);
					printf("\n");
					puts("Usage: tome [options] [-- subopts]");
					puts("  -h                 This help");
					puts("  -n                 Start a new character");
					puts("  -f                 Request fiddle mode");
					puts("  -w                 Request wizard mode");
					puts("  -v                 Request sound mode");
					puts("  -g                 Request graphics mode");
					puts("  -o                 Request original keyset");
					puts("  -r                 Request rogue-like keyset");					                 
					puts("  -H <list of files> Convert helpfile to html");
					puts("  -c f1 f2           Convert changelog f1 to nice txt f2");
					puts("  -s<num>            Show <num> high scores");
					puts("  -u<who>            Use your <who> savefile");
					puts("  -M<which>            Use the <which> module");
					puts("  -m<sys>            Force 'main-<sys>.c' usage");
					puts("  -d<def>            Define a 'lib' dir sub-path");
					puts("");
					puts("  --lua-exec         Execute this lua code after tome.cfg loading");
					puts("");
#ifdef COMPILE_LUA
					puts("");
					puts("  --no-lua-bytecode  Do not compile lua to bytecode");
					puts("  --dont-strip       Do not strip lua bytecode of debug info");
					puts("  --dont-optimize    Do not optimize lua bytecode");
					puts("");
#endif

#ifdef LUA_TERMINAL_CONSOLE
					puts("");
					puts("  --lua-console <f>  Runs the game lua interpreter on the given file");
					puts("");
#endif

#ifdef USE_GTK
					puts("  -mgtk              To use GTK");
					puts("  --                 Sub options");
					puts("  -- -n #            Number of terms to use");
					puts("  -- -b              Turn off software backing store");
# ifdef USE_GRAPHICS
					puts("  -- -s              Turn off smoothscaling graphics");
					puts("  -- -g 8            Requests \"8x8\" graphics");
					puts("  -- -g 16           Requests \"16x16\" graphics");
					puts("  -- -g 32           Requests \"32x32\" graphics");
					puts("  -- -t              Enable transparency effect");
# endif  /* USE_GRAPHICS */
#endif /* USE_GTK */

#ifdef USE_GTK2
					puts("  -mgtk2             To use GTK2");
					puts("  --                 Sub options");
					puts("  -- -n #            Number of terms to use");
					puts("  -- -b              Turn off software backing store");
# ifdef USE_GRAPHICS
					puts("  -- -s              Turn off smoothscaling graphics");
					puts("  -- -g 8            Requests \"8x8\" graphics");
					puts("  -- -g 16           Requests \"16x16\" graphics");
					puts("  -- -g 32           Requests \"32x32\" graphics");
					puts("  -- -t              Enable transparency effect");
# endif  /* USE_GRAPHICS */
#endif /* USE_GTK2 */

#ifdef USE_X11
					puts("  -mx11              To use X11");
					puts("  --                 Sub options");
					puts("  -- -n #            Number of terms to use");
					puts("  -- -d<name>        Display to use");
# ifdef USE_GRAPHICS
					puts("  -- -s              Turn off smoothscaling graphics");
					puts("  -- -g 8            Requests \"8x8\" graphic tiles");
					puts("  -- -g 16           Requests \"16x16\" graphic tiles");
					puts("  -- -g 32           Requests \"32x32\" graphic tiles");
					puts("  -- -b              Requests double-width tiles");
# endif  /* USE_GRAPHICS */
#endif /* USE_X11 */

#ifdef USE_GCU
					puts("  -mgcu              To use curses");
					puts("  --                 Sub options");
					puts("  -- -b              Requests big screen");
#endif /* USE_GCU */

#ifdef USE_CAP
					puts("  -mcap              To use termcap");
#endif /* USE_CAP */

#ifdef USE_SLA
					puts("  -msla              To use SLang");
#endif /* USE_SLA */

#ifdef USE_SDL
					puts("  -msdl              To use SDL");
					puts("  --                 Sub options");

# ifdef USE_GRAPHICS
					puts("  -- -g 0            Turn off graphic tiles (text mode)");
					puts("  -- -g 8            Requests \"8x8\" graphic tiles");
					puts("  -- -g 16           Requests \"16x16\" graphic tiles");
					puts("  -- -g 32           Requests \"32x32\" graphic tiles");
# endif /* USE_GRAPHICS */

#if 0
					puts("  -- -b              Requests double-width tiles");
					puts("  -- -bpp #          Request screen color depth in bits");
					puts("  -- -f #            Request font name");
					puts("  -- -s #            Request font size");
#endif
					puts("  -- -a              Use font alpha blending (for hardware accelerated video cards)");
					puts("  -- -f              Full-screen (Very slow under Windows)");
					puts("  -- -i #            Set key repeat interval");
					puts("  -- -u              Set keypad to default to auto-run");
					puts("  -- -k              Show key codes");
					puts("  -- -l              Log info, warning and error messages (default: errors only)");
					puts("  -- -r #            Set refresh rate (in ms, >= 16ms) [For debug only]");
					puts("  -- -x #            Request screen width in pixels");
					puts("  -- -y #            Request screen height in pixels");
#endif /* USE_SDL */

					/* Actually abort the process */
					quit(NULL);
				}
			}
		}
	}

	/* Hack -- Forget standard args */
	if (args)
	{
		argc = 1;
		argv[1] = NULL;
	}


	/* Process the player name */
	process_player_name(TRUE);


	/* Install "quit" hook */
	quit_aux = quit_hook;

	/* Load up settings */
	esettings_load();

	ignore_numlock = 1;

#ifdef USE_SDL
       /* Attempt to use the "main-sdl2.c" support */
       if (!done && (!mstr || (streq(mstr, "sdl"))))
       {
               extern errr sdl_start(s32b, char**);
               if (0 == sdl_start(argc, argv))
               {
                       ANGBAND_SYS = "sdl";
                        done = TRUE;
                }
        }
#endif

#ifdef USE_GLU
	/* Attempt to use the "main-glu.c" support */
	if (!done && (!mstr || (streq(mstr, "glu"))))
	{
		extern errr init_glu(s32b, char**);
		if (0 == init_glu(argc, argv))
		{
			ANGBAND_SYS = "glu";
			done = TRUE;
		}
	}
#endif

#ifdef USE_GTK2
	/* Attempt to use the "main-gtk2.c" support */
	if (!done && (!mstr || (streq(mstr, "gtk2"))))
	{
		extern errr init_gtk2(s32b, char**);
		if (0 == init_gtk2(argc, argv))
		{
			ANGBAND_SYS = "gtk2";
			done = TRUE;
		}
	}
#endif

#ifdef USE_GTK
	/* Attempt to use the "main-gtk.c" support */
	if (!done && (!mstr || (streq(mstr, "gtk"))))
	{
		extern errr init_gtk(s32b, char**);
		if (0 == init_gtk(argc, argv))
		{
			ANGBAND_SYS = "gtk";
			done = TRUE;
		}
	}
#endif

#ifdef USE_X11
	/* Attempt to use the "main-x11.c" support */
	if (!done && (!mstr || (streq(mstr, "x11"))))
	{
		extern errr init_x11(s32b, char**);
		if (0 == init_x11(argc, argv))
		{
			ANGBAND_SYS = "x11";
			done = TRUE;
		}
	}
#endif

#ifdef USE_GCU
	/* Attempt to use the "main-gcu.c" support */
	if (!done && (!mstr || (streq(mstr, "gcu"))))
	{
		extern errr init_gcu(s32b, char**);
		if (0 == init_gcu(argc, argv))
		{
			ANGBAND_SYS = "gcu";
			done = TRUE;
		}
	}
#endif

#ifdef USE_GLU
	/* Attempt to use the "main-glu.c" support */
	if (!done && (!mstr || (streq(mstr, "glu"))))
	{
		extern errr init_glu(s32b, char**);
		if (0 == init_glu(argc, argv))
		{
			ANGBAND_SYS = "glu";
			done = TRUE;
		}
	}
#endif

#ifdef USE_CAP
	/* Attempt to use the "main-cap.c" support */
	if (!done && (!mstr || (streq(mstr, "cap"))))
	{
		extern errr init_cap(s32b, char**);
		if (0 == init_cap(argc, argv))
		{
			ANGBAND_SYS = "cap";
			done = TRUE;
		}
	}
#endif


#ifdef USE_SLA
	/* Attempt to use the "main-sla.c" support */
	if (!done && (!mstr || (streq(mstr, "sla"))))
	{
		extern errr init_sla(void);
		if (0 == init_sla())
		{
			ANGBAND_SYS = "sla";
			done = TRUE;
		}
	}
#endif


#ifdef USE_PARAGUI
	/* Attempt to use the "main-pgu.c" support */
	if (!done && (!mstr || (streq(mstr, "pgu"))))
	{
		extern errr init_pgu(s32b, char**);
		if (0 == init_pgu(argc, argv))
		{
			ANGBAND_SYS = "pgu";
			done = TRUE;
		}
	}
#endif

#ifdef USE_LUA_GUI
	/* Attempt to use the "main-lua.c" support */
	if (!done && (!mstr || (streq(mstr, "lua"))))
	{
		extern errr init_lua_gui(s32b, char**);
		if (0 == init_lua_gui(argc, argv))
		{
			ANGBAND_SYS = "lua";
			done = TRUE;
		}
	}
#endif

	/* Make sure we have a display! */
	if (!done) quit("Unable to prepare any 'display module'!");


	/* Catch nasty signals */
	signals_init();

	/* Initialize */
	init_angband(force_module);

	/* Hack -- If requested, display scores and quit */
	if (show_score > 0) display_scores(0, show_score);

	/* Wait for response */
	pause_line(15);

	/* Play the game */
	play_game(new_game);

	/* Save settings upon exit */
	esettings_save();

#ifdef CHECK_MEMORY_LEAKS
	CHECK_LEAKS();
#endif

	/* Quit */
	quit(NULL);

	/* Exit */
	return (0);
}

#endif



