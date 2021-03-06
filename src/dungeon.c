/* File: dungeon.c */

/* Purpose: Angband game engine */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#ifdef USE_SDL
#include <SDL.h>
#include "sdl-extern.h"
#endif
#include "lua/lua.h"
#include "tolua.h"
#include "SFMT.h"

extern lua_State* L;

/*
 * I created this when a bug misplaced my character and the game wasn't able
 * to load it again.. very frustrating.
 * So this hack will generate a new level without calling dungeon(), and
 * then the normal behavior will apply.
 */
/* #define SAVE_HACK */
#ifdef SAVE_HACK
bool save_hack = TRUE;
#endif

/*
 * Return a "feeling" (or NULL) about an item.  Method 1 (Heavy).
 */
byte value_check_aux1(object_type *o_ptr)
{
	/* Artifacts */
	if (artifact_p(o_ptr))
	{
		/* Cursed/Broken */
		if (cursed_p(o_ptr)) return (SENSE_TERRIBLE);

		/* Normal */
		return (SENSE_SPECIAL);
	}

	/* Ego-Items */
	if (ego_item_p(o_ptr))
	{
		/* Cursed/Broken */
		if (cursed_p(o_ptr)) return (SENSE_WORTHLESS);

		/* Normal */
		return (SENSE_EXCELLENT);
	}

	/* Cursed items */
	if (cursed_p(o_ptr)) return (SENSE_CURSED);

	/* Good "armor" bonus */
	if (o_ptr->to_a > 0) return (SENSE_GOOD_HEAVY);

	/* Good "weapon" bonus */
	if (o_ptr->to_h + o_ptr->to_d > 0) return (SENSE_GOOD_HEAVY);

	/* Default to "average" */
	return (SENSE_AVERAGE);
}

byte value_check_aux1_magic(object_type *o_ptr)
{
	// DGDGDGDGDG -- hooks to pseudo id ?

	/* No feeling */
	return (SENSE_NONE);
}


/*
 * Return a "feeling" (or NULL) about an item.  Method 2 (Light).
 */
byte value_check_aux2(object_type *o_ptr)
{
	/* Cursed items (all of them) */
	if (cursed_p(o_ptr)) return (SENSE_CURSED);

	/* Artifacts -- except cursed/broken ones */
	if (artifact_p(o_ptr)) return (SENSE_GOOD_LIGHT);

	/* Ego-Items -- except cursed/broken ones */
	if (ego_item_p(o_ptr)) return (SENSE_GOOD_LIGHT);

	/* Good armor bonus */
	if (o_ptr->to_a > 0) return (SENSE_GOOD_LIGHT);

	/* Good weapon bonuses */
	if (o_ptr->to_h + o_ptr->to_d > 0) return (SENSE_GOOD_LIGHT);

	/* No feeling */
	return (SENSE_NONE);
}


byte value_check_aux2_magic(object_type *o_ptr)
{
	// DGDGDGDGDG -- hooks to pseudo id ?

	/* No feeling */
	return (SENSE_NONE);
}


byte select_sense(object_type *o_ptr, bool ok_combat, bool ok_magic)
{
	// DGDGDGDGDG -- hooks to pseudo id ?
	return 0;
}

/*
 * Sense the inventory
 *
 * Combat items (weapons and armour) - Fast, weak if combat skill < 10, strong
 * otherwise.
 *
 * Magic items (scrolls, staffs, wands, potions etc) - Slow, weak if
 * magic skill < 10, strong otherwise.
 *
 * It shouldn't matter a lot to discriminate against magic users, because
 * they learn one form of ID or another, and because most magic items are
 * easy_know.
 */
static void sense_inventory(void)
{
	s32b combat_lev, magic_lev;

	bool heavy_combat, heavy_magic;
	bool ok_combat, ok_magic;

	byte feel;

	char o_name[80];


	/*** Check for "sensing" ***/

	/* No sensing when confused */
	if (intrinsic(CONFUSED)) return;

	/* Can we pseudo id */
#if 0

	if (0 == rand_int(133 - get_skill_scale(SKILL_COMBAT, 130))) ok_combat = TRUE;
	if (0 == rand_int(133 - get_skill_scale(SKILL_MAGIC, 130))) ok_magic = TRUE;

#endif

	/*
	 * In Angband, the chance of pseudo-id uses two different formulae:
	 *
	 * (1) Fast. 0 == rand_int(BASE / (plev * plev + 40)
	 * (2) Slow. 0 == rand_int(BASE / (plev + 5)
	 *
	 * Warriors: Fase with BASE == 9000
	 * Magi: Slow with BASE == 240000
	 * Priests: Fast with BASE == 10000
	 * Rogues: Fase with BASE == 20000
	 * Rangers: Slow with BASE == 120000
	 * Paladins: Fast with BASE == 80000
	 *
	 * In other words, those who have identify spells are penalised.
	 * The problems with Pern/Tome since it externalised player classes
	 * is that it uses the same and slow formula for spellcasters and
	 * fighters.
	 *
	 * In the following code, combat item pseudo-ID improves exponentially,
	 * (fast with BASE 9000) and magic one linear (slow with base 60000 --
	 * twice faster than V rangers).
	 *
	 * I hope this makes it closer to the original model -- pelpel
	 */

	/* The combat skill affects weapon/armour pseudo-ID */
	combat_lev = get_skill(SKILL_COMBAT);

	/* Use the fast formula */
	ok_combat = (0 == rand_int(9000L / (combat_lev * combat_lev + 40)));

	/* The magic skill affects magic item pseudo-ID */
	magic_lev = get_skill(SKILL_MAGIC);

	/*
	 * Use the slow formula, because spellcasters have id spells
	 *
	 * Lowered the base value because V rangers are known to have
	 * pretty useless pseudo-ID. This should make it ten times more often.
	 */
	ok_magic = (0 == rand_int(12000L / (magic_lev + 5)));

	/* Both ID rolls failed */
	if (!ok_combat && !ok_magic) return;

	/* Higher skill levels give the player better sense of items */
	heavy_combat = (combat_lev > 10) ? TRUE : FALSE;
	heavy_magic = (magic_lev > 10) ? TRUE : FALSE;


	/*** Sense everything ***/

	/* Check everything */
	for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		byte okay = 0;

		/* Valid "tval" codes */
		okay = select_sense(o_ptr, ok_combat, ok_magic);

		/* Skip non-sense machines */
		if (!okay) continue;

		/* We know about it already, do not tell us again */
		if (o_ptr->ident & (IDENT_SENSE)) continue;

		/* It is fully known, no information needed */
		if (object_known_p(o_ptr)) continue;

		/* Occasional failure on inventory items */
		if ((inven_nb < INVEN_PACK) && (0 != rand_int(5))) continue;

		/* Check for a feeling */
		if (okay == 1)
		{
			feel = (heavy_combat ? value_check_aux1(o_ptr) : value_check_aux2(o_ptr));
		}
		else
		{
			feel = (heavy_magic ? value_check_aux1_magic(o_ptr) : value_check_aux2_magic(o_ptr));
		}

		/* Skip non-feelings */
		if (feel == SENSE_NONE) continue;

		/* Stop everything */
		if (disturb_minor) disturb(0, 0);

		/* Get an object description */
		object_desc(o_name, o_ptr, FALSE, 0);

		/* Message (equipment) */
		if (inven_nb >= INVEN_PACK)
		{
			msg_format("You feel the %s (%c) you are %s %s %s...",
			           o_name, index_to_label(item_slot), describe_use(item_slot),
			           ((o_ptr->number == 1) ? "is" : "are"), sense_desc[feel]);
		}

		/* Message (inventory) */
		else
		{
			msg_format("You feel the %s (%c) in your pack %s %s...",
			           o_name, index_to_label(item_slot),
			           ((o_ptr->number == 1) ? "is" : "are"), sense_desc[feel]);
		}

		/* We have "felt" it */
		o_ptr->ident |= (IDENT_SENSE);

		/* Set sense property */
		o_ptr->sense = feel;

		/* Combine / Reorder the pack (later) */
		p_ptr->notice |= (PN_COMBINE | PN_REORDER);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_EQUIP);
		flag_bool(&p_ptr->window, FLAG_PW_INVEN);
	}
	end_inventory();

	/* Squelch ! */
	squeltch_inventory();
}


/*
 * If player has inscribed the object with "!!", let him know when it's
 * recharged. -LM-
 */
static void recharged_notice(object_type *o_ptr)
{
	char o_name[80];

	cptr s;


	/* No inscription */
	if (!o_ptr->note) return;

	/* Find a '!' */
	s = strchr(quark_str(o_ptr->note), '!');

	/* Process notification request. */
	while (s)
	{
		/* Find another '!' */
		if (s[1] == '!')
		{
			/* Describe (briefly) */
			object_desc(o_name, o_ptr, FALSE, 0);

			/* Notify the player */
			if (o_ptr->number > 1)
			{
				msg_format("Your %s are recharged.", o_name);
			}
			else
			{
				msg_format("Your %s is recharged.", o_name);
			}

			/* Done. */
			return;
		}

		/* Keep looking for '!'s */
		s = strchr(s + 1, '!');
	}
}

/*
 * Does an object decay?
 *
 * Should belong to object1.c, renamed to object_decays() -- pelpel
 */
bool decays(object_type *o_ptr)
{
	if (has_flag(o_ptr, FLAG_DECAY)) return (TRUE);

	return (FALSE);
}



/*
 * Generate the feature effect
 */
void apply_effect(s32b y, s32b x)
{
	cave_type *c_ptr = &cave[y][x];

	feature_type *f_ptr = &f_info[c_ptr->feat];

	if (turn % 10) return;

	if (f_ptr->d_frequency[0] != 0)
	{
		s32b i;

		for (i = 0; i < 4; i++)
		{
			/* Check the frequency */
			if (f_ptr->d_frequency[i] == 0) continue;

			if (((turn % f_ptr->d_frequency[i]) == 0) &&
			                ((f_ptr->d_side[i] != 0) || (f_ptr->d_dice[i] != 0)))
			{
				s32b dam = 0;
				s32b d = f_ptr->d_dice[i], s = f_ptr->d_side[i];

				if (d == -1) d = p_ptr->lev;
				if (s == -1) s = p_ptr->lev;

				/* Roll damage */
				dam = damroll(d, s);

				/* Apply damage */
				project(WHO_FLOOR, 0, y, x, dam, f_ptr->d_type[i],
					PROJECT_KILL | PROJECT_HIDE | PROJECT_HIDE_BLAST, -1);

				/* Hack -- notice death */
				if (!alive || death) return;
			}
		}
	}
}


/*
 * Handle staying spell effects once every 10 game turns
 */
static void process_effects(void)
{
	s32b i, j;

	/* Not in the small-scale wilderness map */
	if (p_ptr->wild_mode) return;

	/* Calc energies */
	for (i = 0; i < MAX_EFFECTS; i++)
	{
		/* Skip empty slots */
		if (effects[i].time == 0) continue;

		/* Give some energy */
		effects[i].energy += TICK_ENERGY;
	}

	/*
	 * I noticed significant performance degrade after the introduction
	 * of staying spell effects. I believe serious optimisation effort
	 * is required before another release.
	 *
	 * More important is to fix that display weirdness...
	 *
	 * It seems that the game never expects that monster deaths and
	 * terrain feature changes should happen here... Moving these
	 * to process_player() [before resting code, with "every 10 game turn"
	 * 'if'] may or may not fix the problem... -- pelpel to DG
	 */
	for (j = 0; j < cur_hgt - 1; j++)
	{
		for (i = 0; i < cur_wid - 1; i++)
		{
			s32b e = cave[j][i].effect;

			if (e)
			{
				effect_type *e_ptr = &effects[e];

				if (!e_ptr->time)
				{
					cave[j][i].effect = 0;
					lite_spot(j, i);
					continue;
				}

				if (e_ptr->energy < 1000) continue;

				if (e_ptr->time)
				{
					/* Did monster which caused it die? */
					if (e_ptr->who > 0 && !get_monster(e_ptr->who)->r_idx)
						e_ptr->who = WHO_FLOOR;

					/* Apply damage */
					project(e_ptr->who, 0, j, i, e_ptr->dam, e_ptr->type,
						PROJECT_KILL | PROJECT_ITEM | PROJECT_GRID |
						PROJECT_HIDE | PROJECT_HIDE_BLAST, -1);
				}

				if ((e_ptr->flags & EFF_WAVE) && !(e_ptr->flags & EFF_LAST))
				{
					if (distance(e_ptr->cy, e_ptr->cx, j, i) < e_ptr->rad - 1)
						cave[j][i].effect = 0;
				}
				else if ((e_ptr->flags & EFF_STORM) && !(e_ptr->flags & EFF_LAST))
				{
					cave[j][i].effect = 0;
				}

				lite_spot(j, i);
			}
		}
	}

	/* Reduce & handle effects */
	for (i = 0; i < MAX_EFFECTS; i++)
	{
		/* Skip empty slots */
		if (effects[i].time == 0) continue;

		/* No turn yet */
		if (effects[i].energy < 1000) continue;

		/* Use up "some" energy */
		effects[i].energy -= extract_energy[effects[i].speed + 110];

		/* Reduce duration */
		effects[i].time--;

		/* Creates a "wave" effect*/
		if (effects[i].flags & EFF_WAVE)
		{
			effect_type *e_ptr = &effects[i];
			s32b x, y, z;

			e_ptr->rad++;

			/* What a frelling ugly line of ifs ... */
			if (effects[i].flags & EFF_DIR8)
				for (y = e_ptr->cy - e_ptr->rad, z = 0; y <= e_ptr->cy; y++, z++)
				{
					for (x = e_ptr->cx - (e_ptr->rad - z); x <= e_ptr->cx + (e_ptr->rad - z); x++)
					{
						if (!in_bounds(y, x)) continue;

						if (los(e_ptr->cy, e_ptr->cx, y, x) &&
							(distance(e_ptr->cy, e_ptr->cx, y, x) == e_ptr->rad))
							cave[y][x].effect = i;
					}
				}
			else if (effects[i].flags & EFF_DIR2)
				for (y = e_ptr->cy, z = e_ptr->rad; y <= e_ptr->cy + e_ptr->rad; y++, z--)
				{
					for (x = e_ptr->cx - (e_ptr->rad - z); x <= e_ptr->cx + (e_ptr->rad - z); x++)
					{
						if (!in_bounds(y, x)) continue;

						if (los(e_ptr->cy, e_ptr->cx, y, x) &&
							(distance(e_ptr->cy, e_ptr->cx, y, x) == e_ptr->rad))
							cave[y][x].effect = i;
					}
				}
			else if (effects[i].flags & EFF_DIR6)
				for (x = e_ptr->cx, z = e_ptr->rad; x <= e_ptr->cx + e_ptr->rad; x++, z--)
				{
					for (y = e_ptr->cy - (e_ptr->rad - z); y <= e_ptr->cy + (e_ptr->rad - z); y++)
					{
						if (!in_bounds(y, x)) continue;

						if (los(e_ptr->cy, e_ptr->cx, y, x) &&
							(distance(e_ptr->cy, e_ptr->cx, y, x) == e_ptr->rad))
							cave[y][x].effect = i;
					}
				}
			else if (effects[i].flags & EFF_DIR4)
				for (x = e_ptr->cx - e_ptr->rad, z = 0; x <= e_ptr->cx; x++, z++)
				{
					for (y = e_ptr->cy - (e_ptr->rad - z); y <= e_ptr->cy + (e_ptr->rad - z); y++)
					{
						if (!in_bounds(y, x)) continue;

						if (los(e_ptr->cy, e_ptr->cx, y, x) &&
							(distance(e_ptr->cy, e_ptr->cx, y, x) == e_ptr->rad))
							cave[y][x].effect = i;
					}
				}
			else if (effects[i].flags & EFF_DIR9)
				for (y = e_ptr->cy - e_ptr->rad; y <= e_ptr->cy; y++)
				{
					for (x = e_ptr->cx; x <= e_ptr->cx + e_ptr->rad; x++)
					{
						if (!in_bounds(y, x)) continue;

						if (los(e_ptr->cy, e_ptr->cx, y, x) &&
							(distance(e_ptr->cy, e_ptr->cx, y, x) == e_ptr->rad))
							cave[y][x].effect = i;
					}
				}
			else if (effects[i].flags & EFF_DIR1)
				for (y = e_ptr->cy; y <= e_ptr->cy + e_ptr->rad; y++)
				{
					for (x = e_ptr->cx - e_ptr->rad; x <= e_ptr->cx; x++)
					{
						if (!in_bounds(y, x)) continue;

						if (los(e_ptr->cy, e_ptr->cx, y, x) &&
							(distance(e_ptr->cy, e_ptr->cx, y, x) == e_ptr->rad))
							cave[y][x].effect = i;
					}
				}
			else if (effects[i].flags & EFF_DIR7)
				for (y = e_ptr->cy - e_ptr->rad; y <= e_ptr->cy; y++)
				{
					for (x = e_ptr->cx - e_ptr->rad; x <= e_ptr->cx; x++)
					{
						if (!in_bounds(y, x)) continue;

						if (los(e_ptr->cy, e_ptr->cx, y, x) &&
							(distance(e_ptr->cy, e_ptr->cx, y, x) == e_ptr->rad))
							cave[y][x].effect = i;
					}
				}
			else if (effects[i].flags & EFF_DIR3)
				for (y = e_ptr->cy; y <= e_ptr->cy + e_ptr->rad; y++)
				{
					for (x = e_ptr->cx; x <= e_ptr->cx + e_ptr->rad; x++)
					{
						if (!in_bounds(y, x)) continue;

						if (los(e_ptr->cy, e_ptr->cx, y, x) &&
							(distance(e_ptr->cy, e_ptr->cx, y, x) == e_ptr->rad))
							cave[y][x].effect = i;
					}
				}
			else
				for (y = e_ptr->cy - e_ptr->rad; y <= e_ptr->cy + e_ptr->rad; y++)
				{
					for (x = e_ptr->cx - e_ptr->rad; x <= e_ptr->cx + e_ptr->rad; x++)
					{
						if (!in_bounds(y, x)) continue;

						/* This is *slow* -- pelpel */
						if (los(e_ptr->cy, e_ptr->cx, y, x) &&
							(distance(e_ptr->cy, e_ptr->cx, y, x) == e_ptr->rad))
							cave[y][x].effect = i;
					}
				}
		}
		/* Creates a "storm" effect*/
		else if (effects[i].flags & EFF_STORM)
		{
			effect_type *e_ptr = &effects[i];
			s32b x, y;

			if (e_ptr->who > 0)
			{
				monster_type *m_ptr = get_monster(e_ptr->who);
				e_ptr->cy = m_ptr->fy;
				e_ptr->cx = m_ptr->fx;
			}
			else
			{
				e_ptr->cy = p_ptr->py;
				e_ptr->cx = p_ptr->px;
			}
			for (y = e_ptr->cy - e_ptr->rad; y <= e_ptr->cy + e_ptr->rad; y++)
			{
				for (x = e_ptr->cx - e_ptr->rad; x <= e_ptr->cx + e_ptr->rad; x++)
				{
					if (!in_bounds(y, x)) continue;

					if (los(e_ptr->cy, e_ptr->cx, y, x) &&
						(distance(e_ptr->cy, e_ptr->cx, y, x) <= e_ptr->rad))
					{
						cave[y][x].effect = i;
						lite_spot(y, x);
					}
				}
			}
		}
	}

	apply_effect(p_ptr->py, p_ptr->px);
}


/* XXX XXX XXX */
bool is_recall = FALSE;


/*
 * Handle certain things once every 10 game turns
 *
 * Note that a single movement in the overhead wilderness mode
 * consumes 132 times as much energy as a normal one...
 */
static void process_world(void)
{
	timer_type *t_ptr;

	s32b i, j;
	s32b y, x;

	dungeon_info_type *d_ptr = &d_info[dungeon_type];

	/*
	 * Every 10 game turns -- which means this section is invoked once
	 * in a player turn under the normal speed, and 132 times in a move
	 * in the reduced map mode.
	 */
	if (turn % 10) return;

	/*
	 * Call the script once every 10 game turns, *always*.  This can slow
	 * things down while in wilderness mode (reduced map), so use only
	 * if really necessary.
	 */
	process_hooks(HOOK_PROCESS_WORLD, "()");

	/*
	 * This hook is only called while not in the wilderness, for
	 * script hooks that aren't needed in the wilderness or would slow
	 * wilderness processing down too much.
	 */
	if (dun_level || !p_ptr->wild_mode)
		process_hooks(HOOK_PROCESS_WORLD_NON_WILD, "(d)", 10);

	/*
	 * This hook is called once every 1000 game turns while in reduced
	 * map mode (1.32 times per player turn at normal speeds).
	 * This lets the script do things while in the wilderness without
	 * slowing everything down, since it won't be invoked 132 times per
	 * player move.
	 */
	if ((turn % 1000 == 0) && !dun_level && p_ptr->wild_mode)
		process_hooks(HOOK_PROCESS_WORLD_WILD, "(d)", 1000);

	/* Handle the timers */
	for (t_ptr = gl_timers; t_ptr != NULL; t_ptr = t_ptr->next)
	{
		if (!t_ptr->enabled) continue;

		t_ptr->countdown--;
		if (!t_ptr->countdown)
		{
			t_ptr->countdown = t_ptr->delay;
			call_lua(t_ptr->callback, "()", "");
		}
	}

	/*** Check the Time and Load ***/

	/*** Attempt timed autosave ***/
	if (autosave_t && autosave_freq)
	{
		if ((turn % ((s32b)autosave_freq * 10)) == 0)
		{
			is_autosave = TRUE;
			msg_print("Autosaving the game...");
			do_cmd_save_game();
			is_autosave = FALSE;
		}
	}


	/*** Handle the wilderness/town (sunshine) ***/

	/* While in town/wilderness and not in the overworld mode */
	if (has_flag(&d_info[dungeon_type], FLAG_SURFACE_LITE) &&
		!p_ptr->wild_mode)
	{
		/* Day breaks */
		if (is_dawn(turn))
		{
			/* Message */
			msg_print("The sun has risen.");

			if (dun_level == 0)
				lite_level(TRUE);
			else
				lite_level(FALSE);
		}

		/* Night falls */
		else if (is_dusk(turn))
		{
			/* Message */
			msg_print("The sun has fallen.");

			darken_level(FALSE);

			/* Update the monsters */
			p_ptr->update |= (PU_MONSTERS);

			/* Redraw map */
			flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

			/* Window stuff */
			flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);
		}
	}

	/* Tell a day passed */
	if ((turn % (10L * DAY)) == 0)
	{
		cmsg_format(TERM_L_GREEN, "Today is %s.", get_date_string(turn));
	}

	/*** Process the monsters ***/

	/* Check for creature generation. */
	if (!p_ptr->wild_mode && !p_ptr->inside_arena && !p_ptr->inside_quest &&
		(d_info[(dun_level) ? dungeon_type : DUNGEON_WILDERNESS].max_m_alloc_chance > 0) &&
		(rand_int(d_info[(dun_level) ? dungeon_type : DUNGEON_WILDERNESS].max_m_alloc_chance) == 0))
	{
		/* Make a new monster */
		if (!has_flag(dungeon_flags, FLAG_NO_NEW_MONSTER))
		{
			(void)alloc_monster(MAX_SIGHT + 5, FALSE);
		}
	}

	p_ptr->did_nothing = FALSE;

	/*** Timeout Various Things ***/

	/* Handle temporary stat drains */
	for (i = 0; i <= STATS_MAX; i++)
	{
		if (get_stat(i, cnt) > 0)
		{
			add_stat(i, cnt, -1);
			if (get_stat(i, cnt) == 0)
			{
				do_res_stat(i, FALSE);
			}
		}
	}

	if (multi_rew)
	{
		multi_rew = FALSE;
	}

	/* Timed mimicry */
	if (get_skill(SKILL_MIMICRY))
	{
		/* Extract the value and the flags */
		u32b value = p_ptr->mimic_extra >> 16;

		u32b att = p_ptr->mimic_extra & 0xFFFF;

		if ((att & CLASS_LEGS) || (att & CLASS_WALL) || (att & CLASS_ARMS))
		{
			value--;

			if (!value)
			{
				if (att & CLASS_LEGS) msg_print("You lose your extra pair of legs.");
				if (att & CLASS_ARMS) msg_print("You lose your extra pair of arms.");
				if (att & CLASS_WALL) msg_print("You lose your affinity for walls.");

				att &= ~(CLASS_ARMS);
				att &= ~(CLASS_LEGS);
				att &= ~(CLASS_WALL);

				if (disturb_state) disturb(0, 0);
			}

			p_ptr->update |= (PU_BODY);
			p_ptr->mimic_extra = att + (value << 16);
		}
	}


	/* Hack - damage done by the dungeon -SC- */
	if ((dun_level != 0) && (d_ptr->d_frequency[0] != 0))
	{
		s32b i, j, k;

		/* Apply damage to every grid in the dungeon */
		for (i = 0; i < 4; i++)
		{
			/* Check the frequency */
			if (d_ptr->d_frequency[i] == 0) continue;

			if (((turn % d_ptr->d_frequency[i]) == 0) &&
			                ((d_ptr->d_side[i] != 0) || (d_ptr->d_dice[i] != 0)))
			{
				for (j = 0; j < cur_hgt - 1; j++)
				{
					for (k = 0; k < cur_wid - 1; k++)
					{
						s32b l, dam = 0;

						if (!has_flag(dungeon_flags, FLAG_DAMAGE_FEAT))
						{
							/* If the grid is empty, skip it */
							if ((flag_used(&cave[j][k].inventory) == 0) &&
							                ((j != p_ptr->py) && (i != p_ptr->px))) continue;
						}

						/* Let's not hurt poor monsters */
						if (cave[j][k].m_idx) continue;

						/* Roll damage */
						for (l = 0; l < d_ptr->d_dice[i]; l++)
						{
							dam += randint(d_ptr->d_side[i]);
						}

						/* Apply damage */
						project( -100, 0, j, k, dam, d_ptr->d_type[i],
						         PROJECT_KILL | PROJECT_ITEM | PROJECT_HIDE | PROJECT_HIDE_BLAST, -1);
					}
				}
			}
		}
	}

	/* Arg cannot breath? */
#if 0 // DGDGDGDG ?? needed here ? could be in teh regen code in the life counter ?
	if (has_flag(dungeon_flags, FLAG_WATER_BREATH) && (!intrinsic(WATER_BREATH)))
	{
		cmsg_print(TERM_L_RED, "You cannot breathe water, you suffocate!");
		take_hit(damroll(3, p_ptr->lev), "suffocating");
	}
	if (has_flag(dungeon_flags, FLAG_NO_BREATH) && (!intrinsic(MAGIC_BREATH)))
	{
		cmsg_print(TERM_L_RED, "There is no air there! You suffocate!");
		take_hit(damroll(3, p_ptr->lev), "suffocating");
	}
#endif

	/*
	 * Every 1500 turns, warn about any Black Breath not gotten from
	 * an equipped object, and stop any resting. -LM-
	 *
	 * It's apparent that someone has halved the frequency... -- pelpel
	 */
	if (((turn % 3000) == 0) && p_ptr->black_breath)
	{
		bool be_silent = FALSE;

		/* check all equipment for the Black Breath flag. */
		for_inventory(p_ptr, o_ptr, INVEN_PACK, INVEN_TOTAL);
		{
			/* No messages if object has the flag, to avoid annoyance. */
			if (has_flag(o_ptr, FLAG_BLACK_BREATH)) be_silent = TRUE;
		}
		end_inventory();

		/* If we are allowed to speak, warn and disturb. */
		if (!be_silent)
		{
			cmsg_print(TERM_L_DARK, "The Black Breath saps your soul!");
			disturb(0, 0);
		}
	}


	/*** Process Light ***/
	for_inventory(p_ptr, o_ptr, INVEN_PACK, INVEN_TOTAL);
	{
		/* Burn some fuel in the current lite */
		if (has_flag(o_ptr, FLAG_FUEL))
		{
			/* Hack -- Use some fuel */
			if (has_flag(o_ptr, FLAG_FUEL_LITE) && (get_flag(o_ptr, FLAG_FUEL) > 0))
			{
				/* Decrease life-span */
				dec_flag(o_ptr, FLAG_FUEL);

				/* Hack -- notice interesting fuel steps */
				if ((get_flag(o_ptr, FLAG_FUEL) < 100) || ((get_flag(o_ptr, FLAG_FUEL) % 100) == 0))
				{
					/* Window stuff */
					flag_bool(&p_ptr->window, FLAG_PW_EQUIP);
				}

				/* Hack -- Special treatment when blind */
				if (intrinsic(BLIND))
				{
					/* Hack -- save some light for later */
					if (get_flag(o_ptr, FLAG_FUEL) == 0) inc_flag(o_ptr, FLAG_FUEL);
				}

				/* The light is now out */
				else if (get_flag(o_ptr, FLAG_FUEL) < 1)
				{
					disturb(0, 0);
					cmsg_print(TERM_YELLOW, "Your light has gone out!");
				}

				/* The light is getting dim */
				else if (get_flag(o_ptr, FLAG_FUEL) < 100)
				{
					if (disturb_minor) disturb(0, 0);
					cmsg_print(TERM_YELLOW, "Your light is growing faint.");
					drop_from_wild();
				}
			}
		}
	}
	end_inventory();

	/* Calculate torch radius */
	p_ptr->update |= (PU_TORCH);


	/*** Process Inventory ***/

	/*
	 * Handle experience draining.  In Oangband, the effect is worse,
	 * especially for high-level characters.  As per Tolkien, hobbits
	 * are resistant.
	 */
	if (p_ptr->black_breath)
	{
		byte chance = 0;
		s32b plev = p_ptr->lev;

		if (has_flag(p_ptr, FLAG_RESIST_BLACK_BREATH)) chance = 2;
		else chance = 5;

		if ((rand_int(100) < chance) && (p_ptr->exp > 0))
		{
			p_ptr->exp -= 1 + plev / 5;
			p_ptr->max_exp -= 1 + plev / 5;
			(void)do_dec_stat(rand_int(6), STAT_DEC_NORMAL);
			check_experience();
		}
	}

	/* Handle experience draining */
	if (intrinsic(DRAIN_EXP))
	{
		if ((rand_int(100) < 10) && (p_ptr->exp > 0))
		{
			p_ptr->exp--;
			p_ptr->max_exp--;
			check_experience();
		}
	}

	/* Process equipment */
	j = 0;
	for_inventory(p_ptr, o_ptr, INVEN_PACK, INVEN_TOTAL);
	{
		/* Auto Curse */
		if (has_flag(o_ptr, FLAG_AUTO_CURSE) && (rand_int(get_flag(o_ptr, FLAG_AUTO_CURSE)) == 0))
		{
			/* The object recurse itself ! */
			o_ptr->ident |= IDENT_CURSED;
		}

		/*
		 * Hack: Uncursed teleporting items (e.g. Dragon Weapons)
		 * can actually be useful!
		 */
		if (has_flag(o_ptr, FLAG_TELEPORT) && (rand_int(100) < 1))
		{
			if ((o_ptr->ident & IDENT_CURSED) && !intrinsic(NO_TELE))
			{
				disturb(0, 0);

				/* Teleport player */
				teleport_player(40);
			}
			else
			{
				if (p_ptr->wild_mode ||
				                (o_ptr->note && strchr(quark_str(o_ptr->note), '.')))
				{
					/* Do nothing */
					/* msg_print("Teleport aborted.") */;
				}
				else if (get_check("Teleport? "))
				{
					disturb(0, 0);
					teleport_player(50);
				}
			}
		}


		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Recharge activatable objects */
		if (o_ptr->timeout > 0)
		{
			/* Recharge */
			o_ptr->timeout--;

			/* Notice changes */
			if (o_ptr->timeout == 0)
			{
				recharged_notice(o_ptr);
				j++;
			}
		}
	}
	end_inventory();

	/* Notice changes */
	if (j)
	{
		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_EQUIP);
	}

	/* Decay pack items */
	j = 0;
	if (use_corpse_decay)
	{
		for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
		{
			/* Decay objects in pack */
			if (decays(o_ptr))
			{
				/* Decay it */
				if (has_flag(o_ptr, FLAG_MONSTER_DECAY))
				{
					if (has_flag(dungeon_flags, FLAG_HOT))
					{
						mod_flag(o_ptr, FLAG_MONSTER_DECAY, -2);
					}
					else if (has_flag(dungeon_flags, FLAG_COLD) && rand_int(2))
					{
						if (magik(50)) dec_flag(o_ptr, FLAG_MONSTER_DECAY);
					}
					else
					{
						dec_flag(o_ptr, FLAG_MONSTER_DECAY);
					}

					/* Notice changes */
					if (get_flag(o_ptr, FLAG_MONSTER_DECAY) <= 0)
					{
						pack_decay(compute_slot(inven_nb, item_nb));
						j++;
					}
				} /* if (get_flag(o_ptr, FLAG_MONSTER_DECAY) != 0) */
			} /* if (decays(o_ptr)) */
		}
		end_inventory();
	}

	/* Notice changes */
	if (j)
	{
		/* Combine pack */
		p_ptr->notice |= (PN_COMBINE);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_INVEN);
	}

	/* Feel the p_ptr->inventory */
	if (dun_level || (!p_ptr->wild_mode))
	{
		sense_inventory();
	}

	/*** Process Objects ***/

	/* Process objects (but not in wilderness mode) */
	if (use_corpse_decay && !p_ptr->wild_mode)
	{
		for (y = 0; y < MAX_HGT; y++)
		{
			for (x = 0; x < MAX_WID; x++)
			{
				object_type *o_ptr;
				vector_type *to_delete = vector_new();

				flags_type *inven = &cave[y][x].inventory;

				/* Not using for_inventory_slot_ro since that's read-only. */
				for_inventory_slot(inven, o_ptr);
				{
					if (decays(o_ptr))
					{
						/* Decays corpses */
						if (get_flag(o_ptr, FLAG_MONSTER_DECAY) != 0)
						{
							if (has_flag(dungeon_flags, FLAG_HOT))
							{
								mod_flag(o_ptr, FLAG_MONSTER_DECAY, -2);
							}
							else if (has_flag(dungeon_flags, FLAG_COLD) && rand_int(2))
							{
								if (magik(50)) dec_flag(o_ptr, FLAG_MONSTER_DECAY);
							}
							else
							{
								dec_flag(o_ptr, FLAG_MONSTER_DECAY);
							}
						}
					
						/* Turn it into a skeleton */
						if (get_flag(o_ptr, FLAG_MONSTER_DECAY) <= 0)
						{
							floor_decay(o_ptr);
							vector_push(to_delete, item_nb);
						}
					}
				}
				end_inventory();

				/* Delete the rotten corpses backwards to keep valid indices */
				while (!vector_is_empty(to_delete))
				{
					s32b idx = vector_pop(to_delete);
					s32b max = flag_max_key(inven);

					flag_remove(inven, idx);

					/* Slide everything down */
					for (i = idx; i < max; i++)
					{
						object_type *j_ptr = flag_get_obj(inven, i + 1);
						flag_remove_zap(inven, i + 1);
						flag_set_obj(inven, i, j_ptr);
					}
				}

				vector_free(to_delete);
			} // for x
		} // for y
	} // if !wild_mode

#if 0 // DGDGDGDG -- ARG ! oups .. gha ...
	for (i = 1; i < o_max; i++)
	{
		/* Access object */
		o_ptr = o_list[i];

		/* Skip dead objects */
		if (!o_ptr->k_idx) continue;

		/* Temporary items are destroyed */
		if (has_flag(o_ptr, FLAG_TEMPORARY))
		{
			o_ptr->timeout--;

			if (o_ptr->timeout <= 0)
			{
//DGDGDGDG				item_increase(i, -99);
//DGDGDGDG				item_optimize(i);

				/* Combine and Reorder pack */
				p_ptr->notice |= (PN_COMBINE | PN_REORDER);
			}
		}
#endif

	/*** Involuntary Movement ***/

	/* Delayed Word-of-Recall */
	if (p_ptr->word_recall)
	{
		/* Can we ? */
		if (process_hooks(HOOK_RECALL, "()", ""))
		{
			p_ptr->word_recall = 0;
		}

		/* No recall. sorry */
		else if (has_flag(dungeon_flags, FLAG_NO_RECALL_OUT))
		{
			cmsg_print(TERM_L_DARK, "You cannot recall from here.");
			p_ptr->word_recall = 0;
		}

		/* I think the 'inside_quest' code belongs here -- pelpel */

		/* Normal WoR */
		else
		{
			/*
			 * HACK: Autosave BEFORE resetting the recall counter (rr9)
			 * The player is yanked up/down as soon as
			 * he loads the autosaved game.
			 */
			if (autosave_l && (p_ptr->word_recall == 1))
			{
				is_autosave = TRUE;
				msg_print("Autosaving the game...");
				do_cmd_save_game();
				is_autosave = FALSE;
			}

			/* Count down towards recall */
			p_ptr->word_recall--;

			/* Activate the recall */
			if (p_ptr->word_recall == 0)
			{
				/* Disturbing! */
				disturb(0, 0);

				/* Determine the level */
				if (p_ptr->inside_quest)
				{
					msg_print("The recall is cancelled by a powerful magic force!");
				}
				else if (dun_level)
				{
					msg_print("You feel yourself yanked upwards!");

					p_ptr->recall_dungeon = dungeon_type;
					dun_level = 0;

					is_recall = TRUE;

					p_ptr->inside_quest = 0;
					p_ptr->leaving = TRUE;
				}
				else
				{
					msg_print("You feel yourself yanked downwards!");

					/* New depth */
					dungeon_type = p_ptr->recall_dungeon;
					dun_level = max_dlv[dungeon_type];
					if (dun_level < 1) dun_level = 1;

					/* Reset player position */
					p_ptr->oldpx = p_ptr->px;
					p_ptr->oldpy = p_ptr->py;

					/* Leaving */
					is_recall = TRUE;

					p_ptr->leaving = TRUE;
					p_ptr->wild_mode = FALSE;
				}

				/* Sound */
				sound(SOUND_TPLEVEL);
			}
		}
	}
}


/*
 * Verify use of "wizard" mode
 */
static bool enter_wizard_mode(void)
{
	/* Ask first time, but not while loading a dead char with the -w option */
	if (!noscore)
	{
		/* Mention effects */
		msg_print("Wizard mode is for debugging and experimenting.");
		msg_print("The game will not be scored if you enter wizard mode.");
		msg_print(NULL);

		/* Verify request */
		if (!get_check("Are you sure you want to enter wizard mode? "))
		{
			return (FALSE);
		}

		/* Mark savefile */
		noscore |= 0x0002;
	}

	/* Success */
	return (TRUE);
}

#ifdef ALLOW_BORG

/*
 * Verify use of "borg" commands
 */
static bool enter_borg_mode(void)
{
	/* Ask first time */
	if (!(noscore & 0x0010))
	{
		/* Mention effects */
		msg_print("The borg commands are for debugging and experimenting.");
		msg_print("The game will not be scored if you use borg commands.");
		msg_print(NULL);

		/* Verify request */
		if (!get_check("Are you sure you want to use borg commands? "))
		{
			return (FALSE);
		}

		/* Mark savefile */
		noscore |= 0x0010;
	}

	/* Success */
	return (TRUE);
}


/*
 * Hack -- Declare the Ben Borg
 */
extern void do_cmd_borg(void);

#endif /* ALLOW_BORG */



/*
 * Parse and execute the current command
 * Give "Warning" on illegal commands.
 *
 * XXX XXX XXX Make some "blocks"
 */
static void process_command(void)
{
	ignore_numlock = 1;
	
#ifdef ALLOW_REPEAT /* TNB */

	/* Handle repeating the last command */
	repeat_check();

#endif /* ALLOW_REPEAT -- TNB */

	/* Process the appropriate hooks */
	if (process_hooks_ret(HOOK_KEYPRESS, "d", "(d)", command_cmd))
	{
		/* The script told us the key is to be used as an unknown key */
		if (process_hooks_return[0].num)
		{
			if (!process_hooks(HOOK_KEYPRESS_UNKNOWN, "(d)", command_cmd))
			{
				prt("Type '?' for help.", 0, 0);
			}
		}

		/*
		 * Hack: ensure last message printed is shown while auto_more
		 * is set.
		 */
		if (auto_more) {
			auto_more = FALSE;
			cmsg_print_last();
			auto_more = TRUE;
		}
		return;
	}

	/* Parse the command */
	switch (command_cmd)
	{
		/* Ignore */
	case ESCAPE:
	case ' ':
	case 0:
		{
			break;
		}

		/* Ignore return */
	case '\r':
		{
			break;
		}

#ifdef ALLOW_QUITTING

	case KTRL('L'):
		{
			quit("CHEATER");
			break;
		}

#endif

#ifdef ALLOW_WIZARD

		/*** Wizard Commands ***/

		/* Toggle Wizard Mode */
	case KTRL('W'):
		{
			if (wizard)
			{
				wizard = FALSE;
				msg_print("Wizard mode off.");
			}
			else if (enter_wizard_mode())
			{
				wizard = TRUE;
				msg_print("Wizard mode on.");
			}

			/* Update monsters */
			p_ptr->update |= (PU_MONSTERS);

			/* Redraw "title" */
			flag_bool(&p_ptr->redraw, FLAG_PR_TITLE);

			break;
		}

		/* Special "debug" commands */
	case KTRL('A'):
		{
			do_cmd_debug();

			break;
		}

#endif /* ALLOW_WIZARD */


		/*** Standard "Movement" Commands ***/

		/* Alter a grid */
	case '+':
		{
			if (p_ptr->control) break;
			if (!p_ptr->wild_mode) do_cmd_alter();
			break;
		}

		/* Move (usually pick up things) */
	case ';':
		{
			if (do_control_walk()) break;

			do_cmd_walk(always_pickup);

			break;
		}

		/* Move (usually do not pick up) */
	case '-':
		{
			if (do_control_walk()) break;

			do_cmd_walk(!always_pickup);

			break;
		}


		/*** Running, Resting, Searching, Staying */

		/* Begin Running -- Arg is Max Distance */
	case '.':
		{
			if (p_ptr->control || p_ptr->wild_mode) break;
			do_cmd_run();
			break;
		}

		/* Stay still (usually pick things up) */
	case ',':
		{
			if (do_control_pickup()) break;
			do_cmd_stay(always_pickup);
			break;
		}

		/* Stay still (usually do not pick up) */
	case 'g':
		{
			if (p_ptr->control) break;
			do_cmd_stay(!always_pickup);
			break;
		}

		/*** Stairs and Doors and Chests and Traps ***/

		/* Enter store */
	case '_':
		{
			if (p_ptr->control) break;
			if (!p_ptr->wild_mode) do_cmd_store();
			break;
		}

		/* Go up staircase */
	case '<':
		{
			if (p_ptr->control) break;
			/* Normal cases */
			if (p_ptr->wild_mode || dun_level || is_quest(dun_level))
			{
				do_cmd_go_up_down(FALSE);
			}
			else if (vanilla_town)
			{
				/* Do nothing */
			}
			else if (intrinsic(HURT_LITE) && is_daylight(turn))
			{
				/* Burn vampires! burn! */
				msg_print("You can't travel during the day!");
			}
			/* TODO: make the above stuff use this hook */
			else if (!process_hooks(HOOK_FORBID_TRAVEL, "()"))
			{
				change_wild_mode();

				/* Update the known wilderness */
				reveal_wilderness_around_player(p_ptr->wilderness_y,
				                                p_ptr->wilderness_x,
				                                0, WILDERNESS_SEE_RADIUS);
			}

			break;
		}

		/* Go down staircase */
	case '>':
		{
			if (p_ptr->control) break;
			/* Normal cases */
			if (!p_ptr->wild_mode)
			{
				do_cmd_go_up_down(TRUE);
			}

			/* Special cases */
			else
			{
				if ((wf_info[wild_map[p_ptr->py][p_ptr->px].feat].entrance >= 1000) ||
				                (wild_map[p_ptr->py][p_ptr->px].entrance > 1000))
				{
					p_ptr->wilderness_x = p_ptr->px;
					p_ptr->wilderness_y = p_ptr->py;
					p_ptr->wild_mode = !p_ptr->wild_mode;
					do_cmd_go_up_down(TRUE);

					if (dun_level == 0)
					{
						p_ptr->wild_mode = !p_ptr->wild_mode;
					}
					else
					{
						change_wild_mode();
					}
				}
				else
				{
					change_wild_mode();
				}
			}

			break;
		}

		/* Open a door or chest */
	case 'o':
		{
			if (p_ptr->control) break;
			if (!p_ptr->wild_mode) do_cmd_open();
			break;
		}

		/* Close a door */
	case 'c':
		{
			if (p_ptr->control) break;
			if (!p_ptr->wild_mode) do_cmd_close();
			break;
		}

		/* Give an item */
	case 'y':
		{
			if (p_ptr->control) break;
			if (!p_ptr->wild_mode) do_cmd_give();
			break;
		}

		/* Chat */
	case 'Y':
		{
			if (p_ptr->control) break;
			if (!p_ptr->wild_mode) do_cmd_chat();
			break;
		}

		/* Disarm a trap or chest */
	case 'D':
		{
			if (p_ptr->control) break;
			if (!p_ptr->wild_mode) do_cmd_disarm();
			break;
		}


		/*** Magic and Prayers ***/

		/* Interact with skills */
	case 'G':
		{
			if (p_ptr->control) break;
			do_cmd_skill();
			break;
		}

		/* Interact with abilities */
/*	case 'N':
		{
			if (p_ptr->control) break;
			do_cmd_ability();
			break;
		}
*/
		/* Browse a book */
	case 'b':
		{
			if (p_ptr->control) break;
			do_cmd_activate_skill(BROWSE_SKILL);
			break;
		}

		/* Cast a spell */
	case 'm':
		{
			if (do_control_magic()) break;

			/* No magic in the overworld map */
			if (p_ptr->wild_mode) break;

			/* Neither in the Arena */
			if (p_ptr->inside_arena)
			{
				msg_print("The arena absorbs all attempted magic!");

				break;
			}
			do_cmd_activate_skill(ACTIVATE_SKILL);
			squeltch_inventory();
			squeltch_grid();
			break;
		}

		/*** Use various objects ***/

		/* Inscribe an object */
	case '{':
		{
			if (p_ptr->control) break;
			do_cmd_inscribe();
			break;
		}

		/* Uninscribe an object */
	case '}':
		{
			if (p_ptr->control) break;
			do_cmd_uninscribe();
			break;
		}

		/*** Looking at Things (nearby or on map) ***/

		/* Full dungeon map */
	case 'M':
		{
			if (!p_ptr->wild_mode) do_cmd_view_map();
			break;
		}

		/* Locate player on map */
	case 'L':
		{
			do_cmd_locate();
			break;
		}

		/* Look around */
	case 'l':
		{
			do_cmd_look();
			break;
		}

		/* Target monster or location */
	case '*':
		{
			if (p_ptr->control) break;
			if (!p_ptr->wild_mode) do_cmd_target();
			break;
		}

		/*** Help and Such ***/

		/* Help */
	case '?':
		{
			do_cmd_help();
			break;
		}

		/* Identify symbol */
	case '/':
		{
			do_cmd_query_symbol();
			break;
		}

		/* Character description */
	case 'C':
		{
			call_lua("character_sheet.show", "()", "");
			break;
		}


		/*** System Commands ***/

		/* Hack -- User interface */
	case '!':
		{
			(void)Term_user(0);
			break;
		}

		/* Single line from a pref file */
	case '"':
		{
			do_cmd_pref();
			break;
		}

		/* Interact with macros */
	case '@':
		{
			do_cmd_macros();
			break;
		}

		/* Interact with visuals */
	case '%':
		{
			do_cmd_visuals();
			break;
		}

		/* Interact with colors */
	case '&':
		{
			do_cmd_colors();
			break;
		}

		/* Interact with options */
	case '=':
		{
			do_cmd_options(NULL);
			break;
		}


		/*** Misc Commands ***/

		/* Take notes */
	case ':':
		{
			do_cmd_note();
			break;
		}

		/* Version info */
	case 'V':
		{
			do_cmd_version();
			break;
		}

		/* Repeat level feeling */
	case KTRL('F'):
		{
			if (!p_ptr->wild_mode)
				do_cmd_feeling();
			break;
		}

		/* Show previous message */
	case KTRL('O'):
		{
			do_cmd_message_one();
			break;
		}

		/* Show previous messages */
	case KTRL('P'):
		{
			do_cmd_messages();
			break;
		}

		/* Redraw the screen */
	case KTRL('R'):
		{
			do_cmd_redraw();
			break;
		}

#ifndef VERIFY_SAVEFILE

		/* Hack -- Save and don't quit */
	case KTRL('S'):
		{
			is_autosave = FALSE;
			do_cmd_save_game();
			break;
		}

#endif /* VERIFY_SAVEFILE */

	case KTRL('T'):
		{
			do_cmd_time();
		}
		break;

		/* Save and quit */
	case KTRL('X'):
		{
			alive = FALSE;

			/* Leaving */
			p_ptr->leaving = TRUE;

			break;
		}

		/* Quit (commit suicide) */
	case 'Q':
		{
			do_cmd_suicide();
			break;
		}

		/* Activate cmovie */
	case '|':
		{
			/* Stop ? */
			if (do_movies == 1)
			{
				do_stop_cmovie();
				msg_print("Cmovie recording stopped.");
			}
			else
			{
				do_start_cmovie();
			}
			break;
		}

		/* Extended command */
	case '#':
		{
			do_cmd_cli();
			break;
		}

		/* Commands only available as extended commands: */

		/* Extended command help. */
	case CMD_CLI_HELP:
		{
			do_cmd_cli_help();
			break;
		}

		/* Game time. */
	case CMD_SHOW_TIME:
		{
			do_cmd_time();
			break;
		}

		/* Check skills. */
	case CMD_SHOW_SKILL:
		{
			do_cmd_skill();
			break;
		}

		/* Check abilities. */
/*	case CMD_SHOW_ABILITIES:
		{
			do_cmd_ability();
			break;
		}
*/
		/* Save a html screenshot. */
	case CMD_DUMP_HTML:
		{
			do_cmd_html_dump();
			break;
		}

		/* Record a macro. */
	case '$':
	case CMD_MACRO:
		{
			do_cmd_macro_recorder();
			break;
		}

		/* Hack -- Unknown command */
	default:
		{
			if (!process_hooks(HOOK_KEYPRESS_UNKNOWN, "(d)", command_cmd))
			{
				prt("Type '?' for help.", 0, 0);
			}
			break;
		}
	}

	/*
	 * Hack: ensure last message printed is shown while auto_more is
	 * set.
	 */
	if (auto_more) {
		auto_more = FALSE;
		cmsg_print_last();
		auto_more = TRUE;
	}

	ignore_numlock = 0;
}




/*
 * Process the player
 *
 * Notice the annoying code to handle "pack overflow", which
 * must come first just in case somebody manages to corrupt
 * the savefiles by clever use of menu commands or something.
 */
void process_player(void)
{
	s32b i, j;

	/*** Apply energy ***/

	/* Give the player some energy */
	p_ptr->energy += TICK_ENERGY;

	/* No turn yet */
	if (p_ptr->energy < 1000) return;


	/*** Check for interupts ***/

	/* Handle "abort" */
	if (!avoid_abort)
	{
		/* Check for "player abort" (semi-efficiently for resting) */
		if (running || command_rep)
		{
			/* Do not wait */
			inkey_scan = TRUE;

			/* Check for a key */
			if (inkey())
			{
				/* Flush input */
				flush();

				/* Disturb */
				disturb(0, 0);

				/* Hack -- Show a Message */
				msg_print("Cancelled.");
			}
		}
	}


	/*** Handle actual user input ***/

	/* Repeat until out of energy */
	while (p_ptr->energy >= 1000)
	{
		/* Notice stuff (if needed) */
		if (p_ptr->notice) notice_stuff();

		/* Update stuff (if needed) */
		if (p_ptr->update) update_stuff();

		/* Redraw stuff (if needed) */
		if (flag_used(&p_ptr->redraw)) redraw_stuff();

		/* Redraw stuff (if needed) */
		if (flag_used(&p_ptr->window)) window_stuff();

		/* Hack -- mark current wilderness location as known */
		if (!p_ptr->wild_mode && dun_level == 0)
			wild_map[p_ptr->wilderness_y][p_ptr->wilderness_x].known = TRUE;


		/* Place the cursor on the player */
		move_cursor_relative(p_ptr->py, p_ptr->px);

		/* Refresh (optional) */
		if (fresh_before) Term_fresh();

		/* Hack -- Pack Overflow */

		if (get_object_split(INVEN_INVEN,inventory_limit(INVEN_INVEN)+1) != NULL)
		{
			s32b item = compute_slot(INVEN_INVEN,inventory_limit(INVEN_INVEN)+1);

			char o_name[80];

			object_type *o_ptr;

			/* Access the slot to be dropped */
			o_ptr = object_dup(get_object_split(INVEN_INVEN,inventory_limit(INVEN_INVEN)+1));

			/* Disturbing */
			disturb(0, 0);

			/* Warning */
			msg_print("Your pack overflows!");

			/* Describe */
			object_desc(o_name, o_ptr, TRUE, 3);

			/* Message */
			msg_format("You drop %s (%c).", o_name, index_to_label(item));

			/* Drop it (carefully) near the player */
			drop_near(o_ptr, 0, p_ptr->py, p_ptr->px);

			/* Modify, Describe, Optimize */
			item_increase(item, -255);
			item_describe(item);
			item_optimize(item);

			/* Notice stuff (if needed) */
			if (p_ptr->notice) notice_stuff();

			/* Update stuff (if needed) */
			if (p_ptr->update) update_stuff();

			/* Redraw stuff (if needed) */
			if (flag_used(&p_ptr->redraw)) redraw_stuff();

			/* Redraw stuff (if needed) */
			if (flag_used(&p_ptr->window)) window_stuff();
		}



		/* Hack -- cancel "lurking browse mode" */
		if (!command_new) command_see = FALSE;


		/* Assume free turn */
		energy_use = 0;


		/* Paralyzed or Knocked Out */
		if (intrinsic(PARALYZED))
		{
			/* Take a turn */
			energy_use = get_player_energy(SPEED_GLOBAL);
		}

		/* Running */
		else if (running)
		{
			/* Take a step */
			run_step(0);
			if (realtime_freq) energy_use = energy_use / realtime_run_factor;

			/*
			 * Commented out because it doesn't make any sense
			 * to require a player holding down direction keys
			 * instead of using running commands when s/he follows
			 * Eru and do the opposite for the other deities -- pelpel
			 */
			/* p_ptr->did_nothing = TRUE; */
		}

		/* Repeated command */
		else if (command_rep)
		{
			/* Count this execution */
			command_rep--;

			/* Redraw the state */
			flag_bool(&p_ptr->redraw, FLAG_PR_STATE);

			/* Redraw stuff */
			redraw_stuff();

			/* Hack -- Assume messages were seen */
			msg_flag = FALSE;

			/* Clear the top line */
			prt("", 0, 0);

			/* Process the command */
			process_command();

			p_ptr->did_nothing = TRUE;
		}

		/* Normal command */
		else
		{
			/* Place the cursor on the player */
			move_cursor_relative(p_ptr->py, p_ptr->px);

			/* Get a command */
			request_command();

			/* Process the command */
			process_command();
		}


		/*** Clean up ***/

		/* Significant */
		if (energy_use)
		{
			process_hooks(HOOK_PLAYER_USED_ENERGY, "()");

			/* Use some energy */
			p_ptr->energy -= energy_use;


			/* Hack -- constant hallucination */
			if (intrinsic(HALLUCINATE)) flag_bool(&p_ptr->redraw, FLAG_PR_MAP);


			/* Shimmer monsters if needed */
			if (!avoid_other && !use_graphics && shimmer_monsters)
			{
				/* Clear the flag */
				shimmer_monsters = FALSE;

				/* Shimmer multi-hued monsters */
				for_flags(&monst_list);
				{
					i = __key;
					monster_type *m_ptr;

					/* Access monster */
					m_ptr = get_monster(i);

					/* Skip dead monsters */
					if (!m_ptr || !m_ptr->r_idx) continue;

					/* Skip non-multi-hued monsters */
					if (!has_flag(m_ptr, FLAG_ATTR_MULTI)) continue;

					/* Reset the flag */
					shimmer_monsters = TRUE;

					/* Redraw regardless */
					lite_spot(m_ptr->fy, m_ptr->fx);
				}
				end_for_flags();
			}

			/* Shimmer objects if needed and requested */
			if (!avoid_other && !avoid_shimmer && !use_graphics &&
			                shimmer_objects)
			{
				/* Clear the flag */
				shimmer_objects = FALSE;

				/* Shimmer multi-hued objects */
#if 0 // DGDGDGDG -- arg too ..
				for (i = 1; i < o_max; i++)
				{
					/* Acquire object -- for speed only base items are allowed to shimmer */
					object_type *o_ptr = o_list[i];

					/* Skip dead or carried objects */
					if ((!o_ptr->k_idx) || (!o_ptr->ix)) continue;

					/* Skip non-multi-hued monsters */
					if (!(has_flag(o_ptr, FLAG_ATTR_MULTI))) continue;

					/* Reset the flag */
					shimmer_objects = TRUE;

					/* Redraw regardless */
					lite_spot(o_ptr->iy, o_ptr->ix);
				}
#endif
			}

			/*
			 * Shimmer features if needed and requested
			 *
			 * Note: this can be unbearably slow when a player chooses
			 * to use a REALLY big screen in levels filled with shallow
			 * water.  I believe this also hurts a lot on multiuser systems.
			 * However fast modern processors are, I/O cannot be made that
			 * fast, and that's why shimmering has been limited to small
			 * number of monsters -- pelpel
			 */
			if (!avoid_other && !avoid_shimmer && !use_graphics &&
			                !running)
			{
				for (j = panel_row_min; j <= panel_row_max; j++)
				{
					for (i = panel_col_min; i <= panel_col_max; i++)
					{
						cave_type *c_ptr = &cave[j][i];
						feature_type *f_ptr;

						/* Apply terrain feature mimics */
						if (c_ptr->mimic)
						{
							f_ptr = &f_info[c_ptr->mimic];
						}
						else
						{
							f_ptr = &f_info[f_info[c_ptr->feat].mimic];
						}

						/* Skip normal features */
						if (!(has_flag(f_ptr, FLAG_ATTR_MULTI))) continue;

						/* Redraw a shimmering spot */
						lite_spot(j, i);
					}
				}
			}


			/* Handle monster detection */
			if (repair_monsters)
			{
				/* Reset the flag */
				repair_monsters = FALSE;

				/* Rotate detection flags */
				for_flags(&monst_list);
				{
					i = __key;
					monster_type *m_ptr;

					/* Access monster */
					m_ptr = get_monster(i);

					/* Skip dead monsters */
					if (!m_ptr->r_idx) continue;

					/* Nice monsters get mean */
					if (m_ptr->mflag & (MFLAG_NICE))
					{
						/* Nice monsters get mean */
						m_ptr->mflag &= ~(MFLAG_NICE);
					}

					/* Handle memorized monsters */
					if (m_ptr->mflag & (MFLAG_MARK))
					{
						/* Maintain detection */
						if (m_ptr->mflag & (MFLAG_SHOW))
						{
							/* Forget flag */
							m_ptr->mflag &= ~(MFLAG_SHOW);

							/* Still need repairs */
							repair_monsters = TRUE;
						}

						/* Remove detection */
						else
						{
							/* Forget flag */
							m_ptr->mflag &= ~(MFLAG_MARK);

							/* Assume invisible */
							m_ptr->ml = FALSE;

							/* Update the monster */
							update_mon(i, FALSE);

							/* Redraw regardless */
							lite_spot(m_ptr->fy, m_ptr->fx);
						}
					}
				}
				end_for_flags();
			}

			/*
			 * Moved from dungeon() -- It'll get called whenever player
			 * spends energy, so that maze isn't incredibly easy for
			 * Sorcerors and alike any longer -- pelpel
			 *
			 * Forget everything when requested hehe I'm *NASTY*
			 */
			if (dun_level && has_flag(dungeon_flags, FLAG_FORGET))
			{
				wiz_dark();
			}
		}


		/* Hack -- notice death */
		if (!alive || death) break;

		/* Handle "leaving" */
		if (p_ptr->leaving) break;
	}
}

/*
 * Run the main game loop
 */
static bool allow_main_loop = FALSE;
static bool main_loop()
{
	if (realtime_freq && !allow_main_loop) return FALSE;
	allow_main_loop = FALSE;

	/* Process the player */
	process_player();

	/* Notice stuff */
	if (p_ptr->notice) notice_stuff();

	/* Update stuff */
	if (p_ptr->update) update_stuff();

	/* Redraw stuff */
	if (flag_used(&p_ptr->redraw)) redraw_stuff();

	/* Redraw stuff */
	if (flag_used(&p_ptr->window)) window_stuff();

	/* Hack -- Hilite the player */
	move_cursor_relative(p_ptr->py, p_ptr->px);

	/* Optional fresh */
	if (fresh_after) Term_fresh();

	/* Hack -- Notice death or departure */
	if (!alive || death)
	{
		allow_main_loop = TRUE;
		return TRUE;
	}

	/* Process spell effects */
	process_effects();

	/* Reset placed monsters array to empty. */
	placed_monsters_num = 0;

	/* Notice stuff */
	if (p_ptr->notice) notice_stuff();

	/* Update stuff */
	if (p_ptr->update) update_stuff();

	/* Redraw stuff */
	if (flag_used(&p_ptr->redraw)) redraw_stuff();

	/* Redraw stuff */
	if (flag_used(&p_ptr->window)) window_stuff();

	/* Hack -- Hilite the player */
	move_cursor_relative(p_ptr->py, p_ptr->px);

	/* Optional fresh */
	if (fresh_after) Term_fresh();

	/* Hack -- Notice death or departure */
	if (!alive || death)
	{
		allow_main_loop = TRUE;
		return TRUE;
	}

	/* Process all of the monsters */
	process_monsters();

	/* Reset placed monsters array to empty. */
	placed_monsters_num = 0;

	/* Notice stuff */
	if (p_ptr->notice) notice_stuff();

	/* Update stuff */
	if (p_ptr->update) update_stuff();

	/* Redraw stuff */
	if (flag_used(&p_ptr->redraw)) redraw_stuff();

	/* Redraw stuff */
	if (flag_used(&p_ptr->window)) window_stuff();

	/* Hack -- Hilite the player */
	move_cursor_relative(p_ptr->py, p_ptr->px);

	/* Optional fresh */
	if (fresh_after) Term_fresh();

	/* Hack -- Notice death or departure */
	if (!alive || death)
	{
		allow_main_loop = TRUE;
		return TRUE;
	}

	/* Process the world */
	process_world();

	/* Reset placed monsters array to empty. */
	placed_monsters_num = 0;

	/* Process the appropriate hooks */
	process_hooks(HOOK_END_TURN, "(d)", is_quest(dun_level));

	/* Reset placed monsters array to empty. */
	placed_monsters_num = 0;

	/* Make it pulsate and live !!!! */
	if (has_flag(dungeon_flags, FLAG_EVOLVE) && dun_level)
	{
		if (!(turn % 10)) evolve_level(TRUE);
	}

	/* Notice stuff */
	if (p_ptr->notice) notice_stuff();

	/* Update stuff */
	if (p_ptr->update) update_stuff();

	/* Redraw stuff */
	if (flag_used(&p_ptr->redraw)) redraw_stuff();

	/* Window stuff */
	if (flag_used(&p_ptr->window)) window_stuff();

	/* Hack -- Hilite the player */
	move_cursor_relative(p_ptr->py, p_ptr->px);

	/* Optional fresh */
	if (fresh_after) Term_fresh();

	/* Hack -- Notice death or departure */
	if (!alive || death)
	{
		allow_main_loop = TRUE;
		return TRUE;
	}

	/* Handle "leaving" */
	if (p_ptr->leaving)
	{
		allow_main_loop = TRUE;
		return TRUE;
	}

	/* Count game turns */
	turn++;
	allow_main_loop = TRUE;
	return FALSE;
}


/*
 * Interact with the current dungeon level.
 *
 * This function will not exit until the level is completed,
 * the user dies, or the game is terminated.
 */
static void dungeon(void)
{
	/* Reset various flags */
	hack_mind = FALSE;

	/* Not leaving */
	p_ptr->leaving = FALSE;

	/* Reset the "command" vars */
	command_cmd = 0;
	command_new = 0;
	command_rep = 0;
	command_arg = 0;
	command_dir = 0;


	/* Cancel the target */
	target_who = 0;

	/* Cancel the health bar */
	health_track(0);


	/* Check visual effects */
	shimmer_monsters = TRUE;
	shimmer_objects = TRUE;
	repair_monsters = TRUE;
	repair_objects = TRUE;


	/* Disturb */
	disturb(1, 0);

	/* Track maximum player level */
	if (p_ptr->max_plv < p_ptr->lev)
	{
		p_ptr->max_plv = p_ptr->lev;
	}

	/* Track maximum dungeon level (if not in quest -KMW-) */
	if ((max_dlv[dungeon_type] < dun_level) && !p_ptr->inside_quest)
	{
		max_dlv[dungeon_type] = dun_level;
	}

	/* Hack - Assume invalid panel */
	panel_row_min = cur_hgt;
	panel_row_max = 0;
	panel_col_min = cur_wid;
	panel_col_max = 0;

	/* Center the panel */
	verify_panel();

	/* Flush messages */
	msg_print(NULL);


	/* Enter "xtra" mode */
	character_xtra = TRUE;

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_ALL);

	/* Redraw dungeon */
	flag_bool(&p_ptr->redraw, FLAG_PR_WIPE);
	flag_bool(&p_ptr->redraw, FLAG_PR_BASIC);
	flag_bool(&p_ptr->redraw, FLAG_PR_EXTRA);

	/* Redraw map */
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

	/* Update stuff */
	p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS | PU_POWERS | PU_SANITY | PU_BODY);

	/* Calculate torch radius */
	p_ptr->update |= (PU_TORCH);

	/* Update stuff */
	update_stuff();

	/* Redraw stuff */
	redraw_stuff();

	/* Redraw stuff */
	window_stuff();

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_FLOW | PU_DISTANCE | PU_MON_LITE);

	/* Update stuff */
	update_stuff();

	/* Redraw stuff */
	redraw_stuff();

	/* Leave "xtra" mode */
	character_xtra = FALSE;

	/* Update stuff */
	p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS | PU_POWERS | PU_BODY);

	/* Combine / Reorder the pack */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Notice stuff */
	notice_stuff();

	/* Update stuff */
	update_stuff();

	/* Redraw stuff */
	redraw_stuff();

	/* Window stuff */
	window_stuff();

	/* Refresh */
	Term_fresh();


	/* Announce (or repeat) the feeling */
	if (dun_level) do_cmd_feeling();


	/* Hack -- notice death or departure */
	if (!alive || death) return;

	/*** Process this dungeon level ***/

	/* Reset the monster generation level */
	monster_level = dun_level;

	/* Reset the object generation level */
	object_level = dun_level;

	hack_mind = TRUE;

	process_hooks(HOOK_LEVEL_LOOP, "()");

	/* Main loop */
	while (TRUE)
	{
		/* Reset placed monsters array to empty. */
		placed_monsters_num = 0;

		/* If we do realtime the "main loop" is called by the scheduler, so here we just tell the UI to process events */
		if (realtime_freq)
		{
			/* Process events (wait for one) */
			allow_main_loop = TRUE;
			Term_xtra(TERM_XTRA_EVENT, TRUE);
			allow_main_loop = FALSE;

			if (p_ptr->leaving || !alive || death) break;
		}
		else
		{
			/* Not realtime, just go on as usual */
			allow_main_loop = TRUE;
			if (main_loop()) break;
		}
	}

	/* Did we leave a dungeon ? */
	if ((dun_level < d_info[dungeon_type].mindepth) && !is_recall)
	{
		dun_level = 0;

		if (d_info[dungeon_type].ix > -1)
		{
			p_ptr->wilderness_x = d_info[dungeon_type].ix;
			p_ptr->wilderness_y = d_info[dungeon_type].iy;
		}

		dungeon_type = DUNGEON_WILDERNESS;

		process_hooks(HOOK_ENTERED_DUNGEON, "(d)", dungeon_type);
	}

	if (dun_level > d_info[dungeon_type].maxdepth)
	{
		dun_level = 0;

		if (d_info[dungeon_type].ox > -1)
		{
			p_ptr->wilderness_x = d_info[dungeon_type].ox;
			p_ptr->wilderness_y = d_info[dungeon_type].oy;
		}

		dungeon_type = DUNGEON_WILDERNESS;

		process_hooks(HOOK_ENTERED_DUNGEON, "(d)", dungeon_type);
	}

	is_recall = FALSE;
}


static void load_pref_from_descriptor(player_descriptor *d_ptr)
{
	char buf[1024];
	s32b  i;

	if ((d_ptr->type == NULL) || (strcmp(d_ptr->type, "base") == 0) ||
		(strcmp(d_ptr->type, "sex") == 0))
		return;

	/* Access the descriptor's pref file */
	sprintf(buf, "%s.prf", d_ptr->title);

	/* Process that file */
	if (process_pref_file(buf) == 0)
		/* No errors, done. */
		return;

	/* Maybe it should be lower case? */
	for (i = 0; i < strlen(buf); i++)
		buf[i] = tolower(buf[i]);

	process_pref_file(buf);
}

/*
 * Load some "user pref files"
 */
static void load_all_pref_files(void)
{
	char buf[1024];
	s32b  i;

	/* Access the "character" pref file */
	sprintf(buf, "%s.prf", player_name);

	/* Process that file */
	process_pref_file(buf);

	/* Load prefs for each descriptor. */
	for (i = 1; i <= flag_max_key(&p_ptr->descriptors); i++)
	{
		s32b d_num = flag_get(&p_ptr->descriptors, i);
		player_descriptor *d_ptr = &descriptor_info[d_num];

		load_pref_from_descriptor(d_ptr);
	}

	/* Process player specific automatizer sets */
	tome_dofile_anywhere(TENGINE_DIR_USER, format("%s.atm", player_name), FALSE, FALSE);
}


/*
 * Setup(or down) realtime mode
 */
#ifdef USE_SDL
static SDL_TimerID __realtime_timer_id = 0;
void realtime_mode(u32b freq)
{
	if (freq)
	{
		/* Do we need to remove the old timer first ? */
		if (realtime_freq) rt_timer_del(__realtime_timer_id);
		__realtime_timer_id = rt_timer_add(freq, main_loop);
	}
	else
	{
		rt_timer_del(__realtime_timer_id);
	}
	realtime_freq = freq;
}
#else
void realtime_mode(u32b freq)
{
}
#endif

/*
 * Actually play a game
 *
 * If the "new_game" parameter is true, then, after loading the
 * savefile, we will commit suicide, if necessary, to allow the
 * player to start a new game.
 */
void play_game(bool new_game)
{
	s32b i, tmp_dun, tmp_d_idx;
	s32b tmp_wy, tmp_wx;

	bool cheat_death = FALSE;
	bool quickstart = FALSE;

	/* Hack -- Character is "icky" */
	character_icky++;


	/* Make sure main term is active */
	Term_activate(angband_term[0]);

	/* Initialise the resize hook XXX XXX XXX */
	angband_term[0]->resize_hook = resize_map;

	/* XXX XXX XXX hardcoded number of terms */
	for (i = 1; i < 8; i++)
	{
		if (angband_term[i])
		{
			/* Add redraw hook */
			angband_term[i]->resize_hook = resize_window;
		}
	}


	/* Hack -- turn off the cursor */
	(void)Term_set_cursor(0);

	/* Character list */
	if (!new_game && !no_begin_screen) new_game = begin_screen(&quickstart);
	if (force_new_game) new_game = TRUE;
	no_begin_screen = FALSE;

	/* Attempt to load */
	if (!load_player(quickstart))
	{
		/* Oops */
		quit("broken savefile");
	}

	/* Nothing loaded */
	if (!character_loaded)
		/* Make new player */
		new_game = TRUE;
	else
	{
		s32b i;

		/* Init new skills to their defaults */
		for (i = old_max_s_idx; i < max_s_idx; i++)
		{
			s32b value = 0, mod = 0;

			compute_skills(&value, &mod, i);

			init_skill(value, mod, i);
		}
	}

#if 1

	/* Process old character */
	if (!new_game)
	{
		/* Process the player name */
		process_player_name(FALSE);
	}

#endif

	/* Init the RNG */
	if (Rand_quick || quickstart)
	{
		u32b seed;

		/* Basic seed */
		seed = (time(NULL));

#ifdef SET_UID

		/* Mutate the seed on Unix machines */
		seed = ((seed >> 3) * (getpid() << 1));

#endif

		/* Use the complex RNG */
		Rand_quick = FALSE;

		/* Seed the "complex" RNG */
		Rand_state_init(seed);
	}
	init_gen_rand((u32b)time(NULL));

	/* Roll new character */
	if (new_game)
	{
		s32b ret;

		/* Are we authorized to create new chars? */
		call_lua("get_module_info", "(s)", "d", "allow_birth", &ret);

		if (!ret)
		{
			msg_box("Sorry, this module does not allow character creation.", -1, -1);

			/* Close stuff */
			close_game();

			/* Quit */
			quit(NULL);
		}
	}

	if (new_game || quickstart)
	{
		process_hooks(HOOK_INIT, "(b)", new_game);

		/* The dungeon is not ready */
		character_dungeon = FALSE;

		/* Hack -- seed for flavors */
		seed_flavor = rand_int(0x10000000);

		/* Hack -- seed for town layout */
		seed_town = rand_int(0x10000000);

		player_birth(new_game);

		/* Start in town, or not */
		dun_level = 0;
		p_ptr->inside_quest = 0;
		p_ptr->inside_arena = 0;

		/* Hack -- enter the world */
		turn = 1;

		process_hooks(HOOK_INIT_DONE, "(b)", new_game);
	}

	if (!new_game)
		/* We must ensure the player descriptor is correctly computed */
		call_lua("birth.compute_final_descriptor", "()", "");

	/* Flash a message */
	prt("Please wait...", 0, 0);

	/* Flush the message */
	Term_fresh();

	/* Hack -- Enter wizard mode */
	if (arg_wizard && enter_wizard_mode()) wizard = TRUE;

	/* Initialize the flavours */
	call_lua("flavour.init", "()", "");

	/* Reset the visual mappings */
	reset_visuals();

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_ALL);

	/* Window stuff */
	window_stuff();

	/* load user file */
	process_pref_file("user.prf");

	/* Load the "pref" files */
	load_all_pref_files();

	/* Set or clear "rogue_like_commands" if requested */
	if (arg_force_original) rogue_like_commands = FALSE;
	if (arg_force_roguelike) rogue_like_commands = TRUE;

	/* Initialize hooks */
	init_hooks();
	ingame_help(p_ptr->help.enabled);

	/* React to changes */
	Term_xtra(TERM_XTRA_REACT, 0);

	/* Mega hack, prevent lots of bugs */
	if ((p_ptr->px == 0) || (p_ptr->py == 0))
	{
		p_ptr->px = 1;
		p_ptr->py = 1;
	};

	/* Hack - if note file exists, load it */
	if (!new_game && take_notes)
	{
		add_note_type(NOTE_ENTER_DUNGEON);
	}

	/* Ok tell the scripts that the game is about to start */
	process_hooks(HOOK_GAME_START, "(b)", new_game);

	/* Generate a dungeon level if needed */
	if (!character_dungeon) generate_cave();

	/* Character is now "complete" */
	character_generated = TRUE;


	/* Hack -- Character is no longer "icky" */
	character_icky--;


	/* Start game */
	alive = TRUE;

	/* Should we use old colors */
	if (autoload_old_colors)
	{
		process_pref_file("422color.prf");
	}

	if (new_game) save_player_quickstart();

#ifdef USE_SDL
	if (!strcmp(ANGBAND_SYS, "sdl"))
	{
		term *old = Term;

		/* show the proper terms for the selected module */
		for (i = 0; i < max_terms; i++)
		{
			if (flag_used(&window_flag[i]) > 0)
			{
				/* Activate */
				Term_activate(angband_term[i]);

				/* Erase */
				Term_clear();

				/* Refresh */
				Term_fresh();

				/* show the term */
				data[i].vp.visible = 1;
				ext_term_init_sdl(&data[i].t);
			}
		}

		/* Restore */
		Term_activate(old);

		do_cmd_redraw();

		init_complete = TRUE;
		refresh_timer_id = sdl_create_timer(refresh_rate, &vp_refresh_callback, NULL);
	}
#endif

//	realtime_mode(50);

	/* Process */
	while (TRUE)
	{
		/* Reinit ickyness, just in case */
		character_icky = 0;

		/* Save the level */
		old_dungeon_type = dungeon_type;
		old_dun_level = dun_level;
		p_ptr->old_wild_mode = p_ptr->wild_mode;
		p_ptr->old_wilderness_y = p_ptr->wilderness_y;
		p_ptr->old_wilderness_x = p_ptr->wilderness_x;

		/* Update monster list window */
		flag_bool(&p_ptr->window, FLAG_PW_M_LIST);

#ifdef SAVE_HACK

		/* Process the level */
		if (!save_hack)
		{
			dungeon();
		}
		else
		{
			generate_cave();
		}

		save_hack = FALSE;

		p_ptr->leaving = TRUE;

#else

		/* Process the level */
		dungeon();

#endif

		/* Save the current level if in a persistent level */
		tmp_dun = dun_level;
		tmp_d_idx = dungeon_type;
		tmp_wy = p_ptr->wilderness_y;
		tmp_wx = p_ptr->wilderness_x;
		dungeon_type = old_dungeon_type;
		dun_level = old_dun_level;
		p_ptr->wilderness_y = p_ptr->old_wilderness_y;
		p_ptr->wilderness_x = p_ptr->old_wilderness_x;
		if (!p_ptr->old_wild_mode) save_dungeon(turn);
		dun_level = tmp_dun;
		dungeon_type = tmp_d_idx;
		p_ptr->wilderness_y = tmp_wy;
		p_ptr->wilderness_x = tmp_wx;

		/* Notice stuff */
		if (p_ptr->notice) notice_stuff();

		/* Update stuff */
		if (p_ptr->update) update_stuff();

		/* Redraw stuff */
		if (flag_used(&p_ptr->redraw)) redraw_stuff();

		/* Window stuff */
		if (flag_used(&p_ptr->window)) window_stuff();

		/* Cancel the target */
		target_who = 0;

		/* Cancel the health bar */
		health_track(0);


		/* Forget the lite */
		forget_mon_lite();

		/* Forget the view */
		forget_view();

		/* Handle "quit and save" */
		if (!alive && !death) break;

		/* XXX XXX XXX */
		msg_print(NULL);

		/* Accidental Death */
		if (alive && death)
		{
			cheat_death = FALSE;

			/* Can we die ? please let us die ! */
			if (process_hooks(HOOK_DIE_PRE, "()"))
			{
				cheat_death = TRUE;
			}

			/* Blood of life */
			else if (p_ptr->allow_one_death > 0)
			{
				cheat_death = TRUE;

				/* Lose one extra life */
				p_ptr->allow_one_death--;

				cmsg_print(TERM_L_GREEN,
					"You have been saved by the Blood of Life!");
				msg_print(NULL);
			}

			/* Cheat death option */
			else if ((wizard || cheat_live) && !get_check("Die? "))
			{
				cheat_death = TRUE;

				/* Mark social class, reset age, if needed */
				if (p_ptr->sc) p_ptr->sc = p_ptr->age = 0;

				/* Increase age */
				p_ptr->age++;

				/* Mark savefile */
				noscore |= 0x0001;
				msg_print("You invoke wizard mode and cheat death.");
				msg_print(NULL);
			}

			if (cheat_death)
			{
				/* Restore the winner status */
				total_winner = has_won;

				/* One more life spent */
				p_ptr->lives++;

				/* Hack -- Healing */
				call_lua("timed_effect.reset", "()", "");
				call_lua("counter.reset", "()", "");

				/* accounting for a new ailment. -LM- */
				p_ptr->black_breath = FALSE;

				/* Hack -- don't go to undead form */
				p_ptr->necro_extra &= ~CLASS_UNDEAD;

				/* Hack -- cancel recall */
				if (p_ptr->word_recall)
				{
					/* Message */
					msg_print("A tension leaves the air around you...");
					msg_print(NULL);

					/* Hack -- Prevent recall */
					p_ptr->word_recall = 0;
				}

				/* Note cause of death XXX XXX XXX */
				(void)strcpy(died_from, "Cheating death");

				/* Do not die */
				death = FALSE;

				/* New depth -KMW- */
				/* dun_level = 0; */
				p_ptr->inside_arena = 0;
				leaving_quest = 0;
				p_ptr->inside_quest = 0;

				/* Leaving */
				p_ptr->leaving = TRUE;

				/*
				 * Death might have been caused by something in one of
				 * the PROCESS_WORLD hooks.  If so, and if we leave
				 * the turn counter unchanged, the same hook(s) will
				 * be called again, and we might die again.  Or
				 * again and again and again, if we're using wizard
				 * mode to try to cheat death.  So, increment the
				 * turn counter when cheating death.
				 */
				turn++;
			}
		}

		/* Handle "death" */
		if (death)
		{
			process_hooks(HOOK_DIE_POST, "()");
			break;
		}

		/* Mega hack */
		if (dun_level) p_ptr->wild_mode = FALSE;

		/* Make a new level */
		process_hooks(HOOK_NEW_LEVEL, "(d)", is_quest(dun_level));
		generate_cave();

		/* Wipe the get_item results list */
		call_lua("__core_objects.clean_get_item", "()", "");
	}

	/* Close stuff */
	close_game();

	/* Quit */
	quit(NULL);
}

