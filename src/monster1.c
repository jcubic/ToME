/* File: monster1.c */

/* Purpose: describe monsters (using monster memory) */

/*
 * Copyright (c) 1989 James E. Wilson, Christopher J. Stuart
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

void monstmem_add(s32b class, s32b type, monster_type *m_ptr, s32b val)
{
	call_lua("memory.add","(d,d,M,d)","",class, type, m_ptr, val);
}

void monstmem_add2(s32b class, s32b type1, s32b type2, monster_type *m_ptr,
				   s32b val)
{
	call_lua("memory.add2","(d,d,d,M,d)","",class, type1, type2, m_ptr, val);
}

s32b mon_killwall(s32b y, s32b x, s32b m_idx)
{
	s32b ret;
	call_lua("kill_wall.mon_kill", "(d,d,d)", "d", y, x, m_idx, &ret);

	return ret;
}

#if 0
/*
 * Pronoun arrays, by gender.
 */
static cptr wd_he[3] = { "it", "he", "she" };
static cptr wd_his[3] = { "its", "his", "her" };


/*
 * Pluralizer.  Args(count, singular, plural)
 */
#define plural(c,s,p) \
(((c) == 1) ? (s) : (p))






/*
 * Determine if the "armor" is known
 * The higher the level, the fewer kills needed.
 */
static bool know_armour(s32b r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	s32b level = r_ptr->level;

	s32b kills = r_ptr->r_tkills;

	/* Normal monsters */
	if (kills > 304 / (4 + level)) return (TRUE);

	/* Skip non-uniques */
	if (!(has_flag(r_ptr, FLAG_UNIQUE))) return (FALSE);

	/* Unique monsters */
	if (kills > 304 / (38 + (5*level) / 4)) return (TRUE);

	/* Assume false */
	return (FALSE);
}


/*
 * Determine if the "damage" of the given attack is known
 * the higher the level of the monster, the fewer the attacks you need,
 * the more damage an attack does, the more attacks you need
 */
static bool know_damage(s32b r_idx, s32b i)
{
	monster_race *r_ptr = &r_info[r_idx];

	s32b level = r_ptr->level;

	s32b a = r_ptr->r_blows[i];

	s32b d1 = r_ptr->blow[i].d_dice;
	s32b d2 = r_ptr->blow[i].d_side;

	s32b d = d1 * d2;

	/* Normal monsters */
	if ((4 + level) * a > 80 * d) return (TRUE);

	/* Skip non-uniques */
	if (!(has_flag(r_ptr, FLAG_UNIQUE))) return (FALSE);

	/* Unique monsters */
	if ((4 + level) * (2 * a) > 80 * d) return (TRUE);

	/* Assume false */
	return (FALSE);
}
#endif

/*
 * Hack -- display monster information using "text_out()"
 *
 * Note that there is now a compiler option to only read the monster
 * descriptions from the raw file when they are actually needed, which
 * saves about 60K of memory at the cost of disk access during monster
 * recall, which is optional to the user.
 *
 * This function should only be called with the cursor placed at the
 * left edge of the screen, on a cleared line, in which the recall is
 * to take place.  One extra blank line is left after the recall.
 */
static void roff_aux(s32b r_idx, s32b ego, s32b remem)
{
	call_lua("memory.main","(d,d)","",r_idx,ego);
}

/*
 * Hack -- Display the "name" and "attr/chars" of a monster race
 */
static void roff_top(s32b r_idx, s32b ego)
{
	monster_race *r_ptr = race_info_idx(r_idx, ego);

	byte	a1, a2;
	char	c1, c2;


	/* Access the chars */
	c1 = r_ptr->d_char;
	c2 = r_ptr->x_char;

	/* Access the attrs */
	a1 = r_ptr->d_attr;
	a2 = r_ptr->x_attr;

	/* Hack -- fake monochrome */
	if (!use_color) a1 = TERM_WHITE;
	if (!use_color) a2 = TERM_WHITE;

	/* Clear the top line */
	Term_erase(0, 0, 255);

	/* Reset the cursor */
	Term_gotoxy(0, 0);

	/* A title (use "The" for non-uniques) */
	if (!(has_flag(r_ptr, FLAG_UNIQUE)))
	{
		Term_addstr( -1, TERM_WHITE, "The ");
	}

	/* Dump the name */
	if (ego)
	{
		if (re_info[ego].before) Term_addstr( -1, TERM_WHITE, format("%s %s", re_info[ego].name, r_ptr->name));
		else Term_addstr( -1, TERM_WHITE, format("%s %s", r_ptr->name, re_info[ego].name));
	}
	else
	{
		Term_addstr( -1, TERM_WHITE, r_ptr->name);
	}

	/* Append the "standard" attr/char info */
	Term_addstr( -1, TERM_WHITE, " ('");
	Term_addch(a1, c1);
	if (use_bigtile && (a1 & 0x80)) Term_addch(255, 127);
	Term_addstr( -1, TERM_WHITE, "')");

	/* Append the "optional" attr/char info */
	Term_addstr( -1, TERM_WHITE, "/('");
	Term_addch(a2, c2);
	if (use_bigtile && (a2 & 0x80)) Term_addch(255, 127);
	Term_addstr( -1, TERM_WHITE, "'):");
}



/*
 * Hack -- describe the given monster race at the top of the screen
 */
void screen_roff(s32b r_idx, s32b ego, s32b remember)
{
	/* Flush messages */
	msg_print(NULL);

	/* Begin recall */
	Term_erase(0, 1, 255);

	/* Recall monster */
	roff_aux(r_idx, ego, remember);

	/* Describe monster */
	roff_top(r_idx, ego);
}




/*
 * Hack -- describe the given monster race in the current "term" window
 */
void display_roff(s32b r_idx, s32b ego)
{
	s32b ret, y;

	/* Efficiency hack: only output text if it's changed */
	call_lua("memory.main_changed", "(d,d)", "d", r_idx, ego, &ret);
	if(!ret) return;

	/* Erase the window */
	for (y = 0; y < Term->hgt; y++)
	{
		/* Erase the line */
		Term_erase(0, y, 255);
	}

	/* Begin recall */
	Term_gotoxy(0, 1);

	/* Recall monster */
	roff_aux(r_idx, ego, 0);

	/* Describe monster */
	roff_top(r_idx, ego);
}


bool monster_quest(s32b r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	/* Random quests are in the dungeon */
	if (!has_flag(r_ptr, FLAG_DUNGEON)) return FALSE;

	/* No random quests for aquatic monsters */
	if (has_flag(r_ptr, FLAG_AQUATIC)) return FALSE;

	/* No random quests for multiplying monsters */
	if (has_flag(r_ptr, FLAG_MULTIPLY)) return FALSE;

	return TRUE;
}

bool monster_dungeon(s32b r_idx)
{
	return TRUE;
}


void set_mon_num_hook(void)
{
	get_mon_num_hook = monster_dungeon;
}


/*
 * Check if monster can cross terrain
 */
bool monster_can_cross_terrain(byte feat, flags_type *flags)
{
#if 0 // DGDGDGDG
	/* Deep water */
	if (feat == FEAT_DEEP_WATER)
	{
		if (flag_exists(flags, FLAG_AQUATIC) ||
		                flag_exists(flags, FLAG_CAN_FLY) ||
		                flag_exists(flags, FLAG_CAN_SWIM))
			return TRUE;
		else
			return FALSE;
	}
	/* Shallow water */
	else if (feat == FEAT_SHAL_WATER)
	{
		if (flag_exists(flags, FLAG_AURA_FIRE))
			return FALSE;
		else
			return TRUE;
	}
	/* Aquatic monster */
	else if (flag_exists(flags, FLAG_AQUATIC) &&
	                !flag_exists(flags, FLAG_CAN_FLY))
	{
		return FALSE;
	}
	/* Lava */
	else if ((feat == FEAT_SHAL_LAVA) ||
	                (feat == FEAT_DEEP_LAVA))
	{
		if (flag_exists(flags, FLAG_IM_FIRE) ||
		                flag_exists(flags, FLAG_CAN_FLY))
			return TRUE;
		else
			return FALSE;
	}
#endif
	return TRUE;
}


void set_mon_num2_hook(s32b y, s32b x)
{
	get_mon_num2_hook = NULL;
}
