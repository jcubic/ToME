/* File: monster3.c */

/* Purpose: Monsters AI */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/*
 ****************** Factions code *********************
 */

/*
 * Sets friend_faction to be percent% friendly toward faction
 * >0 means friend, =0 means neutral, <0 means ennemy
 */
void set_faction_friendliness(u32b faction, u32b friend_faction, s32b percent)
{
	flags_type *faction_table;

	/* Get the factions table, make if unexisting */
	if (!flag_exists(&factions, faction))
	{
		faction_table = flag_new(2);
		flag_set_flags(&factions, faction, faction_table);
	}
	else
		faction_table = flag_get_flags(&factions, faction);

	flag_set(faction_table, friend_faction, percent);
}

/*
 * Gets friend_faction friendly rating friendly toward faction
 * >0 means friend, =0 means neutral, <0 means ennemy
 */
s32b get_faction_friendliness(u32b faction, u32b friend_faction)
{
	flags_type *faction_table;

	/* We love each others !*/
	if (faction == friend_faction) return 100;

	/* Get the factions table, make if unexisting */
	if (!flag_exists(&factions, faction))
	{
		faction_table = flag_new(2);
		flag_set_flags(&factions, faction, faction_table);
	}
	else
		faction_table = flag_get_flags(&factions, faction);

	/* If not present defaults to 0(neutral) */
	return flag_get(faction_table, friend_faction);
}

/*
 * Attempt to change a monster's faction.
 */
bool change_monst_faction(monster_type *m_ptr, u32b new_faction)
{
	u32b old_faction = m_ptr->faction;

	/* No change to do. */
	if (old_faction == new_faction)
		return TRUE;

	if (process_hooks(HOOK_MON_CHANGE_FACTION_PRE, "(M,d,d)",
					  m_ptr, old_faction, new_faction))
		return FALSE;

	m_ptr->faction = new_faction;

	process_hooks(HOOK_MON_CHANGE_FACTION_POST, "(M,d,d)",
				  m_ptr, old_faction, new_faction);

	return TRUE;
}

/*
 * Find an appropriate hostile faction for the given monster.
 */
u32b get_hostile_faction(monster_type *m_ptr, u32b hostile_to,
						 u32b default_faction)
{
	u32b          faction = m_ptr->faction;
	monster_race *r_ptr   = &r_info[m_ptr->r_idx];
	monster_ego  *re_ptr  = &re_info[m_ptr->ego];

	if (get_faction_friendliness(hostile_to, faction) < 0)
		return faction;

	if (has_flag(m_ptr, FLAG_FACTION))
	{
		faction = get_flag(m_ptr, FLAG_FACTION);

		if (get_faction_friendliness(hostile_to, faction) < 0)
			return faction;
	}

	if (m_ptr->ego != 0 && has_flag(r_ptr, FLAG_FACTION))
	{
		faction = get_flag(re_ptr, FLAG_FACTION);

		if (get_faction_friendliness(hostile_to, faction) < 0)
			return faction;
	}

	if (has_flag(r_ptr, FLAG_FACTION))
	{
		faction = get_flag(r_ptr, FLAG_FACTION);

		if (get_faction_friendliness(hostile_to, faction) < 0)
			return faction;
	}

	return default_faction;
}

/*
 * Is the monster in friendly state(pet, friend, ..) toward the player
 * -1 = enemy, 0 = neutral, 1 = friend
 * Simply uses factions to figure it out
 */
s32b is_friend(monster_type *m_ptr)
{
	s32b val = get_faction_friendliness(m_ptr->faction, FACTION_PLAYER);

	if (val < 0) return -1;
	else if (val > 0) return 1;
	else return 0;
}

/* Should they attack each others */
bool is_enemy(monster_type *m_ptr, monster_type *t_ptr)
{
	return (get_faction_friendliness(m_ptr->faction, t_ptr->faction) < 0) ? TRUE : FALSE;
}

/* Player and monster swap places */
bool player_monster_swap(monster_type *m_ptr)
{
	char m_name[80];
	cave_type *c_ptr;

	if (!m_ptr) return FALSE;

	if (has_flag(m_ptr, FLAG_NO_PUSHBACK)) return FALSE;

	c_ptr = &cave[m_ptr->fy][m_ptr->fx];

	m_ptr->csleep = 0;

	/* Extract monster name (or "it") */
	monster_desc(m_name, m_ptr, 0);

	/* Auto-Recall if possible and visible */
	if (m_ptr->ml) monster_race_track(m_ptr->r_idx, m_ptr->ego);

	/* Track a new monster */
	if (m_ptr->ml) health_track(c_ptr->m_idx);

	/* displace? */
	if (cave_floor_bold(p_ptr->py, p_ptr->px) ||
		monst_can_pass_square(m_ptr, p_ptr->py, p_ptr->px, NULL))
	{
		msg_format("You push past %s.", m_name);
		m_ptr->fy = p_ptr->py;
		m_ptr->fx = p_ptr->px;
		cave[p_ptr->py][p_ptr->px].m_idx = c_ptr->m_idx;
		c_ptr->m_idx = 0;
		update_mon(cave[p_ptr->py][p_ptr->px].m_idx, TRUE);
		return TRUE;
	}
	else
	{
		msg_format("%^s is in your way!", m_name);
		energy_use = 0;
		return FALSE;
	}
}

void talk_to_monster(s32b m_idx)
{
	char m_name[100];

	/* Extract monster name (or "it") */
	monster_desc(m_name, get_monster(m_idx), 0);

	/* Process hook if there are any */
	if (!process_hooks(HOOK_CHAT, "(d)", m_idx))
	{
		msg_print("The monster does not want to chat.");
	}
}

/* Multiply !! */
bool ai_multiply(s32b m_idx)
{
	monster_type *m_ptr = get_monster(m_idx);
	s32b k, y, x, oy = m_ptr->fy, ox = m_ptr->fx;
	bool is_frien = (is_friend(m_ptr) > 0);

	/* Count the adjacent monsters */
	for (k = 0, y = oy - 1; y <= oy + 1; y++)
	{
		for (x = ox - 1; x <= ox + 1; x++)
		{
			if (cave[y][x].m_idx) k++;
		}
	}

	if (is_friend(m_ptr) > 0)
	{
		is_frien = TRUE;
	}
	else
	{
		is_frien = FALSE;
	}

	/* Hack -- multiply slower in crowded areas */
	if ((k < 4) && (!k || !rand_int(k * MON_MULT_ADJ)))
	{
		/* Try to multiply */
		if (multiply_monster(m_idx, (is_frien), FALSE))
		{
			/* Take note if visible */
			if (m_ptr->ml)
				monstmem_add(RT_MISC, FLAG_MULTIPLY, m_ptr, 0);

			/* Multiplying takes energy */
			return TRUE;
		}
	}
	return FALSE;
}

/* Possessor incarnates */
bool ai_possessor(s32b m_idx, s32b o_idx)
{
#if 0 // DGDGDGDG
	object_type *o_ptr = o_list[o_idx];
	monster_type *m_ptr = get_monster(m_idx);
	s32b r_idx = m_ptr->r_idx, r2_idx = get_flag(o_ptr, FLAG_MONSTER_IDX);
	s32b i;
	monster_race *r_ptr = &r_info[r2_idx];
	char m_name[80], m_name2[80];

	monster_desc(m_name, m_ptr, 0x00);
	monster_race_desc(m_name2, r2_idx, 0);

	if (m_ptr->ml) msg_format("%^s incarnates into a %s!", m_name, m_name2);

	/* Remove the old one */
	delete_object_idx(o_idx);

	m_ptr->r_idx = r2_idx;
	m_ptr->ego = 0;

	/* No "damage" yet */
	m_ptr->stunned = 0;
	m_ptr->confused = 0;
	m_ptr->monfear = 0;

	/* No target yet */
	m_ptr->target = -1;

	/* Assume no sleeping */
	m_ptr->csleep = 0;

	/* Assign maximal hitpoints */
	if (has_flag(r_ptr, FLAG_FORCE_MAXHP))
	{
		m_ptr->maxhp = maxroll(r_ptr->hdice, r_ptr->hside);
	}
	else
	{
		m_ptr->maxhp = damroll(r_ptr->hdice, r_ptr->hside);
	}

	/* And start out fully healthy */
	m_ptr->hp = m_ptr->maxhp;

	/* Some basic info */
	for (i = 0; i < 4; i++)
	{
		m_ptr->blow[i].method = r_ptr->blow[i].method;
		m_ptr->blow[i].effect = r_ptr->blow[i].effect;
		m_ptr->blow[i].d_dice = r_ptr->blow[i].d_dice;
		m_ptr->blow[i].d_side = r_ptr->blow[i].d_side;
	}
	m_ptr->ac = r_ptr->ac;
	m_ptr->level = r_ptr->level;
	m_ptr->speed = r_ptr->speed;
	m_ptr->exp = MONSTER_EXP(m_ptr->level);

	/* Extract the monster base speed */
	m_ptr->mspeed = m_ptr->speed;

	m_ptr->energy = 0;

	/* Hack -- Count the number of "reproducers" */
	if (has_flag(r_ptr, FLAG_MULTIPLY)) num_repro++;

	/* Hack -- Notice new multi-hued monsters */
	if (has_flag(r_ptr, FLAG_ATTR_MULTI)) shimmer_monsters = TRUE;

	/* Hack -- Count the monsters on the level */
	r_ptr->cur_num++;
	r_info[r_idx].cur_num--;

	m_ptr->possessor = r_idx;

	/* Update the monster */
	update_mon(m_idx, TRUE);
#endif
	return TRUE;
}

void ai_deincarnate(s32b m_idx)
{
#if 0 // DGDGDGDG
	monster_type *m_ptr = get_monster(m_idx);
	s32b r2_idx = m_ptr->possessor, r_idx = m_ptr->r_idx;
	monster_race *r_ptr = &r_info[r2_idx];
	s32b i;
	char m_name[80];

	monster_desc(m_name, m_ptr, 0x04);

	if (m_ptr->ml) msg_format("The soul of %s deincarnates!", m_name);

	m_ptr->r_idx = r2_idx;
	m_ptr->ego = 0;

	/* No "damage" yet */
	m_ptr->stunned = 0;
	m_ptr->confused = 0;
	m_ptr->monfear = 0;

	/* No target yet */
	m_ptr->target = -1;

	/* Assume no sleeping */
	m_ptr->csleep = 0;

	/* Assign maximal hitpoints */
	if (has_flag(r_ptr, FLAG_FORCE_MAXHP))
	{
		m_ptr->maxhp = maxroll(r_ptr->hdice, r_ptr->hside);
	}
	else
	{
		m_ptr->maxhp = damroll(r_ptr->hdice, r_ptr->hside);
	}

	/* And start out fully healthy */
	m_ptr->hp = m_ptr->maxhp;

	/* Some basic info */
	for (i = 0; i < 4; i++)
	{
		m_ptr->blow[i].method = r_ptr->blow[i].method;
		m_ptr->blow[i].effect = r_ptr->blow[i].effect;
		m_ptr->blow[i].d_dice = r_ptr->blow[i].d_dice;
		m_ptr->blow[i].d_side = r_ptr->blow[i].d_side;
	}
	m_ptr->ac = r_ptr->ac;
	m_ptr->level = r_ptr->level;
	m_ptr->speed = r_ptr->speed;
	m_ptr->exp = MONSTER_EXP(m_ptr->level);

	/* Extract the monster base speed */
	m_ptr->mspeed = m_ptr->speed;

	m_ptr->energy = 0;

	/* Hack -- Count the number of "reproducers" */
	if (has_flag(r_ptr, FLAG_MULTIPLY)) num_repro++;

	/* Hack -- Notice new multi-hued monsters */
	if (has_flag(r_ptr, FLAG_ATTR_MULTI)) shimmer_monsters = TRUE;

	/* Hack -- Count the monsters on the level */
	r_ptr->cur_num++;
	r_info[r_idx].cur_num--;

	m_ptr->possessor = 0;

	/* Update the monster */
	update_mon(m_idx, TRUE);
#endif
}

/* Returns if a new companion is allowed */
bool can_create_companion(void)
{
	s32b i, mcnt = 0;

	for_flags(&monst_list);
	{
		i = __key;
		/* Access the monster */
		monster_type *m_ptr = get_monster(i);

		/* Ignore "dead" monsters */
		if (!m_ptr->r_idx) continue;

		if ((m_ptr->faction == FACTION_PLAYER) && has_flag(m_ptr, FLAG_PERMANENT)) mcnt++;
	}
	end_for_flags();

	if (mcnt < (1 + get_skill_scale(SKILL_LORE, 6))) return (TRUE);
	else return (FALSE);
}


/* Player controlled monsters */
bool do_control_walk(void)
{
	/* Get a "repeated" direction */
	if (p_ptr->control)
	{
		s32b dir;

		if (get_rep_dir(&dir))
		{
			/* Take a turn */
			energy_use = get_player_energy(SPEED_WALK);

			/* Actually move the monster */
			p_ptr->control_dir = dir;
		}
		return TRUE;
	}
	else
		return FALSE;
}

bool do_control_inven(void)
{
	if (!p_ptr->control) return FALSE;
	screen_save();
	prt("Carried items", 0, 0);
	// DGDGDGDG	show_monster_inven(p_ptr->control, monst_list);
	inkey();
	screen_load();
	return TRUE;
}

bool do_control_pickup(void)
{
	monster_type *m_ptr = get_monster(p_ptr->control);
	cave_type *c_ptr;
	bool done = FALSE;

	if (!p_ptr->control) return FALSE;

	/* Scan all objects in the grid */
	c_ptr = &cave[m_ptr->fy][m_ptr->fx];
#if 0 //DGDGDGDG
	for_inventory_slot(&c_ptr->inventory, o_ptr);
	{
		/* Skip gold */
		if (has_flag(o_ptr, FLAG_GOLD_VALUE) continue;

		/* Excise the object */
		excise_object_idx(this_o_idx);

		/* Forget mark */
		o_ptr->marked = FALSE;

		/* Forget location */
		o_ptr->iy = o_ptr->ix = 0;

		/* Memorize monster */
		o_ptr->held_m_idx = p_ptr->control;

		/* Build a stack */
		o_ptr->next_o_idx = m_ptr->hold_o_idx;

		/* Carry object */
		m_ptr->hold_o_idx = this_o_idx;
		done = TRUE;
	}
#endif
	if (done) msg_print("You pick up all objects on the floor.");
	return TRUE;
}

bool do_control_drop(void)
{
	monster_type *m_ptr = get_monster(p_ptr->control);

	if (!p_ptr->control) return FALSE;
	monster_drop_carried_objects(m_ptr);
	return TRUE;
}

bool do_control_magic(void)
{
	s32b ret;

	call_lua("control_monster_force_casting", "()", "d", &ret);
	return ret;
}

/* Finds the controlled monster and "reconnect" to it */
bool do_control_reconnect()
{
	s32b i;

	for_flags(&monst_list);
	{
		i = __key;
		/* Access the monster */
		monster_type *m_ptr = get_monster(i);

		/* Ignore "dead" monsters */
		if (!m_ptr->r_idx) continue;

		if (m_ptr->mflag & MFLAG_CONTROL)
		{
			p_ptr->control = i;
			return TRUE;
		}
	}
	end_for_flags();
	return FALSE;
}

/*
 * Turns a simple pet into a faithful companion
 */
void do_cmd_companion()
{
	monster_type *m_ptr;
	s32b ii, jj;

	if (!can_create_companion())
	{
		msg_print("You cannot get anymore companion.");
		return;
	}

	if (!tgt_pt(&ii, &jj))
	{
		msg_print("You must target a pet.");
		return;
	}

	if (cave[jj][ii].m_idx)
	{
		char m_name[100];

		m_ptr = get_monster(cave[jj][ii].m_idx);

		monster_desc(m_name, m_ptr, 0);
		if (m_ptr->faction == FACTION_PLAYER)
		{
			bool_flag(m_ptr, FLAG_PERMANENT);
			msg_format("%^s agrees to follow you.", m_name);
		}
		else
		{
			msg_format("%^s is not in your faction!", m_name);
		}
	}
	else
		msg_print("You must target a monster of your own faction.");
}

/*
 * Find a monster index based on it's race/sval
 */
s16b lookup_race(char d_char, s16b sval)
{
	s32b k;

	/* Look for it */
	for (k = 1; k < max_r_idx; k++)
	{
		monster_race *r_ptr = &r_info[k];

		/* Found a match */
		if ((r_ptr->d_char == d_char) && (r_ptr->sval == sval)) return (k);
	}

	/* Oops */
	if (wizard) msg_format("No race (%c,%d)", d_char, sval);

	/* Oops */
	return (0);
}

/*
 ********************** Monster_blow things ***********************
 */
monster_blow *monster_blow_new()
{
	monster_blow *blow;

	MAKE(blow, monster_blow);

	return blow;
}
void monster_blow_del(monster_blow *blow)
{
	FREE(blow, monster_blow);
}

monster_blow *monster_blow_dup(monster_blow *blow)
{
	monster_blow *blow2 = monster_blow_new();

	*blow2 = *blow;

	return blow2;
}

void monster_blow_loadsave(byte type, monster_blow **bblow, s32b flag)
{
	monster_blow *blow = *bblow;
	do_byte(&blow->method, flag);
	do_byte(&blow->effect, flag);
	do_byte(&blow->d_dice, flag);
	do_byte(&blow->d_side, flag);
}

/*
 ********************** Monster_type flags things ***********************
 */
monster_type *monster_type_new()
{
	monster_type *m_ptr;

	MAKE(m_ptr, monster_type);

	return m_ptr;
}
void monster_type_del(monster_type *m_ptr)
{
	delete_monster_type(m_ptr, TRUE);
}

monster_type *monster_type_dup(monster_type *m_ptr)
{
	monster_type *m_ptr2 = monster_type_new();

	monster_type_copy(m_ptr2, m_ptr);

	return m_ptr2;
}

void monster_type_copy(monster_type *dest, monster_type*src)
{
	*dest = *src;
	flag_init(&dest->blow, 2);
	flag_copy(&dest->blow, &src->blow);

	flag_init(&dest->inventory, 2);
	flag_copy(&dest->inventory, &src->inventory);

	flag_init(&dest->flags, 2);
	flag_copy(&dest->flags, &src->flags);

	flag_init(&dest->spells, 2);
	flag_copy(&dest->spells, &src->spells);

	flag_init(&dest->ai_states, 2);
	flag_copy(&dest->ai_states, &src->ai_states);

	if (dest->sr_ptr != NULL)
		cmsg_print(TERM_VIOLET, "*ERROR*: monster_type_dup called with a sr_ptr! Contact the maintainer.");
	if (dest->mind != NULL)
		cmsg_print(TERM_VIOLET, "*ERROR*: monster_type_dup called with a mind! Contact the maintainer.");
}


/*
 **************************** Monster AI **********************
 */
void set_monster_ai(monster_type *m_ptr, u32b ai)
{
	/*
	 * If we dont have a correct AI state for this new AI, make a default one
	 * NOTE: Really it's betetr to init the states with the correct parameters
	 */
//	if (!flag_exists(&m_ptr->ai_states, ai))
	{
		call_lua("ai.init", "(M,d)", "", m_ptr, ai);
	}

	m_ptr->ai = ai;
}
