/* File: modules.c */

/* Purpose: T-engine modules */

/*
 * Copyright (c) 2003 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

static void setup_writable_dir()
{
	s32b mod_version_major, mod_version_minor;
	cptr module_short_name;
	cptr module_short_dir;
	cptr module_dependent_path;
	cptr mod_dir;

	/* Set write dir to the engine dir */
	if (!PHYSFS_setWriteDir(ENGINE_DIR_DEP))
		quit(format("ERROR: Failed to set engine write dir (%s)\n\n%s", ENGINE_DIR_DEP, PHYSFS_getLastError()));

	/* Get module directory name */
	call_lua("get_module_info", "(s)", "s", "shortname", &module_short_name);
	call_lua("get_module_info", "(s,d)", "d", "version", 1, &mod_version_major);
	call_lua("get_module_info", "(s,d)", "d", "version", 2, &mod_version_minor);

	/* Make the module config dir if needed */
	module_short_dir = string_make(format("%s/%d.%d", module_short_name, mod_version_major, mod_version_minor));
	PHYSFS_mkdir(module_short_dir);

	/* Set the write dir to the newly created module config dir.
	   This needs to include the engine version, otherwise physfs will
	   choose the module directory over the newly created directory */
	module_dependent_path = string_make(format("%s/%s", ENGINE_VERSION, module_short_dir));
	mod_dir = PHYSFS_getDependentPath(module_dependent_path);
	if (mod_dir == NULL || !PHYSFS_setWriteDir(mod_dir))
		quit(format("ERROR: Failed to set module write dir (%s)\n\n%s", mod_dir, PHYSFS_getLastError()));

	/* Make data, save and user if needed */
	PHYSFS_mkdir("scores");
	if (tome_write_raw_files)
	{
		PHYSFS_mkdir("raw");
		PHYSFS_mkdir("raw/dungeon");
		PHYSFS_mkdir("raw/player");
		PHYSFS_mkdir("raw/stores");
		PHYSFS_mkdir("raw/items");
		PHYSFS_mkdir("raw/monsters");
		PHYSFS_mkdir("raw/wild");
	}
	PHYSFS_mkdir("save");
	PHYSFS_mkdir("user");
	PHYSFS_mkdir("note");
	PHYSFS_mkdir("cmov");
	PHYSFS_mkdir("tmp");

	/* Prepend the module config dir so that it is tried first */
	if (!PHYSFS_addToSearchPath(mod_dir, FALSE))
		quit(format("Physfs:addToSearchPath(): Failure reason: %s (%s).\n", PHYSFS_getLastError(), mod_dir));

	string_free(module_short_dir);
	string_free(module_dependent_path);
	string_free(mod_dir);
}

static void activate_module()
{
	/* Initialize the module table */
	call_lua("assign_current_module", "(s)", "", game_module);

	/* Initialize writable directory */
	setup_writable_dir();

	/* Do misc inits  */
	call_lua("get_module_info", "(s)", "d", "max_money", &PY_MAX_GOLD);
	call_lua("get_module_info", "(s)", "d", "max_exp", &PY_MAX_EXP);
	call_lua("get_module_info", "(s)", "d", "max_plev", &max_plev);
	PY_MAX_LEVEL = max_plev;
	C_MAKE(player_exp, max_plev, s32b);
	C_MAKE(player_hp, max_plev, s16b);

	call_lua("get_module_info", "(s)", "d", "max_skill_level", &SKILL_MAX);
	SKILL_MAX *= SKILL_STEP;

	call_lua("get_module_info", "(s,d)", "d", "version", 1, &VERSION_MAJOR);
	call_lua("get_module_info", "(s,d)", "d", "version", 2, &VERSION_MINOR);
	call_lua("get_module_info", "(s,d)", "d", "version", 3, &VERSION_PATCH);
	version_major = VERSION_MAJOR;
	version_minor = VERSION_MINOR;
	version_patch = VERSION_PATCH;

	/* Change window name if needed */
	if (strcmp(game_module, "ToME"))
	{
		strnfmt(angband_term_name[0], 79, "T-Engine: %s", game_module);
		Term_xtra(TERM_XTRA_RENAME_MAIN_WIN, 0);
	}

	/* Load graphics */
	Term_xtra(TERM_XTRA_LOAD_GFX, 0);

	/* Reprocess the player name, just in case */
	process_player_base();
}

/* Did the player force a module on command line */
cptr force_module = NULL;

/* Display possible modules and select one */
bool module_initialized = FALSE;
bool select_module(cptr force_module)
{
	cptr selected_module;

	/* Hack */
	use_color = TRUE;

	/* Set write dir to the engine dir */
	if (!PHYSFS_setWriteDir(ENGINE_DIR_DEP))
		quit(format("ERROR: Failed to set engine write dir (%s)\n\n%s", ENGINE_DIR_DEP, PHYSFS_getLastError()));

	/* Init some lua */
	tome_dofile_anywhere(TENGINE_DIR_ENGINE, "modules.lua", TRUE, FALSE);

	module_initialized = TRUE;

	/* Preprocess the basic prefs, we need them to have movement keys */
	process_pref_file("pref.prf");

	call_lua("select_module_screen", "(s)", "s", force_module, &selected_module);
	if (selected_module == NULL) exit(0);
	game_module = string_make(selected_module);
	activate_module();
	return FALSE;
}
