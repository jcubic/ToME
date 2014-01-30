/* File: cmd1.c */

/* Purpose: Movement commands (part 1) */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/*
 * Player "wants" to pick up an object or gold.
 * Note that we ONLY handle things that can be picked up.
 * See "move_player()" for handling of other things.
 */
void carry(s32b pickup)
{
	if (!has_flag(&p_ptr->body_monster, FLAG_NO_TAKE_ITEM))
	{
		py_pickup_floor(pickup);
	}
}


/*
 * Handle player hitting a real trap
 */
static void hit_trap(void)
{
	cave_type *c_ptr;

	/* Get the cave grid */
	c_ptr = &cave[p_ptr->py][p_ptr->px];

	/* At least one thing to check */
	if (get_num_location_traps(p_ptr->py, p_ptr->px, FALSE, FACTION_PLAYER,
							   FALSE, FALSE) > 0)
		if (activate_location_traps(0, p_ptr->py, p_ptr->px, FALSE))
			disturb(0, 0);
} /* hit_trap() */

/*
 * Player attacks a (poor, defenseless) creature        -RAK-
 *
 * If no "weapon" is available, then "punch" the monster one time.
 */
bool py_attack(s32b y, s32b x, s32b max_blow, s32b dir)
{
	s32b ret;
	call_lua("call_attack", "(d,d,d,dir)", "d", y, x, max_blow, dir, &ret);
	return ret;
}


bool player_can_enter(byte feature)
{
	/* Don't let the player kill himself with one keystroke */
	if (p_ptr->wild_mode)
	{
		/* Specific enter test function ? */
		if (f_info[feature].can_enter != -1)
		{
			s32b ret;
			open_lua_functions_registry(get_lua_functions_registry_domain("f_info"), f_info[feature].can_enter);
			call_lua(NULL, "(s)", "d", "map", &ret);
			close_lua_functions_registry();
			return ret;
		}
	}

	/* Specific enter test function ? */
	if (f_info[feature].can_enter != -1)
	{
		s32b ret;
		open_lua_functions_registry(get_lua_functions_registry_domain("f_info"), f_info[feature].can_enter);
		call_lua(NULL, "()", "d", &ret);
		close_lua_functions_registry();
		return ret;
	}

	/* Some basic rules */
	if ((intrinsic(CLIMB)) && has_flag(&f_info[feature], FLAG_CAN_CLIMB))
		return (TRUE);
	else if (can_pass_aux(&f_info[feature].flags, &p_ptr->intrinsic, NULL))
		return (TRUE);
	else if (can_pass_aux(&f_info[feature].flags, &p_ptr->flags, NULL))
		return (TRUE);

	return (FALSE);
}

/*
 * Move player in the given direction, with the given "pickup" flag.
 *
 * This routine should (probably) always induce energy expenditure.
 *
 * Note that moving will *always* take a turn, and will *always* hit
 * any monster which might be in the destination grid.  Previously,
 * moving into walls was "free" and did NOT hit invisible monsters.
 */
void move_player_aux(s32b dir, s32b do_pickup, s32b run, bool disarm)
{
	s32b y, x, tmp;
	cave_type *c_ptr = &cave[p_ptr->py][p_ptr->px];

	monster_type *m_ptr;
	monster_type *body = &p_ptr->body_monster;

	bool old_dtrap, new_dtrap;

	bool oktomove = TRUE;

	/* Hack - random movement */
	if ((has_flag(body, FLAG_RAND_MOVEMENT)))
	{
		if (randint(100) < get_flag(body, FLAG_RAND_MOVEMENT))
			tmp = randint(9);
		else
			tmp = dir;
	}
	else
	{
		tmp = dir;
	}

	/* Let script change the direction */
	if (process_hooks_ret(HOOK_MOVE_PRE_DIR, "d", "(d,d,d,d)", tmp,
						  do_pickup, run, disarm))
		tmp = process_hooks_return[0].num;

	/* Find the result of moving */
	y = p_ptr->py + ddy[tmp];
	x = p_ptr->px + ddx[tmp];

	/* Let script prevent movement */
	if (process_hooks(HOOK_MOVE_PRE, "(d,d,d,d,d,d)", y, x, tmp,
					  do_pickup, run, disarm))
		return;

	/* Examine the destination */
	c_ptr = &cave[y][x];

	/* Get the monster */
	m_ptr = get_monster(c_ptr->m_idx);

	/* Let non-moving players attack monsters. */
	if (intrinsic(CANNOT_MOVE) && c_ptr->m_idx == 0)
	{
		msg_print("You are unable to move.");
		return;
	}

	/* Place the player well when going back to big mode */
	if (p_ptr->wild_mode)
	{
		p_ptr->wild_force_side_y_ok = TRUE;
		p_ptr->wild_force_side_x_ok = TRUE;
		if (ddy[tmp] > 0) p_ptr->wild_force_side_y = -1;
		if (ddy[tmp] < 0) p_ptr->wild_force_side_y = 1;
		if (ddy[tmp] == 0) p_ptr->wild_force_side_y = 0;
		if (ddx[tmp] > 0) p_ptr->wild_force_side_x = -1;
		if (ddx[tmp] < 0) p_ptr->wild_force_side_x = 1;
		if (ddx[tmp] == 0) p_ptr->wild_force_side_x = 0;
	}

	/* Exit the area */
	if (!dun_level && !p_ptr->wild_mode && !is_quest(dun_level) &&
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

	/* Hack -- attack monsters */
	if (c_ptr->m_idx && m_ptr && (m_ptr->ml || player_can_enter(c_ptr->feat)))
	{
		oktomove = py_attack(y, x, -1, dir);
	}

	/*
	 * Disarm a visible trap, but don't disarm visible traps that
	 * belong to the player.
	 */
	else if (easy_disarm && disarm && (c_ptr->info & (CAVE_TRDT)) &&
			 (get_num_location_traps(y, x, TRUE, FACTION_PLAYER,
									 FALSE, TRUE) > 0))
	{
		(void)do_cmd_disarm_aux(y, x, tmp, do_pickup);
		return;
	}

	/* Player can't enter ? soo bad for him/her ... */
	else if (!player_can_enter(c_ptr->feat))
	{
		oktomove = FALSE;

		/* Disturb the player */
		disturb(0, 0);

		/* For things like probability travel. */
		if (process_hooks(HOOK_HIT_WALL, "(d,d,d)", y, x, tmp))
			return;

		/* Notice things in the dark */
		if (!(c_ptr->info & (CAVE_MARK)) && !(c_ptr->info & (CAVE_SEEN)))
		{
			s32b feat;

			if (c_ptr->mimic) feat = c_ptr->mimic;
			else
				feat = f_info[c_ptr->feat].mimic;

			msg_format("You feel %s.", f_info[feat].block);
			c_ptr->info |= (CAVE_MARK);
			lite_spot(y, x);
		}

		/* Notice things */
		else
		{
			s32b feat;

			if (c_ptr->mimic)
				feat = c_ptr->mimic;
			else
				feat = f_info[c_ptr->feat].mimic;

			msg_format("There is %s.", f_info[feat].block);

			if (!(intrinsic(CONFUSED) || intrinsic(HALLUCINATE)))
				energy_use = 0;
		}

		/* Sound */
		sound(SOUND_HITWALL);
	}


	/*
	 * Check trap detection status -- retrieve them here
	 * because they are used by the movement code as well
	 */
	old_dtrap = ((cave[p_ptr->py][p_ptr->px].info & CAVE_DETECT) != 0);
	new_dtrap = ((cave[y][x].info & CAVE_DETECT) != 0);

	/* Normal movement */
	if (oktomove && running && disturb_detect)
	{
		/*
		 * Disturb the player when about to leave the trap detected
		 * area
		 */
		if (old_dtrap && !new_dtrap)
		{
			/* Disturb player */
			disturb(0, 0);

			/* but don't take a turn */
			energy_use = 0;

			/* Tell player why */
			cmsg_print(TERM_VIOLET, "You are about to leave a trap detected zone.");
			/* Flush */
			/* msg_print(NULL); */

			oktomove = FALSE;
		}
	}

	/* Normal movement */
	if (oktomove)
	{
		s32b oy, ox;
		s32b feat;

		/* Save old location */
		oy = p_ptr->py;
		ox = p_ptr->px;

		/* Move the player */
		p_ptr->py = y;
		p_ptr->px = x;

		/*
		 * Inform script that move is okay (before stuff like
		 * auto-pickup is done)
		 */
		process_hooks(HOOK_MOVE_OK, "(d,d,d,d,d,d)", oy, ox, tmp,
					  do_pickup, run, disarm);

		c_ptr = &cave[p_ptr->py][p_ptr->px];

		if (c_ptr->mimic)
			feat = c_ptr->mimic;
		else
			feat = c_ptr->feat;

		/* Redraw new spot */
		lite_spot(p_ptr->py, p_ptr->px);

		/* Redraw old spot */
		lite_spot(oy, ox);

		/* Sound */
		/* sound(SOUND_WALK); */

		/* Check for new panel (redraw map) */
		verify_panel();

		/* Check detection status */
		if (old_dtrap && !new_dtrap)
		{
			cmsg_print(TERM_VIOLET, "You leave a trap detected zone.");
			if (running) msg_print(NULL);
			flag_bool(&p_ptr->redraw, FLAG_PR_DTRAP);
		}
		else if (!old_dtrap && new_dtrap)
		{
			cmsg_print(TERM_L_BLUE, "You enter a trap detected zone.");
			if (running) msg_print(NULL);
			flag_bool(&p_ptr->redraw, FLAG_PR_DTRAP);
		}

		/* Update stuff */
		p_ptr->update |= (PU_VIEW | PU_FLOW | PU_MON_LITE);

		/* Update the monsters */
		p_ptr->update |= (PU_DISTANCE);

		/* Window stuff */
		if (!run)
			flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);

		/* Some feature descs */
		if (strcmp(f_info[c_ptr->feat].text, "") ||
			has_flag(c_ptr, FLAG_TERRAIN_TEXT))
		{
			cptr text = NULL;

			if (has_flag(c_ptr, FLAG_TERRAIN_TEXT))
				text = flag_get_string(&c_ptr->flags, FLAG_TERRAIN_TEXT);

			/* Mega-hack for dungeon branches */
			if (cave_feat_is(c_ptr, FLAG_LEVEL_CHANGER) &&
				has_flag(c_ptr, FLAG_LEVEL_CHANGER))
			{
				s32b branch = get_flag(c_ptr, FLAG_LEVEL_CHANGER);

				if (text != NULL)
					msg_format("There is %s", text);
				else
					msg_format("There is %s", d_info[branch].text);
			}
			else
			{
				if (text != NULL)
					msg_print(text);
				else
					msg_print(f_info[feat].text);
			}

			/* Flush message while running */
			if (running) msg_print(NULL);
		}

		/* Handle "objects" */
		carry(do_pickup);

		/* Handle "store doors" */
		if (cave_feat_is(c_ptr, FLAG_CONTAINS_BUILDING))
		{
			/* Disturb */
			disturb(0, 0);

			/* Hack -- Enter store */
			command_new = '_';
			energy_use = 0;
		}

		/* Discover invisible traps */
		else if (flag_used(&c_ptr->activations) &&
				 !cave_feat_is(c_ptr, FLAG_DOOR))
			hit_trap();

		/* Inform script that move has finished */
		process_hooks(HOOK_MOVE_POST, "(d,d,d,d,d,d)", oy, ox, tmp,
					  do_pickup, run, disarm);
	}

	/* Update wilderness knowledge */
	if (p_ptr->wild_mode)
	{
		if (wizard) msg_format("y:%d, x:%d", p_ptr->py, p_ptr->px);

		/* Update the known wilderness */
		reveal_wilderness_around_player(p_ptr->py, p_ptr->px, 0, WILDERNESS_SEE_RADIUS);

		/* Walking the wild isnt meaningfull */
		p_ptr->did_nothing = TRUE;
	}
}

void move_player(s32b dir, s32b do_pickup)
{
	move_player_aux(dir, do_pickup, 0, TRUE);
}


/*
 * Hack -- Grid-based version of see_obstacle
 */
static s32b see_obstacle_grid(cave_type *c_ptr)
{
	/*
	 * Hack -- Avoid hitting detected traps, because we cannot rely on
	 * the CAVE_MARK check below, and traps can be set to nearly
	 * everything the player can move on to XXX XXX XXX
	 */
	if (c_ptr->info & (CAVE_TRDT)) return (TRUE);

	/* Hack -- Handle special cases XXX XXX */
	if (cave_feat_is(c_ptr, FLAG_CAN_FLY) &&
		(cave_feat_get(c_ptr, FLAG_CAN_FLY) <= intrinsic(FLY)))
		return (FALSE);

	/* "Safe" floor grids aren't obstacles */
	if (has_flag(&f_info[c_ptr->feat], FLAG_CAN_RUN)) return (FALSE);

	/* Must be known to the player */
	if (!(c_ptr->info & (CAVE_MARK))) return (FALSE);

	/* Default */
	return (TRUE);
}


/*
 * Hack -- Check for a "known wall" or "dangerous" feature (see below)
 */
static s32b see_obstacle(s32b dir, s32b y, s32b x)
{
	/* Get the new location */
	y += ddy[dir];
	x += ddx[dir];

	/* Illegal grids are not known walls */
	if (!in_bounds2(y, x)) return (FALSE);

	/* Analyse the grid */
	return (see_obstacle_grid(&cave[y][x]));
}


/*
 * Hack -- Check for an "unknown corner" (see below)
 */
static s32b see_nothing(s32b dir, s32b y, s32b x)
{
	/* Get the new location */
	y += ddy[dir];
	x += ddx[dir];

	/* Illegal grids are unknown */
	if (!in_bounds2(y, x)) return (TRUE);

	/* Memorized grids are always known */
	if (cave[y][x].info & (CAVE_MARK)) return (FALSE);

	/* Non-floor grids are unknown */
	if (!cave_floor_bold(y, x)) return (TRUE);

	/* Viewable door/wall grids are known */
	if (player_can_see_bold(y, x)) return (FALSE);

	/* Default */
	return (TRUE);
}





/*
 * The running algorithm:                       -CJS-
 *
 * In the diagrams below, the player has just arrived in the
 * grid marked as '@', and he has just come from a grid marked
 * as 'o', and he is about to enter the grid marked as 'x'.
 *
 * Of course, if the "requested" move was impossible, then you
 * will of course be blocked, and will stop.
 *
 * Overview: You keep moving until something interesting happens.
 * If you are in an enclosed space, you follow corners. This is
 * the usual corridor scheme. If you are in an open space, you go
 * straight, but stop before entering enclosed space. This is
 * analogous to reaching doorways. If you have enclosed space on
 * one side only (that is, running along side a wall) stop if
 * your wall opens out, or your open space closes in. Either case
 * corresponds to a doorway.
 *
 * What happens depends on what you can really SEE. (i.e. if you
 * have no light, then running along a dark corridor is JUST like
 * running in a dark room.) The algorithm works equally well in
 * corridors, rooms, mine tailings, earthquake rubble, etc, etc.
 *
 * These conditions are kept in static memory:
 * find_openarea         You are in the open on at least one
 * side.
 * find_breakleft        You have a wall on the left, and will
 * stop if it opens
 * find_breakright       You have a wall on the right, and will
 * stop if it opens
 *
 * To initialize these conditions, we examine the grids adjacent
 * to the grid marked 'x', two on each side (marked 'L' and 'R').
 * If either one of the two grids on a given side is seen to be
 * closed, then that side is considered to be closed. If both
 * sides are closed, then it is an enclosed (corridor) run.
 *
 * LL           L
 * @x          LxR
 * RR          @R
 *
 * Looking at more than just the immediate squares is
 * significant. Consider the following case. A run along the
 * corridor will stop just before entering the center point,
 * because a choice is clearly established. Running in any of
 * three available directions will be defined as a corridor run.
 * Note that a minor hack is inserted to make the angled corridor
 * entry (with one side blocked near and the other side blocked
 * further away from the runner) work correctly. The runner moves
 * diagonally, but then saves the previous direction as being
 * straight into the gap. Otherwise, the tail end of the other
 * entry would be perceived as an alternative on the next move.
 *
 * #.#
 * ##.##
 * .@x..
 * ##.##
 * #.#
 *
 * Likewise, a run along a wall, and then into a doorway (two
 * runs) will work correctly. A single run rightwards from @ will
 * stop at 1. Another run right and down will enter the corridor
 * and make the corner, stopping at the 2.
 *
 * #@x    1
 * ########### ######
 * 2        #
 * #############
 * #
 *
 * After any move, the function area_affect is called to
 * determine the new surroundings, and the direction of
 * subsequent moves. It examines the current player location
 * (at which the runner has just arrived) and the previous
 * direction (from which the runner is considered to have come).
 *
 * Moving one square in some direction places you adjacent to
 * three or five new squares (for straight and diagonal moves
 * respectively) to which you were not previously adjacent,
 * marked as '!' in the diagrams below.
 *
 * ...!   ...
 * .o@!   .o.!
 * ...!   ..@!
 * !!!
 *
 * You STOP if any of the new squares are interesting in any way:
 * for example, if they contain visible monsters or treasure.
 *
 * You STOP if any of the newly adjacent squares seem to be open,
 * and you are also looking for a break on that side. (that is,
 * find_openarea AND find_break).
 *
 * You STOP if any of the newly adjacent squares do NOT seem to be
 * open and you are in an open area, and that side was previously
 * entirely open.
 *
 * Corners: If you are not in the open (i.e. you are in a corridor)
 * and there is only one way to go in the new squares, then turn in
 * that direction. If there are more than two new ways to go, STOP.
 * If there are two ways to go, and those ways are separated by a
 * square which does not seem to be open, then STOP.
 *
 * Otherwise, we have a potential corner. There are two new open
 * squares, which are also adjacent. One of the new squares is
 * diagonally located, the other is straight on (as in the diagram).
 * We consider two more squares further out (marked below as ?).
 *
 * We assign "option" to the straight-on grid, and "option2" to the
 * diagonal grid, and "check_dir" to the grid marked 's'.
 *
 * .s
 * @x?
 * #?
 *
 * If they are both seen to be closed, then it is seen that no
 * benefit is gained from moving straight. It is a known corner.
 * To cut the corner, go diagonally, otherwise go straight, but
 * pretend you stepped diagonally into that next location for a
 * full view next time. Conversely, if one of the ? squares is
 * not seen to be closed, then there is a potential choice. We check
 * to see whether it is a potential corner or an intersection/room entrance.
 * If the square two spaces straight ahead, and the space marked with 's'
 * are both blank, then it is a potential corner and enter if find_examine
 * is set, otherwise must stop because it is not a corner.
 */




/*
 * Hack -- allow quick "cycling" through the legal directions
 */
static byte cycle[] = { 1, 2, 3, 6, 9, 8, 7, 4, 1, 2, 3, 6, 9, 8, 7, 4, 1 };

/*
 * Hack -- map each direction into the "middle" of the "cycle[]" array
 */
static byte chome[] = { 0, 8, 9, 10, 7, 0, 11, 6, 5, 4 };

/*
 * The direction we are running
 */
static byte find_current;

/*
 * The direction we came from
 */
static byte find_prevdir;

/*
 * We are looking for open area
 */
static bool find_openarea;

/*
 * We are looking for a break
 */
static bool find_breakright;
static bool find_breakleft;



/*
 * Initialize the running algorithm for a new direction.
 *
 * Diagonal Corridor -- allow diaginal entry into corridors.
 *
 * Blunt Corridor -- If there is a wall two spaces ahead and
 * we seem to be in a corridor, then force a turn into the side
 * corridor, must be moving straight into a corridor here. ???
 *
 * Diagonal Corridor    Blunt Corridor (?)
 *       # #                  #
 *       #x#                 @x#
 *       @p.                  p
 */
static void run_init(s32b dir)
{
	s32b row, col, deepleft, deepright;

	s32b i, shortleft, shortright;


	/* Save the direction */
	find_current = dir;

	/* Assume running straight */
	find_prevdir = dir;

	/* Assume looking for open area */
	find_openarea = TRUE;

	/* Assume not looking for breaks */
	find_breakright = find_breakleft = FALSE;

	/* Assume no nearby walls */
	deepleft = deepright = FALSE;
	shortright = shortleft = FALSE;

	/* Find the destination grid */
	row = p_ptr->py + ddy[dir];
	col = p_ptr->px + ddx[dir];

	/* Extract cycle index */
	i = chome[dir];

	/* Check for walls */
	if (see_obstacle(cycle[i + 1], p_ptr->py, p_ptr->px))
	{
		find_breakleft = TRUE;
		shortleft = TRUE;
	}
	else if (see_obstacle(cycle[i + 1], row, col))
	{
		find_breakleft = TRUE;
		deepleft = TRUE;
	}

	/* Check for walls */
	if (see_obstacle(cycle[i - 1], p_ptr->py, p_ptr->px))
	{
		find_breakright = TRUE;
		shortright = TRUE;
	}
	else if (see_obstacle(cycle[i - 1], row, col))
	{
		find_breakright = TRUE;
		deepright = TRUE;
	}

	/* Looking for a break */
	if (find_breakleft && find_breakright)
	{
		/* Not looking for open area */
		find_openarea = FALSE;

		/* Hack -- allow angled corridor entry */
		if (dir & 0x01)
		{
			if (deepleft && !deepright)
			{
				find_prevdir = cycle[i - 1];
			}
			else if (deepright && !deepleft)
			{
				find_prevdir = cycle[i + 1];
			}
		}

		/* Hack -- allow blunt corridor entry */
		else if (see_obstacle(cycle[i], row, col))
		{
			if (shortleft && !shortright)
			{
				find_prevdir = cycle[i - 2];
			}
			else if (shortright && !shortleft)
			{
				find_prevdir = cycle[i + 2];
			}
		}
	}
}


/*
 * Update the current "run" path
 *
 * Return TRUE if the running should be stopped
 */
static bool run_test(void)
{
	s32b prev_dir, new_dir, check_dir = 0;

	s32b row, col;

	s32b i, max, inv;

	s32b option = 0, option2 = 0;

	cave_type *c_ptr;


	/* Where we came from */
	prev_dir = find_prevdir;


	/* Range of newly adjacent grids */
	max = (prev_dir & 0x01) + 1;


	/* Look at every newly adjacent square. */
	for (i = -max; i <= max; i++)
	{
		/* New direction */
		new_dir = cycle[chome[prev_dir] + i];

		/* New location */
		row = p_ptr->py + ddy[new_dir];
		col = p_ptr->px + ddx[new_dir];

		/* Access grid */
		c_ptr = &cave[row][col];


		/* Visible monsters abort running */
		if (c_ptr->m_idx)
		{
			monster_type *m_ptr = get_monster(c_ptr->m_idx);

			/* Visible monster */
			if (m_ptr->ml) return (TRUE);
		}

		/* Visible objects abort running */
		for_inventory_slot(&c_ptr->inventory, o_ptr);
		{
			/* Visible object */
			if (o_ptr->marked) return (TRUE);
		}
		end_inventory_slot();

		/* Assume unknown */
		inv = TRUE;

		/* Check memorized grids */
		if (c_ptr->info & (CAVE_MARK))
		{
			bool notice = TRUE;

			if (cave_feat_is(c_ptr, FLAG_CAN_FLY) &&
				(cave_feat_get(c_ptr, FLAG_CAN_FLY) <= intrinsic(FLY)))
				notice = FALSE;

			/*
			 * DGDGDGDG -- Here lied the find_door/stairs options
			 */

			/* Check the "don't notice running" flag */
			if (has_flag(&f_info[c_ptr->feat], FLAG_DONT_NOTICE_RUNNING))
			{
				notice = FALSE;
			}

			/* A detected trap is interesting */
			if (c_ptr->info & (CAVE_TRDT)) notice = TRUE;

			/* Interesting feature */
			if (notice) return (TRUE);

			/* The grid is "visible" */
			inv = FALSE;
		}

		/* Mega-Hack -- Maze code removes CAVE_MARK XXX XXX XXX */
		if (c_ptr->info & (CAVE_TRDT)) return (TRUE);

		/* Analyze unknown grids and floors */
		if (inv || cave_floor_bold(row, col))
		{
			/* Looking for open area */
			if (find_openarea)
			{
				/* Nothing */
			}

			/* The first new direction. */
			else if (!option)
			{
				option = new_dir;
			}

			/* Three new directions. Stop running. */
			else if (option2)
			{
				return (TRUE);
			}

			/* Two non-adjacent new directions.  Stop running. */
			else if (option != cycle[chome[prev_dir] + i - 1])
			{
				return (TRUE);
			}

			/* Two new (adjacent) directions (case 1) */
			else if (new_dir & 0x01)
			{
				check_dir = cycle[chome[prev_dir] + i - 2];
				option2 = new_dir;
			}

			/* Two new (adjacent) directions (case 2) */
			else
			{
				check_dir = cycle[chome[prev_dir] + i + 1];
				option2 = option;
				option = new_dir;
			}
		}

		/* Obstacle, while looking for open area */
		else
		{
			if (find_openarea)
			{
				if (i < 0)
				{
					/* Break to the right */
					find_breakright = TRUE;
				}

				else if (i > 0)
				{
					/* Break to the left */
					find_breakleft = TRUE;
				}
			}
		}
	}


	/* Looking for open area */
	if (find_openarea)
	{
		/* Hack -- look again */
		for (i = -max; i < 0; i++)
		{
			new_dir = cycle[chome[prev_dir] + i];

			row = p_ptr->py + ddy[new_dir];
			col = p_ptr->px + ddx[new_dir];

			/* Access grid */
			c_ptr = &cave[row][col];

			/* Unknown grids or non-obstacle */
			if (!see_obstacle_grid(c_ptr))
			{
				/* Looking to break right */
				if (find_breakright)
				{
					return (TRUE);
				}
			}

			/* Obstacle */
			else
			{
				/* Looking to break left */
				if (find_breakleft)
				{
					return (TRUE);
				}
			}
		}

		/* Hack -- look again */
		for (i = max; i > 0; i--)
		{
			new_dir = cycle[chome[prev_dir] + i];

			row = p_ptr->py + ddy[new_dir];
			col = p_ptr->px + ddx[new_dir];

			/* Access grid */
			c_ptr = &cave[row][col];

			/* Unknown grid or non-obstacle */
			if (!see_obstacle_grid(c_ptr))
			{
				/* Looking to break left */
				if (find_breakleft)
				{
					return (TRUE);
				}
			}

			/* Obstacle */
			else
			{
				/* Looking to break right */
				if (find_breakright)
				{
					return (TRUE);
				}
			}
		}
	}


	/* Not looking for open area */
	else
	{
		/* No options */
		if (!option)
		{
			return (TRUE);
		}

		/* One option */
		else if (!option2)
		{
			/* Primary option */
			find_current = option;

			/* No other options */
			find_prevdir = option;
		}

		/* Two options, examining corners */
		else if (find_examine && !find_cut)
		{
			/* Primary option */
			find_current = option;

			/* Hack -- allow curving */
			find_prevdir = option2;
		}

		/* Two options, pick one */
		else
		{
			/* Get next location */
			row = p_ptr->py + ddy[option];
			col = p_ptr->px + ddx[option];

			/* Don't see that it is closed off. */
			/* This could be a potential corner or an intersection. */
			if (!see_obstacle(option, row, col) || !see_obstacle(check_dir, row, col))
			{
				/* Can not see anything ahead and in the direction we */
				/* are turning, assume that it is a potential corner. */
				if (find_examine &&
				                see_nothing(option, row, col) &&
				                see_nothing(option2, row, col))
				{
					find_current = option;
					find_prevdir = option2;
				}

				/* STOP: we are next to an intersection or a room */
				else
				{
					return (TRUE);
				}
			}

			/* This corner is seen to be enclosed; we cut the corner. */
			else if (find_cut)
			{
				find_current = option2;
				find_prevdir = option2;
			}

			/* This corner is seen to be enclosed, and we */
			/* deliberately go the long way. */
			else
			{
				find_current = option;
				find_prevdir = option2;
			}
		}
	}


	/* About to hit a known wall, stop */
	if (see_obstacle(find_current, p_ptr->py, p_ptr->px))
	{
		return (TRUE);
	}


	/* Failure */
	return (FALSE);
}



/*
 * Take one step along the current "run" path
 */
void run_step(s32b dir)
{
	/* Start running */
	if (dir)
	{
		/* Hack -- do not start silly run */
		if (see_obstacle(dir, p_ptr->py, p_ptr->px))
		{
			/* Message */
			msg_print("You cannot run in that direction.");

			/* Disturb */
			disturb(0, 0);

			/* Done */
			return;
		}

		/* Calculate torch radius */
		p_ptr->update |= (PU_TORCH);

		/* Initialize */
		run_init(dir);
	}

	/* Keep running */
	else
	{
		/* Update run */
		if (run_test())
		{
			/* Disturb */
			disturb(0, 0);

			/* Done */
			return;
		}
	}

	/* Decrease the run counter */
	if (--running <= 0) return;

	/* Take time */
	energy_use = get_player_energy(SPEED_WALK);


	/* Move the player, using the "pickup" flag */
	move_player_aux(find_current, always_pickup, 1, TRUE);
}

/*
 * Choose an inscription and engrave it
 */
void do_cmd_engrave()
{
#if 0 //DGDGDGDG
	char buf[41] = "";

	byte i;

	strnfmt(buf, 41, "%s", inscription_info[cave[p_ptr->py][p_ptr->px].inscription].text);

	get_string("Engrave what? ", buf, 40);

	/* Silently do nothing when player his escape or enters an empty string */
	if (!buf[0]) return;

	for (i = 0; i < MAX_INSCRIPTIONS; i++)
	{
		if (!strcmp(inscription_info[i].text, buf))
		{
			if (inscription_info[i].know)
			{
				/* Save the inscription */
				cave[p_ptr->py][p_ptr->px].inscription = i;
			}
			else
				msg_print("You can't use this inscription for now.");
		}
	}

	/* Execute the inscription */
	if (inscription_info[cave[p_ptr->py][p_ptr->px].inscription].when & INSCRIP_EXEC_ENGRAVE)
	{
		execute_inscription(cave[p_ptr->py][p_ptr->px].inscription, p_ptr->py, p_ptr->px);
	}

	energy_use = get_player_energy(SPEED_ENGRAVE);
#endif
}


/*
 * Let's do a spinning around attack:                   -- DG --
 *     aDb
 *     y@k
 *     ooT
 * Ah ... all of those will get hit.
 */
void do_spin()
{
	s32b i, j;


	msg_print("You start spinning around...");

	for (j = p_ptr->py - 1; j <= p_ptr->py + 1; j++)
	{
		for (i = p_ptr->px - 1; i <= p_ptr->px + 1; i++)
		{
			/* Avoid stupid bugs */
			if (in_bounds(j, i) && cave[j][i].m_idx)
				py_attack(j, i, 1, -1);
		}
	}
}
