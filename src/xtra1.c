/* File: misc.c */

/* Purpose: misc code */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"


/*
 * Converts stat num into a six-char (right justified) string
 */
void cnv_stat(s32b val, char *out_val)
{
	sprintf(out_val, "    %2ld", val);
}



/*
 * Modify a stat value by a "modifier", return new value
 *
 * Stats go up: 3,4,...,17,18,18/10,18/20,...,18/220
 * Or even: 18/13, 18/23, 18/33, ..., 18/220
 *
 * Stats go down: 18/220, 18/210,..., 18/10, 18, 17, ..., 3
 * Or even: 18/13, 18/03, 18, 17, ..., 3
 */
s16b modify_stat_value(s32b value, s32b amount)
{
	value += amount;

	if (value < MIN_STAT_VALUE) value = MIN_STAT_VALUE;
	if (value >= MAX_STAT_VALUE) value = MAX_STAT_VALUE;

	/* Return new value */
	return (value);
}


/*
 * Hack -- display recent messages in sub-windows
 *
 * XXX XXX XXX Adjust for width and split messages
 */
void fix_message(void)
{
	s32b j, i;
	s32b w, h;
	s32b x, y;

	/* Scan windows */
	for (j = 0; j < 8; j++)
	{
		term *old = Term;

		/* No window */
		if (!angband_term[j]) continue;

		/* No relevant flags */
		if (!flag_exists(&window_flag[j], FLAG_PW_MESSAGE)) continue;

		/* Activate */
		Term_activate(angband_term[j]);

		/* Get size */
		Term_get_size(&w, &h);

		/* Dump messages */
		for (i = 0; i < h; i++)
		{
			/* Dump the message on the appropriate line */
			display_message(0, (h - 1) - i, strlen(message_str((s16b)i)), message_color((s16b)i), message_str((s16b)i));

			/* Cursor */
			Term_locate(&x, &y);

			/* Clear to end of line */
			Term_erase(x, y, 255);
		}

		/* Fresh */
		Term_fresh();

		/* Restore */
		Term_activate(old);
	}
}

/*
 * Hack -- display monster recall in sub-windows
 */
static void fix_monster(void)
{
	s32b j;

	/* Scan windows */
	for (j = 0; j < 8; j++)
	{
		term *old = Term;

		/* No window */
		if (!angband_term[j]) continue;

		/* No relevant flags */
		if (!flag_exists(&window_flag[j], FLAG_PW_MONSTER)) continue;

		/* Activate */
		Term_activate(angband_term[j]);

		/* Display monster race info */
		if (monster_race_idx) display_roff(monster_race_idx, monster_ego_idx);

		/* Fresh */
		Term_fresh();

		/* Restore */
		Term_activate(old);
	}
}


/*
 * Hack -- display object recall in sub-windows
 */
static void fix_object(void)
{
	s32b j;

	/* Scan windows */
	for (j = 0; j < 8; j++)
	{
		term *old = Term;

		/* No window */
		if (!angband_term[j]) continue;

		/* No relevant flags */
		if (!flag_exists(&window_flag[j], FLAG_PW_OBJECT)) continue;

		/* Activate */
		Term_activate(angband_term[j]);

		/* Clear */
		Term_clear();

		/* Display object info */
		if (tracked_object)
			if (!object_out_desc(tracked_object, NULL, FALSE, FALSE)) text_out("You see nothing special.");

		/* Fresh */
		Term_fresh();

		/* Restore */
		Term_activate(old);
	}
}

/* Show the monster list in a window */

static void fix_m_list(void)
{
	s32b i, j;

	/* Scan windows */
	for (j = 0; j < 8; j++)
	{
		term *old = Term;

		s32b c = 0;

		/* No window */
		if (!angband_term[j]) continue;

		/* No relevant flags */
		if (!flag_exists(&window_flag[j], FLAG_PW_M_LIST)) continue;

		/* Activate */
		Term_activate(angband_term[j]);

		/* Clear */
		Term_clear();

		/* Hallucination */
		if (intrinsic(HALLUCINATE))
		{
			c_prt(TERM_WHITE, "You can not see clearly", 0, 0);

			/* Fresh */
			Term_fresh();

			/* Restore */
			Term_activate(old);

			return;
		}

		/* reset visible count */
		for (i = 1; i < max_r_idx; i++)
		{
			monster_race *r_ptr = &r_info[i];

			r_ptr->total_visible = 0;
		}

		/* Count up the number visible in each race */
		for_flags(&monst_list);
		{
			i = __key;
			monster_type *m_ptr = get_monster(i);
			monster_race *r_ptr = &r_info[m_ptr->r_idx];
			object_type  *o_ptr = get_obj_mimic_obj_at(m_ptr->fy,
													   m_ptr->fx);

			/* Skip dead monsters */
			if (m_ptr->hp < 0) continue;

			/* Skip unseen monsters */
			if (o_ptr != NULL && !o_ptr->marked)
			{
				/* Memorized objects */
				if (!o_ptr->marked) continue;
			}
			else
				if (!m_ptr->ml) continue;

			/* Increase for this race */
			r_ptr->total_visible++;

			/* Increase total Count */
			c++;
		}
		end_for_flags();

		/* Are monsters visible? */
		if (c)
		{
			s32b w, h, num = 0;

			(void)Term_get_size(&w, &h);

			c_prt(TERM_WHITE, format("You can see %d monster%s", c, (c > 1 ? "s:" : ":")), 0, 0);

			for (i = 1; i < max_r_idx; i++)
			{
				monster_race *r_ptr = &r_info[i];

				/* Default Colour */
				byte attr = TERM_SLATE;

				/* Only visible monsters */
				if (!r_ptr->total_visible) continue;

				/* Uniques */
				if (has_flag(r_ptr, FLAG_UNIQUE))
				{
					attr = TERM_L_BLUE;
				}

				/* Have we ever killed one? */
				if (r_ptr->r_tkills)
				{
					if (r_ptr->level > dun_level)
					{
						attr = TERM_VIOLET;

						if (has_flag(r_ptr, FLAG_UNIQUE))
						{
							attr = TERM_RED;
						}
					}
				}
				else
				{
					if (!has_flag(r_ptr, FLAG_UNIQUE)) attr = TERM_GREEN;
				}


				/* Dump the monster name */
				if (r_ptr->total_visible == 1)
				{
					c_prt(attr, (r_ptr->name), (num % (h - 1)) + 1, (num / (h - 1) * 26));
				}
				else
				{
					c_prt(attr, format("%s (x%d)", r_ptr->name, r_ptr->total_visible), (num % (h - 1)) + 1, (num / (h - 1)) * 26);
				}

				num++;

			}

		}
		else
		{
			c_prt(TERM_WHITE, "You see no monsters.", 0, 0);
		}

		/* Fresh */
		Term_fresh();

		/* Restore */
		Term_activate(old);
	}
}

/*
 * Extract and set the current "lite radius"
 *
 * SWD: Experimental modification: multiple light sources have additive effect.
 *
 */
static void calc_torch(void)
{
	/* Assume no light */
	p_ptr->cur_lite = 0;

	/* Loop through all wielded items */
	for_inventory(p_ptr, o_ptr, INVEN_PACK, INVEN_TOTAL);
	{
		/* does this item glow? */
		if ((has_flag(o_ptr, FLAG_FUEL_LITE) && (get_flag(o_ptr, FLAG_FUEL) > 0)) ||
			(!has_flag(o_ptr, FLAG_FUEL_LITE)))
		{
			p_ptr->cur_lite += get_flag(o_ptr, FLAG_LITE);
		}
	}
	end_inventory();

	/* max radius is 5 without rewriting other code -- */
	/* see cave.c:update_lite() and defines.h:LITE_MAX */
	if (p_ptr->cur_lite > 5) p_ptr->cur_lite = 5;

	/* check if the player doesn't have a lite source, */
	/* but does glow as an intrinsic.                  */
	if (p_ptr->cur_lite == 0 && intrinsic(LITE)) p_ptr->cur_lite = 1;

	/* Hooked powers */
	process_hooks(HOOK_CALC_LITE, "()");

	/* end experimental mods */

	/* Reduce lite in the small-scale wilderness map */
	if (p_ptr->wild_mode)
	{
		/* Reduce the lite radius if needed */
		if (p_ptr->cur_lite > WILDERNESS_SEE_RADIUS)
		{
			p_ptr->cur_lite = WILDERNESS_SEE_RADIUS;
		}
	}


	/* Reduce lite when running if requested */
	if (running && view_reduce_lite)
	{
		/* Reduce the lite radius if needed */
		if (p_ptr->cur_lite > 1) p_ptr->cur_lite = 1;
	}

	/* Notice changes in the "lite radius" */
	if (p_ptr->old_lite != p_ptr->cur_lite)
	{
		/* Update the view */
		p_ptr->update |= (PU_VIEW);

		/* Update the monsters */
		p_ptr->update |= (PU_MONSTERS);

		/* Remember the old lite */
		p_ptr->old_lite = p_ptr->cur_lite;
	}
}



/*
 * Computes current weight limit.
 */
s32b weight_limit(void)
{
	s32b i = 0;

	call_lua("player.weight_limit", "()", "d", &i);

	/* Return the result */
	return i;
}

/*
 * Calc which body parts the player have, based on the
 * monster he incarnate, note that that's bnot a hack
 * since body parts of the player when in it's own body
 * are also defined in r_info(monster 0)
 */
void calc_body()
{
	monster_type *body  = &p_ptr->body_monster;
	monster_race *r_ptr = &r_info[body->r_idx];
	s32b i;

	if (p_ptr->body_monster.r_idx == 0)
	{
		/* Setup for player race */
		for (i = 0; i < INVEN_TOTAL; i++)
		{
			/* Get the race/subrace slots */
			s32b max = flag_get(&p_ptr->descriptor.body_parts, i);
			if (max < 0) max = 0;

			flag_set(get_inven(p_ptr, i), INVEN_LIMIT_KEY, max);
		}
	}
	else
	{
		/* Setup for monster race */
		for (i = 0; i < INVEN_TOTAL; i++)
		{
			s32b max = flag_get(&r_ptr->body_parts, i);
			if (max < 0) max = 0;

			flag_set(get_inven(p_ptr, i), INVEN_LIMIT_KEY, max);
		}
	}

	/* Do we need more parts ? ;) */
	process_hooks(HOOK_BODY_PARTS, "()");

	/* Ok now if the player lost a body part, he must drop the object he had on it */
	for (i = 0; i < INVEN_TOTAL; i++)
	{
		flags_type *inven = get_inven(p_ptr, i);
		s32b j;

		/* For all objects that are in excess, drop them */
		for (j = flag_max_key(inven); j > inventory_limit_inven(inven); j--)
		{
			/* Drop it NOW ! */
			if (i > INVEN_INVEN)
				inven_takeoff(compute_slot(i, j), 255, TRUE, FALSE);
			else
				inven_drop(compute_slot(i, j), 255, p_ptr->py, p_ptr->px, FALSE);
		}
	}
}

/* Should be called by every calc_bonus call */
void calc_body_bonus()
{
	monster_type *body = &p_ptr->body_monster;

	/* If in the player body nothing have to be done */
	if (p_ptr->body_monster.r_idx == 0) return;

	p_ptr->ac += body->ac;
	p_ptr->pspeed = body->speed;

	if (has_flag(body, FLAG_NEVER_MOVE)) set_intrinsic(BLINK_MOVE, TRUE);
	if (has_flag(body, FLAG_REFLECTING)) set_intrinsic(REFLECT, TRUE);
	if (has_flag(body, FLAG_INVISIBLE)) inc_intrinsic(INVIS, 20);
	if (has_flag(body, FLAG_REGENERATE)) set_intrinsic(REGENERATE, TRUE);
	if (has_flag(body, FLAG_PASS_WALL)) set_intrinsic(WRAITH, TRUE);
	if (has_flag(body, FLAG_CAN_FLY)) set_intrinsic(FEATHER, TRUE);
	if (has_flag(body, FLAG_AQUATIC)) set_intrinsic(WATER_BREATH, TRUE);
}

/*
 * Calculate the players current "state", taking into account
 * not only race/class intrinsics, but also objects being worn
 * and temporary spell effects.
 *
 * See also calc_mana() and calc_hitpoints().
 *
 * Take note of the new "speed code", in particular, a very strong
 * player will start slowing down as soon as he reaches 150 pounds,
 * but not until he reaches 450 pounds will he be half as fast as
 * a normal kobold.  This both hurts and helps the player, hurts
 * because in the old days a player could just avoid 300 pounds,
 * and helps because now carrying 300 pounds is not very painful.
 *
 * The "weapon" and "bow" do *not* add to the bonuses to hit or to
 * damage, since that would affect non-combat things.  These values
 * are actually added in later, at the appropriate place.
 *
 * This function induces various "status" messages, unless silent is
 * TRUE.
 */
void calc_bonuses(bool silent)
{
	if (silent)
		call_lua("player.calc_bonuses", "(n)", "");
	else
		call_lua("player.calc_bonuses", "(b)", "", silent);
}



/*
 * Handle "p_ptr->notice"
 */
void notice_stuff(void)
{
	/* Notice stuff */
	if (!p_ptr->notice) return;

	/* Combine the pack */
	if (p_ptr->notice & (PN_COMBINE))
	{
		p_ptr->notice &= ~(PN_COMBINE);
		combine_pack();
	}

	/* Reorder the pack */
	if (p_ptr->notice & (PN_REORDER))
	{
		p_ptr->notice &= ~(PN_REORDER);
		reorder_pack();
	}
}


/*
 * Handle "p_ptr->update"
 */
void update_stuff(void)
{
	/* Update stuff */
	if (!p_ptr->update) return;


	if (p_ptr->update & (PU_BODY))
	{
		p_ptr->update &= ~(PU_BODY);
		calc_body();
	}

	if (p_ptr->update & (PU_BONUS))
	{
		p_ptr->update &= ~(PU_BONUS);
		calc_bonuses(FALSE);
	}

	if (p_ptr->update & (PU_TORCH))
	{
		p_ptr->update &= ~(PU_TORCH);
		calc_torch();
	}

	if (p_ptr->update & (PU_HP))
	{
		p_ptr->update &= ~(PU_HP);
		call_lua("player.calc_hitpoints", "()", "");
	}

	if (p_ptr->update & (PU_SANITY))
	{
		p_ptr->update &= ~(PU_SANITY);
		call_lua("player.calc_sanity", "()", "");
	}

	if (p_ptr->update & (PU_MANA))
	{
		p_ptr->update &= ~(PU_MANA);
		call_lua("player.calc_mana", "()", "");
	}

	/* Character is not ready yet, no screen updates */
	if (!character_generated) return;


	/* Character is in "icky" mode, no screen updates */
	if (character_icky) return;


	if (p_ptr->update & (PU_UN_VIEW))
	{
		p_ptr->update &= ~(PU_UN_VIEW);
		forget_view();
	}

	if (p_ptr->update & (PU_VIEW))
	{
		p_ptr->update &= ~(PU_VIEW);
		update_view();
	}

	if (p_ptr->update & (PU_DISTANCE))
	{
		p_ptr->update &= ~(PU_DISTANCE);
		p_ptr->update &= ~(PU_MONSTERS);
		update_monsters(TRUE);
	}

	if (p_ptr->update & (PU_MONSTERS))
	{
		p_ptr->update &= ~(PU_MONSTERS);
		update_monsters(FALSE);
	}

	if (p_ptr->update & (PU_MON_LITE))
	{
		p_ptr->update &= ~(PU_MON_LITE);
		if (monster_lite) update_mon_lite();
	}
}


/*
 * Handle "p_ptr->redraw"
 */
void redraw_stuff(void)
{
	/* Redraw stuff */
	if (!flag_used(&p_ptr->redraw)) return;


	/* Character is not ready yet, no screen updates */
	if (!character_generated) return;


	/* Character is in "icky" mode, no screen updates */
	if (character_icky) return;

	/* Should we tell lua to redisplay too ? */
	process_hooks(HOOK_REDRAW, "()");

	flag_empty(&p_ptr->redraw);
}


/*
 * Handle "p_ptr->window"
 */
void window_stuff(void)
{
	/* Character is not ready yet, no screen updates */
	if (!character_generated || character_icky) return;

	/* Nothing to do */
	if (!flag_used(&p_ptr->window)) return;

	/* Display monster list */
	if (flag_exists(&p_ptr->window, FLAG_PW_M_LIST) || flag_exists(&p_ptr->window, FLAG_PW_ALL))
	{
		fix_m_list();
	}

	/* Display overhead view */
	if (flag_exists(&p_ptr->window, FLAG_PW_MESSAGE) || flag_exists(&p_ptr->window, FLAG_PW_ALL))
	{
		fix_message();
	}

	/* Display monster recall */
	if (flag_exists(&p_ptr->window, FLAG_PW_MONSTER) || flag_exists(&p_ptr->window, FLAG_PW_ALL))
	{
		fix_monster();
	}

	/* Display object recall */
	if (flag_exists(&p_ptr->window, FLAG_PW_OBJECT) || flag_exists(&p_ptr->window, FLAG_PW_ALL))
	{
		fix_object();
	}

	call_lua("windows.display", "()", "");

	flag_empty(&p_ptr->window);
}


/*
 * Handle "p_ptr->update" and "p_ptr->redraw" and "p_ptr->window"
 */
void handle_stuff(void)
{
	/* Update stuff */
	if (p_ptr->update) update_stuff();

	/* Redraw stuff */
	if (flag_used(&p_ptr->redraw)) redraw_stuff();

	/* Window stuff */
	if (flag_used(&p_ptr->window)) window_stuff();
}

/*
 * Return a luck number between a certain range
 */
s32b luck(s32b min, s32b max)
{
	s32b luck = p_ptr->luck_cur;
	s32b range = max - min;

	if (luck < -30) luck = -30;
	if (luck > 30) luck = 30;
	luck += 30;

	luck *= range;
	luck /= 60;

	return (luck + min);
}

/*
 * Change the current body the player is using.  Return FALSE if the
 * change was forbidden by hooks, TRUE otherwise.
 */
bool change_player_body(monster_type* new_body, monster_type* old_body_out)
{
	monster_type* old_body = monster_type_dup(&p_ptr->body_monster);

	if (process_hooks(HOOK_BODY_CHANGE_PRE, ("(M,M)"), old_body, new_body))
	{
		if (old_body_out != NULL)
			monster_type_copy(old_body_out, old_body);

		monster_type_del(old_body);

		return FALSE;
	}

	process_hooks(HOOK_BODY_CHANGE, ("(M,M)"), old_body, new_body);

	monster_type_copy(&p_ptr->body_monster, new_body);

	p_ptr->update |= (PU_BODY | PU_BONUS | PU_HP | PU_SANITY | PU_MANA |
					  PU_SPELLS | PU_POWERS);
	do_cmd_redraw();

	process_hooks(HOOK_BODY_CHANGE_POST, ("(M,M)"), old_body,
				  &p_ptr->body_monster);

	if (old_body_out != NULL)
		monster_type_copy(old_body_out, old_body);

	monster_type_del(old_body);

	return TRUE;
}
