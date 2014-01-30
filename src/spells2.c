/* File: spells2.c */

/* Purpose: Spell code (part 2) */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

#define WEIRD_LUCK      12
#define BIAS_LUCK       20
/*
 * Bias luck needs to be higher than weird luck,
 * since it is usually tested several times...
 */

void summon_dragon_riders();


/*
 * Grow things
 */
void grow_things(s16b type, s32b rad)
{
	s32b a, i, j;

	for (a = 0; a < rad * rad + 11; a++)
	{
		i = (rand_int((rad * 2) + 1)-rad + rand_int((rad * 2) + 1)-rad) / 2;
		j = (rand_int((rad * 2) + 1)-rad + rand_int((rad * 2) + 1)-rad) / 2;

		if (!in_bounds(p_ptr->py + j, p_ptr->px + i)) continue;
		if (distance(p_ptr->py, p_ptr->px, p_ptr->py + j, p_ptr->px + i) > rad) continue;

		if (cave_clean_bold(p_ptr->py + j, p_ptr->px + i))
		{
			cave_set_feat(p_ptr->py + j, p_ptr->px + i, type);
		}
	}
}

/*
 * Grow trees
 */
void grow_trees(s32b rad)
{
	s32b a, i, j;

	for (a = 0; a < rad * rad + 11; a++)
	{
		i = (rand_int((rad * 2) + 1)-rad + rand_int((rad * 2) + 1)-rad) / 2;
		j = (rand_int((rad * 2) + 1)-rad + rand_int((rad * 2) + 1)-rad) / 2;

		if (!in_bounds(p_ptr->py + j, p_ptr->px + i)) continue;
		if (distance(p_ptr->py, p_ptr->px, p_ptr->py + j, p_ptr->px + i) > rad) continue;

		if (cave_clean_bold(p_ptr->py + j, p_ptr->px + i) && has_flag(&f_info[cave[p_ptr->py][p_ptr->px].feat], FLAG_SUPPORT_GROWTH))
		{
//DGDGDGDG			cave_set_feat(p_ptr->py + j, p_ptr->px + i, FEAT_TREES);
		}
	}
}

/*
 * Grow grass
 */
void grow_grass(s32b rad)
{
	s32b a, i, j;

	for (a = 0; a < rad * rad + 11; a++)
	{
		i = (rand_int((rad * 2) + 1)-rad + rand_int((rad * 2) + 1)-rad) / 2;
		j = (rand_int((rad * 2) + 1)-rad + rand_int((rad * 2) + 1)-rad) / 2;

		if (!in_bounds(p_ptr->py + j, p_ptr->px + i)) continue;
		if (distance(p_ptr->py, p_ptr->px, p_ptr->py + j, p_ptr->px + i) > rad) continue;

		if (cave_clean_bold(p_ptr->py + j, p_ptr->px + i) && has_flag(&f_info[cave[p_ptr->py][p_ptr->px].feat], FLAG_SUPPORT_GROWTH))
		{
//DGDGDGDG			cave_set_feat(p_ptr->py + j, p_ptr->px + i, FEAT_GRASS);
		}
	}
}

/*
 * Increase players hit points, notice effects
 */
bool hp_player(s32b num)
{
	s32b ret;
	call_lua("hp_player", "(d)", "d", num, &ret);
	return ret;
}

/*
 * Lose a "point"
 */
bool do_dec_stat(s32b stat, s32b mode)
{
	cptr str;
	bool sust = FALSE;

	/* Access the "sustain" */
	flags_type *susts = flag_get_flags(&p_ptr->intrinsic, FLAG_SUST_STATS);

	sust = flag_exists(susts, stat);

	/* Get the stat negative description */
	call_lua("player.get_stat_info", "(d,s)", "s", stat, "desc_negative", &str);

	/* Sustain */
	if (sust)
	{
		/* Message */
		msg_format("You feel %s for a moment, but the feeling passes.", str);

		/* Notice effect */
		return (TRUE);
	}

	/* Attempt to reduce the stat */
	if (dec_stat(stat, 10, mode))
	{
		/* Message */
		msg_format("You feel very %s.", str);

		/* Notice effect */
		return (TRUE);
	}

	/* Nothing obvious */
	return (FALSE);
}


/*
 * Restore lost "points" in a stat
 */
bool do_res_stat(s32b stat, bool full)
{
	cptr str;

	/* Keep a copy of the current stat, so we can evaluate it if necessary */
	s32b cur_stat = get_stat(stat, ind);

	/* Attempt to increase */
	if (res_stat(stat, full))
	{
		/* Message, depending on whether we got stronger or weaker */
		if (cur_stat > get_stat(stat, ind))
		{
			/* Get the stat description */
			call_lua("player.get_stat_info", "(d,s)", "s", stat, "name", &str);

			msg_format("You feel your %s boost drain away.", str);
		}
		else
		{
			/* Get the stat negative description */
			call_lua("player.get_stat_info", "(d,s)", "s", stat, "desc_negative", &str);

			msg_format("You feel less %s.", str);
		}

		/* Notice */
		return (TRUE);
	}

	/* Nothing obvious */
	return (FALSE);
}


/*
 * Gain a "point" in a stat
 */
bool do_inc_stat(s32b stat)
{
	cptr str;
	bool res;

	/* Restore strength */
	res = res_stat(stat, TRUE);

	/* Attempt to increase */
	if (inc_stat(stat))
	{
		/* Get the stat positive description */
		call_lua("player.get_stat_info", "(d,s)", "s", stat, "desc_positive", &str);

		/* Message */
		msg_format("Wow!  You feel very %s!", str);

		/* Notice */
		return (TRUE);
	}

	/* Restoration worked */
	if (res)
	{
		/* Get the stat negative description */
		call_lua("player.get_stat_info", "(d,s)", "s", stat, "desc_negative", &str);

		/* Message */
		msg_format("You feel less %s.", str);

		/* Notice */
		return (TRUE);
	}

	/* Nothing obvious */
	return (FALSE);
}



/*
 * Identify everything being carried.
 * Done by a potion of "self knowledge".
 */
void identify_pack(void)
{
	/* Simply identify and know every item */
	for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		/* Aware and Known */
		object_aware(o_ptr);
		object_known(o_ptr);

		/* Process the appropriate hooks */
		process_hooks(HOOK_IDENTIFY_POST, "(O,d,s)", o_ptr, item_slot,
					  "normal");
	}
	end_inventory();
}

/*
 * common portions of identify_fully and identify_pack_fully
 */
void make_item_fully_identified(object_type *o_ptr)
{
	/* Identify it fully */
	object_aware(o_ptr);
	object_known(o_ptr);

	/* Identify all flags */
	flag_learn_all(&o_ptr->flags);

	/* Mark the item as fully known */
	o_ptr->ident |= (IDENT_MENTAL);
}

/*
 * Identify everything being carried.
 * Done by a potion of "self knowledge".
 */
void identify_pack_fully(void)
{
	/* Simply identify and know every item */
	for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		make_item_fully_identified(o_ptr);

		/* Process the appropriate hooks */
		process_hooks(HOOK_IDENTIFY_POST, "(O,d,s)", o_ptr, item_slot,
					  "full");
	}
	end_inventory();
}

bool remove_curse_object(object_type *o_ptr, bool all)
{
	/* Skip non-objects */
	if (!o_ptr->k_idx) return FALSE;

	/* Uncursed already */
	if (!cursed_p(o_ptr)) return FALSE;

	/* Heavily Cursed Items need a special spell */
	if (!all && (has_flag(o_ptr, FLAG_HEAVY_CURSE))) return FALSE;

	/* Perma-Cursed Items can NEVER be uncursed */
	if (has_flag(o_ptr, FLAG_PERMA_CURSE)) return FALSE;

	/* Uncurse it */
	o_ptr->ident &= ~(IDENT_CURSED);

	/* Hack -- Assume felt */
	o_ptr->ident |= (IDENT_SENSE);

	if (has_flag(o_ptr, FLAG_CURSED))
		unset_flag(o_ptr, FLAG_CURSED);

	if (has_flag(o_ptr, FLAG_HEAVY_CURSE))
		unset_flag(o_ptr, FLAG_HEAVY_CURSE);

	/* Take note */
	o_ptr->sense = SENSE_UNCURSED;

	/* Reverse the curse effect */
	/* jk - scrolls of *remove curse* have a 1 in (55-level chance to */
	/* reverse the curse effects - a ring of damage(-15) {cursed} then */
	/* becomes a ring of damage (+15) */
	/* this does not go for artifacts - a Sword of Mormegil +40,+60 would */
	/* be somewhat unbalancing */
	/* due to the nature of this procedure, it only works on cursed items */
	/* ie you get only one chance! */
	if ((randint(55-p_ptr->lev) == 1) && !artifact_p(o_ptr))
	{
		if (o_ptr->to_a < 0) o_ptr->to_a = -o_ptr->to_a;
		if (o_ptr->to_h < 0) o_ptr->to_h = -o_ptr->to_h;
		if (o_ptr->to_d < 0) o_ptr->to_d = -o_ptr->to_d;
	}

	/* Recalculate the bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_EQUIP);

	return TRUE;
}

/*
 * Removes curses from items in inventory
 *
 * Note that Items which are "Perma-Cursed" (The One Ring,
 * The Crown of Morgoth) can NEVER be uncursed.
 *
 * Note that if "all" is FALSE, then Items which are
 * "Heavy-Cursed" (Mormegil, Calris, and Weapons of Morgul)
 * will not be uncursed.
 */
static s32b remove_curse_aux(s32b all)
{
	s32b cnt = 0;

	/* Attempt to uncurse items being worn */
	for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		if (!remove_curse_object(o_ptr, all)) continue;

		/* Count the uncursings */
		cnt++;
	}
	end_inventory();

	/* Return "something uncursed" */
	return (cnt);
}


/*
 * Remove most curses
 */
bool remove_curse(void)
{
	return (remove_curse_aux(FALSE) ? TRUE : FALSE);
}

/*
 * Remove all curses
 */
bool remove_all_curse(void)
{
	return (remove_curse_aux(TRUE) ? TRUE : FALSE);
}



/*
 * Restores any drained experience
 */
bool restore_level(void)
{
	/* Restore experience */
	if (p_ptr->exp < p_ptr->max_exp)
	{
		/* Message */
		msg_print("You feel your life energies returning.");

		/* Restore the experience */
		p_ptr->exp = p_ptr->max_exp;

		/* Check the experience */
		check_experience();

		/* Did something */
		return (TRUE);
	}

	/* No effect */
	return (FALSE);
}


bool alchemy(void) /* Turns an object into gold, gain some of its value in a shop */
{
	s32b item, amt = 1;
	s32b old_number;
	long price;
	bool force = FALSE;
	object_type *o_ptr;
	char o_name[80];
	char out_val[160];

	cptr q, s;

	/* Hack -- force destruction */
	if (command_arg > 0) force = TRUE;

	/* Get an item */
	q = "Turn which item to gold? ";
	s = "You have nothing to turn to gold.";
	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return (FALSE);

	/* Get the item (in the pack) */
	o_ptr = get_object(item);


	/* See how many items */
	if (o_ptr->number > 1)
	{
		/* Get a quantity */
		amt = get_quantity(NULL, o_ptr->number);

		/* Allow user abort */
		if (amt <= 0) return FALSE;
	}


	/* Describe the object */
	old_number = o_ptr->number;
	o_ptr->number = amt;
	object_desc(o_name, o_ptr, TRUE, 3);
	o_ptr->number = old_number;

	/* Verify unless quantity given */
	if (!force)
	{
		if (!((auto_destroy) && (object_value(o_ptr) < 1)))
		{
			/* Make a verification */
			sprintf(out_val, "Really turn %s to gold? ", o_name);
			if (!get_check(out_val)) return FALSE;
		}
	}

	/* Artifacts cannot be destroyed */
	if (artifact_p(o_ptr) || o_ptr->art_name)
	{
		byte feel = SENSE_SPECIAL;

		/* Message */
		msg_format("You fail to turn %s to gold!", o_name);

		/* Hack -- Handle icky artifacts */
		if (cursed_p(o_ptr)) feel = SENSE_TERRIBLE;

		/* Hack -- inscribe the artifact */
		o_ptr->sense = feel;

		/* We have "felt" it (again) */
		o_ptr->ident |= (IDENT_SENSE);

		/* Combine the pack */
		p_ptr->notice |= (PN_COMBINE);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_INVEN);
		flag_bool(&p_ptr->window, FLAG_PW_EQUIP);

		/* Done */
		return FALSE;
	}

	price = object_value_real(o_ptr);

	if (price <= 0)
		/* Message */
		msg_format("You turn %s to fool's gold.", o_name);
	else
	{
		price /= 3;

		if (amt > 1) price *= amt;

		msg_format("You turn %s to %ld coins worth of gold.", o_name, price);
		p_ptr->au += price;

		/* Redraw gold */
		flag_bool(&p_ptr->redraw, FLAG_PR_GOLD);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_INVEN);
	}

	/* Eliminate the item (from the pack) */
	item_increase(item, -amt);
	item_describe(item);
	item_optimize(item);

	return TRUE;
}




/*
 * self-knowledge... idea from nethack.  Useful for determining powers and
 * resistances of items.  It saves the screen, clears it, then starts listing
 * attributes, a screenful at a time.  (There are a LOT of attributes to
 * list.  It will probably take 2 or 3 screens for a powerful character whose
 * using several artifacts...) -CFT
 *
 * It is now a lot more efficient. -BEN-
 *
 * See also "identify_fully()".
 *
 * XXX XXX XXX Use the "show_file()" method, perhaps.
 */
void self_knowledge(PHYSFS_file *fff)
{
        call_lua("self_knowledge", "()", "", NULL);
}

/*
 * Forget everything
 */
bool lose_all_info(void)
{
	/* Forget info about objects */
	for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		/* Allow "protection" by the MENTAL flag */
		if (o_ptr->ident & (IDENT_MENTAL)) continue;

		/* Remove sensing */
		o_ptr->sense = SENSE_NONE;

		/* Hack -- Clear the "empty" flag */
		o_ptr->ident &= ~(IDENT_EMPTY);

		/* Hack -- Clear the "known" flag */
		o_ptr->ident &= ~(IDENT_KNOWN);

		/* Hack -- Clear the "felt" flag */
		o_ptr->ident &= ~(IDENT_SENSE);
	}
	end_inventory();

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_INVEN);
	flag_bool(&p_ptr->window, FLAG_PW_EQUIP);
	flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

	/* Mega-Hack -- Forget the map */
	wiz_dark();

	/* It worked */
	return (TRUE);
}

void curse_artifact(object_type * o_ptr)
{
	if (o_ptr->to_a) o_ptr->to_a = 0 - ((o_ptr->to_a) + randint(4));
	if (o_ptr->to_h) o_ptr->to_h = 0 - ((o_ptr->to_h) + randint(4));
	if (o_ptr->to_d) o_ptr->to_d = 0 - ((o_ptr->to_d) + randint(4));
	bool_flag(o_ptr, FLAG_HEAVY_CURSE);
	bool_flag(o_ptr, FLAG_CURSED);
	if (randint(3) == 1) bool_flag(o_ptr, FLAG_FOUL_CURSE);
	if (randint(2) == 1) bool_flag(o_ptr, FLAG_AGGRAVATE);
	if (randint(3) == 1) bool_flag(o_ptr, FLAG_DRAIN_EXP);
	if (randint(3) == 1) bool_flag(o_ptr, FLAG_BLACK_BREATH);
	if (randint(2) == 1) bool_flag(o_ptr, FLAG_TELEPORT);
	else if (randint(3) == 1) bool_flag(o_ptr, FLAG_NO_TELE);
	o_ptr->ident |= IDENT_CURSED;
}


/*
 * Determines if an item is not identified
 */
static bool item_tester_hook_unknown(object_type *o_ptr, s32b item)
{
	return (object_known_p(o_ptr) ? FALSE : TRUE);
}


/*
 * Identify an object in the inventory (or on the floor)
 * This routine does *not* automatically combine objects.
 * Returns TRUE if something was identified, else FALSE.
 */
bool ident_spell(void)
{
	s32b item;

	object_type *o_ptr;

	char o_name[80];

	cptr q, s;

	/* Get an item */
	item_tester_hook = item_tester_hook_unknown;
	q = "Identify which item? ";
	s = "You have nothing to identify.";
	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return (FALSE);

	/* Get the item (in the pack) */
	o_ptr = get_object(item);


	/* Identify it fully */
	object_aware(o_ptr);
	object_known(o_ptr);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_INVEN);
	flag_bool(&p_ptr->window, FLAG_PW_EQUIP);
	flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

	/* Description */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Describe */
	if (item >= INVEN_PACK)
	{
		msg_format("%^s: %s (%c).",
		           describe_use(item), o_name, index_to_label(item));
	}
	else if (item >= 0)
	{
		msg_format("In your pack: %s (%c).",
		           o_name, index_to_label(item));
	}
	else
	{
		msg_format("On the ground: %s.",
		           o_name);
	}

	/* If the item was an artifact, and if the auto-note is selected, write a message. */
	if (take_notes && auto_notes && (artifact_p(o_ptr) || o_ptr->artifact_id))
	{
		char note[150];
		char item_name[80];
		object_desc(item_name, o_ptr, FALSE, 0);

		/* Build note and write */
		sprintf(note, "Found The %s", item_name);
		add_note(note, 'A');
	}
	/* Process the appropriate hooks */
	process_hooks(HOOK_IDENTIFY_POST, "(O,d,s)", o_ptr, item, "normal");

	/* Something happened */
	return (TRUE);
}

/*
 * Identify all objects in the level
 */
bool ident_all(void)
{
#if 0 // DGDGDGDG
	s32b i;

	object_type *o_ptr;

	for (i = 1; i < o_max; i++)
	{
		/* Acquire object */
		o_ptr = o_list[i];

		/* Identify it fully */
		object_aware(o_ptr);
		object_known(o_ptr);

		/* If the item was an artifact, and if the auto-note is selected, write a message. */
		if (take_notes && auto_notes && (artifact_p(o_ptr) || o_ptr->artifact_id))
		{
			char note[150];
			char item_name[80];
			object_desc(item_name, o_ptr, FALSE, 0);

			/* Build note and write */
			sprintf(note, "Found The %s", item_name);
			add_note(note, 'A');
		}
		/* Process the appropriate hooks */
		process_hooks(HOOK_IDENTIFY_POST, "(O,d,s)", o_ptr, -i, "normal");
	}

	/* Something happened */
	return (TRUE);
#endif

	return(FALSE);
}



/*
 * Determine if an object is not fully identified
 */
static bool item_tester_hook_no_mental(object_type *o_ptr, s32b item)
{
	return ((o_ptr->ident & (IDENT_MENTAL)) ? FALSE : TRUE);
}

/*
 * Fully "identify" an object in the inventory  -BEN-
 * This routine returns TRUE if an item was identified.
 */
bool identify_fully(void)
{
	s32b item;
	object_type *o_ptr;
	char o_name[80];

	cptr q, s;

	/* Get an item */
	item_tester_hook = item_tester_hook_no_mental;
	q = "Identify which item? ";
	s = "You have nothing to identify.";
	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return (FALSE);

	/* Get the item (in the pack) */
	o_ptr = get_object(item);

	/* Do the identification */
	make_item_fully_identified(o_ptr);

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_INVEN);
	flag_bool(&p_ptr->window, FLAG_PW_EQUIP);
	flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

	/* Handle stuff */
	handle_stuff();

	/* Description */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Describe */
	if (item >= INVEN_PACK)
	{
		msg_format("%^s: %s (%c).",
		           describe_use(item), o_name, index_to_label(item));
	}
	else if (item >= 0)
	{
		msg_format("In your pack: %s (%c).",
		           o_name, index_to_label(item));
	}
	else
	{
		msg_format("On the ground: %s.",
		           o_name);
	}

	/* If the item was an artifact, and if the auto-note is selected, write a message. */
	if (take_notes && auto_notes && (artifact_p(o_ptr) || o_ptr->artifact_id))
	{
		char note[150];
		char item_name[80];
		object_desc(item_name, o_ptr, FALSE, 0);

		/* Build note and write */
		sprintf(note, "Found The %s", item_name);
		add_note(note, 'A');
	}

	/* Describe it fully */
	object_out_desc(o_ptr, NULL, FALSE, TRUE);

	/* Process the appropriate hooks */
	process_hooks(HOOK_IDENTIFY_POST, "(O,d,s)", o_ptr, item, "full");

	/* Success */
	return (TRUE);
}



/*
 * Apply a "project()" directly to all viewable monsters
 *
 * Note that affected monsters are NOT auto-tracked by this usage.
 */
bool project_hack(s32b typ, s32b dam)
{
	s32b i, x, y;
	s32b flg = PROJECT_JUMP | PROJECT_KILL | PROJECT_HIDE | PROJECT_HIDE_BLAST;
	bool obvious = FALSE;


	/* Affect all (nearby) monsters */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Require line of sight */
		if (!player_has_los_bold(y, x)) continue;

		/* Jump directly to the target monster */
		if (project(spell_caster, 0, y, x, dam, typ, flg, -1)) obvious = TRUE;
	}
	end_for_flags();

	/* Result */
	return (obvious);
}

/*
 * Apply a "project()" a la meteor shower
 */
void project_meteor(s32b radius, s32b typ, s32b dam, u32b flg)
{
	s32b x, y, dx, dy, d, count = 0, i;
	s32b b = radius + randint(radius);
	for (i = 0; i < b; i++)
	{
		for (count = 0; count < 1000; count++)
		{
			x = p_ptr->px - 5 + randint(10);
			y = p_ptr->py - 5 + randint(10);
			if ((x < 0) || (x >= cur_wid) ||
			                (y < 0) || (y >= cur_hgt)) continue;
			dx = (p_ptr->px > x) ? (p_ptr->px - x) : (x - p_ptr->px);
			dy = (p_ptr->py > y) ? (p_ptr->py - y) : (y - p_ptr->py);
			/* Approximate distance */
			d = (dy > dx) ? (dy + (dx >> 1)) : (dx + (dy >> 1));
			if ((d <= 5) && (player_has_los_bold(y, x))) break;
		}
		if (count >= 1000) break;
		project(spell_caster, 2, y, x, dam, typ, PROJECT_JUMP | flg, -1);
	}
}


/*
 * Wake up all monsters near player, and speed up "los" monsters.
 */
void aggravate_monsters(s32b who)
{
	s32b i;
	bool sleep = FALSE;
	bool speed = FALSE;


	/* Aggravate everyone nearby */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);
		monster_race *r_ptr = race_inf(m_ptr);

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Skip aggravating monster (or player) */
		if (i == who) continue;

		/* Wake up nearby sleeping monsters */
		if (m_ptr->cdis < MAX_SIGHT * 2)
		{
			/* Wake up */
			if (m_ptr->csleep)
			{
				/* Wake up */
				m_ptr->csleep = 0;
				sleep = TRUE;
			}
		}

		/* Speed up monsters in line of sight */
		if (player_has_los_bold(m_ptr->fy, m_ptr->fx))
		{
			/* Speed up (instantly) to racial base + 10 */
			if (m_ptr->mspeed < r_ptr->speed + 10)
			{
				/* Speed up */
				m_ptr->mspeed = r_ptr->speed + 10;
				speed = TRUE;
			}

			/* Pets may get angry (50% chance) */
			if (is_friend(m_ptr) > 0)
			{
				if (randint(2) == 1)
				{
					/* Get into teh default enemy faction */
					u32b faction = get_hostile_faction(m_ptr, FACTION_PLAYER,
													   FACTION_DUNGEON);
					change_monst_faction(m_ptr, faction);
				}
			}
		}
	}
	end_for_flags();

	/* Messages */
	if (speed) msg_print("You feel a sudden stirring nearby!");
	else if (sleep) msg_print("You hear a sudden stirring in the distance!");
}

/*
 * Wake up monsters near a specific point, without aggravating them.
 * If "power" is non-zero, it will decrease monsters sleep counter by
 * that amount, thus only waking some of the monsters (or maybe even
 * none).
 */
void wake_monsters(s32b y, s32b x, s32b rad, u32b power)
{
	int  unseen = 0;
	s32b xx, yy;

	for (yy = y - rad; yy <= y + rad; yy++)
		for (xx = x - rad; xx <= x + rad; xx++)
		{
			int           m_idx;
			monster_type *m_ptr;

			if (!in_bounds(yy, xx)) continue;

			m_idx = cave[yy][xx].m_idx;
			if (m_idx == 0) continue;

			m_ptr = get_monster(m_idx);

			/* Paranoia: skip dead monsters */
			if (m_ptr->r_idx == 0) continue;

			/* Already awake */
			if (m_ptr->csleep == 0) continue;

			if (power == 0)
				m_ptr->csleep = 0;
			else
				m_ptr->csleep = MAX(0, m_ptr->csleep - power);

			if (m_ptr->csleep == 0 && !m_ptr->ml)
				unseen++;
		}

	if (unseen > 0)
		msg_print("You hear a sudden stirring in the distance!");
} /* wake_monsters() */


/*
 * Generic genocide race selection
 */
bool get_genocide_race(cptr msg, char *typ)
{
	s32b i, j;
	cave_type *c_ptr;

	msg_print(msg);
	if (!tgt_pt(&i, &j)) return FALSE;

	c_ptr = &cave[j][i];

	if (c_ptr->m_idx)
	{
		monster_type *m_ptr = get_monster(c_ptr->m_idx);
		monster_race *r_ptr = race_inf(m_ptr);

		*typ = r_ptr->d_char;
		return TRUE;
	}

	msg_print("You must select a monster.");
	return FALSE;
}

/*
 * Inflict dam damage of type typee to all monster of the given race
 */
bool invoke(s32b dam, s32b typee)
{
	s32b i;
	char typ;
	bool result = FALSE;
	s32b msec = delay_factor * delay_factor * delay_factor;

	if ((has_flag(dungeon_flags, FLAG_NO_GENO))) return (FALSE);

	/* Get a monster symbol */
	if (!get_genocide_race("Target a monster to select the race to invoke on.", &typ)) return FALSE;

	/* Delete the monsters of that "type" */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);
		monster_race *r_ptr = race_inf(m_ptr);

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Hack -- Skip Unique Monsters */
		if (has_flag(m_ptr, FLAG_UNIQUE)) continue;

		/* Hack -- Skip Quest Monsters */
		if (m_ptr->mflag & MFLAG_QUEST) continue;

		/* Skip "wrong" monsters */
		if (r_ptr->d_char != typ) continue;

		project_m(spell_caster, 0, m_ptr->fy, m_ptr->fx, dam, typee,
				  PROJECT_KILL, 0);

		/* Visual feedback */
		move_cursor_relative(p_ptr->py, p_ptr->px);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

		/* Handle */
		handle_stuff();

		/* Fresh */
		Term_fresh();

		/* Delay */
		Term_xtra(TERM_XTRA_DELAY, msec);

		/* Take note */
		result = TRUE;
	}
	end_for_flags();

	return (result);
}


/*
 * Delete all non-unique/non-quest monsters of a given "type" from the level
 */
bool genocide_aux(bool player_cast, char typ)
{
	s32b i, count = 0, total_dam = 0;
	bool result = FALSE;
	s32b msec = delay_factor * delay_factor * delay_factor;

	/* Delete the monsters of that "type" */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);
		monster_race *r_ptr = race_inf(m_ptr);

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Hack -- Skip Unique Monsters */
		if (has_flag(m_ptr, FLAG_UNIQUE)) continue;

		/* Hack -- Skip Quest Monsters */
		if (m_ptr->mflag & MFLAG_QUEST) continue;

		/* Skip "wrong" monsters */
		if (r_ptr->d_char != typ) continue;

		/* Delete the monster */
		delete_monster_idx(i);

		if (player_cast)
		{
			count++;
		}

		/* Visual feedback */
		move_cursor_relative(p_ptr->py, p_ptr->px);

		/* Redraw */
		flag_bool(&p_ptr->redraw, FLAG_PR_HP);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

		/* Handle */
		handle_stuff();

		/* Fresh */
		Term_fresh();

		/* Delay */
		Term_xtra(TERM_XTRA_DELAY, msec);

		/* Take note */
		result = TRUE;
	}
	end_for_flags();

	/* Calculate damage */
	for (i = 0; i < count; i++)
	{
	    total_dam += randint(4);
	}

	/* Hack -- visual feedback */
	if (total_dam) take_hit(total_dam, "the strain of casting Genocide");

	return (result);
}

bool genocide(bool player_cast)
{
	char typ;

	if ((has_flag(dungeon_flags, FLAG_NO_GENO))) return (FALSE);

	/* Mega-Hack -- Get a monster symbol */
	if (!get_genocide_race("Target a monster to select the race to genocide.", &typ)) return FALSE;

	return (genocide_aux(player_cast, typ));
}


/*
 * Delete all nearby (non-unique) monsters
 */
bool mass_genocide(bool player_cast)
{
	s32b i, count = 0, total_dam = 0;

	bool result = FALSE;

	s32b msec = delay_factor * delay_factor * delay_factor;

	if ((has_flag(dungeon_flags, FLAG_NO_GENO))) return (FALSE);

	/* Delete the (nearby) monsters */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Hack -- Skip unique monsters */
		if (has_flag(m_ptr, FLAG_UNIQUE)) continue;

		/* Hack -- Skip Quest Monsters */
		if (m_ptr->mflag & MFLAG_QUEST) continue;

		/* Skip distant monsters */
		if (m_ptr->cdis > MAX_SIGHT) continue;

		/* Delete the monster */
		delete_monster_idx(i);

		if (player_cast)
		{
			count++;
		}

		move_cursor_relative(p_ptr->py, p_ptr->px);

		/* Redraw */
		flag_bool(&p_ptr->redraw, FLAG_PR_HP);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

		/* Handle */
		handle_stuff();

		/* Fresh */
		Term_fresh();

		/* Delay */
		Term_xtra(TERM_XTRA_DELAY, msec);

		/* Note effect */
		result = TRUE;
	}
	end_for_flags();

	/* Calculate damage */
	for (i = 0; i < count; i++)
	{
	    total_dam += randint(3);
	}

	/* Hack -- visual feedback */
	if (total_dam) take_hit(total_dam, "the strain of casting Mass Genocide");

	return (result);
}

/* Probe a monster */
void do_probe(s32b m_idx)
{
	char m_name[80];
	monster_type *m_ptr = get_monster(m_idx);

	/* Get "the monster" or "something" */
	monster_desc(m_name, m_ptr, 0x04);

	/* Describe the monster */
	if (!wizard && ((m_ptr->faction != FACTION_PLAYER) || (!has_flag(m_ptr, FLAG_PERMANENT)))) msg_format("%^s has %d hit points.", m_name, m_ptr->hp);
	else
	{
		s32b i;
		char t_name[80];
		msg_format("%^s has %d(%d) hit points, %d ac, %d speed.", m_name, m_ptr->hp, m_ptr->maxhp, m_ptr->ac, m_ptr->mspeed - 110);
		msg_format("%^s attacks with:", m_name);

		for (i = 0; i <= flag_max_key(&m_ptr->blow); i++)
		{
			monster_blow *blow = flag_get_blow(&m_ptr->blow, i);
			msg_format("    Blow %d: %dd%d", i, blow->d_dice, blow->d_side);
		}

		if (m_ptr->target > 0)
			monster_desc(t_name, get_monster(m_ptr->target), 0x04);
		else if (!m_ptr->target)
			sprintf(t_name, "you");
		else
			sprintf(t_name, "nothing");
		msg_format("%^s target is %s.", m_name, t_name);

		msg_format("%^s has %ld exp and needs %ld.", m_name, m_ptr->exp, MONSTER_EXP(m_ptr->level + 1));
	}

	/* Learn all of the non-spell, non-treasure flags */
	lore_do_probe(m_idx);
}

/*
 * Probe nearby monsters
 */
bool probing(void)
{
	s32b i;

	bool probe = FALSE;


	/* Probe all (nearby) monsters */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Require line of sight */
		if (!player_has_los_bold(m_ptr->fy, m_ptr->fx)) continue;

		/* Probe visible monsters */
		if (m_ptr->ml)
		{
			/* Start the message */
			if (!probe) msg_print("Probing...");

			/* Actualy probe */
			do_probe(i);

			/* Probe worked */
			probe = TRUE;
		}
	}
	end_for_flags();

	/* Done */
	if (probe)
	{
		msg_print("That's all.");
	}

	/* Result */
	return (probe);
}


/*
 * Wipe -- Empties a part of the dungeon
 */
void wipe(s32b y1, s32b x1, s32b r)
{
	s32b y, x, k;

	cave_type *c_ptr;

	bool flag = FALSE;

	if ((has_flag(dungeon_flags, FLAG_NO_GENO)))
	{
		msg_print("Not on special levels!");
		return;
	}
	if (p_ptr->inside_quest)
	{
		return;
	}

	/* Big area of affect */
	for (y = (y1 - r); y <= (y1 + r); y++)
	{
		for (x = (x1 - r); x <= (x1 + r); x++)
		{
			/* Skip illegal grids */
			if (!in_bounds(y, x)) continue;

			/* Extract the distance */
			k = distance(y1, x1, y, x);

			/* Stay in the circle of death */
			if (k > r) continue;

			/* Access the grid */
			c_ptr = &cave[y][x];

			/* Lose room and vault */
			c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

			/* Lose light and knowledge */
			c_ptr->info &= ~(CAVE_MARK | CAVE_GLOW);

			if (!has_flag(get_monster(c_ptr->m_idx), FLAG_PERMANENT)) delete_monster(y, x);
			delete_object_location(y, x);
			place_floor(y, x);
		}
	}


	/* Hack -- Affect player */
	if (flag)
	{
		/* Message */
		msg_print("There is a searing blast of light!");
	}


	/* Mega-Hack -- Forget the view */
	p_ptr->update |= (PU_UN_VIEW);

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_FLOW | PU_MON_LITE);

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);
}


/*
 * The spell of destruction
 *
 * This spell "deletes" monsters (instead of "killing" them).
 *
 * Later we may use one function for both "destruction" and
 * "earthquake" by using the "full" to select "destruction".
 */
void destroy_area(s32b y1, s32b x1, s32b r, bool full, bool bypass)
{
	s32b y, x, k, t;

	cave_type *c_ptr;

	bool flag = FALSE;


	/* XXX XXX */
	full = full ? full : 0;

	if ((has_flag(dungeon_flags, FLAG_NO_GENO)))
	{
		msg_print("Not on special levels!");
		return;
	}
	if (p_ptr->inside_quest && (!bypass))
	{
		return;
	}

	/* Big area of affect */
	for (y = (y1 - r); y <= (y1 + r); y++)
	{
		for (x = (x1 - r); x <= (x1 + r); x++)
		{
			/* Skip illegal grids */
			if (!in_bounds(y, x)) continue;

			/* Extract the distance */
			k = distance(y1, x1, y, x);

			/* Stay in the circle of death */
			if (k > r) continue;

			/* Access the grid */
			c_ptr = &cave[y][x];

			/* Lose room and vault */
			c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

			/* Lose light and knowledge */
			c_ptr->info &= ~(CAVE_MARK | CAVE_GLOW);

			/* Hack -- Notice player affect */
			if ((x == p_ptr->px) && (y == p_ptr->py))
			{
				/* Hurt the player later */
				flag = TRUE;

				/* Do not hurt this grid */
				continue;
			}

			/* Hack -- Skip the epicenter */
			if ((y == y1) && (x == x1)) continue;

			/* Delete the monster (if any) */
			if (c_ptr->m_idx)
			{
				monster_type *t = get_monster(c_ptr->m_idx);
				if ((!has_flag(t, FLAG_PERMANENT)) ||
					(!(t->mflag & MFLAG_QUEST)) ||
					(!(t->mflag & MFLAG_QUEST2)))
					delete_monster(y, x);
			}

			/* Destroy "valid" grids */
			if (cave_valid_bold(y, x))
			{
				/* Delete objects */
				delete_object_location(y, x);

				/* Wall (or floor) type */
				t = rand_int(200);

				/* Granite */
#if 0 //DGDGDGDG
				if (t < 20)
				{
					/* Create granite wall */
					cave_set_feat(y, x, FEAT_WALL_EXTRA);
				}

				/* Quartz */
				else if (t < 70)
				{
					/* Create quartz vein */
					cave_set_feat(y, x, FEAT_QUARTZ);
				}

				/* Magma */
				else if (t < 100)
				{
					/* Create magma vein */
					cave_set_feat(y, x, FEAT_MAGMA);
				}

				/* Floor */
				else
				{
					/* Create floor */
					cave_set_feat(y, x, FEAT_FLOOR);
				}
#endif
			}
		}
	}


	/* Hack -- Affect player */
	if (flag)
	{
		/* Message */
		msg_print("There is a searing blast of light!");
	}


	/* Mega-Hack -- Forget the view */
	p_ptr->update |= (PU_UN_VIEW);

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_FLOW | PU_MON_LITE);

	/* Update the monsters */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);
}


/*
 * Induce an "earthquake" of the given radius at the given location.
 *
 * This will turn some walls into floors and some floors into walls.
 *
 * The player will take damage and "jump" into a safe grid if possible,
 * otherwise, he will "tunnel" through the rubble instantaneously.
 *
 * Monsters will take damage, and "jump" into a safe grid if possible,
 * otherwise they will be "buried" in the rubble, disappearing from
 * the level in the same way that they do when genocided.
 *
 * Note that thus the player and monsters (except eaters of walls and
 * passers through walls) will never occupy the same grid as a wall.
 * Note that as of now (2.7.8) no monster may occupy a "wall" grid, even
 * for a single turn, unless that monster can pass_walls or kill_walls.
 * This has allowed massive simplification of the "monster" code.
 */
void earthquake(s32b cy, s32b cx, s32b r)
{
	s32b i, t, y, x, yy, xx, dy, dx, oy, ox;
	s32b damage = 0;
	s32b sn = 0, sy = 0, sx = 0;
	bool hurt = FALSE;
	cave_type *c_ptr;
	bool map[32][32];

	if (p_ptr->inside_quest)
	{
		return;
	}

	/* Paranoia -- Enforce maximum range */
	if (r > 12) r = 12;

	/* Clear the "maximal blast" area */
	for (y = 0; y < 32; y++)
	{
		for (x = 0; x < 32; x++)
		{
			map[y][x] = FALSE;
		}
	}

	/* Check around the epicenter */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip illegal grids */
			if (!in_bounds(yy, xx)) continue;

			/* Skip distant grids */
			if (distance(cy, cx, yy, xx) > r) continue;

			/* Access the grid */
			c_ptr = &cave[yy][xx];

			/* Lose room and vault */
			c_ptr->info &= ~(CAVE_ROOM | CAVE_ICKY);

			/* Lose light and knowledge */
			c_ptr->info &= ~(CAVE_GLOW | CAVE_MARK);

			/* Skip the epicenter */
			if (!dx && !dy) continue;

			/* Skip most grids */
			if (rand_int(100) < 85) continue;

			/* Damage this grid */
			map[16 + yy - cy][16 + xx - cx] = TRUE;

			/* Hack -- Take note of player damage */
			if ((yy == p_ptr->py) && (xx == p_ptr->px)) hurt = TRUE;
		}
	}

	/* First, affect the player (if necessary) */
	if (hurt && !intrinsic(WRAITH))
	{
		/* Check around the player */
		for (i = 0; i < 8; i++)
		{
			/* Access the location */
			y = p_ptr->py + ddy[i];
			x = p_ptr->px + ddx[i];

			/* Skip non-empty grids */
			if (!cave_empty_bold(y, x)) continue;

			/* Important -- Skip "quake" grids */
			if (map[16 + y - cy][16 + x - cx]) continue;

			/* Count "safe" grids */
			sn++;

			/* Randomize choice */
			if (rand_int(sn) > 0) continue;

			/* Save the safe location */
			sy = y;
			sx = x;
		}

		/* Random message */
		switch (randint(3))
		{
		case 1:
			{
				msg_print("The cave ceiling collapses!");
				break;
			}
		case 2:
			{
				msg_print("The cave floor twists in an unnatural way!");
				break;
			}
		default:
			{
				msg_print("The cave quakes!  You are pummeled with debris!");
				break;
			}
		}

		/* Hurt the player a lot */
		if (!sn)
		{
			/* Message and damage */
			msg_print("You are severely crushed!");
			damage = 300;
		}

		/* Destroy the grid, and push the player to safety */
		else
		{
			/* Calculate results */
			switch (randint(3))
			{
			case 1:
				{
					msg_print("You nimbly dodge the blast!");
					damage = 0;
					break;
				}
			case 2:
				{
					msg_print("You are bashed by rubble!");
					damage = damroll(10, 4);
					break;
				}
			case 3:
				{
					msg_print("You are crushed between the floor and ceiling!");
					damage = damroll(10, 8);
					break;
				}
			}

			/* Save the old location */
			oy = p_ptr->py;
			ox = p_ptr->px;

			/* Move the player to the safe location */
			p_ptr->py = sy;
			p_ptr->px = sx;

			/* Redraw the old spot */
			lite_spot(oy, ox);

			/* Redraw the new spot */
			lite_spot(p_ptr->py, p_ptr->px);

			/* Check for new panel */
			verify_panel();
		}

		/* Important -- no wall on player */
		map[16 + p_ptr->py - cy][16 + p_ptr->px - cx] = FALSE;

		/* Semi-wraiths have to be hurt *some*, says DG */
		if (has_flag(p_ptr, FLAG_SEMI_WRAITH))
			damage /= 4;

		/* Take some damage */
		if (damage) take_hit(damage, "an earthquake");
	}


	/* Examine the quaked region */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip unaffected grids */
			if (!map[16 + yy - cy][16 + xx - cx]) continue;

			/* Access the grid */
			c_ptr = &cave[yy][xx];

			/* Process monsters */
			if (c_ptr->m_idx)
			{
				monster_type *m_ptr = get_monster(c_ptr->m_idx);

				/* Most monsters cannot co-exist with rock */
				if (!(has_flag(m_ptr, FLAG_KILL_WALL)) &&
					!monst_can_pass_square(m_ptr, yy, xx, NULL))
				{
					char m_name[80];

					/* Assume not safe */
					sn = 0;

					/* Monster can move to escape the wall */
					if (!(has_flag(m_ptr, FLAG_NEVER_MOVE)))
					{
						/* Look for safety */
						for (i = 0; i < 8; i++)
						{
							/* Access the grid */
							y = yy + ddy[i];
							x = xx + ddx[i];

							/* Ignore illegal locations */
							if (!in_bounds(y, x)) continue;

							if (!monst_can_enter_square(m_ptr, y, x, NULL))
								continue;

							/* Important -- Skip "quake" grids */
							if (map[16 + y - cy][16 + x - cx]) continue;

							/* Count "safe" grids */
							sn++;

							/* Randomize choice */
							if (rand_int(sn) > 0) continue;

							/* Save the safe grid */
							sy = y;
							sx = x;
						}
					}

					/* Describe the monster */
					monster_desc(m_name, m_ptr, 0);

					/* Scream in pain */
					msg_format("%^s wails out in pain!", m_name);

					/* Take damage from the quake */
					damage = (sn ? damroll(4, 8) : 200);

					/* Monster is certainly awake */
					m_ptr->csleep = 0;

					/* Apply damage directly */
					m_ptr->hp -= damage;

					/* Delete (not kill) "dead" monsters */
					if (m_ptr->hp < 0)
					{
						/* Message */
						msg_format("%^s is embedded in the rock!", m_name);

						/* Delete the monster */
						delete_monster(yy, xx);

						/* No longer safe */
						sn = 0;
					}

					/* Hack -- Escape from the rock */
					if (sn)
					{
						s32b m_idx = cave[yy][xx].m_idx;

						/* Update the new location */
						cave[sy][sx].m_idx = m_idx;

						/* Update the old location */
						cave[yy][xx].m_idx = 0;

						/* Move the monster */
						m_ptr->fy = sy;
						m_ptr->fx = sx;

						/* Update the monster (new location) */
						update_mon(m_idx, TRUE);

						/* Redraw the old grid */
						lite_spot(yy, xx);

						/* Redraw the new grid */
						lite_spot(sy, sx);
					}
				}
			}
		}
	}


	/* Examine the quaked region */
	for (dy = -r; dy <= r; dy++)
	{
		for (dx = -r; dx <= r; dx++)
		{
			/* Extract the location */
			yy = cy + dy;
			xx = cx + dx;

			/* Skip unaffected grids */
			if (!map[16 + yy - cy][16 + xx - cx]) continue;

			/* Access the cave grid */
			c_ptr = &cave[yy][xx];

			/* Paranoia -- never affect player */
			if ((yy == p_ptr->py) && (xx == p_ptr->px)) continue;

			/* Destroy location (if valid) */
			if (cave_valid_bold(yy, xx))
			{
				bool floor = cave_floor_bold(yy, xx);

				/* Delete objects */
				delete_object_location(yy, xx);

				/* Wall (or floor) type */
				t = (floor ? rand_int(100) : 200);

				/* Granite */
#if 0 //DGDGDGDG
				if (t < 20)
				{
					/* Create granite wall */
					cave_set_feat(yy, xx, FEAT_WALL_EXTRA);
				}

				/* Quartz */
				else if (t < 70)
				{
					/* Create quartz vein */
					cave_set_feat(yy, xx, FEAT_QUARTZ);
				}

				/* Magma */
				else if (t < 100)
				{
					/* Create magma vein */
					cave_set_feat(yy, xx, FEAT_MAGMA);
				}

				/* Floor */
				else
				{
					/* Create floor */
					cave_set_feat(yy, xx, FEAT_FLOOR);
				}
#endif
			}
		}
	}


	/* Mega-Hack -- Forget the view */
	p_ptr->update |= (PU_UN_VIEW);

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_FLOW | PU_MON_LITE);

	/* Update the monsters */
	p_ptr->update |= (PU_DISTANCE);

	/* Update the health bar */
	flag_bool(&p_ptr->redraw, FLAG_PR_HEALTH);

	/* Redraw map */
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);
}



/*
 * This routine clears the entire "temp" set.
 *
 * This routine will Perma-Lite all "temp" grids.
 *
 * This routine is used (only) by "lite_room()"
 *
 * Dark grids are illuminated.
 *
 * Also, process all affected monsters.
 *
 * SMART monsters always wake up when illuminated
 * NORMAL monsters wake up 1/4 the time when illuminated
 * STUPID monsters wake up 1/10 the time when illuminated
 */
static void cave_temp_room_lite(void)
{
	s32b i;

	/* Apply flag changes */
	for (i = 0; i < temp_n; i++)
	{
		s32b y = temp_y[i];
		s32b x = temp_x[i];

		cave_type *c_ptr = &cave[y][x];

		/* No longer in the array */
		c_ptr->info &= ~(CAVE_TEMP);

		/* Update only non-CAVE_GLOW grids */
		/* if (c_ptr->info & (CAVE_GLOW)) continue; */

		/* Perma-Lite */
		c_ptr->info |= (CAVE_GLOW);
	}

	/* Fully update the visuals */
	p_ptr->update |= (PU_UN_VIEW | PU_VIEW | PU_MONSTERS | PU_MON_LITE);

	/* Update stuff */
	update_stuff();

	/* Process the grids */
	for (i = 0; i < temp_n; i++)
	{
		s32b y = temp_y[i];
		s32b x = temp_x[i];

		cave_type *c_ptr = &cave[y][x];

		/* Redraw the grid */
		lite_spot(y, x);

		/* Process affected monsters */
		if (c_ptr->m_idx)
		{
			s32b chance = 25;

			monster_type *m_ptr = get_monster(c_ptr->m_idx);

			/* Update the monster */
			update_mon(c_ptr->m_idx, FALSE);

			/* Stupid monsters rarely wake up */
			if (has_flag(m_ptr, FLAG_STUPID)) chance = 10;

			/* Smart monsters always wake up */
			if (has_flag(m_ptr, FLAG_SMART)) chance = 100;

			/* Sometimes monsters wake up */
			if (m_ptr->csleep && (rand_int(100) < chance))
			{
				/* Wake up! */
				m_ptr->csleep = 0;

				/* Notice the "waking up" */
				if (m_ptr->ml)
				{
					char m_name[80];

					/* Acquire the monster name */
					monster_desc(m_name, m_ptr, 0);

					/* Dump a message */
					msg_format("%^s wakes up.", m_name);
				}
			}
		}
	}

	/* None left */
	temp_n = 0;
}



/*
 * This routine clears the entire "temp" set.
 *
 * This routine will "darken" all "temp" grids.
 *
 * In addition, some of these grids will be "unmarked".
 *
 * This routine is used (only) by "unlite_room()"
 *
 * Also, process all affected monsters
 */
static void cave_temp_room_unlite(void)
{
	s32b i;

	/* Apply flag changes */
	for (i = 0; i < temp_n; i++)
	{
		s32b y = temp_y[i];
		s32b x = temp_x[i];

		cave_type *c_ptr = &cave[y][x];

		/* No longer in the array */
		c_ptr->info &= ~(CAVE_TEMP);

		/* Darken the grid */
		c_ptr->info &= ~(CAVE_GLOW);

		/* Hack -- Forget "boring" grids */
		if (cave_plain_floor_grid(c_ptr))
		{
			/* Forget the grid */
			c_ptr->info &= ~(CAVE_MARK);

			/* Notice */
			/* note_spot(y, x); */
		}
	}

	/* Fully update the visuals */
	p_ptr->update |= (PU_UN_VIEW | PU_VIEW | PU_MONSTERS | PU_MON_LITE);

	/* Update stuff */
	update_stuff();

	/* Process the grids */
	for (i = 0; i < temp_n; i++)
	{
		s32b y = temp_y[i];
		s32b x = temp_x[i];

		/* Redraw the grid */
		lite_spot(y, x);
	}

	/* None left */
	temp_n = 0;
}




/*
 * Aux function -- see below
 */
static void cave_temp_room_aux(s32b y, s32b x)
{
	cave_type *c_ptr = &cave[y][x];

	/* Avoid infinite recursion */
	if (c_ptr->info & (CAVE_TEMP)) return;

	/* Do not "leave" the current room */
	if (!(c_ptr->info & (CAVE_ROOM))) return;

	/* Paranoia -- verify space */
	if (temp_n == TEMP_MAX) return;

	/* Mark the grid as "seen" */
	c_ptr->info |= (CAVE_TEMP);

	/* Add it to the "seen" set */
	temp_y[temp_n] = y;
	temp_x[temp_n] = x;
	temp_n++;
}




/*
 * Illuminate any room containing the given location.
 */
void lite_room(s32b y1, s32b x1)
{
	s32b i, x, y;

	/* Add the initial grid */
	cave_temp_room_aux(y1, x1);

	/* While grids are in the queue, add their neighbors */
	for (i = 0; i < temp_n; i++)
	{
		x = temp_x[i], y = temp_y[i];

		/* Walls get lit, but stop light */
		if (!cave_floor_bold(y, x)) continue;

		/* Spread adjacent */
		cave_temp_room_aux(y + 1, x);
		cave_temp_room_aux(y - 1, x);
		cave_temp_room_aux(y, x + 1);
		cave_temp_room_aux(y, x - 1);

		/* Spread diagonal */
		cave_temp_room_aux(y + 1, x + 1);
		cave_temp_room_aux(y - 1, x - 1);
		cave_temp_room_aux(y - 1, x + 1);
		cave_temp_room_aux(y + 1, x - 1);
	}

	/* Now, lite them all up at once */
	cave_temp_room_lite();
}


/*
 * Darken all rooms containing the given location
 */
void unlite_room(s32b y1, s32b x1)
{
	s32b i, x, y;

	/* Add the initial grid */
	cave_temp_room_aux(y1, x1);

	/* Spread, breadth first */
	for (i = 0; i < temp_n; i++)
	{
		x = temp_x[i], y = temp_y[i];

		/* Walls get dark, but stop darkness */
		if (!cave_floor_bold(y, x)) continue;

		/* Spread adjacent */
		cave_temp_room_aux(y + 1, x);
		cave_temp_room_aux(y - 1, x);
		cave_temp_room_aux(y, x + 1);
		cave_temp_room_aux(y, x - 1);

		/* Spread diagonal */
		cave_temp_room_aux(y + 1, x + 1);
		cave_temp_room_aux(y - 1, x - 1);
		cave_temp_room_aux(y - 1, x + 1);
		cave_temp_room_aux(y + 1, x - 1);
	}

	/* Now, darken them all at once */
	cave_temp_room_unlite();
}



/*
 * Cast a ball spell
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 */
bool fire_ball(s32b typ, s32b dir, s32b dam, s32b rad, s32b max_range)
{
	s32b tx, ty;

	s32b flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;

	/* Use the given direction */
	tx = p_ptr->px + 99 * ddx[dir];
	ty = p_ptr->py + 99 * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5))
	{
		flg &= ~(PROJECT_STOP);
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target".  Hurt items on floor. */
	return (project(spell_caster, (rad > 16) ? 16 : rad, ty, tx, dam, typ, flg, max_range));
}

/*
 * Cast a cone spell
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 */
bool fire_cone(s32b typ, s32b dir, s32b dam, s32b rad, s32b max_range)
{
	s32b tx, ty;

	s32b flg = PROJECT_CONE | PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL | PROJECT_HIDE;

	/* Use the given direction */
	tx = p_ptr->px + 99 * ddx[dir];
	ty = p_ptr->py + 99 * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5))
	{
		flg &= ~(PROJECT_STOP);
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target".  Hurt items on floor. */
	return (project(spell_caster, (rad > 16) ? 16 : rad, ty, tx, dam, typ, flg, max_range));
}

/*
 * Cast a cloud spell
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 */
bool fire_cloud(s32b typ, s32b dir, s32b dam, s32b rad, s32b time, s32b speed, s32b max_range)
{
	s32b tx, ty;

	s32b flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL | PROJECT_STAY;

	/* Use the given direction */
	tx = p_ptr->px + 99 * ddx[dir];
	ty = p_ptr->py + 99 * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5))
	{
		flg &= ~(PROJECT_STOP);
		tx = target_col;
		ty = target_row;
	}
	project_time = time;
	project_time_speed = speed;

	/* Analyze the "dir" and the "target".  Hurt items on floor. */
	return (project(spell_caster, (rad > 16) ? 16 : rad, ty, tx, dam, typ, flg, max_range));
}

/*
 * Cast a wave spell
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 */
bool fire_wave(s32b typ, s32b dir, s32b dam, s32b rad, s32b time, s32b speed, s32b eff, s32b max_range)
{
	project_time_effect = eff;
	return (fire_cloud(typ, dir, dam, rad, time, speed, max_range));
}

/*
 * Cast a persistant beam spell
 * Pass through monsters, as a "beam"
 * Affect monsters (not grids or objects)
 */
bool fire_wall(s32b typ, s32b dir, s32b dam, s32b time, s32b speed, s32b max_range)
{
	s32b flg = PROJECT_BEAM | PROJECT_KILL | PROJECT_STAY | PROJECT_GRID;
	project_time = time;
	project_time_speed = speed;
	return (project_hook(typ, dir, dam, flg, max_range));
}

/*
 * Cast a druidistic ball spell
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 */
bool fire_druid_ball(s32b typ, s32b dir, s32b dam, s32b rad, s32b max_range)
{
	s32b tx, ty;

	s32b flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL | PROJECT_MANA_PATH;

	/* Use the given direction */
	tx = p_ptr->px + 99 * ddx[dir];
	ty = p_ptr->py + 99 * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5))
	{
		flg &= ~(PROJECT_STOP);
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target".  Hurt items on floor. */
	return (project(spell_caster, (rad > 16) ? 16 : rad, ty, tx, dam, typ, flg, max_range));
}


/*
 * Cast a ball-beamed spell
 * Stop if we hit a monster, act as a "ball"
 * Allow "target" mode to pass over monsters
 * Affect grids, objects, and monsters
 */
bool fire_ball_beam(s32b typ, s32b dir, s32b dam, s32b rad, s32b max_range)
{
	s32b tx, ty;

	s32b flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL | PROJECT_BEAM;

	/* Use the given direction */
	tx = p_ptr->px + 99 * ddx[dir];
	ty = p_ptr->py + 99 * ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5))
	{
		flg &= ~(PROJECT_STOP);
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target".  Hurt items on floor. */
	return (project(spell_caster, (rad > 16) ? 16 : rad, ty, tx, dam, typ, flg, max_range));
}


void teleport_swap(s32b dir)
{
	s32b tx, ty;
	cave_type * c_ptr;
	monster_type * m_ptr;

	if (intrinsic(RES_CONTINUUM))
	{
		msg_print("The space-time continuum can't be disrupted.");
		return;
	}

	if ((dir == 5))
	{
		tx = target_col;
		ty = target_row;
	}
	else
	{
		tx = p_ptr->px + ddx[dir];
		ty = p_ptr->py + ddy[dir];
	}
	c_ptr = &cave[ty][tx];

	if (!c_ptr->m_idx)
	{
		msg_print("You can't trade places with that!");
	}
	else
	{
		m_ptr = get_monster(c_ptr->m_idx);

		if ((has_flag(m_ptr, FLAG_RES_TELE)))
		{
			msg_print("Your teleportation is blocked!");
		}
		else
		{
			sound(SOUND_TELEPORT);

			cave[p_ptr->py][p_ptr->px].m_idx = c_ptr->m_idx;

			/* Update the old location */
			c_ptr->m_idx = 0;

			/* Move the monster */
			m_ptr->fy = p_ptr->py;
			m_ptr->fx = p_ptr->px;

			/* Move the player */
			p_ptr->px = tx;
			p_ptr->py = ty;

			tx = m_ptr->fx;
			ty = m_ptr->fy;

			/* Update the monster (new location) */
			update_mon(cave[ty][tx].m_idx, TRUE);

			/* Redraw the old grid */
			lite_spot(ty, tx);

			/* Redraw the new grid */
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
	}
}

void swap_position(s32b lty, s32b ltx)
{
	s32b tx = ltx, ty = lty;
	cave_type * c_ptr;
	monster_type * m_ptr;

	if (intrinsic(RES_CONTINUUM))
	{
		msg_print("The space-time continuum can't be disrupted.");
		return;
	}

	c_ptr = &cave[ty][tx];

	if (!c_ptr->m_idx)
	{
		sound(SOUND_TELEPORT);

		/* Keep trace of the old location */
		tx = p_ptr->px;
		ty = p_ptr->py;

		/* Move the player */
		p_ptr->px = ltx;
		p_ptr->py = lty;

		/* Redraw the old grid */
		lite_spot(ty, tx);

		/* Redraw the new grid */
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
	else
	{
		m_ptr = get_monster(c_ptr->m_idx);

		sound(SOUND_TELEPORT);

		cave[p_ptr->py][p_ptr->px].m_idx = c_ptr->m_idx;

		/* Update the old location */
		c_ptr->m_idx = 0;

		/* Move the monster */
		m_ptr->fy = p_ptr->py;
		m_ptr->fx = p_ptr->px;

		/* Move the player */
		p_ptr->px = tx;
		p_ptr->py = ty;

		tx = m_ptr->fx;
		ty = m_ptr->fy;

		/* Update the monster (new location) */
		update_mon(cave[ty][tx].m_idx, TRUE);

		/* Redraw the old grid */
		lite_spot(ty, tx);

		/* Redraw the new grid */
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
}


/*
 * Hack -- apply a "projection()" in a direction (or at the target)
 */
bool project_hook(s32b typ, s32b dir, s32b dam, s32b flg, s32b max_range)
{
	s32b tx, ty;

	/* Pass through the target if needed */
	flg |= (PROJECT_THRU);

	/* Use the given direction */
	tx = p_ptr->px + ddx[dir];
	ty = p_ptr->py + ddy[dir];

	/* Hack -- Use an actual "target" */
	if ((dir == 5))
	{
		tx = target_col;
		ty = target_row;
	}

	/* Analyze the "dir" and the "target", do NOT explode */
	return (project(spell_caster, 0, ty, tx, dam, typ, flg, max_range));
}


/*
 * Cast a bolt spell
 * Stop if we hit a monster, as a "bolt"
 * Affect monsters (not grids or objects)
 */
bool fire_bolt(s32b typ, s32b dir, s32b dam, s32b max_range)
{
	s32b flg = PROJECT_STOP | PROJECT_KILL;
	return (project_hook(typ, dir, dam, flg, max_range));
}

/*
 * Cast a druidistic bolt spell
 * Stop if we hit a monster, as a "bolt"
 * Affect monsters (not grids or objects)
 */
bool fire_druid_bolt(s32b typ, s32b dir, s32b dam, s32b max_range)
{
	s32b flg = PROJECT_STOP | PROJECT_KILL | PROJECT_MANA_PATH;
	return (project_hook(typ, dir, dam, flg, max_range));
}


/*
 * Cast a druidistic beam spell
 * Pass through monsters, as a "beam"
 * Affect monsters (not grids or objects)
 */
bool fire_druid_beam(s32b typ, s32b dir, s32b dam, s32b max_range)
{
	s32b flg = PROJECT_BEAM | PROJECT_KILL | PROJECT_MANA_PATH;
	return (project_hook(typ, dir, dam, flg, max_range));
}

/*
 * Cast a beam spell
 * Pass through monsters, as a "beam"
 * Affect monsters (not grids or objects)
 */
bool fire_beam(s32b typ, s32b dir, s32b dam, s32b max_range)
{
	s32b flg = PROJECT_BEAM | PROJECT_KILL;
	return (project_hook(typ, dir, dam, flg, max_range));
}


/*
 * Cast a bolt spell, or rarely, a beam spell
 */
bool fire_bolt_or_beam(s32b prob, s32b typ, s32b dir, s32b dam, s32b max_range)
{
	if (rand_int(100) < prob)
	{
		return (fire_beam(typ, dir, dam, max_range));
	}
	else
	{
		return (fire_bolt(typ, dir, dam, max_range));
	}
}

bool fire_godly_wrath(s32b y, s32b x, s32b typ, s32b rad, s32b dam, s32b max_range)
{
	s32b flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;

	return (project(spell_caster, rad, y, x, dam, typ, flg, max_range));
}

bool fire_explosion(s32b y, s32b x, s32b typ, s32b rad, s32b dam, s32b max_range)
{
	s32b flg = PROJECT_STOP | PROJECT_GRID | PROJECT_ITEM | PROJECT_KILL;

	return (project(spell_caster, rad, y, x, dam, typ, flg, max_range));
}


/*
 * Detect all "nonliving", "undead" or "demonic" monsters on current panel
 */
bool detect_monsters_nonliving(s32b rad)
{
	s32b i, y, x;
	bool flag = FALSE;

	/* Scan monsters */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Location */
		y = m_ptr->fy;
		x = m_ptr->fx;

		/* Only detect nearby monsters */
		if (m_ptr->cdis > rad) continue;

		/* Detect evil monsters */
		if ((has_flag(m_ptr, FLAG_NONLIVING)))
		{
			/* Update monster recall window */
			if (monster_race_idx == m_ptr->r_idx)
			{
				/* Window stuff */
				flag_bool(&p_ptr->window, FLAG_PW_MONSTER);
			}

			/* Repair visibility later */
			repair_monsters = TRUE;

			/* Hack -- Detect monster */
			m_ptr->mflag |= (MFLAG_MARK | MFLAG_SHOW);

			/* Hack -- See monster */
			m_ptr->ml = TRUE;

			/* Redraw */
			if (panel_contains(y, x)) lite_spot(y, x);

			/* Detect */
			flag = TRUE;
		}
	}
	end_for_flags();

	/* Describe */
	if (flag)
	{
		/* Describe result */
		msg_print("You sense the presence of unnatural beings!");
	}

	/* Result */
	return (flag);
}



void change_wild_mode(void)
{
	if (intrinsic(BLINK_MOVE) && !p_ptr->wild_mode)
	{
		msg_print("Hum, blinking there will take time.");
	}

	if (p_ptr->word_recall && !p_ptr->wild_mode)
	{
		msg_print("You will soon be recalled.");
		return;
	}

	if (autosave_l)
	{
		is_autosave = TRUE;
		msg_print("Autosaving the game...");
		do_cmd_save_game();
		is_autosave = FALSE;
	}

	if (!p_ptr->wild_mode)
	{
		p_ptr->oldpx = p_ptr->px;
		p_ptr->oldpy = p_ptr->py;

		p_ptr->py = p_ptr->wilderness_y;
		p_ptr->px = p_ptr->wilderness_x;

		p_ptr->wild_force_side_y_ok = 0;
		p_ptr->wild_force_side_x_ok = 0;
	} 
	else if (p_ptr->wild_mode)
	{
		p_ptr->wilderness_y = p_ptr->py;
		p_ptr->wilderness_x = p_ptr->px;

		p_ptr->px = p_ptr->oldpx;
		p_ptr->py = p_ptr->oldpy;
	}

	p_ptr->wild_mode = !p_ptr->wild_mode;

	/* Leaving */
	p_ptr->leaving = TRUE;
}


void alter_reality(void)
{
	msg_print("The world changes!");

	if (autosave_l)
	{
		is_autosave = TRUE;
		msg_print("Autosaving the game...");
		do_cmd_save_game();
		is_autosave = FALSE;
	}

	/* Leaving */
	p_ptr->leaving = TRUE;
}

/*
 * Send the player shooting through walls in the given direction until
 * they reach a non-wall space, or a monster, or a permanent wall.
 */
bool passwall(s32b dir, bool safe)
{
	s32b x = p_ptr->px, y = p_ptr->py, ox = p_ptr->px, oy = p_ptr->py, lx = p_ptr->px, ly = p_ptr->py;
	cave_type *c_ptr;
	bool ok = FALSE;

	if (p_ptr->wild_mode) return FALSE;
	if (p_ptr->inside_quest) return FALSE;
	if ((has_flag(dungeon_flags, FLAG_NO_TELEPORT))) return FALSE;

	/* Must go somewhere */
	if (dir == 5) return FALSE;

	while (TRUE)
	{
		x += ddx[dir];
		y += ddy[dir];
		c_ptr = &cave[y][x];

		/* Perm walls stops the transfer */
		if ((!in_bounds(y, x)) && has_flag(&f_info[c_ptr->feat], FLAG_PERMANENT))
		{
			/* get the last working position */
			x -= ddx[dir];
			y -= ddy[dir];
			ok = FALSE;
			break;
		}

		/* Never on a monster */
		if (c_ptr->m_idx) continue;

		/* Never stop in vaults */
		if (c_ptr->info & CAVE_ICKY) continue;

		/* From now on, the location COULD be used in special case */
		lx = x;
		ly = y;

		/* Pass over walls */
		if (has_flag(&f_info[c_ptr->feat], FLAG_WALL)) continue;

		/* So it must be ok */
		ok = TRUE;
		break;
	}

	if (!ok)
	{
		x = lx;
		y = ly;

		if (!safe)
		{
			msg_print("You emerge in the wall!");
			take_hit(damroll(10, 8), "becoming one with a wall");
		}
		place_floor(y, x);
	}

	/* Move */
	p_ptr->px = x;
	p_ptr->py = y;

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

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);

	/* Handle stuff XXX XXX XXX */
	handle_stuff();

	return (TRUE);
}

/*
 * Print a batch of dungeons.
 */
static void print_dungeon_batch(s32b *p, s32b start, s32b max, bool mode)
{
	char buf[80];
	s32b i, j;
	byte attr;


	if (mode) prt(format("     %-31s", "Name"), 1, 20);

	for (i = 0, j = start; i < 20 && j < max; i++, j++)
	{
		dungeon_info_type *d_ptr = &d_info[p[j]];

		strnfmt(buf, 80, "  %c) %-30s", I2A(i), d_ptr->name);
		if (mode)
		{
			if (d_ptr->min_plev > p_ptr->lev)
			{
				attr = TERM_L_DARK;
			}
			else
			{
				attr = TERM_WHITE;
			}
			c_prt(attr, buf, 2 + i, 20);
		}
	}
	if (mode) prt("", 2 + i, 20);

	prt(format("Select a dungeon (a-%c), * to list, @ to select by name, +/- to scroll:", I2A(i - 1)), 0, 0);
}

s32b reset_recall_aux()
{
	s32b which;
	s32b  *p;
	s32b  max = 0, i, start = 0;
	s32b  ret;
	bool mode = easy_inven;


	C_MAKE(p, max_d_idx, s32b);

	/* Count the max */
	for (i = 1; i < max_d_idx; i++)
	{
		/* skip "blocked" dungeons */
		if (has_flag(&d_info[i], FLAG_NO_RECALL)) continue;

		if (max_dlv[i])
		{
			p[max++] = i;
		}
	}

	character_icky++;
	Term_save();

	while (1)
	{
		print_dungeon_batch(p, start, max, mode);
		which = inkey();

		if (which == ESCAPE)
		{
			ret = -1;
			break;
		}

		else if (which == '*' || which == '?' || which == ' ')
		{
			mode = (mode) ? FALSE : TRUE;
			Term_load();
			character_icky--;
		}

		else if (which == '+')
		{
			start += 20;
			if (start >= max) start -= 20;
			Term_load();
			character_icky--;
		}

		else if (which == '-')
		{
			start -= 20;
			if (start < 0) start += 20;
			Term_load();
			character_icky--;
		}

		else if (which == '@')
		{
			char buf[80], buf2[80];

			strcpy(buf, (d_info[p_ptr->recall_dungeon].name));
			if (!get_string("Which dungeon? ", buf, 79)) continue;

			/* Find the index corresponding to the name */
			for (i = 1; i < max_d_idx; i++)
			{
				sprintf(buf2, "%s", d_info[i].name);

				/* Lowercase the name */
				strlower(buf);
				strlower(buf2);
				if (strstr(buf2, buf))
				{
					/* valid dungeon found */
					break;
				}
			}

			if (i >= max_d_idx)
			{
				msg_print("Never heard of that place!");
				msg_print(NULL);
				continue;
			}
			else if (has_flag(&d_info[i], FLAG_NO_RECALL))
			{
				msg_print("This place blocks my magic!");
				msg_print(NULL);
				continue;
			}
			else if (d_info[i].min_plev > p_ptr->lev)
			{
				msg_print("You cannot go there yet!");
				msg_print(NULL);
				continue;
			}
			ret = i;
			break;
		}

		else
		{
			which = tolower(which);
			if (start + A2I(which) >= max)
			{
				bell();
				continue;
			}
			if (start + A2I(which) < 0)
			{
				bell();
				continue;
			}
			ret = p[start + A2I(which)];
			break;
		}
	}

	Term_load();
	character_icky--;

	C_FREE(p, max_d_idx, s32b);

	return ret;
}

bool reset_recall(bool no_trepas_max_depth)
{
	s32b dun, depth, max;

	/* Choose dungeon */
	dun = reset_recall_aux();

	if (dun < 1) return FALSE;

	/* Choose depth */
	if (!no_trepas_max_depth)
		max = d_info[dun].maxdepth;
	else
		max = max_dlv[dun];
	depth = get_quantity(format("Which level in %s(%d-%d)? ",
	                            d_info[dun].name,
	                            d_info[dun].mindepth, max),
	                     max);

	if (depth < 1) return FALSE;

	/* Enforce minimum level */
	if (depth < d_info[dun].mindepth) depth = d_info[dun].mindepth;

	/* Mega hack -- Forbid levels 99 and 100 */
	if ((depth == 99) || (depth == 100)) depth = 98;

	p_ptr->recall_dungeon = dun;
	max_dlv[p_ptr->recall_dungeon] = depth;

	return TRUE;
}

/* The only way to get rid of the dreaded MORG_CURSE*/
void remove_morg_curse()
{
	/* Parse all the items */
	for_inventory(p_ptr, o_ptr, INVEN_PACK, INVEN_TOTAL);
	{
		if (o_ptr->k_idx && ((has_flag(o_ptr, FLAG_MORG_CURSE))))
		{
			unset_flag(o_ptr, FLAG_HEAVY_CURSE);
			unset_flag(o_ptr, FLAG_CURSED);
			unset_flag(o_ptr, FLAG_MORG_CURSE);
			msg_print("The Morgothian Curse withers away.");
		}
	}
	end_inventory();
}

/*
 * Creates a between gate
 */
void create_between_gate(s32b dist, s32b y, s32b x)
{
	s32b ii, ij, plev = get_skill(SKILL_CONVEYANCE);

	if ((has_flag(dungeon_flags, FLAG_NO_TELEPORT)))
	{
		msg_print("Not on special levels!");
		return;
	}

	if ((!x) || (!y))
	{
		msg_print("You open a Void Jumpgate. Choose a destination.");

		if (!tgt_pt(&ii, &ij)) return;
		p_ptr->energy -= get_player_energy(SPEED_GLOBAL) * (60 - plev) / 100;

		if (!cave_empty_bold(ij, ii) ||
		                (cave[ij][ii].info & CAVE_ICKY) ||
		                (distance(ij, ii, p_ptr->py, p_ptr->px) > dist) ||
		                (rand_int(plev * plev / 2) == 0))
		{
			msg_print("You fail to exit the void correctly!");
			p_ptr->energy -= get_player_energy(SPEED_GLOBAL);
			get_pos_player(10, &ij, &ii);
		}
	}
	else
	{
		ij = y;
		ii = x;
	}
#if 0 //DGDGDGDG
	if (!has_flag(&f_info[cave[p_ptr->py][p_ptr->px].feat], FLAG_PERMANENT))
	{
		cave_set_feat(p_ptr->py, p_ptr->px, FEAT_BETWEEN);
		cave[p_ptr->py][p_ptr->px].special = ii + (ij << 8);
	}
	if (!has_flag(&f_info[cave[ij][ii].feat], FLAG_PERMANENT))
	{
		cave_set_feat(ij, ii, FEAT_BETWEEN);
		cave[ij][ii].special = p_ptr->px + (p_ptr->py << 8);
	}
#endif
}
