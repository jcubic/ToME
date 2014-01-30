/* File: spells1.c */

/* Purpose: Spell code (part 1) */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lauxlib.h"
#include "tolua.h"

/*
 * Lua state
 */
extern lua_State* L;


/* 1/x chance of reducing stats (for elemental attacks) */
#define HURT_CHANCE 32

/* 1/x chance of hurting even if invulnerable!*/
#define PENETRATE_INVULNERABILITY 13

/* Maximum number of tries for teleporting */
#define MAX_TRIES 100


/*
 * Helper function -- return a "nearby" race for polymorphing
 *
 * Note that this function is one of the more "dangerous" ones...
 */
s16b poly_r_idx(s32b r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	s32b i, r;

#if 0 /* No more -- hehehe -- DG */
	/* Allowable range of "levels" for resulting monster */
	s32b lev1 = r_ptr->level - ((randint(20) / randint(9)) + 1);
	s32b lev2 = r_ptr->level + ((randint(20) / randint(9)) + 1);
#endif

	/* Hack -- Uniques never polymorph */
	if (has_flag(r_ptr, FLAG_UNIQUE))
		return (r_idx);

	/* Pick a (possibly new) non-unique race */
	for (i = 0; i < 1000; i++)
	{
		/* Pick a new race, using a level calculation */
		r = get_mon_num((dun_level + r_ptr->level) / 2 + 5);

		/* Handle failure */
		if (!r) break;

		/* Obtain race */
		r_ptr = &r_info[r];

		/* Ignore unique monsters */
		if (has_flag(r_ptr, FLAG_UNIQUE)) continue;

#if 0
		/* Ignore monsters with incompatible levels */
		if ((r_ptr->level < lev1) || (r_ptr->level > lev2)) continue;
#endif

		/* Use that index */
		r_idx = r;

		/* Done */
		break;
	}

	/* Result */
	return (r_idx);
}

/*
 * Teleport player, using a distance and a direction as a rough guide.
 *
 * This function is not at all obsessive about correctness.
 * This function allows teleporting into vaults (!)
 */
void teleport_player_directed(s32b rad, s32b dir)
{
	s32b y = p_ptr->py;
	s32b x = p_ptr->px;
	s32b yfoo = ddy[dir];
	s32b xfoo = ddx[dir];
	s32b min = rad / 4;
	s32b dis = rad;
	s32b i, d, j;
	bool look = TRUE;
	bool y_major = FALSE;
	bool x_major = FALSE;
	s32b y_neg = 1;
	s32b x_neg = 1;
	cave_type *c_ptr;

	if (xfoo == 0 && yfoo == 0)
	{
		teleport_player(rad);
		return;
	}

	if (yfoo == 0) x_major = TRUE;
	if (xfoo == 0) y_major = TRUE;
	if (yfoo < 0) y_neg = -1;
	if (xfoo < 0) x_neg = -1;

	/* Look until done */
	while (look)
	{
		/* Verify max distance */
		if (dis > 200)
		{
			teleport_player(rad);
			return;
		}

		/* Try several locations */
		for (i = 0; i < 500; i++)
		{
			/* Pick a (possibly illegal) location */
			for (j = 0; j < 500; j++)
			{
				if (y_major)
				{
					y = rand_spread(p_ptr->py + y_neg * dis / 2, dis / 2);
				}
				else
				{
					y = rand_spread(p_ptr->py, dis / 3);
				}

				if (x_major)
				{
					x = rand_spread(p_ptr->px + x_neg * dis / 2, dis / 2);
				}
				else
				{
					x = rand_spread(p_ptr->px, dis / 3);
				}

				d = distance(p_ptr->py, p_ptr->px, y, x);
				if ((d >= min) && (d <= dis)) break;
			}

			/* Ignore illegal locations */
			if (!in_bounds(y, x)) continue;

			/* Require "naked" floor space */
			if (!cave_empty_bold(y, x)) continue;

			/* This grid looks good */
			look = FALSE;

			/* Stop looking */
			break;
		}

		/* Increase the maximum distance */
		dis = dis * 2;

		/* Decrease the minimum distance */
		min = min / 2;

	}

	/* Sound */
	sound(SOUND_TELEPORT);

	/* Move player */
	teleport_player_to(y, x);

	/* Handle stuff XXX XXX XXX */
	handle_stuff();

	c_ptr = &cave[y][x];

	/* Hack -- enter a store if we are on one */
	if (has_flag(&f_info[c_ptr->feat], FLAG_CONTAINS_BUILDING))
	{
		/* Disturb */
		disturb(0, 0);

		/* Hack -- enter store */
		command_new = '_';
		energy_use = 0;
	}
}


/*
 * Teleport a monster, normally up to "dis" grids away.
 *
 * Attempt to move the monster at least "dis/2" grids away.
 *
 * But allow variation to prevent infinite loops.
 */
void teleport_away(s32b m_idx, s32b dis)
{
	s32b ny = 0, nx = 0, oy, ox, d, i, min;
	s32b tries = 0;

	bool look = TRUE;

	monster_type *m_ptr = get_monster(m_idx);

	if (intrinsic(RES_CONTINUUM))
	{
		msg_print("The space-time continuum can't be disrupted.");
		return;
	}

	/* Paranoia */
	if (!m_ptr->r_idx) return;

	/* Save the old location */
	oy = m_ptr->fy;
	ox = m_ptr->fx;

	/* Minimum distance */
	min = dis / 2;

	/* Look until done */
	while (look)
	{
		tries++;

		/* Verify max distance */
		if (dis > 200) dis = 200;

		/* Try several locations */
		for (i = 0; i < 500; i++)
		{
			/* Pick a (possibly illegal) location */
			while (1)
			{
				ny = rand_spread(oy, dis);
				nx = rand_spread(ox, dis);
				d = distance(oy, ox, ny, nx);
				if ((d >= min) && (d <= dis)) break;
			}

			/* Ignore illegal locations */
			if (!in_bounds(ny, nx)) continue;

			if (!monst_can_enter_square(m_ptr, ny, nx, NULL)) continue;

			/* No teleporting into vaults and such */
			if (!(p_ptr->inside_quest || p_ptr->inside_arena))
				if (cave[ny][nx].info & (CAVE_ICKY)) continue;

			/* This grid looks good */
			look = FALSE;

			/* Stop looking */
			break;
		}

		/* Increase the maximum distance */
		dis = dis * 2;

		/* Decrease the minimum distance */
		min = min / 2;

		/* Stop after MAX_TRIES tries */
		if (tries > MAX_TRIES) return;
	}

	/* Sound */
	sound(SOUND_TPOTHER);

	/* Update the new location */
	cave[ny][nx].m_idx = m_idx;
	last_teleportation_y = ny;
	last_teleportation_x = nx;

	/* Update the old location */
	cave[oy][ox].m_idx = 0;

	/* Move the monster */
	m_ptr->fy = ny;
	m_ptr->fx = nx;

	/* Update the monster (new location) */
	update_mon(m_idx, TRUE);

	/* Redraw the old grid */
	lite_spot(oy, ox);

	/* Redraw the new grid */
	lite_spot(ny, nx);

	/* Update monster light */
	if (has_flag(m_ptr, FLAG_HAS_LITE)) p_ptr->update |= (PU_MON_LITE);
}


/*
 * Teleport monster next to the player
 */
void teleport_to_player(s32b m_idx)
{
	s32b ny = 0, nx = 0, oy, ox, d, i, min;
	s32b dis = 2;

	bool look = TRUE;

	monster_type *m_ptr = get_monster(m_idx);

	s32b attempts = 500;

	if (intrinsic(RES_CONTINUUM))
	{
		msg_print("The space-time continuum can't be disrupted.");
		return;
	}

	/* Paranoia */
	if (!m_ptr->r_idx) return;

	/* "Skill" test */
	if (randint(100) > m_ptr->level) return;

	/* Save the old location */
	oy = m_ptr->fy;
	ox = m_ptr->fx;

	/* Minimum distance */
	min = dis / 2;

	/* Look until done */
	while (look && --attempts)
	{
		/* Verify max distance */
		if (dis > 200) dis = 200;

		/* Try several locations */
		for (i = 0; i < 500; i++)
		{
			/* Pick a (possibly illegal) location */
			while (1)
			{
				ny = rand_spread(p_ptr->py, dis);
				nx = rand_spread(p_ptr->px, dis);
				d = distance(p_ptr->py, p_ptr->px, ny, nx);
				if ((d >= min) && (d <= dis)) break;
			}

			/* Ignore illegal locations */
			if (!in_bounds(ny, nx)) continue;

			if (!monst_can_enter_square(m_ptr, ny, nx, NULL)) continue;

			/* No teleporting into vaults and such */
			/* if (cave[ny][nx].info & (CAVE_ICKY)) continue; */

			/* This grid looks good */
			look = FALSE;

			/* Stop looking */
			break;
		}

		/* Increase the maximum distance */
		dis = dis * 2;

		/* Decrease the minimum distance */
		min = min / 2;
	}

	if (attempts < 1) return;

	/* Sound */
	sound(SOUND_TPOTHER);

	/* Update the new location */
	cave[ny][nx].m_idx = m_idx;
	last_teleportation_y = ny;
	last_teleportation_x = nx;

	/* Update the old location */
	cave[oy][ox].m_idx = 0;

	/* Move the monster */
	m_ptr->fy = ny;
	m_ptr->fx = nx;

	/* Update the monster (new location) */
	update_mon(m_idx, TRUE);

	/* Redraw the old grid */
	lite_spot(oy, ox);

	/* Redraw the new grid */
	lite_spot(ny, nx);

	/* Update monster light */
	if (has_flag(m_ptr, FLAG_HAS_LITE)) p_ptr->update |= (PU_MON_LITE);
}


/*
 * Teleport the player to a location up to "dis" grids away.
 *
 * If no such spaces are readily available, the distance may increase.
 * Try very hard to move the player at least a quarter that distance.
 */
/* It'd be better if this was made an argument ... */
bool teleport_player_bypass = FALSE;

void teleport_player(s32b dis)
{
	s32b d, i, j, min, ox, oy, x = 0, y = 0;
	s32b tries = 0;

	bool look = TRUE;

	if (intrinsic(RES_CONTINUUM) && (!teleport_player_bypass))
	{
		msg_print("The space-time continuum can't be disrupted.");
		return;
	}

	if (p_ptr->wild_mode) return;

	if (intrinsic(NO_TELE) && (!teleport_player_bypass))
	{
		msg_print("A mysterious force prevents you from teleporting!");
		return;
	}

	if (has_flag(dungeon_flags, FLAG_NO_TELEPORT) && (!teleport_player_bypass))
	{
		msg_print("No teleport on special levels...");
		return;
	}


	if (dis > 200) dis = 200;  /* To be on the safe side... */

	/* Minimum distance */
	min = dis / 2;

	/* Look until done */
	while (look)
	{
		tries++;

		/* Verify max distance */
		if (dis > 200) dis = 200;

		/* Try several locations */
		for (i = 0; i < 500; i++)
		{
			/* Pick a (possibly illegal) location */
			for (j = 0; j < 500; j++)
			{
				y = rand_spread(p_ptr->py, dis);
				x = rand_spread(p_ptr->px, dis);
				d = distance(p_ptr->py, p_ptr->px, y, x);
				if ((d >= min) && (d <= dis)) break;
			}

			/* Ignore illegal locations */
			if (!in_bounds(y, x)) continue;

			/* Require "naked" floor space */
			if (!cave_naked_bold(y, x)) continue;

			/* No teleporting into vaults and such */
			if (cave[y][x].info & (CAVE_ICKY)) continue;

			/* This grid looks good */
			look = FALSE;

			/* Stop looking */
			break;
		}

		/* Increase the maximum distance */
		dis = dis * 2;

		/* Decrease the minimum distance */
		min = min / 2;

		/* Stop after MAX_TRIES tries */
		if (tries > MAX_TRIES) return;
	}

	/* Sound */
	sound(SOUND_TELEPORT);

	/* Save the old location */
	oy = p_ptr->py;
	ox = p_ptr->px;

	/* Move the player */
	p_ptr->py = y;
	p_ptr->px = x;
	last_teleportation_y = y;
	last_teleportation_x = x;

	/* Redraw the old spot */
	lite_spot(oy, ox);

	/* Redraw the new spot */
	lite_spot(p_ptr->py, p_ptr->px);

	/* Check for new panel (redraw map) */
	verify_panel();

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_FLOW | PU_MON_LITE);

	/* Update the monsters */
	p_ptr->update |= (PU_DISTANCE);

	/* Redraw trap detection status */
	flag_bool(&p_ptr->redraw, FLAG_PR_DTRAP);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);

	/* Handle stuff XXX XXX XXX */
	handle_stuff();
}


/*
 * get a grid near the given location
 *
 * This function is slightly obsessive about correctness.
 */
void get_pos_player(s32b dis, s32b *ny, s32b *nx)
{
	s32b d, i, min, x = 0, y = 0;
	s32b tries = 0;

	bool look = TRUE;

	if (dis > 200) dis = 200;  /* To be on the safe side... */

	/* Minimum distance */
	min = dis / 2;

	/* Look until done */
	while (look)
	{
		tries++;

		/* Verify max distance */
		if (dis > 200) dis = 200;

		/* Try several locations */
		for (i = 0; i < 500; i++)
		{
			/* Pick a (possibly illegal) location */
			while (1)
			{
				y = rand_spread(p_ptr->py, dis);
				x = rand_spread(p_ptr->px, dis);
				d = distance(p_ptr->py, p_ptr->px, y, x);
				if ((d >= min) && (d <= dis)) break;
			}

			/* Ignore illegal locations */
			if (!in_bounds(y, x)) continue;

			/* Require "naked" floor space */
			if (!cave_naked_bold(y, x)) continue;

			/* No teleporting into vaults and such */
			if (cave[y][x].info & (CAVE_ICKY)) continue;

			/* This grid looks good */
			look = FALSE;

			/* Stop looking */
			break;
		}

		/* Increase the maximum distance */
		dis = dis * 2;

		/* Decrease the minimum distance */
		min = min / 2;

		/* Stop after MAX_TRIES tries */
		if (tries > MAX_TRIES) return;
	}

	*ny = y;
	*nx = x;
}

/*
 * Teleport a monster to a grid near the given location
 *
 * This function is slightly obsessive about correctness.
 */
void teleport_monster_to(s32b m_idx, s32b ny, s32b nx)
{
	s32b y, x, oy, ox, dis = 0, ctr = 0;
	monster_type *m_ptr = get_monster(m_idx);

	if (intrinsic(RES_CONTINUUM))
	{
		msg_print("The space-time continuum can't be disrupted.");
		return;
	}

	if (intrinsic(NO_TELE))
	{
		msg_print("A mysterious force prevents you from teleporting!");
		return;
	}

	/* Find a usable location */
	while (1)
	{
		/* Pick a nearby legal location */
		while (1)
		{
			y = rand_spread(ny, dis);
			x = rand_spread(nx, dis);
			if (in_bounds(y, x)) break;
		}

		/* Not on the player's grid */
		/* Accept "naked" floor grids */
		if (cave_naked_bold(y, x) && (y != p_ptr->py) && (x != p_ptr->px)) break;

		/* Occasionally advance the distance */
		if (++ctr > (4 * dis * dis + 4 * dis + 1))
		{
			ctr = 0;
			dis++;
		}
	}

	/* Sound */
	sound(SOUND_TPOTHER);

	/* Save the old position */
	oy = m_ptr->fy;
	ox = m_ptr->fx;
	cave[oy][ox].m_idx = 0;

	/* Move the monster */
	m_ptr->fy = y;
	m_ptr->fx = x;
	cave[y][x].m_idx = m_idx;
	last_teleportation_y = y;
	last_teleportation_x = x;

	/* Update the monster (new location) */
	update_mon(m_idx, TRUE);

	/* Redraw the old spot */
	lite_spot(oy, ox);

	/* Redraw the new spot */
	lite_spot(m_ptr->fy, m_ptr->fx);
}


/*
 * Teleport player to a grid near the given location
 *
 * This function is slightly obsessive about correctness.
 * This function allows teleporting into vaults (!)
 */
void teleport_player_to(s32b ny, s32b nx)
{
	s32b y, x, oy, ox, dis = 0, ctr = 0;

	if (intrinsic(RES_CONTINUUM))
	{
		msg_print("The space-time continuum can't be disrupted.");
		return;
	}

	if (intrinsic(NO_TELE))
	{
		msg_print("A mysterious force prevents you from teleporting!");
		return;
	}

	if (has_flag(dungeon_flags, FLAG_NO_TELEPORT))
	{
		msg_print("No teleport on special levels...");
		return;
	}

	/* Find a usable location */
	while (1)
	{
		/* Pick a nearby legal location */
		while (1)
		{
			y = rand_spread(ny, dis);
			x = rand_spread(nx, dis);
			if (in_bounds(y, x)) break;
		}

		/* Accept "naked" floor grids */
		if (cave_naked_bold2(y, x)) break;

		/* Occasionally advance the distance */
		if (++ctr > (4 * dis * dis + 4 * dis + 1))
		{
			ctr = 0;
			dis++;
		}
	}

	/* Sound */
	sound(SOUND_TELEPORT);

	/* Save the old location */
	oy = p_ptr->py;
	ox = p_ptr->px;

	/* Move the player */
	p_ptr->py = y;
	p_ptr->px = x;
	last_teleportation_y = y;
	last_teleportation_x = x;

	/* Redraw the old spot */
	lite_spot(oy, ox);

	/* Redraw the new spot */
	lite_spot(p_ptr->py, p_ptr->px);

	/* Check for new panel (redraw map) */
	verify_panel();

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_FLOW | PU_MON_LITE);

	/* Update the monsters */
	p_ptr->update |= (PU_DISTANCE);

	/* Redraw trap detection status */
	flag_bool(&p_ptr->redraw, FLAG_PR_DTRAP);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);

	/* Handle stuff XXX XXX XXX */
	handle_stuff();
}



/*
 * Teleport the player one level up or down (random when legal)
 */
void teleport_player_level(void)
{
	bool rise = FALSE;

	/* No effect in arena or quest */
	if (p_ptr->inside_arena || p_ptr->inside_quest || (dun_level == 0))
	{
		msg_print("There is no effect.");
		return;
	}
	if (has_flag(dungeon_flags, FLAG_NO_TELEPORT))
	{
		msg_print("No teleport on special levels...");
		return;
	}
	if (has_flag(dungeon_flags, FLAG_NO_EASY_MOVE))
	{
		msg_print("Some powerfull force prevents your from teleporting.");
		return;
	}

	if (intrinsic(RES_CONTINUUM))
	{
		msg_print("The space-time continuum can't be disrupted.");
		return;
	}

	if (intrinsic(NO_TELE))
	{
		msg_print("A mysterious force prevents you from teleporting!");
		return;
	}

	if (!is_quest(dun_level) &&
		(d_info[dungeon_type].mindepth == d_info[dungeon_type].maxdepth))
	{
		/* A non-quest dungeon with only one level?  Weird. */
		msg_print("You bob up and down for a few moments, then nothing.");
		return;
	}

	if (is_quest(dun_level) || (dun_level == d_info[dungeon_type].maxdepth))
		rise = TRUE;
	else if (dun_level == d_info[dungeon_type].mindepth)
		rise = FALSE;
	else if (rand_int(100) < 50)
		rise = TRUE;
	else
		rise = FALSE;

	if (rise)
	{
		msg_print("You rise up through the ceiling.");

		if (autosave_l)
		{
			is_autosave = TRUE;
			msg_print("Autosaving the game...");
			do_cmd_save_game();
			is_autosave = FALSE;
		}

		dun_level--;

		/* Leaving */
		p_ptr->leaving = TRUE;
	}
	else
	{
		msg_print("You sink through the floor.");

		if (autosave_l)
		{
			is_autosave = TRUE;
			msg_print("Autosaving the game...");
			do_cmd_save_game();
			is_autosave = FALSE;
		}

		dun_level++;

		/* Leaving */
		p_ptr->leaving = TRUE;
	}

	/* Sound */
	sound(SOUND_TPLEVEL);
}



/*
 * Recall the player to town or dungeon
 */
void recall_player(s32b d, s32b f)
{
#if 0
	if (!p_ptr->town_num)
	{
		/* TODO: Recall the player to the last visited town */
		msg_print("Nothing happens.");
		return;
	}
#endif

	if (dun_level && (max_dlv[dungeon_type] > dun_level) &&
					!p_ptr->inside_quest)
	{
		if (get_check("Reset recall depth? "))
			max_dlv[dungeon_type] = dun_level;

	}
	if (!p_ptr->word_recall)
	{
		p_ptr->word_recall = rand_int(d) + f;
		msg_print("The air about you becomes charged...");
	}
	else
	{
		p_ptr->word_recall = 0;
		msg_print("A tension leaves the air around you...");
	}
	flag_bool(&p_ptr->redraw, FLAG_PR_DEPTH);
}



/*
 * Return a color to use for the bolt/ball spells
 */
byte spell_color(s32b type)
{
	/* Hooks! */
	if (process_hooks_ret(HOOK_DAM_COLOR, "d", "(d,d)", type, streq(TOME_GRAPHICS, "16")))
	{
		return process_hooks_return[0].num;
	}

	/* Standard "color" */
	return (TERM_WHITE);
}


/*
 * Find the attr/char pair to use for a spell effect
 *
 * It is moving (or has moved) from (x,y) to (nx,ny).
 *
 * If the distance is not "one", we (may) return "*".
 */
static u16b bolt_pict(s32b y, s32b x, s32b ny, s32b nx, s32b typ)
{
	byte a;
	char c;

	/* No motion (*) */
	if ((ny == y) && (nx == x)) c = '*';

	/* Vertical (|) */
	else if (nx == x) c = '|';

	/* Horizontal (-) */
	else if (ny == y) c = '-';

	/* Diagonal (/) */
	else if ((ny - y) == (x - nx)) c = '/';

	/* Diagonal (\) */
	else if ((ny - y) == (nx - x)) c = '\\';

	/* Weird (*) */
	else c = '*';

	/* Basic spell color */
	a = spell_color(typ);

	/* Create pict */
	return (PICT(a, c));
}

/*
 * Decreases players hit points and sets death flag if necessary
 */
void take_hit(s32b damage, cptr hit_from)
{
	call_lua("take_hit", "(d,s)", "", damage, hit_from);
}

/*
 * Increases a stat by one level             -RAK-
 *
 * Note that this function (used by stat potions) now restores
 * the stat BEFORE increasing it.
 */
bool inc_stat(s32b stat)
{
	s32b value;

	/* Then augment the current/max stat */
	value = get_stat(stat, cur);

	/* Cannot go above a predefined limit */
	if (value < MAX_STAT_NATURAL)
	{
		value++;

		/* Save the new value */
		set_stat(stat, cur, value);

		/* Bring up the maximum too */
		if (value > get_stat(stat, max))
		{
			set_stat(stat, max, value);
		}

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);
		flag_bool(&p_ptr->redraw, FLAG_PR_STATS);

		/* Success */
		return (TRUE);
	}

	/* Nothing to gain */
	return (FALSE);
}



/*
 * Decreases a stat by an amount indended to vary from 0 to 100 percent.
 *
 * Amount could be a little higher in extreme cases to mangle very high
 * stats from massive assaults.  -CWS
 *
 * Note that "permanent" means that the *given* amount is permanent,
 * not that the new value becomes permanent.  This may not work exactly
 * as expected, due to "weirdness" in the algorithm, but in general,
 * if your stat is already drained, the "max" value will not drop all
 * the way down to the "cur" value.
 */
bool dec_stat(s32b stat, s32b amount, s32b mode)
{
	s32b cur, max, loss = 0, same, res = FALSE;


	/* Acquire current value */
	cur = get_stat(stat, cur);
	max = get_stat(stat, max);

	/* Note when the values are identical */
	same = (cur == max);

	/* Damage "current" value */
	if (cur > 0)
	{
		if (amount > 90) cur--;
		if (amount > 50) cur--;
		if (amount > 20) cur--;
		cur--;

		/* Prevent illegal values */
		if (cur < 0) cur = 0;

		/* Something happened */
		if (cur != get_stat(stat, cur)) res = TRUE;
	}

	/* Damage "max" value */
	if ((mode == STAT_DEC_PERMANENT) && (max > 0))
	{
		if (amount > 90) max--;
		if (amount > 50) max--;
		if (amount > 20) max--;
		max--;

		/* Hack -- keep it clean */
		if (same || (max < cur)) max = cur;

		/* Something happened */
		if (max != get_stat(stat, max)) res = TRUE;
	}

	/* Apply changes */
	if (res)
	{
		if (mode == STAT_DEC_TEMPORARY)
		{
			u16b dectime;

			/* a little crude, perhaps */
			dectime = rand_int(max_dlv[dungeon_type] * 50) + 50;

			/* Prevent overflow on the drain counter */
			if ((get_stat(stat, cnt) + dectime) <
			      get_stat(stat, cnt))
				return FALSE;

			/* Calculate loss */
			loss = get_stat(stat, cur) - cur;

			/* Combine with another temporary drain... */
			if (get_stat(stat, los) > 0)
			{
				add_stat(stat, cnt, dectime);
				add_stat(stat, los, loss);
			}
			else
			{
				set_stat(stat, cnt, dectime);
				set_stat(stat, los, loss);
			}
		}

		/* Actually set the stat to its new value. */
		set_stat(stat, cur, cur);
		set_stat(stat, max, max);

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);
		flag_bool(&p_ptr->redraw, FLAG_PR_STATS);
	}

	/* Done */
	return (res);
}


/*
 * Restore a stat.  Return TRUE only if this actually makes a difference.
 */
bool res_stat(s32b stat, bool full)
{
	/* Fully restore */
	if (full)
	{
		/* Restore if needed */
		if (get_stat(stat, cur) != get_stat(stat, max))
		{
			/* Restore */
			set_stat(stat, cur, get_stat(stat, max));

			/* Remove temporary drain */
			set_stat(stat, cnt, 0);
			set_stat(stat, los, 0);

			/* Recalculate bonuses */
			p_ptr->update |= (PU_BONUS);
			flag_bool(&p_ptr->redraw, FLAG_PR_STATS);

			/* Something happened */
			return (TRUE);
		}
	}

	/* Restore temporary drained stat */
	else
	{
		/* Restore if needed */
		if (get_stat(stat, los))
		{
			/* Restore */
			add_stat(stat, cur, get_stat(stat, los));

			/* Remove temporary drain */
			set_stat(stat, cnt, 0);
			set_stat(stat, los, 0);

			/* Recalculate bonuses */
			p_ptr->update |= (PU_BONUS);
			flag_bool(&p_ptr->redraw, FLAG_PR_STATS);

			/* Something happened */
			return (TRUE);
		}
	}

	/* Nothing to restore */
	return (FALSE);
}





void corrupt_player(void)
{
	s32b max1, cur1, max2, cur2, ii, jj;

	/* Pick a pair of stats */
	ii = rand_int(6);
	for (jj = ii; jj == ii; jj = rand_int(6)) /* loop */;

	max1 = get_stat(ii, max);
	cur1 = get_stat(ii, cur);
	max2 = get_stat(jj, max);
	cur2 = get_stat(jj, cur);

	set_stat(ii, max, max2);
	set_stat(ii, cur, cur2);
	set_stat(jj, max, max1);
	set_stat(jj, cur, cur1);

	p_ptr->update |= (PU_BONUS);
}


/*
 * Convert 2 couples of coordonates to a direction
 */
s32b yx_to_dir(s32b y2, s32b x2, s32b y1, s32b x1)
{
	s32b y = y2 - y1, x = x2 - x1;

	if ((y == 0) && (x == 1)) return 6;
	if ((y == 0) && (x == -1)) return 4;
	if ((y == -1) && (x == 0)) return 8;
	if ((y == 1) && (x == 0)) return 2;
	if ((y == -1) && (x == -1)) return 7;
	if ((y == -1) && (x == 1)) return 9;
	if ((y == 1) && (x == 1)) return 3;
	if ((y == 1) && (x == -1)) return 1;

	return 5;
}

/*
 * Give the opposate direction of the given one
 */
s32b invert_dir(s32b dir)
{
	if (dir == 4) return 6;
	if (dir == 6) return 4;
	if (dir == 8) return 2;
	if (dir == 2) return 8;
	if (dir == 7) return 3;
	if (dir == 9) return 1;
	if (dir == 1) return 9;
	if (dir == 3) return 7;
	return 5;
}


/*
 * Determine which way the mana path follow
 */
s32b get_mana_path_dir(s32b y, s32b x, s32b oy, s32b ox, s32b pdir, s32b mana)
{
	s32b dir[8] = {5, 5, 5, 5, 5, 5, 5, 5}, n = 0, i, r = 0;

	/* Check which case are allowed */
	if (cave[y - 1][x].mana == mana) dir[n++] = 8;
	if (cave[y + 1][x].mana == mana) dir[n++] = 2;
	if (cave[y][x - 1].mana == mana) dir[n++] = 4;
	if (cave[y][x + 1].mana == mana) dir[n++] = 6;

	/* If only 2 possibilities select the only good one */
	if (n == 2)
	{
		if (invert_dir(yx_to_dir(y, x, oy, ox)) != dir[0]) return dir[0];
		if (invert_dir(yx_to_dir(y, x, oy, ox)) != dir[1]) return dir[1];

		/* Should never happen */
		return 5;
	}


	/* Check if it's not your last place */
	for (i = 0; i < n; i++)
	{
		if ((oy == y + ddy[dir[i]]) && (ox == x + ddx[dir[i]]))
		{
			if (dir[i] == 8) dir[i] = 2;
			else if (dir[i] == 2) dir[i] = 8;
			else if (dir[i] == 6) dir[i] = 4;
			else if (dir[i] == 4) dir[i] = 6;
		}
	}

	/* Select the desired one if possible */
	for (i = 0; i < n; i++)
	{
		if ((dir[i] == pdir) &&
						(cave[y + ddy[dir[i]]][x + ddx[dir[i]]].mana == mana))
		{
			return dir[i];
		}
	}

	/* If not select a random one */
	if (n > 2)
	{
		byte nb = 200;

		while (nb)
		{
			nb--;

			r = rand_int(n);
			if ((dir[r] != 5) && (yx_to_dir(y, x, oy, ox) != dir[r])) break;
		}
		return dir[r];
	}
	/* If nothing is found return 5 */
	else return 5;
}


/*
 * Determine the path taken by a projection.
 *
 * The projection will always start from the grid (y1,x1), and will travel
 * towards the grid (y2,x2), touching one grid per unit of distance along
 * the major axis, and stopping when it enters the destination grid or a
 * wall grid, or has travelled the maximum legal distance of "range".
 *
 * Note that "distance" in this function (as in the "update_view()" code)
 * is defined as "MAX(dy,dx) + MIN(dy,dx)/2", which means that the player
 * actually has an "octagon of projection" not a "circle of projection".
 *
 * The path grids are saved into the grid array pointed to by "gp", and
 * there should be room for at least "range" grids in "gp".  Note that
 * due to the way in which distance is calculated, this function normally
 * uses fewer than "range" grids for the projection path, so the result
 * of this function should never be compared directly to "range".  Note
 * that the initial grid (y1,x1) is never saved into the grid array, not
 * even if the initial grid is also the final grid.  XXX XXX XXX
 *
 * The "flg" flags can be used to modify the behavior of this function.
 *
 * In particular, the "PROJECT_STOP" and "PROJECT_THRU" flags have the same
 * semantics as they do for the "project" function, namely, that the path
 * will stop as soon as it hits a monster, or that the path will continue
 * through the destination grid, respectively.
 *
 * The "PROJECT_JUMP" flag, which for the "project()" function means to
 * start at a special grid (which makes no sense in this function), means
 * that the path should be "angled" slightly if needed to avoid any wall
 * grids, allowing the player to "target" any grid which is in "view".
 * This flag is non-trivial and has not yet been implemented, but could
 * perhaps make use of the "vinfo" array (above).  XXX XXX XXX
 *
 * This function returns the number of grids (if any) in the path.  This
 * function will return zero if and only if (y1,x1) and (y2,x2) are equal.
 *
 * This algorithm is similar to, but slightly different from, the one used
 * by "update_view_los()", and very different from the one used by "los()".
 */
s32b project_path(u16b *gp, s32b range, s32b y1, s32b x1, s32b y2, s32b x2, s32b flg)
{
#if 0 /* Old code */
	s32b y, x, mana = 0, dir = 0;

	s32b n = 0;
	s32b k = 0;

	/* Absolute */
	s32b ay, ax;

	/* Offsets */
	s32b sy, sx;

	/* Fractions */
	s32b frac;

	/* Scale factors */
	s32b full, half;

	/* Slope */
	s32b m;


	/* No path necessary (or allowed) */
	if ((x1 == x2) && (y1 == y2)) return (0);

	/* Hack -- to make a bolt/beam/ball follow a mana path */
	if (flg & PROJECT_MANA_PATH)
	{
		s32b oy = y1, ox = x1, pdir = yx_to_dir(y2, x2, y1, x1);

		/* Get the mana path level to follow */
		mana = cave[y1][x1].mana;

		/* Start */
		dir = get_mana_path_dir(y1, x1, y1, x1, pdir, mana);
		y = y1 + ddy[dir];
		x = x1 + ddx[dir];

		/* Create the projection path */
		while (1)
		{
			/* Save grid */
			gp[n++] = GRID(y, x);

			/* Hack -- Check maximum range */
			if (n >= range + 10) return n;

			/* Always stop at non-initial wall grids */
			if ((n > 0) && (cave_wall_bold(y, x) && !(flg & PROJECT_WALL)))
				return n;

			/* Sometimes stop at non-initial monsters/players */
			if (flg & (PROJECT_STOP))
			{
				if ((n > 0) && (cave[y][x].m_idx != 0)) return n;
			}

			/* Get the new direction */
			dir = get_mana_path_dir(y, x, oy, ox, pdir, mana);
			if (dir == 5) return n;
			oy = y;
			ox = x;
			y += ddy[dir];
			x += ddx[dir];
		}
	}

	/* Analyze "dy" */
	if (y2 < y1)
	{
		ay = (y1 - y2);
		sy = -1;
	}
	else
	{
		ay = (y2 - y1);
		sy = 1;
	}

	/* Analyze "dx" */
	if (x2 < x1)
	{
		ax = (x1 - x2);
		sx = -1;
	}
	else
	{
		ax = (x2 - x1);
		sx = 1;
	}


	/* Number of "units" in one "half" grid */
	half = (ay * ax);

	/* Number of "units" in one "full" grid */
	full = half << 1;


	/* Vertical */
	if (ay > ax)
	{
		/* Start at tile edge */
		frac = ax * ax;

		/* Let m = ((dx/dy) * full) = (dx * dx * 2) = (frac * 2) */
		m = frac << 1;

		/* Start */
		y = y1 + sy;
		x = x1;

		/* Create the projection path */
		while (1)
		{
			/* Save grid */
			gp[n++] = GRID(y, x);

			/* Hack -- Check maximum range */
			if ((n + (k >> 1)) >= range) break;

			/* Sometimes stop at destination grid */
			if (!(flg & (PROJECT_THRU)))
			{
				if ((x == x2) && (y == y2)) break;
			}

			/* Always stop at non-initial wall grids */
			if ((n > 0) && (cave_wall_bold(y, x) && !(flg & PROJECT_WALL)))
				break;

			/* Sometimes stop at non-initial monsters/players */
			if (flg & (PROJECT_STOP))
			{
				if ((n > 0) && ((cave[y][x].m_idx != 0) || ((p_ptr->py == y) && (p_ptr->px == x)))) break;
			}

			/* Slant */
			if (m)
			{
				/* Advance (X) part 1 */
				frac += m;

				/* Horizontal change */
				if (frac >= half)
				{
					/* Advance (X) part 2 */
					x += sx;

					/* Advance (X) part 3 */
					frac -= full;

					/* Track distance */
					k++;
				}
			}

			/* Advance (Y) */
			y += sy;
		}
	}

	/* Horizontal */
	else if (ax > ay)
	{
		/* Start at tile edge */
		frac = ay * ay;

		/* Let m = ((dy/dx) * full) = (dy * dy * 2) = (frac * 2) */
		m = frac << 1;

		/* Start */
		y = y1;
		x = x1 + sx;

		/* Create the projection path */
		while (1)
		{
			/* Save grid */
			gp[n++] = GRID(y, x);

			/* Hack -- Check maximum range */
			if ((n + (k >> 1)) >= range) break;

			/* Sometimes stop at destination grid */
			if (!(flg & (PROJECT_THRU)))
			{
				if ((x == x2) && (y == y2)) break;
			}

			/* Always stop at non-initial wall grids */
			if ((n > 0) && (!cave_sight_bold(y, x) ||
							cave_wall_bold(y, x)) && !(flg & PROJECT_WALL))
				break;

			/* Sometimes stop at non-initial monsters/players */
			if (flg & (PROJECT_STOP))
			{
				if ((n > 0) && ((cave[y][x].m_idx != 0) || ((p_ptr->py == y) && (p_ptr->px == x)))) break;
			}

			/* Slant */
			if (m)
			{
				/* Advance (Y) part 1 */
				frac += m;

				/* Vertical change */
				if (frac >= half)
				{
					/* Advance (Y) part 2 */
					y += sy;

					/* Advance (Y) part 3 */
					frac -= full;

					/* Track distance */
					k++;
				}
			}

			/* Advance (X) */
			x += sx;
		}
	}

	/* Diagonal */
	else
	{
		/* Start */
		y = y1 + sy;
		x = x1 + sx;

		/* Create the projection path */
		while (1)
		{
			/* Save grid */
			gp[n++] = GRID(y, x);

			/* Hack -- Check maximum range */
			if ((n + (n >> 1)) >= range) break;

			/* Sometimes stop at destination grid */
			if (!(flg & (PROJECT_THRU)))
			{
				if ((x == x2) && (y == y2)) break;
			}

			/* Always stop at non-initial wall grids */
			if ((n > 0) && (!cave_sight_bold(y, x) ||
							cave_wall_bold(y, x)) && !(flg & PROJECT_WALL))
				break;

			/* Sometimes stop at non-initial monsters/players */
			if (flg & (PROJECT_STOP))
			{
				if ((n > 0) && ((cave[y][x].m_idx != 0) || ((p_ptr->py == y) && (p_ptr->px == x)))) break;
			}

			/* Advance (Y) */
			y += sy;

			/* Advance (X) */
			x += sx;
		}
	}


	/* Length */
	return (n);
#else /* New code, uses mmove2 for a general uniformity */
	s32b y, x;
	s32b n = 0;

	/* No path necessary (or allowed) */
	if ((x1 == x2) && (y1 == y2)) return (0);

	/* Start at the initial location and move once */
	y = y1;
	x = x1;
	mmove2(&y, &x, y1, x1, y2, x2);

	for (n = 0; n <= range; )
	{
		/* Save grid */
		gp[n++] = GRID(y, x);

		/* Sometimes stop at destination grid */
		if (!(flg & (PROJECT_THRU)))
		{
			if ((x == x2) && (y == y2)) break;
		}

		/* Always stop at non-initial wall grids */
		if ((cave_wall_bold(y, x) && !(flg & PROJECT_WALL))) break;

		/* Sometimes stop at non-initial monsters/players */
		if (flg & (PROJECT_STOP))
		{
			if ((cave[y][x].m_idx != 0) || ((p_ptr->py == y) && (p_ptr->px == x))) break;
		}

		/* Not too far away */
		if (distance(y1, x1, y, x) >= range) break;

		/* Calculate the new location */
		mmove2(&y, &x, y1, x1, y2, x2);
	}

	return n;
#endif
}



/*
 * Mega-Hack -- track "affected" monsters (see "project()" comments)
 */
static s32b project_m_n;
static s32b project_m_x;
static s32b project_m_y;



/*
 * We are called from "project()" to "damage" terrain features
 *
 * We are called both for "beam" effects and "ball" effects.
 *
 * The "r" parameter is the "distance from ground zero".
 *
 * Note that we determine if the player can "see" anything that happens
 * by taking into account: blindness, line-of-sight, and illumination.
 *
 * We return "TRUE" if the effect of the projection is "obvious".
 *
 * XXX XXX XXX We also "see" grids which are "memorized", probably a hack
 *
 * XXX XXX XXX Perhaps we should affect doors?
 */
bool project_f(s32b who, s32b r, s32b y, s32b x, s32b dam, s32b typ, s32b flg,
			   s32b dir)
{
	bool obvious = FALSE;

	bool seen;

	s32b faction = FACTION_DUNGEON;

	s32b oldtop;

	/* XXX XXX XXX */
	who = who ? who : 0;

	if(who == 0) faction = FACTION_PLAYER;
	else if(who > 0)
	{
		monster_type *t = get_monster(who);
		faction = t->faction;
	}

	/* Reduce damage by distance */
	if (!(flg & PROJECT_NO_RADIUS_DAM)) dam = (dam + r) / (r + 1);

	/* Remember if the grid is with the LoS of player */
	seen = player_can_see_bold(y, x);

	/*
	 * Build the calling code to the lua stuff
	 * do it with the lua API directly to avoid speed overheads
	 */
	oldtop = lua_gettop(L);

	/* Grab the function to call */
	lua_getglobal(L, "dam");

	lua_pushstring(L, "call");
	lua_gettable(L, -2);

	/* 2 firts args, type and target */
	lua_pushnumber(L, typ);
	lua_pushstring(L, "grid");

	/* Push the args in a table */
	lua_newtable(L);

	lua_pushstring(L, "dam");
	lua_pushnumber(L, dam);
	lua_settable(L, -3);

	lua_pushstring(L, "who");
	lua_pushnumber(L, who);
	lua_settable(L, -3);

	lua_pushstring(L, "rad");
	lua_pushnumber(L, r);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, y);
	lua_settable(L, -3);

	lua_pushstring(L, "x");
	lua_pushnumber(L, x);
	lua_settable(L, -3);

	lua_pushstring(L, "dir");
	lua_pushnumber(L, dir);
	lua_settable(L, -3);

	/* Call the function */
	if (lua_call(L, 3, 1))
	{
		cmsg_format(TERM_VIOLET, "ERROR in lua_call while calling 'project_f'.");
		lua_settop(L, oldtop);
		return(FALSE);
	}

	/* Analyze the resulting table */
	lua_pushstring(L, "dam");
	lua_gettable(L, -2);
	dam = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "obvious");
	lua_gettable(L, -2);
	obvious = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_settop(L, oldtop);

	/* Return "Anything seen?" */
	return (obvious);
}


/*
 * We are called from "project()" to "damage" objects
 *
 * We are called both for "beam" effects and "ball" effects.
 *
 * Perhaps we should only SOMETIMES damage things on the ground.
 *
 * The "r" parameter is the "distance from ground zero".
 *
 * Note that we determine if the player can "see" anything that happens
 * by taking into account: blindness, line-of-sight, and illumination.
 *
 * XXX XXX XXX We also "see" grids which are "memorized", probably a hack
 *
 * We return "TRUE" if the effect of the projection is "obvious".
 */
bool project_o(s32b who, s32b r, s32b y, s32b x, s32b dam, s32b typ, s32b flg,
			   s32b dir)
{
	cave_type *c_ptr = &cave[y][x];

	bool obvious = FALSE;

	char o_name[80];

	s32b o_sval = 0;

	vector_type *to_delete = vector_new();

	u32b faction = FACTION_DUNGEON;

	if (!who) faction = FACTION_PLAYER;
	else if(who > 0)
	{
		monster_type *t = get_monster(who);
		faction = t->faction;
	}

	/* XXX XXX XXX */
	who = who ? who : 0;

	/* Reduce damage by distance */
	if (!(flg & PROJECT_NO_RADIUS_DAM)) dam = (dam + r) / (r + 1);

	/* Scan all objects in the grid */
	for_inventory_slot(&c_ptr->inventory, o_ptr);
	{
		bool is_art = FALSE;
		bool ignore = FALSE;
		bool plural = FALSE;
		bool do_kill = FALSE;
		s32b oldtop;

		cptr note_kill = NULL;

		/* Get the "plural"-ness */
		if (o_ptr->number > 1) plural = TRUE;

		/* Check for artifact */
		if ((artifact_p(o_ptr) || o_ptr->art_name))
			is_art = TRUE;

		/* Does the object ignore this type of damage? */
		if (has_flag(o_ptr, FLAG_IGNORE))
		{
			flags_type *ignore_flag =
				flag_get_flags(&o_ptr->flags, FLAG_IGNORE);

			if (flag_exists(ignore_flag, dam))
				ignore = TRUE;
		}

		/*
		 * Build the calling code to the lua stuff
		 * do it with the lua API directly to avoid speed overheads
		 */
		oldtop = lua_gettop(L);

		/* Grab the function to call */
		lua_getglobal(L, "dam");

		lua_pushstring(L, "call");
		lua_gettable(L, -2);

		/* 2 firts args, type and target */
		lua_pushnumber(L, typ);
		lua_pushstring(L, "object");

		/* Push the args in a table */
		lua_newtable(L);

		lua_pushstring(L, "dam");
		lua_pushnumber(L, dam);
		lua_settable(L, -3);

		lua_pushstring(L, "who");
		lua_pushnumber(L, who);
		lua_settable(L, -3);

		lua_pushstring(L, "rad");
		lua_pushnumber(L, r);
		lua_settable(L, -3);

		lua_pushstring(L, "y");
		lua_pushnumber(L, y);
		lua_settable(L, -3);

		lua_pushstring(L, "x");
		lua_pushnumber(L, x);
		lua_settable(L, -3);

		lua_pushstring(L, "dir");
		lua_pushnumber(L, dir);
		lua_settable(L, -3);

		lua_pushstring(L, "item_nb");
		lua_pushnumber(L, item_nb);
		lua_settable(L, -3);

		lua_pushstring(L, "o_ptr");
		tolua_pushusertype(L, (void*)o_ptr, tolua_tag(L, "object_type"));
		lua_settable(L, -3);

		/* Call the function */
		if (lua_call(L, 3, 1))
		{
			cmsg_format(TERM_VIOLET, "ERROR in lua_call while calling 'project_o'.");
			lua_settop(L, oldtop);
			return(FALSE);
		}

		/* Analyze the resulting table */
		lua_pushstring(L, "dam");
		lua_gettable(L, -2);
		dam = lua_tonumber(L, -1); lua_pop(L, 1);

		lua_pushstring(L, "obvious");
		lua_gettable(L, -2);
		obvious = lua_tonumber(L, -1); lua_pop(L, 1);

		lua_pushstring(L, "kill");
		lua_gettable(L, -2);
		do_kill = lua_tonumber(L, -1); lua_pop(L, 1);

		lua_pushstring(L, "msg_die");
		lua_gettable(L, -2);
		note_kill = lua_tostring(L, -1); lua_pop(L, 1);

		lua_settop(L, oldtop);

		/* Attempt to destroy the object */
		if (do_kill)
		{
			/* Effect "observed" */
			if (o_ptr->marked)
			{
				obvious = TRUE;
				object_desc(o_name, o_ptr, FALSE, 0);
			}

			/* Artifacts, and other objects, get to resist */
			if (is_art || ignore)
			{
				/* Observe the resist */
				if (o_ptr->marked)
				{
					msg_format("The %s %s unaffected!",
							   o_name, (plural ? "are" : "is"));
				}
			}

			/* Kill it */
			else
			{
				/* Describe if needed */
				if (o_ptr->marked && note_kill)
				{
					msg_format("The %s%s", o_name, note_kill);
				}

				o_sval = o_ptr->sval;

				/* Delete the object */
				vector_push(to_delete, item_nb);

				/* Redraw */
				lite_spot(y, x);
			}
		}
	}
	end_inventory_slot();

	/*
	 * Now delete all marked items, in FILO(First In Last Out) way to ensure
	 * integrity checking
	 */
	while (!vector_is_empty(to_delete))
	{
		s32b idx = vector_pop(to_delete);
		s32b item;

		/* Make an "item" wrapper around it be able to use standart delete protocols */
		call_lua("generate_item_floor_index", "(d,d,d)", "d", y, x, idx, &item);

		/* Now delete it */
		item_increase(item, -255);
		item_optimize(item);
	}

	vector_free(to_delete);

	/* Return "Anything seen?" */
	return (obvious);
}

/* Can the monster be hurt ? */
bool hurt_monster(monster_type *m_ptr)
{
	if (m_ptr->faction == FACTION_PLAYER) return FALSE;
	else return TRUE;
}

s32b check_flag(monster_type *m_ptr, s32b flag, s32b cat, s32b obvious)
{
        s32b r = has_flag(m_ptr, flag);
        if (r && obvious) monstmem_add(cat, flag, m_ptr, 1);
        return r;
}

s32b resist_stack(s32b a, s32b b)
{
        return 100 - (100-a)*(100-b)/100;
}

void apply_res(s32b *dam, s32b *res, s32b res2)
{
	s32b r = res2 > 100 ? 100 : res2;
	*res = resist_stack(*res,r);
	*dam = (*dam)*(100-r)/100;
}

/*
 * Helper function for "project()" below.
 *
 * Handle a beam/bolt/ball causing damage to a monster.
 *
 * This routine takes a "source monster" (by index) which is mostly used to
 * determine if the player is causing the damage, and a "radius" (see below),
 * which is used to decrease the power of explosions with distance, and a
 * location, via integers which are modified by certain types of attacks
 * (polymorph and teleport being the obvious ones), a default damage, which
 * is modified as needed based on various properties, and finally a "damage
 * type" (see below).
 *
 * Note that this routine can handle "no damage" attacks (like teleport) by
 * taking a "zero" damage, and can even take "parameters" to attacks (like
 * confuse) by accepting a "damage", using it to calculate the effect, and
 * then setting the damage to zero.  Note that the "damage" parameter is
 * divided by the radius, so monsters not at the "epicenter" will not take
 * as much damage (or whatever)...
 *
 * Note that "polymorph" is dangerous, since a failure in "place_monster()"'
 * may result in a dereference of an invalid pointer.  XXX XXX XXX
 *
 * Various messages are produced, and damage is applied.
 *
 * Just "casting" a substance (i.e. plasma) does not make you immune, you must
 * actually be "made" of that substance, or "breathe" big balls of it.
 *
 * We assume that "Plasma" monsters, and "Plasma" breathers, are immune
 * to plasma.
 *
 * We assume "Nether" is an evil, necromantic force, so it doesn't hurt undead,
 * and hurts evil less.  If can breath nether, then it resists it as well.
 *
 * Damage reductions use the following formulas:
 *   Note that "dam = dam * 6 / (randint(6) + 6);"
 *     gives avg damage of .655, ranging from .858 to .500
 *   Note that "dam = dam * 5 / (randint(6) + 6);"
 *     gives avg damage of .544, ranging from .714 to .417
 *   Note that "dam = dam * 4 / (randint(6) + 6);"
 *     gives avg damage of .444, ranging from .556 to .333
 *   Note that "dam = dam * 3 / (randint(6) + 6);"
 *     gives avg damage of .327, ranging from .427 to .250
 *   Note that "dam = dam * 2 / (randint(6) + 6);"
 *     gives something simple.
 *
 * In this function, "result" messages are postponed until the end, where
 * the "note" string is appended to the monster name, if not NULL.  So,
 * to make a spell have "no effect" just set "note" to NULL.  You should
 * also set "notice" to FALSE, or the player will learn what the spell does.
 *
 * We attempt to return "TRUE" if the player saw anything "useful" happen.
 */
bool project_m(s32b who, s32b r, s32b y, s32b x, s32b dam, s32b typ, s32b flags,
			   s32b dir)
{
	s32b tmp;

	cave_type *c_ptr = &cave[y][x];

	monster_type *m_ptr;
	monster_race *r_ptr;

	/* Is the monster "seen"? */
	bool seen;

	/* Were the effects "obvious" (if seen)? */
	bool obvious = FALSE;

	/* Were the effects "irrelevant"? */
	bool skipped = FALSE;


	/* Move setting */
	s32b x1 = 0;
	s32b y1 = 0;
	s32b a = 0;
	s32b b = 0;
	s32b do_move = 0;

	/* Polymorph setting (true or FALSE) */
	s32b do_poly = 0;

	/* Teleport setting (max distance) */
	s32b do_dist = 0;

	/* Confusion setting (amount to confuse) */
	s32b do_conf = 0;

	/* Stunning setting (amount to stun) */
	s32b do_stun = 0;

	/* Bleeding amount */
	s32b do_cut = 0;

	/* Poison amount */
	s32b do_pois = 0;

	/* Sleep amount (amount to sleep) */
	s32b do_sleep = 0;

	/* Fear amount (amount to fear) */
	s32b do_fear = 0;

	/* Hold the monster name */
	char m_name[80];

	/* Assume no note */
	cptr note = NULL;

	/* Assume no note */
	cptr note_heal = NULL;

	/* Assume a default death */
	cptr note_dies = " dies.";

	/* Keep track of total resistance */
	s32b get_angry = 100;

	s32b oldtop;

#if 0

	/* Walls protect monsters */
	/* (No, they don't)  */
	if (!cave_floor_bold(y, x)) return (FALSE);

#endif

	/* Nobody here */
	if (!c_ptr->m_idx) return (FALSE);

	m_ptr = get_monster(c_ptr->m_idx);
	r_ptr = race_inf(m_ptr);

	/* Never affect projector */
	if (who && (c_ptr->m_idx == who) && (!(flags & PROJECT_CASTER))) return (FALSE);

	/*
	 * Don't affect already dead monsters
	 * Prevents problems with chain reactions of exploding monsters
	 */
	if (m_ptr->hp < 0) return (FALSE);

	/* Remember if the monster is within player's line of sight */
	seen = m_ptr->ml && ((who != WHO_FLOOR) && (who != WHO_DUNGEON));

	/* Reduce damage by distance */
	if (!(flags & PROJECT_NO_RADIUS_DAM)) dam = (dam + r) / (r + 1);

	/* Get the monster name (BEFORE polymorphing) */
	monster_desc(m_name, m_ptr, 0);

	/* Some monsters have special dying messages. */
	if (has_flag(m_ptr, FLAG_NOTE_DIES))
	{
		/* Special note at death */
		note_dies = flag_get_string(&m_ptr->flags, FLAG_NOTE_DIES);
	}

	/*
	 * Build the calling code to the lua stuff
	 * do it with the lua API directly to avoid speed overheads
	 */
	oldtop = lua_gettop(L);

	/* Grab the function to call */
	lua_getglobal(L, "dam");

	lua_pushstring(L, "call");
	lua_gettable(L, -2);

	/* 2 firts args, type and target */
	lua_pushnumber(L, typ);
	lua_pushstring(L, "monster");

	/* Push the args in a table */
	lua_newtable(L);

	lua_pushstring(L, "dam");
	lua_pushnumber(L, dam);
	lua_settable(L, -3);

	lua_pushstring(L, "who");
	lua_pushnumber(L, who);
	lua_settable(L, -3);

	lua_pushstring(L, "rad");
	lua_pushnumber(L, r);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
 	lua_pushnumber(L, y);
	lua_settable(L, -3);

	lua_pushstring(L, "x");
	lua_pushnumber(L, x);
	lua_settable(L, -3);

	lua_pushstring(L, "dir");
	lua_pushnumber(L, dir);
	lua_settable(L, -3);

	lua_pushstring(L, "m_idx");
	lua_pushnumber(L, c_ptr->m_idx);
	lua_settable(L, -3);

	lua_pushstring(L, "m_ptr");
	tolua_pushusertype(L, (void*)m_ptr, tolua_tag(L, "monster_type"));
	lua_settable(L, -3);

	/* Call the function */
	if (lua_call(L, 3, 1))
	{
		cmsg_format(TERM_VIOLET, "ERROR in lua_call while calling 'project_m'.");
		lua_settop(L, oldtop);
		return(FALSE);
	}

	/* Analyze the resulting table */
	lua_pushstring(L, "dam");
	lua_gettable(L, -2);
	dam = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "dir");
	lua_gettable(L, -2);
	dir = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "obvious");
	lua_gettable(L, -2);
	obvious = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "sleep");
	lua_gettable(L, -2);
	do_sleep = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "stun");
	lua_gettable(L, -2);
	do_stun = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "fear");
	lua_gettable(L, -2);
	do_fear = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "conf");
	lua_gettable(L, -2);
	do_conf = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "dist");
	lua_gettable(L, -2);
	do_dist = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "pois");
	lua_gettable(L, -2);
	do_pois = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "cut");
	lua_gettable(L, -2);
	do_cut = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "poly");
	lua_gettable(L, -2);
	do_poly = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "stun");
	lua_gettable(L, -2);
	do_stun = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "move");
	lua_gettable(L, -2);
	do_move = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "angry");
	lua_gettable(L, -2);
	get_angry = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "msg");
	lua_gettable(L, -2);
	note = lua_tostring(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "msg_die");
	lua_gettable(L, -2);
	note_dies = lua_tostring(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "msg_heal");
	lua_gettable(L, -2);
	note_heal = lua_tostring(L, -1); lua_pop(L, 1);

	lua_settop(L, oldtop);

	/* Anger your good friend ? that's bad! */
	if (!who && (m_ptr->faction != FACTION_PLAYER) && get_angry)
	{
		s32b old_friendliness = get_faction_friendliness(FACTION_PLAYER, m_ptr->faction);
		s32b new_friendliness = old_friendliness + get_angry;

		set_faction_friendliness(FACTION_PLAYER, m_ptr->faction, new_friendliness);

		if ((old_friendliness >= 0) && (new_friendliness < 0))
			msg_format("%^s gets angry!", m_name);
		else if ((old_friendliness < 0) && (new_friendliness >= 0))
			msg_format("%^s gets friendlier!", m_name);
	}

	/* Absolutely no effect */
	if (skipped) return (FALSE);

	/* "Unique" monsters cannot be polymorphed */
	if (has_flag(m_ptr, FLAG_UNIQUE)) do_poly = FALSE;

	/*
	 * "Quest" monsters cannot be polymorphed
	 */
	if (m_ptr->mflag & MFLAG_QUEST)
		do_poly = FALSE;

	/*
	 * "Quest" monsters can only be "killed" by the player
	 */
	if (m_ptr->mflag & MFLAG_QUEST)
	{
		if ((who > 0) && (dam > m_ptr->hp)) dam = m_ptr->hp;
	}

	if (do_pois && (!((has_flag(m_ptr, FLAG_IM_POIS)))) && hurt_monster(m_ptr))
	{
		if (m_ptr->poisoned) note = " is more poisoned.";
		else note = " is poisoned.";
		m_ptr->poisoned += do_pois;
	}

	if (do_cut && hurt_monster(m_ptr))
	{
		if (m_ptr->bleeding) note = " bleeds more strongly.";
		else note = " starts bleeding.";
		m_ptr->bleeding += do_cut;
	}

	/* Check for death */
	if ((dam > m_ptr->hp) && hurt_monster(m_ptr))
	{
		/* Extract method of death */
		note = note_dies;
	}

	/* Mega-Hack -- Handle "polymorph" -- monsters get a saving throw */
	else if (do_poly && cave_floor_bold(y, x) && (randint(90) > m_ptr->level))
	{
		/* Default -- assume no polymorph */
		note = " is unaffected!";

		/* Handle polymorph */
		if (do_poly_monster(y, x))
		{
			/* Obvious */
			if (seen) obvious = TRUE;

			/* Monster polymorphs */
			note = " changes!";

			/* Turn off the damage */
			dam = 0;

			/* Hack -- Get new monster */
			m_ptr = get_monster(c_ptr->m_idx);

			/* Hack -- Get new race */
			r_ptr = race_inf(m_ptr);
		}
	}

	/* Handle moving the monster.
	 *
	 * Note: This is a effect of force, but only when used
	 * by the player. (For the moment). The usual stun effect
	 * is not applied.
	 */
	else if (do_move && hurt_monster(m_ptr))
	{
		s32b back = 0;

		/* Obvious */
		if (seen) obvious = TRUE;

		back = 0;  /* Default of no movement */

		if (dir == 5)
		{
			a = rand_int(2) ? 1 : -1;
			b = rand_int(2) ? 1 : -1;
		}
		else
		{
			a = ddx[dir];
			b = ddy[dir];
		}

		/* How far can we push the monster? */
		for (do_move = 1; do_move < 3; do_move++)
		{
			/* Get monster coords */
			/* And offset position */
			y1 = m_ptr->fy + (b * do_move);
			x1 = m_ptr->fx + (a * do_move);

			if (!in_bounds(y1, x1)) continue;

			if (!monst_can_enter_square(m_ptr, y1, x1, NULL)) continue;

			/* amount moved */
			back = do_move;
		}

		/* Move the monster */
		if (back)
		{
			y1 = m_ptr->fy + (b * back);
			x1 = m_ptr->fx + (a * back);
			monster_swap(m_ptr->fy, m_ptr->fx, y1, x1);

			if (back == 2)
			{
				note = " is knocked back!";
			}
			if (back == 1)
			{
				note = " is knocked back and crushed!";

				/* was kept from being pushed all the way, do extra dam */
				dam = dam * 13 / 10;
			}

			/* Get new position */
			y = y1;
			x = x1;

			/* Hack -- get new grid */
			c_ptr = &cave[y][x];
		}
		else /* could not move the monster */
		{
			note = " is severely crushed!";

			/* Do extra damage (1/3)*/
			dam = dam * 15 / 10;
		}
	}

	/* Handle "teleport" */
	else if (do_dist)
	{
		/* Obvious */
		if (seen) obvious = TRUE;

		/* Message */
		note = " disappears!";

		/* Teleport */
		teleport_away(c_ptr->m_idx, do_dist);

		/* Hack -- get new location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Hack -- get new grid */
		c_ptr = &cave[y][x];
	}

	/* Sound and Impact breathers never stun */
	else if (do_stun &&
					hurt_monster(m_ptr))
	{
		/* Obvious */
		if (seen) obvious = TRUE;

		/* Get confused */
		if (m_ptr->stunned)
		{
			note = " is more dazed.";
			tmp = m_ptr->stunned + (do_stun / 2);
		}
		else
		{
			note = " is dazed.";
			tmp = do_stun;
		}

		/* Apply stun */
		m_ptr->stunned = (tmp < 200) ? tmp : 200;
	}

	/* Confusion and Chaos breathers (and sleepers) never confuse */
	else if (do_conf &&
					!(has_flag(m_ptr, FLAG_NO_CONF)) &&
					hurt_monster(m_ptr))
	{
		/* Obvious */
		if (seen) obvious = TRUE;

		/* Already partially confused */
		if (m_ptr->confused)
		{
			note = " looks more confused.";
			tmp = m_ptr->confused + (do_conf / 2);
		}

		/* Was not confused */
		else
		{
			note = " looks confused.";
			tmp = do_conf;
		}

		/* Apply confusion */
		m_ptr->confused = (tmp < 200) ? tmp : 200;
	}


	/* Fear */
	if (do_fear && hurt_monster(m_ptr))
	{
		/* Increase fear */
		tmp = m_ptr->monfear + do_fear;

		/* Set fear */
		m_ptr->monfear = (tmp < 200) ? tmp : 200;
	}


	/* If the damage turned into healing due to extra high "resistance" */
	if (dam < 0)
	{
		/* Wake up */
		m_ptr->csleep = 0;

		/* Heal */
		m_ptr->hp += -dam;

		/* No overflow */
		if (m_ptr->hp > m_ptr->maxhp) m_ptr->hp = m_ptr->maxhp;

		/* Redraw (later) if needed */
		if (health_who == c_ptr->m_idx) flag_bool(&p_ptr->redraw, FLAG_PR_HEALTH);

		/* Message */
		if (!(flags & PROJECT_SILENT))
		{
			if (seen) msg_format("%^s%s", m_name, (note_heal == NULL) ? " looks healthier." : note_heal);
		}
	}
	else
	{
		/* If another monster did the damage, hurt the monster by hand */
		if (who != WHO_PLAYER)
		{
			bool fear = FALSE;

			/* Dead monster */
			if (mon_take_hit_mon(who, c_ptr->m_idx, dam, &fear, note_dies))
			{}

			/* Damaged monster */
			else
			{
				if (!(flags & PROJECT_SILENT))
				{
					/* Give detailed messages if visible or destroyed */
					if (note && seen) msg_format("%^s%s", m_name, note);
				}

				/* Hack -- handle sleep */
				if (do_sleep) m_ptr->csleep = do_sleep;
			}
		}
		/* If the player did it, give him experience, check fear */
		else if (hurt_monster(m_ptr))
		{
			bool fear = FALSE;

			/* Hurt the monster, check for fear and death */
			if (mon_take_hit(c_ptr->m_idx, dam, &fear, note_dies))
			{
				/* Dead monster */
			}

			/* Damaged monster */
			else
			{
				if (!(flags & PROJECT_SILENT))
				{
					/* Give detailed messages if visible or destroyed */
					if (note && seen) msg_format("%^s%s", m_name, note);
				}

				/* Take note */
				if ((fear || do_fear) && (m_ptr->ml))
				{
					/* Sound */
					sound(SOUND_FLEE);

					/* Message */
					msg_format("%^s flees in terror!", m_name);
				}

				/* Hack -- handle sleep */
				if (do_sleep) m_ptr->csleep = do_sleep;
			}
		}
	}

	/* XXX XXX XXX Verify this code */

	/* Update the monster */
	update_mon(c_ptr->m_idx, FALSE);

	/* Redraw the monster grid */
	lite_spot(y, x);


	/* Update monster recall window */
	if (monster_race_idx == m_ptr->r_idx)
	{
		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_MONSTER);
	}


	/* Track it */
	project_m_n++;
	project_m_x = x;
	project_m_y = y;


	/* Return "Anything seen?" */
	return (obvious);
}


/*
 * Helper function for "project()" below.
 *
 * Handle a beam/bolt/ball causing damage to the player.
 *
 * This routine takes a "source monster" (by index), a "distance", a default
 * "damage", and a "damage type".  See "project_m()" above.
 *
 * If "rad" is non-zero, then the blast was centered elsewhere, and the damage
 * is reduced (see "project_m()" above).  This can happen if a monster breathes
 * at the player and hits a wall instead.
 *
 * NOTE (Zangband): 'Bolt' attacks can be reflected back, so we need to know
 * if this is actually a ball or a bolt spell
 *
 *
 * We return "TRUE" if any "obvious" effects were observed.  XXX XXX Actually,
 * we just assume that the effects were obvious, for historical reasons.
 */
bool project_p(s32b who, s32b r, s32b y, s32b x, s32b dam, s32b typ, s32b a_rad,
			   s32b flags, s32b dir)
{
	s32b do_move = 0, a = 0, b = 0, x1 = 0, y1 = 0;

	/* Hack -- assume obvious */
	bool obvious = TRUE;

	/* Player blind-ness */
	bool blind = (intrinsic(BLIND) ? TRUE : FALSE);

	/* Player needs a "description" (he is blind) */
	bool fuzzy = FALSE;

	/* Source monster */
	monster_type *m_ptr = NULL;

	/* Monster name (for attacks) */
	char m_name[80];

	/* Monster name (for damage) */
	char killer[80];

	/* Old lua stack top */
	s32b oldtop;

	/* Fuzzy message */
	cptr fuzzy_msg = NULL;

	/* General message */
	cptr note = NULL;

	/* Player is not here */
	if ((x != p_ptr->px) || (y != p_ptr->py)) return (FALSE);

	if (process_hooks_ret(HOOK_PROJECT_P_PRE_LUA, "(d,d,d)",
						  "d,d,d,d,d,d,d,d,d",
						  who, r, y, x, dam, typ, a_rad, flags, dir))
	{
		dam   = process_hooks_return[0].num;
		typ   = process_hooks_return[1].num;
		flags = process_hooks_return[2].num;
	}

	/* Player cannot hurt himself */
	if ((!who) && (!(flags & PROJECT_CASTER))) return (FALSE);

	/*
	 * Effects caused by the dungeon of floor aren't bolts, so they
	 * don't relfect.  The player damaging him or herslef aren't
	 * bolts, so they don't reflect either.  Effects caused by divine
	 * wrath don't reflect because, well, they're caused by a *GOD*.
	 * Otherwise, the REFLECT flag is the perecent chance of
	 * reflecting.
	 */
	if ((a_rad == 0) && !(flags & PROJECT_NO_REFLECT) &&
		(who != WHO_FLOOR) && (who != WHO_DUNGEON) &&
		(who != WHO_PLAYER) && magik(intrinsic(REFLECT)))
	{
		s32b t_y, t_x;

		if (blind) msg_print("Something bounces!");
		else msg_print("The attack bounces!");

		if (who < 0)
		{
			/*
			 * Bolt didn't come from a monster, so just reverse the
			 * direction it came from.
			 */

			if ((dir == 0) || (dir == 5))
				/*
				 * If the bolt isn't coming from any direction in
				 * particular, then pick a random direction.
				 */
				dir = ddd[rand_int(8)];

			t_y = p_ptr->py + (ddy[dir] * -99);
			t_x = p_ptr->px + (ddx[dir] * -99);
		}
		else
		{
			monster_type *t = get_monster(who);

			/* Choose 'new' target */
			s32b max_attempts = 10;

			do
			{
				t_y = t->fy - 1 + randint(3);
				t_x = t->fx - 1 + randint(3);
				max_attempts--;
			}
			while (max_attempts && in_bounds2(t_y, t_x) &&
				   !(player_has_los_bold(t_y, t_x)));

			if (max_attempts < 1)
			{
				t_y = t->fy;
				t_x = t->fx;
			}
		}

		project(0, 0, t_y, t_x, dam, typ, (PROJECT_STOP | PROJECT_KILL), -1);

		disturb(1, 0);
		return TRUE;
	}

	/* Reduce damage by distance */
	if (!(flags & PROJECT_NO_RADIUS_DAM)) dam = (dam + r) / (r + 1);


	/* If the player is blind, be more descriptive */
	if (blind) fuzzy = TRUE;

	/* If the player is hit by a trap, be more descritive */
	if (who == WHO_TRAP) fuzzy = TRUE;

	/* Did the dungeon do it? */
	if (who == WHO_DUNGEON)
		sprintf(killer, "%s", d_info[dungeon_type].name);
	else if (who == WHO_FLOOR)
		sprintf(killer, "%s", f_info[cave[p_ptr->py][p_ptr->px].feat].name);
	else if (who == WHO_PLAYER)
		strcpy(killer, "self");
	else if (who > 0)
	{
		/* Get the source monster */
		m_ptr = get_monster(who);

		/* Get the monster name */
		monster_desc(m_name, m_ptr, 0);

		/* Get the monster's real name */
		monster_desc(killer, m_ptr, 0x88);
	}
	else if (who == WHO_TRAP)
	{
		cave_type *c_ptr = &cave[p_ptr->py][p_ptr->px];

		if (flag_used(&c_ptr->activations))
		{
			s32b i;

			for (i = 0; i < c_ptr->activations.size; i++)
			{
				if (c_ptr->activations.node[i].flags & FLAG_FLAG_USED)
				{
					/* Does the firts spell has trap info ? */
					if (flag_exists(c_ptr->activations.node[i].vals.flags, FLAG_TRAP_CHAR))
					{
						// DGDGDGDG						sprintf(killer, school_spells[flag_get(c_ptr->activations.node[i].vals.flags, FLAG_SPELL_IDX)].name);
						sprintf(killer, "trap");
						break;
					}
				}
			}
		}
	}
	else
		strcpy(killer, "UNKNOWN");

	/*
	 * Build the calling code to the lua stuff
	 * do it with the lua API directly to avoid speed overheads
	 */
	oldtop = lua_gettop(L);

	/* Grab the function to call */
	lua_getglobal(L, "dam");

	lua_pushstring(L, "call");
	lua_gettable(L, -2);

	/* Push the type and target arguments */
	lua_pushnumber(L, typ);
	lua_pushstring(L, "player");

	/* Push the args in a table */
	lua_newtable(L);

	lua_pushstring(L, "dam");
	lua_pushnumber(L, dam);
	lua_settable(L, -3);

	lua_pushstring(L, "who");
	lua_pushnumber(L, who);
	lua_settable(L, -3);

	lua_pushstring(L, "rad");
	lua_pushnumber(L, r);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, y);
	lua_settable(L, -3);

	lua_pushstring(L, "x");
	lua_pushnumber(L, x);
	lua_settable(L, -3);

	lua_pushstring(L, "dir");
	lua_pushnumber(L, dir);
	lua_settable(L, -3);

	/* Call the function */
	if (lua_call(L, 3, 1))
	{
		cmsg_format(TERM_VIOLET, "ERROR in lua_call while calling 'project_p'.");
		lua_settop(L, oldtop);
		return(FALSE);
	}

	/* Analyze the resulting table */
	lua_pushstring(L, "dam");
	lua_gettable(L, -2);
	dam = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "dir");
	lua_gettable(L, -2);
	dir = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "fuzzy");
	lua_gettable(L, -2);
	fuzzy_msg = lua_tostring(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "msg");
	lua_gettable(L, -2);
	note = lua_tostring(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "move");
	lua_gettable(L, -2);
	do_move = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_pushstring(L, "obvious");
	lua_gettable(L, -2);
	obvious = lua_tonumber(L, -1); lua_pop(L, 1);

	lua_settop(L, oldtop);

	if (process_hooks_ret(HOOK_PROJECT_P_POST_LUA, "(d)",
						  "d,d,d,d,d,d,d,d,d",
						  who, r, y, x, dam, typ, a_rad, flags, dir))
		dam = process_hooks_return[0].num;

	/* Do it! */
	if (fuzzy && fuzzy_msg) msg_print(fuzzy_msg);

	if (note) msg_print(note);

	/* Handle moving the player.
	 *
	 * Note: This is a effect of force
	 */
	if (do_move)
	{
		s32b back = 0;

		back = 0;  /* Default of no movement */

		if (dir == 5)
		{
			a = rand_int(2) ? 1 : -1;
			b = rand_int(2) ? 1 : -1;
		}
		else
		{
			a = ddx[dir];
			b = ddy[dir];
		}

		/* How far can we push the monster? */
		for (do_move = 1; do_move < 3; do_move++)
		{
			/* Get monster coords */
			/* And offset position */
			y1 = p_ptr->py + (b * do_move);
			x1 = p_ptr->px + (a * do_move);

			if (!in_bounds(y1, x1)) continue;

			/* Require "empty" floor space */
			if (!in_bounds(y1, x1) || !cave_empty_bold(y1, x1)) continue;

			/* amount moved */
			back = do_move;
		}

		/* Move the monster */
		if (back)
		{
			y1 = p_ptr->py + (b * back);
			x1 = p_ptr->px + (a * back);
			swap_position(y1, x1);

			if (back == 2)
			{
				msg_print("You are knocked back!");
			}
			if (back == 1)
			{
				msg_print("You are knocked back and crushed!");

				/* was kept from being pushed all the way, do extra dam */
				dam = dam * 13 / 10;
			}

			/* Get new position */
			y = y1;
			x = x1;
		}
		else /* could not move the monster */
		{
			msg_print("You are severely crushed!");

			/* Do extra damage (1/3)*/
			dam = dam * 15 / 10;
		}

	}

	/*
	 * If damage is negative due to ultra high resistances, get healed
	 * Note that resistances are naturally capped at 100% and thus cannot heal
	 * it requires special treatment to allow it
	 */
	if (dam < 0)
		hp_player(-dam);
	else
		take_hit(dam, killer);


	/*
	 * Don't disturb if there was no damage and project was called
	 * by something other than a monster or the player (so, for example,
	 * a player with fire immunity won't be disturbed when resting on
	 * lava).
	 */
	if ((dam > 0) || (who >= 0))
		disturb(1, 0);

	/* Return "Anything seen?" */
	return (obvious);
}

/*
 * Find the distance from (x, y) to a line.
 */
s32b dist_to_line(s32b x, s32b y, s32b x1, s32b y1, s32b x2, s32b y2)
{
	/* Vector from (x, y) to (x1, y1) */
	s32b py = y1 - y;
	s32b px = x1 - x;

	/* Normal vector */
	s32b ny = x2 - x1;
	s32b nx = y1 - y2;

	/* Length of N */
	s32b d = distance(y1, x1, y2, x2);

	/* Component of P on N */
	d = ((d) ? ((py * ny + px * nx) / d) : 0);

	/* Absolute value */
	return ((d >= 0) ? d : 0 - d);
}


/*
 * Generic "beam"/"bolt"/"ball" projection routine.
 *
 * Input:
 *   who: Index of "source" monster (negative for "player")
 *   rad: Radius of explosion (0 = beam/bolt, 1 to 9 = ball)
 *   y,x: Target location (or location to travel "towards")
 *   dam: Base damage roll to apply to affected monsters (or player)
 *   typ: Type of damage to apply to monsters (and objects)
 *   flg: Extra bit flags (see PROJECT_xxxx in "defines.h")
 *
 * Return:
 *   TRUE if any "effects" of the projection were observed, else FALSE
 *
 * Allows a monster (or player) to project a beam/bolt/ball of a given kind
 * towards a given location (optionally passing over the heads of interposing
 * monsters), and have it do a given amount of damage to the monsters (and
 * optionally objects) within the given radius of the final location.
 *
 * A "bolt" travels from source to target and affects only the target grid.
 * A "beam" travels from source to target, affecting all grids passed through.
 * A "ball" travels from source to the target, exploding at the target, and
 *   affecting everything within the given radius of the target location.
 *
 * Traditionally, a "bolt" does not affect anything on the ground, and does
 * not pass over the heads of interposing monsters, much like a traditional
 * missile, and will "stop" abruptly at the "target" even if no monster is
 * positioned there, while a "ball", on the other hand, passes over the heads
 * of monsters between the source and target, and affects everything except
 * the source monster which lies within the final radius, while a "beam"
 * affects every monster between the source and target, except for the casting
 * monster (or player), and rarely affects things on the ground.
 *
 * Two special flags allow us to use this function in special ways, the
 * "PROJECT_HIDE" flag allows us to perform "invisible" projections, while
 * the "PROJECT_JUMP" flag allows us to affect a specific grid, without
 * actually projecting from the source monster (or player).
 *
 * The player will only get "experience" for monsters killed by himself
 * Unique monsters can only be destroyed by attacks from the player
 *
 * Only 256 grids can be affected per projection, limiting the effective
 * "radius" of standard ball attacks to nine units (diameter nineteen).
 *
 * One can project in a given "direction" by combining PROJECT_THRU with small
 * offsets to the initial location (see "line_spell()"), or by calculating
 * "virtual targets" far away from the player.
 *
 * One can also use PROJECT_THRU to send a beam/bolt along an angled path,
 * continuing until it actually hits something (useful for "stone to mud").
 *
 * Bolts and Beams explode INSIDE walls, so that they can destroy doors.
 *
 * Balls must explode BEFORE hitting walls, or they would affect monsters
 * on both sides of a wall.  Some bug reports indicate that this is still
 * happening in 2.7.8 for Windows, though it appears to be impossible.
 *
 * We "pre-calculate" the blast area only in part for efficiency.
 * More importantly, this lets us do "explosions" from the "inside" out.
 * This results in a more logical distribution of "blast" treasure.
 * It also produces a better (in my opinion) animation of the explosion.
 * It could be (but is not) used to have the treasure dropped by monsters
 * in the middle of the explosion fall "outwards", and then be damaged by
 * the blast as it spreads outwards towards the treasure drop location.
 *
 * Walls and doors are included in the blast area, so that they can be
 * "burned" or "melted" in later versions.
 *
 * This algorithm is intended to maximize simplicity, not necessarily
 * efficiency, since this function is not a bottleneck in the code.
 *
 * We apply the blast effect from ground zero outwards, in several passes,
 * first affecting features, then objects, then monsters, then the player.
 * This allows walls to be removed before checking the object or monster
 * in the wall, and protects objects which are dropped by monsters killed
 * in the blast, and allows the player to see all affects before he is
 * killed or teleported away.  The semantics of this method are open to
 * various interpretations, but they seem to work well in practice.
 *
 * We process the blast area from ground-zero outwards to allow for better
 * distribution of treasure dropped by monsters, and because it provides a
 * pleasing visual effect at low cost.
 *
 * Note that the damage done by "ball" explosions decreases with distance.
 * This decrease is rapid, grids at radius "dist" take "1/dist" damage.
 *
 * Notice the "napalm" effect of "beam" weapons.  First they "project" to
 * the target, and then the damage "flows" along this beam of destruction.
 * The damage at every grid is the same as at the "center" of a "ball"
 * explosion, since the "beam" grids are treated as if they ARE at the
 * center of a "ball" explosion.
 *
 * Currently, specifying "beam" plus "ball" means that locations which are
 * covered by the initial "beam", and also covered by the final "ball", except
 * for the final grid (the epicenter of the ball), will be "hit twice", once
 * by the initial beam, and once by the exploding ball.  For the grid right
 * next to the epicenter, this results in 150% damage being done.  The center
 * does not have this problem, for the same reason the final grid in a "beam"
 * plus "bolt" does not -- it is explicitly removed.  Simply removing "beam"
 * grids which are covered by the "ball" will NOT work, as then they will
 * receive LESS damage than they should.  Do not combine "beam" with "ball".
 *
 * The array "gy[],gx[]" with current size "grids" is used to hold the
 * collected locations of all grids in the "blast area" plus "beam path".
 *
 * Note the rather complex usage of the "gm[]" array.  First, gm[0] is always
 * zero.  Second, for N>1, gm[N] is always the index (in gy[],gx[]) of the
 * first blast grid (see above) with radius "N" from the blast center.  Note
 * that only the first gm[1] grids in the blast area thus take full damage.
 * Also, note that gm[rad+1] is always equal to "grids", which is the total
 * number of blast grids.
 *
 * Note that once the projection is complete, (y2,x2) holds the final location
 * of bolts/beams, and the "epicenter" of balls.
 *
 * Note also that "rad" specifies the "inclusive" radius of projection blast,
 * so that a "rad" of "one" actually covers 5 or 9 grids, depending on the
 * implementation of the "distance" function.  Also, a bolt can be properly
 * viewed as a "ball" with a "rad" of "zero".
 *
 * Note that if no "target" is reached before the beam/bolt/ball travels the
 * maximum distance allowed (MAX_RANGE), no "blast" will be induced.  This
 * may be relevant even for bolts, since they have a "1x1" mini-blast.
 *
 * Note that for consistency, we "pretend" that the bolt actually takes "time"
 * to move from point A to point B, even if the player cannot see part of the
 * projection path.  Note that in general, the player will *always* see part
 * of the path, since it either starts at the player or ends on the player.
 *
 * Hack -- we assume that every "projection" is "self-illuminating".
 *
 * Hack -- when only a single monster is affected, we automatically track
 * (and recall) that monster, unless "PROJECT_JUMP" is used.
 *
 * Note that all projections now "explode" at their final destination, even
 * if they were being projected at a more distant destination.  This means
 * that "ball" spells will *always* explode.
 *
 * Note that we must call "handle_stuff()" after affecting terrain features
 * in the blast radius, in case the "illumination" of the grid was changed,
 * and "update_view()" and "update_monsters()" need to be called.
 */
bool project(s32b who, s32b rad, s32b y, s32b x, s32b dam, s32b typ, s32b flg, s32b max_range)
{
	s32b i, t, dist, dir;

	s32b y1, x1; /* Projection path start */
	s32b y2, x2; /* Projection path end */

	s32b source_y, source_x; /* Location of source of damage */

	s32b dist_hack = 0;

	s32b y_saver, x_saver;  /* For reflecting monsters */

	s32b msec = delay_factor * delay_factor * delay_factor;

	/* Assume the player sees nothing */
	bool notice = FALSE;

	/* Assume the player has seen nothing */
	bool visual = FALSE;

	/* Assume the player has seen no blast grids */
	bool drawn = FALSE;

	/* Is the player blind? */
	bool blind = (intrinsic(BLIND) ? TRUE : FALSE);

	/* Is it a breath ? */
	bool breath = (flg & PROJECT_CONE) ? TRUE : FALSE;

	/* Number of grids in the "path" */
	s32b path_n = 0;

	/* Actual grids in the "path" */
	u16b path_g[1024];

	/* Number of grids in the "blast area" (including the "beam" path) */
	s32b grids = 0;

	s32b effect = 0;

	/* Coordinates of the affected grids */
	byte gx[1024], gy[1024];

	/* Direction damage is traveling at that particular grid */
	byte gdir[1024];

	/* Encoded "radius" info (see above) */
	byte gm[64];

	/* Actual radius encoded in gm[] */
	s32b gm_rad = rad;

	/* Start at player */
	if ((who <= 0) && ((who != WHO_FLOOR) && (who != WHO_DUNGEON)&&
							(who != WHO_TRAP)))
	{
		x1 = p_ptr->px;
		y1 = p_ptr->py;
	}

	/* Start at monster */
	else if (who > 0)
	{
		monster_type *t = get_monster(who);
		x1 = t->fx;
		y1 = t->fy;
	}

	/* Oops */
	else
	{
		x1 = x;
		y1 = y;
	}

	source_x = x1;
	source_y = y1;

	/*
	 * Hack -- Jump to target
	 * NOTE: Do this after we've figured out the coordinates of the
	 * source of the damage.
	 */
	if (flg & (PROJECT_JUMP))
	{
		x1 = x;
		y1 = y;

		/* Clear the flag */
		flg &= ~(PROJECT_JUMP);
	}

	y_saver = y1;
	x_saver = x1;

	/* Default "destination" */
	y2 = y;
	x2 = x;


	/* Hack -- verify stuff */
	if (flg & (PROJECT_THRU))
	{
		if ((x1 == x2) && (y1 == y2))
		{
			flg &= ~(PROJECT_THRU);
		}
	}


	/* Hack -- Assume there will be no blast (max radius 16) */
	for (dist = 0; dist < 64; dist++) gm[dist] = 0;


	/* Initial grid */
	y = y1;
	x = x1;
	dist = 0;

	/* Collect beam grids */
	if (flg & (PROJECT_BEAM))
	{
		gy[grids]   = y;
		gx[grids]   = x;
		gdir[grids] = line_to_dir(source_y, source_x, y, x);
		grids++;
	}


	/* Calculate the projection path */
	if ((who == WHO_FLOOR) || (who == WHO_DUNGEON))
		path_n = 0;
	else
		path_n = project_path(path_g, (max_range == -1) ? MAX_RANGE : max_range, y1, x1, y2, x2, flg);


	/* Hack -- Handle stuff */
	handle_stuff();

	/* Project along the path */
	for (i = 0; i < path_n; ++i)
	{
		s32b oy = y;
		s32b ox = x;

		s32b ny = GRID_Y(path_g[i]);
		s32b nx = GRID_X(path_g[i]);

		/* Hack -- Balls explode before reaching walls */
		if (cave_wall_bold(ny, nx) && (rad > 0))
		{
			/*
			 * After exploding, the origin point of the damage becomes
			 * the center of the blast.
			 */
			source_x = nx;
			source_y = ny;

			break;
		}

		/* Advance */
		y = ny;
		x = nx;

		/* Collect beam grids */
		if (flg & (PROJECT_BEAM))
		{
			gy[grids] = y;
			gx[grids] = x;
			gdir[grids] = line_to_dir(source_y, source_x, y, x);
			grids++;
		}

		/* Only do visuals if requested */
		if (!blind && !(flg & (PROJECT_HIDE)))
		{
			/* Only do visuals if the player can "see" the bolt */
			if (panel_contains(y, x) && player_has_los_bold(y, x))
			{
				u16b p;

				byte a;
				char c;

				/* Obtain the bolt pict */
				p = bolt_pict(oy, ox, y, x, typ);

				/* Extract attr/char */
				a = PICT_A(p);
				c = PICT_C(p);

				/*
				if (!c)
					c = '*';
				*/

				/* Visual effects */
				sdl_utl_fast_draw(TRUE);
				print_rel(c, a, y, x);
				move_cursor_relative(y, x);
				if (fresh_before) Term_fresh();
				Term_xtra(TERM_XTRA_EFFECTS_DELAY, msec);
				lite_spot(y, x);
				if (fresh_before) Term_fresh();
				sdl_utl_fast_draw(FALSE);

				/* Display "beam" grids */
				if (flg & (PROJECT_BEAM))
				{
					/* Obtain the explosion pict */
					p = bolt_pict(y, x, y, x, typ);

					/* Extract attr/char */
					a = PICT_A(p);
					c = PICT_C(p);

					/* Visual effects */
					print_rel(c, a, y, x);
				}

				/* Hack -- Activate delay */
				visual = TRUE;
			}

			/* Hack -- delay anyway for consistency */
			else if (visual)
			{
				/* Delay for consistency */
				Term_xtra(TERM_XTRA_EFFECTS_DELAY, msec);
			}
		}
	}

	/* Save the "blast epicenter" */
	y2 = y;
	x2 = x;

	/* Start the "explosion" */
	gm[0] = 0;

	/* Hack -- make sure beams get to "explode" */
	gm[1] = grids;

	dist_hack = dist;

	/* Explode */
	if (TRUE)
	{
		/* Hack -- remove final beam grid */
		if (flg & (PROJECT_BEAM))
		{
			grids--;
		}

		/*
		 * Create a conical breath attack
		 *
		 *         ***
		 *     ********
		 * D********@**
		 *     ********
		 *         ***
		 */
		if (breath)
		{
			s32b by, bx;
			s32b brad = 0;
			s32b bdis = 0;
			s32b cdis;

			/* Not done yet */
			bool done = FALSE;

			by = y1;
			bx = x1;

			dist = path_n;

			while (bdis <= dist + rad)
			{
				/* Travel from center outward */
				for (cdis = 0; cdis <= brad; cdis++)
				{
					/* Scan the maximal blast area of radius "cdis" */
					for (y = by - cdis; y <= by + cdis; y++)
					{
						for (x = bx - cdis; x <= bx + cdis; x++)
						{
							/* Ignore "illegal" locations */
							if (!in_bounds(y, x)) continue;

							/* Enforce a circular "ripple" */
							if (distance(y1, x1, y, x) != bdis) continue;

							/* Enforce an arc */
							if (distance(by, bx, y, x) != cdis) continue;

							/*
							 * The blast is stopped cold by unpassable
							 * walls.
							 */
							if (cave_wall_bold(y, x) && !cave_pass_bold(y, x))
								continue;

							/*
							 * The blast penetrates the first wall grid,
							 * but not beyond.
							 */
							if (!wallless_line(y2, x2, y, x,
											   (flg & PROJECT_WALL) != 0))
								continue;

							/* Save this grid */
							gy[grids] = y;
							gx[grids] = x;
							gdir[grids] = line_to_dir(source_y, source_x,
													  y, x);
							grids++;
						}
					}
				}

				/* Encode some more "radius" info */
				gm[bdis + 1] = grids;

				/* Stop moving */
				if ((by == y2) && (bx == x2)) done = TRUE;

				/* Finish */
				if (done)
				{
					bdis++;
					continue;
				}

				/* Ripple outwards */
				mmove2(&by, &bx, y1, x1, y2, x2);

				/* Find the next ripple */
				bdis++;

				/* Increase the size */
				brad = (rad * bdis) / dist;
			}

			/* Store the effect size */
			gm_rad = bdis;
		}
		else
		{
			/* Determine the blast area, work from the inside out */
			for (dist = 0; dist <= rad; dist++)
			{
				/* Scan the maximal blast area of radius "dist" */
				for (y = y2 - dist; y <= y2 + dist; y++)
				{
					for (x = x2 - dist; x <= x2 + dist; x++)
					{
						/* Ignore "illegal" locations */
						if (!in_bounds(y, x)) continue;

						/* Enforce a "circular" explosion */
						if (distance(y2, x2, y, x) != dist) continue;

						/*
						 * The blast is stopped cold by unpassable
						 * walls.
						 */
						if (cave_wall_bold(y, x) && !cave_pass_bold(y, x))
							continue;

						/*
						 * The blast penetrates the first wall grid,
						 * but not beyond.
						 */
						if (!wallless_line(y2, x2, y, x,
										   (flg & PROJECT_WALL) != 0))
							continue;

						/* Save this grid */
						gy[grids] = y;
						gx[grids] = x;
						gdir[grids] = line_to_dir(source_y, source_x, y, x);
						grids++;
					}
				}

				/* Encode some more "radius" info */
				gm[dist + 1] = grids;
			}
		}
	}


	/* Speed -- ignore "non-explosions" */
	if (!grids) return (FALSE);


	/* Display the "blast area" if requested */
	sdl_utl_fast_draw(TRUE);
	if (!blind && !(flg & (PROJECT_HIDE_BLAST)))
	{
		/* Then do the "blast", from inside out */
		for (t = 0; t <= gm_rad; t++)
		{
			/* Dump everything with this radius */
			for (i = gm[t]; i < gm[t + 1]; i++)
			{
				/* Extract the location */
				y = gy[i];
				x = gx[i];

				/* Only do visuals if the player can "see" the blast */
				if (panel_contains(y, x) && player_has_los_bold(y, x))
				{
					u16b p;

					byte a;
					char c;

					drawn = TRUE;

					/* Obtain the explosion pict */
					p = bolt_pict(y, x, y, x, typ);

					/* Extract attr/char */
					a = PICT_A(p);
					c = PICT_C(p);

					/* Visual effects -- Display */
					print_rel(c, a, y, x);
				}
			}

			/* Hack -- center the cursor */
			move_cursor_relative(y2, x2);

			/* Flush each "radius" seperately */
			if (fresh_before) Term_fresh();

			/* Delay (efficiently) */
			if (visual || drawn)
			{
				Term_xtra(TERM_XTRA_EFFECTS_DELAY, msec);
			}
		}

		/* Flush the erasing */
		if (drawn)
		{
			/* Erase the explosion drawn above */
			for (i = 0; i < grids; i++)
			{
				/* Extract the location */
				y = gy[i];
				x = gx[i];

				/* Hack -- Erase if needed */
				if (panel_contains(y, x) && player_has_los_bold(y, x))
				{
					lite_spot(y, x);
				}
			}

			/* Hack -- center the cursor */
			move_cursor_relative(y2, x2);

			/* Flush the explosion */
			if (fresh_before) Term_fresh();
		}
	}
	sdl_utl_fast_draw(FALSE);


	/* Check features */
	if (flg & (PROJECT_GRID))
	{
		/* Start with "dist" of zero */
		dist = 0;

		/* Effect ? */
		if (flg & PROJECT_STAY)
		{
			effect = new_effect(who, typ, dam, project_time, gy[0], gx[0], rad, project_time_effect, project_time_speed);
			project_time = 0;
			project_time_speed = 0;
			project_time_effect = 0;
		}

		/* Scan for features */
		for (i = 0; i < grids; i++)
		{
			/* Hack -- Notice new "dist" values */
			if (gm[dist + 1] == i) dist++;

			/* Get the grid location */
			y   = gy[i];
			x   = gx[i];
			dir = gdir[i];

			/* Find the closest point in the blast */
			if (breath)
			{
				s32b d = dist_to_line(x, y, x1, y1, x2, y2);

				/* Affect the grid */
				if (project_f(who, d, y, x, dam, typ, flg, dir))
					notice = TRUE;
			}
			else
			{
				/* Affect the feature in that grid */
				if (project_f(who, dist, y, x, dam, typ, flg, dir))
					notice = TRUE;
			}

			/* Effect ? */
			if (flg & PROJECT_STAY)
			{
				cave[y][x].effect = effect;
				lite_spot(y, x);
			}
		}
	}


	/* Update stuff if needed */
	if (p_ptr->update) update_stuff();


	/* Check objects */
	if (flg & (PROJECT_ITEM))
	{
		/* Start with "dist" of zero */
		dist = 0;

		/* Scan for objects */
		for (i = 0; i < grids; i++)
		{
			/* Hack -- Notice new "dist" values */
			if (gm[dist + 1] == i) dist++;

			/* Get the grid location */
			y   = gy[i];
			x   = gx[i];
			dir = gdir[i];

			if (breath)
			{
				s32b d = dist_to_line(x, y, x1, y1, x2, y2);

				/* Affect the object in the grid */
				if (project_o(who, d, y, x, dam, typ, flg, dir)) notice = TRUE;
			}
			else
			{
				/* Affect the object in the grid */
				if (project_o(who, dist, y, x, dam, typ, flg, dir))
					notice = TRUE;
			}
		}
	}


	/* Check monsters */
	if (flg & (PROJECT_KILL))
	{
		/* Mega-Hack */
		project_m_n = 0;
		project_m_x = 0;
		project_m_y = 0;

		/* Start with "dist" of zero */
		dist = 0;

		/* Scan for monsters */
		for (i = 0; i < grids; i++)
		{
			/* Hack -- Notice new "dist" values */
			if (gm[dist + 1] == i) dist++;

			/* Get the grid location */
			y   = gy[i];
			x   = gx[i];
			dir = gdir[i];

			if (grids > 1)
			{
				/* Find the closest point in the blast */
				if (breath)
				{
					s32b d = dist_to_line(x, y, x1, y1, x2, y2);

					/* Affect the monster in the grid */
					if (project_m(who, d, y, x, dam, typ, flg, dir))
						notice = TRUE;
				}
				else
				{
					/* Affect the monster in the grid */
					if (project_m(who, dist, y, x, dam, typ, flg, dir))
						notice = TRUE;
				}
			}
			else
			{
				s32b m_idx   = cave[y][x].m_idx;

				if (m_idx)
				{
					monster_type *m_ptr = get_monster(m_idx);
					monster_race *ref_ptr = race_inf(m_ptr);

					/* Paranoia about dead monsters */
					if (m_ptr->r_idx == 0)
					{
						cmsg_format(TERM_VIOLET,
									"BUG: dead monster encountered in "
									"project()");
						cave[y][x].m_idx = 0;
						continue;
					}

					/*
					 * Effects caused by the dungeon of floor aren't
					 * bolts, so they don't relfect.  The monster damaging
					 * itself isn't bolts, so they don't reflect either.
					 * Effects caused by divine wrath don't reflect
					 * because, well, they're caused by a *GOD*.
					 * Otherwise, the REFLECTING flag is the perecent
					 * chance of reflecting.
					 */
					if (!(flg & PROJECT_NO_REFLECT) && (dist_hack > 1) &&
						(who != WHO_FLOOR) && (who != WHO_DUNGEON) &&
						(who != m_idx) &&
						magik(get_flag(ref_ptr, FLAG_REFLECTING)))
					{
						s32b t_y, t_x;

						if (who < 0)
						{
							/*
							 * Bolt didn't come from a monster, so just
							 * reverse the direction it came from.
							 */

							if ((dir == 0) || (dir == 5))
								/*
								 * If the bolt isn't coming from any
								 * direction in particular, then pick a
								 * random direction.
								 */
								dir = ddd[rand_int(8)];

							t_y = y + (ddy[dir] * -99);
							t_x = x + (ddx[dir] * -99);
						}
						else
						{
							s32b max_attempts = 10;
							/* Choose 'new' target */
							do
							{
								t_y = y_saver - 1 + randint(3);
								t_x = x_saver - 1 + randint(3);
								max_attempts--;
							}
							while (max_attempts && in_bounds2(t_y, t_x) &&
								!(los(y, x, t_y, t_x)));

							if (max_attempts < 1)
							{
								t_y = y_saver;
								t_x = x_saver;
							}
						}

						if (m_ptr->ml)
						{
							msg_print("The attack bounces!");
							monstmem_add(RT_MISC, FLAG_REFLECTING,
								get_monster(cave[y][x].m_idx),0);
							flag_bool(&ref_ptr->r_flags, FLAG_REFLECTING);
						}

						project(cave[y][x].m_idx, 0, t_y, t_x, dam, typ, flg, -1);
					}
					else
					{
						if (project_m(who, dist, y, x, dam, typ, flg, dir))
							notice = TRUE;
					}
				}
			}
		}

		/* Player affected one monster (without "jumping") */
		if ((who < 0) && (project_m_n == 1) && !(flg & (PROJECT_JUMP)))
		{
			/* Location */
			x = project_m_x;
			y = project_m_y;

			/* Track if possible */
			if (cave[y][x].m_idx > 0)
			{
				monster_type *m_ptr = get_monster(cave[y][x].m_idx);

				/* Hack -- auto-recall */
				if (m_ptr->ml) monster_race_track(m_ptr->r_idx, m_ptr->ego);

				/* Hack - auto-track */
				if (m_ptr->ml) health_track(cave[y][x].m_idx);
			}
		}
	}


	/* Check player */
	if (flg & (PROJECT_KILL))
	{
		/* Start with "dist" of zero */
		dist = 0;

		/* Scan for player */
		for (i = 0; i < grids; i++)
		{
			/* Hack -- Notice new "dist" values */
			if (gm[dist + 1] == i) dist++;

			/* Get the grid location */
			y   = gy[i];
			x   = gx[i];
			dir = gdir[i];

			/* Find the closest point in the blast */
			if (breath)
			{
				s32b d = dist_to_line(x, y, x1, y1, x2, y2);

				/* Affect the player */
				if (project_p(who, d, y, x, dam, typ, rad, flg, dir))
					notice = TRUE;
			}
			else
			{
				/* Affect the player */
				if (project_p(who, dist, y, x, dam, typ, rad, flg, dir))
					notice = TRUE;
			}
		}
	}

	/* Return "something was noticed" */
	return (notice);
}


/*
 * Polymorph a monster at given location.
 */
s16b do_poly_monster(s32b y, s32b x)
{
	cave_type *c_ptr = &cave[y][x];

	monster_type *m_ptr;

	s16b hack_m_idx;
	s16b old_m_idx;
	s16b new_m_idx = 0;

	s16b new_r_idx;

	/* Get a "old" monster */
	old_m_idx = c_ptr->m_idx;

	/* Giga-Hack -- Remember monster */
	hack_m_idx = old_m_idx;

	/* Get a monster */
	m_ptr = get_monster(c_ptr->m_idx);

	/* Pick a "new" monster race */
	new_r_idx = poly_r_idx(m_ptr->r_idx);

	/* No polymorph happend */
	if (new_r_idx == m_ptr->r_idx) return 0;

	/* Giga-Hack -- Removes the moster XXX XXX XXX XXX */
	c_ptr->m_idx = 0;

	/*
	 * Handle polymorph --
	 * Create a new monster (no groups)
	 */
	if (place_monster_aux(y, x, new_r_idx, FALSE, FALSE, m_ptr->faction))
	{
		/* Get a "new" monster */
		new_m_idx = c_ptr->m_idx;

		/* Giga-Hack -- Remember "new" monster */
		hack_m_idx = new_m_idx;

		/* "Kill" the "old" monster */
		delete_monster_idx(old_m_idx);

		flag_bool(&p_ptr->redraw, FLAG_PR_MAP);
	}

	/* Giga-Hack -- restore saved monster XXX XXX XXX */
	c_ptr->m_idx = hack_m_idx;

	return new_m_idx;
}
