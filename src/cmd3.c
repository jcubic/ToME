/* File: cmd3.c */

/* Purpose: Inventory commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/*
 * Remove the inscription from an object
 * XXX Mention item (when done)?
 */
void do_cmd_uninscribe(void)
{
	s32b item;

	object_type *o_ptr;

	cptr q, s;


	/* Get an item */
	q = "Un-inscribe which item? ";
	s = "You have nothing to un-inscribe.";
	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return;

	/* Get the item (in the pack) */
	o_ptr = get_object(item);

	/* Nothing to remove */
	if (!o_ptr->note)
	{
		msg_print("That item had no inscription to remove.");
		return;
	}

	/* Message */
	msg_print("Inscription removed.");

	/* Remove the incription */
	o_ptr->note = 0;

	/* Combine the pack */
	p_ptr->notice |= (PN_COMBINE);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_INVEN);
	flag_bool(&p_ptr->window, FLAG_PW_EQUIP);
}


/*
 * Inscribe an object with a comment
 */
void do_cmd_inscribe(void)
{
	s32b item;

	object_type *o_ptr;

	char o_name[80];

	char out_val[80];

	cptr q, s;


	/* Get an item */
	q = "Inscribe which item? ";
	s = "You have nothing to inscribe.";
	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return;

	/* Get the item (in the pack) */
	o_ptr = get_object(item);

	/* Describe the activity */
	object_desc(o_name, o_ptr, TRUE, 3);

	/* Message */
	msg_format("Inscribing %s.", o_name);
	msg_print(NULL);

	/* Start with nothing */
	strcpy(out_val, "");

	/* Use old inscription */
	if (o_ptr->note)
	{
		/* Start with the old inscription */
		strcpy(out_val, quark_str(o_ptr->note));
	}

	/* Get a new inscription (possibly empty) */
	if (get_string("Inscription: ", out_val, 80))
	{
		/* Save the inscription */
		o_ptr->note = quark_add(out_val);

		/* Combine the pack */
		p_ptr->notice |= (PN_COMBINE);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_INVEN);
		flag_bool(&p_ptr->window, FLAG_PW_EQUIP);
	}
}




/*
 * Target command
 */
void do_cmd_target(void)
{
	/* Target set */
	if (target_set(TARGET_KILL))
	{
		msg_print("Target Selected.");
	}

	/* Target aborted */
	else
	{
		msg_print("Target Aborted.");
	}
}



/*
 * Look command
 */
void do_cmd_look(void)
{
	/* Look around */
	if (target_set(TARGET_LOOK))
	{
		msg_print("Target Selected.");
	}
}



/*
 * Allow the player to examine other sectors on the map
 */
void do_cmd_locate(void)
{
	s32b dir, y1, x1, y2, x2;
	s32b panel_hgt, panel_wid;
	char tmp_val[80];
	char out_val[160];

	/* Retrieve size of the Angband window */
	panel_wid = COL_MAP_WID;
	panel_hgt = ROW_MAP_HGT;

	/* Calcurate size of the dungeon map area */
	panel_hgt = (panel_hgt - (ROW_MAP + 1)) / 2;
	panel_wid = (panel_wid - (COL_MAP + 1)) / 2;

	/* Start at current panel */
	y2 = y1 = panel_row_min;
	x2 = x1 = panel_col_min;

	/* Show panels until done */
	while (1)
	{
		/* Describe the location */
		if ((y2 == y1) && (x2 == x1))
		{
			tmp_val[0] = '\0';
		}
		else
		{
			strnfmt(tmp_val, 80, "%s%s of",
			        ((y2 < y1) ? " North" : (y2 > y1) ? " South" : ""),
			        ((x2 < x1) ? " West" : (x2 > x1) ? " East" : ""));
		}

		/* Panels are measured by current map area size */
		strnfmt(out_val, 160,
			"Map sector [%d(%02d),%d(%02d)], which is%s your sector. Direction?",
			y2 / panel_hgt, y2 % panel_hgt,
			x2 / panel_wid, x2 % panel_wid, tmp_val);

		/* Assume no direction */
		dir = 0;

		/* Get a direction */
		while (!dir)
		{
			s32b ch;

			/* Get a command (or cancel) */
			if (!get_com(out_val, &ch)) break;

			/* Extract the action (if any) */
			dir = get_keymap_dir(ch);

			/* Error */
			if (!dir) bell();
		}

		/* No direction */
		if (!dir) break;

		/* Apply the motion */
		if (change_panel(ddy[dir], ddx[dir]))
		{
			y2 = panel_row_min;
			x2 = panel_col_min;
		}
	}

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
}



/*
 * Sorting hook -- Comp function -- see below
 *
 * We use "u" to point to array of monster indexes,
 * and "v" to select the type of sorting to perform on "u".
 */
static bool ang_sort_comp_hook(vptr u, vptr v, s32b a, s32b b)
{
	u16b *who = (u16b*)(u);

	u16b *why = (u16b*)(v);

	s32b w1 = who[a];

	s32b w2 = who[b];

	s32b z1, z2;


	/* Sort by player kills */
	if (*why >= 4)
	{
		/* Extract player kills */
		z1 = r_info[w1].r_pkills;
		z2 = r_info[w2].r_pkills;

		/* Compare player kills */
		if (z1 < z2) return (TRUE);
		if (z1 > z2) return (FALSE);
	}


	/* Sort by total kills */
	if (*why >= 3)
	{
		/* Extract total kills */
		z1 = r_info[w1].r_tkills;
		z2 = r_info[w2].r_tkills;

		/* Compare total kills */
		if (z1 < z2) return (TRUE);
		if (z1 > z2) return (FALSE);
	}


	/* Sort by monster level */
	if (*why >= 2)
	{
		/* Extract levels */
		z1 = r_info[w1].level;
		z2 = r_info[w2].level;

		/* Compare levels */
		if (z1 < z2) return (TRUE);
		if (z1 > z2) return (FALSE);
	}


	/* Sort by monster experience */
	if (*why >= 1)
	{
		/* Extract experience */
		z1 = r_info[w1].mexp;
		z2 = r_info[w2].mexp;

		/* Compare experience */
		if (z1 < z2) return (TRUE);
		if (z1 > z2) return (FALSE);
	}


	/* Compare indexes */
	return (w1 <= w2);
}


/*
 * Sorting hook -- Swap function -- see below
 *
 * We use "u" to point to array of monster indexes,
 * and "v" to select the type of sorting to perform.
 */
static void ang_sort_swap_hook(vptr u, vptr v, s32b a, s32b b)
{
	u16b *who = (u16b*)(u);

	u16b holder;


	/* XXX XXX */
	v = v ? v : 0;

	/* Swap */
	holder = who[a];
	who[a] = who[b];
	who[b] = holder;
}



/*
 * Hack -- Display the "name" and "attr/chars" of a monster race
 */
static void roff_top(s32b r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	byte a1, a2;

	char c1, c2;


	/* Access the chars */
	c1 = r_ptr->d_char;
	c2 = r_ptr->x_char;

	/* Access the attrs */
	a1 = r_ptr->d_attr;
	a2 = r_ptr->x_attr;

	/* Hack -- fake monochrome */
	if (!use_color) a1 = TERM_WHITE;
	if (!use_color) a2 = TERM_WHITE;


	/* Clear the top line */
	Term_erase(0, 0, 255);

	/* Reset the cursor */
	Term_gotoxy(0, 0);

	/* A title (use "The" for non-uniques) */
	if (!(has_flag(r_ptr, FLAG_UNIQUE)))
	{
		Term_addstr( -1, TERM_WHITE, "The ");
	}

	/* Dump the name */
	Term_addstr( -1, TERM_WHITE, (r_ptr->name));

	/* Append the "standard" attr/char info */
	Term_addstr( -1, TERM_WHITE, " ('");
	Term_addch(a1, c1);
	if (use_bigtile && (a1 & 0x80)) Term_addch(255, 127);
	Term_addstr( -1, TERM_WHITE, "')");

	/* Append the "optional" attr/char info */
	Term_addstr( -1, TERM_WHITE, "/('");
	Term_addch(a2, c2);
	if (use_bigtile && (a2 & 0x80)) Term_addch(255, 127);
	Term_addstr( -1, TERM_WHITE, "'):");
}


/*
 * Identify a character, allow recall of monsters
 *
 * Several "special" responses recall "multiple" monsters:
 *   ^A (all monsters)
 *   ^U (all unique monsters)
 *   ^N (all non-unique monsters)
 *   ^M (case insensitive name search)
 *
 * The responses may be sorted in several ways, see below.
 *
 * Note that the player ghosts are ignored. XXX XXX XXX
 */
void do_cmd_query_symbol(void)
{
	s32b i, n, r_idx;

	s32b query;

	s32b sym;

	char buf[128];


	bool all = FALSE;

	bool uniq = FALSE;

	bool norm = FALSE;


	bool name = FALSE;

	char temp[80] = "";


	bool recall = FALSE;


	u16b why = 0;

	u16b *who;

	cptr info;


	/* Get a character, or abort */
	if (!get_com("Enter character to be identified, "
	                "or (Ctrl-A, Ctrl-U, Ctrl-N, Ctrl-M):", &sym)) return;

	/* Find that character info, and describe it */
	call_lua("get_symbol_info", "(d)", "s", sym, &info);

	/* Describe */
	if (sym == KTRL('A'))
	{
		all = TRUE;
		strcpy(buf, "Full monster list.");
	}
	else if (sym == KTRL('U'))
	{
		all = uniq = TRUE;
		strcpy(buf, "Unique monster list.");
	}
	else if (sym == KTRL('N'))
	{
		all = norm = TRUE;
		strcpy(buf, "Non-unique monster list.");
	}
	else if (sym == KTRL('M'))
	{
		all = name = TRUE;
		if (!get_string("Name:", temp, 70)) return;
		strnfmt(buf, 128, "Monsters with a name \"%s\"", temp);
		strlower(temp);
	}
	strnfmt(buf, 128, "%c - %s.", sym, info);

	/* Display the result */
	prt(buf, 0, 0);

	/* Allocate the "who" array */
	C_MAKE(who, max_r_idx, u16b);

	/* Collect matching monsters */
	for (n = 0, i = 1; i < max_r_idx; i++)
	{
		monster_race *r_ptr = &r_info[i];

		/* Nothing to recall */
		if (!cheat_know && !r_ptr->r_sights) continue;

		/* Require non-unique monsters if needed */
		if (norm && (has_flag(r_ptr, FLAG_UNIQUE))) continue;

		/* Require unique monsters if needed */
		if (uniq && !(has_flag(r_ptr, FLAG_UNIQUE))) continue;

		/* Require monsters with the name requested if needed */
		if (name)
		{
			char mon_name[80];

			strcpy(mon_name, r_ptr->name);
			strlower(mon_name);

			if (!strstr(mon_name, temp)) continue;
		}

		/* Collect "appropriate" monsters */
		if (all || (r_ptr->d_char == sym)) who[n++] = i;
	}

	/* Nothing to recall */
	if (!n)
	{
		/* Free the "who" array */
		C_KILL(who, max_r_idx, u16b);

		return;
	}


	/* Prompt XXX XXX XXX */
	put_str("Recall details? (k/p/y/n): ", 0, 40);

	/* Query */
	query = inkey();

	/* Restore */
	prt(buf, 0, 0);


	/* Sort by kills (and level) */
	if (query == 'k')
	{
		why = 4;
		query = 'y';
	}

	/* Sort by level */
	if (query == 'p')
	{
		why = 2;
		query = 'y';
	}

	/* Catch "escape" */
	if (query != 'y')
	{
		/* Free the "who" array */
		C_KILL(who, max_r_idx, u16b);

		return;
	}


	/* Sort if needed */
	if (why)
	{
		/* Select the sort method */
		ang_sort_comp = ang_sort_comp_hook;
		ang_sort_swap = ang_sort_swap_hook;

		/* Sort the array */
		ang_sort(who, &why, n);
	}


	/* Start at the end */
	i = n - 1;

	/* Scan the monster memory */
	while (1)
	{
		/* Extract a race */
		r_idx = who[i];

		/* Hack -- Auto-recall */
		monster_race_track(r_idx, 0);

		/* Hack -- Handle stuff */
		handle_stuff();

		/* Hack -- Begin the prompt */
		roff_top(r_idx);

		/* Hack -- Complete the prompt */
		Term_addstr( -1, TERM_WHITE, " [(r)ecall, ESC]");

		/* Interact */
		while (1)
		{
			/* Recall */
			if (recall)
			{
				/* Save the screen */
				character_icky++;
				Term_save();

				/* Recall on screen */
				screen_roff(who[i], 0, 0);

				/* Hack -- Complete the prompt (again) */
				Term_addstr( -1, TERM_WHITE, " [(r)ecall, ESC]");
			}

			/* Command */
			query = inkey();

			/* Unrecall */
			if (recall)
			{
				/* Restore */
				Term_load();
				character_icky--;
			}

			/* Normal commands */
			if (query != 'r') break;

			/* Toggle recall */
			recall = !recall;
		}

		/* Stop scanning */
		if (query == ESCAPE) break;

		/* Move to "prev" monster */
		if (query == '-')
		{
			if (++i == n)
			{
				i = 0;
				if (!expand_list) break;
			}
		}

		/* Move to "next" monster */
		else
		{
			if (i-- == 0)
			{
				i = n - 1;
				if (!expand_list) break;
			}
		}
	}

	/* Re-display the identity */
	prt(buf, 0, 0);

	/* Free the "who" array */
	C_KILL(who, max_r_idx, u16b);
}


/*
 *  research_mon
 *  -KMW-
 */
bool research_mon()
{
	s32b i, n, r_idx;

	s32b query;

	s32b sym;

	char buf[128];


	s16b oldkills;

	byte oldwake;

	bool oldcheat;


	bool all = FALSE;

	bool uniq = FALSE;

	bool norm = FALSE;

	bool notpicked;


	bool recall = FALSE;

	u16b why = 0;

	monster_race *r2_ptr;

	u16b *who;

	cptr info;


	/* Hack -- Remember "cheat_know" flag */
	oldcheat = cheat_know;


	/* Get a character, or abort */
	if (!get_com("Enter character of monster: ", &sym)) return (TRUE);

	/* Allocate the "who" array */
	C_MAKE(who, max_r_idx, u16b);

	/* Find that character info, and describe it */
	call_lua("get_symbol_info", "(d)", "s", sym, &info);

	strnfmt(buf, 128, "%c - %s.", sym, info);

	/* Display the result */
	prt(buf, 16, 10);


	/* Collect matching monsters */
	for (n = 0, i = 1; i < max_r_idx; i++)
	{
		monster_race *r_ptr = &r_info[i];

		/* Hack -- Force "cheat_know" */
		cheat_know = TRUE;

		/* Nothing to recall */
		if (!cheat_know && !r_ptr->r_sights) continue;

		/* Require non-unique monsters if needed */
		if (norm && (has_flag(r_ptr, FLAG_UNIQUE))) continue;

		/* Require unique monsters if needed */
		if (uniq && !(has_flag(r_ptr, FLAG_UNIQUE))) continue;

		/* Collect "appropriate" monsters */
		if (all || (r_ptr->d_char == sym)) who[n++] = i;
	}

	/* Nothing to recall */
	if (!n)
	{
		/* Free the "who" array */
		C_KILL(who, max_r_idx, u16b);

		/* Restore the "cheat_know" flag */
		cheat_know = oldcheat;

		return (TRUE);
	}


	/* Sort by level */
	why = 2;
	query = 'y';

	/* Sort if needed */
	if (why)
	{
		/* Select the sort method */
		ang_sort_comp = ang_sort_comp_hook;
		ang_sort_swap = ang_sort_swap_hook;

		/* Sort the array */
		ang_sort(who, &why, n);
	}


	/* Start at the end */
	i = n - 1;

	notpicked = TRUE;

	/* Scan the monster memory */
	while (notpicked)
	{
		/* Extract a race */
		r_idx = who[i];

		/* Hack -- Auto-recall */
		monster_race_track(r_idx, 0);

		/* Hack -- Handle stuff */
		handle_stuff();

		/* Hack -- Begin the prompt */
		roff_top(r_idx);

		/* Hack -- Complete the prompt */
		Term_addstr( -1, TERM_WHITE, " [(r)ecall, ESC, space to continue]");

		/* Interact */
		while (1)
		{
			/* Recall */
			if (recall)
			{
				/* Save the screen */
				character_icky++;
				Term_save();

				/* Recall on screen */
				r2_ptr = &r_info[r_idx];

				oldkills = r2_ptr->r_tkills;
				oldwake = r2_ptr->r_wake;
				screen_roff(who[i], 0, 1);
				r2_ptr->r_tkills = oldkills;
				r2_ptr->r_wake = oldwake;
				r2_ptr->r_sights = 1;
				cheat_know = oldcheat;
				notpicked = FALSE;
				break;

			}

			/* Command */
			query = inkey();

			/* Unrecall */
			if (recall)
			{
				/* Restore */
				Term_load();
				character_icky--;
			}

			/* Normal commands */
			if (query != 'r') break;

			/* Toggle recall */
			recall = !recall;
		}

		/* Stop scanning */
		if (query == ESCAPE) break;

		/* Move to "prev" monster */
		if (query == '-')
		{
			if (++i == n)
			{
				i = 0;
				if (!expand_list) break;
			}
		}

		/* Move to "next" monster */
		else
		{
			if (i-- == 0)
			{
				i = n - 1;
				if (!expand_list) break;
			}
		}
	}


	/* Re-display the identity */
	/* prt(buf, 5, 5);*/

	/* Free the "who" array */
	C_KILL(who, max_r_idx, u16b);

	/* Restore the "cheat_know" flag */
	cheat_know = oldcheat;

	return (notpicked);
}


/*
 * Try to "sense" the grid's mana
 */
bool do_cmd_sense_grid_mana()
{
	s32b chance, i;


	/* Take (a lot of) time */
	energy_use = get_player_energy(SPEED_SENSE) * 2;

	/* Base chance of success */
	chance = p_ptr->skill_dev;

	/* Confusion hurts skill */
	if (intrinsic(CONFUSED)) chance = chance / 2;

	/* Hight mana grids are harder */
	chance = chance - (cave[p_ptr->py][p_ptr->px].mana / 10);

	/* Give everyone a (slight) chance */
	if ((chance < USE_DEVICE) && (rand_int(USE_DEVICE - chance + 1) == 0))
	{
		chance = USE_DEVICE;
	}

	/* Roll for usage */
	if ((chance < USE_DEVICE) || (randint(chance) < USE_DEVICE))
	{
		if (flush_failure) flush();
		msg_print("You failed to sense the grid's mana.");
		sound(SOUND_FAIL);
		return FALSE;
	}

	/* Try to give an "average" value */
	i = (101 - p_ptr->skill_dev) / 2;
	i = (i < 1) ? 1 : (i > 50) ? 50 : i;

	if (wizard)
	{
		msg_format("Grid's mana: %d.", cave[p_ptr->py][p_ptr->px].mana);
		msg_format("Average grid's mana: %d.", (cave[p_ptr->py][p_ptr->px].mana / i) * i);
	}
	else
	{
		msg_format("Average Area's mana: %d", (cave[p_ptr->py][p_ptr->px].mana / i) * i);
	}
	return TRUE;
}


/*
 * Calculate the weight of the portable holes
 */
s32b portable_hole_weight(void)
{
	s32b weight = 0;
#if 0 // DGDGDGDG -- kill me
	store_type *st_ptr = &town_info[TOWN_RANDOM].store[STORE_HOME];


	/* Sum the objects in the appropriate home */
	for (i = 0, weight = 0; i < st_ptr->stock_num; i++)
	{
		object_type *o_ptr = st_ptr->stock[i];

		weight += (o_ptr->weight * o_ptr->number);
	}

	/* Multiply the sum with 1.5 */
	weight = (weight * 3) / 2 + 2;
#endif
	return (weight);
}

/*
 * Try to add a CLI action.
 */
void cli_add(cptr active, cptr trigger, cptr descr)
{
	s16b num;
	cli_comm *cli_ptr, *old_ptr;

	/* Too many macros. */
	if (cli_total >= CLI_MAX) return;

	/* First try to read active as a number. */
	if (strtol(active, 0, 0))
	{
		num = strtol(active, 0, 0);
	}
	/* Then try to read it as a character. */
	else if (strlen(active) == 1)
	{
		num = active[0];
	}
	/* Give up if it doesn't work. */
	else
	{
		return;
	}

	/* Dump the macro. */
	cli_ptr = cli_info + cli_total;
	old_ptr = cli_info + cli_total - 1;

	/*
	 * Trim 's from the ends of a token. This turns '@' into @ and
	 * ''' into '. This may be the intent of the code in tokenize(),
	 * but I've left it for lack of comments to back me up.
	 */
	if (strchr(trigger, '\''))
	{
		char temp[80], *t;
		cptr s;
		for (s = trigger, t = temp; ; s++, t++)
		{
			/* tokenize() causes each ' to be followed by another character,
			 * and then another '. Trim the 's here. */
			if (*s == '\'')
			{
				*t = *(++s);
				s++;
			}
			else
			{
				*t = *s;
			}
			if (*t == '\0') break;
		}
		cli_ptr->comm = string_make(temp);
	}
	else
	{
		cli_ptr->comm = string_make(trigger);
	}

	/* First try copying everything across. */
	cli_ptr->key = num;
	cli_ptr->descrip = string_make(descr);

	/* Take description for the previous record if appropriate. */
	if ((cli_total > 0) && (old_ptr->key == cli_ptr->key) && (cli_ptr->descrip == 0))
	{
		cli_ptr->descrip = old_ptr->descrip;
	}

	/* Accept the macro. */
	if (cli_ptr->key && cli_ptr->comm && cli_ptr->descrip) cli_total++;
}



/*
 * Get a string using CLI completion.
 */
bool get_string_cli(cptr prompt, char *buf, s32b len)
{
	bool res;


	/* Paranoia XXX XXX XXX */
	msg_print(NULL);

	/* Display prompt */
	prt(prompt, 0, 0);

	/* Ask the user for a string */
	askfor_aux_complete = TRUE;
	res = askfor_aux(buf, len);
	askfor_aux_complete = FALSE;

	/* Clear prompt */
	prt("", 0, 0);

	/* Result */
	return (res);
}


/*
 * Do a command line command
 *
 * This is a wrapper around process command to provide a "reverse keymap"
 * whereby a set of keypresses is mapped to one.
 *
 * This is useful because command_cmd is a s16b, and so allows each command a
 * unique representation.
 *
 * See defines.h for a list of the codes used.
 */
void do_cmd_cli(void)
{
	char buff[80];

	cli_comm *cli_ptr;

	/* Clear the input buffer */
	strcpy(buff, "");

	/* Accept command */
	if (!get_string_cli("Command: ", buff, 30)) return;


	/* Analyse the input */
	for (cli_ptr = cli_info; cli_ptr->comm; cli_ptr++)
	{
		if (!strcmp(buff, cli_ptr->comm))
		{
			/* Process the command without keymaps or macros. */
			command_new = cli_ptr->key;
			return;
		}
	}

	msg_format("No such command: %s", buff);
}


/*
 * Display on-line help for the CLI commands
 */
void do_cmd_cli_help()
{
	s32b i, j;

	PHYSFS_file *fff;

	char file_name[1024];


	/* Temporary file */
	if (path_temp(file_name, 1024)) return;

	/* Open a new file */
	fff = my_fopen(file_name, "w");

	for (i = 0, j = -1; i < cli_total; i++)
	{
		if (j < i - 1) fprintf(fff, "/");
		fprintf(fff, "[[[[[G%s]", cli_info[i].comm);
		if (cli_info[i].descrip != cli_info[i + 1].descrip)
		{
			fprintf(fff, "   %s\n", cli_info[i].descrip);
			j = i;
		}
	}

	/* Close the file */
	my_fclose(fff);

	/* Enter "icky" mode */
	character_icky++;

	/* Save the screen */
	Term_save();

	/* Display the file contents */
	show_file(file_name, "Command line help", 0, 0);

	/* Restore the screen */
	Term_load();

	/* Leave "icky" mode */
	character_icky--;

	/* Remove the file */
	fd_kill(file_name);
}


/*
 * Dump screen shot in HTML
 */
void do_cmd_html_dump()
{
	char tmp_val[81];
	bool html = TRUE;
	term_win *save;

	/* Save the screen */
	save = Term_save_to();

	if (wizard && get_check("WIZARD MODE: Do an help file dump?"))
		html = FALSE;

	/* Ask for a file */
	if (html)
	{
		strcpy(tmp_val, "dummy.htm");
		if (!get_string("File(you can post it to http://angband.oook.cz/): ", tmp_val, 80))
		{
			/* Now restore the screen to initial state */
			Term_load_from(save, TRUE);
			Term_fresh();
			return;
		}
	}
	else
	{
		strcpy(tmp_val, "dummy.txt");
		if (!get_string("File: ", tmp_val, 80))
		{
			/* Now restore the screen to initial state */
			Term_load_from(save, TRUE);
			Term_fresh();
			return;
		}
	}

	/* Now restore the screen to dump it */
	Term_load_from(save, TRUE);

	if (html)
		html_screenshot(tmp_val);
	else
		help_file_screenshot(tmp_val);

	Term_erase(0, 0, 255);
	msg_print("Dump saved.");
	Term_fresh();
	fix_message();
}
