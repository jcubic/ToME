/* File: skills.c */

/* Purpose: player skills */

/*
 * Copyright (c) 2001 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"


/*
 * Advance the skill point of the skill specified by i and
 * modify related skills
 */
void increase_skill(s32b i, s16b *invest)
{
	s32b max_skill_overage, max_level;
	cptr error = NULL;

	/* No skill points to be allocated */
	if (!p_ptr->skill_points) return;

	/* The skill cannot be increased */
	if (!s_info[i].mod) return;

	/* The skill is already maxed */
	call_lua("get_module_info", "(s)", "d", "max_skill_level", &max_level);
	if (s_info[i].value >= max_level * SKILL_STEP) return;

	/* Cannot allocate more than player level + max_skill_overage levels */
	call_lua("get_module_info", "(s)", "d", "max_skill_overage", &max_skill_overage);
	if (((s_info[i].value + s_info[i].mod) / SKILL_STEP) >= (p_ptr->lev + max_skill_overage + 1))
	{
		s32b hgt, wid;

		Term_get_size(&wid, &hgt);
		msg_box(format("Cannot raise a skill value to or above %d until you get more experienced.", p_ptr->lev + max_skill_overage), (s32b)(hgt / 2), (s32b)(wid / 2));
		return;
	}

	/* More complex checking */
	call_lua("get_module_info", "(s,d)", "s", "allow_skill_raise", i, &error);
	if (error)
	{
		s32b hgt, wid;

		Term_get_size(&wid, &hgt);
		msg_box(error, (s32b)(hgt / 2), (s32b)(wid / 2));
		return;
	}

	/* Spend an unallocated skill point */
	p_ptr->skill_points--;

	/* Increase the skill */
	s_info[i].value += s_info[i].mod;
	invest[i]++;
}


/*
 * Descrease the skill point of the skill specified by i and
 * modify related skills
 */
void decrease_skill(s32b i, s16b *invest)
{
	/* Cannot decrease more */
	if (!invest[i]) return;

	/* The skill cannot be decreased */
	if (!s_info[i].mod) return;

	/* The skill already has minimal value */
	if (!s_info[i].value) return;

	/* Free a skill point */
	p_ptr->skill_points++;

	/* Decrease the skill */
	s_info[i].value -= s_info[i].mod;
	invest[i]--;
}


/*
 * Given the name of a skill, returns skill index or -1 if no
 * such skill is found
 */
s16b find_skill(cptr name)
{
	u16b i;

	/* Scan skill list */
	for (i = 1; i < max_s_idx; i++)
	{
		/* The name matches */
		if (streq(s_info[i].name, name)) return (i);
	}

	/* No match found */
	return ( -1);
}
s16b find_skill_i(cptr name)
{
	u16b i;

	/* Scan skill list */
	for (i = 1; i < max_s_idx; i++)
	{
		/* The name matches */
		if (0 == stricmp(s_info[i].name, name)) return (i);
	}

	/* No match found */
	return ( -1);
}


s32b get_skill_raw(s32b skill)
{
	return (skill > -1) ? s_info[skill].value + flag_get(&p_ptr->skill_bonuses, skill) : 0;
}

/*
 *
 */
s32b get_skill(s32b skill)
{
	return (get_skill_raw(skill) / SKILL_STEP);
}


/*
 * Return "scale" (a misnomer -- this is max value) * (current skill value)
 * / (max skill value)
 */
s32b get_skill_scale(s32b skill, s32b scale)
{
	s32b temp;

	/*
	* SKILL_STEP shouldn't matter here because the second parameter is
	* relatively small (the largest one being somewhere around 200),
	* AND because we could have used much simpler 0--50 if the ability
	* progression were only possible at step boundaries.
	*
	* Because I'm not at all certain about my interpretation of the mysterious
	* formula given above, I verified this works the same by using a tiny
	* scheme program... -- pelpel
	*/
	temp = scale * get_skill_raw(skill);
	return (temp / (s32b)SKILL_MAX);
}


/*
 *
 */
s32b get_idx(s32b i)
{
	s32b j;

	for (j = 1; j < max_s_idx; j++)
	{
		if (s_info[j].order == i)
			return (j);
	}
	return (0);
}

static bool is_known(s32b s_idx)
{
	s32b i;

	if (wizard) return TRUE;
	if (get_skill_raw(s_idx) || s_info[s_idx].mod) return TRUE;

	for (i = 0; i < max_s_idx; i++)
	{
		/* It is our child, if we don't know it we continue to search, if we know it it is enough*/
		if (s_info[i].father == s_idx)
		{
			if (is_known(i))
				return TRUE;
		}
	}

	/* Ok know none */
	return FALSE;
}

/*
 *
 */
void init_table_aux(s32b **table, s32b *idx, s32b father, s32b lev,
                    bool full)
{
	s32b j, i;

	for (j = 1; j < max_s_idx; j++)
	{
		i = get_idx(j);

		if (s_info[i].father != father) continue;
		if (s_info[i].hidden) continue;
		if (!is_known(i)) continue;

		table[*idx][0] = i;
		table[*idx][1] = lev;
		(*idx)++;
		if (s_info[i].dev || full) init_table_aux(table, idx, i, lev + 1, full);
	}
}


void init_table(s32b **table, s32b *max, bool full)
{
	*max = 0;
	init_table_aux(table, max, -1, 0, full);
}


bool has_child(s32b sel)
{
	s32b i;

	for (i = 1; i < max_s_idx; i++)
	{
		if ((s_info[i].father == sel) && (is_known(i)))
			return (TRUE);
	}
	return (FALSE);
}


/*
 * Dump the skill tree
 */
void dump_skills(PHYSFS_file *fff)
{
	s32b i, j, max = 0;
	s32b **table;
	char buf[80];

	C_MAKE(table, max_s_idx, s32b*);
	for (i = 0; i < max_s_idx; i++) C_MAKE(table[i], 2, s32b);

	init_table(table, &max, TRUE);

	fprintf(fff, "\nSkills (points left: %d)", p_ptr->skill_points);

	for (j = 0; j < max; j++)
	{
		s32b z;

		i = table[j][0];

		if (get_skill(i) == 0)
		{
			if (s_info[i].mod == 0) continue;
		}

		sprintf(buf, "\n");

		for (z = 0; z < table[j][1]; z++) strcat(buf, "         ");

		if (!has_child(i))
		{
			strcat(buf, format(" . %s", s_info[i].name));
		}
		else
		{
			strcat(buf, format(" - %s", s_info[i].name));
		}

		fprintf(fff, "%-50s%c%02ld.%03ld [%01d.%03d]", 	buf, (get_skill_raw(i) < 0) ? '-' : ' ',
			abs(get_skill_raw(i)) / SKILL_STEP, abs(get_skill_raw(i)) % SKILL_STEP,
		        s_info[i].mod / 1000, s_info[i].mod % 1000);
	}

	fprintf(fff, "\n");

	for (i = 0; i < max_s_idx; i++) C_FREE(table[i], 2, s32b);
	C_FREE(table, max_s_idx, s32b*);
}

#if 0 /* Not used anymore, see print_all, left in as a reminder how it worked */
/*
 * Draw the skill tree
 */
void print_skills(s32b **table, s32b max, s32b sel, s32b start)
{
	s32b i, j;
	s32b wid, hgt;
	cptr keys;

	Term_clear();
	Term_get_size(&wid, &hgt);

	c_prt(TERM_WHITE, format("%s Skills Screen", game_module), 0, 28);
	keys = format("#BEnter#W to develop a branch, #Bup#W/#Bdown#W to move, #Bright#W/#Bleft#W to modify, #B?#W for help");
	display_message(0, 1, strlen(keys), TERM_WHITE, keys);
	c_prt((p_ptr->skill_points) ? TERM_L_BLUE : TERM_L_RED,
	      format("Skill points left: %d", p_ptr->skill_points), 2, 0);
	print_desc_aux(s_info[table[sel][0]].desc, 3, 0);

	for (j = start; j < start + (hgt - 7); j++)
	{
		byte color = TERM_WHITE;
		char deb = ' ', end = ' ';

		if (j >= max) break;

		i = table[j][0];

		if (get_skill(i) == 0)
		{
			if (s_info[i].mod == 0) color = TERM_L_DARK;
			else color = TERM_ORANGE;
		}
		else if (get_skill_raw(i) == SKILL_MAX) color = TERM_L_BLUE;
		if (s_info[i].hidden) color = TERM_L_RED;
		if (j == sel)
		{
			color = TERM_L_GREEN;
			deb = '[';
			end = ']';
		}
		if (!has_child(i))
		{
			c_prt(color, format("%c.%c%s", deb, end, s_info[i].name),
			      j + 7 - start, table[j][1] * 4);
		}
		else if (s_info[i].dev)
		{
			c_prt(color, format("%c-%c%s", deb, end, s_info[i].name),
			      j + 7 - start, table[j][1] * 4);
		}
		else
		{
			c_prt(color, format("%c+%c%s", deb, end, s_info[i].name),
				j + 7 - start, table[j][1] * 4);
		}
		c_prt(color,
			format("%c%02ld.%03ld [%01d.%03d]", (get_skill_raw(i) < 0) ? '-' : ' ',
				abs(get_skill_raw(i)) / SKILL_STEP, abs(get_skill_raw(i)) % SKILL_STEP,
				s_info[i].mod / 1000, s_info[i].mod % 1000),
			j + 7 - start, 60);
	}
}
#endif

/*
 * Checks various stuff to do when skills change, like new spells, ...
 */
void recalc_skills(bool init)
{
	process_hooks(HOOK_RECALC_SKILLS, "()");

	/* Update stuffs */
	p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS | PU_POWERS |
		PU_SANITY | PU_BODY);

	/* Redraw various info */
	flag_bool(&p_ptr->redraw, FLAG_PR_WIPE);
	flag_bool(&p_ptr->redraw, FLAG_PR_BASIC);
	flag_bool(&p_ptr->redraw, FLAG_PR_EXTRA);
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);
}

/*
 * Recalc the skill value
 */
void recalc_skills_theory(s16b *invest, s32b *base_val, s32b *base_mod, s32b *bonus)
{
	s32b i, j;

	/* First we assign the normal points */
	for (i = 0; i < max_s_idx; i++)
	{
		/* Calc the base */
		s_info[i].value = base_val[i] + (base_mod[i] * invest[i]) + bonus[i];

		/* It cannot exceed SKILL_MAX */
		if (s_info[i].value > SKILL_MAX) s_info[i].value = SKILL_MAX;
	}

	/* Then we modify related skills */
	for (i = 0; i < max_s_idx; i++)
	{
		for (j = 1; j < max_s_idx; j++)
		{
			/* Ignore self */
			if (j == i) continue;

			/* Exclusive skills */
			if ((flag_get(&s_info[i].action, j) == SKILL_EXCLUSIVE) && invest[i])
			{
				/* Turn it off */
				p_ptr->skill_points += invest[j];
				invest[j] = 0;
				s_info[j].value = 0;
			}

			/* Non-exclusive skills */
			else if (flag_get(&s_info[i].action, j))
			{
				/* Increase / decrease with a % */
				s32b val = s_info[j].value + (invest[i] * s_info[j].mod * flag_get(&s_info[i].action, j) / 100);

				/* It cannot exceed SKILL_MAX */
				if (val > SKILL_MAX) val = SKILL_MAX;

				/* Save the modified value */
				s_info[j].value = val;
			}
		}
	}
}

#if 0 /* Not used anymore, see print_all, left in as a reminder how it worked */
/*
 * Interreact with skills
 */
void do_cmd_skill()
{
	s32b sel = 0, start = 0, max;
	s32b c;
	s32b **table;
	s32b i;
	s32b wid, hgt;
	s16b skill_points_save;
	s32b *skill_values_save;
	s32b *skill_mods_save;
	s16b *skill_rates_save;
	s16b *skill_invest;
	s32b *skill_bonus;

	recalc_skills(TRUE);

	/* Save the screen */
	screen_save();

	/* Allocate arrays to save skill values */
	C_MAKE(table, max_s_idx, s32b*);
	for (i = 0; i < max_s_idx; i++) C_MAKE(table[i], 2, s32b);
	C_MAKE(skill_values_save, max_s_idx, s32b);
	C_MAKE(skill_mods_save, max_s_idx, s32b);
	C_MAKE(skill_rates_save, max_s_idx, s16b);
	C_MAKE(skill_invest, max_s_idx, s16b);
	C_MAKE(skill_bonus, max_s_idx, s32b);

	/* Save skill points */
	skill_points_save = p_ptr->skill_points;

	/* Save skill values */
	for (i = 0; i < max_s_idx; i++)
	{
		skill_type *s_ptr = &s_info[i];

		skill_values_save[i] = s_ptr->value;
		skill_mods_save[i] = s_ptr->mod;
		skill_rates_save[i] = s_ptr->rate;
		skill_invest[i] = 0;
	}

	/* Clear the screen */
	Term_clear();

	/* Initialise the skill list */
	init_table(table, &max, FALSE);

	while (TRUE)
	{
		Term_get_size(&wid, &hgt);

		/* Display list of skills */
		recalc_skills_theory(skill_invest, skill_values_save, skill_mods_save, skill_bonus);
		print_skills(table, max, sel, start);

		/* Wait for user input */
		c = inkey();

		/* Leave the skill screen */
		if (c == ESCAPE) break;

		/* Expand / collapse list of skills */
		else if (c == '\r')
		{
			if (s_info[table[sel][0]].dev) s_info[table[sel][0]].dev = FALSE;
			else s_info[table[sel][0]].dev = TRUE;
			init_table(table, &max, FALSE);
		}

		/* Next page */
		else if (c == 'n')
		{
			sel += (hgt - 7);
			if (sel >= max) sel = max - 1;
		}

		/* Previous page */
		else if (c == 'p')
		{
			sel -= (hgt - 7);
			if (sel < 0) sel = 0;
		}

		/* Select / increase a skill */
		else
		{
			s32b dir;

			/* Allow use of numpad / arrow keys / roguelike keys */
			dir = get_keymap_dir(c);

			/* Move cursor down */
			if (dir == 2) sel++;

			/* Move cursor up */
			if (dir == 8) sel--;

			/* Increase the current skill */
			if (dir == 6) increase_skill(table[sel][0], skill_invest);

			/* Decrease the current skill */
			if (dir == 4) decrease_skill(table[sel][0], skill_invest);

			/* XXX XXX XXX Wizard mode commands outside of wizard2.c */

			/* Increase the skill */
			if (wizard && (c == '+')) skill_bonus[table[sel][0]] += SKILL_STEP;

			/* Decrease the skill */
			if (wizard && (c == '-')) skill_bonus[table[sel][0]] -= SKILL_STEP;

			/* Contextual help */
			if (c == '?') exec_lua(format("ingame_help('select_context', 'skill', '%s')", s_info[table[sel][0]].name));
			;

			/* Handle boundaries and scrolling */
			if (sel < 0) sel = max - 1;
			if (sel >= max) sel = 0;
			if (sel < start) start = sel;
			if (sel >= start + (hgt - 7)) start = sel - (hgt - 7) + 1;
		}
	}


	/* Some skill points are spent */
	if (p_ptr->skill_points != skill_points_save)
	{
		/* Flush input as we ask an important and irreversible question */
		flush();

		/* Ask we can commit the change */
		if (msg_box("Save and use these skill values? (y/n)", (s32b)(hgt / 2), (s32b)(wid / 2)) != 'y')
		{
			/* User declines -- restore the skill values before exiting */

			/* Restore skill points */
			p_ptr->skill_points = skill_points_save;

			/* Restore skill values */
			for (i = 0; i < max_s_idx; i++)
			{
				skill_type *s_ptr = &s_info[i];

				s_ptr->value = skill_values_save[i];
				s_ptr->mod = skill_mods_save[i];
				s_ptr->rate = skill_rates_save[i];
			}
		}
	}


	/* Free arrays to save skill values */
	C_FREE(skill_values_save, max_s_idx, s32b);
	C_FREE(skill_mods_save, max_s_idx, s32b);
	C_FREE(skill_rates_save, max_s_idx, s16b);
	C_FREE(skill_invest, max_s_idx, s16b);
	C_FREE(skill_bonus, max_s_idx, s32b);
	for (i = 0; i < max_s_idx; i++) C_FREE(table[i], 2, s32b);
	C_FREE(table, max_s_idx, s32b*);

	/* Load the screen */
	screen_load();

	recalc_skills(FALSE);
}
#endif


/*
 * List of combat skills
 */
s16b get_combat_skill()
{
	return p_ptr->combat_style;
}

static s32b get_combat_num()
{
	s32b max;

	call_lua("get_combat_num", "()", "d", &max);
	return max;
}

static void choose_combat()
{
	call_lua("choose_combat", "(d)", "", TRUE);
}

void select_default_combat()
{
	call_lua("select_default_combat", "()", "");
}

/*
 * Print a batch of skills.
 */
static void print_skill_batch(s32b *p, cptr *p_desc, s32b start, s32b max, bool mode)
{
	char buff[80];
	s32b i = start, j = 0;

	if (mode) prt(format("         %-31s", "Name"), 1, 20);

	for (i = start; i < (start + 20); i++)
	{
		if (i >= max) break;

		if (p[i] > 0)
			sprintf(buff, "  %c - %d) %-30s", I2A(j), (int) p[i], p_desc[i]);
		else
			sprintf(buff, "  %c - %d) %-30s", I2A(j), (int) p[i],
					"Change combat style");

		if (mode) prt(buff, 2 + j, 20);
		j++;
	}
	if (mode) prt("", 2 + j, 20);
	prt(format("Select a skill (a-%c), @ to select by name, +/- to scroll:", I2A(j - 1)), 0, 0);
}

s32b do_cmd_activate_skill_aux(s32b type)
{
	s32b which;
	s32b  max = 0, i, start = 0;
	s32b  ret = 0, x_idx;
	bool mode = easy_inven;
	s32b  *p;
	cptr *p_desc;

	C_MAKE(p, max_s_idx + max_ab_idx, s32b);
	C_MAKE(p_desc, max_s_idx + max_ab_idx, cptr);

	/* Count the max */

	/* More than 1 combat skill ? */
	if (get_combat_num() > 1)
	{
		p_desc[max] = "Change combat mode";
		p[max++] = 0;
	}

	for (i = 1; i < max_s_idx; i++)
	{
		x_idx = s_info[i].action_mkey[type];
		if (x_idx && get_skill(i) && (!s_info[i].hidden))
		{
			s32b j;
			bool next = FALSE;

			/* Already got it ? */
			for (j = 0; j < max; j++)
			{
				if (x_idx == p[j])
				{
					next = TRUE;
					break;
				}
			}
			if (next) continue;

			if(process_hooks(HOOK_MKEY_HIDE, "(d,d)", x_idx, type))
				continue;

			p_desc[max] = s_info[i].action_desc[type];
			p[max++] = x_idx;
		}
	}

	for (i = 0; i < max_ab_idx; i++)
	{
		x_idx = ab_info[i].action_mkey[type];
		if (x_idx && ab_info[i].acquired)
		{
			s32b j;
			bool next = FALSE;

			/* Already got it ? */
			for (j = 0; j < max; j++)
			{
				if (x_idx == p[j])
				{
					next = TRUE;
					break;
				}
			}
			if (next) continue;

			if(process_hooks(HOOK_MKEY_HIDE, "(d,d)", x_idx, type))
				continue;

			p_desc[max] = ab_info[i].action_desc[type];
			p[max++] = x_idx;
		}
	}

	if (!max)
	{
		msg_print("You don't have any activable skills or abilities.");
		return -1;
	}

	character_icky++;
	Term_save();

	while (1)
	{
		print_skill_batch(p, p_desc, start, max, mode);
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
			Term_save();
		}
		else if (which == '+')
		{
			start += 20;
			if (start >= max) start -= 20;
			Term_load();
			Term_save();
		}
		else if (which == '-')
		{
			start -= 20;
			if (start < 0) start += 20;
			Term_load();
			Term_save();
		}
		else if (which == '@')
		{
			char buf[80];

			strcpy(buf, "Cast a spell");
			if (!get_string("Skill action? ", buf, 79))
				break;

			/* Find the skill it is related to */
			for (i = 0; i < max; i++)
			{
				if (!strcmp(buf, p_desc[i]))
					break;
			}
			if ((i < max))
			{
				ret = p[i];
				break;
			}

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

	C_FREE(p, max_s_idx + max_ab_idx, s32b);
	C_FREE(p_desc, max_s_idx + max_ab_idx, cptr);

	return ret;
}

/* Ask & execute a skill */
void do_cmd_activate_skill(s32b type)
{
	s32b x_idx;
	bool push = TRUE;

	/* Get the skill, if available */
	if (repeat_pull(&x_idx))
	{
		push = FALSE;
	}
	else if (!command_arg) x_idx = do_cmd_activate_skill_aux(type);
	else
	{
		s32b i, j;

		x_idx = command_arg;

		/* Check validity */
		for (i = 1; i < max_s_idx; i++)
		{
			if (get_skill(i) && (s_info[i].action_mkey[type] == x_idx))
				break;
		}
		for (j = 0; j < max_ab_idx; j++)
		{
			if (ab_info[j].acquired && (ab_info[j].action_mkey[type] == x_idx))
				break;
		}

		if ((j == max_ab_idx) && (i == max_s_idx))
		{
			msg_print("Uh?");
			return;
		}
	}

	if (x_idx == -1) return;

	if (push) repeat_push(x_idx);

	if (!x_idx)
	{
		choose_combat();
		return;
	}

	if (type == ACTIVATE_SKILL)
	{
		switch (x_idx)
		{
		case MKEY_STEAL:
			do_cmd_steal();
			break;
		case MKEY_SCHOOL:
			cast_school_spell();
			break;
		case MKEY_COMPANION:
			if (get_skill(SKILL_LORE) >= 12)
				do_cmd_companion();
			else
				msg_print("You need a skill level of at least 12.");
			break;
		default:
			process_hooks(HOOK_MKEY, "(d,d)", x_idx, type);
			break;
		}
	}
	else if (type == BROWSE_SKILL)
	{
		switch (x_idx)
		{
		case MKEY_SCHOOL:
			do_cmd_browse();
			break;
		default:
			process_hooks(HOOK_MKEY, "(d,d)", x_idx, type);
			break;
		}
	}
}

/*
 * Gets the base value of a skill, given a race/class/...
 */
void compute_skills(s32b *v, s32b *m, s32b i)
{
	s32b z;
	s32b value, mod;

	/***** Apply each descriptors skills, in order *****/
	for (z = 1; z <= flag_max_key(&p_ptr->descriptors); z++)
	{
		player_descriptor *d_ptr = &descriptor_info[flag_get(&p_ptr->descriptors, z)];

		value = flag_get(&d_ptr->skill_base, i);
		mod = flag_get(&d_ptr->skill_mod, i);

		*v = modify_aux(*v, value, flag_get2(&d_ptr->skill_base, i));
		*m = modify_aux(*m, mod, flag_get2(&d_ptr->skill_mod, i));
	}
}

/*
 * Initialize a skill with given values
 */
void init_skill(s32b value, s32b mod, s32b i)
{
	s_info[i].value = value;
	s_info[i].mod = mod;

	if (has_flag(&s_info[i], FLAG_HIDDEN))
		s_info[i].hidden = TRUE;
	else
		s_info[i].hidden = FALSE;
}

void do_get_new_skill()
{
	char *items[4];
	s32b skl[4];
	s32b val[4], mod[4];
	bool *used;
	s32b *available_skills;
	s32b max = 0, max_a = 0, res, i;

	C_MAKE(used, max_s_idx, bool);
	C_MAKE(available_skills, max_s_idx, s32b);

	/* Check if some skills didn't influence other stuff */
	recalc_skills(TRUE);

	/* Grab the ones we can gain */
	max = 0;
	for (i = 0; i < max_s_idx; i++)
	{
		if (has_flag(&s_info[i], FLAG_RANDOM_GAIN))
			available_skills[max++] = i;
	}
	available_skills[max++] = -1;

	/* Init */
	for (max = 0; max < max_s_idx; max++)
	{
		used[max] = FALSE;
	}

	/* Count the number of available skills */
	while (available_skills[max_a] != -1) max_a++;

	/* Get 4 skills */
	for (max = 0; max < 4; max++)
	{
		s32b i;
		skill_type *s_ptr;

		/* Get an non used skill */
		do
		{
			i = rand_int(max_a);

			/* Does it pass the check? */
			if (!magik(get_flag(&s_info[i], FLAG_RANDOM_GAIN)))
				continue;
		}
		while (used[available_skills[i]]);

		s_ptr = &s_info[available_skills[i]];
		used[available_skills[i]] = TRUE;

		if (s_ptr->mod)
		{
			if (s_ptr->mod < 500)
			{
				val[max] = s_ptr->mod * 1;
				mod[max] = 100;
				if (mod[max] + s_ptr->mod > 500)
					mod[max] = 500 - s_ptr->mod;
			}
			else
			{
				val[max] = s_ptr->mod * 3;
				mod[max] = 0;
			}
		}
		else
		{
			mod[max] = 300;
			val[max] = 1000;
		}
		if (s_ptr->value + val[max] > SKILL_MAX) val[max] = SKILL_MAX - s_ptr->value;
		skl[max] = available_skills[i];
		items[max] = (char *)string_make(format("%-40s: +%02ld.%03ld value, +%01d.%03d modifier", s_ptr->name, val[max] / SKILL_STEP, val[max] % SKILL_STEP, mod[max] / SKILL_STEP, mod[max] % SKILL_STEP));
	}

	while (TRUE)
	{
		res = ask_menu("Choose a skill to learn(a-d to choose, ESC to cancel)?", (char **)items, 4);

		/* Ok ? lets learn ! */
		if (res > -1)
		{
			skill_type *s_ptr;
			bool oppose = FALSE;
			s32b oppose_skill = -1;

			/* Check we don't oppose an existing skill */
			for (i = 0; i < max_s_idx; i++)
			{
				if ((flag_get(&s_info[i].action, skl[res]) == SKILL_EXCLUSIVE) &&
				                (s_info[i].value != 0))
				{
					oppose = TRUE;
					oppose_skill = i;
					break;
				}
			}

			/* Ok we oppose, so be sure */
			if (oppose)
			{
				cptr msg;

				/*
				 * Because this is SO critical a question, we must flush
				 * input to prevent killing character off -- pelpel
				 */
				flush();

				/* Prepare prompt */
				msg = format("This skill is mutually exclusive with "
				             "at least %s, continue?",
				             s_info[oppose_skill].name);

				/* The player rejected the choice */
				if (!get_check(msg)) continue;
			}

			s_ptr = &s_info[skl[res]];
			s_ptr->value += val[res];
			s_ptr->mod += mod[res];
			if (mod[res])
			{
				msg_format("You can now learn the %s skill.",
				           s_ptr->name);
			}
			else
			{
				msg_format("Your knowledge of the %s skill increases.",
				           s_ptr->name);
			}
			break;
		}
	}

	/* Free them ! */
	for (max = 0; max < 4; max++)
	{
		string_free(items[max]);
	}

	/* Check if some skills didn't influence other stuff */
	recalc_skills(FALSE);

	C_FREE(used, max_s_idx, bool);
	C_FREE(available_skills, max_s_idx, s32b);
}




/**************************************** ABILITIES *****************************************/

/*
 * Given the name of an ability, returns ability index or -1 if no
 * such ability is found
 */
s16b find_ability(cptr name)
{
	u16b i;

	/* Scan ability list */
	for (i = 0; i < max_ab_idx; i++)
	{
		/* The name matches */
		if (streq(ab_info[i].name, name)) return (i);
	}

	/* No match found */
	return ( -1);
}

/*
 * Do the player have the ability
 */
bool has_ability(s32b ab)
{
	return (ab > -1) ? ab_info[ab].acquired : FALSE;
}

/* Do we meet the requirements */
bool can_learn_ability(s32b ab)
{
	ability_type *ab_ptr = &ab_info[ab];
	s32b i;

	if (ab_ptr->acquired)
		return FALSE;

	if (p_ptr->skill_points < ab_info[ab].cost)
		return FALSE;

	for (i = 0; i < 10; i++)
	{
		/* Must have skill level */
		if (ab_ptr->skills[i] > -1)
		{
			if (get_skill(ab_ptr->skills[i]) < ab_ptr->skill_levels[i])
				return FALSE;
		}

		/* Must have ability */
		if (ab_ptr->need_abilities[i] > -1)
		{
			if (!ab_info[ab_ptr->need_abilities[i]].acquired)
				return FALSE;
		}

		/* Must not have ability */
		if (ab_ptr->forbid_abilities[i] > -1)
		{
			if (ab_info[ab_ptr->forbid_abilities[i]].acquired)
				return FALSE;
		}
	}

	for (i = 0; i <= STATS_MAX; i++)
	{
		/* Must have stat */
		if (ab_ptr->stat[i] > -1)
		{
			if (get_stat(i, ind) < ab_ptr->stat[i] - 3)
				return FALSE;
		}
	}

	if (ab_ptr->allow != -1)
	{
		s32b ret;
		open_lua_functions_registry(get_lua_functions_registry_domain("ab_info"), ab_ptr->allow);
		call_lua(NULL, "(d)", "d", ab, &ret);
		close_lua_functions_registry();
		if (!ret) return FALSE;
	}

	/* Do the script allow us? */
	if (process_hooks(HOOK_LEARN_ABILITY, "(d)", ab))
		return FALSE;

	return TRUE;
}

/* Learn an ability */
static void gain_ability(s32b ab)
{
	s32b wid, hgt;
	Term_get_size(&wid, &hgt);

	if (!can_learn_ability(ab))
	{
		msg_box("You cannot learn this ability.", (s32b)(hgt / 2), (s32b)(wid / 2));
		return;
	}

	/* Flush input as we ask an important and irreversible question */
	flush();

	/* Ask we can commit the change */
/*	if (msg_box("Learn this ability(this is permanent)? (y/n)", (s32b)(hgt / 2), (s32b)(wid / 2)) != 'y')
	{
		return;
	}*/

	ab_info[ab].acquired = TRUE;
	p_ptr->skill_points -= ab_info[ab].cost;
}
static void ungain_ability(s32b ab, bool old)
{
	if ((!old) && ab_info[ab].acquired)
	{
		ab_info[ab].acquired = FALSE;
		p_ptr->skill_points += ab_info[ab].cost;
	}
}

/* helper function to generate a sorted table */
static void add_sorted_ability(s32b *table, s32b *max, s32b ab)
{
	s32b i;

	for (i = 0; i < *max; i++)
	{
		if (strcmp(ab_info[ab].name, ab_info[table[i]].name) < 0)
		{
			s32b z;

			/* Move all indexes up */
			for (z = *max; z > i; z--)
			{
				table[z] = table[z - 1];
			}
			break;
		}
	}
	table[i] = ab;

	(*max)++;
}

/*
 * Print the abilities list
 */
void dump_abilities(PHYSFS_file *fff)
{
	s32b i, j;
	s32b *table;
	s32b max = 0;

	C_MAKE(table, max_ab_idx, s32b);

	/* Initialise the abilities list */
	for (i = 0; i < max_ab_idx; i++)
	{
		if (ab_info[i].name && has_ability(i))
			add_sorted_ability(table, &max, i);
	}

	if (max)
	{
		fprintf(fff, "\nAbilities");

		for (j = 0; j < max; j++)
		{
			i = table[j];

			fprintf(fff, "\n * %s", ab_info[i].name);
		}

		fprintf(fff, "\n");
	}
}

#if 0 /* Not used anymore, see print_all, left in as a reminder how it worked */
/*
 * Draw the abilities list
 */
void print_abilities(s32b table[], s32b max, s32b sel, s32b start)
{
	s32b i, j;
	s32b wid, hgt;
	cptr keys;

	Term_clear();
	Term_get_size(&wid, &hgt);

	c_prt(TERM_WHITE, format("%s Abilities Screen", game_module), 0, 28);
	keys = format("#Bup#W/#Bdown#W to move, #Bright#W to buy, #B?#W for help");
	display_message(0, 1, strlen(keys), TERM_WHITE, keys);
	c_prt((p_ptr->skill_points) ? TERM_L_BLUE : TERM_L_RED,
	      format("Skill points left: %d", p_ptr->skill_points), 2, 0);

	print_desc_aux(ab_info[table[sel]].desc, 3, 0);

	for (j = start; j < start + (hgt - 7); j++)
	{
		byte color = TERM_WHITE;
		char deb = ' ', end = ' ';

		if (j >= max) break;

		i = table[j];

		if (ab_info[i].acquired)
			color = TERM_L_BLUE;
		else if (can_learn_ability(i))
			color = TERM_WHITE;
		else
			color = TERM_L_DARK;


		if (j == sel)
		{
			color = TERM_L_GREEN;
			deb = '[';
			end = ']';
		}

		c_prt(color, format("%c.%c%s", deb, end, ab_info[i].name),
		      j + 7 - start, 0);

		if (!ab_info[i].acquired)
		{
			c_prt(color, format("%d", ab_info[i].cost), j + 7 - start, 60);
		}
		else
		{
			c_prt(color, "Known", j + 7 - start, 60);
		}
	}
}
#endif

static bool show_ability(s32b ab)
{
	ability_type *ab_ptr = &ab_info[ab];
	s32b          i;

	if (ab_ptr->acquired)
		return (TRUE);

	for (i = 0; i < 10; i++)
	{
		/*
		 * If we don't have the skill level need to get the ability,
		 * we must be able to increase the skill level in the future,
		 * or we're never going to get it.
		 */
		if (ab_ptr->skills[i] > -1)
		{
			if ((get_skill(ab_ptr->skills[i]) < ab_ptr->skill_levels[i]) &&
				s_info[ab_ptr->skills[i]].mod == 0)
				return FALSE;
		}

		/*
		 * If there's pre-requisite abilities, we either have to have them
		 * or be cable of getting them in the future.
		 */
		if (ab_ptr->need_abilities[i] > -1 &&
			!show_ability(ab_ptr->need_abilities[i]))
			return FALSE;

		/* Must not have ability */
		if (ab_ptr->forbid_abilities[i] > -1 &&
			ab_info[ab_ptr->forbid_abilities[i]].acquired)
			return FALSE;
	}

	return TRUE;
}

#if 0 /* Not used anymore, see print_all, left in as a reminder how it worked */
/*
 * Interreact with abilitiess
 */
void do_cmd_ability()
{
	s32b  sel = 0, start = 0, max = 0;
	s32b c;
	s32b  *table;
	s32b  i;
	s32b  wid, hgt;

	if(!&ab_info[0])
	{
		msg_print("There are no abilities for you to learn!");
		return;
	}

	C_MAKE(table, max_ab_idx, s32b);

	/* Initialise the abilities list */
	for (i = 0; i < max_ab_idx; i++)
	{
		if (ab_info[i].name &&
			(wizard || (!ab_info[i].hidden && show_ability(i))))
			add_sorted_ability(table, &max, i);
	}

	if (max == 0)
	{
		C_FREE(table, max_ab_idx, s32b);
		msg_print("There are no abilities for you to learn!");
		return;
	}

	/* Save the screen */
	screen_save();

	/* Clear the screen */
	Term_clear();

	while (TRUE)
	{
		Term_get_size(&wid, &hgt);

		/* Display list of skills */
		print_abilities(table, max, sel, start);

		/* Wait for user input */
		c = inkey();

		/* Leave the skill screen */
		if (c == ESCAPE) break;

		/* Next page */
		else if (c == 'n')
		{
			sel += (hgt - 7);
			if (sel >= max) sel = max - 1;
		}

		/* Previous page */
		else if (c == 'p')
		{
			sel -= (hgt - 7);
			if (sel < 0) sel = 0;
		}

		/* Select / increase a skill */
		else
		{
			s32b dir;

			/* Allow use of numpad / arrow keys / roguelike keys */
			dir = get_keymap_dir(c);

			/* Move cursor down */
			if (dir == 2) sel++;

			/* Move cursor up */
			if (dir == 8) sel--;

			/* gain ability */
			if (dir == 6) gain_ability(table[sel]);

			/* XXX XXX XXX Wizard mode commands outside of wizard2.c */

			if (wizard && (c == '+')) ab_info[table[sel]].acquired = TRUE;
			if (wizard && (c == '-')) ab_info[table[sel]].acquired = FALSE;

			/* Contextual help */
			if (c == '?') exec_lua(format("ingame_help('select_context', 'ability', '%s')", ab_info[table[sel]].name));
			;

			/* Handle boundaries and scrolling */
			if (sel < 0) sel = max - 1;
			if (sel >= max) sel = 0;
			if (sel < start) start = sel;
			if (sel >= start + (hgt - 7)) start = sel - (hgt - 7) + 1;
		}
	}

	/* Load the screen */
	screen_load();

	C_FREE(table, max_ab_idx, s32b);

	/* Update stuffs */
	p_ptr->update |= (PU_BONUS | PU_HP | PU_MANA | PU_SPELLS | PU_POWERS |
	                  PU_SANITY | PU_BODY);

	/* Redraw various info */
	flag_bool(&p_ptr->redraw, FLAG_PR_WIPE);
	flag_bool(&p_ptr->redraw, FLAG_PR_BASIC);
	flag_bool(&p_ptr->redraw, FLAG_PR_EXTRA);
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);
}
#endif

static void print_all(s32b **table, s32b max, s32b table_ab[], s32b max_ab, s32b sel, s32b start)
{
	s32b i, j;
	s32b wid, hgt;
	cptr keys;

	Term_clear();
	Term_get_size(&wid, &hgt);

	c_prt(TERM_WHITE, format("%s Skills and Abilities Screen", game_module), 0, 24);
	keys = format("#BEnter#W to develop a branch, #Bup#W/#Bdown#W to move, #Bright#W/#Bleft#W to modify, #B?#W for help");
	display_message(0, 1, strlen(keys), TERM_WHITE, keys);
	c_prt((p_ptr->skill_points) ? TERM_L_BLUE : TERM_L_RED,
	      format("Skill points left: %d", p_ptr->skill_points), 2, 0);

	if (sel == 0)
	{
		c_prt(TERM_YELLOW, "Skills represent your level of proficiency in the various fields of mastery", 3, 0);
		c_prt(TERM_YELLOW, "available to your character.", 4, 0);
		c_prt(TERM_YELLOW, format("Skills can naturally be raised to %ld.", SKILL_MAX / SKILL_STEP), 5, 0);
	}
	else if (sel == max)
	{
		c_prt(TERM_YELLOW, "Abilities are various traits your character may acquire. Contrary to skills", 3, 0);
		c_prt(TERM_YELLOW, "abilities are on/off toggles, you either now them or not, there is no level", 4, 0);
		c_prt(TERM_YELLOW, "involved.", 5, 0);
	}
	else if (sel < max)
		print_desc_aux(s_info[table[sel-1][0]].desc, 3, 0);
	else
		print_desc_aux(ab_info[table_ab[sel-1 - max]].desc, 3, 0);

	for (j = start; j < start + (hgt - 7); j++)
	{
		byte color = TERM_WHITE;
		char deb = ' ', end = ' ';

		if ((j == 0) && max)
		{
			deb = end = '-';
			color = TERM_YELLOW;
			if (j == sel)
			{
				color = TERM_ORANGE;
				deb = '[';
				end = ']';
			}
			c_prt(color, format("%c-%c---------------------------------------------------------------------- Skills", deb, end), j + 7 - start, 0);
		}
		else if ((j == max) && max_ab)
		{
			deb = end = '-';
			color = TERM_YELLOW;
			if (j == sel)
			{
				color = TERM_ORANGE;
				deb = '[';
				end = ']';
			}
			c_prt(color, format("%c-%c------------------------------------------------------------------- Abilities", deb, end), j + 7 - start, 0);
		}
		else if (j < max)
		{
			i = table[j-1][0];

			if (get_skill(i) == 0)
			{
				if (s_info[i].mod == 0) color = TERM_L_DARK;
				else color = TERM_ORANGE;
			}
			else if (get_skill_raw(i) == SKILL_MAX) color = TERM_L_BLUE;
			if (s_info[i].hidden) color = TERM_L_RED;
			if (j == sel)
			{
				color = TERM_L_GREEN;
				deb = '[';
				end = ']';
			}
			if (!has_child(i))
			{
				c_prt(color, format("%c.%c%s", deb, end, s_info[i].name),
					j + 7 - start, table[j-1][1] * 4);
			}
			else if (s_info[i].dev)
			{
				c_prt(color, format("%c-%c%s", deb, end, s_info[i].name),
					j + 7 - start, table[j-1][1] * 4);
			}
			else
			{
				c_prt(color, format("%c+%c%s", deb, end, s_info[i].name),
					j + 7 - start, table[j-1][1] * 4);
			}
			c_prt(color,
				format("%c%02ld.%03ld [%01d.%03d]", (get_skill_raw(i) < 0) ? '-' : ' ',
					abs(get_skill_raw(i)) / SKILL_STEP, abs(get_skill_raw(i)) % SKILL_STEP,
					s_info[i].mod / 1000, s_info[i].mod % 1000),
				j + 7 - start, 60);
		}
		else
		{
			byte color = TERM_WHITE;
			char deb = ' ', end = ' ';

			if (j - max >= max_ab) break;

			i = table_ab[j-1 - max];

			if (ab_info[i].acquired)
				color = TERM_L_BLUE;
			else if (can_learn_ability(i))
				color = TERM_WHITE;
			else
				color = TERM_L_DARK;


			if (j == sel)
			{
				color = TERM_L_GREEN;
				deb = '[';
				end = ']';
			}

			c_prt(color, format("%c.%c%s", deb, end, ab_info[i].name),
				j + 7 - start, 0);

			if (!ab_info[i].acquired)
			{
				c_prt(color, format("%d", ab_info[i].cost), j + 7 - start, 61);
			}
			else
			{
				c_prt(color, "Known", j + 7 - start, 61);
			}
		}
	}
}

/*
 * Interreact with skills
 */
void do_cmd_skill()
{
	s32b sel = 0, start = 0, max, max_ab = 0;
	s32b c;
	s32b **table, *table_ab;
	s32b i;
	s32b wid, hgt;
	s16b skill_points_save;
	s32b *skill_values_save;
	s32b *skill_mods_save;
	s16b *skill_rates_save;
	s16b *skill_invest;
	s32b *skill_bonus;
	bool *ab_learned;

	recalc_skills(TRUE);

	/* Save the screen */
	screen_save();

	/* Allocate arrays to save skill values */
	C_MAKE(table, max_s_idx, s32b*);
	for (i = 0; i < max_s_idx; i++) C_MAKE(table[i], 2, s32b);
	C_MAKE(skill_values_save, max_s_idx, s32b);
	C_MAKE(skill_mods_save, max_s_idx, s32b);
	C_MAKE(skill_rates_save, max_s_idx, s16b);
	C_MAKE(skill_invest, max_s_idx, s16b);
	C_MAKE(skill_bonus, max_s_idx, s32b);

	/* Initialise the abilities list */
	C_MAKE(ab_learned, max_ab_idx, s32b);
	C_MAKE(table_ab, max_ab_idx, s32b);
	for (i = 0; i < max_ab_idx; i++)
	{
		ab_learned[i] = ab_info[i].acquired;
		if (ab_info[i].name &&
			(wizard || (!ab_info[i].hidden && show_ability(i))))
			add_sorted_ability(table_ab, &max_ab, i);
	}

	/* Save skill points */
	skill_points_save = p_ptr->skill_points;

	/* Save skill values */
	for (i = 0; i < max_s_idx; i++)
	{
		skill_type *s_ptr = &s_info[i];

		skill_values_save[i] = s_ptr->value;
		skill_mods_save[i] = s_ptr->mod;
		skill_rates_save[i] = s_ptr->rate;
		skill_invest[i] = 0;
	}

	/* Clear the screen */
	Term_clear();

	/* Initialise the skill list */
	init_table(table, &max, FALSE);
	if (max) max++;
	if (max_ab) max_ab++;

	if (max > 1) sel = 1;

	while (TRUE)
	{
		Term_get_size(&wid, &hgt);

		/* Display list of skills */
		recalc_skills_theory(skill_invest, skill_values_save, skill_mods_save, skill_bonus);
		print_all(table, max, table_ab, max_ab, sel, start);

		/* Wait for user input */
		c = inkey();

		/* Leave the skill screen */
		if (c == ESCAPE) break;

		/* Expand / collapse list of skills */
		else if ((sel < max) && (sel != 0) && (c == '\r'))
		{
			if (s_info[table[sel-1][0]].dev) s_info[table[sel-1][0]].dev = FALSE;
			else s_info[table[sel-1][0]].dev = TRUE;
			init_table(table, &max, FALSE);
			max++;
		}

		/* Next page */
		else if (c == 'n')
		{
			sel += (hgt - 7);
			if (sel >= max + max_ab) sel = max + max_ab - 1;
		}

		/* Previous page */
		else if (c == 'p')
		{
			sel -= (hgt - 7);
			if (sel < 0) sel = 0;
		}

		/* Select / increase a skill */
		else
		{
			s32b dir;

			/* Allow use of numpad / arrow keys / roguelike keys */
			dir = get_keymap_dir(c);

			/* Move cursor down */
			if (dir == 2) sel++;

			/* Move cursor up */
			if (dir == 8) sel--;

			if (sel == 0 || sel == max)
			{
				// Nothing
			}
			else if (sel < max)
			{
				/* Increase the current skill */
				if (dir == 6) increase_skill(table[sel-1][0], skill_invest);

				/* Decrease the current skill */
				if (dir == 4) decrease_skill(table[sel-1][0], skill_invest);

				/* XXX XXX XXX Wizard mode commands outside of wizard2.c */

				/* Increase the skill */
				if (wizard && (c == '+')) skill_bonus[table[sel-1][0]] += SKILL_STEP;

				/* Decrease the skill */
				if (wizard && (c == '-')) skill_bonus[table[sel-1][0]] -= SKILL_STEP;

				/* Contextual help */
				if (c == '?') exec_lua(format("ingame_help('select_context', 'skill', '%s')", s_info[table[sel-1][0]].name));
			}
			else
			{
				/* Gain ability */
				if (dir == 6) gain_ability(table_ab[sel-1 - max]);
				if (dir == 4) ungain_ability(table_ab[sel-1 - max], ab_learned[table_ab[sel-1 - max]]);

				/* XXX XXX XXX Wizard mode commands outside of wizard2.c */
				if (wizard && (c == '+')) ab_info[table_ab[sel-1 - max]].acquired = TRUE;
				if (wizard && (c == '-')) ab_info[table_ab[sel-1 - max]].acquired = FALSE;

				/* Contextual help */
				if (c == '?') exec_lua(format("ingame_help('select_context', 'ability', '%s')", ab_info[table_ab[sel-1 - max]].name));
			}

			/* Handle boundaries and scrolling */
			if (sel < 0) sel = max + max_ab - 1;
			/* Just in case the skill and ability list is empty */
			if (sel < 0) sel = 0;
			if (sel >= max + max_ab) sel = 0;
			if (sel < start) start = sel;
			if (sel >= start + (hgt - 7)) start = sel - (hgt - 7) + 1;
		}
	}


	/* Some skill points are spent */
	if (p_ptr->skill_points != skill_points_save)
	{
		/* Flush input as we ask an important and irreversible question */
		flush();

		/* Ask we can commit the change */
		if (msg_box("Save and use these skill values? (y/n)", (s32b)(hgt / 2), (s32b)(wid / 2)) != 'y')
		{
			/* User declines -- restore the skill values before exiting */

			/* Restore skill points */
			p_ptr->skill_points = skill_points_save;

			/* Restore skill values */
			for (i = 0; i < max_s_idx; i++)
			{
				skill_type *s_ptr = &s_info[i];

				s_ptr->value = skill_values_save[i];
				s_ptr->mod = skill_mods_save[i];
				s_ptr->rate = skill_rates_save[i];
			}

			/* Restore abilities */
			for (i = 0; i < max_ab_idx; i++)
			{
				ab_info[i].acquired = ab_learned[i];
			}
		}
	}


	/* Free arrays to save skill values */
	C_FREE(skill_values_save, max_s_idx, s32b);
	C_FREE(skill_mods_save, max_s_idx, s32b);
	C_FREE(skill_rates_save, max_s_idx, s16b);
	C_FREE(skill_invest, max_s_idx, s16b);
	C_FREE(skill_bonus, max_s_idx, s32b);
	for (i = 0; i < max_s_idx; i++) C_FREE(table[i], 2, s32b);
	C_FREE(table, max_s_idx, s32b*);
	C_FREE(ab_learned, max_ab_idx, s32b);
	C_FREE(table_ab, max_ab_idx, s32b);

	/* Load the screen */
	screen_load();

	recalc_skills(FALSE);
}


/*
 * Apply abilities to be granted this level
 */
void apply_level_abilities(s32b level)
{
	s32b z, i;

	/***** Apply each descriptors abilities, in order *****/
	for (z = 1; z <= flag_max_key(&p_ptr->descriptors); z++)
	{
		player_descriptor *d_ptr = &descriptor_info[flag_get(&p_ptr->descriptors, z)];

		/* Check all the  */
		for (i = 0; i < d_ptr->abilities.size; i++)
		{
			flag_node *n = &d_ptr->abilities.node[i];

			if (n->flags & FLAG_FLAG_USED)
			{
				if (flag_get(&d_ptr->abilities, n->key) == level)
				{
					if ((level > 1) && (!ab_info[n->key].acquired))
						cmsg_format(TERM_L_GREEN, "You have learned the ability '%s'.", ab_info[n->key].name);
					ab_info[n->key].acquired = TRUE;
				}
			}
		}
	}
}
