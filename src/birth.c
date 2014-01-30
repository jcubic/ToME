
/* File: birth.c */

/* Purpose: create a player character */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"


void print_desc_aux(cptr txt, s32b y, s32b xx)
{
	s32b i = -1, x = xx;


	while (txt[++i] != 0)
	{
		if (txt[i] == '\n')
		{
			x = xx;
			y++;
		}
		else
		{
			Term_putch(x++, y, TERM_YELLOW, txt[i]);
		}
	}
}

void print_desc(cptr txt)
{
	print_desc_aux(txt, 12, 1);
}

/*
 * Initialize a random town
 */
void init_town(s32b t_idx)
{
	town_type *t_ptr = &town_info[t_idx];

	/* Mark it as existent */
	t_ptr->flags |= (TOWN_REAL);

	/* Mark it as not found */
	t_ptr->flags &= ~(TOWN_KNOWN);

	/* Generation seed for the town */
	t_ptr->seed = randint(0x10000000);

	/* Total hack and not even used */
	t_ptr->numstores = 8;
}

/*
 * Clear all the global "character" data
 */
static void player_wipe(bool quickstart)
{
	s32b i, j;

	/* zero the struct */
	if (!quickstart)
	{
		p_ptr = WIPE(p_ptr, player_type);

		prepare_inventory(&p_ptr->inventory);

		/* No items */
		inven_cnt = 0;
		equip_cnt = 0;
	}

	/* Restore the powers */
	flag_empty(&p_ptr->powers);
	flag_empty(&p_ptr->powers_mod);

	/* Be sure to not spew errors */
	p_ptr->combat_style = SKILL_MASTERY;

	/* Not dead yet */
	p_ptr->lives = 0;

	/* Wipe the history */
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 60; j++)
		{
			if (j < 59) history[i][j] = ' ';
			else history[i][j] = '\0';
		}
	}

	/* Wipe the towns */
	for (i = 0; i < max_d_idx; i++)
	{
		for (j = 0; j < MAX_DUNGEON_DEPTH; j++)
		{
			special_lvl[j][i] = 0;
		}
	}

	/* Wipe the towns */
	for (i = max_real_towns + 1; i < max_towns; i++)
	{
		town_info[i].flags = 0;
	}

	/* Wipe the stores in every towns */
	for (i = 0; i < max_towns; i++)
	{
		flag_empty(&town_info[i].stores);
	}

	/* Wipe the quests */
	for (i = 0; i < MAX_Q_IDX_INIT; i++)
	{
		quest[i].status = QUEST_STATUS_UNTAKEN;
		for (j = 0; j < 4; j++)
		{
			quest[i].data[j] = 0;
		}
	}

	/* Start with no artifacts made yet */
	for (i = 0; i < max_a_idx; i++)
	{
		artifact_type *a_ptr = &a_info[i];
		a_ptr->cur_num = 0;
	}

	/* Reset the "objects" */
	for (i = 1; i < max_k_idx; i++)
	{
		object_kind *k_ptr = &k_info[i];

		/* Reset "tried" */
		k_ptr->tried = FALSE;

		/* Reset "aware" */
		k_ptr->aware = FALSE;

		/* Reset "know" */
		k_ptr->know = FALSE;

		/* Reset "artifact" */
		k_ptr->artifact = 0;
	}


	/* Reset the "monsters" */
	for (i = 1; i < max_r_idx; i++)
	{
		monster_race *r_ptr = &r_info[i];

		/* Reset the counter */
		r_ptr->cur_num = 0;

		/* Reset the max counter */
		r_ptr->max_num = 100;

		/* Reset the max counter */
		if (has_flag(r_ptr, FLAG_UNIQUE))
			r_ptr->max_num = get_flag(r_ptr, FLAG_UNIQUE);

		/* Clear player kills */
		r_ptr->r_pkills = 0;
	}


	/* Clear "cheat" options */
	cheat_peek = FALSE;
	cheat_hear = FALSE;
	cheat_room = FALSE;
	cheat_xtra = FALSE;
	cheat_know = FALSE;
	cheat_live = FALSE;

	/* Assume no winning game */
	total_winner = 0;
	has_won = FALSE;

	/* Assume no panic save */
	panic_save = 0;

	/* Assume no cheating */
	noscore = 0;
	wizard = 0;

	/* Clear the fate */
	p_ptr->no_mortal = FALSE;

	/* Player don't have the black breath from the beginning !*/
	p_ptr->black_breath = FALSE;

	/* Default pet command settings */
	p_ptr->pet_follow_distance = 6;
	p_ptr->pet_open_doors = FALSE;
	p_ptr->pet_pickup_items = FALSE;

	/* Body changing initialisation */
	monster_prep(&p_ptr->body_monster, 0, 0);

	/* Wipe spells */
	p_ptr->xtra_spells = 0;

	/* Wipe xtra hp */
	p_ptr->hp_mod = 0;

	/* Wipe the monsters */
	wipe_m_list();

	/* Wipe the doppleganger */
	doppleganger = 0;

	/* Wipe the recall depths */
	for (i = 0; i < max_d_idx; i++)
	{
		max_dlv[i] = 0;
	}

	/* Reset wild_mode to FALSE */
	p_ptr->wild_mode = FALSE;
	p_ptr->old_wild_mode = FALSE;

	/* Initialize allow_one_death */
	p_ptr->allow_one_death = 0;

	/* No companions killed */
	p_ptr->companion_killed = 0;
}

void player_birth_skill()
{
	s32b i;

	/* Show all abilities by default, modules will hide then as needed */
	for (i = 0; i < max_ab_idx; i++)
		ab_info[i].hidden = FALSE;

	/* Finish skills */
	p_ptr->skill_points = 0;
	p_ptr->skill_last_level = 1;
	for (i = 1; i < max_s_idx; i++)
		s_info[i].dev = FALSE;
	for (i = 1; i < max_s_idx; i++)
	{
		s32b value = 0, mod = 0;

		compute_skills(&value, &mod, i);

		init_skill(value, mod, i);

		/* Develop only revelant branches */
		if (get_skill(i) || s_info[i].mod)
		{
			s32b z = s_info[i].father;

			while (z != -1)
			{
				s_info[z].dev = TRUE;
				z = s_info[z].father;
				if (z == 0)
					break;
			}
		}
	}

	recalc_skills(FALSE);

	/* grab level 1 abilities */
	for (i = 0; i < max_ab_idx; i++)
		ab_info[i].acquired = FALSE;
	apply_level_abilities(1);

	/* Select the default combat type */
	select_default_combat();
}

/*
 * Create a new character.
 *
 * Note that we may be called with "junk" leftover in the various
 * fields, so we must be sure to clear them first.
 */
void player_birth(bool new_game)
{
	s32b i, j, rtown = TOWN_RANDOM;

	if (new_game)
	{
		/* Create a new character */
		while (1)
		{
			s32b res = FALSE;

			/* Wipe the player */
			player_wipe(FALSE);

			/* Roll up a new character */
			call_lua("birth.be_born", "()", "d", &res);
			if (res) break;
		}
	}
	else
		player_wipe(TRUE);

	/*
	 * Wipe saved levels We do it now because previously we might not
	 * have had a valid savefile name yet
	 */
	wipe_saved();

	/* Make a note file if that option is set */
	if (take_notes)
	{
		add_note_type(NOTE_BIRTH);
	}

	/* Note player birth in the message recall */
	message_add(MESSAGE_MSG, " ", TERM_L_BLUE);
	message_add(MESSAGE_MSG, "  ", TERM_L_BLUE);
	message_add(MESSAGE_MSG, "====================", TERM_L_BLUE);
	message_add(MESSAGE_MSG, "  ", TERM_L_BLUE);
	message_add(MESSAGE_MSG, " ", TERM_L_BLUE);

	/* Hack -- outfit the player */
//	player_outfit();

	/* Initialize random towns in the dungeons */
	for (i = 0; i < max_d_idx; i++)
	{
		dungeon_info_type *d_ptr = &d_info[i];
		s32b num = 0, z;

		d_ptr->t_num = 0;
		for (z = 0; z < TOWN_DUNGEON; z++)
		{
			d_ptr->t_idx[z] = 0;
			d_ptr->t_level[z] = 0;
		}
		if (!has_flag(d_ptr, FLAG_RANDOM_TOWNS)) continue;

		/* Can we add a town ? */
		while (magik(TOWN_CHANCE - (num * 10)))
		{
			s32b lev;

			d_ptr->t_idx[num] = rtown;
			rtown++;

			while (TRUE)
			{
				s32b j;
				bool ok = TRUE;

				lev = rand_range(d_ptr->mindepth, d_ptr->maxdepth - 1);

				/* Be sure it wasnt already used */
				for (j = 0; j < num; j++)
				{
					if (d_ptr->t_level[j] == lev) ok = FALSE;
				}

				/* Ok found one */
				if (ok) break;
			}
			d_ptr->t_level[num] = lev;

			if (wizard) message_add(MESSAGE_MSG, format("Random dungeon town: d_idx:%d, lev:%d", i, lev), TERM_WHITE);

			/* Create the town */
			init_town(d_ptr->t_idx[num]);

			num++;

			/* No free slots left */
			if (num >= TOWN_DUNGEON) break;
		}

		d_ptr->t_num = num;
	}

	/* Init the towns */
	for (i = 1; i < max_towns; i++)
	{
		/* Not destroyed ! yet .. ;) */
		town_info[i].destroyed = FALSE;

		/* Ignore non-existent towns */
		if (!(town_info[i].flags & (TOWN_REAL))) continue;
	}

	/* Init wilderness seeds */
	for (i = 0; i < max_wild_x; i++)
	{
		for (j = 0; j < max_wild_y; j++)
		{
			wild_map[j][i].seed = rand_int(0x10000000);
			wild_map[j][i].entrance = 0;
			wild_map[j][i].known = FALSE;
		}
	}

	/* Init the wilderness(to place the player) */
	call_lua("load_map", "(s)", "", "wild/world.map");
}


char savefile_module[46][80];
char savefile_names[46][30];
char savefile_desc[46][80];
bool savefile_alive[46];
s32b savefile_idx[46];

/*
 * Grab all the names from an index
 */
s32b load_savefile_names()
{
	PHYSFS_file *fff;
	char buf[1024];
	char tmp[50];
	char player_base_save[32];
	s32b max = 0;


	/* Build the filename */
#ifdef SAVEFILE_USE_UID
	strnfmt(tmp, 50, "user.%d.svg", player_uid);
#else
	strcpy(tmp, "global.svg");
#endif /* SAVEFILE_USE_UID */
	path_build(buf, 1024, TENGINE_DIR_SAVE, tmp);

	/* Read the file */
	fff = my_fopen(buf, "r");

	/* Failure */
	if (!fff) return (0);


	/* Save the current 'player_base' */
	strncpy(player_base_save, player_base, 32);


	/*
	 * Parse, use '@' intead of ':' as a separator because it cannot exists
	 * in savefiles
	 */
	while (0 == my_fgets(fff, buf, 1024))
	{
		s32b i = 0, start, count;

		/* Check for pre-ToME 2.1.2 file */
		count = 0;
		i = 0;
		while (buf[i] && buf[i] != '\n')
		{
			if (buf[i] == '@')
				++count;
			++i;
		}

		/* Check module if a current svg file */
		start = 0;
		i = 0;
		if (count > 1)
		{
			while (buf[i] != '@')
			{
				savefile_module[max][i - start] = buf[i];
				i++;
			}
			savefile_module[max][i] = '\0';
			i++;
		}
		/* Default to ToME for old files */
		else
		{
			savefile_module[max][0] = 'T';
			savefile_module[max][1] = 'o';
			savefile_module[max][2] = 'M';
			savefile_module[max][3] = 'E';
			savefile_module[max][4] = '\0';
		}

		if (buf[i] == '0') savefile_alive[max] = FALSE;
		else if (buf[i] == '1') savefile_alive[max] = TRUE;

		i++;
		start = i;
		while (buf[i] != '@')
		{
			savefile_names[max][i - start] = buf[i];
			i++;
		}
		savefile_names[max][i - start] = '\0';
		i++;
		strcpy(savefile_desc[max], buf + i);

		/* Build platform-dependent savefile name */
		strncpy(player_base, savefile_names[max], 32);
		process_player_name(TRUE);

		/* Try to open the savefile */
		if (file_exist(savefile)) max++;
	}

	my_fclose(fff);

	/* Restore the values of 'player_base' and 'savefile' */
	strncpy(player_base, player_base_save, 32);
	process_player_name(TRUE);

	return (max);
}


/*
 * Save all the names from an index
 */
void save_savefile_names()
{
	PHYSFS_file *fff;
	char buf[1024];
	char tmp[50];
	s32b max = load_savefile_names(), i;


	/* Build the filename */
#ifdef SAVEFILE_USE_UID
	strnfmt(tmp, 50, "user.%d.svg", player_uid);
#else
	strcpy(tmp, "global.svg");
#endif /* SAVEFILE_USE_UID */
	path_build(buf, 1024, TENGINE_DIR_SAVE, tmp);

	/* Read the file */
	fff = my_fopen(buf, "w");

	/* Failure */
	if (!fff) return;

	/*
	 * Save, use '@' intead of ':' as a separator because it cannot exists
	 * in savefiles
	 */
	fprintf(fff, "%s@%c%s@%s, is %s\n", game_module,
	        (death) ? '0' : '1', player_base, player_name,
	        (!death) ? "alive" : "dead");

	for (i = 0; i < max; i++)
	{
		if (!strcmp(savefile_names[i], player_base)) continue;
		fprintf(fff, "%s@%c%s@%s\n", savefile_module[i],
		        (savefile_alive[i]) ? '1' : '0', savefile_names[i], savefile_desc[i]);
	}

	my_fclose(fff);
}


static void dump_savefiles(s32b sel, s32b max)
{
	s32b i;

	char buf[40], pre = ' ', post = ')';

	char ind;


	for (i = 0; i < max; i++)
	{
		ind = I2A(i % 26);
		if (i >= 26) ind = toupper(ind);

		if (sel == i)
		{
			pre = '[';
			post = ']';
		}
		else
		{
			pre = ' ';
			post = ')';
		}

		if (i == 0) strnfmt(buf, 40, "%c%c%c New Character", pre, ind, post);
		else if (i == 1) strnfmt(buf, 40, "%c%c%c Load Savefile", pre, ind, post);
		else strnfmt(buf, 40, "%c%c%c %s", pre, ind, post, savefile_names[savefile_idx[i - 2]]);

		if (sel == i)
		{
			if (i >= 2)
			{
				if (savefile_alive[i - 2]) c_put_str(TERM_L_GREEN, savefile_desc[savefile_idx[i - 2]], 5, 0);
				else c_put_str(TERM_L_RED, savefile_desc[savefile_idx[i - 2]], 5, 0);
			}
			else if (i == 1) c_put_str(TERM_YELLOW, "Load an existing savefile that is not in the list", 5, 0);
			else c_put_str(TERM_YELLOW, "Create a new character", 5, 0);
			c_put_str(TERM_L_BLUE, buf, 8 + (i / 4), 20 * (i % 4));
		}
		else
			put_str(buf, 8 + (i / 4), 20 * (i % 4));
	}
}


/* Asks for new game or load game */
bool no_begin_screen = FALSE;

bool begin_screen(bool *quickstart)
{
	s32b m, k, sel, max;

savefile_try_again:
	sel = 0;

	/* Hack */
	use_color = TRUE;

	/* Grab the savefiles */
	max = load_savefile_names();

	/* Get only the usable savefiles */
	for (k = 0, m = 0; k < max; k++)
	{
		s32b can_use;

		call_lua("module_savefile_loadable", "(s)", "d", savefile_module[k], &can_use);
		if (can_use)
		{
			savefile_idx[m++] = k;
		}
	}
	max = m + 2;
	if (max > 2) sel = 2;

	while (TRUE)
	{
		/* Clear screen */
		Term_clear();

		/* Let the user choose */
		c_put_str(TERM_YELLOW, format("Welcome to %s!  To play you will need a character.", game_module), 1, 10);
		put_str("Press 8/2/4/6 to move, Return to select, Backspace to delete a savefile.", 3, 3);
		put_str("and Esc to quit.", 4, 32);

		dump_savefiles(sel, max);

		k = inkey();

		if (k == ESCAPE)
		{
			quit(NULL);
		}
		if (k == '6')
		{
			sel++;
			if (sel >= max) sel = 0;
			continue;
		}
		else if (k == '4')
		{
			sel--;
			if (sel < 0) sel = max - 1;
			continue;
		}
		else if (k == '2')
		{
			sel += 4;
			if (sel >= max) sel = sel % max;
			continue;
		}
		else if (k == '8')
		{
			sel -= 4;
			if (sel < 0) sel = (sel + max - 1) % max;
			continue;
		}
		else if (k == '\r')
		{
			if (sel < 26) k = I2A(sel);
			else k = toupper(I2A(sel));
		}
		else if (((k == 0x7F) || (k == '\010')) && (sel >= 2))
		{
			char player_base_save[32];

			if (!get_check(format("Really delete '%s'?", savefile_names[savefile_idx[sel - 2]]))) continue;

			/* Save current 'player_base' */
			strncpy(player_base_save, player_base, 32);

			/* Build platform-dependent save file name */
			strncpy(player_base, savefile_names[savefile_idx[sel - 2]], 32);
			process_player_name(TRUE);

			/* Remove the savefile */
			delete_savefile_dir(savefile);

			/* Restore 'player_base' and 'savefile' */
			strncpy(player_base, player_base_save, 32);
			process_player_name(TRUE);

			/* Reload, gods I hate using goto .. */
			goto savefile_try_again;

			continue;
		}

		if (k == 'a')
		{
			/* Display prompt */
			prt("Enter the name of the savefile that will hold this character: ", 23, 0);

			/* Ask the user for a string */
			if (!askfor_aux(player_base, 15)) continue;

			/* Process the player name */
			process_player_name(TRUE);

			return (TRUE);
		}
		if (k == 'b')
		{
			/* Display prompt */
			prt("Enter the name of a savefile: ", 23, 0);

			/* Ask the user for a string */
			if (!askfor_aux(player_base, 15)) continue;

			/* Process the player name */
			process_player_name(TRUE);

			return (FALSE);
		}
		else
		{
			s32b x;
			char buf[1024];

			if (islower(k)) x = A2I(k);
			else x = A2I(tolower(k)) + 26;

			if ((x < 2) || (x >= max)) continue;

			strnfmt(player_base, 128, "%s", savefile_names[savefile_idx[x - 2]]);

			/* Process the player name */
			process_player_name(TRUE);

			/* Check for quickstart */
			if (!savefile_alive[x - 2])
			{
				path_build(buf, 1024, savefile, "quickstart");
				if (file_exist(buf))
				{
					s32b wid, hgt;

					Term_get_size(&wid, &hgt);
					if (msg_box("Use quickstart? (y/n)", hgt / 2, wid / 2) == 'y')
					{
						*quickstart = TRUE;
					}
				}
			}


			return (FALSE);
		}
	}

	/* Shouldnt happen */
	return (FALSE);
}

