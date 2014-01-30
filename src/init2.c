/* File: init2.c */

/* Purpose: Initialisation (part 2) -BEN- */

#include "angband.h"
#include "music.h"
#include "lua.h"
#include "tolua.h"
extern lua_State* L;

#ifdef AUTOPACKAGE
/* Use autopackage binreloc utility to locate the config file */
#include "prefix.h"
#endif


/*
 * This file is used to initialise various variables and arrays for the
 * Angband game.  Note the use of "fd_read()" and "fd_write()" to bypass
 * the common limitation of "read()" and "write()" to only 32767 bytes
 * at a time.
 *
 * Several of the arrays for Angband are built from "template" files in
 * the "lib/file" directory, from which quick-load binary "image" files
 * are constructed whenever they are not present in the "lib/data"
 * directory, or if those files become obsolete, if we are allowed.
 *
 * Warning -- the "ascii" file parsers use a minor hack to collect the
 * name and text information in a single pass.  Thus, the game will not
 * be able to load any template file with more than 20K of names or 60K
 * of text, even though technically, up to 64K should be legal.
 *
 * The "init1.c" file is used only to parse the ascii template files,
 * to create the binary image files.  If you include the binary image
 * files instead of the ascii template files, then you can undefine
 * "ALLOW_TEMPLATES", saving about 20K by removing "init1.c".  Note
 * that the binary image files are extremely system dependant.
 */


/*
 * Setup physfs default paths
 */
void init_physfs(char *argv0)
{
	char *base_dir = NULL;
	s32b base_dir_len = 0;

	/* Init the physfs abstraction layer*/
	if (!PHYSFS_init(argv0))
	{
		quit(format("PHYSFS_init() failed!\n  reason: %s.\n", PHYSFS_getLastError()));
	}

	/*
	 * Get the initial search path from tome.cfg
	 * we need lua initialized to do that
	 */
	init_lua();

	/* Now we add the current path to load tome.cfg */
#if defined(EMBEDED_TOME_CFG)
	if (argv0 && (strlen(argv0) > 0))
	{
		/* Extract executable name */
		char *exe_name = &argv0[strlen(argv0) - 1];

		while ((exe_name >= argv0) && (strstr(exe_name, PHYSFS_getDirSeparator()) == NULL))
			exe_name--;
		exe_name++;

		base_dir_len = strlen(PHYSFS_getBaseDir()) + strlen(exe_name) + strlen(PHYSFS_getDirSeparator()) + 1;
		C_MAKE(base_dir, base_dir_len, char);
		strcpy(base_dir, PHYSFS_getBaseDir());
		strcat(base_dir, PHYSFS_getDirSeparator());
		strcat(base_dir, exe_name);
	}
	else
		quit(format("Argv0 is null or 0 sized, this is WRONG with an embeded %s file!", SETTINGS_LUA));
#elif defined(FORCE_TOME_CFG_BINRELOC)
	/* Use binreloc */
	{
		BrInitError error;

		if (br_init(&error) == 0 && error != BR_INIT_ERROR_DISABLED)
		{
			quit(format("Warning: BinReloc failed to initialize (error code %d)\n", error));
		}

		base_dir = br_strcat(br_find_data_dir("/usr/share"), "/tome3");
	}
#elif defined(FORCE_TOME_CFG_PATH)
	/* tome.cfg path forced, let's comply */
	base_dir = FORCE_TOME_CFG_PATH;
#else
	base_dir = (char*)PHYSFS_getBaseDir();
#endif
	if (!PHYSFS_addToSearchPath(base_dir, TRUE))
		quit(format("Oops! Could not set the path(%s) to find %s. Aborting. (%s)", base_dir, SETTINGS_LUA, PHYSFS_getLastError()));

	/* Does tome.cfg even exists ?*/
	if (!file_exist(SETTINGS_LUA))
		quit(format("Oops! Could not find %s. Aborting.", SETTINGS_LUA));

	/* Load it ! */
	lua_pushstring(L, base_dir);
	lua_setglobal(L, "TOME_CFG_PATH");

	/* Add some helpers to the mix */
	exec_lua(
		"TOME_WRITE_ROOT = fs.get_user_dir()\n"
		"TOME_WRITE_RAW_FILES = 1\n"
		"TOME_BABY_INTERFACE = 1\n"
		"PATH_SEP = fs.get_dir_separator()\n"
		"if strsub(TOME_CFG_PATH, strlen(TOME_CFG_PATH)) ~= PATH_SEP then\n"
		"	TOME_CFG_PATH = TOME_CFG_PATH..PATH_SEP\n"
		"end\n"
		"function set_initial_search_path(t)\n"
		"	foreachi(t, function(i, path)\n"
		"		if type(path) == 'table' and path[2] == 'auto make' then\n"
		"			path[1] = gsub(path[1], '(/)', PATH_SEP)\n"
		"			local old = fs.get_write_dir()\n"
		"			fs.set_write_dir(path[3])\n"
		"			fs.mkdir(path[4])\n"
		"			fs.set_write_dir(old)\n"
		"			fs.add_to_search_path(path[1], true)\n"
		"		elseif type(path) == 'table' and path[2] == 'no fail' then\n"
		"			path[1] = gsub(path[1], '(/)', PATH_SEP)\n"
		"			fs.add_to_search_path(path[1], true)\n"
		"		else\n"
		"			path = gsub(path, '(/)', PATH_SEP)\n"
		"			if not fs.add_to_search_path(path, true) then quit('Unable to find '..path) end\n"
		"		end\n"
		"	end)\n"
		"end\n"
	);

	lua_dofile(L, SETTINGS_LUA);

	get_lua_var("TOME_WRITE_RAW_FILES", 'd', &tome_write_raw_files);

	/* And remove the current dir from search path*/
#ifndef EMBEDED_TOME_CFG
	base_dir_len = base_dir_len; /* Avoid silly warning */
	PHYSFS_removeFromSearchPath(base_dir);
#else
	C_FREE(base_dir, base_dir_len, char);
#endif

	/*
	 * Now we are done with the real filesystem TENGINE_DIR
	 * it becomes the root(/) of the virtual FS
	 */
	string_free(TENGINE_DIR);
	TENGINE_DIR = string_make("/");

	/* So I can make links for modules, i'm lazy, also helps testing*/
	PHYSFS_permitSymbolicLinks(TRUE);
}


/*
 * Setup the users .tome and engine directory
 */
static void init_user_home(void)
{
	cptr write_dir;
	s32b major, minor, patch;

	/* Set write dir to user dir to make .tome if neccesary */
	get_lua_var("TOME_WRITE_ROOT", 's', &write_dir);
	PHYSFS_setWriteDir(write_dir);
	PHYSFS_addToSearchPath(write_dir, FALSE);

	/* Make the additional dirs if needed */
	PHYSFS_mkdir(PRIVATE_USER_PATH "/modules/");
	PHYSFS_mkdir(PRIVATE_USER_PATH "/tmp/");

	/* Get engine version */
	tome_dofile_anywhere(TENGINE_DIR_ENGINE, "mods_aux.lua", TRUE, FALSE);
	tome_dofile_anywhere(TENGINE_DIR_ENGINE, "engine.lua", TRUE, FALSE);
	call_lua("get_engine_version", "()", "ddd", &major, &minor, &patch);

	/* Create the engine version directory */
	ENGINE_VERSION = string_make(format("/%d.%d/", major, minor));
	ENGINE_DIR = string_make(format(PRIVATE_USER_PATH "%s", ENGINE_VERSION));
	PHYSFS_mkdir(ENGINE_DIR);

	/* Update the search paths */
	ENGINE_DIR_DEP = PHYSFS_getDependentPath(ENGINE_DIR);
	PHYSFS_removeFromSearchPath(write_dir);
	if (!PHYSFS_addToSearchPath(ENGINE_DIR_DEP, TRUE))
		quit(format("Physfs:addToSearchPath(): Failure reason: %s (%s).\n", PHYSFS_getLastError(), ENGINE_DIR_DEP));
}


/*
 * Find the default paths to all of our important sub-directories.
 *
 * The purpose of each sub-directory is described in "variable.c".
 *
 * All of the sub-directories should, by default, be located inside
 * the main "lib" directory, whose location is very system dependant.
 *
 * This function takes a writable buffer, initially containing the
 * "path" to the "lib" directory, for example, "/pkg/lib/angband/",
 * or a system dependant string, for example, ":lib:".  The buffer
 * must be large enough to contain at least 32 more characters.
 *
 * Various command line options may allow some of the important
 * directories to be changed to user-specified directories, most
 * importantly, the "info" and "user" and "save" directories,
 * but this is done after this function, see "main.c".
 *
 * In general, the initial path should end in the appropriate "PATH_SEP"
 * string.  All of the "sub-directory" paths (created below or supplied
 * by the user) will NOT end in the "PATH_SEP" string, see the special
 * "path_build()" function in "util.c" for more information.
 *
 * Hack -- first we free all the strings, since this is known
 * to succeed even if the strings have not been allocated yet,
 * as long as the variables start out as "NULL".  This allows
 * this function to be called multiple times, for example, to
 * try several base "path" values until a good one is found.
 */
void init_file_paths(char *argv0)
{
	/*** Free everything ***/

	/* Free the main path */
	string_free(TENGINE_DIR);

	/* Free the sub-paths */
	string_free(TENGINE_DIR_SCORES);
	string_free(TENGINE_DIR_BONE);
	string_free(TENGINE_DIR_CMOV);
	string_free(TENGINE_DIR_DATA);
	string_free(TENGINE_DIR_DNGN);
	string_free(TENGINE_DIR_ENGINE);
	string_free(TENGINE_DIR_FILE);
	string_free(TENGINE_DIR_FONT);
	string_free(TENGINE_DIR_GRAF);
	string_free(TENGINE_DIR_HELP);
	string_free(TENGINE_DIR_INFO);
	string_free(TENGINE_DIR_MODULES);
	string_free(TENGINE_DIR_NOTE);
	string_free(TENGINE_DIR_PATCH);
	string_free(TENGINE_DIR_PREF);
	string_free(TENGINE_DIR_RAW);
	string_free(TENGINE_DIR_SAVE);
	string_free(TENGINE_DIR_SCPT);
	string_free(TENGINE_DIR_USER);
	string_free(TENGINE_DIR_XTRA);

	/* Init fs virtualization */
	init_physfs(argv0);

	/*** Build the sub-directory names ***/

	TENGINE_DIR_SCORES = string_make("/scores");
	TENGINE_DIR_BONE = string_make("/bone");
	TENGINE_DIR_CMOV = string_make("/cmov");
	TENGINE_DIR_DATA = string_make("/data");
	TENGINE_DIR_DNGN = string_make("/dngn");
	TENGINE_DIR_ENGINE = string_make("/engine");
	TENGINE_DIR_FILE = string_make("/file");
	TENGINE_DIR_FONT = string_make("/media/font");
	TENGINE_DIR_GRAF = string_make("/media/graf");
	TENGINE_DIR_HELP = string_make("/help");
	TENGINE_DIR_INFO = string_make("/info");
	TENGINE_DIR_MODULES = string_make("/modules");
	TENGINE_DIR_NOTE = string_make("/note");
	TENGINE_DIR_PATCH = string_make("/addons");
	TENGINE_DIR_PREF = string_make("/pref");
	TENGINE_DIR_RAW = string_make("/raw");
	TENGINE_DIR_SAVE = string_make("/save");
	TENGINE_DIR_SCPT = string_make("/scripts");
	TENGINE_DIR_USER = string_make("/user");
	TENGINE_DIR_XTRA = string_make("/media");


	/* create the users .tome and game directories */
	init_user_home();
}

/*
 * Hack -- take notes on line 23
 */
static void note(cptr str)
{
	Term_erase(0, 23, 255);
	Term_putstr(20, 23, -1, TERM_WHITE, str);
	Term_fresh();
}

/*
 * Initialize the very basic arrays
 */
static void init_basic()
{
	/* Macro variables */
	C_MAKE(macro__pat, MACRO_MAX, cptr);
	C_MAKE(macro__act, MACRO_MAX, cptr);
	C_MAKE(macro__cmd, MACRO_MAX, bool);

	/* Macro action buffer */
	C_MAKE(macro__buf, 1024, char);

	/* Extended trigger macros */
	C_MAKE(cli_info, CLI_MAX, cli_comm);
}

/*
 * Pseudo, dummy quest initializer, to actualy disable them
 */
static bool quest_disable_init_hook(s32b q_idx)
{
	q_idx = q_idx;
	return FALSE;
}


/*
 * Initialise misc. values
 */
static errr init_misc(void)
{
	s32b i;

	/*** Prepare the various "bizarre" arrays ***/

	/* Inventory */
	prepare_inventory(&p_ptr->inventory);

	/* Dungeon flags */
	MAKE(dungeon_flags, flags_holder_type);
	flag_init(&dungeon_flags->flags, 2);

	/* Quark variables */
	C_MAKE(quark__str, QUARK_MAX, cptr);

	/* Message variables */
	C_MAKE(message__ptr, MESSAGE_MAX, u16b);
	C_MAKE(message__color, MESSAGE_MAX, byte);
	C_MAKE(message__type, MESSAGE_MAX, byte);
	C_MAKE(message__count, MESSAGE_MAX, u16b);
	C_MAKE(message__buf, MESSAGE_BUF, char);

	/* Hack -- No messages yet */
	message__tail = MESSAGE_BUF;

	/* Prepare powers */
	powers_type = NULL;
	power_max = POWER_MAX_INIT;
	reinit_powers_type(power_max);
	(void) C_COPY(powers_type, powers_type_init, POWER_MAX_INIT, power_type);

	/* Prepare inventory slots */
	inventory_slot_names = NULL;
	reinit_inventory_slot_names(INVEN_TOTAL);
	(void) C_COPY(inventory_slot_names, inventory_slot_names_init, INVEN_TOTAL,
				  inventory_slot_type);

	quest = NULL;
	max_q_idx = MAX_Q_IDX_INIT;
	reinit_quests(max_q_idx);

	(void) C_COPY(quest, quest_init_tome, MAX_Q_IDX_INIT, quest_type);

	/* If we dont allow C quests, we dont let them init */
	for (i = 0; i < MAX_Q_IDX_INIT; i++)
	{
		quest[i].init = quest_disable_init_hook;
	}

	process_hooks(HOOK_INIT_GAME, "(s)", "begin");

	/* Initialise the values */
	tome_dofile_anywhere(TENGINE_DIR_DATA, "limits.lua", TRUE, FALSE);

	/* Init the spell effects */
	for (i = 0; i < MAX_EFFECTS; i++)
		effects[i].time = 0;

	return 0;
}


/*
 * Initialise town array
 */
static errr init_towns(void)
{
	s32b i = 0;

	/*** Prepare the Towns ***/

	/* Allocate the towns */
	C_MAKE(town_info, max_towns, town_type);

	for (i = 1; i < max_towns; i++)
	{
		if (i <= max_real_towns) town_info[i].flags |= (TOWN_REAL);

#if 0 // DGDGDGDG
		/* Allocate the stores */
		C_MAKE(town_info[i].store, max_st_idx, store_type);

		/* Fill in each store */
		for (j = 0; j < max_st_idx; j++)
		{
			/* Access the store */
			store_type *st_ptr = &town_info[i].store[j];

			/* Know who we are */
			st_ptr->st_idx = j;

			/* Assume full stock */
			st_ptr->stock_size = 0;
		}
#endif
	}
	return 0;
}

#if 0 // DGDGDGDG
void create_stores_stock(s32b t)
{
	s32b j, k;
	town_type *t_ptr = &town_info[t];

	if (t_ptr->stocked) return;

	for (j = 0; j < max_st_idx; j++)
	{
		store_type *st_ptr = &t_ptr->store[j];

		/* Assume full stock */
		st_ptr->stock_size = st_info[j].max_obj;

		/* Allocate the stock */
		C_MAKE(st_ptr->stock, st_ptr->stock_size, object_type*);

		for (k = 0; k < st_ptr->stock_size; k++)
			st_ptr->stock[k] = new_object();
	}
	t_ptr->stocked = TRUE;
}
#endif

/*
 * Pointer to wilderness_map
 */
typedef wilderness_map *wilderness_map_ptr;

/*
 * Initialise wilderness map array
 */
static errr init_wilderness(void)
{
	s32b i, j;

	/* Grab the size it */
	call_lua("load_map", "(s)", "", "wild/world.map");

	/* Allocate the wilderness (two-dimension array) */
	C_MAKE(wild_map, max_wild_y, wilderness_map_ptr);
	C_MAKE(wild_map[0], max_wild_x * max_wild_y, wilderness_map);

	/* Init the other pointers */
	for (i = 1; i < max_wild_y; i++)
		wild_map[i] = wild_map[0] + i * max_wild_x;

	for (i = 0; i < max_wild_y; i++)
		for (j = 0; j < max_wild_x; j++)
			flag_init(&wild_map[i][j].flags, 2);

	/* Load it */
	exec_lua("map.wild = map.wild_load");
	call_lua("load_map", "(s)", "", "wild/world.map");

	return 0;
}

/*
 * XXX XXX XXX XXX XXX Realloc is not guaranteed to work (see main-gtk.c
 * and main-mac.c.
 */
void reinit_powers_type(s16b new_size)
{
	power_type *new_powers_type;

	C_MAKE(new_powers_type, new_size, power_type);

	/* Reallocate the extra memory */
	if (powers_type)
	{
		(void) C_COPY(new_powers_type, powers_type, power_max, power_type);

		C_FREE(powers_type, power_max, power_type);
	}

	powers_type = new_powers_type;

	power_max = new_size;
}

void reinit_quests(s16b new_size)
{
	quest_type *new_quest;

	C_MAKE(new_quest, new_size, quest_type);

	/* Reallocate the extra memory */
	if (quest)
	{
		(void) C_COPY(new_quest, quest, max_q_idx, quest_type);

		C_FREE(quest, max_q_idx, quest_type);
	}

	quest = new_quest;

	max_q_idx = new_size;
}

void reinit_inventory_slot_names(s16b new_size)
{
	inventory_slot_type *new_inven;

	C_MAKE(new_inven, new_size, inventory_slot_type);

	/* Reallocate the extra memory */
	if (inventory_slot_names)
	{
		(void) C_COPY(new_inven, inventory_slot_names, INVEN_TOTAL,
					  inventory_slot_type);

		C_FREE(inventory_slot_names, INVEN_TOTAL, inventory_slot_type);
	}

	inventory_slot_names = new_inven;

	INVEN_TOTAL = new_size;
}


void init_tome_color(byte id, cptr name, char chr, s32b r, s32b g, s32b b)
{
	color_names[id] = strdup(name);
	angband_color_table[id][0] = 0;
	angband_color_table[id][1] = r;
	angband_color_table[id][2] = g;
	angband_color_table[id][3] = b;
	Term_xtra(TERM_XTRA_REACT, 0);
}

/*
 * Initialise some other arrays
 */
static errr init_other(void)
{
	s32b i;

	/*** Prepare the "dungeon" information ***/

	/* Allocate and Wipe the special gene flags */
	C_MAKE(m_allow_special, max_r_idx, bool);
	C_MAKE(k_allow_special, max_k_idx, bool);
	C_MAKE(a_allow_special, max_a_idx, bool);


	/*** Prepare "vinfo" array ***/

	/* Used by "update_view()" */
	(void)vinfo_init();


	/* Allocate and Wipe the monster list */
	flag_empty(&monst_list);

	/* Allocate and Wipe the to keep monster list */
	flag_empty(&keep_monst_list);

	/* Allocate and Wipe the max dungeon level */
	C_MAKE(max_dlv, max_d_idx, s16b);

	/* Allocate and Wipe the special levels */
	for (i = 0; i < MAX_DUNGEON_DEPTH; i++)
	{
		C_MAKE(special_lvl[i], max_d_idx, bool);
	}

	/* Allocate and wipe each line of the cave */
	for (i = 0; i < MAX_HGT; i++)
	{
		/* Allocate one row of the cave */
		C_MAKE(cave[i], MAX_WID, cave_type);
	}

	/*** Pre-allocate the basic "auto-inscriptions" ***/

	/* The "basic" feelings */
	(void)quark_add("cursed");
	(void)quark_add("broken");
	(void)quark_add("average");
	(void)quark_add("good");

	/* The "extra" feelings */
	(void)quark_add("excellent");
	(void)quark_add("worthless");
	(void)quark_add("special");
	(void)quark_add("terrible");

	/* Some extra strings */
	(void)quark_add("uncursed");
	(void)quark_add("on sale");

	/*
	 * Install the various level generators
	 */
	add_level_generator("dungeon", level_generate_dungeon, TRUE, TRUE, TRUE, TRUE);
	add_level_generator("maze", level_generate_maze, TRUE, TRUE, TRUE, TRUE);
	add_level_generator("life", level_generate_life, TRUE, TRUE, TRUE, TRUE);

	/*** Pre-allocate space for the "format()" buffer ***/

	/* Hack -- Just call the "format()" function */
	(void)format("%s (%s).", "Dark God <darkgod@t-o-m-e.net>", MAINTAINER);

	/* Success */
	return (0);
}



/*
 * Initialise some other arrays
 */
static errr init_alloc(void)
{
	s32b i;

	monster_race *r_ptr;

	alloc_entry *table;

	s16b num[MAX_DEPTH_MONSTER];

	s16b aux[MAX_DEPTH_MONSTER];

	s16b *tmp;

	/*** Analyze monster allocation info ***/

	/* Clear the "aux" array */
	tmp = C_WIPE(&aux, MAX_DEPTH_MONSTER, s16b);

	/* Clear the "num" array */
	tmp = C_WIPE(&num, MAX_DEPTH_MONSTER, s16b);

	/* Size of "alloc_race_table" */
	alloc_race_size = 0;

	/* Scan the monsters */
	for (i = 1; i < max_r_idx; i++)
	{
		/* Get the i'th race */
		r_ptr = &r_info[i];

		/* Legal monsters */
		if (r_ptr->rarity)
		{
			/* Count the entries */
			alloc_race_size++;

			/* Group by level */
			num[r_ptr->level]++;
		}
	}

	/* Collect the level indexes */
	for (i = 1; i < MAX_DEPTH_MONSTER; i++)
	{
		/* Group by level */
		num[i] += num[i - 1];
	}

	/* Paranoia */
// DGDGDGDG why ?	if (!num[0]) quit("No town monsters!");


	/*** Initialise monster allocation info ***/

	/* Allocate the alloc_race_table */
	C_MAKE(alloc_race_table, alloc_race_size, alloc_entry);

	/* Access the table entry */
	table = alloc_race_table;

	/* Scan the monsters */
	for (i = 1; i < max_r_idx; i++)
	{
		/* Get the i'th race */
		r_ptr = &r_info[i];

		/* Count valid pairs */
		if (r_ptr->rarity)
		{
			s32b p, x, y, z;

			/* Extract the base level */
			x = r_ptr->level;

			/* Extract the base probability */
			p = (100 / r_ptr->rarity);

			/* Skip entries preceding our locale */
			y = (x > 0) ? num[x - 1] : 0;

			/* Skip previous entries at this locale */
			z = y + aux[x];

			/* Load the entry */
			table[z].index = i;
			table[z].level = x;
			table[z].prob1 = p;
			table[z].prob2 = p;
			table[z].prob3 = p;

			/* Another entry complete for this locale */
			aux[x]++;
		}
	}


	/* Success */
	return (0);
}

/* Init the sets in a_info */
void init_sets_aux()
{
	s32b i, j;

	for (i = 0; i < max_a_idx; i++)
		a_info[i].set = -1;
	for (i = 0; i < max_set_idx; i++)
	{
		for (j = 0; j < set_info[i].num; j++)
		{
			a_info[set_info[i].arts[j].a_idx].set = i;
			a_info[set_info[i].arts[j].a_idx].set_pos = j;
		}
	}
}

/*
 * Mark guardians and their artifacts with SPECIAL_GENE flag
 */
static void init_guardians(void)
{
	s32b i;

	/* Scan dungeons */
	for (i = 0; i < max_d_idx; i++)
	{
		dungeon_info_type *d_ptr = &d_info[i];

		/* Mark the guadian monster */
		if (has_flag(d_ptr, FLAG_DUNGEON_GUARDIAN))
		{
			monster_race *r_ptr = &r_info[get_flag(d_ptr, FLAG_DUNGEON_GUARDIAN)];

			bool_flag(r_ptr, FLAG_SPECIAL_GENE);

			/* Mark the final artifact */
			if (has_flag(d_ptr, FLAG_DUNGEON_ARTIFACT))
			{
				artifact_type *a_ptr = &a_info[get_flag(d_ptr, FLAG_DUNGEON_ARTIFACT)];

				bool_flag(a_ptr, FLAG_SPECIAL_GENE);
			}

			/* Mark the final object */
			if (has_flag(d_ptr, FLAG_DUNGEON_OBJECT))
			{
				object_kind *k_ptr = &k_info[get_flag(d_ptr, FLAG_DUNGEON_OBJECT)];

				bool_flag(k_ptr, FLAG_SPECIAL_GENE);
			}

			/* Give randart if there are no final artifacts */
			if ((!has_flag(d_ptr, FLAG_DUNGEON_ARTIFACT)) && (!has_flag(d_ptr, FLAG_DUNGEON_OBJECT)))
			{
				bool_flag(r_ptr, FLAG_DROP_RANDART);
			}
		}
	}
}

/*
 * Hack -- Explain a broken "lib" folder and quit (see below).
 *
 * XXX XXX XXX This function is "messy" because various things
 * may or may not be initialised, but the "plog()" and "quit()"
 * functions are "supposed" to work under any conditions.
 */
static void init_angband_aux(cptr why)
{
	/* Why */
	plog(why);

	/* Explain */
	plog("The 'lib' directory is probably missing or broken.");

	/* More details */
	plog("Perhaps the archive was not extracted correctly.");

	/* Explain */
	plog("See the 'README' file for more information.");

	/* Quit with error */
	quit("Fatal Error.");
}

/* Init some variables */
void init_vars()
{
	/* Init */
#define init_skill(skill, name) \
	(skill) = find_skill(name);
#define init_ability(ab, name) \
	(ab) = find_ability(name);
#include "init.h"
#undef init_skill
#undef init_ability

	/* Now export */
/*	tolua_open(L);
#define init_skill(skill, name) \
	if (skill != -1) tolua_constant(L, NULL, #skill, skill);
#define init_ability(skill, name) \
	if (skill != -1) tolua_constant(L, NULL, #skill, skill);
#include "init.h"
#undef init_skill
#undef init_ability
*/
}


/*
 * Hack -- main Angband initialisation entry point
 *
 * Verify some files, display the "news.txt" file, create
 * the high score file, initialise all internal arrays, and
 * load the basic "user pref files".
 *
 * Note that we blindly assume that "news2.txt" exists. XXX
 *
 * Be very careful to keep track of the order in which things
 * are initialised, in particular, the only thing *known* to
 * be available when this function is called is the "z-term.c"
 * package, and that may not be fully initialised until the
 * end of this function, when the default "user pref files"
 * are loaded and "Term_xtra(TERM_XTRA_REACT,0)" is called.
 *
 * Note that this function attempts to verify the "news" file,
 * and the game aborts (cleanly) on failure, since without the
 * "news" file, it is likely that the "lib" folder has not been
 * correctly located.  Otherwise, the news file is displayed for
 * the user.
 *
 * Note that this function attempts to verify (or create) the
 * "high score" file, and the game aborts (cleanly) on failure,
 * since one of the most common "extraction" failures involves
 * failing to extract all sub-directories (even empty ones), such
 * as by failing to use the "-d" option of "pkunzip", or failing
 * to use the "save empty directories" option with "Compact Pro".
 * This error will often be caught by the "high score" creation
 * code below, since the "lib/apex" directory, being empty in the
 * standard distributions, is most likely to be "lost", making it
 * impossible to create the high score file.
 *
 * Note that various things are initialised by this function,
 * including everything that was once done by "init_some_arrays".
 *
 * This initialisation involves the parsing of special files
 * in the "lib/data" and sometimes the "lib/edit" directories.
 *
 * Note that the "template" files are initialised first, since they
 * often contain errors.  This means that macros and message recall
 * and things like that are not available until after they are done.
 *
 * We load the default "user pref files" here in case any "color"
 * changes are needed before character creation.
 *
 * Note that the "graf-xxx.prf" file must be loaded separately,
 * if needed, in the first (?) pass through "TERM_XTRA_REACT".
 */
void init_angband(cptr force_module)
{
	PHYSFS_file *fp;

	char *news_file;

	char buf[1024];

	/* Init some VERY basic stuff, like macro arrays */
	init_basic();

	/* Initilize the socket */
	init_net_subsystem();

	/* Init the sounds */
	init_music_subsystem();

	/* Initialize hooks list */
	wipe_hooks();

	/* Select & init a module if needed */
	select_module(force_module);

	/*** Choose which news.txt file to use ***/

	/* Choose the news file */
	if ((time(NULL) % 2) && file_exist("/file/news2.txt"))
		news_file = "news2.txt";
	else
		news_file = "news.txt";

	/*** Verify the "news" file ***/

	/* Build the filename */
	path_build(buf, sizeof(buf), TENGINE_DIR_FILE, news_file);

	/* Failure */
	if (!file_exist(buf))
	{
		char why[1024];

		/* Message */
		sprintf(why, "Cannot access the '%s' file!", buf);

		/* Crash and burn */
		init_angband_aux(why);
	}


	/*** Display the "news" file ***/

	/* Clear screen */
	Term_clear();

	/* Build the filename */
	path_build(buf, sizeof(buf), TENGINE_DIR_FILE, news_file);

	/* Open the News file */
	fp = my_fopen(buf, "r");

	/* Dump */
	if (fp)
	{
		s32b i = 0;

		/* Dump the file to the screen */
		while (0 == my_fgets(fp, buf, 1024))
		{
			/* Display and advance - we use display_message to parse colour codes XXX */
			display_message(0, i++, strlen(buf), TERM_WHITE, buf);
		}

		/* Close */
		my_fclose(fp);
	}

	/* Flush it */
	Term_fresh();

	/*** Initialise some arrays ***/

	/* Default factions hostility */
	set_faction_friendliness(FACTION_PLAYER, FACTION_DUNGEON, -100);
	set_faction_friendliness(FACTION_DUNGEON, FACTION_PLAYER, -100);
	set_faction_friendliness(FACTION_PLAYER, FACTION_TOWN, 0);
	set_faction_friendliness(FACTION_TOWN, FACTION_PLAYER, 0);
	set_faction_friendliness(FACTION_TOWN, FACTION_DUNGEON, -100);
	set_faction_friendliness(FACTION_DUNGEON, FACTION_TOWN, -100);
	flag_set_string(&faction_names, FACTION_PLAYER, "ally");
	flag_set_string(&faction_names, FACTION_TOWN, "townspeople");
	flag_set_string(&faction_names, FACTION_DUNGEON, "enemy");

	/* Initialise misc. values */
	note("[Initialising values... (misc)]");
	if (init_misc()) quit("Cannot initialise misc. values");

	/* Register all the basic flags */
	note("[Initialising flags... (flags)]");
	setup_flags();

	/* Register all the basic speeds */
	note("[Initialising speeds... (speeds)]");
	setup_speeds();

	/* Initialise some other arrays */
	note("[Initialising lua scripting... (lua)]");
	init_lua();
	init_lua_init();

	/* Load function registry, if any */
	note("[Initialising lua functions registry... (loading)]");
	if (load_functions_registry()) quit("Cannot load lua functions registry");

	/* Initialise skills info */
	note("[Initialising arrays... (skills)]");
	if (init_s_info()) quit("Cannot initialise skills");

	/* Initialise abilities info */
	note("[Initialising arrays... (abilities)]");
	if (init_ab_info()) quit("Cannot initialise abilities");

	/* Initialise feature info */
	note("[Initialising arrays... (features)]");
	if (init_f_info()) quit("Cannot initialise features");

	/* Initialise object info */
	note("[Initialising arrays... (objects)]");
	if (init_k_info()) quit("Cannot initialise objects");

	/* Initialise player info */
	note("[Initialising arrays... (players)]");
	if (init_descriptor_info()) quit("Cannot initialise players");

	/* Initialise artifact info */
	note("[Initialising arrays... (artifacts)]");
	if (init_a_info()) quit("Cannot initialise artifacts");

	/* Initialise set info */
	note("[Initialising item sets... (sets)]");
	if (init_set_info()) quit("Cannot initialise item sets");

	/* Initialise ego-item info */
	note("[Initialising arrays... (ego-items)]");
	if (init_e_info()) quit("Cannot initialise ego-items");

	/* Initialise randart parts info */
	note("[Initialising arrays... (randarts)]");
	if (init_ra_info()) quit("Cannot initialise randarts");

	/* Initialise monster info */
	note("[Initialising arrays... (monsters)]");
	if (init_r_info()) quit("Cannot initialise monsters");

	/* Initialise ego monster info */
	note("[Initialising arrays... (ego monsters)]");
	if (init_re_info()) quit("Cannot initialise ego monsters");

	/* Initialise dungeon type info */
	note("[Initialising arrays... (dungeon types)]");
	if (init_d_info()) quit("Cannot initialise dungeon types");
	init_guardians();

	/* Initialise actions type info */
	note("[Initialising arrays... (action types)]");
	if (init_ba_info()) quit("Cannot initialise action types");

	/* Initialise owners type info */
	note("[Initialising arrays... (owners types)]");
	if (init_ow_info()) quit("Cannot initialise owners types");

	/* Initialise stores type info */
	note("[Initialising arrays... (stores types)]");
	if (init_st_info()) quit("Cannot initialise stores types");

	/* Initialize vault info */
	note("[Initialising vaults... (vaults types)]");
	if (init_v_info()) quit("Cannot initialize vaults");

	/* Initialise wilderness features array */
	note("[Initialising arrays... (wilderness features)]");
	if (init_wf_info()) quit("Cannot initialise wilderness features");

	/* Initialise wilderness map array */
	note("[Initialising arrays... (wilderness map)]");
	if (init_wilderness()) quit("Cannot initialise wilderness map");

	/* Init some variables */
	init_vars();

	/* Ok it is done, lets load patches data if any */
	done_loading_data_files = TRUE;
	process_hooks(HOOK_INFO_DATA_LOADED, "()");
	init_sets_aux();

	/* Initialise town array */
	note("[Initialising arrays... (towns)]");
	if (init_towns()) quit("Cannot initialise towns");

	/* Initialise some other arrays */
	note("[Initialising arrays... (other)]");
	if (init_other()) quit("Cannot initialise other stuff");

	/* Initialise some other arrays */
	note("[Initialising arrays... (alloc)]");
	if (init_alloc()) quit("Cannot initialise alloc stuff");

	/* Save function registry, if any */
	if (tome_write_raw_files)
	{
		note("[Initialising lua functions registry... (saving)]");
		if (save_functions_registry()) quit("Cannot save lua functions registry");
	}

	/*** Load default user pref files ***/

	/* Initialise feature info */
	note("[Initialising user pref files...]");

	/* Access the "basic" pref file */
	strcpy(buf, "pref.prf");

	/* Process that file */
	process_pref_file(buf);

	/* Access the "basic" system pref file */
	sprintf(buf, "pref-%s.prf", ANGBAND_SYS);

	/* Process that file */
	process_pref_file(buf);

	/* Access the "user" pref file */
	sprintf(buf, "user.prf");

	/* Process that file */
	process_pref_file(buf);

	/* Access the "user" system pref file */
	sprintf(buf, "user-%s.prf", ANGBAND_SYS);

	/* Process that file */
	process_pref_file(buf);

	/* Initialise the automatizer */
	tome_dofile_anywhere(TENGINE_DIR_USER, "automat.atm", FALSE, FALSE);

	/* Done */
	note("[Initialisation complete]");

	process_hooks(HOOK_INIT_GAME, "(s)", "end");
}

/*
 * Clean up after init routines before ending game; mostly this means
 * freeing memory.  Though memory cleanup at this point is not
 * necessary, since it will be reclaimed on process exit anyways,
 * doing this prevents a memory leak from showing up when using
 * memory debugging tools.
 */
void cleanup_angband(void)
{
}
