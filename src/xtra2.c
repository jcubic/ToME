/* File: xtra2.c */
/* File: xtra2.c */

/* Purpose: effects of various "objects", targetting and panel handling */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

void drop_from_wild()
{
	/* Hack -- Not if player were in normal mode in previous turn */
	if (!p_ptr->old_wild_mode) return;

	if (p_ptr->wild_mode && (!dun_level))
	{
		p_ptr->wilderness_x = p_ptr->px;
		p_ptr->wilderness_y = p_ptr->py;
		change_wild_mode();
		p_ptr->energy = TURN_ENERGY;
		energy_use = 0;
	}
}

/*
 * Advance experience levels and print experience
 */
void check_experience_obj(object_type *o_ptr)
{
	s32b i;



	/* Note current level */
	i = o_ptr->elevel;

	/* Hack -- lower limit */
	if (o_ptr->exp < 0) o_ptr->exp = 0;

	/* Hack -- upper limit */
	if (o_ptr->exp > PY_MAX_EXP) o_ptr->exp = PY_MAX_EXP;

	/* Gain levels while possible */
	while ((o_ptr->elevel < PY_MAX_LEVEL) && (o_ptr->exp >= player_exp[o_ptr->elevel - 1]))
	{
		char buf[100];

		/*
		 * Object might do different things on gaining a level based on
		 * the player's bonuses, so recalculate them before each level
		 * gain.
		 */
		calc_bonuses(TRUE);

		/* Get object name */
		object_desc(buf, o_ptr, 1, 0);

		if (process_hooks(HOOK_OBJECT_LEVEL_PRE, "(O)", o_ptr))
		{
			if (wizard)
				printf("Skipping obj leveling for %s\n", buf);
			break;
		}

		/* Add a level */
		o_ptr->elevel++;

		if (object_known_p(o_ptr))
			cmsg_format(TERM_L_BLUE, "%s gains a level!", buf);
		else
			msg_format("%s suddenly seems to be more powerful.", buf);

		if (process_hooks(HOOK_OBJECT_LEVEL_DO, "(O)", o_ptr))
		{
			process_hooks(HOOK_OBJECT_LEVEL_POST, "(O)", o_ptr);
			continue;
		}

		/* What does it gains ? */
		open_lua_functions_registry(flag_get(&o_ptr->flags, FLAG_LEVELS), flag_get2(&o_ptr->flags, FLAG_LEVELS));
		call_lua(NULL, "(O)", "", o_ptr);
		close_lua_functions_registry();

		process_hooks(HOOK_OBJECT_LEVEL_POST, "(O)", o_ptr);
	}

	/*
	 * If the object is fully identified, then learn any new flags
	 * it might have acquired while gaining a level.
	 */
	if (o_ptr->ident & IDENT_MENTAL)
		flag_learn_all(&o_ptr->flags);

	/*
	 * Object's properties have most likely changed, so recalculate
	 * bonuses.
	 */
	calc_bonuses(FALSE);
}

/*
 * Advance experience levels and print experience
 */
void check_experience(void)
{
	s32b i, gained = 0;

	/* Note current level */
	i = p_ptr->lev;

	/* Hack -- lower limit */
	if (p_ptr->exp < 0) p_ptr->exp = 0;

	/* Hack -- lower limit */
	if (p_ptr->max_exp < 0) p_ptr->max_exp = 0;

	/* Hack -- upper limit */
	if (p_ptr->exp > PY_MAX_EXP) p_ptr->exp = PY_MAX_EXP;

	/* Hack -- upper limit */
	if (p_ptr->max_exp > PY_MAX_EXP) p_ptr->max_exp = PY_MAX_EXP;

	/* Hack -- maintain "max" experience */
	if (p_ptr->exp > p_ptr->max_exp) p_ptr->max_exp = p_ptr->exp;

	/* Redraw experience */
	flag_bool(&p_ptr->redraw, FLAG_PR_EXP);

	/* Handle stuff */
	handle_stuff();


	/* Lose levels while possible */
	while ((p_ptr->lev > 1) &&
	                (p_ptr->exp < (player_exp[p_ptr->lev - 2] * p_ptr->expfact / 100L)))
	{
		/* Lose a level */
		p_ptr->lev--;
		gained--;
		lite_spot(p_ptr->py, p_ptr->px);

		/* Update some stuff */
		p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS | PU_SANITY);

		/* Redraw some stuff */
		flag_bool(&p_ptr->redraw, FLAG_PR_LEV);
		flag_bool(&p_ptr->redraw, FLAG_PR_TITLE);
		flag_bool(&p_ptr->redraw, FLAG_PR_EXP);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

		/* Handle stuff */
		handle_stuff();
	}


	/* Gain levels while possible */
	while ((p_ptr->lev < PY_MAX_LEVEL) && (p_ptr->lev < max_plev) &&
	                (p_ptr->exp >= (player_exp[p_ptr->lev - 1] * p_ptr->expfact / 100L)))
	{
		/* Gain a level */
		p_ptr->lev++;
		gained++;
		lite_spot(p_ptr->py, p_ptr->px);

		/* Save the highest level */
		if (p_ptr->lev > p_ptr->max_plv)
		{
			p_ptr->max_plv = p_ptr->lev;
		}

		/* Sound */
		sound(SOUND_LEVEL);

		/* Message */
		cmsg_format(TERM_L_GREEN, "Welcome to level %d.", p_ptr->lev);

		if (p_ptr->skill_last_level < p_ptr->lev)
		{
			s32b pts;
			call_lua("get_module_info", "(s)", "d", "skill_per_level", &pts);

			p_ptr->skill_last_level = p_ptr->lev;
			p_ptr->skill_points += pts;
			if (p_ptr->skill_points)
				cmsg_format(TERM_L_GREEN, "You can increase %d more skills.", p_ptr->skill_points);
			flag_bool(&p_ptr->redraw, FLAG_PR_STUDY);
		}

		/* Gain this level's abilities */
		apply_level_abilities(p_ptr->lev);

		/* If auto-note taking enabled, write a note to the file.
		 * Only write this note when the level is gained for the first
		 * time.
		 */
		if (take_notes && auto_notes)
		{
			char note[80];

			/* Write note */
			sprintf(note, "Reached level %d", p_ptr->lev);
			add_note(note, 'L');
		}

		/* Update some stuff */
		p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS | PU_SANITY);

		/* Redraw some stuff */
		flag_bool(&p_ptr->redraw, FLAG_PR_LEV);
		flag_bool(&p_ptr->redraw, FLAG_PR_TITLE);
		flag_bool(&p_ptr->redraw, FLAG_PR_EXP);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

		/* Handle stuff */
		handle_stuff();
	}

	/* Hook it! */
	process_hooks(HOOK_PLAYER_LEVEL, "(d)", gained);
}

/*
 * Gain experience (share it to objects if needed)
 */
void gain_exp(s32b amount)
{
	/* Hook it! */
	if (process_hooks_ret(HOOK_PLAYER_EXP, "d", "(d)", amount))
		amount = process_hooks_return[0].num;

	/* Gain some experience */
	p_ptr->exp += amount;

	/* Slowly recover from experience drainage */
	if (p_ptr->exp < p_ptr->max_exp)
	{
		/* Gain max experience (20%) (was 10%) */
		p_ptr->max_exp += amount / 5;
	}

	/* Check Experience */
	check_experience();
}


/*
 * Lose experience
 */
void lose_exp(s32b amount)
{
	/* Never drop below zero experience */
	if (amount > p_ptr->exp) amount = p_ptr->exp;

	/* Lose some experience */
	p_ptr->exp -= amount;

	/* Hook it! */
	process_hooks(HOOK_PLAYER_EXP, "(d)", amount);

	/* Check Experience */
	check_experience();
}




/*
 * Hack -- Return the "automatic coin type" of a monster race
 * Used to allocate proper treasure when "Creeping coins" die
 *
 * XXX XXX XXX Note the use of actual "monster names"
 */
s32b get_coin_type(monster_race *r_ptr)
{
	cptr name = (r_ptr->name);

	/* Analyze "coin" monsters */
	if (r_ptr->d_char == '$')
	{
		/* Look for textual clues */
		if (strstr(name, " copper ")) return (2);
		if (strstr(name, " silver ")) return (5);
		if (strstr(name, " gold ")) return (10);
		if (strstr(name, " mithril ")) return (16);
		if (strstr(name, " adamantite ")) return (17);

		/* Look for textual clues */
		if (strstr(name, "Copper ")) return (2);
		if (strstr(name, "Silver ")) return (5);
		if (strstr(name, "Gold ")) return (10);
		if (strstr(name, "Mithril ")) return (16);
		if (strstr(name, "Adamantite ")) return (17);
	}

	/* Assume nothing */
	return (0);
}

/*
 * This routine handles the production of corpses/skeletons/heads/skulls
 * when a monster is killed.
 */
static bool place_corpse(monster_type *m_ptr)
{
	monster_race *r_ptr = race_inf(m_ptr);

	object_type *i_ptr;

	s32b x = m_ptr->fx;
	s32b y = m_ptr->fy;

	bool corpse = has_flag(m_ptr, FLAG_DROP_CORPSE);
	bool skel   = has_flag(m_ptr, FLAG_DROP_SKELETON);

	if (!(corpse || skel))
		return FALSE;

	if(process_hooks(HOOK_CORPSE_CREATE_PRE, "(d)", m_ptr->r_idx))
		return FALSE;

	if (corpse)
	{
		/* Get local object */
		i_ptr = new_object();

		/* Wipe the object */
		object_prep(i_ptr, lookup_kind(TV_CORPSE, SV_CORPSE_CORPSE));

		/* Calculate length of time before decay */
		set_flag(i_ptr, FLAG_MONSTER_DECAY,
				 r_ptr->weight + rand_int(r_ptr->weight));

		/* Set weight */
		i_ptr->weight = (r_ptr->weight + rand_int(r_ptr->weight) / 10) + 1;
	}
	else
	{
		/* Get local object */
		i_ptr = new_object();

		/* Wipe the object */
		object_prep(i_ptr, lookup_kind(TV_CORPSE, SV_CORPSE_SKELETON));

		set_flag(i_ptr, FLAG_MONSTER_DECAY, 0);
		
		/* Set weight */
		i_ptr->weight = (r_ptr->weight / 4 + rand_int(r_ptr->weight) / 40) + 1;
	}

	/* Save the monster object */
	flag_set_monster(&i_ptr->flags, FLAG_MONSTER_OBJ,
					 monster_type_dup(m_ptr));

	i_ptr->found = OBJ_FOUND_MONSTER;
	i_ptr->found_aux1 = m_ptr->r_idx;
	i_ptr->found_aux2 = m_ptr->ego;
	i_ptr->found_aux3 = dungeon_type;
	i_ptr->found_aux4 = level_or_feat(dungeon_type, dun_level);

	/* Drop it in the dungeon */
	if (drop_near(i_ptr, -1, y, x) == 0)
		/* Failed */
		return FALSE;

	process_hooks(HOOK_CORPSE_CREATE_POST, "(O,M)", i_ptr, m_ptr);

	return TRUE;
} /* place_corpse() */


/*
 * Handle the "death" of a monster.
 *
 * Disperse treasures centered at the monster location based on the
 * various flags contained in the monster flags fields.
 *
 * Check for "Quest" completion when a quest monster is killed.
 *
 * Note that only the player can induce "monster_death()" on Uniques.
 * Thus (for now) all Quest monsters should be Uniques.
 *
 * Note that monsters can now carry objects, and when a monster dies,
 * it drops all of its objects, which may disappear in crowded rooms.
 */
void monster_death(s32b m_idx, s32b who)
{
	s32b y, x;

	s32b dump_item = 0;
	s32b dump_gold = 0;

	monster_type *m_ptr = get_monster(m_idx);

	monster_race *r_ptr = race_inf(m_ptr);

	bool visible = (m_ptr->ml || (has_flag(m_ptr, FLAG_UNIQUE)));

	bool cloned = FALSE;
	s32b force_coin = get_coin_type(r_ptr);

	if (m_ptr->r_idx == 0)
	{
		s32b y = m_ptr->fy;
		s32b x = m_ptr->fx;

		cmsg_format(TERM_L_RED, "ERROR: monster_death()'s m_ptr is "
					"already dead");

		if (in_bounds(y, x))
			cave[y][x].m_idx = 0;

		return;
	}

	/* Get the location */
	y = m_ptr->fy;
	x = m_ptr->fx;

	/* Process the appropriate hooks */
	process_hooks(HOOK_MONSTER_DEATH, "(d,d)", m_idx, who);

	/* If companion dies, take note */
	if ((m_ptr->faction == FACTION_PLAYER) && (has_flag(m_ptr, FLAG_PERMANENT))) p_ptr->companion_killed++;

	/* Handle reviving if undead */
	if ((p_ptr->necro_extra & CLASS_UNDEAD) && p_ptr->necro_extra2)
	{
		p_ptr->necro_extra2--;

		if (!p_ptr->necro_extra2)
		{
			msg_print("Your death has been avenged -- you return to life.");
			p_ptr->necro_extra &= ~CLASS_UNDEAD;

			/* Display the hitpoints */
			p_ptr->update |= (PU_HP);
			flag_bool(&p_ptr->redraw, FLAG_PR_HP);

			/* Window stuff */
			flag_bool(&p_ptr->window, FLAG_PW_PLAYER);
		}
		else
		{
			msg_format("You still have to kill %d monster%s.", p_ptr->necro_extra2, (p_ptr->necro_extra2 == 1) ? "" : "s");
		}
	}

	/* Handle the possibility of player vanquishing arena combatant -KMW- */
	if (p_ptr->inside_arena)
	{
		p_ptr->exit_bldg = TRUE;
		msg_print("Victorious! You're on your way to becoming Champion.");
		p_ptr->arena_number++;
	}

	if (m_ptr->smart &(SM_CLONED)) cloned = TRUE;

	/* If the doppleganger die, the variable must be set accordingly */
	if (has_flag(m_ptr, FLAG_DOPPLEGANGER)) doppleganger = 0;

	/* Drop objects being carried */
	for_inventory_copy(m_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		/* Remove it from the real inventory */
		flag_remove_zap(get_inven(m_ptr, inven_nb), item_nb);

		o_ptr->held_m_idx = 0;

		if (has_flag(o_ptr, FLAG_GOLD_VALUE)) dump_gold++;
		else dump_item++;

		/* Drop it */
		drop_near(o_ptr, -1, y, x);
	}
	end_inventory_copy();

	/* Average dungeon and monster levels */
	object_level = (dun_level + m_ptr->level) / 2;

	/* Hack - the protected monsters must be advanged */
	if (has_flag(m_ptr, FLAG_WYRM_PROTECT))
	{
		s32b xx = x, yy = y;
		s32b attempts = 100;

		cmsg_print(TERM_VIOLET, "This monster was under the protection of a great wyrm of power!");

		do
		{
			scatter(&yy, &xx, y, x, 6, 0);
		}
		while (!(in_bounds(yy, xx) && cave_floor_bold(yy, xx)) && --attempts);

		place_monster_aux(yy, xx, test_monster_name("Great Wyrm of Power"), FALSE, FALSE, m_ptr->faction);
	}

	/* Let monsters explode! */
	call_lua("monster_explode", "(d)", "", m_idx);

	if (force_coin || !(has_flag(m_ptr, FLAG_DROP_CORPSE) ||
						has_flag(m_ptr, FLAG_DROP_SKELETON)))
		; /* Do nothing */
	else if (process_hooks(HOOK_DO_DROP_CORPSE, "(M)", m_ptr))
		place_corpse(m_ptr);
	else if (wizard && m_ptr->ml)
	{
		char buf[80];
		char m_name[80];

		monster_desc(m_name, m_ptr, 0x80);

		sprintf(buf, "Force corpse drop for %s?", m_name);
				
		if (get_check(buf))
			place_corpse(m_ptr);
	}

	/* Take note of any dropped treasure */
	if (visible && (dump_item || dump_gold))
	{
		/* Take notes on treasure */
		lore_treasure(m_idx, dump_item, dump_gold);
	}
} /* void monster_death() */




/*
 * Decreases monsters hit points, handling monster death.
 *
 * We return TRUE if the monster has been killed (and deleted).
 *
 * We announce monster death (using an optional "death message"
 * if given, and a otherwise a generic killed/destroyed message).
 *
 * Only "physical attacks" can induce the "You have slain" message.
 * Missile and Spell attacks will induce the "dies" message, or
 * various "specialized" messages.  Note that "You have destroyed"
 * and "is destroyed" are synonyms for "You have slain" and "dies".
 *
 * Hack -- unseen monsters yield "You have killed it." message.
 *
 * Added fear (DGK) and check whether to print fear messages -CWS
 *
 * Genericized name, sex, and capitilization -BEN-
 *
 * Hack -- we "delay" fear messages by passing around a "fear" flag.
 *
 * XXX XXX XXX Consider decreasing monster experience over time, say,
 * by using "(m_exp * m_lev * (m_lev)) / (p_lev * (m_lev + n_killed))"
 * instead of simply "(m_exp * m_lev) / (p_lev)", to make the first
 * monster worth more than subsequent monsters.  This would also need
 * to induce changes in the monster recall code.
 */
bool mon_take_hit(s32b m_idx, s32b dam, bool *fear, cptr note)
{
	monster_type *m_ptr = get_monster(m_idx);
	monster_race *r_ptr = race_inf(m_ptr);
	s32b new_exp;


	/* Redraw (later) if needed */
	if (health_who == m_idx) flag_bool(&p_ptr->redraw, FLAG_PR_HEALTH);

	/* Some mosnters are immune to death */
	if (has_flag(m_ptr, FLAG_NO_DEATH)) return FALSE;

	if (process_hooks_ret(HOOK_MONSTER_TAKE_HIT, "d", "(d,d,M,d)", 0, m_idx, m_ptr, dam))
		dam = process_hooks_return[0].num;

	if (dam == 0) return FALSE;

	/* Wake it up */
	m_ptr->csleep = 0;

	/* Hurt it */
	m_ptr->hp -= dam;

	/* Notice the player */
	if (wizard) {
		char m_name[80];

		monster_desc(m_name, m_ptr, 0x80);
		msg_format("%s take hit for #G%d#W damage.", m_name, dam);
	}

	/* It is dead now */
	if (m_ptr->hp < 0)
	{
		char m_name[80];

		/* Lets face it, you cannot get rid of a possessor that easily */
		if (m_ptr->possessor)
		{
			ai_deincarnate(m_idx);

			return FALSE;
		}

		/* Extract monster name */
		monster_desc(m_name, m_ptr, 0);

		/* Make a sound */
		if (has_flag(m_ptr, FLAG_SOUND_KILL))
			sound(get_flag(m_ptr, FLAG_SOUND_KILL));
		else
			sound(SOUND_KILL);

		/* Death by Missile/Spell attack */
		if (note)
		{
			cmsg_format(TERM_L_RED, "%^s%s", m_name, note);
		}

		/* Death by physical attack -- invisible monster */
		else if (!m_ptr->ml)
		{
			cmsg_format(TERM_L_RED, "You have killed %s.", m_name);
		}

		/* Death by Physical attack -- special verb */
		else if (has_flag(m_ptr, FLAG_KILLED_VERB))
		{
			cmsg_format(TERM_L_RED, "You have %s %s.",
						flag_get_string(&m_ptr->flags, FLAG_KILLED_VERB),
						m_name);
		}

		/* Death by Physical attack -- normal verb monster */
		else
		{
			cmsg_format(TERM_L_RED, "You have slain %s.", m_name);
		}

		call_lua("player.gain_exp_from_monster", "(M)", "d", m_ptr, &new_exp);

		/* Gain experience */
		if (new_exp > 0)
			gain_exp(new_exp);

		/* When the player kills a Unique, it stays dead */
		if (has_flag(m_ptr, FLAG_UNIQUE))
		{
			r_ptr->max_num = 0;
		}

		/* Generate treasure */
		monster_death(m_idx, WHO_PLAYER);

		/*
		* XXX XXX XXX Mega-Hack -- Remove random quest rendered
		* impossible
		*/
		if (has_flag(m_ptr, FLAG_UNIQUE))
		{
			s32b i;

			/* Search for all the random quests */
			for (i = 0; i < MAX_RANDOM_QUEST; i++)
			{
				random_quest *q_ptr = &random_quests[i];

				/* Ignore invalid entries */
				if (q_ptr->type == 0) continue;

				/* It's done */
				if (q_ptr->done) continue;

				/*
				 * XXX XXX XXX Not yet completed quest is
				 * to kill a unique you've just killed
				 */
				if (r_ptr == &r_info[q_ptr->r_idx])
				{
					/* Invalidate it */
					q_ptr->type = 0;
				}
			}
		}

		/* If the player kills a Unique, and the notes options are on, write a note */
		if (has_flag(m_ptr, FLAG_UNIQUE) && take_notes && auto_notes)
		{
			char note[80];

			/* Get true name even if blinded/hallucinating */
			cptr monst = (r_ptr->name);

			/* Write note */
			sprintf(note, "Killed %s", monst);

			add_note(note, 'U');
		}

		/* Recall even invisible uniques or winners */
		if (m_ptr->ml || (has_flag(m_ptr, FLAG_UNIQUE)))
		{
			/* Count kills this life */
			if (r_ptr->r_pkills < MAX_SHORT) r_ptr->r_pkills++;

			/* Count kills in all lives */
			if (r_ptr->r_tkills < MAX_SHORT) r_ptr->r_tkills++;

			/* Hack -- Auto-recall */
			monster_race_track(m_ptr->r_idx, m_ptr->ego);
		}

		/* Delete the monster */
		delete_monster_idx(m_idx);

		/* Not afraid */
		(*fear) = FALSE;

		/* Monster is dead */
		return (TRUE);
	}

	/* Not dead yet */
	return (FALSE);
}


/*
 * Get term size and calculate screen size
 */
void get_screen_size(s32b *wid_p, s32b *hgt_p)
{
	*wid_p = COL_MAP_WID;
	*hgt_p = ROW_MAP_HGT;
	*hgt_p -= ROW_MAP + 1;
	*wid_p -= COL_MAP + 1;
	if (use_bigtile) *wid_p /= 2;
}

/*
 * Calculates current boundaries
 * Called below.
 */
void panel_bounds(void)
{
	s32b wid, hgt;

	/* Retrieve current screen size */
	get_screen_size(&wid, &hgt);

	/* + 24 - 1 - 2 =  + 21 */
	panel_row_max = panel_row_min + hgt - 1;
	panel_row_prt = panel_row_min - ROW_MAP;

	/* Paranoia -- Boundary check */
	if (panel_row_max > cur_hgt - 1) panel_row_max = cur_hgt - 1;

	panel_col_max = panel_col_min + wid - 1;
	panel_col_prt = panel_col_min - COL_MAP;

	/* Paranoia -- Boundary check */
	if (panel_col_max > cur_wid - 1) panel_col_max = cur_wid - 1;
}


/*
 * Handle a request to change the current panel
 *
 * Return TRUE if the panel was changed.
 *
 * Also used in do_cmd_locate()
 */
bool change_panel(s32b dy, s32b dx)
{
	s32b y, x;
	s32b wid, hgt;

	/* Get size */
	get_screen_size(&wid, &hgt);

	/* Apply the motion */
	y = panel_row_min + dy * (hgt / 2);
	x = panel_col_min + dx * (wid / 2);

	/* Calculate bounds */
	if (y > cur_hgt - hgt) y = cur_hgt - hgt;
	if (y < 0) y = 0;
	if (x > cur_wid - wid) x = cur_wid - wid;
	if (x < 0) x = 0;

	/* Handle changes */
	if ((y != panel_row_min) || (x != panel_col_min))
	{
		/* Save the new panel info */
		panel_row_min = y;
		panel_col_min = x;

		/* Recalculate the boundaries */
		panel_bounds();

		/* Update stuff */
		p_ptr->update |= (PU_MONSTERS);

		/* Redraw map */
		flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

		/* Handle stuff */
		handle_stuff();

		/* Success */
		return (TRUE);
	}

	/* No changes */
	return (FALSE);
}


/*
 * Given an row (y) and col (x), this routine detects when a move
 * off the screen has occurred and figures new borders. -RAK-
 *
 * "Update" forces a "full update" to take place.
 *
 * The map is reprinted if necessary, and "TRUE" is returned.
 */
void verify_panel(void)
{
	s32b y = p_ptr->py;
	s32b x = p_ptr->px;

	s32b wid, hgt;

	s32b prow_min;
	s32b pcol_min;

	s32b panel_hgt, panel_wid;

	s32b max_prow_min;
	s32b max_pcol_min;


	/*
	 * Make sure panel_row/col_max have correct values -- now taken care of
	 * by the hook function below, which eliminates glitches, but does it
	 * in a very hackish way XXX XXX XXX
	 */
	/* panel_bounds(); */

	/* Get size */
	get_screen_size(&wid, &hgt);

	/* Calculate scroll amount */
	panel_hgt = hgt / 2;
	panel_wid = wid / 2;

	/* Upper boundary of panel_row/col_min */
	max_prow_min = cur_hgt - hgt;
	max_pcol_min = cur_wid - wid;

	/* Boundary check */
	if (max_prow_min < 0) max_prow_min = 0;
	if (max_pcol_min < 0) max_pcol_min = 0;

	/* An option: center on player */
	if (center_player)
	{
		/* Center vertically */
		prow_min = y - panel_hgt;

		/* Boundary check */
		if (prow_min < 0) prow_min = 0;
		else if (prow_min > max_prow_min) prow_min = max_prow_min;

		/* Center horizontally */
		pcol_min = x - panel_wid;

		/* Boundary check */
		if (pcol_min < 0) pcol_min = 0;
		else if (pcol_min > max_pcol_min) pcol_min = max_pcol_min;
	}

	/* No centering */
	else
	{
		prow_min = panel_row_min;
		pcol_min = panel_col_min;

		/* Scroll screen when 2 grids from top/bottom edge */
		if (y > panel_row_max - 2)
		{
			while (y > prow_min + hgt - 2)
			{
				prow_min += panel_hgt;
			}

			if (prow_min > max_prow_min) prow_min = max_prow_min;
		}

		if (y < panel_row_min + 2)
		{
			while (y < prow_min + 2)
			{
				prow_min -= panel_hgt;
			}

			if (prow_min < 0) prow_min = 0;
		}

		/* Scroll screen when 4 grids from left/right edge */
		if (x > panel_col_max - 4)
		{
			while (x > pcol_min + wid - 4)
			{
				pcol_min += panel_wid;
			}

			if (pcol_min > max_pcol_min) pcol_min = max_pcol_min;
		}

		if (x < panel_col_min + 4)
		{
			while (x < pcol_min + 4)
			{
				pcol_min -= panel_wid;
			}

			if (pcol_min < 0) pcol_min = 0;
		}
	}

	/* Check for "no change" */
	if ((prow_min == panel_row_min) && (pcol_min == panel_col_min)) return;

	/* Save the new panel info */
	panel_row_min = prow_min;
	panel_col_min = pcol_min;

	/* Hack -- optional disturb on "panel change" */
	if (disturb_panel && !center_player) disturb(0, 0);

	/* Recalculate the boundaries */
	panel_bounds();

	/* Update stuff */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);
}


/*
 * Map resizing whenever the main term changes size
 */
void resize_map(void)
{
	/* Only if the dungeon exists */
	if (!character_dungeon) return;

	/* Mega-Hack -- No panel yet, assume illegal panel */
	panel_row_min = cur_hgt;
	panel_row_max = 0;
	panel_col_min = cur_wid;
	panel_col_max = 0;

	/* Select player panel */
	verify_panel();

	/*
	 * The following should be the same as the main window code
	 * in the do_cmd_redraw()
	 */

	/* Combine and reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	/* Update torch */
	p_ptr->update |= (PU_TORCH);

	/* Update stuff */
	p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS | PU_POWERS |
	                  PU_SANITY | PU_BODY);

	/* Forget and update view */
	p_ptr->update |= (PU_UN_VIEW | PU_VIEW | PU_MONSTERS | PU_MON_LITE);

	/* Redraw everything */
	flag_bool(&p_ptr->redraw, FLAG_PR_WIPE);
	flag_bool(&p_ptr->redraw, FLAG_PR_BASIC);
	flag_bool(&p_ptr->redraw, FLAG_PR_EXTRA);
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

	/* Hack -- update */
	handle_stuff();

	/* Redraw */
	Term_redraw();

	/* Refresh */
	Term_fresh();
}


/*
 * Redraw a term when it is resized
 */
void resize_window(void)
{
	/* Only if the dungeon exists */
	if (!character_dungeon) return;

	/* Hack -- Activate the Angband window for the redraw */
	Term_activate(&term_screen[0]);

	/* Hack -- react to changes */
	Term_xtra(TERM_XTRA_REACT, 0);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_ALL);


	/* Hack -- update */
	handle_stuff();

	/* Refresh */
	Term_fresh();
}




/*
 * Monster health description
 */
cptr look_mon_desc(s32b m_idx)
{
	monster_type *m_ptr = get_monster(m_idx);

	bool living = TRUE;
	s32b perc;

	/* Determine if the monster is "living" (vs "undead") */
	if (has_flag(m_ptr, FLAG_NONLIVING)) living = FALSE;

	/* Healthy monsters */
	if (m_ptr->hp >= m_ptr->maxhp)
	{
		/* No damage */
		return (living ? "unhurt" : "undamaged");
	}


	/* Calculate a health "percentage" */
	perc = 100L * m_ptr->hp / m_ptr->maxhp;

	if (perc >= 60)
	{
		return (living ? "somewhat wounded" : "somewhat damaged");
	}

	if (perc >= 25)
	{
		return (living ? "wounded" : "damaged");
	}

	if (perc >= 10)
	{
		return (living ? "badly wounded" : "badly damaged");
	}

	return (living ? "almost dead" : "almost destroyed");
}



/*
 * Angband sorting algorithm -- quick sort in place
 *
 * Note that the details of the data we are sorting is hidden,
 * and we rely on the "ang_sort_comp()" and "ang_sort_swap()"
 * function hooks to interact with the data, which is given as
 * two pointers, and which may have any user-defined form.
 */
void ang_sort_aux(vptr u, vptr v, s32b p, s32b q)
{
	s32b z, a, b;

	/* Done sort */
	if (p >= q) return;

	/* Pivot */
	z = p;

	/* Begin */
	a = p;
	b = q;

	/* Partition */
	while (TRUE)
	{
		/* Slide i2 */
		while (!(*ang_sort_comp)(u, v, b, z)) b--;

		/* Slide i1 */
		while (!(*ang_sort_comp)(u, v, z, a)) a++;

		/* Done partition */
		if (a >= b) break;

		/* Swap */
		(*ang_sort_swap)(u, v, a, b);

		/* Advance */
		a++, b--;
	}

	/* Recurse left side */
	ang_sort_aux(u, v, p, b);

	/* Recurse right side */
	ang_sort_aux(u, v, b + 1, q);
}


/*
 * Angband sorting algorithm -- quick sort in place
 *
 * Note that the details of the data we are sorting is hidden,
 * and we rely on the "ang_sort_comp()" and "ang_sort_swap()"
 * function hooks to interact with the data, which is given as
 * two pointers, and which may have any user-defined form.
 */
void ang_sort(vptr u, vptr v, s32b n)
{
	/* Sort the array */
	ang_sort_aux(u, v, 0, n - 1);
}



/*** Targetting Code ***/


/*
 * Determine is a monster makes a reasonable target
 *
 * The concept of "targetting" was stolen from "Morgul" (?)
 *
 * The player can target any location, or any "target-able" monster.
 *
 * Currently, a monster is "target_able" if it is visible, and if
 * the player can hit it with a projection, and the player is not
 * hallucinating.  This allows use of "use closest target" macros.
 *
 * Future versions may restrict the ability to target "trappers"
 * and "mimics", but the semantics is a little bit weird.
 */
bool target_able(s32b m_idx)
{
	monster_type *m_ptr = get_monster(m_idx);

	/* Monster must be alive */
	if (!m_ptr || !m_ptr->r_idx) return (FALSE);

	/* Monster must be visible */
	if (!m_ptr->ml) return (FALSE);

	/* Monster must be projectable */
	if (!projectable(p_ptr->py, p_ptr->px, m_ptr->fy, m_ptr->fx)) return (FALSE);

	/* Hack -- no targeting hallucinations */
	if (intrinsic(HALLUCINATE)) return (FALSE);

	/* Dont target pets */
	if (is_friend(m_ptr) > 0) return (FALSE);

	/* Honor flag */
	if (has_flag(&r_info[m_ptr->r_idx], FLAG_NO_TARGET)) return (FALSE);

	/* XXX XXX XXX Hack -- Never target trappers */
	/* if (CLEAR_ATTR && (CLEAR_CHAR)) return (FALSE); */

	/* Assume okay */
	return (TRUE);
}




/*
 * Update (if necessary) and verify (if possible) the target.
 *
 * We return TRUE if the target is "okay" and FALSE otherwise.
 */
bool target_okay(void)
{
	/* Accept stationary targets */
	if (target_who < 0) return (TRUE);

	/* Check moving targets */
	if (target_who > 0)
	{
		/* Accept reasonable targets */
		if (target_able(target_who))
		{
			monster_type *m_ptr = get_monster(target_who);

			/* Acquire monster location */
			target_row = m_ptr->fy;
			target_col = m_ptr->fx;

			/* Good target */
			return (TRUE);
		}
	}

	/* Assume no target */
	return (FALSE);
}



/*
 * Sorting hook -- comp function -- by "distance to player"
 *
 * We use "u" and "v" to point to arrays of "x" and "y" positions,
 * and sort the arrays by double-distance to the player.
 */
static bool ang_sort_comp_distance(vptr u, vptr v, s32b a, s32b b)
{
	byte *x = (byte*)(u);
	byte *y = (byte*)(v);

	s32b da, db, kx, ky;

	/* Absolute distance components */
	kx = x[a];
	kx -= p_ptr->px;
	kx = ABS(kx);
	ky = y[a];
	ky -= p_ptr->py;
	ky = ABS(ky);

	/* Approximate Double Distance to the first point */
	da = ((kx > ky) ? (kx + kx + ky) : (ky + ky + kx));

	/* Absolute distance components */
	kx = x[b];
	kx -= p_ptr->px;
	kx = ABS(kx);
	ky = y[b];
	ky -= p_ptr->py;
	ky = ABS(ky);

	/* Approximate Double Distance to the first point */
	db = ((kx > ky) ? (kx + kx + ky) : (ky + ky + kx));

	/* Compare the distances */
	return (da <= db);
}


/*
 * Sorting hook -- swap function -- by "distance to player"
 *
 * We use "u" and "v" to point to arrays of "x" and "y" positions,
 * and sort the arrays by distance to the player.
 */
static void ang_sort_swap_distance(vptr u, vptr v, s32b a, s32b b)
{
	byte *x = (byte*)(u);
	byte *y = (byte*)(v);

	byte temp;

	/* Swap "x" */
	temp = x[a];
	x[a] = x[b];
	x[b] = temp;

	/* Swap "y" */
	temp = y[a];
	y[a] = y[b];
	y[b] = temp;
}



/*
 * Hack -- help "select" a location (see below)
 */
static s16b target_pick(s32b y1, s32b x1, s32b dy, s32b dx)
{
	s32b i, v;

	s32b x2, y2, x3, y3, x4, y4;

	s32b b_i = -1, b_v = 9999;


	/* Scan the locations */
	for (i = 0; i < temp_n; i++)
	{
		/* Point 2 */
		x2 = temp_x[i];
		y2 = temp_y[i];

		/* Directed distance */
		x3 = (x2 - x1);
		y3 = (y2 - y1);

		/* Verify quadrant */
		if (dx && (x3 * dx <= 0)) continue;
		if (dy && (y3 * dy <= 0)) continue;

		/* Absolute distance */
		x4 = ABS(x3);
		y4 = ABS(y3);

		/* Verify quadrant */
		if (dy && !dx && (x4 > y4)) continue;
		if (dx && !dy && (y4 > x4)) continue;

		/* Approximate Double Distance */
		v = ((x4 > y4) ? (x4 + x4 + y4) : (y4 + y4 + x4));

		/* XXX XXX XXX Penalize location */

		/* Track best */
		if ((b_i >= 0) && (v >= b_v)) continue;

		/* Track best */
		b_i = i;
		b_v = v;
	}

	/* Result */
	return (b_i);
}


/*
 * Hack -- determine if a given location is "interesting"
 */
static bool target_set_accept(s32b y, s32b x)
{
	cave_type *c_ptr;

	/* Player grid is always interesting */
	if ((y == p_ptr->py) && (x == p_ptr->px)) return (TRUE);


	/* Handle hallucination */
	if (intrinsic(HALLUCINATE)) return (FALSE);


	/* Examine the grid */
	c_ptr = &cave[y][x];

	/* Visible monsters */
	if (c_ptr->m_idx && c_ptr->m_idx < max_r_idx)
	{

		monster_type *m_ptr = get_monster(c_ptr->m_idx);
		/* Visible monsters */
		if (m_ptr->ml) return (TRUE);
	}

	/* Scan all objects in the grid */
	for_inventory_slot(&c_ptr->inventory, o_ptr);
	{
		/* Memorized object */
		if (o_ptr->marked) return (TRUE);
	}
	end_inventory_slot();

	/* Interesting memorized features */
	if (c_ptr->info & (CAVE_MARK))
	{
		/* Traps are interesting */
		if (c_ptr->info & (CAVE_TRDT)) return (TRUE);

		/* Hack -- Doors are boring */
		if (has_flag(&f_info[c_ptr->feat], FLAG_DOOR)) return (FALSE);

		/* Accept 'naturally' interesting features */
		if (has_flag(&f_info[c_ptr->feat], FLAG_NOTICE)) return (TRUE);
	}

	/* Nope */
	return (FALSE);
}


/*
 * Prepare the "temp" array for "target_set"
 *
 * Return the number of target_able monsters in the set.
 */
static void target_set_prepare(s32b mode)
{
	s32b y, x;

	/* Reset "temp" array */
	temp_n = 0;

	/* Scan the current panel */
	for (y = panel_row_min; y <= panel_row_max; y++)
	{
		for (x = panel_col_min; x <= panel_col_max; x++)
		{
			cave_type *c_ptr = &cave[y][x];

			/* Require line of sight, unless "look" is "expanded" */
			if (!expand_look && !player_has_los_bold(y, x)) continue;

			/* Require "interesting" contents */
			if (!target_set_accept(y, x)) continue;

			/* Require target_able monsters for "TARGET_KILL" */
			if ((mode & (TARGET_KILL)) && !target_able(c_ptr->m_idx)) continue;

			/* Save the location */
			temp_x[temp_n] = x;
			temp_y[temp_n] = y;
			temp_n++;
		}
	}

	/* Set the sort hooks */
	ang_sort_comp = ang_sort_comp_distance;
	ang_sort_swap = ang_sort_swap_distance;

	/* Sort the positions */
	ang_sort(temp_x, temp_y, temp_n);
}


bool target_object(s32b y, s32b x, s32b mode, cptr info, bool *boring,
                   object_type *o_ptr, char *out_val, cptr *s1, cptr *s2, cptr *s3,
                   s32b *query)
{
	char o_name[80];

	/* Not boring */
	*boring = FALSE;

	/* Obtain an object description */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Describe the object */
	sprintf(out_val, "%s%s%s%s [%s]", *s1, *s2, *s3, o_name, info);
	prt(out_val, 0, 0);
	move_cursor_relative(y, x);
	*query = inkey();

	/* Always stop at "normal" keys */
	if ((*query != '\r') && (*query != '\n') && (*query != ' ')) return (TRUE);

	/* Sometimes stop at "space" key */
	if ((*query == ' ') && !(mode & (TARGET_LOOK))) return (TRUE);

	/* Change the intro */
	*s1 = "It is ";

	/* Plurals */
	if (o_ptr->number != 1) *s1 = "They are ";

	/* Preposition */
	*s2 = "on ";
	return (FALSE);
}

/*
 * Examine a grid, return a keypress.
 *
 * The "mode" argument contains the "TARGET_LOOK" bit flag, which
 * indicates that the "space" key should scan through the contents
 * of the grid, instead of simply returning immediately.  This lets
 * the "look" command get complete information, without making the
 * "target" command annoying.
 *
 * The "info" argument contains the "commands" which should be shown
 * inside the "[xxx]" text.  This string must never be empty, or grids
 * containing monsters will be displayed with an extra comma.
 *
 * Note that if a monster is in the grid, we update both the monster
 * recall info and the health bar info to track that monster.
 *
 * Eventually, we may allow multiple objects per grid, or objects
 * and terrain features in the same grid. XXX XXX XXX
 *
 * This function must handle blindness/hallucination.
 */
static s32b target_set_aux(s32b y, s32b x, s32b mode, cptr info)
{
	cave_type *c_ptr = &cave[y][x];

	s16b this_o_idx = 0;

	cptr s1, s2, s3;

	bool boring;

	s32b feat;

	s32b query;

	char out_val[160];

	process_hooks(HOOK_TARGET, "(d,d,d,s)", y, x, mode, info);

	/* Repeat forever */
	while (1)
	{
		/* Paranoia */
		query = ' ';

		/* Assume boring */
		boring = TRUE;

		/* Default */
		s1 = "You see ";
		s2 = "";
		s3 = "";

		/* Hack -- under the player */
		if ((y == p_ptr->py) && (x == p_ptr->px))
		{
			/* Description */
			s1 = "You are ";

			/* Preposition */
			s2 = "on ";
		}


		/* Hack -- hallucination */
		if (intrinsic(HALLUCINATE))
		{
			cptr name = "something strange";

			/* Display a message */
			sprintf(out_val, "%s%s%s%s [%s]", s1, s2, s3, name, info);
			prt(out_val, 0, 0);
			move_cursor_relative(y, x);
			query = inkey();

			/* Stop on everything but "return" */
			if ((query != '\r') && (query != '\n')) break;

			/* Repeat forever */
			continue;
		}


		/* Actual monsters */
		if (c_ptr->m_idx)
		{
			monster_type *m_ptr = get_monster(c_ptr->m_idx);

			/* Mimics special treatment -- looks like an object */
			if (has_flag(m_ptr, FLAG_MIMIC) && (m_ptr->csleep))
			{
				object_type *o_ptr;

				/* Acquire object */
				o_ptr = flag_get_obj(get_inven(m_ptr, INVEN_INVEN), 1);

				if (o_ptr->marked)
				{
					if (target_object(y, x, mode, info, &boring, o_ptr, out_val, &s1, &s2, &s3, &query)) break;
				}
			}
			else
			{
				/* Visible */
				if (m_ptr->ml)
				{
					bool recall = FALSE;

					char m_name[80];

					/* Not boring */
					boring = FALSE;

					/* Get the monster name ("a kobold") */
					monster_desc(m_name, m_ptr, 0x08);

					/* Hack -- track this monster race */
					monster_race_track(m_ptr->r_idx, m_ptr->ego);

					/* Hack -- health bar for this monster */
					health_track(c_ptr->m_idx);

					/* Hack -- handle stuff */
					handle_stuff();

					/* Interact */
					while (1)
					{
						/* Recall */
						if (recall)
						{
							/* Save */
							character_icky++;
							Term_save();

							/* Recall on screen */
							screen_roff(m_ptr->r_idx, m_ptr->ego, 0);

							/* Hack -- Complete the prompt (again) */
							Term_addstr( -1, TERM_WHITE, format("  [r,%s]", info));

							/* Command */
							query = inkey();

							/* Restore */
							Term_load();
							character_icky--;
						}

						/* Normal */
						else
						{
							cptr mstat = flag_get_string(&faction_names, m_ptr->faction);;

							if (m_ptr->mflag & MFLAG_PARTIAL) mstat = " (partial) ";

							/* Describe, and prompt for recall */
                                                        if (!wizard)
								sprintf(out_val, "%s%s%s%s (level %d, %s%s)%s%s[r,%s]",
									s1, s2, s3, m_name,
									m_ptr->level, look_mon_desc(c_ptr->m_idx),
									(m_ptr->mflag & MFLAG_QUEST) ? ", quest" : "",
									(m_ptr->smart & SM_CLONED ? " (clone)" : ""),
									(mstat), info);
                                                        else
								sprintf(out_val, "%s%s%s%s (LOS(%ld/%d) lvl %d, m_idx %d, %s%s)%s%s[r,%s]",
									s1, s2, s3, m_name,
									c_ptr->los_turn, c_ptr->los_dist,
									m_ptr->level, c_ptr->m_idx, look_mon_desc(c_ptr->m_idx),
									(m_ptr->mflag & MFLAG_QUEST) ? ", quest" : "",
									(m_ptr->smart & SM_CLONED ? " (clone)" : ""),
									(mstat), info);

							prt(out_val, 0, 0);

							/* Place cursor */
							move_cursor_relative(y, x);

							/* Command */
							query = inkey();
						}

						/* Normal commands */
						if (query != 'r') break;

						/* Toggle recall */
						recall = !recall;
					}

					/* Always stop at "normal" keys */
					if ((query != '\r') && (query != '\n') && (query != ' ')) break;

					/* Sometimes stop at "space" key */
					if ((query == ' ') && !(mode & (TARGET_LOOK))) break;

					/* Change the intro */
					s1 = "It is ";

					/* Hack -- take account of gender */
					if (has_flag(m_ptr, FLAG_FEMALE)) s1 = "She is ";
					else if (has_flag(m_ptr, FLAG_MALE)) s1 = "He is ";

					/* Use a preposition */
					s2 = "carrying ";

					/* Scan all objects being carried */
					for_inventory(m_ptr, o_ptr, 0, INVEN_TOTAL);
					{
						char o_name[80];

						/* Obtain an object description */
						object_desc(o_name, o_ptr, TRUE, 3);

						/* Describe the object */
						sprintf(out_val, "%s%s%s%s [%s]", s1, s2, s3, o_name, info);
						prt(out_val, 0, 0);
						move_cursor_relative(y, x);
						query = inkey();

						/* Always stop at "normal" keys */
						if ((query != '\r') && (query != '\n') && (query != ' ')) break;

						/* Sometimes stop at "space" key */
						if ((query == ' ') && !(mode & (TARGET_LOOK))) break;

						/* Change the intro */
						s2 = "also carrying ";
					}
					end_inventory();

					/* Double break */
					if (this_o_idx) break;

					/* Use a preposition */
					s2 = "on ";
				}
			}
		}

		/* Scan all objects in the grid */
		this_o_idx = 0;
		for_inventory_slot(&c_ptr->inventory, o_ptr);
		{
			/* Describe it */
			if (o_ptr->marked)
			{
				if (target_object(y, x, mode, info, &boring, o_ptr, out_val, &s1, &s2, &s3, &query)) { this_o_idx = 1; break; }
			}
		}
		end_inventory_slot();

		/* Double break */
		if (this_o_idx) break;

		/* Actual traps */
		if ((c_ptr->info & (CAVE_TRDT)) && flag_used(&c_ptr->activations))
		{
			cptr        name = "a trap", s4;
			flags_type* trap;
			s32b        ret;
			s32b        idx = 0, junk;
			s16b        level = 0;

			call_lua("traps.get_trap", "(d,d,d,d)", "Fd",
				 -1, -1, y, x, &trap, &junk);

			idx   = flag_get(trap, FLAG_TRAP_IDX);
			level = flag_get(trap, FLAG_LEVEL);

			/* Name trap */
			call_lua("traps.is_ident", "(d)", "d", idx, &ret);
			if (ret)
			{
				cptr tmp;
				call_lua("traps.ident_name", "(d,F)", "s",
					 idx, trap, &tmp);
				s4 = format("(%s)", tmp);

				if (wizard)
					s4 = format("%s (level %d)", s4,
						    level);
			}
			else
			{
				s4 = "an unknown trap";
			}

			if (get_num_location_traps(y, x, TRUE, -1, FALSE, FALSE) > 1)
				s4 = "multiple traps";

			/* Diidxay a message */
			sprintf(out_val, "%s%s%s%s [%s]", s1, s2, s3,
				name, s4);
			prt(out_val, 0, 0);
			move_cursor_relative(y, x);
			query = inkey();

			/* Stop on everything but "return" */
			if ((query != '\r') && (query != '\n')) break;

			/* Repeat forever */
			continue;
		}

		/* Feature (apply "mimic") */
		if (c_ptr->mimic)
		{
			feat = c_ptr->mimic;
		}
		else
		{
			feat = f_info[c_ptr->feat].mimic;
		}

		/* Require knowledge about grid, or ability to see grid */
		if (!(c_ptr->info & (CAVE_MARK)) && !player_can_see_bold(y, x))
		{
			/* Forget feature */
			feat = 0;
		}

		/* Terrain feature if needed */
//DGDGDGDG needed?		if (boring || (feat >= FEAT_GLYPH))
		{
			cptr name;

			if (cave_feat_is(c_ptr, FLAG_CONTAINS_BUILDING))
				/* Hack -- special handling for building doors */
				name = st_info[get_flag(c_ptr, FLAG_CONTAINS_BUILDING)].name;
			else if (has_flag(c_ptr, FLAG_TERRAIN_NAME))
				name = flag_get_string(&c_ptr->flags, FLAG_TERRAIN_NAME);
			else
				name = f_info[feat].name;

			/* Hack -- handle unknown grids */
			if (feat == 0) name = "unknown grid";

			/* Pick a prefix */
#if 0 //DGDGDGDG
			if (*s2 &&
			                (((feat >= FEAT_MINOR_GLYPH) &&
			                  (feat <= FEAT_PATTERN_XTRA2)) ||
			                 (feat == FEAT_DIRT) ||
			                 (feat == FEAT_GRASS) ||
			                 (feat == FEAT_FLOWER))) s2 = "on ";
			else if (*s2 && (feat == FEAT_SMALL_TREES)) s2 = "by ";
			else if (*s2 && (feat >= FEAT_DOOR_HEAD)) s2 = "in ";
#endif
			/* Pick proper indefinite article */
			s3 = (is_a_vowel(name[0])) ? "an " : "a ";

			/* Hack -- special introduction for store & building doors */
			if (feat == FEAT_SHOP)
			{
				s3 = "the entrance to the ";
			}
			if (cave_feat_is(c_ptr, FLAG_LEVEL_CHANGER) &&
				has_flag(c_ptr, FLAG_LEVEL_CHANGER))
			{
				s3 = "";
				name = d_info[get_flag(c_ptr, FLAG_LEVEL_CHANGER)].text;
			}

			if (p_ptr->wild_mode)
			{
				s3 = "";
				name = format("%s(%s)",
				              wf_info[wild_map[y][x].feat].name,
				              wf_info[wild_map[y][x].feat].text);
			}

			/* Display a message */
			if (!wizard)
			{
				sprintf(out_val, "%s%s%s%s [%s]", s1, s2, s3, name, info);
			}
			else
			{
				sprintf(out_val, "%s%s%s%s [%s] (LOS(%ld/%d) %d:%d:%x:%d:%d)",
					s1, s2, s3, name, info,
					c_ptr->los_turn, c_ptr->los_dist,
				        c_ptr->feat, c_ptr->mimic, c_ptr->info, (c_ptr->info & CAVE_VIEW) != 0, (c_ptr->info & CAVE_SEEN) != 0);
			}
			prt(out_val, 0, 0);
			move_cursor_relative(y, x);
			query = inkey();

			/* Always stop at "normal" keys */
			if ((query != '\r') && (query != '\n') && (query != ' ')) break;
		}

		/* Stop on everything but "return" */
		if ((query != '\r') && (query != '\n')) break;
	}

	/* Keep going */
	return (query);
}




/*
 * Handle "target" and "look".
 *
 * Note that this code can be called from "get_aim_dir()".
 *
 * All locations must be on the current panel.  Consider the use of
 * "panel_bounds()" to allow "off-panel" targets, perhaps by using
 * some form of "scrolling" the map around the cursor.  XXX XXX XXX
 * That is, consider the possibility of "auto-scrolling" the screen
 * while the cursor moves around.  This may require changes in the
 * "update_mon()" code to allow "visibility" even if off panel, and
 * may require dynamic recalculation of the "temp" grid set.
 *
 * Hack -- targetting/observing an "outer border grid" may induce
 * problems, so this is not currently allowed.
 *
 * The player can use the direction keys to move among "interesting"
 * grids in a heuristic manner, or the "space", "+", and "-" keys to
 * move through the "interesting" grids in a sequential manner, or
 * can enter "location" mode, and use the direction keys to move one
 * grid at a time in any direction.  The "t" (set target) command will
 * only target a monster (as opposed to a location) if the monster is
 * target_able and the "interesting" mode is being used.
 *
 * The current grid is described using the "look" method above, and
 * a new command may be entered at any time, but note that if the
 * "TARGET_LOOK" bit flag is set (or if we are in "location" mode,
 * where "space" has no obvious meaning) then "space" will scan
 * through the description of the current grid until done, instead
 * of immediately jumping to the next "interesting" grid.  This
 * allows the "target" command to retain its old semantics.
 *
 * The "*", "+", and "-" keys may always be used to jump immediately
 * to the next (or previous) interesting grid, in the proper mode.
 *
 * The "return" key may always be used to scan through a complete
 * grid description (forever).
 *
 * This command will cancel any old target, even if used from
 * inside the "look" command.
 */
bool target_set(s32b mode)
{
	s32b i, d, m;
	s32b y = p_ptr->py;
	s32b x = p_ptr->px;

	bool done = FALSE;

	bool flag = TRUE;

	char query;

	char info[80];

	cave_type *c_ptr;

	s32b screen_wid, screen_hgt;
	s32b panel_wid, panel_hgt;

	/* Get size */
	get_screen_size(&screen_wid, &screen_hgt);

	/* Calculate the amount of panel movement */
	panel_hgt = screen_hgt / 2;
	panel_wid = screen_wid / 2;

	/* Cancel target */
	target_who = 0;


	/* Cancel tracking */
	/* health_track(0); */


	/* Prepare the "temp" array */
	target_set_prepare(mode);

	/* Start near the player */
	m = 0;

	/* Interact */
	while (!done)
	{
		/* Interesting grids */
		if (flag && temp_n)
		{
			y = temp_y[m];
			x = temp_x[m];

			/* Access */
			c_ptr = &cave[y][x];

			/* Allow target */
			if (c_ptr->m_idx && target_able(c_ptr->m_idx))
			{
				strcpy(info, "q,t,p,o,+,-,'dir'");
			}

			/* Dis-allow target */
			else
			{
				strcpy(info, "q,p,o,+,-,'dir'");
			}

			/* Describe and Prompt */
			query = target_set_aux(y, x, mode, info);

			/* Cancel tracking */
			/* health_track(0); */

			/* Assume no "direction" */
			d = 0;

			/* Analyze */
			switch (query)
			{
			case ESCAPE:
			case 'q':
				{
					done = TRUE;
					break;
				}

			case 't':
			case '.':
			case '5':
			case '0':
				{
					if (target_able(c_ptr->m_idx))
					{
						health_track(c_ptr->m_idx);
						target_who = c_ptr->m_idx;
						target_row = y;
						target_col = x;
						done = TRUE;
					}
					else
					{
						bell();
					}
					break;
				}

			case ' ':
			case '*':
			case '+':
				{
					if (++m == temp_n)
					{
						m = 0;
						if (!expand_list) done = TRUE;
					}
					break;
				}

			case '-':
				{
					if (m-- == 0)
					{
						m = temp_n - 1;
						if (!expand_list) done = TRUE;
					}
					break;
				}

			case 'p':
				{
					/* Recenter the map around the player */
					verify_panel();

					/* Update stuff */
					p_ptr->update |= (PU_MONSTERS);

					/* Redraw map */
					flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

					/* Window stuff */
					flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);

					/* Handle stuff */
					handle_stuff();

					/* Recalculate interesting grids */
					target_set_prepare(mode);

					y = p_ptr->py;
					x = p_ptr->px;

					/* Fall through... */
				}

			case 'o':
				{
					flag = FALSE;
					break;
				}

			case 'm':
				{
					break;
				}

			default:
				{
					/* Extract the action (if any) */
					d = get_keymap_dir(query);

					if (!d) bell();
					break;
				}
			}

			/* Hack -- move around */
			if (d)
			{
				/* Find a new monster */
				i = target_pick(temp_y[m], temp_x[m], ddy[d], ddx[d]);

				/* Scroll to find interesting grid */
				if (i < 0)
				{
					s32b dy;
					s32b dx;

					dy = ddy[d];
					dx = ddx[d];

					/* Note panel change */
					if (change_panel(dy, dx))
					{
						s32b ty = temp_y[m];
						s32b tx = temp_x[m];

						/* Recalculate interesting grids */
						target_set_prepare(mode);

						/* Find a new monster */
						i = target_pick(ty, tx, dy, dx);

						/* Restore panel if needed */
						if (i < 0)
						{
							/* Restore panel */
							change_panel( -dy, -dx);

							/* Recalculate interesting grids */
							target_set_prepare(mode);
						}
					}
				}

				/* Use that grids */
				if (i >= 0) m = i;
			}
		}

		/* Arbitrary grids */
		else
		{
			/* Access */
			c_ptr = &cave[y][x];

			/* Default prompt */
			strcpy(info, "q,t,p,m,+,-,'dir'");

			/* Describe and Prompt (enable "TARGET_LOOK") */
			query = target_set_aux(y, x, mode | TARGET_LOOK, info);

			/* Cancel tracking */
			/* health_track(0); */

			/* Assume no direction */
			d = 0;

			/* Analyze the keypress */
			switch (query)
			{
			case ESCAPE:
			case 'q':
				{
					done = TRUE;
					break;
				}

			case 't':
			case '.':
			case '5':
			case '0':
				{
					target_who = -1;
					target_row = y;
					target_col = x;
					done = TRUE;
					break;
				}

			case ' ':
			case '*':
			case '+':
			case '-':
				{
					break;
				}

			case 'p':
				{
					y = p_ptr->py;
					x = p_ptr->px;
				}

			case 'o':
				{
					break;
				}

			case 'm':
				{
					flag = TRUE;
					break;
				}

			default:
				{
					/* Extract the action (if any) */
					d = get_keymap_dir(query);

					if (!d) bell();
					break;
				}
			}

			/* Handle "direction" */
			if (d)
			{
				s32b dy = ddy[d];
				s32b dx = ddx[d];

				/* Move */
				y += dy;
				x += dx;

				/* Do not move horizontally if unnecessary */
				if (((x < panel_col_min + panel_wid) && (dx > 0)) ||
				                ((x > panel_col_min + panel_wid) && (dx < 0)))
				{
					dx = 0;
				}

				/* Do not move vertically if unnecessary */
				if (((y < panel_row_min + panel_hgt) && (dy > 0)) ||
				                ((y > panel_row_min + panel_hgt) && (dy < 0)))
				{
					dy = 0;
				}
				/* Apply the motion */
				if ((y >= panel_row_min + screen_hgt) ||
				                (y < panel_row_min) ||
				                (x > panel_col_min + screen_wid) ||
				                (x < panel_col_min))
				{
					/* Change panel and recalculate interesting grids */
					if (change_panel(dy, dx)) target_set_prepare(mode);
				}

				/* Boundary checks */
				if (!wizard)
				{
					/* Hack -- Verify y */
					if (y <= 0) y = 1;
					else if (y >= cur_hgt - 1) y = cur_hgt - 2;

					/* Hack -- Verify x */
					if (x <= 0) x = 1;
					else if (x >= cur_wid - 1) x = cur_wid - 2;
				}
				else
				{
					/* Hack -- Verify y */
					if (y < 0) y = 0;
					else if (y > cur_hgt - 1) y = cur_hgt - 1;

					/* Hack -- Verify x */
					if (x < 0) x = 0;
					else if (x > cur_wid - 1) x = cur_wid - 1;
				}
			}
		}
	}

	/* Forget */
	temp_n = 0;

	/* Clear the top line */
	prt("", 0, 0);

	/* Recenter the map around the player */
	verify_panel();

	/* Update stuff */
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw map */
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);

	/* Handle stuff */
	handle_stuff();

	/* Failure to set target */
	if (!target_who) return (FALSE);

	/* Success */
	return (TRUE);
}



/*
 * Get an "aiming direction" from the user.
 *
 * The "dir" is loaded with 1,2,3,4,6,7,8,9 for "actual direction", and
 * "0" for "current target", and "-1" for "entry aborted".
 *
 * Note that "Force Target", if set, will pre-empt user interaction,
 * if there is a usable target already set.
 *
 * Note that confusion over-rides any (explicit?) user choice.
 */
bool get_aim_dir(s32b *dp)
{
	s32b dir;

	s32b command;

	cptr p;

#ifdef ALLOW_REPEAT /* TNB */

	if (repeat_pull(dp))
	{
		/* Confusion? */

		/* Verify */
		if (!(*dp == 5 && !target_okay()))
		{
			return (TRUE);
		}
	}

#endif /* ALLOW_REPEAT -- TNB */

	/* Initialize */
	(*dp) = 0;

	/* Global direction */
	dir = command_dir;

	/* Hack -- auto-target if requested */
	if (use_old_target && target_okay()) dir = 5;

	/* Ask until satisfied */
	while (!dir)
	{
		/* Choose a prompt */
		if (!target_okay())
		{
			p = "Direction ('*' to choose a target, Escape to cancel)? ";
		}
		else
		{
			p = "Direction ('5' for target, '*' to re-target, Escape to cancel)? ";
		}

		/* Get a command (or Cancel) */
		if (!get_com(p, &command)) break;

		/* Convert various keys to "standard" keys */
		switch (command)
		{
			/* Use current target */
		case 'T':
		case 't':
		case '.':
		case '5':
		case '0':
			{
				dir = 5;
				break;
			}

			/* Set new target */
		case '*':
			{
				if (target_set(TARGET_KILL)) dir = 5;
				break;
			}

		default:
			{
				/* Extract the action (if any) */
				dir = get_keymap_dir(command);

				break;
			}
		}

		/* Verify requested targets */
		if ((dir == 5) && !target_okay()) dir = 0;

		/* Error */
		if (!dir) bell();
	}

	/* No direction */
	if (!dir) return (FALSE);

	/* Save the direction */
	command_dir = dir;

	/* Check for confusion */
	if (intrinsic(CONFUSED))
	{
		/* XXX XXX XXX */
		/* Random direction */
		dir = ddd[rand_int(8)];
	}

	/* Notice confusion */
	if (command_dir != dir)
	{
		/* Warn the user */
		msg_print("You are confused.");
	}

	/* Save direction */
	(*dp) = dir;

#ifdef ALLOW_REPEAT /* TNB */

	repeat_push(dir);

#endif /* ALLOW_REPEAT -- TNB */

	/* A "valid" direction was entered */
	return (TRUE);
}



/*
 * Request a "movement" direction (1,2,3,4,6,7,8,9) from the user,
 * and place it into "command_dir", unless we already have one.
 *
 * This function should be used for all "repeatable" commands, such as
 * run, walk, open, close, bash, disarm, spike, tunnel, etc, as well
 * as all commands which must reference a grid adjacent to the player,
 * and which may not reference the grid under the player.  Note that,
 * for example, it is no longer possible to "disarm" or "open" chests
 * in the same grid as the player.
 *
 * Direction "5" is illegal and will (cleanly) abort the command.
 *
 * This function tracks and uses the "global direction", and uses
 * that as the "desired direction", to which "confusion" is applied.
 */
bool get_rep_dir(s32b *dp)
{
	s32b dir;

#ifdef ALLOW_REPEAT /* TNB */

	if (repeat_pull(dp))
	{
		return (TRUE);
	}

#endif /* ALLOW_REPEAT -- TNB */

	/* Initialize */
	(*dp) = 0;

	/* Global direction */
	dir = command_dir;

	/* Get a direction */
	while (!dir)
	{
		s32b ch;

		/* Get a command (or Cancel) */
		if (!get_com("Direction (Escape to cancel)? ", &ch)) break;

		/* Look up the direction */
		dir = get_keymap_dir(ch);

		/* Oops */
		if (!dir) bell();
	}

	/* Prevent weirdness */
	if (dir == 5) dir = 0;

	/* Aborted */
	if (!dir) return (FALSE);

	/* Save desired direction */
	command_dir = dir;

	/* Apply "confusion" */
	if (intrinsic(CONFUSED))
	{
		/* Standard confusion */
		if (rand_int(100) < 75)
		{
			/* Random direction */
			dir = ddd[rand_int(8)];
		}
	}

	/* Notice confusion */
	if (command_dir != dir)
	{
		/* Warn the user */
		msg_print("You are confused.");
	}

	/* Save direction */
	(*dp) = dir;

#ifdef ALLOW_REPEAT /* TNB */

	repeat_push(dir);

#endif /* ALLOW_REPEAT -- TNB */

	/* Success */
	return (TRUE);
}

/*
 * old -- from PsiAngband.
 */
bool tgt_pt(s32b *x, s32b *y)
{
	return tgt_pt_prompt("Select a point and press space.", x, y);
}

/*
 * Like the old tgt_pt(), but the caller gets to choose a prompt besides
 * "Select a point and press space."
 */
bool tgt_pt_prompt(cptr prompt, s32b *x, s32b *y)
{
	s32b ch = 0;
	s32b  d, cu, cv;
	s32b  screen_wid, screen_hgt;
	bool success = FALSE;

	*x = p_ptr->px;
	*y = p_ptr->py;

	/* Get size */
	get_screen_size(&screen_wid, &screen_hgt);

	cu = Term->scr->cu;
	cv = Term->scr->cv;
	Term->scr->cu = 0;
	Term->scr->cv = 1;
	msg_print(prompt);

	while ((ch != 27) && (ch != ' '))
	{
		move_cursor_relative(*y, *x);
		ch = inkey();
		switch (ch)
		{
		case 27:
			break;
		case ' ':
			success = TRUE;
			break;
		default:
			/* Look up the direction */
			d = get_keymap_dir(ch);

			if (!d) break;

			*x += ddx[d];
			*y += ddy[d];

			/* Hack -- Verify x */
			if ((*x >= cur_wid - 1) || (*x >= panel_col_min + screen_wid)) (*x)--;
			else if ((*x <= 0) || (*x <= panel_col_min)) (*x)++;

			/* Hack -- Verify y */
			if ((*y >= cur_hgt - 1) || (*y >= panel_row_min + screen_hgt)) (*y)--;
			else if ((*y <= 0) || (*y <= panel_row_min)) (*y)++;

			break;
		}
	}

	Term->scr->cu = cu;
	Term->scr->cv = cv;
	Term_fresh();
	return success;
}

bool get_hack_dir(s32b *dp)
{
	s32b  dir;
	cptr p;
	s32b command;


	/* Initialize */
	(*dp) = 0;

	/* Global direction */
	dir = 0;

	/* (No auto-targetting */

	/* Ask until satisfied */
	while (!dir)
	{
		/* Choose a prompt */
		if (!target_okay())
		{
			p = "Direction ('*' to choose a target, Escape to cancel)? ";
		}
		else
		{
			p = "Direction ('5' for target, '*' to re-target, Escape to cancel)? ";
		}

		/* Get a command (or Cancel) */
		if (!get_com(p, &command)) break;

		/* Convert various keys to "standard" keys */
		switch (command)
		{
			/* Use current target */
		case 'T':
		case 't':
		case '.':
		case '5':
		case '0':
			{
				dir = 5;
				break;
			}

			/* Set new target */
		case '*':
			{
				if (target_set(TARGET_KILL)) dir = 5;
				break;
			}

		default:
			{
				/* Look up the direction */
				dir = get_keymap_dir(command);

				break;
			}
		}

		/* Verify requested targets */
		if ((dir == 5) && !target_okay()) dir = 0;

		/* Error */
		if (!dir) bell();
	}

	/* No direction */
	if (!dir) return (FALSE);

	/* Save the direction */
	command_dir = dir;

	/* Check for confusion */
	if (intrinsic(CONFUSED))
	{
		/* XXX XXX XXX */
		/* Random direction */
		dir = ddd[rand_int(8)];
	}

	/* Notice confusion */
	if (command_dir != dir)
	{
		/* Warn the user */
		msg_print("You are confused.");
	}

	/* Save direction */
	(*dp) = dir;

	/* A "valid" direction was entered */
	return (TRUE);
}

bool test_object_wish(char *name, object_type *o_ptr, char *what)
{
	s32b i, j, jb, save_aware;
	char buf[200];

	/* try all objects, this *IS* a very ugly and slow method :( */
	for (i = 0; i < max_k_idx; i++)
	{
		object_kind *k_ptr = &k_info[i];

		object_wipe(o_ptr);

		if (!k_ptr->name) continue;
		if (has_flag(k_ptr, FLAG_NORM_ART)) continue;
		if (has_flag(k_ptr, FLAG_INSTA_ART)) continue;
		if (has_flag(k_ptr, FLAG_GOLD_VALUE)) continue;

		object_prep(o_ptr, i);
		apply_magic(o_ptr, dun_level, FALSE, FALSE, FALSE);
		/* Hack : aware status must be restored after describing the item name */
		save_aware = k_ptr->aware;
		object_aware(o_ptr);
		object_known(o_ptr);
		object_desc(buf, o_ptr, FALSE, 0);
		strlower(buf);
		k_ptr->aware = save_aware;

		if (strstr(name, buf))
		{
			/* try all ego */
			for (j = max_e_idx - 1; j >= 0; j--)
			{
				ego_item_type *e_ptr = &e_info[j];
				bool ok = FALSE;

				if (j && !e_ptr->name) continue;

				/* Must have the correct fields */
				if (j)
				{
					s32b z;

					for (z = 0; z < 6; z++)
					{
						if (e_ptr->tval[z] == k_ptr->tval)
						{
							if ((e_ptr->min_sval[z] <= k_ptr->sval) &&
							                (e_ptr->max_sval[z] >= k_ptr->sval)) ok = TRUE;
						}
						if (ok) break;
					}
					if (!ok)
					{
						continue;
					}
				}

				/* try all ego */
				for (jb = max_e_idx - 1; jb >= 0; jb--)
				{
					ego_item_type *eb_ptr = &e_info[jb];
					bool ok = FALSE;

					if (jb && !eb_ptr->name) continue;

					if (j && jb && (e_ptr->before == eb_ptr->before)) continue;

					/* Must have the correct fields */
					if (jb)
					{
						s32b z;

						for (z = 0; z < 6; z++)
						{
							if (eb_ptr->tval[z] == k_ptr->tval)
							{
								if ((eb_ptr->min_sval[z] <= k_ptr->sval) &&
								                (eb_ptr->max_sval[z] >= k_ptr->sval)) ok = TRUE;
							}
							if (ok) break;
						}
						if (!ok)
						{
							continue;
						}
					}

					object_prep(o_ptr, i);
					o_ptr->artifact_id = 0;
					o_ptr->ego_id[0] = j;
					o_ptr->ego_id[1] = jb;
					apply_magic(o_ptr, dun_level, FALSE, FALSE, FALSE);
					object_aware(o_ptr);
					object_known(o_ptr);
					object_desc(buf, o_ptr, FALSE, 0);
					strlower(buf);

					if (!stricmp(buf, name))
					{
						/* Don't search any more */
						return TRUE;
					}
					else
					{
						/* Restore again the aware status */
						k_ptr->aware = save_aware;
					}
				}
			}
		}
	}
	return FALSE;
}

void clean_wish_name(char *buf, char *name)
{
	char *p;
	s32b i, j;

	/* Lowercase the wish */
	strlower(buf);

	/* Nuke uneccesary spaces */
	p = buf;
	while (*p == ' ') p++;
	i = 0;
	j = 0;
	while (p[i])
	{
		if ((p[i] == ' ') && (p[i + 1] == ' '))
		{
			i++;
			continue;
		}
		name[j++] = p[i++];
	}
	name[j++] = '\0';
	if (j)
	{
		j--;
		while (j && (name[j] == ' '))
		{
			name[j] = '\0';
			j--;
		}
	}
}

/*
 * Allow the player to make a wish
 */
void make_wish(void)
{
	s32b ret;

        call_lua("wish.make", "()", "d", &ret);
}

/*
 * Quick mimic name to index function
 */
s32b resolve_mimic_name(cptr name)
{
	s32b idx;

	call_lua("resolve_mimic_name", "(s)", "d", name, &idx);
	return idx;
}
