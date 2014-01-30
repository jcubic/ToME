/* File: melee1.c */

/* Purpose: Monster attacks */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/*
 * Attack the player via physical attacks.
 */
bool make_attack_normal(s32b m_idx, byte divis)
{
	s32b ret;

	call_lua("monster_attack", "(d,d,d)", "d", m_idx, p_ptr->py, p_ptr->px, &ret);
	return ret;
}
