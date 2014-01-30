 /* File: cmd2.c */

/* Purpose: Movement commands (part 2) */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/*
 * Try to bash an altar
 */
/*static bool do_cmd_bash_altar(s32b y, s32b x)
{
	msg_print("Are you mad ? You want to anger the gods ?");
	return (FALSE);
}*/


/*
 * Try to bash a fountain
 */
/*static bool do_cmd_bash_fountain(s32b y, s32b x)
{*/
#if 0 // DGDGDGDGDG
	s32b bash, temp;

	cave_type *c_ptr;

	bool more = TRUE;

	monster_race *r_ptr = &r_info[p_ptr->body_monster];


	if ((p_ptr->body_monster != 0) && !((has_flag(r_ptr, FLAG_BASH_DOOR))))
	{
		msg_print("You cannot do that.");

		return (FALSE);
	}

	/* Take a turn */
	energy_use = get_player_energy(SPEED_GLOBAL);

	/* Get grid */
	c_ptr = &cave[y][x];

	/* Message */
	msg_print("You smash into the fountain!");

	/* Hack -- Bash power based on strength */
	/* (Ranges from 3 to 20 to 100 to 200) */
	bash = adj_str_blow[get_stat(A_STR, ind)];

	/* Compare bash power to door power XXX XXX XXX */
	temp = (bash - 50);

	/* Hack -- always have a chance */
	if (temp < 1) temp = 1;

	/* Hack -- attempt to bash down the door */
	if (rand_int(200) < temp)
	{
		/* Message */
		msg_print("The fountain breaks!");

//DGDGDGDG		fire_ball(GF_WATER, 5, damroll(6, 8), 2);

		cave_set_feat(y, x, FEAT_DEEP_WATER);
		more = FALSE;
	}

	return (more);
#endif
/*
	return FALSE;
}
*/

/*
 * Go up/down one/many level(s)
 * updown = TRUE := go down
 * updown = FALSE := go up
 */
void do_cmd_go_up_down(bool updown)
{
	cave_type *c_ptr;

	s32b levels = 0, j;
	bool ask_leave = FALSE;
	bool hook_did = FALSE;

	char* updown_str = updown ? "down" : "up";
	cptr  hook_msg   = NULL;

	s32b i;

	s32b old_dun = dun_level;

	dungeon_info_type *d_ptr = &d_info[dungeon_type];

	/* Player grid */
	c_ptr = &cave[p_ptr->py][p_ptr->px];

	/* test if on special level */
	ask_leave = has_flag(dungeon_flags, FLAG_ASK_LEAVE) &&
		((!has_flag(dungeon_flags, FLAG_SPECIAL)) ||
		 (has_flag(dungeon_flags, FLAG_SPECIAL) &&
		  generate_special_feeling));

	/* Can we ? */
	if (process_hooks(HOOK_STAIR_PRE, "(s)", updown_str))
		return;

	/* Quest exit */
	if (cave_feat_get(c_ptr, FLAG_QUEST_CHANGER) < 0)
	{
		leaving_quest = p_ptr->inside_quest;

		p_ptr->inside_quest = get_flag(c_ptr, FLAG_QUEST_CHANGER);
		dun_level = 0;
		p_ptr->oldpx = 0;
		p_ptr->oldpy = 0;
		p_ptr->leaving = TRUE;

		process_hooks(HOOK_STAIR_POST, "(s)", updown_str);

		return;
	}

	if (process_hooks_ret(HOOK_STAIR_DO, "ds", "(s,d)", updown_str,
						  ask_leave))
	{
		hook_did = TRUE;
		/*
		 * The hook handled things, but how?  If the number of levels
		 * changed is 0, then we stayed on the same level (like
		 * with using a void jumpgate), and thus are done.
		 */
		levels   = process_hooks_return[0].num;
		hook_msg = process_hooks_return[1].str;

		if (levels == 0)
			return;
	}
	else if (cave_feat_is(c_ptr, FLAG_LEVEL_CHANGER))
	{
		/* How many levels to go up/down? */
		levels = cave_feat_get(c_ptr, FLAG_LEVEL_CHANGER);
		if (flag_get2(&f_info[c_ptr->feat].flags, FLAG_LEVEL_CHANGER) != 0)
			levels = rand_range(levels,
								flag_get2(&f_info[c_ptr->feat].flags,
										  FLAG_LEVEL_CHANGER));
	}
	else
	{
		msg_print("There is nothing to go in here.");
		return;
	}

	/* Normal up stairs */
	if (levels != 0)
	{
		/*
		 * A hook can make the player go in opposite the intended
		 * direction, so when the hook did something don't check that
		 * the number of levels changed matches 'updown'.
		 */
		if ((levels > 0) && !updown && !hook_did)
		{
			msg_print("There is nothing here with which to go to the "
					  "previous area.");
			return;
		}
		if ((levels < 0) && updown && !hook_did)
		{
			msg_print("There is nothing here with which to go to the next "
					  "area.");
			return;
		}

		if ((!dun_level) && updown)
		{
			/* Save old player position */
			p_ptr->oldpx = p_ptr->px;
			p_ptr->oldpy = p_ptr->py;
		}
		else
		{
			if (confirm_stairs)
			{
				if (!get_check("Really leave the level? "))
					return;
			}
		}

		if (ask_leave)
		{
			prt("Leave this unique level forever (y/n) ? ", 0, 0);
			flush();
			i = inkey();
			prt("", 0, 0);
			if (i != 'y') return;
		}
	}
	else if (!hook_did)
	{
		msg_print("There is nothing to go in here.");
		return;
	}

	energy_use = 0;

	if (hook_msg)
		msg_print(hook_msg);
	else
		msg_print("You enter the next area.");

	process_hooks(HOOK_STAIR_POST, "(s)", updown_str);

	if (autosave_l)
	{
		is_autosave = TRUE;
		msg_print("Autosaving the game...");
		do_cmd_save_game();
		is_autosave = FALSE;
	}

	if (levels > 0)
	{
		for (j = 1; j <= levels; j++)
		{
			dun_level++;
			if (is_quest(dun_level)) break;
			if (d_ptr->maxdepth == dun_level) break;
		}
	}
	else if (levels < 0)
	{
		for (j = 1; j <= -levels; j++)
		{
			dun_level--;
			if (d_ptr->mindepth == dun_level) break;
		}
	}

	/* We change place */
	if (has_flag(c_ptr, FLAG_DUNGEON) && (!hook_did))
	{
		if (d_info[get_flag(c_ptr, FLAG_DUNGEON)].min_plev <= p_ptr->lev)
		{
			dungeon_info_type *d_ptr = &d_info[get_flag(c_ptr, FLAG_DUNGEON)];

			/* Do the lua scripts refuse ? ;) */
			if (process_hooks(HOOK_ENTER_DUNGEON, "(d)",
							  get_flag(c_ptr, FLAG_DUNGEON)))
			{
				dun_level = old_dun;
				return;
			}

			/* Ok go in the new dungeon */
			dungeon_type = get_flag(c_ptr, FLAG_DUNGEON);
			d_ptr = &d_info[dungeon_type];

			if ((p_ptr->wilderness_x == d_ptr->ix) &&
				(p_ptr->wilderness_y == d_ptr->iy))
			{
				dun_level = d_ptr->mindepth;
			}
			else if ((p_ptr->wilderness_x == d_ptr->ox) &&
				(p_ptr->wilderness_y == d_ptr->oy))
			{
				dun_level = d_ptr->maxdepth;
			}
			else
			{
				dun_level = d_ptr->mindepth;
			}

			msg_format("You go into %s",
				d_info[dungeon_type].text);

			process_hooks(HOOK_ENTERED_DUNGEON, "(d)",
						  dungeon_type);
		}
		else
		{
			msg_print("You don't feel yourself experienced enough to go there...");
			dun_level = old_dun;
			return;
		}
	}

	/* Leaving */
	p_ptr->leaving = TRUE;

	if (levels > 0)
	{
		if (levels > 1)
			create_down_shaft = TRUE;
		else
			create_down_stair = TRUE;
	}
	else if (levels < 0)
	{
		if (levels < -1)
			create_up_shaft = TRUE;
		else
			create_up_stair = TRUE;
	}
}

/*
 * Determine if a grid contains an openable object.
 */
static s16b openable_obj_check(s32b y, s32b x)
{
	cave_type *c_ptr = &cave[y][x];

	/* Scan all objects in the grid */
	for_inventory_slot(&c_ptr->inventory, o_ptr);
	{
		/* Check for openable object */
		if (has_flag(o_ptr, FLAG_CLOSED))
			return (item_nb);
	}
	end_inventory_slot();

	/* No openable object */
	return (0);
}


/*
 * Attempt to open the object at the given location
 *
 * Assume there is no monster blocking the destination
 *
 * Returns TRUE if repeated commands may continue
 */
bool do_cmd_open_obj(s32b y, s32b x, s16b o_idx)
{
	bool          more  = FALSE;
	cave_type*    c_ptr = &cave[y][x];
	object_type*  o_ptr = flag_get_obj(&(c_ptr->inventory), o_idx);
	monster_type *body  = &p_ptr->body_monster;

	if (!has_flag(body, FLAG_OPEN_DOOR))
	{
		msg_print("You cannot open objects.");

		return (FALSE);
	}

	if (has_flag(o_ptr, FLAG_OPEN)) {
		char buf[80];

		object_desc(buf, o_ptr, FALSE, 0);

		msg_format("The %s is already open.", buf);

		return (FALSE);
	}
	else if (!has_flag(o_ptr, FLAG_CLOSED)) {
		char buf[80];

		object_desc(buf, o_ptr, FALSE, 0);

		msg_format("The %s can neither be opened nor closed.", buf);

		return (FALSE);
	}

	/* Can we open? */
	if (process_hooks(HOOK_OPEN_PRE, "(d,d,d,O)", y, x, o_idx, o_ptr))
		return (FALSE);

	/* If locked, attempt to unlock it */
	if (has_flag(o_ptr, FLAG_LOCKED))
	{
		s32b i, j;

		energy_use += get_player_energy(SPEED_DISARM);

		/* Get the "disarm" factor */
		i = p_ptr->skill_dis;

		/* Penalize some conditions */
		if (intrinsic(BLIND) || no_lite()) i = i / 10;
		if (intrinsic(CONFUSED) || intrinsic(HALLUCINATE)) i = i / 10;

		/* Extract the difficulty */
		j = i - get_flag(o_ptr, FLAG_LOCKED);

		/* Always have a small chance of success */
		if (j < 2) j = 2;

		/* Success -- May still have traps */
		if (rand_int(100) < j)
		{
			msg_print("You have picked the lock.");
			gain_exp(1);

			flag_remove(&(o_ptr->flags), FLAG_LOCKED);
			set_flag(o_ptr, FLAG_UNLOCKED, 1);

			/* Player is opening object *AND* picking lock. */
			energy_use += get_player_energy(SPEED_DISARM);
		}

		/* Failure -- Keep trying */
		else
		{
			/* We may continue repeating */
			more = TRUE;

			if (flush_failure) flush();

			msg_print("You failed to pick the lock.");
		}
	}

	/*
	 * Lock has been picked, or it wasn't locked in the first place,
	 * so open it.
	 */
	if (!has_flag(o_ptr, FLAG_LOCKED))
	{
		char buf[80];

		energy_use += get_player_energy(SPEED_GLOBAL);

		if (has_flag(o_ptr, FLAG_TRAP)) {
			activate_object_traps(-1, y, x, o_idx, FALSE);

			/* Object traps go away when opened. */
			flag_remove(&(o_ptr->flags), FLAG_TRAP);
		}

		flag_remove(&(o_ptr->flags), FLAG_CLOSED);
		set_flag(o_ptr, FLAG_OPEN, 1);

		object_desc(buf, o_ptr, FALSE, 0);
		msg_format("You have opened the %s.", buf);

		process_hooks(HOOK_OPEN_POST,  "(d,d,d,O)",
			       y, x, o_idx, o_ptr);

		more = FALSE;
	}

	/* Result */
	return (more);
}

/*
 * Determine if a grid contains an closeable object.
 */
static s16b closeable_obj_check(s32b y, s32b x)
{
	cave_type *c_ptr = &cave[y][x];

	/* Scan all objects in the grid */
	for_inventory_slot(&c_ptr->inventory, o_ptr);
	{
		/* Check for openable object */
		if (has_flag(o_ptr, FLAG_OPEN))
			return (item_nb);
	}
	end_inventory_slot();

	/* No closeable object */
	return (0);
} /* closeable_obj_check() */

/*
 * Attempt to open the object at the given location
 *
 * Assume there is no monster blocking the destination
 *
 * Returns TRUE if repeated commands may continue
 */
static bool do_cmd_close_obj(s32b y, s32b x, s16b o_idx)
{
	bool          more  = FALSE;
	cave_type*    c_ptr = &cave[y][x];
	object_type*  o_ptr = flag_get_obj(&(c_ptr->inventory), o_idx);
	monster_type *body  = &p_ptr->body_monster;
	char          buf[80];

	if (!has_flag(body, FLAG_OPEN_DOOR))
	{
		msg_print("You cannot close objects.");

		return (FALSE);
	}

	if (has_flag(o_ptr, FLAG_CLOSED)) {
		char buf[80];

		object_desc(buf, o_ptr, FALSE, 0);

		msg_format("The %s is already closed.", buf);

		return (FALSE);
	}
	else if (!has_flag(o_ptr, FLAG_OPEN)) {
		char buf[80];

		object_desc(buf, o_ptr, FALSE, 0);

		msg_format("The %s can neither be opened nor closed.", buf);

		return (FALSE);
	}

	/* Can we open? */
	if (process_hooks(HOOK_CLOSE_PRE, "(d,d,d,O)", y, x, o_idx, o_ptr))
		return (FALSE);

	energy_use += get_player_energy(SPEED_GLOBAL);

	flag_remove(&(o_ptr->flags), FLAG_OPEN);
	set_flag(o_ptr, FLAG_CLOSED, 1);

	object_desc(buf, o_ptr, FALSE, 0);
	msg_format("You have closed the %s.", buf);

	process_hooks(HOOK_CLOSE_POST,  "(d,d,d,O)",
		      y, x, o_idx, o_ptr);

	more = FALSE;

	/* Result */
	return (more);
} /* do_cmd_close_obj() */

/*
 * Determine if a grid contains an object which is trapped, and the
 * player is aware of the trap.
 */
static s16b trapped_obj_check(s32b y, s32b x)
{
	cave_type *c_ptr = &cave[y][x];

	/* Scan all objects in the grid */
	for_inventory_slot(&c_ptr->inventory, o_ptr);
	{
		if (!has_flag(o_ptr, FLAG_TRAP_AWARE))
			continue;

		if (get_num_object_traps(o_ptr, TRUE, -1, FALSE, FALSE) > 0)
			return (item_nb);
	}
	end_inventory_slot();

	return (0);
} /* trapped_obj_check() */

#if defined(ALLOW_EASY_OPEN) || defined(ALLOW_EASY_DISARM)

/*
 * Original code by TNB, improvement for Angband 2.9.3 by rr9
 * Slightly modified for ToME because of its trap implementation
 */

/*
 * Return TRUE if the given grid is an open door
 */
static bool is_open(cave_type *c_ptr, s32b y, s32b x)
{
	return cave_mimic_is(c_ptr, FLAG_OPEN);
}


/*
 * Return TRUE if the given grid is a closed door
 */
static bool is_closed(cave_type *c_ptr, s32b y, s32b x)
{
	return cave_mimic_is(c_ptr, FLAG_CLOSED);
}


/*
 * Return TRUE if the given grid has a trap which looks like it doesn't
 * belong to the player.
 */
static bool is_trap(cave_type *c_ptr, s32b y, s32b x)
{
	if ((c_ptr->info & CAVE_TRDT) == 0)
		return FALSE;

	return (get_num_location_traps(y, x, TRUE, FACTION_PLAYER, FALSE, TRUE) > 0);
}


/*
 * Return the number of doors/traps around (or under)
 * the character using the filter function 'test'
 */
static s32b count_feats(s32b *y, s32b *x, bool (*test) (cave_type *c_ptr,
						     s32b y, s32b x),
                       bool under)
{
	s32b d;

	s32b xx, yy;

	s32b count;


	/* Clear match counter */
	count = 0;

	/* Check around (and under) the character */
	for (d = 0; d < 9; d++)
	{
		/* Ignore current grid if told so -- See tables.c */
		if ((d == 8) && !under) continue;

		/* Extract adjacent (legal) location */
		yy = p_ptr->py + ddy_ddd[d];
		xx = p_ptr->px + ddx_ddd[d];

		/* Paranoia */
		if (!in_bounds(yy, xx)) continue;

		/* Must have knowledge */
		if (!(cave[yy][xx].info & (CAVE_MARK))) continue;

		/* Not looking for this feature */
		if (!(*test) (&cave[yy][xx], yy, xx)) continue;

		/* Count it */
		count++;

		/* Remember the location. Only meaningful if there's
		   exactly one match */
		*y = yy;
		*x = xx;
	}

	/* All done */
	return (count);
}

/*
 * Return the number of openable objects around (or under) the character.
 * If requested, count only trapped objects.
 */
static s32b count_openable_objs(s32b *y, s32b *x, bool trapped)
{
	s32b d, count, o_idx;

	object_type *o_ptr;

	/* Count how many matches */
	count = 0;

	/* Check around (and under) the character */
	for (d = 0; d < 9; d++)
	{
		/* Extract adjacent (legal) location */
		s32b yy = p_ptr->py + ddy_ddd[d];
		s32b xx = p_ptr->px + ddx_ddd[d];

		cave_type* c_ptr = &cave[yy][xx];

		/* No (visible) chest is there */
		if ((o_idx = openable_obj_check(yy, xx)) == 0)
			continue;

		/* Grab the object */
		o_ptr = flag_get_obj(&(c_ptr->inventory), o_idx);

		/* No (known) traps here */
		if (trapped && !has_flag(o_ptr, FLAG_TRAP))
			continue;

		/* OK */
		++count;

		/* Remember the location. Only useful if only one match */
		*y = yy;
		*x = xx;
	}

	/* All done */
	return (count);
} /* count_openable_objs() */

/*
 * Return the number of closeable objects around (or under) the character.
 */
static s32b count_closeable_objs(s32b *y, s32b *x)
{
	s32b d, count, o_idx;

	/* Count how many matches */
	count = 0;

	/* Check around (and under) the character */
	for (d = 0; d < 9; d++)
	{
		/* Extract adjacent (legal) location */
		s32b yy = p_ptr->py + ddy_ddd[d];
		s32b xx = p_ptr->px + ddx_ddd[d];

		/* No (visible) chest is there */
		if ((o_idx = closeable_obj_check(yy, xx)) == 0)
			continue;

		/* OK */
		++count;

		/* Remember the location. Only useful if only one match */
		*y = yy;
		*x = xx;
	}

	/* All done */
	return (count);
} /* count_closeable_objs() */

/*
 * Return the number of trapped (that the player knows of) objects
 * around (or under) the player.
 */
static s32b count_trapped_objs(s32b *y, s32b *x)
{
	s32b d, count, o_idx;

	/* Count how many matches */
	count = 0;

	/* Check around (and under) the character */
	for (d = 0; d < 9; d++)
	{
		/* Extract adjacent (legal) location */
		s32b yy = p_ptr->py + ddy_ddd[d];
		s32b xx = p_ptr->px + ddx_ddd[d];

		/* No (visible) chest is there */
		if ((o_idx = trapped_obj_check(yy, xx)) == 0)
			continue;

		/* OK */
		++count;

		/* Remember the location. Only useful if only one match */
		*y = yy;
		*x = xx;
	}

	/* All done */
	return (count);
} /* count_trapped_objs() */

/*
 * Convert an adjacent location to a direction.
 */
static s32b coords_to_dir(s32b y, s32b x)
{
	s32b d[3][3] =
	        {
	                {7, 4, 1},
	                {8, 5, 2},
	                {9, 6, 3} };

	s32b dy, dx;


	dy = y - p_ptr->py;
	dx = x - p_ptr->px;

	/* Paranoia */
	if (ABS(dx) > 1 || ABS(dy) > 1) return (0);

	return d[dx + 1][dy + 1];
}

#endif /* defined(ALLOW_EASY_OPEN) || defined(ALLOW_EASY_DISARM) -- TNB */


/*
 * Perform the basic "open" command on doors
 *
 * Assume destination is a closed/locked/jammed door
 *
 * Assume there is no monster blocking the destination
 *
 * Returns TRUE if repeated commands may continue
 */
bool do_cmd_open_aux(s32b y, s32b x, s32b dir)
{
	s32b i, j;

	cave_type    *c_ptr = &cave[y][x];
	feature_type *f_ptr = &f_info[c_ptr->feat];
	feature_type *mimic = &f_info[c_ptr_mimic_or_feat(c_ptr)];
	flag_node    *close_node = flag_get_node(&f_ptr->flags, FLAG_CLOSED);
	flag_node    *mimic_node = flag_get_node(&mimic->flags, FLAG_CLOSED);

	bool more = FALSE;
	bool success;

	monster_type *body = &p_ptr->body_monster;

	if (mimic_node == NULL)
	{
		msg_print("There is nothing there to open.");
		return(FALSE);
	}

	if (close_node == NULL)
	{
		cmsg_print(TERM_L_RED, "ERROR: Feature has no FLAG_CLOSED!");
		return(FALSE);
	}

	if (!has_flag(body, FLAG_OPEN_DOOR))
	{
		msg_format("You body is not capable of opening the %s.",
				   mimic->name);

		return (FALSE);
	}

	/* Can we open? */
	if (process_hooks(HOOK_OPEN_PRE, "(d,d,d)", y, x, dir))
		return (FALSE);

	/* Feature won't let istelf be opened. */
	if ((close_node->flags & FLAG_FLAG_PTR) ||
		get_flag(f_ptr, FLAG_CLOSED) == 0)
	{
		if (mimic_node->flags & FLAG_FLAG_PTR)
			msg_print(flag_get_string(&mimic->flags, FLAG_CLOSED));
		else if (close_node->flags & FLAG_FLAG_PTR)
			msg_print(flag_get_string(&f_ptr->flags, FLAG_CLOSED));
		else
			msg_format("You are unnable to open the %s.",
					   mimic->name);

		return (FALSE);
	}

	/* Feature has Lua function to do the opening */
	if (close_node->flags & FLAG_FLAG_BOTH)
	{
        s32b domain = flag_get( &f_ptr->flags, FLAG_CLOSED);
        s32b ref    = flag_get2(&f_ptr->flags, FLAG_CLOSED);
		s32b success, more;

        open_lua_functions_registry(domain, ref);
        call_lua(NULL, "(d, d, d)", "d,d", y, x, dir, &success, &more);
        close_lua_functions_registry();

		if (success)
			process_hooks(HOOK_OPEN_POST,  "(d,d,d)", y, x, dir);

		/* Lua has handled everything, stop. */
		return (bool) more;
	}

	/* Jammed door */
	if (has_flag(f_ptr, FLAG_JAMMED))
	{
		/* Take a turn */
		energy_use += get_player_energy(SPEED_GLOBAL);

		/* Stuck */
		msg_print("The door appears to be stuck.");

		success = FALSE;
	}

	/* Locked door */
	else if (has_flag(f_ptr, FLAG_LOCKED))
	{
		energy_use += get_player_energy(SPEED_DISARM);

		/* Disarm factor */
		i = p_ptr->skill_dis;

		/* Penalize some conditions */
		if (intrinsic(BLIND) || no_lite()) i = i / 10;
		if (intrinsic(CONFUSED) || intrinsic(HALLUCINATE)) i = i / 10;

		/* Extract the lock power */
		j = get_flag(f_ptr, FLAG_LOCKED);

		/* Extract the difficulty XXX XXX XXX */
		j = i - j;

		/* Always have a small chance of success */
		if (j < 2) j = 2;

		/* Success */
		if (rand_int(100) < j)
		{
			/* Message */
			msg_print("You have picked the lock.");

			/* Set off trap */
			activate_trap_door(y, x);

			/* Open the door */
			cave_set_feat(y, x, get_flag(f_ptr, FLAG_CLOSED));

			/* Update some things */
			p_ptr->update |= (PU_VIEW | PU_MONSTERS | PU_MON_LITE);

			/* Sound */
			sound(SOUND_OPENDOOR);

			/* Experience */
			gain_exp(1);

			/* Burn energy opening door */
			energy_use += get_player_energy(SPEED_GLOBAL);

			success = TRUE;
		}

		/* Failure */
		else
		{
			/* Failure */
			if (flush_failure) flush();

			/* Message */
			msg_print("You failed to pick the lock.");

			/* We may keep trying */
			more = TRUE;

			success = FALSE;
		}
	}

	/* Closed door */
	else
	{
		/* Set off trap */
		activate_trap_door(y, x);

		/* Open the door */
		cave_set_feat(y, x, get_flag(f_ptr, FLAG_CLOSED));

		/* Update some things */
		p_ptr->update |= (PU_VIEW | PU_MONSTERS | PU_MON_LITE);

		/* Sound */
		sound(SOUND_OPENDOOR);

		success = TRUE;
	}

	if (success)
	{
		process_hooks(HOOK_OPEN_POST,  "(d,d,d)", y, x, dir);
		msg_format("You have opened the %s.", mimic->name);
	}
	
	/* Result */
	return (more);
} /* do_cmd_open_aux() */

/*
 * Open a closed/locked/jammed door or a closed/locked chest.
 *
 * Unlocking a locked door/chest is worth one experience point.
 */
void do_cmd_open(void)
{
	s32b y, x, dir = 0;

	s16b o_idx;

	cave_type *c_ptr;
	feature_type *f_ptr;

	bool more = FALSE;

	monster_type *body = &p_ptr->body_monster;

	if (!has_flag(body, FLAG_OPEN_DOOR))
	{
		msg_print("You cannot open doors or objects.");

		return;
	}

	/* Option: Pick a direction */
	if (easy_open)
	{
		s32b num_doors, num_chests;

		/* Count closed doors (locked or jammed) */
		num_doors = count_feats(&y, &x, is_closed, TRUE);

		/* Count chests (locked) */
		num_chests = count_openable_objs(&y, &x, FALSE);

		/* There is nothing the player can open */
		if ((num_doors + num_chests) == 0)
		{
			/* Message */
			msg_print("You see nothing there to open.");

			/* Done */
			return;
		}

		/* Set direction if there is only one target */
		else if ((num_doors + num_chests) == 1)
		{
			command_dir = coords_to_dir(y, x);
		}

		if (command_dir == 5)
		{
			command_arg = 0;
			command_rep = 0;
		}
	}

	/* Allow repeated command */
	if (command_arg)
	{
		/* Set repeat count */
		command_rep = command_arg - 1;

		/* Redraw the state */
		flag_bool(&p_ptr->redraw, FLAG_PR_STATE);

		/* Cancel the arg */
		command_arg = 0;
	}

	/* Get a "repeated" direction */
	if (get_rep_dir(&dir) || command_dir == 5)
	{
		if (command_dir == 5)
			dir = 5;

		/* Get requested location */
		y = p_ptr->py + ddy[dir];
		x = p_ptr->px + ddx[dir];

		/* Get requested grid */
		c_ptr = &cave[y][x];
		f_ptr = &f_info[c_ptr->feat];

		/* Check for chest */
		o_idx = openable_obj_check(y, x);

		/* Nothing useful */
		if (!cave_mimic_is(c_ptr, FLAG_CLOSED) && !o_idx)
		{
			/* Message */
			msg_print("You see nothing there to open.");
		}

		else if (!cave_feat_is(c_ptr, FLAG_CLOSED))
		{
			/* Illusionary door on top of a non-door. */
			msg_format("You seem unable to open the %s.",
					   f_info[c_ptr->mimic].name);
		}

		/* Monster in the way */
		else if (c_ptr->m_idx)
		{
			/* Take a turn */
			energy_use = get_player_energy(SPEED_GLOBAL);

			/* Message */
			msg_print("There is a monster in the way!");

			/* Attack */
			py_attack(y, x, -1, dir);
		}

		/* Handle openable objects */
		else if (o_idx)
		{
			/* Open the chest */
			more = do_cmd_open_obj(y, x, o_idx);
		}

		/* Handle doors */
		else
		{
			/* Open the door */
			more = do_cmd_open_aux(y, x, dir);
		}
	}

	/* Cancel repeat unless we may continue */
	if (!more) disturb(0, 0);
} /* do_cmd_open() */

/*
 * Perform the basic "close" command
 *
 * Assume destination is an open/broken door
 *
 * Assume there is no monster blocking the destination
 *
 * Returns TRUE if repeated commands may continue
 */
static bool do_cmd_close_aux(s32b y, s32b x, s32b dir)
{
	cave_type    *c_ptr = &cave[y][x];
	feature_type *f_ptr = &f_info[c_ptr->feat];
	feature_type *mimic = &f_info[c_ptr_mimic_or_feat(c_ptr)];
	flag_node    *open_node  = flag_get_node(&f_ptr->flags, FLAG_OPEN);
	flag_node    *mimic_node = flag_get_node(&mimic->flags, FLAG_OPEN);

	bool more = FALSE;

	monster_type *body = &p_ptr->body_monster;

	if (mimic_node == NULL)
	{
		msg_print("There is nothing there to open.");
		return(FALSE);
	}

	if (open_node == NULL)
	{
		cmsg_print(TERM_L_RED, "ERROR: Feature has no FLAG_OPEN!");
		return(FALSE);
	}

	if (!has_flag(body, FLAG_OPEN_DOOR))
	{
		msg_format("Your body is not cappable of closing the %s.",
				   mimic->name);

		return (FALSE);
	}

	if (process_hooks(HOOK_CLOSE_PRE, "(d,d,d)", y, x, dir))
		return (FALSE);

	/* Feature won't let istelf be closeed. */
	if ((open_node->flags & FLAG_FLAG_PTR) ||
		get_flag(f_ptr, FLAG_OPEN) == 0)
	{
		if (mimic_node->flags & FLAG_FLAG_PTR)
			msg_print(flag_get_string(&mimic->flags, FLAG_OPEN));
		else if (open_node->flags & FLAG_FLAG_PTR)
			msg_print(flag_get_string(&f_ptr->flags, FLAG_OPEN));
		else
			msg_format("You are unnable to close the %s.",
					   mimic->name);

		return (FALSE);
	}

	/* Feature has Lua function to do the closing */
	if (open_node->flags & FLAG_FLAG_BOTH)
	{
        s32b domain = flag_get( &f_ptr->flags, FLAG_OPEN);
        s32b ref    = flag_get2(&f_ptr->flags, FLAG_OPEN);
		s32b success, more;

        open_lua_functions_registry(domain, ref);
        call_lua(NULL, "(d, d, d)", "d,d", y, x, dir, &success, &more);
        close_lua_functions_registry();

		if (success)
			process_hooks(HOOK_CLOSE_POST,  "(d,d,d)", y, x, dir);

		/* Lua has handled everything, stop. */
		return (bool) more;
	}

	/* Take a turn */
	energy_use = get_player_energy(SPEED_GLOBAL);

	/* Get grid and contents */
	c_ptr = &cave[y][x];
	f_ptr = &f_info[c_ptr->feat];

	/* Set off trap */
	activate_trap_door(y, x);

	/* Broken door */
	if (has_flag(f_ptr, FLAG_JAMMED))
	{
		/* Message */
		msg_print("The door appears to be broken.");

		more = FALSE;
	}

	/* Open door */
	else
	{
		/* Close the door */
		cave_set_feat(y, x, get_flag(f_ptr, FLAG_OPEN));

		/* Update some things */
		p_ptr->update |= (PU_VIEW | PU_MONSTERS | PU_MON_LITE);

		/* Sound */
		sound(SOUND_SHUTDOOR);

		process_hooks(HOOK_CLOSE_POST, "(d,d,d)", y, x, dir);

		mimic = &f_info[c_ptr_mimic_or_feat(c_ptr)];
		msg_format("You have closed the %s.", mimic->name);

		more = FALSE;
	}

	/* Result */
	return (more);
} /* do_cmd_close_aux() */


/*
 * Close an open door.
 */
void do_cmd_close(void)
{
	s32b y, x, dir;

	cave_type *c_ptr;
	feature_type *f_ptr;

	bool more = FALSE;


#ifdef ALLOW_EASY_OPEN			/* TNB */

	/* Option: Pick a direction */
	if (easy_open)
	{
		s32b num_doors, num_objs;

		/* Count open doors and open objects*/
		num_doors = count_feats(&y, &x, is_open, TRUE);
		num_objs  = count_closeable_objs(&y, &x);

		/* There are no doors or objects the player can close */
		if ((num_doors + num_objs) == 0)
		{
			/* Message */
			msg_print("You see nothing there to close.");

			/* Done */
			return;
		}

		/* Exactly one closeable door or object*/
		else if ((num_doors + num_objs) == 1)
		{
			command_dir = coords_to_dir(y, x);
		}

		if (command_dir == 5)
		{
			command_arg = 0;
			command_rep = 0;
		}
	}

#endif /* ALLOW_EASY_OPEN -- TNB */

	/* Allow repeated command */
	if (command_arg)
	{
		/* Set repeat count */
		command_rep = command_arg - 1;

		/* Redraw the state */
		flag_bool(&p_ptr->redraw, FLAG_PR_STATE);

		/* Cancel the arg */
		command_arg = 0;
	}

	/* Get a "repeated" direction */
	if (get_rep_dir(&dir) || command_dir == 5)
	{
		s32b o_idx;

		if (command_dir == 5)
			dir = 5;

		/* Get requested location */
		y = p_ptr->py + ddy[dir];
		x = p_ptr->px + ddx[dir];

		/* Get grid and contents */
		c_ptr = &cave[y][x];
		f_ptr = &f_info[c_ptr->feat];

		o_idx = closeable_obj_check(y, x);

		/* Monster in the way */
		if (c_ptr->m_idx)
		{
			/* Take a turn */
			energy_use = get_player_energy(SPEED_GLOBAL);

			/* Message */
			msg_print("There is a monster in the way!");

			/* Attack */
			py_attack(y, x, -1, dir);
		}

		else if (o_idx != 0)
			/* Close the object */
			more = do_cmd_close_obj(y, x, o_idx);

		/* Require open/broken door */
		else if (!cave_mimic_is(c_ptr, FLAG_OPEN))
		{
			/* Message */
			msg_print("You see nothing there to close.");
		}
		else if (!cave_feat_is(c_ptr, FLAG_OPEN))
		{
			/* A illusionary door on top of a non-door. */
			msg_format("You cannot seem to manage to close the %s.",
					   f_info[c_ptr->mimic].name);
		}

		/* Close the door */
		else
			/* Close the door */
			more = do_cmd_close_aux(y, x, dir);
	}

	/* Cancel repeat unless we may continue */
	if (!more) disturb(0, 0);
} /* do_cmd_close() */


#if 0
static bool do_cmd_disarm_obj(s32b y, s32b x, s16b o_idx)
{
	cave_type*    c_ptr = &cave[y][x];
	object_type*  o_ptr = flag_get_obj(&(c_ptr->inventory), o_idx);

	s32b  result;
	char buf[80];

	object_desc(buf, o_ptr, FALSE, 0);

	if (!has_flag(o_ptr, FLAG_TRAP_AWARE)) {
		msg_format("The %s does not appear to be trapped.", buf);

		return (FALSE);
	}
	else if (get_num_object_traps(o_ptr, TRUE, -1, FALSE, FALSE) == 0) {
		msg_format("The %s is already disarmed.", buf);

		return (FALSE);
	}

	result = disarm_one_object_trap(TRUE, 0, o_ptr);

	switch(result)
	{
	case DISARM_SUCCESS:
		if (get_num_object_traps(o_ptr, TRUE, -1, FALSE, FALSE) > 0)
			msg_print("There still remain more traps to disarm.");
		return FALSE;

	case DISARM_FAILURE:
		// Disarming failed, but didn't trigger the trap; try again
		return TRUE;

	case DISARM_TRIGGERED:
		// Disarming failed AND triggered the traps.
		activate_object_traps(0, y, x, o_idx, FALSE);

		return FALSE;

	default:
		msg_print("ERROR: Unknown disarm return value!");
		return FALSE;
	}

	return FALSE;
} /* do_cmd_disarm_obj() */
#endif

/*
 * Perform the basic "disarm" command
 *
 * Assume destination is a visible trap
 *
 * Assume there is no monster blocking the destination
 *
 * Returns TRUE if repeated commands may continue
 */
#ifdef ALLOW_EASY_DISARM		/* TNB */
bool do_cmd_disarm_aux(s32b y, s32b x, s32b dir, s32b do_pickup)
#else  /* ALLOW_EASY_DISARM -- TNB */
static bool do_cmd_disarm_aux(s32b y, s32b x, s32b dir, s32b do_pickup)
#endif /* ALLOW_EASY_DISARM -- TNB */
{
	cave_type* c_ptr = &cave[y][x];
	s32b        ret;

	if (get_num_location_traps(y, x, TRUE, -1, FALSE, TRUE) == 0)
	{
		msg_print("No traps there to disarm.");
		return (FALSE);
	}


	ret = disarm_one_location_trap(TRUE, 0, y, x);

	switch(ret)
	{
	case DISARM_SUCCESS:
		if (!cave_feat_is(c_ptr, FLAG_DOOR))
		{
			if (get_num_location_traps(y, x, TRUE, FACTION_PLAYER, FALSE,
									   TRUE) > 0)
				msg_print("The are still more traps to disarm.");
			else
				move_player_aux(dir, do_pickup, 0, FALSE);
		}
		return FALSE;

	case DISARM_FAILURE:
		/* Disarming failed, but didn't trigger the trap; try again */
		return TRUE;

	case DISARM_TRIGGERED:
		/* Disarming failed AND triggered the trap */
		if (!cave_feat_is(c_ptr, FLAG_DOOR))
			/* Don't move us into a closed door. */
			move_player_aux(dir, do_pickup, 0, FALSE);

		/*
		 * If the player somehow manage to set off
		 * their own trap, activate it it here, since
		 * move_player_aux() won't it for us.
		 */
		if (get_num_location_traps(y, x, FALSE, FACTION_PLAYER,
								   TRUE, FALSE) > 0)
			activate_location_traps(0, y, x, TRUE);

		return FALSE;

	default:
		msg_print("ERROR: Unknown disarm return value!");
		return FALSE;
	}

	return FALSE;
} /* do_cmd_disarm_aux() */


/*
 * Disarms a trap, or chest
 */
void do_cmd_disarm(void)
{
	s32b           y, x, dir;
	s16b          o_idx;
	cave_type*    c_ptr;
	bool          more = FALSE;
	monster_type *body = &p_ptr->body_monster;

	if (!has_flag(body, FLAG_OPEN_DOOR))
	{
		msg_print("You cannot disarm things.");

		return;
	}

#ifdef ALLOW_EASY_DISARM		/* TNB */

	/* Option: Pick a direction */
	if (easy_disarm)
	{
		s32b num_traps, num_objs;

		/* Count visible traps */
		num_traps = count_feats(&y, &x, is_trap, TRUE);

		/* Count chests (trapped) */
		num_objs = count_trapped_objs(&y, &x);

		/* See if only one target */
		if (num_traps || num_objs)
		{
			if (num_traps + num_objs <= 1)
				command_dir = coords_to_dir(y, x);
		}
	}

#endif /* ALLOW_EASY_DISARM -- TNB */

	/* Allow repeated command */
	if (command_arg)
	{
		/* Set repeat count */
		command_rep = command_arg - 1;

		/* Redraw the state */
		flag_bool(&p_ptr->redraw, FLAG_PR_STATE);

		/* Cancel the arg */
		command_arg = 0;
	}

	/* Get a direction (or abort) */
	if (get_rep_dir(&dir))
	{
		/* Get location */
		y = p_ptr->py + ddy[dir];
		x = p_ptr->px + ddx[dir];

		/* Get grid and contents */
		c_ptr = &cave[y][x];

		/* Check for trapped */
		o_idx = trapped_obj_check(y, x);

		/* Monster in the way */
		if (c_ptr->m_idx)
		{
			/* Message */
			msg_print("There is a monster in the way!");

			/* Attack */
			py_attack(y, x, -1, dir);
		}

		/* Disarm object */
		else if (o_idx)
		{
/* DGDGDGDG -- well no dont ...			more = do_cmd_disarm_obj(y, x, o_idx); */
		}

		/* Disarm trap; ignore player's traps */
		else if (c_ptr->info & CAVE_TRDT)
		{
			/* Disarm the trap */
			more = do_cmd_disarm_aux(y, x, dir, always_pickup);
		}

		/* Nothing there. */
		else
		{
			/* Message */
			msg_print("You see nothing there to disarm.");
		}

	}

	/* Cancel repeat unless told not to */
	if (!more)
		disturb(0, 0);
} /* do_cmd_disarm() */


/*
 * Perform the basic "bash" command
 *
 * Assume destination is a closed/locked/jammed door
 *
 * Assume there is no monster blocking the destination
 *
 * Returns TRUE if repeated commands may continue
 */
static bool do_cmd_bash_aux(s32b y, s32b x, s32b dir)
{
#if 0 // DGDGDGDGDG
	s32b bash, temp;

	cave_type *c_ptr;

	bool more = FALSE;

	monster_race *r_ptr = &r_info[p_ptr->body_monster];


	if ((p_ptr->body_monster != 0) && !((has_flag(r_ptr, FLAG_BASH_DOOR))))
	{
		msg_print("You cannot do that.");

		return (FALSE);
	}

	/* Take a turn */
	energy_use = get_player_energy(SPEED_WALK);

	/* Get grid */
	c_ptr = &cave[y][x];

	/* Message */
	msg_print("You smash into the door!");

	/* Hack -- Bash power based on strength */
	/* (Ranges from 3 to 20 to 100 to 200) */
	bash = adj_str_blow[get_stat(A_STR, ind)];

	/* Extract door power */
	temp = ((c_ptr->feat - FEAT_DOOR_HEAD) & 0x07);

	/* Compare bash power to door power XXX XXX XXX */
	temp = (bash - (temp * 10));

	/* Hack -- always have a chance */
	if (temp < 1) temp = 1;

	/* Hack -- attempt to bash down the door */
	if (rand_int(100) < temp)
	{
		/* Message */
		msg_print("The door crashes open!");

		/* Break down the door */
		if (rand_int(100) < 50)
		{
			/* Set off trap */
			activate_trap_door(y, x);

			cave_set_feat(y, x, FEAT_BROKEN);
		}

		/* Open the door */
		else
		{
			/* Set off trap */
			activate_trap_door(y, x);

			cave_set_feat(y, x, FEAT_OPEN);
		}

		/* Sound */
		sound(SOUND_OPENDOOR);

		/* Hack -- Fall through the door */
		move_player(dir, always_pickup);

		/* Update some things */
		p_ptr->update |= (PU_VIEW | PU_MON_LITE);
		p_ptr->update |= (PU_DISTANCE);
	}

	/* Saving throw against stun */
	else if (rand_int(100) < adj_dex_safe[get_stat(A_DEX, ind)] + p_ptr->lev)
	{
		/* Message */
		msg_print("The door holds firm.");

		/* Allow repeated bashing */
		more = TRUE;
	}

	/* High dexterity yields coolness */
	else
	{
		/* Message */
		msg_print("You are off-balance.");
	}

	/* Result */
	return (more);
#endif

	return (FALSE);
}


/*
 * Bash open a door, success based on character strength
 *
 * For a closed door, pval is positive if locked; negative if stuck.
 *
 * For an open door, pval is positive for a broken door.
 *
 * A closed door can be opened - harder if locked. Any door might be
 * bashed open (and thereby broken). Bashing a door is (potentially)
 * faster! You move into the door way. To open a stuck door, it must
 * be bashed. A closed door can be jammed (see do_cmd_spike()).
 *
 * Creatures can also open or bash doors, see elsewhere.
 */
void do_cmd_bash(void)
{
#if 0 // DGDGDGDGDG
	s32b y, x, dir;

	cave_type *c_ptr;

	bool more = FALSE;

	monster_race *r_ptr = &r_info[p_ptr->body_monster];


	if ((p_ptr->body_monster != 0) && !((has_flag(r_ptr, FLAG_BASH_DOOR))))
	{
		msg_print("You cannot do that.");

		return;
	}

	/* Allow repeated command */
	if (command_arg)
	{
		/* Set repeat count */
		command_rep = command_arg - 1;

		/* Redraw the state */
		flag_bool(&p_ptr->redraw, FLAG_PR_STATE);

		/* Cancel the arg */
		command_arg = 0;
	}

	/* Get a "repeated" direction */
	if (get_rep_dir(&dir))
	{
		/* Bash location */
		y = p_ptr->py + ddy[dir];
		x = p_ptr->px + ddx[dir];

		/* Get grid */
		c_ptr = &cave[y][x];

		/* Nothing useful */
		if ((c_ptr->feat < FEAT_DOOR_HEAD ||
		                c_ptr->feat > FEAT_DOOR_TAIL) &&
		                (c_ptr->feat < FEAT_ALTAR_HEAD ||
		                 c_ptr->feat > FEAT_ALTAR_TAIL) && (c_ptr->feat != FEAT_FOUNTAIN))
		{
			/* Message */
			msg_print("You see nothing there to bash.");
		}

		/* Monster in the way */
		else if (c_ptr->m_idx)
		{
			/* Take a turn */
			energy_use = get_player_energy(SPEED_WALK);

			/* Message */
			msg_print("There is a monster in the way!");

			/* Attack */
			py_attack(y, x, -1, dir);
		}

		else if (c_ptr->feat >= FEAT_ALTAR_HEAD &&
		                c_ptr->feat <= FEAT_ALTAR_TAIL)
		{
			more = do_cmd_bash_altar(y, x);
		}
		/* Bash a closed door */
		else if (c_ptr->feat == FEAT_FOUNTAIN)
		{
			more = do_cmd_bash_fountain(y, x);
		}
		else
		{
			/* Bash the door */
			more = do_cmd_bash_aux(y, x, dir);
		}
	}

	/* Unless valid action taken, cancel bash */
	if (!more) disturb(0, 0);
#endif
}



/*
 * Manipulate an adjacent grid in some way
 *
 * Attack monsters, tunnel through walls, disarm traps, open doors.
 *
 * Consider confusion XXX XXX XXX
 *
 * This command must always take a turn, to prevent free detection
 * of invisible monsters.
 */
void do_cmd_alter(void)
{
	s32b y, x, dir;

	cave_type *c_ptr;
	feature_type *f_ptr;

	bool more = FALSE;


	/* Allow repeated command */
	if (command_arg)
	{
		/* Set repeat count */
		command_rep = command_arg - 1;

		/* Redraw the state */
		flag_bool(&p_ptr->redraw, FLAG_PR_STATE);

		/* Cancel the arg */
		command_arg = 0;
	}

	/* Get a direction */
	if (!get_rep_dir(&dir))
	{
		disturb(0, 0);
		return;
	}

	/* Get location */
	y = p_ptr->py + ddy[dir];
	x = p_ptr->px + ddx[dir];

	/* Get grid */
	c_ptr = &cave[y][x];
	f_ptr = &f_info[c_ptr->feat];

	/* Attack monsters */
	if (c_ptr->m_idx)
	{
		/* Attack */
		py_attack(y, x, -1, dir);
	}

	/*
	 * Let Lua do stuff. If Lua hooks want doors and traps to be
	 * handled by us, it has to check for them and decline to do
	 * anything if found.
	 */
	else if (process_hooks_ret(HOOK_ALTER_GRID, "b", "(d, d, d)",
							   y, x, dir))
	{
		more = process_hooks_return[0].boolean;
	}

	/* Player must know what's there to do anything. */
	else if (!(c_ptr->info & CAVE_MARK))
	{
		/* Message */
		msg_print("You see nothing there.");
		more = FALSE;
	}

	/* Bash jammed doors */
	else if (cave_mimic_is(c_ptr, FLAG_DOOR) &&
			 !has_flag(f_ptr, FLAG_SECRET) &&
			 !has_flag(f_ptr, FLAG_OPEN) && has_flag(f_ptr, FLAG_JAMMED))
	{
		/* Bash */
		energy_use = get_player_energy(SPEED_GLOBAL);
		more = do_cmd_bash_aux(y, x, dir);
	}

	/* Open closed doors */
	else if (cave_mimic_is(c_ptr, FLAG_DOOR) &&
			 !has_flag(f_ptr, FLAG_SECRET) &&
			 !has_flag(f_ptr, FLAG_OPEN))
	{
		/* Tunnel */
		energy_use = get_player_energy(SPEED_GLOBAL);
		more = do_cmd_open_aux(y, x, dir);
	}

	/* Disarm traps */
	else if (get_num_location_traps(y, x, TRUE, FACTION_PLAYER,
									FALSE, TRUE) > 0)
	{
		/* Disarm */
		energy_use = get_player_energy(SPEED_DISARM);
		more = do_cmd_disarm_aux(y, x, dir, always_pickup);
	}

	/* Oops */
	else
	{
		/* Oops */
		energy_use = get_player_energy(SPEED_GLOBAL);
		msg_print("You attack the empty air.");
	}

	/* Cancel repetition unless we can continue */
	if (!more) disturb(0, 0);
}

void do_cmd_walk_jump(s32b pickup)
{
	s32b dir;

	bool more = FALSE;


	/* Allow repeated command */
	if (command_arg)
	{
		/* Set repeat count */
		command_rep = command_arg - 1;

		/* Redraw the state */
		flag_bool(&p_ptr->redraw, FLAG_PR_STATE);

		/* Cancel the arg */
		command_arg = 0;
	}

	/* Get a "repeated" direction */
	if (get_rep_dir(&dir))
	{
		/* Take a turn */
		energy_use = get_player_energy(SPEED_WALK);

		/* Actually move the character */
		move_player(dir, pickup);

		/* Allow more walking */
		more = TRUE;
	}

	/* Hack -- In small scale wilderness it takes MUCH more time to move */
	energy_use *= (p_ptr->wild_mode) ? ((cur_hgt + cur_wid) / 2) : 1;

	/* Hack again -- Is there a special encounter ??? */
	if (p_ptr->wild_mode)
		process_hooks(HOOK_WILD_SMALL_MOVE_POST, "()");

	/* Cancel repeat unless we may continue */
	if (!more) disturb(0, 0);
}


/*
 * Support code for the "Walk" and "Jump" commands
 */
void do_cmd_walk(s32b pickup)
{
	/* Move (usually pickup) */
	if (intrinsic(BLINK_MOVE))
	{
		do_cmd_unwalk();
	}
	else
	{
		do_cmd_walk_jump(pickup);
	}
}


void do_cmd_run_run()
{
	s32b dir;


	/* Hack -- no running when confused */
	if (intrinsic(CONFUSED))
	{
		msg_print("You are too confused!");
		return;
	}

	/* Get a "repeated" direction */
	if (get_rep_dir(&dir))
	{
		/* Hack -- Set the run counter */
		running = (command_arg ? command_arg : 1000);

		/* First step */
		run_step(dir);
	}
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);
}


/*
 * Start running.
 */
void do_cmd_run(void)
{
	if (intrinsic(BLINK_MOVE))
	{
		return;
	}
	else
	{
		do_cmd_run_run();
	}
}



/*
 * Stay still.  Search.  Enter stores.
 * Pick up treasure if "pickup" is true.
 */
void do_cmd_stay(s32b pickup)
{
	cave_type *c_ptr = &cave[p_ptr->py][p_ptr->px];


	/* Allow repeated command */
	if (command_arg)
	{
		/* Set repeat count */
		command_rep = command_arg - 1;

		/* Redraw the state */
		flag_bool(&p_ptr->redraw, FLAG_PR_STATE);

		/* Cancel the arg */
		command_arg = 0;
	}


	/* Take a turn */
	energy_use = get_player_energy(SPEED_WALK);

	/* Handle "objects" */
	carry(pickup);


	/* Hack -- enter a store if we are on one */
	if (flag_exists(&f_info[c_ptr->feat].flags, FLAG_CONTAINS_BUILDING))
	{
		/* Disturb */
		disturb(0, 0);

		/* Hack -- enter store */
		command_new = '_';
		energy_use = 0;
	}

	process_hooks(HOOK_STAY_POST, "()");
}

/*
 * Determines the odds of an object breaking when thrown at a monster
 *
 * Note that artifacts never break, see the "drop_near()" function.
 */
s32b breakage_chance(object_type *o_ptr)
{
	s32b chance;

	call_lua("breakage_chance", "(O)", "d", o_ptr, &chance);

	return (chance);
}

/*
 * Try to ``walk'' using phase door.
 */
void do_cmd_unwalk()
{
	s32b dir, y, x;
	s16b feat;

	cave_type *c_ptr;
	feature_type *f_ptr;

	bool more = FALSE;


	if (!get_rep_dir(&dir)) return;

#if 0							/* No more, but there are penalities */

	/* A mold can't blink in small scale mode */
	if (p_ptr->wild_mode)
	{
		msg_print("You cannot move in the overview display.");
		return;
	}

#endif

	y = p_ptr->py + ddy[dir];
	x = p_ptr->px + ddx[dir];

	c_ptr = &cave[y][x];
	feat = c_ptr->feat;
	f_ptr = &f_info[feat];

	if (intrinsic(CANNOT_MOVE) && c_ptr->m_idx == 0)
	{
		msg_print("You are unable to move.");
		return;
	}

	/* Must have knowledge to know feature XXX XXX */
	if (!(c_ptr->info & (CAVE_MARK))) feat = 0;

	/* Take a turn */
	energy_use = get_player_energy(SPEED_WALK);
	energy_use *= (p_ptr->wild_mode) ? (5 * (cur_hgt + cur_wid) / 2) : 1;


	/* Allow repeated command */
	if (command_arg)
	{
		/* Set repeat count */
		command_rep = command_arg - 1;

		/* Redraw the state */
		flag_bool(&p_ptr->redraw, FLAG_PR_STATE);

		/* Cancel the arg */
		command_arg = 0;
	}


	/* Attack monsters */
	if (c_ptr->m_idx > 0)
	{
		/* Attack */
		py_attack(y, x, -1, -1);
	}

	/* Exit the area */
	else if ((!dun_level) && (!p_ptr->wild_mode) &&
	                ((x == 0) || (x == cur_wid - 1) || (y == 0) || (y == cur_hgt - 1)))
	{
		/* Can the player enter the grid? */
		if (player_can_enter(c_ptr->mimic) &&
			!process_hooks(HOOK_WILD_LARGE_NEW_AREA_PRE, "(d,d)", y, x))
		{
			bool new_area = FALSE;

			/* Hack: move to new area */
			if ((y == 0) && (x == 0))
			{
				p_ptr->wilderness_y--;
				p_ptr->wilderness_x--;
				p_ptr->wild_force_side_y_ok = TRUE;
				p_ptr->wild_force_side_x_ok = TRUE;
				p_ptr->wild_force_side_y = 1;
				p_ptr->wild_force_side_x = 1;
				new_area = TRUE;
			}

			else if ((y == 0) && (x == cur_wid - 1))
			{
				p_ptr->wilderness_y--;
				p_ptr->wilderness_x++;
				p_ptr->wild_force_side_y_ok = TRUE;
				p_ptr->wild_force_side_x_ok = TRUE;
				p_ptr->wild_force_side_y = 1;
				p_ptr->wild_force_side_x = -1;
				new_area = TRUE;
			}

			else if ((y == cur_hgt - 1) && (x == 0))
			{
				p_ptr->wilderness_y++;
				p_ptr->wilderness_x--;
				p_ptr->wild_force_side_y_ok = TRUE;
				p_ptr->wild_force_side_x_ok = TRUE;
				p_ptr->wild_force_side_y = -1;
				p_ptr->wild_force_side_x = 1;
				new_area = TRUE;
			}

			else if ((y == cur_hgt - 1) && (x == cur_wid - 1))
			{
				p_ptr->wilderness_y++;
				p_ptr->wilderness_x++;
				p_ptr->wild_force_side_y_ok = TRUE;
				p_ptr->wild_force_side_x_ok = TRUE;
				p_ptr->wild_force_side_y = -1;
				p_ptr->wild_force_side_x = -1;
				new_area = TRUE;
			}

			else if (y == 0)
			{
				p_ptr->wilderness_y--;
				p_ptr->wild_force_side_y_ok = TRUE;
				p_ptr->wild_force_side_y = 1;
				new_area = TRUE;
			}

			else if (y == cur_hgt - 1)
			{
				p_ptr->wilderness_y++;
				p_ptr->wild_force_side_y_ok = TRUE;
				p_ptr->wild_force_side_y = -1;
				new_area = TRUE;
			}

			else if (x == 0)
			{
				p_ptr->wilderness_x--;
				p_ptr->wild_force_side_x_ok = TRUE;
				p_ptr->wild_force_side_x = 1;
				new_area = TRUE;
			}

			else if (x == cur_wid - 1)
			{
				p_ptr->wilderness_x++;
				p_ptr->wild_force_side_x_ok = TRUE;
				p_ptr->wild_force_side_x = -1;
				new_area = TRUE;
			}

			p_ptr->leaving = TRUE;

			/* This should make both ways of handling wilderness edges work. */
			if (wild_see_through)
			{
				p_ptr->oldpy = p_ptr->py;
				p_ptr->oldpx = p_ptr->px;
			}

			if (new_area)
				process_hooks(HOOK_WILD_LARGE_NEW_AREA_POST, "(d,d)", y, x);

			return;
		}
	}

	/* Hack -- Ignore weird terrain types. */
	else if (!cave_floor_grid(c_ptr))
	{
		teleport_player(10);
	}

	/* Enter quests */
	else if (has_flag(f_ptr, FLAG_CONTAINS_BUILDING) || has_flag(f_ptr, FLAG_LEVEL_CHANGER))
	{
		move_player(dir, always_pickup);
		more = FALSE;
	}

	/* Hack -- Ignore wilderness mofe. */
	else if (p_ptr->wild_mode)
	{
		/* Chance to not blink right */
		if (magik(15))
		{
			do
			{
				dir = rand_range(1, 9);
			}
			while (dir == 5);
		}

		move_player(dir, always_pickup);
	}

	/* Walking semantics */
	else
	{
		teleport_player_directed(10, dir);
	}

	/* Cancel repetition unless we can continue */
	if (!more) disturb(0, 0);
}


static bool tport_vertically(bool how)
{
	/* arena or quest -KMW- */
	if ((p_ptr->inside_arena) || (p_ptr->inside_quest))
	{
		msg_print("There is no effect.");
		return (FALSE);
	}

	if ((has_flag(dungeon_flags, FLAG_NO_EASY_MOVE)))
	{
		msg_print("Some powerful force prevents you from teleporting.");
		return FALSE;
	}

	/* Go down */
	if (how)
	{
		if (dun_level >= d_info[dungeon_type].maxdepth)
		{
			msg_print("The floor is impermeable.");
			return (FALSE);
		}

		msg_print("You sink through the floor.");
		dun_level++;
		p_ptr->leaving = TRUE;
	}
	else
	{
		if (dun_level < d_info[dungeon_type].mindepth)
		{
			msg_print("There is nothing above you but air.");
			return (FALSE);
		}

		msg_print("You rise through the ceiling.");
		dun_level--;
		p_ptr->leaving = TRUE;
	}

	return (TRUE);
}

/*
 * Display a list of the items that the given monster carries.
 */
#if 0 // DGDGDGDG
byte show_monster_inven(s32b m_idx, s32b *monst_list)
{
	s32b i, j, k, l;

	s32b col, len, lim;

	object_type *o_ptr;

	char o_name[80];

	char tmp_val[80];

	s32b out_index[23];

	byte out_color[23];

	char out_desc[23][80];

	s32b monst_num;


	/* Default length */
	len = 79 - 50;

	/* Maximum space allowed for descriptions */
	lim = 79 - 3;

	/* Require space for weight (if needed) */
	if (show_weights) lim -= 9;

	/* Scan for objects on the monster */
	(void)scan_monst(monst_list, &monst_num, m_idx);

	/* Display the inventory */
	for (k = 0, i = 0; i < monst_num; i++)
	{
		o_ptr = o_list[monst_list[i]];

		/* Describe the object */
		object_desc(o_name, o_ptr, TRUE, 3);

		/* Hack -- enforce max length */
		o_name[lim] = '\0';

		/* Save the index */
		out_index[k] = i;

		/* Acquire inventory color */
		out_color[k] = tval_to_attr[o_ptr->tval & 0x7F];

		/* Save the object description */
		strcpy(out_desc[k], o_name);

		/* Find the predicted "line length" */
		l = strlen(out_desc[k]) + 5;

		/* Be sure to account for the weight */
		if (show_weights) l += 9;

		/* Maintain the maximum length */
		if (l > len) len = l;

		/* Advance to next "line" */
		k++;
	}

	/* Find the column to start in */
	col = (len > 76) ? 0 : (79 - len);

	/* Output each entry */
	for (j = 0; j < k; j++)
	{
		/* Get the index */
		i = monst_list[out_index[j]];

		/* Get the item */
		o_ptr = o_list[i];

		/* Clear the line */
		prt("", j + 1, col ? col - 2 : col);

		/* Prepare an index --(-- */
		strnfmt(tmp_val, 80, "%c)", index_to_label(j));

		/* Clear the line with the (possibly indented) index */
		put_str(tmp_val, j + 1, col);

		/* Display the entry itself */
		c_put_str(out_color[j], out_desc[j], j + 1, col + 3);

		/* Display the weight if needed */
		if (show_weights)
		{
			s32b wgt = o_ptr->weight * o_ptr->number;
			strnfmt(tmp_val, 80, "%3d.%1d lb", wgt / 10, wgt % 10);
			put_str(tmp_val, j + 1, 71);
		}
	}

	/* Make a "shadow" below the list (only if needed) */
	if (j && (j < 23)) prt("", j + 1, col ? col - 2 : col);

	return monst_num;
}
#endif

/*
 * Steal an object from a monster
 */
void do_cmd_steal()
{
#if 0 // DGDGDGDG
	s32b x, y, dir = 0, item = -1, k = -1;

	cave_type *c_ptr;

	monster_type *m_ptr;

	object_type *o_ptr;

	byte num = 0;

	bool done = FALSE;

	s32b monst_list[23];


	/* Only works on adjacent monsters */
	if (!get_rep_dir(&dir)) return;
	y = p_ptr->py + ddy[dir];
	x = p_ptr->px + ddx[dir];
	c_ptr = &cave[y][x];

	if (!(c_ptr->m_idx))
	{
		msg_print("There is no monster there!");
		return;
	}

	m_ptr = get_monster(c_ptr->m_idx);

	/* There were no non-gold items */
	if (!m_ptr->hold_o_idx)
	{
		msg_print("That monster has no objects!");
		return;
	}

	/* The monster is immune */
	if (has_flag(&r_info[m_ptr->r_idx], FLAG_NO_THEFT))
	{
		msg_print("The monster is guarding the treasures.");
		return;
	}

	screen_save();

	num = show_monster_inven(c_ptr->m_idx, monst_list);

	/* Repeat until done */
	while (!done)
	{
		char tmp_val[80];
		s32b which = ' ';

		/* Build the prompt */
		strnfmt(tmp_val, 80, "Choose an item to steal (a-%c) or ESC:",
		        'a' - 1 + num);

		/* Show the prompt */
		prt(tmp_val, 0, 0);

		/* Get a key */
		which = inkey();

		/* Parse it */
		switch (which)
		{
		case ESCAPE:
			{
				done = TRUE;

				break;
			}

		default:
			{
				s32b ver;

				/* Extract "query" setting */
				ver = isupper(which);
				which = tolower(which);

				k = islower(which) ? A2I(which) : -1;
				if (k < 0 || k >= num)
				{
					bell();

					break;
				}

				/* Verify the item */
				if (ver && !verify("Try", 0 - monst_list[k]))
				{
					done = TRUE;

					break;
				}

				/* Accept that choice */
				item = monst_list[k];
				done = TRUE;

				break;
			}
		}
	}

	if (item != -1)
	{
		s32b chance;

		chance = MAX_STAT_VALUE - get_stat(A_DEX, ind);
		chance +=
		        o_list[item]->weight / (get_skill_scale(SKILL_STEALING, 19) + 1);
		chance += get_skill_scale(SKILL_STEALING, 29) + 1;
		chance -= (m_ptr->csleep) ? 10 : 0;
		chance += m_ptr->level;

		/* Failure check */
		if (rand_int(chance) > 1 + get_skill_scale(SKILL_STEALING, 25))
		{
			/* Take a turn */
			energy_use = get_player_energy(SPEED_STEAL);

			/* Wake up */
			m_ptr->csleep = 0;

			/* Speed up because monsters are ANGRY when you try to thief them */
			m_ptr->mspeed += 5;

			screen_load();

			msg_print("Oops ! The monster is now really *ANGRY*.");

			return;
		}

		/* Reconnect the objects list */
		if (num == 1) m_ptr->hold_o_idx = 0;
		else
		{
			if (k > 0) o_list[monst_list[k - 1]]->next_o_idx = monst_list[k + 1];
			if (k + 1 >= num) o_list[monst_list[k - 1]]->next_o_idx = 0;
			if (k == 0) m_ptr->hold_o_idx = monst_list[k + 1];
		}

		/* Rogues gain some xp */
		if (has_flag(p_ptr, FLAG_EASE_STEAL))
		{
			s32b max_point;

			/* Max XP gained from stealing */
			max_point = (o_list[item]->weight / 2) + (m_ptr->level * 10);

			/* Randomise it a bit, with half a max guaranteed */
			gain_exp((max_point / 2) + (randint(max_point) / 2));

			/* Allow escape */
			if (get_check("Phase door?")) teleport_player(10);
		}

		/* Special handling for gold */
		if (has_flag(o_list[item], FLAG_GOLD_VALUE))
		{
			/* Collect the gold */
			p_ptr->au += get_flag(o_list[item], FLAG_GOLD_VALUE);

			/* Redraw gold */
			flag_bool(&p_ptr->redraw, FLAG_PR_GOLD);

			/* Window stuff */
			flag_bool(&p_ptr->window, FLAG_PW_PLAYER);
		}
		else
		{
			/* Get the item */
			o_ptr = new_object();

			object_copy(o_ptr, o_list[item]);

			inven_carry(o_ptr, FALSE);
		}

		/* Delete it */
		o_list[item]->k_idx = 0;
	}

	screen_load();

	/* Take a turn */
	energy_use = get_player_energy(SPEED_STEAL);
#endif
}


/*
 * Give an item to a monster
 */
void do_cmd_give()
{
	s32b dir, x, y;

	cave_type *c_ptr;

	cptr q, s;

	s32b item;

	bool success;

	/* Get a "repeated" direction */
	if (!get_rep_dir(&dir)) return;

	/* Get requested location */
	y = p_ptr->py + ddy[dir];
	x = p_ptr->px + ddx[dir];

	/* Get requested grid */
	c_ptr = &cave[y][x];

	/* No monster in the way */
	if (c_ptr->m_idx == 0)
	{
		msg_print("There is no monster there.");
		return;
	}

	/* Get an item */
	q = "What item do you want to offer? ";
	s = "You have nothing to offer.";
	if (!get_item(&item, q, s, USE_INVEN)) return;

	/* Process hooks if there are any */
	if (!process_hooks(HOOK_GIVE_PRE, "(d,d)", c_ptr->m_idx, item))
	{
		msg_print("The monster does not want your item.");
		success = FALSE;
	}
	else
		success = TRUE;

	/* Take a turn, even if the offer is declined */
	energy_use = get_player_energy(SPEED_GLOBAL);

	if (success)
		process_hooks(HOOK_GIVE_POST, "(d,d)", c_ptr->m_idx, item);
}


/*
 * Chat with a monster
 */
void do_cmd_chat()
{
	s32b dir, x, y;

	cave_type *c_ptr;

	/* Get a "repeated" direction */
	if (!get_rep_dir(&dir)) return;

	/* Get requested location */
	y = p_ptr->py + ddy[dir];
	x = p_ptr->px + ddx[dir];

	/* Get requested grid */
	c_ptr = &cave[y][x];

	/* No monster in the way */
	if (c_ptr->m_idx == 0)
	{
		msg_print("There is no monster there.");
		return;
	}

	talk_to_monster(c_ptr->m_idx);
}
