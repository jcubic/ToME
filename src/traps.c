/* File: traps.c */

/* Purpose: handle traps */

/*
 * Copyright (c) 2003 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

void activate_trap_door(s32b y, s32b x)
{
}

/* helper function to activate a trap from a monster */
bool mon_hit_trap(s32b m_idx, s32b y, s32b x)
{
	monster_type *m_ptr = get_monster(m_idx);
	if (get_num_location_traps(y, x, FALSE, m_ptr->faction,
							   FALSE, FALSE) == 0)
		return FALSE;

	return activate_location_traps(m_idx, y, x, FALSE);
}

bool find_one_trap(flags_type* traps)
{
	s32b ret;

	call_lua("traps.find_one_trap", "(F)", "d",
		 traps, &ret);

	return (bool) ret;
}

bool find_one_location_trap(s32b y, s32b x)
{
	s32b ret;

	call_lua("traps.find_one_location_trap", "(d,d)", "d",
			 y, x, &ret);

	return (bool) ret;
}

bool find_one_object_trap(object_type* obj)
{
	s32b ret;

	call_lua("traps.find_one_object_trap", "(O)", "d",
		 obj, &ret);

	return (bool) ret;
}

bool find_all_traps(flags_type* traps)
{
	s32b ret;

	call_lua("traps.find_all_traps", "(F)", "d",
			 traps, &ret);

	return (bool) ret;
}

bool find_all_location_traps(s32b y, s32b x)
{
	s32b ret;

	call_lua("traps.find_all_location_traps", "(d,d)", "d",
			 y, x, &ret);

	return (bool) ret;
}

bool find_all_object_traps(object_type* obj)
{
	s32b ret;

	call_lua("traps.find_all_object_traps", "(O)", "d",
			 obj, &ret);

	return (bool) ret;
}

s32b get_num_traps(flags_type* traps, bool known_traps, s16b faction,
				  bool friendly_traps, bool apparent_traps)
{
	s32b num = 0;
	s32b i;

	for (i = 0; i < traps->size; i++)
	{
		flag_node*  n = &traps->node[i];
		flags_type* trap;

		/* Not used */
		if (!(n->flags & FLAG_FLAG_USED)) continue;

		if (!(n->flags & FLAG_FLAG_FLAGS)) continue;

		if (known_traps && !(n->flags & FLAG_FLAG_KNOWN)) continue;

		trap = n->vals.flags;

		if (!flag_exists(trap, FLAG_TRAP_IDX)) continue;

		if (faction != -1) {
			s32b trap_faction;
			s32b friendliness;

			if (apparent_traps && flag_exists(trap, FLAG_APPARENT_FACTION))
				trap_faction = flag_get(trap, FLAG_APPARENT_FACTION);
			else
				trap_faction = flag_get(trap, FLAG_FACTION);

			friendliness = get_faction_friendliness(trap_faction, faction);

			if ((friendly_traps && friendliness < 0) ||
				(!friendly_traps && friendliness >= 0))
				continue;
		}
		num++;
	}

	return(num);
} /* get_num_traps() */

/*
 * Count the number of traps at certain dungeon square.  If you want
 * to counta traps regardless of their faction, set "faction" to -1.
 */
s32b get_num_location_traps(s16b y, s16b x, bool known_traps, s16b faction,
						   bool friendly_traps, bool apparent_traps)
{
	cave_type*  c_ptr = &cave[y][x];
	flags_type* traps = &c_ptr->activations;

	return get_num_traps(traps, known_traps, faction, friendly_traps,
						 apparent_traps);
} /* get_num_location_traps() */

/*
 * Count the number of traps at certain dungeon square.  If you want
 * to counts traps regardless of their faction, set "faction" to -1.
 */
s32b get_num_object_traps(object_type* obj, bool known_traps, s16b faction,
						 bool friendly_traps, bool apparent_traps)
{
	flags_type* traps;

	if (!has_flag(obj, FLAG_TRAP))
		return 0;

	traps = flag_get_flags(&obj->flags, FLAG_TRAP);

	if (traps == NULL)
		return 0;

	return get_num_traps(traps, known_traps, faction, friendly_traps,
						 apparent_traps);
} /* get_num_location_traps() */

/*
 * Can return DISARM_SUCCESS, DISARM_FAILURE, or DISARM_TRIGGERED.
 * If it returns DISARM_TRIGGERED, it is the responsibility of the
 * caller to activate the trap.
 */
s32b disarm_one_location_trap(bool manual, s32b who, s32b y, s32b x)
{
	s32b ret;

	call_lua("traps.disarm_one_location_trap", "(b,d,d,d)", "d",
		 manual, who, y, x, &ret);

	return ret;
} /* disarm_location_trap() */

s32b disarm_one_object_trap(bool manual, s32b who, object_type* o_ptr)
{
	s32b ret;

	call_lua("traps.disarm_one_object_trap", "(b,d,O)", "d",
		 manual, who, o_ptr, &ret);

	return ret;
} /* disarm_object_trap() */

bool activate_location_traps(s32b who, s32b y, s32b x, bool friendly_traps)
{
	s32b ret;

	call_lua("traps.activate_location_traps", "(d,d,d,b,b)", "d",
			 who, y, x, FALSE, friendly_traps, &ret);

	return (bool) ret;
} /* activate_location_trap() */

bool activate_object_traps(s32b who, s32b y, s32b x, s32b o_idx,
						   bool friendly_traps)
{
	s32b ret;
	cave_type*   c_ptr = &cave[y][x];
	object_type* o_ptr = flag_get_obj(&(c_ptr->inventory), o_idx);

	call_lua("traps.activate_object_traps", "(d,O,b,b)", "d",
			 who, o_ptr, FALSE, friendly_traps, &ret);

	return (bool) ret;
} /* activate_object_trap() */

/* Randomly select a trap and places it there */
void place_trap(s32b y, s32b x, s32b faction)
{
	s32b ret;

	call_lua("traps.place_random_location_trap", "(d,d,d)", "d",
		 y, x, faction, &ret);
} /* place_trap() */

