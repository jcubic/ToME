/* File: levels.c */

/* Purpose: Levels functions */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"


/*
 * Return the parameter of the given command in the given file
 */
static bool get_command(s32b dungeon, s32b level, cptr info, char *param)
{
	cptr ret = NULL;

	call_lua("__d_info.get_level_info", "(d,d,s)", "s", dungeon, level, info, &ret);
	if (ret != NULL)
	{
		strcpy(param, ret);
		return TRUE;
	}

	/* Assume command not found */
	return FALSE;
}


/*
 * Return the dungeon branch starting form the current dungeon/level
 */
s32b get_branch()
{
	char buf[10];

	/* Get and return the branch */
	if (get_command(dungeon_type, dun_level, "branch", buf)) return (atoi(buf));

	/* No branch ? return 0 */
	else return 0;
}

/*
 * Return the father dungeon branch
 */
s32b get_fbranch()
{
	char buf[10];

	/* Get and return the branch */
	if (get_command(dungeon_type, dun_level, "father", buf)) return (atoi(buf));

	/* No branch ? return 0 */
	else return 0;
}

/*
 * Return the father dungeon level
 */
s32b get_flevel()
{
	char buf[10];

	/* Get and return the level */
	if (get_command(dungeon_type, dun_level, "father_level", buf)) return (atoi(buf));

	/* No level ? return 0 */
	else return 0;
}

/*
 * Return the level generator
 */
bool get_dungeon_generator(char *buf)
{
	/* Get and return the level */
	if (get_command(dungeon_type, dun_level, "generator", buf)) return (TRUE);
	else return FALSE;
}

/*
 * Return the special level
 */
bool get_dungeon_special(char *buf)
{
	/* Get and return the level */
	if (get_command(dungeon_type, dun_level, "level_map", buf)) return (TRUE);
	else return FALSE;
}

/*
 * Return the special level name
 */
bool get_dungeon_name(char *buf)
{
	/* Get and return the level */
	if (get_command(dungeon_type, dun_level, "name", buf)) return (TRUE);
	else return FALSE;
}

/*
 * Return the special level name
 */
void get_level_flags()
{
	call_lua("__d_info.get_level_info", "(d,d,s)", "", dungeon_type, dun_level, "flags");
}

/*
 * Return the special level desc
 */
bool get_level_desc(char *buf)
{
	/* Get and return the level */
	if (get_command(dungeon_type, dun_level, "desc", buf)) return (TRUE);
	else return FALSE;
}
