/* File: wizard2.c */

/* Purpose: Wizard commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include <math.h>

void do_cmd_wizard_body(s16b);
extern void status_main(void);

/*
 * Adds a lvl to a monster
 */
void wiz_inc_monster_level(s32b level)
{
	monster_type *m_ptr;
	s32b ii, jj;

	if (!tgt_pt(&ii, &jj)) return;

	if (cave[jj][ii].m_idx)
	{
		m_ptr = get_monster(cave[jj][ii].m_idx);

		m_ptr->exp = MONSTER_EXP(m_ptr->level + level);
		monster_check_experience(cave[jj][ii].m_idx, FALSE);
	}
}

void wiz_align_monster(s32b status)
{
	monster_type *m_ptr;
	s32b ii, jj;

	if (!tgt_pt(&ii, &jj)) return;

	if (cave[jj][ii].m_idx)
	{
		u32b old_faction;
		m_ptr = get_monster(cave[jj][ii].m_idx);

		old_faction = m_ptr->faction;

		if (old_faction == status)
			return;

		m_ptr->faction = status;

		process_hooks(HOOK_MON_CHANGE_FACTION_POST, "(M,d,d)",
					  m_ptr, old_faction, status);
	}
}

/*
 * Teleport directly to a town
 */
void teleport_player_town()
{
	s32b x = 0, y = 0;
	s32b town = 0; /* XXXXXXX */

	if (autosave_l)
	{
		is_autosave = TRUE;
		msg_print("Autosaving the game...");
		do_cmd_save_game();
		is_autosave = FALSE;
	}

	/* Change town */
	dun_level = 0;
	p_ptr->town_num = town;

	for (x = 0; x < max_wild_x; x++)
		for (y = 0; y < max_wild_y; y++)
			if (p_ptr->town_num == wf_info[wild_map[y][x].feat].entrance) goto finteletown;
finteletown:
	p_ptr->wilderness_y = y;
	p_ptr->wilderness_x = x;

	p_ptr->inside_arena = 0;
	leaving_quest = p_ptr->inside_quest;
	p_ptr->inside_quest = 0;

	/* Leaving */
	p_ptr->leaving = TRUE;
}


/*
 * Hack -- Rerate Hitpoints
 */
void do_cmd_rerate(void)
{
	s32b min_value, max_value, i, percent;

	min_value = (PY_MAX_LEVEL * 3 * (p_ptr->hitdie - 1)) / 8;
	min_value += PY_MAX_LEVEL;

	max_value = (PY_MAX_LEVEL * 5 * (p_ptr->hitdie - 1)) / 8;
	max_value += PY_MAX_LEVEL;

	player_hp[0] = p_ptr->hitdie;

	/* Rerate */
	while (1)
	{
		/* Collect values */
		for (i = 1; i < PY_MAX_LEVEL; i++)
		{
			player_hp[i] = randint(p_ptr->hitdie);
			player_hp[i] += player_hp[i - 1];
		}

		/* Legal values */
		if ((player_hp[PY_MAX_LEVEL - 1] >= min_value) &&
		                (player_hp[PY_MAX_LEVEL - 1] <= max_value)) break;
	}

	percent = (s32b)(((long)player_hp[PY_MAX_LEVEL - 1] * 200L) /
	                (p_ptr->hitdie + ((PY_MAX_LEVEL - 1) * p_ptr->hitdie)));

	/* Update and redraw hitpoints */
	p_ptr->update |= (PU_HP);
	flag_bool(&p_ptr->redraw, FLAG_PR_HP);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_PLAYER);

	/* Handle stuff */
	handle_stuff();

	/* Message */
	msg_format("Current Life Rating is %d/100.", percent);
}


#ifdef ALLOW_WIZARD


/*
 * Hack -- quick debugging hook
 */
void do_cmd_wiz_hack_ben(s32b num)
{
	char buf[1000];
	PHYSFS_file *fff;

	fff = my_fopen("/data/test.txt", "r");

	if (!fff)
		return;

	my_fgets(fff, buf, 1024);
	printf("buf: %s\n", buf);
	my_fclose(fff);
}

void do_cmd_lua_script()
{
	char script[80] = "tome_dofile_anywhere(TENGINE_DIR_SCPT, 'gen_idx.lua')";

	if (!get_string("Script:", script, 80)) return;

	exec_lua(script);

	/* Success */
	return;
}


/* Summon a horde of monsters */
static void do_cmd_summon_horde()
{
	s32b wy = p_ptr->py, wx = p_ptr->px;
	s32b attempts = 1000;

	while (--attempts)
	{
		scatter(&wy, &wx, p_ptr->py, p_ptr->px, 3, 0);
		if (cave_naked_bold(wy, wx)) break;
	}

	(void)alloc_horde(wy, wx);
}

/*
 * Aux function for "do_cmd_wiz_change()".	-RAK-
 */
static void do_cmd_wiz_change_aux(void)
{
	s32b   i;
	s32b   tmp_int;
	short tmp_short;
	long  tmp_long;
	char  tmp_val[160];
	char  ppp[80];
	long  next_lev_exp, prev_lev_exp, new_exp;

	/* Query the stats */
	for (i = 0; i <= STATS_MAX; i++)
	{
		/* Prompt */
		sprintf(ppp, "%s:  (0-%d): ", stat_names[i], MAX_STAT_NATURAL);

		/* Default */
		sprintf(tmp_val, "%ld", (s32b) get_stat(i, max));

		/* Query */
		if (!get_string(ppp, tmp_val, 3)) return;

		/* Extract */
		tmp_int = atoi(tmp_val);

		/* Verify */
		if (tmp_int > 18 + 100) tmp_int = 18 + 100;
		else if (tmp_int < 3) tmp_int = 3;

		/* Save it */
		set_stat(i, cur, tmp_int);
		set_stat(i, max, tmp_int);
	}


	/* Default */
	sprintf(tmp_val, "%ld", (long)(p_ptr->au));

	/* Query */
	if (!get_string("Gold: ", tmp_val, 9)) return;

	/* Extract */
	tmp_long = atol(tmp_val);

	/* Verify */
	if (tmp_long < 0) tmp_long = 0L;

	/* Save */
	p_ptr->au = tmp_long;


	/* Default */
	sprintf(tmp_val, "%ld", (s32b)(p_ptr->lev));

	/* Query */
	if (get_string("Level: ", tmp_val, 5)) {
		/*
		 * This is a two step process (set experience for a given
		 * level, then tweak the expereince), so don't bail if
		 * the user enter's nothing for leve.
		 */

		/* Extract */
		tmp_short = atoi(tmp_val);

		/* Verify */
		if (tmp_short < 1) tmp_short = 1;
		if (tmp_short > PY_MAX_LEVEL) tmp_short = PY_MAX_LEVEL;

		if (tmp_short != p_ptr->lev)
		{
			if (tmp_short == 1)
				p_ptr->max_exp = 0;
			else
				p_ptr->max_exp = player_exp[tmp_short - 2] *
					p_ptr->expfact / 100L;

			p_ptr->exp     = p_ptr->max_exp;

			/* Update */
			check_experience();
		}
	}

	/* Default */
	tmp_val[0] = '\0';

	/* Query */
	next_lev_exp = player_exp[MIN(p_ptr->lev - 1, PY_MAX_LEVEL - 2)] *
		p_ptr->expfact / 100L;
	prev_lev_exp = player_exp[MAX(p_ptr->lev - 2, 0)]  *
		p_ptr->expfact / 100L - 1;

	if (p_ptr->lev == 1)
		sprintf(ppp, "Modify exp (%ld to lev 2): ",
				next_lev_exp - p_ptr->exp);
	else if (p_ptr->lev == PY_MAX_LEVEL)
		sprintf(ppp, "Modify exp (%ld to lev %d): ",
				prev_lev_exp - p_ptr->exp, p_ptr->lev - 1);
	else
		sprintf(ppp, "Modify exp (%ld to lev %d, %ld to lev %d): ",
				prev_lev_exp - p_ptr->exp, p_ptr->lev - 1,
				next_lev_exp - p_ptr->exp, p_ptr->lev + 1);

	if (!get_string(ppp, tmp_val, 9)) return;

	/* Verify */
	tmp_long = atol(tmp_val);
	new_exp = p_ptr->exp + tmp_long;

	if (new_exp < 0)
		new_exp = 0;
	else if (new_exp > PY_MAX_EXP)
		new_exp = PY_MAX_EXP;

	p_ptr->max_exp = new_exp;
	p_ptr->exp     = new_exp;

	/* Update */
	check_experience();

	/* Default */
	sprintf(tmp_val, "%d", p_ptr->luck_base);

	/* Query */
	if (!get_string("Luck(base): ", tmp_val, 3)) return;

	/* Extract */
	tmp_long = atol(tmp_val);

	/* Save */
	p_ptr->luck_base = tmp_long;
	p_ptr->luck_max = tmp_long;
}


/*
 * Change various "permanent" player variables.
 */
static void do_cmd_wiz_change(void)
{
	/* Interact */
	do_cmd_wiz_change_aux();

	/* Redraw everything */
	do_cmd_redraw();
}


/*
 * Wizard routines for creating objects		-RAK-
 * And for manipulating them!                   -Bernd-
 *
 * This has been rewritten to make the whole procedure
 * of debugging objects much easier and more comfortable.
 *
 * The following functions are meant to play with objects:
 * Create, modify, roll for them (for statistic purposes) and more.
 * The original functions were by RAK.
 * The function to show an item's debug information was written
 * by David Reeve Sward <sward+@CMU.EDU>.
 *                             Bernd (wiebelt@mathematik.hu-berlin.de)
 *
 * Here are the low-level functions
 * - wiz_display_item()
 *     display an item's debug-info
 * - wiz_create_itemtype()
 *     specify tval and sval (type and subtype of object)
 * - wiz_tweak_item()
 *     specify pval, +AC, +tohit, +todam
 *     Note that the wizard can leave this function anytime,
 *     thus accepting the default-values for the remaining values.
 *     pval comes first now, since it is most important.
 * - wiz_reroll_item()
 *     apply some magic to the item or turn it into an artifact.
 * - wiz_roll_item()
 *     Get some statistics about the rarity of an item:
 *     We create a lot of fake items and see if they are of the
 *     same type (tval and sval), then we compare pval and +AC.
 *     If the fake-item is better or equal it is counted.
 *     Note that cursed items that are better or equal (absolute values)
 *     are counted, too.
 *     HINT: This is *very* useful for balancing the game!
 * - wiz_quantity_item()
 *     change the quantity of an item, but be sane about it.
 *
 * And now the high-level functions
 * - do_cmd_wiz_play()
 *     play with an existing object
 * - wiz_create_item()
 *     create a new object
 *
 * Note -- You do not have to specify "pval" and other item-properties
 * directly. Just apply magic until you are satisfied with the item.
 *
 * Note -- For some items (such as wands, staffs, some rings, etc), you
 * must apply magic, or you will get "broken" or "uncharged" objects.
 *
 * Note -- Redefining artifacts via "do_cmd_wiz_play()" may destroy
 * the artifact.  Be careful.
 *
 * Hack -- this function will allow you to create multiple artifacts.
 * This "feature" may induce crashes or other nasty effects.
 */

/*
 * Just display an item's properties (debug-info)
 * Originally by David Reeve Sward <sward+@CMU.EDU>
 * Verbose item flags by -Bernd-
 */
static void wiz_display_item(object_type *o_ptr)
{
	s32b i, j = 13;
	char buf[256];

	/* Clear the screen */
	for (i = 1; i <= 23; i++) prt("", i, j - 2);

	/* Describe fully */
	object_desc_store(buf, o_ptr, TRUE, 3);

	prt(buf, 2, j);

	prt(format("kind = %-5d  level = %-4d  tval = %-5d  sval = %-5d",
	           o_ptr->k_idx, k_info[o_ptr->k_idx].level,
	           o_ptr->tval, o_ptr->sval), 4, j);

	prt(format("number = %-3d  wgt = %-6d  ac = %-5d    damage = %dd%d",
	           o_ptr->number, o_ptr->weight,
	           o_ptr->ac, o_ptr->dd, o_ptr->ds), 5, j);

	prt(format("toac = %-5d  tohit = %-4d  todam = %-4d",
	           o_ptr->to_a, o_ptr->to_h, o_ptr->to_d), 6, j);

	prt(format("artifact_id = %-4d  cost = %ld  ",
	           o_ptr->artifact_id, (long)object_value(o_ptr)), 7, j);

	prt(format("ident = %04x  timeout = %-d",
	           o_ptr->ident, o_ptr->timeout), 8, j);
/*
	prt("+------------FLAGS1------------+", 10, j);
	prt("AFFECT........SLAY........BRAND.", 11, j);
	prt("              cvae      xsqpaefc", 12, j);
	prt("siwdcc  ssidsahanvudotgddhuoclio", 13, j);
	prt("tnieoh  trnipttmiinmrrnrrraiierl", 14, j);
	prt("rtsxna..lcfgdkcpmldncltggpksdced", 15, j);
	prt_binary(f1, 16, j);

	prt("+------------FLAGS2------------+", 17, j);
	prt("SUST....IMMUN.RESIST............", 18, j);
	prt("        aefcprpsaefcpfldbc sn   ", 19, j);
	prt("siwdcc  cliooeatcliooeialoshtncd", 20, j);
	prt("tnieoh  ierlifraierliatrnnnrhehi", 21, j);
	prt("rtsxna..dcedslatdcedsrekdfddrxss", 22, j);
	prt_binary(f2, 23, j);

	prt("+------------FLAGS3------------+", 10, j + 32);
	prt("fe      ehsi  st    iiiiadta  hp", 11, j + 32);
	prt("il   n taihnf ee    ggggcregb vr", 12, j + 32);
	prt("re  nowysdose eld   nnnntalrl ym", 13, j + 32);
	prt("ec  omrcyewta ieirmsrrrriieaeccc", 14, j + 32);
	prt("aa  taauktmatlnpgeihaefcvnpvsuuu", 15, j + 32);
	prt("uu  egirnyoahivaeggoclioaeoasrrr", 16, j + 32);
	prt("rr  litsopdretitsehtierltxrtesss", 17, j + 32);
	prt("aa  echewestreshtntsdcedeptedeee", 18, j + 32);
	prt_binary(f3, 19, j + 32);
*/
}

/*
 * Strip an "object name" into a buffer
 */
static void strip_name(char *out_str, cptr in_str)
{
	char *t;

	/* Skip past leadin_strg characters */
	while ((*in_str == ' ') || (*in_str == '&')) in_str++;

	/* Copy useful chars */
	for (t = out_str; *in_str; in_str++)
	{
		if (*in_str != '~') *t++ = *in_str;
	}

	/* Termin_strate the new name */
	*t = '\0';
}


/*
 * Hack -- title for each column
 *
 * XXX XXX XXX This will not work with "EBCDIC", I would think.
 */
static char head[4] = { 'a', 'A', '0', ':' };


/*
 * Print a string as required by wiz_create_itemtype().
 * Trims characters from the beginning until it fits in the space
 * before the next row or the edge of the screen.
 */
static void wci_string(cptr string, s32b num)
{
	s32b row = 2 + (num % 20), col = 30 * (num / 20);
	s32b ch = head[num / 20] + (num % 20), max_len = 0;

	char frmt_str[20];

	if (76-col < (signed)max_len)
		max_len = 76-col;
	else
		max_len = 30-6;

	/*if (strlen(string) > (unsigned)max_len)
		string = string + (strlen(string) - max_len);*/

	sprintf(frmt_str, "[%%c] %%.%lds", max_len);

	prt(format(frmt_str, ch, string), row, col);
}

/*
 * Specify tval and sval (type and subtype of object) originally
 * by RAK, heavily modified by -Bernd-
 *
 * This function returns the k_idx of an object type, or zero if failed
 *
 * List up to 50 choices in three columns
 */
static s32b wiz_create_itemtype(void)
{
	s32b max_tval;
 	s32b i, num, max_num;
	s32b tval, sval_start;

	cptr tval_desc2;
	s32b ch;

	s32b choice[255];
	s32b offset[255];

	char buf[160];


	/* Clear screen */
	Term_clear();

	call_lua("get_max_tval", "()", "d", &max_tval);

	/* Print all tval's and their descriptions */
	for (tval = i = 0; tval <= max_tval; tval++)
	{
		cptr name;
		s32b max_sval;
		s32b  off = 0;

		call_lua("__k_info.get_tval_info", "(s,d)", "s",
				 "name_and_symb", tval, &name);

		if (!name)
			continue;

		call_lua("get_max_sval", "(d)", "d", tval, &max_sval);

		/*
		 * One tval might have more entries than can be displayed on *
		 * a single screen.
		 */
		do {
			choice[i] = tval;
			offset[i] = off;
			wci_string(name, i++);

			off      += 57;
			max_sval -= 57;
		} while (max_sval > 0);
	}

 /* Me need to know the maximal possible choice number */
	max_num = i;

	/* Choose! */
	if (!get_com("Get what type of object? ", &ch)) return (0);

	if ((ch < 0) || (ch > 255))
		return (0);

	/* Analyze choice */
	num = -1;
	if ((ch >= head[0]) && (ch < head[0] + 20)) num = ch - head[0];
	if ((ch >= head[1]) && (ch < head[1] + 20)) num = ch - head[1] + 20;
	if ((ch >= head[2]) && (ch < head[2] + 17)) num = ch - head[2] + 40;

	/* Bail out if choice is illegal */
	if ((num < 0) || (num >= max_num)) return (0);

	/* Base object type chosen, fill in tval */
	tval = choice[num];
	sval_start = offset[num];

	/* tval_desc2; */
	call_lua("__k_info.get_tval_info", "(s,d)", "s", "name",
			 tval, &tval_desc2);

	/*** And now we go for k_idx ***/

	/* Clear screen */
	Term_clear();

	/* We have to search the whole itemlist. */
	for (num = 0, i = 1; (num < 57) && (i < max_k_idx); i++)
	{
		object_kind *k_ptr = &k_info[i];

		/* Analyze matching items */
		if (k_ptr->tval == tval && k_ptr->sval >= sval_start)
		{
			/* Hack -- Skip instant artifacts */
			if (has_flag(k_ptr, FLAG_INSTA_ART)) continue;

			/* Acquire the "name" of object "i" */
			strip_name(buf, k_ptr->name);

			/* Print it */
			wci_string(buf, num);

			/* Remember the object index */
			choice[num++] = i;
		}
	}

	/* Me need to know the maximal possible remembered object_index */
	max_num = num;

	/* Choose! */
	if (!get_com(format("What Kind of %s? ", tval_desc2), &ch)) return (0);

	if ((ch < 0) || (ch > 255))
		return (0);

	/* Analyze choice */
	num = -1;
	if ((ch >= head[0]) && (ch < head[0] + 20)) num = ch - head[0];
	if ((ch >= head[1]) && (ch < head[1] + 20)) num = ch - head[1] + 20;
	if ((ch >= head[2]) && (ch < head[2] + 17)) num = ch - head[2] + 40;

	/* Bail out if choice is "illegal" */
	if ((num < 0) || (num >= max_num)) return (0);

	/* And return successful */
	return (choice[num]);
}

/*
 * Create the artifact of the specified number -- DAN
 *
 */
static s32b wiz_create_named_art_type()
{
	s32b i, num, num_groups, offset, max_num;
	s32b choice[255];
	char buf[255];
	s32b ch;
	artifact_type *a_ptr;

	num_groups = (s32b) ceil( ( (double) max_a_idx) / 57.0 );

	/* Print all tval's and their descriptions */
	for (i = 0; i < num_groups; i++)
	{
		char group_name[80];

		sprintf(group_name, "Group %ld", i + 1);

		wci_string(group_name, i);
	}

	/* Choose! */
	if (!get_com("Create artifact from which group? ", &ch)) return(0);

	if ((ch < 0) || (ch > 255))
		return (0);

	/* Analyze choice */
	num = -1;
	if ((ch >= head[0]) && (ch < head[0] + 20)) num = ch - head[0];
	if ((ch >= head[1]) && (ch < head[1] + 20)) num = ch - head[1] + 20;
	if ((ch >= head[2]) && (ch < head[2] + 17)) num = ch - head[2] + 40;

	/* Bail out if choice is illegal */
	if ((num < 0) || (num >= num_groups)) return(0);

	offset = num * 57;

	/* Clear screen */
	Term_clear();

	/* We have to search the whole itemlist. */
	for (num = 0, i = 1; i <= 57 && (i + offset) < max_a_idx; i++)
	{
		a_ptr = &a_info[i + offset];

		/* Analyze matching items */
		if (a_ptr->name && strlen(a_ptr->name) > 0)
		{
			/* Print it */
			strip_name(buf, a_ptr->name);
			wci_string(buf, num);

			/* Remember the object index */
			choice[num++] = i + offset;
		}
	}

	/* Me need to know the maximal possible remembered object_index */
	max_num = num;

	/* Choose! */
	if (!get_com("Which artifact? ", &ch)) return(0);

	if ((ch < 0) || (ch > 255))
		return (0);

	/* Analyze choice */
	num = -1;
	if ((ch >= head[0]) && (ch < head[0] + 20)) num = ch - head[0];
	if ((ch >= head[1]) && (ch < head[1] + 20)) num = ch - head[1] + 20;
	if ((ch >= head[2]) && (ch < head[2] + 17)) num = ch - head[2] + 40;

	/* Bail out if choice is "illegal" */
	if ((num < 0) || (num >= max_num)) return(0);

	return (choice[num]);
}

static void wiz_create_named_art()
{
	object_type *q_ptr;
	s32b a_idx, i;
	artifact_type *a_ptr;

	Term_save();
	Term_clear();

	a_idx = wiz_create_named_art_type();

	Term_load();

	if (a_idx <= 0)
		return;

	a_ptr = &a_info[a_idx];

	/* Get local object */
	q_ptr = new_object();

	/* Wipe the object */
	object_wipe(q_ptr);

	/* Ignore "empty" artifacts */
	if (!a_ptr->name)
	{
		msg_print("IMPOSSIBLE: empty artifact");
		delete_object(q_ptr);
		return;
	}

#if 0
	/* Ignore generated artifacts */
	if (a_ptr->cur_num)
	{
		delete_object(q_ptr);
		return;
	}
#endif

	/* Acquire the "kind" index */
	i = lookup_kind(a_ptr->tval, a_ptr->sval);

	/* Oops */
	if (!i)
	{
		msg_print("OOPS: lookup_kind() failed");
		delete_object(q_ptr);
		return;
	}

	/* Create the artifact */
	object_prep(q_ptr, i);

	/* Save the name */
	q_ptr->artifact_id = a_idx;

	apply_magic(q_ptr, -1, TRUE, TRUE, TRUE);

	/* Artifacts can have a different color than their base object */
	if (a_ptr->d_attr != -1)
		q_ptr->d_attr = a_ptr->d_attr;

	if (a_ptr->x_attr != -1)
		q_ptr->x_attr = a_ptr->x_attr;

	/* Identify it fully */
/*	make_item_fully_identified(q_ptr); */

	/* Drop the artifact from heaven */
	q_ptr->found = OBJ_FOUND_DEBUG;
	drop_near(q_ptr, -1, p_ptr->py, p_ptr->px);

	/* All done */
	msg_print("Artifact created.");
}


/*
 * Change an item's egos, by name.
 */
static bool wiz_tweak_egos(object_type *o_ptr)
{
#if 0 // Sorry matthew, I'm breaking your new code :<
	cptr p;
	char tmp_val[80];
	bool found;
	s32b  i;
	s32b  prefix_ego = 0, suffix_ego = 0;

	if (o_ptr->ego_id != 0)
	{
		if (e_info[o_ptr->ego_id].before)
			prefix_ego = o_ptr->ego_id;
		else
			suffix_ego = o_ptr->ego_id;
	}

	if (o_ptr->ego_idb != 0)
	{
		if (e_info[o_ptr->ego_idb].before)
			prefix_ego = o_ptr->ego_idb;
		else
			suffix_ego = o_ptr->ego_idb;
	}

	do {
		p = "Enter new prefix ego : ";

		if (prefix_ego != 0)
			sprintf(tmp_val, "%s", e_info[prefix_ego].name);
		else
			sprintf(tmp_val, "%s", "");

		if (!get_string(p, tmp_val, 20))
			return TRUE;

		if (strlen(tmp_val) == 0) {
			prefix_ego = 0;
			break;
		}

		strlower(tmp_val);

		found = FALSE;
		for (i = 1; i < max_e_idx; i++)
			if (strcmp(tmp_val, e_info[i].name) == 0)
			{
				if (!e_info[i].before)
				{
					msg_format("'%s' is not a prefix ego", tmp_val);
					continue;
				}
				prefix_ego = i;
				found      = TRUE;
				break;
			}

		if (!found)
			msg_format("No such ego as '%s'", tmp_val);
	} while(!found);

	o_ptr->ego_id = prefix_ego;

	do {
		p = "Enter new suffix ego : ";

		if (suffix_ego != 0)
			sprintf(tmp_val, "%s", e_info[suffix_ego].name);
		else
			sprintf(tmp_val, "%s", "");

		if (!get_string(p, tmp_val, 20))
			return TRUE;

		if (strlen(tmp_val) == 0) {
			suffix_ego = 0;
			break;
		}

		strlower(tmp_val);

		found = FALSE;
		for (i = 1; i < max_e_idx; i++)
			if (strcmp(tmp_val, e_info[i].name) == 0)
			{
				if (e_info[i].before)
				{
					msg_format("'%s' is not a suffix ego", tmp_val);
					continue;
				}
				suffix_ego = i;
				found      = TRUE;
				break;
			}

		if (!found)
			msg_format("No such ego as '%s'", tmp_val);
	} while(!found);

	if (o_ptr->ego_id == 0)
		o_ptr->ego_id = suffix_ego;
	else
		o_ptr->ego_idb = suffix_ego;
#endif
	return FALSE;
}

/*
 * Tweak an item
 */
static void wiz_tweak_item(object_type *o_ptr)
{
	cptr p;
	char tmp_val[80];


#if 0 /* DG -- A Wizard can do whatever he/she wants */
	/* Hack -- leave artifacts alone */
	if (artifact_p(o_ptr) || o_ptr->art_name) return;
#endif

	p = "Enter new 'to_a' setting: ";
	sprintf(tmp_val, "%d", o_ptr->to_a);
	if (!get_string(p, tmp_val, 5)) return;
	o_ptr->to_a = atoi(tmp_val);
	wiz_display_item(o_ptr);

	p = "Enter new 'to_h' setting: ";
	sprintf(tmp_val, "%d", o_ptr->to_h);
	if (!get_string(p, tmp_val, 5)) return;
	o_ptr->to_h = atoi(tmp_val);
	wiz_display_item(o_ptr);

	p = "Enter new 'to_d' setting: ";
	sprintf(tmp_val, "%d", o_ptr->to_d);
	if (!get_string(p, tmp_val, 5)) return;
	o_ptr->to_d = atoi(tmp_val);
	wiz_display_item(o_ptr);

	/* Don't tweak egos if the item is an artifact. */
	if (!artifact_p(o_ptr))
		if (wiz_tweak_egos(o_ptr))
			return;

	p = "Enter new 'sval' setting: ";
	sprintf(tmp_val, "%d", o_ptr->sval);
	if (!get_string(p, tmp_val, 5)) return;
	o_ptr->sval = atoi(tmp_val);
	wiz_display_item(o_ptr);

	p = "Enter new 'obj exp' setting: ";
	sprintf(tmp_val, "%ld", o_ptr->exp);
	if (!get_string(p, tmp_val, 9)) return;
	wiz_display_item(o_ptr);
	o_ptr->exp = atoi(tmp_val);
	if (has_flag(o_ptr, FLAG_LEVELS)) check_experience_obj(o_ptr);

	p = "Enter new 'timeout' setting: ";
	sprintf(tmp_val, "%d", o_ptr->timeout);
	if (!get_string(p, tmp_val, 5)) return;
	o_ptr->timeout = atoi(tmp_val);
	wiz_display_item(o_ptr);
}


/*
 * Apply magic to an item or turn it into an artifact. -Bernd-
 */
static void wiz_reroll_item(object_type *o_ptr)
{
	object_type *q_ptr;

	s32b ch;

	bool changed = FALSE;


	/* Hack -- leave artifacts alone */
	if (artifact_p(o_ptr) || o_ptr->art_name) return;


	/* Get local object */
	q_ptr = new_object();

	/* Copy the object */
	object_copy(q_ptr, o_ptr);


	/* Main loop. Ask for magification and artifactification */
	while (TRUE)
	{
		/* Display full item debug information */
		wiz_display_item(q_ptr);

		/* Ask wizard what to do. */
		if (!get_com("[a]ccept, [b]ad, [n]ormal, [g]ood, [e]xcellent, [d]ouble ego? ", &ch))
		{
			changed = FALSE;
			break;
		}

		/* Create/change it! */
		if (ch == 'A' || ch == 'a')
		{
			changed = TRUE;
			break;
		}

		/* Apply bad magic, but first clear object */
		else if (ch == 'b' || ch == 'B')
		{
			object_prep(q_ptr, o_ptr->k_idx);
			hack_apply_magic_power = -2;
			apply_magic(q_ptr, dun_level, FALSE, FALSE, FALSE);
		}

		/* Apply normal magic, but first clear object */
		else if (ch == 'n' || ch == 'N')
		{
			object_prep(q_ptr, o_ptr->k_idx);
			apply_magic(q_ptr, dun_level, FALSE, FALSE, FALSE);
		}

		/* Apply good magic, but first clear object */
		else if (ch == 'g' || ch == 'G')
		{
			object_prep(q_ptr, o_ptr->k_idx);
			apply_magic(q_ptr, dun_level, FALSE, TRUE, FALSE);
		}

		/* Apply great magic, but first clear object */
		else if (ch == 'e' || ch == 'E')
                {
                        s32b ego;
                        object_prep(q_ptr, o_ptr->k_idx);
                        call_lua("rand_obj.get_single_ego", "(d,d,n)", "d", dun_level, o_ptr->k_idx, &ego);
                        if (ego) q_ptr->ego_id[0] = ego;
			apply_magic(q_ptr, dun_level, FALSE, TRUE, TRUE);
		}

		/* Apply great magic, but first clear object */
		else if (ch == 'd' || ch == 'D')
		{
                        s32b ego1, ego2;
                        object_prep(q_ptr, o_ptr->k_idx);
                        call_lua("rand_obj.get_double_ego", "(d,d,n)", "dd", dun_level, o_ptr->k_idx, &ego1, &ego2);
                        if (ego1) q_ptr->ego_id[0] = ego1;
                        if (ego2) q_ptr->ego_id[1] = ego2;
			apply_magic(q_ptr, dun_level, FALSE, TRUE, TRUE);
		}
	}


	/* Notice change */
	if (changed)
	{
		/* Apply changes */
		object_copy(o_ptr, q_ptr);

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Combine / Reorder the pack (later) */
		p_ptr->notice |= (PN_COMBINE | PN_REORDER);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_ALL);
	}

	delete_object(q_ptr);
}



/*
 * Maximum number of rolls
 */
#define TEST_ROLL 100000


/*
 * Try to create an item again. Output some statistics.    -Bernd-
 *
 * The statistics are correct now.  We acquire a clean grid, and then
 * repeatedly place an object in this grid, copying it into an item
 * holder, and then deleting the object.  We fiddle with the artifact
 * counter flags to prevent weirdness.  We use the items to collect
 * statistics on item creation relative to the initial item.
 */
static void wiz_statistics(object_type *o_ptr)
{
	long i, matches, better, worse, other;

	s32b ch;
	char *quality;

	bool good, great;

	object_type	*q_ptr;

	cptr q = "Rolls: %ld, Matches: %ld, Better: %ld, Worse: %ld, Other: %ld";

	/* XXX XXX XXX Mega-Hack -- allow multiple artifacts */
	if (artifact_p(o_ptr))
	{
		a_info[o_ptr->artifact_id].cur_num = 0;
	}


	/* Interact */
	while (TRUE)
	{
		cptr pmt = "Roll for [n]ormal, [g]ood, or [e]xcellent treasure? ";

		/* Display item */
		wiz_display_item(o_ptr);

		/* Get choices */
		if (!get_com(pmt, &ch)) break;

		if (ch == 'n' || ch == 'N')
		{
			good = FALSE;
			great = FALSE;
			quality = "normal";
		}
		else if (ch == 'g' || ch == 'G')
		{
			good = TRUE;
			great = FALSE;
			quality = "good";
		}
		else if (ch == 'e' || ch == 'E')
		{
			good = TRUE;
			great = TRUE;
			quality = "excellent";
		}
		else
		{
			good = FALSE;
			great = FALSE;
			break;
		}

		/* Let us know what we are doing */
		msg_format("Creating a lot of %s items. Base level = %d.",
		           quality, dun_level);
		msg_print(NULL);

		/* Set counters to zero */
		matches = better = worse = other = 0;

		/* Let's rock and roll */
		for (i = 0; i <= TEST_ROLL; i++)
		{
			/* Output every few rolls */
			if ((i < 100) || (i % 100 == 0))
			{
				/* Do not wait */
				inkey_scan = TRUE;

				/* Allow interupt */
				if (inkey())
				{
					/* Flush */
					flush();

					/* Stop rolling */
					break;
				}

				/* Dump the stats */
				prt(format(q, i, matches, better, worse, other), 0, 0);
				Term_fresh();
			}


			/* Create an object */
			q_ptr = make_object(good, great, NULL);

			/* XXX XXX XXX Mega-Hack -- allow multiple artifacts */
			if (artifact_p(q_ptr))
			{
				a_info[q_ptr->artifact_id].cur_num = 0;
			}


			/* Test for the same tval and sval. */
			if ((o_ptr->tval) != (q_ptr->tval))
			{
				delete_object(q_ptr);
				continue;
			}
			if ((o_ptr->sval) != (q_ptr->sval))
			{
				delete_object(q_ptr);
				continue;
			}

			/* Check for match */
			if (
			                (q_ptr->to_a == o_ptr->to_a) &&
			                (q_ptr->to_h == o_ptr->to_h) &&
			                (q_ptr->to_d == o_ptr->to_d))
			{
				matches++;
			}

			/* Check for better */
			else if (
			                (q_ptr->to_a >= o_ptr->to_a) &&
			                (q_ptr->to_h >= o_ptr->to_h) &&
			                (q_ptr->to_d >= o_ptr->to_d))
			{
				better++;
			}

			/* Check for worse */
			else if (
			                (q_ptr->to_a <= o_ptr->to_a) &&
			                (q_ptr->to_h <= o_ptr->to_h) &&
			                (q_ptr->to_d <= o_ptr->to_d))
			{
				worse++;
			}

			/* Assume different */
			else
			{
				other++;
			}

			delete_object(q_ptr);
		}

		/* Final dump */
		msg_format(q, i, matches, better, worse, other);
		msg_print(NULL);
	}


	/* Hack -- Normally only make a single artifact */
	if (artifact_p(o_ptr))
	{
		a_info[o_ptr->artifact_id].cur_num = 1;
	}
}


/*
 * Change the quantity of a the item
 */
static void wiz_quantity_item(object_type *o_ptr)
{
	s32b tmp_int, tmp_qnt;

	char tmp_val[100];


#if 0 /* DG -- A Wizard can do whatever he/she/it wants */
	/* Never duplicate artifacts */
	if (artifact_p(o_ptr) || o_ptr->art_name) return;
#endif

	tmp_qnt = o_ptr->number;

	/* Default */
	sprintf(tmp_val, "%d", o_ptr->number);

	/* Query */
	if (get_string("Quantity: ", tmp_val, 2))
	{
		/* Extract */
		tmp_int = atoi(tmp_val);

		/* Paranoia */
		if (tmp_int < 1) tmp_int = 1;
		if (tmp_int > 99) tmp_int = 99;

		/* Accept modifications */
		o_ptr->number = tmp_int;
	}
}



/*
 * Play with an item. Options include:
 *   - Output statistics (via wiz_roll_item)
 *   - Reroll item (via wiz_reroll_item)
 *   - Change properties (via wiz_tweak_item)
 *   - Change the number of items (via wiz_quantity_item)
 */
static void do_cmd_wiz_play(void)
{
	s32b item;

	object_type *q_ptr;

	object_type *o_ptr;

	s32b ch;

	bool changed;

	cptr q, s;

	/* Get an item */
	q = "Play with which object? ";
	s = "You have nothing to play with.";
	if (!get_item(&item, q, s, (USE_EQUIP | USE_INVEN | USE_FLOOR))) return;

	/* Get the item (in the pack) */
	o_ptr = get_object(item);

	/* The item was not changed */
	changed = FALSE;


	/* Icky */
	character_icky++;

	/* Save the screen */
	Term_save();


	/* Get local object */
	q_ptr = new_object();

	/* Copy object */
	object_copy(q_ptr, o_ptr);


	/* The main loop */
	while (TRUE)
	{
		/* Display the item */
		wiz_display_item(q_ptr);

		/* Get choice */
		if (!get_com("[a]ccept [s]tatistics [r]eroll [t]weak [q]uantity apply[m]agic? ", &ch))
		{
			changed = FALSE;
			break;
		}

		if (ch == 'A' || ch == 'a')
		{
			changed = TRUE;
			break;
		}

		if (ch == 's' || ch == 'S')
		{
			wiz_statistics(q_ptr);
		}

		if (ch == 'r' || ch == 'r')
		{
			wiz_reroll_item(q_ptr);
		}

		if (ch == 't' || ch == 'T')
		{
			wiz_tweak_item(q_ptr);
		}

		if (ch == 'q' || ch == 'Q')
		{
			wiz_quantity_item(q_ptr);
		}

		if (ch == 'm' || ch == 'M')
		{
#if 0 // DGDGDGDG
			s32b e = q_ptr->ego_id, eb = q_ptr->ego_idb;

			object_prep(q_ptr, q_ptr->k_idx);
			q_ptr->ego_id = e;
			q_ptr->ego_idb = eb;
			apply_magic(q_ptr, dun_level, FALSE, FALSE, FALSE);
#endif
		}
	}


	/* Restore the screen */
	Term_load();

	/* Not Icky */
	character_icky--;


	/* Accept change */
	if (changed)
	{
		/* Message */
		msg_print("Changes accepted.");

		/* Change */
		object_copy(o_ptr, q_ptr);

		/* Recalculate bonuses */
		p_ptr->update |= (PU_BONUS);

		/* Combine / Reorder the pack (later) */
		p_ptr->notice |= (PN_COMBINE | PN_REORDER);

		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_ALL);
	}

	/* Ignore change */
	else
	{
		msg_print("Changes ignored.");
	}

	delete_object(q_ptr);
}


/*
 * Wizard routine for creating objects		-RAK-
 * Heavily modified to allow magification and artifactification  -Bernd-
 *
 * Note that wizards cannot create objects on top of other objects.
 *
 * Hack -- this routine always makes a "dungeon object", and applies
 * magic to it, and attempts to decline cursed items.
 */
static void wiz_create_item(void)
{
	object_type *q_ptr;

	s32b k_idx;


	/* Icky */
	character_icky++;

	/* Save the screen */
	Term_save();

	/* Get object base type */
	k_idx = wiz_create_itemtype();

	/* Restore the screen */
	Term_load();

	/* Not Icky */
	character_icky--;


	/* Return if failed */
	if (!k_idx) return;

	/* Get local object */
	q_ptr = new_object();

	/* Create the item */
	object_prep(q_ptr, k_idx);

	/* Apply magic (no messages, no artifacts) */
	apply_magic(q_ptr, dun_level, FALSE, FALSE, FALSE);

	/* Drop the object from heaven */
	q_ptr->found = OBJ_FOUND_DEBUG;
	drop_near(q_ptr, -1, p_ptr->py, p_ptr->px);

	/* All done */
	msg_print("Allocated.");
}

/*
 * As above, but takes the k_idx as a parameter instead of using menus.
 */
static void wiz_create_item_2(void)
{
	object_type *q_ptr;
	s32b a_idx;
	cptr p = "Number of the object :";
	char out_val[80] = "";

	if (!get_string(p, out_val, 4)) return;
	a_idx = atoi(out_val);

	/* Return if failed or out-of-bounds */
	if ((a_idx <= 0) || (a_idx >= max_k_idx)) return;

	/* Get local object */
	q_ptr = new_object();

	/* Create the item */
	object_prep(q_ptr, a_idx);

	/* Apply magic (no messages, no artifacts) */
	apply_magic(q_ptr, dun_level, FALSE, FALSE, FALSE);

	/* Drop the object from heaven */
	q_ptr->found = OBJ_FOUND_DEBUG;
	drop_near(q_ptr, -1, p_ptr->py, p_ptr->px);

	/* All done */
	msg_print("Allocated.");
}


/*
 * Cure everything instantly
 */
void do_cmd_wiz_cure_all(void)
{
	s32b i;

	/* Remove curses */
	(void)remove_all_curse();

	/* Restore stats */
	for (i = 0; i <= STATS_MAX; i++)
		(void)res_stat(i, TRUE);

	/* Restore the level */
	(void)restore_level();

	/* Cure stuff */
	call_lua("timed_effect.reset", "()", "");
	call_lua("counter.reset", "()", "");
	p_ptr->black_breath = FALSE;

	/* Redraw everything */
	do_cmd_redraw();
}


/*
 * Go to any level
 */
static void do_cmd_wiz_jump(void)
{
	/* Ask for level */
	if (command_arg <= 0)
	{
		char	ppp[80];

		char	tmp_val[160];

		/* Prompt */
		msg_format("dungeon_type : %d", dungeon_type);
		sprintf(ppp, "Jump to level (0-%d-%d): ", d_info[dungeon_type].mindepth, d_info[dungeon_type].maxdepth);

		/* Default */
		sprintf(tmp_val, "%d", dun_level);

		/* Ask for a level */
		if (!get_string(ppp, tmp_val, 10)) return;

		/* Extract request */
		command_arg = atoi(tmp_val);
	}

	/* Paranoia */
	if (command_arg < 0) command_arg = 0;

	/* Paranoia */
	if (command_arg > d_info[dungeon_type].maxdepth) command_arg = d_info[dungeon_type].maxdepth;

	/* Accept request */
	msg_format("You jump to dungeon level %d.", command_arg);

	if (autosave_l)
	{
		is_autosave = TRUE;
		msg_print("Autosaving the game...");
		do_cmd_save_game();
		is_autosave = FALSE;
	}

	/* Change level */
	dun_level = command_arg;

	p_ptr->inside_arena = 0;
	leaving_quest = p_ptr->inside_quest;

	p_ptr->inside_quest = 0;

	/* Leaving */
	p_ptr->leaving = TRUE;
}


/*
 * Become aware of a lot of objects
 */
static void do_cmd_wiz_learn(void)
{
	s32b i;

	object_type *q_ptr;

	/* Scan every object */
	for (i = 1; i < max_k_idx; i++)
	{
		object_kind *k_ptr = &k_info[i];

		/* Induce awareness */
		if (k_ptr->level <= command_arg)
		{
			/* Get local object */
			q_ptr = new_object();

			/* Prepare object */
			object_prep(q_ptr, i);

			/* Awareness */
			object_aware(q_ptr);

			delete_object(q_ptr);
		}
	}
}


/*
 * Summon some creatures
 */
static void do_cmd_wiz_summon(s32b num)
{
	s32b i;

	for (i = 0; i < num; i++)
	{
		(void)summon_random(p_ptr->py, p_ptr->px, dun_level, FACTION_DUNGEON);
	}
}


/*
 * Summon a creature of the specified type
 *
 * XXX XXX XXX This function is rather dangerous
 */
static void do_cmd_wiz_named(s32b r_idx, bool slp)
{
	s32b i, x, y;

	/* Paranoia */
	/* if (!r_idx) return; */

	/* Prevent illegal monsters */
	if (r_idx >= max_r_idx) return;

	/* Try 10 times */
	for (i = 0; i < 10; i++)
	{
		s32b d = 1;

		/* Pick a location */
		scatter(&y, &x, p_ptr->py, p_ptr->px, d, 0);

		/* Require empty grids */
		if (!cave_empty_bold(y, x)) continue;

		/* Place it (allow groups) */
		m_allow_special[r_idx] = TRUE;
		if (place_monster_aux(y, x, r_idx, slp, TRUE, FACTION_DUNGEON)) break;
		m_allow_special[r_idx] = FALSE;
	}
}


/*
 * Summon a creature of the specified type
 *
 * XXX XXX XXX This function is rather dangerous
 */
void do_cmd_wiz_named_friendly(s32b r_idx, bool slp)
{
	s32b i, x, y;

	/* Paranoia */
	/* if (!r_idx) return; */

	/* Prevent illegal monsters */
	if (r_idx >= max_r_idx) return;

	/* Try 10 times */
	m_allow_special[r_idx] = TRUE;
	for (i = 0; i < 10; i++)
	{
		s32b d = 1;

		/* Pick a location */
		scatter(&y, &x, p_ptr->py, p_ptr->px, d, 0);

		/* Require empty grids */
		if (!cave_empty_bold(y, x)) continue;

		/* Place it (allow groups) */
		if (place_monster_aux(y, x, r_idx, slp, TRUE, FACTION_PLAYER)) break;
	}
	m_allow_special[r_idx] = FALSE;
}



/*
 * Hack -- Delete all nearby monsters
 */
static void do_cmd_wiz_zap(void)
{
	s32b i;

	/* Genocide everyone nearby */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);

		/* Paranoia -- Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Only delete nearby monsters */
		if (m_ptr->cdis > MAX_SIGHT) continue;

		/* User must explicitly aprove of zapping unkillable monsters. */
		if (has_flag(m_ptr, FLAG_NO_DEATH))
		{
			char buf[256], name[256];

			monster_desc(name, m_ptr, 0x180);
			sprintf(buf, "Zap %s? ", name);

			if (!get_check(buf))
				continue;
		}

		delete_monster_idx(i);
	}
	end_for_flags();
}

#ifdef ALLOW_SPOILERS

/*
 * External function
 */
extern void do_cmd_spoilers(void);

#endif /* ALLOW_SPOILERS */

/*
 * Verify use of "debug" commands
 */
static bool enter_debug_mode(void)
{
	/* Ask first time */
#if 0
	if (!(noscore & 0x0008))
#else
if (!noscore && !wizard)
#endif
	{
		/* Mention effects */
		msg_print("The debug commands are for debugging and experimenting.");
		msg_print("The game will not be scored if you use debug commands.");
		msg_print(NULL);

		/* Verify request */
		if (!get_check("Are you sure you want to use debug commands? "))
		{
			return (FALSE);
		}

		/* Mark savefile */
		noscore |= 0x0008;
	}

	/* Success */
	return (TRUE);
}

/*
 * Ask for and parse a "debug command"
 * The "command_arg" may have been set.
 */
void do_cmd_debug(void)
{
	s32b x, y;
	s32b cmd;

	if (!enter_debug_mode())
		return;

	/* Get a "debug command" */
	get_com("Debug Command: ", &cmd);

	/* Analyze the command */
	switch (cmd)
	{
		/* Nothing */
	case ESCAPE:
	case ' ':
	case '\n':
	case '\r':
		break;


#ifdef ALLOW_SPOILERS

		/* Hack -- Generate Spoilers */
	case '"':
		do_cmd_spoilers();
		break;

#endif /* ALLOW_SPOILERS */

		/* Hack -- Help */
	case '?':
		do_cmd_help();
		break;


		/* Cure all maladies */
	case 'a':
		do_cmd_wiz_cure_all();
		break;

		/* Create any object */
	case '-':
		wiz_create_item_2();
		break;

		/* Create any object */
	case 'c':
		wiz_create_item();
		break;

		/* Create a named artifact */
	case 'C':
		wiz_create_named_art();
		break;

		/* Detect everything */
	case 'd':
//DGDGDGDG		detect_all(DEFAULT_RADIUS);
		break;

		/* Edit character */
	case 'e':
		do_cmd_wiz_change();
		break;

		/* Change grid's mana */
	case 'E':
		cave[p_ptr->py][p_ptr->px].mana = command_arg;
		break;

		/* View item info */
	case 'f':
		identify_fully();
		break;

		/* Good Objects */
	case 'g':
		if (command_arg <= 0) command_arg = 1;
		acquirement(p_ptr->py, p_ptr->px, command_arg, FALSE, TRUE);
		break;

		/* Hitpoint rerating */
	case 'h':
		do_cmd_rerate(); break;

	case 'H':
		do_cmd_summon_horde(); break;

		/* Identify */
	case 'i':
		(void)ident_spell();
		break;

		/* Go up or down in the dungeon */
	case 'j':
		do_cmd_wiz_jump();
		break;

		/* Self-Knowledge */
	case 'k':
		self_knowledge(NULL);
		break;

		/* Learn about objects */
	case 'l':
		do_cmd_wiz_learn();
		break;

		/* Magic Mapping */
	case 'm':
		map_area(p_ptr->py, p_ptr->px, 60);
		break;

		/* Object playing routines */
	case 'o':
		do_cmd_wiz_play();
		break;

		/* Phase Door */
	case 'p':
		teleport_player(10);
		break;

		/* Panic save the game */
	case 'P':
		exit_game_panic();
		break;

		/* Make every dungeon square "known" to test streamers -KMW- */
	case 'u':
		{
			for (y = 0; y < cur_hgt; y++)
			{
				for (x = 0; x < cur_wid; x++)
				{
					cave[y][x].info |= (CAVE_GLOW | CAVE_MARK);
				}
			}
			wiz_lite_extra();
			break;
		}

	case 'U':
		{
			p_ptr->necro_extra |= CLASS_UNDEAD;
			do_cmd_wiz_named(5, TRUE);

			p_ptr->necro_extra2 = 1;

			/* Display the hitpoints */
			p_ptr->update |= (PU_HP);
			flag_bool(&p_ptr->redraw, FLAG_PR_HP);

			/* Window stuff */
			flag_bool(&p_ptr->window, FLAG_PW_ALL);
			break;
		}

		/* Summon Random Monster(s) */
	case 's':
		if (command_arg <= 0) command_arg = 1;
		do_cmd_wiz_summon(command_arg);
		break;

		/* Teleport */
	case 't':
		teleport_player_bypass = TRUE;
		teleport_player(100);
		teleport_player_bypass = FALSE;
		break;

		/* Very Good Objects */
	case 'v':
		if (command_arg <= 0) command_arg = 1;
		acquirement(p_ptr->py, p_ptr->px, command_arg, TRUE, TRUE);
		break;

		/* Wizard Light the Level */
	case 'w':
		wiz_lite(TRUE);
		break;

		/* Make a wish */
	case 'W':
		make_wish();
		break;

		/* Increase Experience */
	case 'x':
		if (command_arg)
		{
			gain_exp(command_arg);
		}
		else
		{
			gain_exp(p_ptr->exp + 1);
		}
		break;

		/* Zap Monsters (Genocide) */
	case 'z':
		do_cmd_wiz_zap();
		break;

		/* Hack -- whatever I desire */
	case '_':
		do_cmd_wiz_hack_ben(command_arg);
		break;

		/* Change the feature of the map */
		/* 'F': implemented in lua */

	case '=':
		wiz_align_monster(command_arg);
		break;

	case '@':
		wiz_inc_monster_level(command_arg);
		break;

	case '>':
		do_cmd_lua_script();
		break;

		/* Not a Wizard Command */
	default:
		if (!process_hooks(HOOK_DEBUG_COMMAND, "(d)", cmd))
			msg_print("That is not a valid debug command.");
		break;
	}
}


#else

void do_cmd_debug(void)
{
	msg_print("Debug (wizard) commands not supported.");
}

#endif
