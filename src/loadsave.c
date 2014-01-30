/* File: loadsave.c */

/* Purpose: interact with savefiles. This file was made by
   unifying load2.c and save.c from the old codebase. Doing it
   this way makes maintenance easier and lets us share code. */

#include "angband.h"
#include "physfs/zlib114/zlib.h"

static void do_string(char *, s32b, s32b);
static void do_lore(s32b, s32b);
static void do_randomizer(s32b flag);
static void note(cptr);
static void do_options(s32b);
static void do_messages(s32b flag);
static void do_xtra(s32b, s32b);
static bool do_savefile_aux(s32b);
static bool do_inventory(s32b);
static bool do_dungeon(s32b);
static void do_grid(s32b);
static void my_sentinel(char *, u16b, s32b);

errr rd_savefile(bool quickstart);

#ifdef SAFER_PANICS
bool panicload;
#endif

static PHYSFS_file *fff; 	/* Local savefile ptr */

/*
 * Functions handling the savefile directory/files structure as follow:
 * /save/"savefile_name"/		Savefile directory
 * /save/"savefile_name"/quickstart	Quickstart savefile
 * /save/"savefile_name"/savefile	Main savefile
 * /save/"savefile_name"/panic		Panic savefile in case of crash
 * /save/"savefile_name"/dunX.Y		Save of the level Y of dungeon X
 * /save/"savefile_name"/wildY.x	Save of the position Y,X of the wilderness
 */

/* Checks if the directory is corectly in place, if not create it */
bool check_savefile_dir(cptr name)
{
	if (PHYSFS_isDirectory(name)) return TRUE;

	if (PHYSFS_mkdir(name)) return TRUE;

	return FALSE;
}

/* Deletes the savefile file structure */
void delete_savefile_dir(cptr name)
{
	char buf[1024];
	char **rc;
	char **i;

	if (!file_exist(name)) return;

	/* Wipe all files and the directory */
	rc = PHYSFS_enumerateFiles(name);

	for (i = rc; *i != NULL; i++)
	{
		path_build(buf, 1024, name, *i);
		PHYSFS_delete(buf);
	}
	PHYSFS_delete(name);

	PHYSFS_freeList(rc);
}

/*
 * Basic byte-level reading from savefile. This provides a single point
 * of interface to the pseudoencryption that ToME (and Angband)
 * uses. I'm thinking about if it might be faster/better to modify all
 * the do_* functions to directly do this stuff -- it'd make the code
 * somewhat uglier to maintain, but concievably might be much faster. Or
 * is it better maybe to scrap the pseudoencryption entirely and adopt
 * some other means of obfuscation, should it still prove useful in any
 * way? -- Improv
 *
 * What's the point of encryption on savefiles anyway? If I wanted to
 * make a cheater savefile, I'd activate debug mode, and hack the game
 * not to save it. There's no point. -- takkaria
 */

static byte sf_get(void)
{
	byte c;

	/* Get a character, decode the value */
	c = fgetc(fff) & 0xFF;

	/* Return the value */
	return (c);
}


static void sf_put(byte v)
{
	(void)fputc((s32b)v, fff);
}

void do_flags(flags_type *f, s32b flag)
{
	if (flag == LS_SAVE)
	{
		s32b i;
		s32b nb = flag_used(f);

                /* How many flags */
		do_s32b(&nb, flag);

                /* Now each key/val pair */
		for (i = 0; i < f->size; i++)
		{
			if (f->node[i].flags & FLAG_FLAG_USED)
			{
				do_u32b(&f->node[i].key, flag);
				do_byte(&f->node[i].flags, flag);
				if (f->node[i].flags & FLAG_FLAG_PTR)
				{
					do_byte(&f->node[i].vals.ptr.type, flag);
					flags_pointer_types[f->node[i].vals.ptr.type].loadsave(f->node[i].vals.ptr.type, &(f->node[i].vals.ptr.ptr), flag);
				}
				else if (f->node[i].flags & FLAG_FLAG_FLAGS)
				{
					do_flags(f->node[i].vals.flags, flag);
				}
				else
				{
					do_s32b(&f->node[i].vals.vals.val, flag);
					do_s32b(&f->node[i].vals.vals.val2, flag);
				}
			}
		}
	}
	else if(flag == LS_LOAD)
	{
		s32b nb = 0;

		do_s32b(&nb, flag);

		/* Now each key/val pair */
		while (nb)
		{
			u32b key;
			s32b val, val2;
			flags_type *val_flags = NULL;
			byte flags;

			do_u32b(&key, flag);
			do_byte(&flags, flag);
			if (flags & FLAG_FLAG_PTR)
			{
				byte type;
				void *ptr = NULL;

				do_byte(&type, flag);
				if (flags_pointer_types[type].alloc != NULL) ptr = flags_pointer_types[type].alloc();
				flags_pointer_types[type].loadsave(type, &ptr, flag);
				val = type;
				val_flags = (flags_type*)ptr;
			}
			else if (flags & FLAG_FLAG_FLAGS)
			{
				val_flags = flag_new(2);
				do_flags(val_flags, flag);
			}
			else
			{
				do_s32b(&val, flag);
				do_s32b(&val2, flag);
			}

			flag_set_full(f, key, flags, val, val2, val_flags);

			nb--;
		}
	}
}


/*
 * Do object memory and similar stuff
 */
static void do_xtra(s32b k_idx, s32b flag)
{
	s16b tval, sval;
	byte tmp8u = 0;
	object_kind *k_ptr;

	if (flag == LS_SAVE)
	{
		k_ptr = &k_info[k_idx];
		tval = k_ptr->tval;
		sval = k_ptr->sval;
		do_s16b(&tval, flag);
		do_s16b(&sval, flag);

		if (k_ptr->aware) tmp8u |= 0x01;
		if (k_ptr->tried) tmp8u |= 0x02;
		if (k_ptr->know) tmp8u |= 0x04;
		if (k_ptr->artifact) tmp8u |= 0x80;

		do_byte(&tmp8u, flag);
	}
	else if (flag == LS_LOAD)
	{
		do_s16b(&tval, flag);
		do_s16b(&sval, flag);
		k_ptr = &k_info[lookup_kind(tval, sval)];

		do_byte(&tmp8u, flag);
		k_ptr->aware = ((tmp8u & 0x01) ? TRUE : FALSE);
		k_ptr->tried = ((tmp8u & 0x02) ? TRUE : FALSE);
		k_ptr->know = ((tmp8u & 0x04) ? TRUE : FALSE);
		k_ptr->artifact = ((tmp8u & 0x80) ? TRUE : FALSE);
	}
}

/*
 * Load/Save quick start data
 */
void do_quick_start(s32b flag)
{
	s32b i;

	do_s16b(&previous_char.sex, flag);
	do_s16b(&previous_char.race, flag);
	do_s16b(&previous_char.rmod, flag);
	do_s16b(&previous_char.pclass, flag);
	do_s16b(&previous_char.spec, flag);
	do_byte(&previous_char.quests, flag);
	do_byte(&previous_char.god, flag);
	do_s32b(&previous_char.grace, flag);
	do_s16b(&previous_char.age, flag);
	do_s16b(&previous_char.wt, flag);
	do_s16b(&previous_char.ht, flag);
	do_s16b(&previous_char.sc, flag);
	do_s32b(&previous_char.au, flag);

	for (i = 0; i < 6; i++) do_s16b(&(previous_char.stat[i]), flag);
	do_s16b(&previous_char.luck, flag);

	do_u32b(&previous_char.weapon, flag);
	do_char(&previous_char.quick_ok, flag);

	for (i = 0; i < 4; i++) do_string(previous_char.history[i], 60, flag);
}

/*
 * The special saved subrace
 */
static void do_subrace(s32b flag)
{
#if 0 // DGDGDGDGDG
	player_race_mod *sr_ptr = &race_mod_info[SUBRACE_SAVE];
	s32b i;
	char buf[81];

	if (flag == LS_SAVE)
		strcpy(buf, sr_ptr->title);
	do_string(buf, 80, flag);
	if (flag == LS_LOAD)
		sr_ptr->title = string_make(buf);

	if (flag == LS_SAVE)
		strcpy(buf, sr_ptr->desc);
	do_string(buf, 80, flag);
	if (flag == LS_LOAD)
		sr_ptr->desc = string_make(buf);

	do_byte(&sr_ptr->place, flag);

	for (i = 0; i < 6; i++)
		do_s16b(&sr_ptr->r_adj[i], flag);

	do_byte(&sr_ptr->luck, flag);
	do_s16b(&sr_ptr->mana, flag);

	do_s16b(&sr_ptr->r_dis, flag);
	do_s16b(&sr_ptr->r_dev, flag);
	do_s16b(&sr_ptr->r_sav, flag);
	do_s16b(&sr_ptr->r_stl, flag);
	do_s16b(&sr_ptr->r_srh, flag);
	do_s16b(&sr_ptr->r_fos, flag);
	do_s16b(&sr_ptr->r_thn, flag);
	do_s16b(&sr_ptr->r_thb, flag);

	do_byte(&sr_ptr->r_mhp, flag);
	do_s16b(&sr_ptr->r_exp, flag);

	do_byte(&sr_ptr->b_age, flag);
	do_byte(&sr_ptr->m_age, flag);

	do_byte(&sr_ptr->m_b_ht, flag);
	do_byte(&sr_ptr->m_m_ht, flag);
	do_byte(&sr_ptr->f_b_ht, flag);
	do_byte(&sr_ptr->f_m_ht, flag);

	do_byte(&sr_ptr->m_b_wt, flag);
	do_byte(&sr_ptr->m_m_wt, flag);
	do_byte(&sr_ptr->f_b_wt, flag);
	do_byte(&sr_ptr->f_m_wt, flag);

	do_byte(&sr_ptr->infra, flag);

	for (i = 0; i < 4; i++)
		do_s16b(&sr_ptr->powers[i], flag);

	do_flags(&sr_ptr->body_parts, flag);

	do_flags(&sr_ptr->flags, flag);

	for (i = 0; i < PY_MAX_LEVEL + 1; i++)
	{
		do_flags(&sr_ptr->oflags[i], flag);
	}

	do_byte(&sr_ptr->g_attr, flag);
	do_byte(&sr_ptr->g_char, flag);

	do_flags(&sr_ptr->skill_base, flag);
	do_flags(&sr_ptr->skill_mod, flag);
#endif
}

/*
 * Misc. other data
 */
static char loaded_game_module[80];
static bool do_extra(s32b flag)
{
	s32b i, j;
	byte tmp8u;
	char tmp8s;
	s16b tmp16s;
	u32b tmp32u;
	u16b tmp16b;

	do_string(player_name, 32, flag);

	do_string(died_from, 80, flag);

	for (i = 0; i < 4; i++)
	{
		do_string(history[i], 60, flag);
	}

	/* Handle the special levels info */
	if (flag == LS_SAVE)
	{
		tmp8u = max_d_idx;
		tmp16s = MAX_DUNGEON_DEPTH;
	}
	do_byte(&tmp8u, flag);

	if (flag == LS_LOAD)
	{
		if (tmp8u > max_d_idx)
		{
			note(format("Too many (%d) dungeon types!", tmp8u));
		}
	}

	do_s16b(&tmp16s, flag);

	if (flag == LS_LOAD)
	{
		if (tmp16s > MAX_DUNGEON_DEPTH)
		{
			note(format("Too many (%d) max level by dungeon type!", tmp16s));
		}
	}

	/* Load the special levels history */
	for (i = 0; i < tmp8u; i++)
	{
		for (j = 0; j < tmp16s; j++)
		{
			do_char(&special_lvl[j][i], flag);
		}
	}

	do_char(&generate_special_feeling, flag);

	/* Load the quick start data */
	do_quick_start(flag);

	/* Load/save the special subrace */
	do_subrace(flag);

	/* Race/Class/Gender/Spells */
	do_s32b(&p_ptr->lives, flag);
	do_flags(&p_ptr->descriptors, flag);
	do_byte(&p_ptr->mimic_form, flag);
	do_s16b(&p_ptr->mimic_level, flag);

	do_byte(&p_ptr->hitdie, flag);
	do_u16b(&p_ptr->expfact, flag);

	do_s16b(&p_ptr->age, flag);
	do_s16b(&p_ptr->ht, flag);
	do_s16b(&p_ptr->wt, flag);

	/* Dump the stats (maximum and current) */
	do_flags(&p_ptr->stat_max, flag);
	do_flags(&p_ptr->stat_cur, flag);
	do_flags(&p_ptr->stat_cnt, flag);
	do_flags(&p_ptr->stat_los, flag);

	/* Dump the skills */
	do_s16b(&p_ptr->skill_points, flag);
	do_s16b(&p_ptr->skill_last_level, flag);
	do_s16b(&p_ptr->combat_style, flag);
	do_s16b(&p_ptr->use_piercing_shots, flag);

	do_flags(&p_ptr->flags, flag);

	tmp16s = max_s_idx;
	do_s16b(&tmp16s, flag);

	if ((flag == LS_LOAD) && (tmp16s > max_s_idx))
	{
		quit("Too many skills");
	}

	if (flag == LS_SAVE) old_max_s_idx = max_s_idx;
	do_u16b(&old_max_s_idx, flag);
	for (i = 0; i < tmp16s; ++i)
	{
		if (i < old_max_s_idx)
		{
			do_s32b(&s_info[i].value, flag);
			do_s32b(&s_info[i].mod, flag);
			do_char(&s_info[i].dev, flag);
			do_char(&s_info[i].hidden, flag);
			do_u32b(&s_info[i].uses, flag);
		}
		else
		{
			do_u32b(&tmp32u, flag);
			do_u16b(&tmp16b, flag);
			do_char(&tmp8s, flag);
			do_char(&tmp8s, flag);
			do_u32b(&tmp32u, flag);
		}
	}

	tmp16s = max_ab_idx;
	do_s16b(&tmp16s, flag);

	if ((flag == LS_LOAD) && (tmp16s > max_ab_idx))
	{
		quit("Too many abilities");
	}

	for (i = 0; i < tmp16s; ++i)
	{
		do_char(&ab_info[i].hidden, flag);
		do_char(&ab_info[i].acquired, flag);
	}

	do_s16b(&p_ptr->luck_base, flag);
	do_s16b(&p_ptr->luck_max, flag);

	do_s32b(&p_ptr->au, flag);

	do_s32b(&p_ptr->max_exp, flag);
	do_s32b(&p_ptr->exp, flag);
	do_u16b(&p_ptr->exp_frac, flag);
	do_s16b(&p_ptr->lev, flag);

	do_s16b(&p_ptr->town_num, flag); 	/* -KMW- */

	/* Write arena and rewards information -KMW- */
	do_s16b(&p_ptr->arena_number, flag);
	do_s16b(&p_ptr->inside_arena, flag);
	do_s16b(&p_ptr->inside_quest, flag);
	do_char(&p_ptr->exit_bldg, flag);


	do_s16b(&p_ptr->oldpx, flag);
	do_s16b(&p_ptr->oldpy, flag);

	do_s16b(&p_ptr->hp_mod, flag);

	/* Max Player and Dungeon Levels */
	do_s16b(&p_ptr->max_plv, flag);

	if (flag == LS_SAVE)
		tmp8u = max_d_idx;
	do_byte(&tmp8u, flag);
	for (i = 0; i < tmp8u; i++)
	{
		if (flag == LS_SAVE)
			tmp16s = max_dlv[i];
		do_s16b(&tmp16s, flag);
		if ((flag == LS_LOAD) && (i <= max_d_idx))
			max_dlv[i] = tmp16s;
	}
	/* Repair max player level??? */
	if ((flag == LS_LOAD) && (p_ptr->max_plv < p_ptr->lev))
		p_ptr->max_plv = p_ptr->lev;

	do_char(&(p_ptr->help.enabled), flag);
	do_u32b(&(p_ptr->help.help1), flag);

	/* More info */
	do_s16b(&p_ptr->sc, flag);
	do_s32b(&p_ptr->energy, flag);
	do_s16b(&p_ptr->control, flag);
	do_byte(&p_ptr->control_dir, flag);
	do_s16b(&p_ptr->word_recall, flag);
	do_s16b(&p_ptr->recall_dungeon, flag);
	do_s16b(&p_ptr->see_infra, flag);

	do_char(&p_ptr->black_breath, flag);
	do_byte(&p_ptr->maximize, flag);
	do_byte(&p_ptr->preserve, flag);
	do_byte(&p_ptr->special, flag);
	do_byte(&p_ptr->allow_one_death, flag);
	do_s16b(&p_ptr->xtra_spells, flag);

	do_byte(&vanilla_town, flag);

	do_u16b(&no_breeds, flag);

	/* Auxilliary variables */
	do_u32b(&p_ptr->mimic_extra, flag);
	do_u32b(&p_ptr->antimagic_extra, flag);
	do_u32b(&p_ptr->druid_extra, flag);
	do_u32b(&p_ptr->druid_extra2, flag);
	do_u32b(&p_ptr->druid_extra3, flag);
	do_u32b(&p_ptr->music_extra, flag);
	do_u32b(&p_ptr->music_extra2, flag);
	do_u32b(&p_ptr->necro_extra, flag);
	do_u32b(&p_ptr->necro_extra2, flag);

	do_u32b(&p_ptr->race_extra1, flag);
	do_u32b(&p_ptr->race_extra2, flag);
	do_u32b(&p_ptr->race_extra3, flag);
	do_u32b(&p_ptr->race_extra4, flag);
	do_u32b(&p_ptr->race_extra5, flag);
	do_u32b(&p_ptr->race_extra6, flag);
	do_u32b(&p_ptr->race_extra7, flag);

	do_monster(0, &p_ptr->body_monster, flag);

	/* Intrinsinc powers */
	do_flags(&p_ptr->powers_mod, flag);

	/* The comapnions killed */
	do_s16b(&p_ptr->companion_killed, flag);

	/* Write the "object seeds" */
	do_u32b(&seed_dungeon, flag);
	do_u32b(&seed_flavor, flag);
	do_u32b(&seed_town, flag);

	/* Special stuff */
	do_u16b(&panic_save, flag);
	do_u16b(&total_winner, flag);
	do_u16b(&has_won, flag);
	do_u16b(&noscore, flag);

	/* Factions */
	do_flags(&factions, flag);

	/* Write death */
	if (flag == LS_SAVE) tmp8u = death;
	do_byte(&tmp8u, flag);
	if (flag == LS_LOAD) death = tmp8u;

	/* Incompatible module? */
	if (flag == LS_LOAD)
	{
		s32b ok;

		call_lua("module_savefile_loadable", "(s,d)", "d", loaded_game_module, death, &ok);

		/* Argh bad game module! */
		if (!ok)
		{
			note(format("Bad game module. Savefile was saved with module '%s' but game is '%s'.", loaded_game_module, game_module));
			return (FALSE);
		}
	}

	/* Write feeling */
	if (flag == LS_SAVE) tmp8u = feeling;
	do_byte(&tmp8u, flag);
	if (flag == LS_LOAD) feeling = tmp8u;

	/* Turn of last "feeling" */
	do_s32b(&old_turn, flag);

	/* Current turn */
	do_s32b(&turn, flag);

	return TRUE;
}

/* Save the current persistent dungeon -SC- */
void save_dungeon(s32b saved_turn)
{
	char tmp[30];
	char name[1024];
	flags_type *saved;

	/* Save only persistent dungeons */
	if (!has_flag(dungeon_flags, FLAG_SAVE_LEVEL)) return;

	/* Construct filename */
	if (dun_level)
		sprintf(tmp, "dun%d.%d", dungeon_type, dun_level);
	else
		sprintf(tmp, "wild%ld.%ld", p_ptr->wilderness_y, p_ptr->wilderness_x);
	path_build(name, 1024, savefile, tmp);

	/* Make sure there arent any compressed or uncompressed forms of it */
	fd_kill(name);

	/* Open the file */
	tmp[0] = 'w';
	tmp[1] = 'b';
	tmp[2] = 'z';
	tmp[3] = I2D(savefile_compress);
	tmp[4] = '\0';

	fff = my_fopen(name, tmp);

	if (!fff)
	{
		if (wizard)
			cmsg_format(TERM_VIOLET,
						"save_dungeon() couldn't open file '%s': %s",
						name, PHYSFS_getLastError());
		return;
	}

	/* Ugly, temporary remove the persistent monsters so they dont duplicate */
	saved = flag_new(monst_list.size);
	for_flags(&monst_list);
	{
		monster_type *m_ptr;

		if (!__key) continue;

		m_ptr = get_monster(__key);
		if (has_flag(m_ptr, FLAG_PERMANENT))
		{
			flag_set_monster(saved, __key, m_ptr);
		}
	}
	end_for_flags();

	/* Now remove them */
	for_flags(saved);
	{
		if (!__key) continue;

		/* Remove but do not dealloc */
		flag_remove_zap(&monst_list, __key);
	}
	end_for_flags();


	/* Save the dungeon */
	do_dungeon(LS_SAVE);
	do_s32b(&saved_turn, LS_SAVE);

	/* Add them back */
	for_flags(saved);
	{
		if (!__key) continue;

		/* Remove but do not dealloc */
		flag_set_monster(&monst_list, __key, flag_get_monster(saved, __key));
	}
	end_for_flags();

	flag_empty_zap(saved);
	flag_free(saved, TRUE);

	my_fclose(fff);
}

/*
 * Medium level player saver
 */
static bool save_player_aux(char *name)
{
	char tmp[8];
	bool ok = FALSE;

	/* No file yet */
	fff = NULL;

	/* Create the savefile */
	tmp[0] = 'w';
	tmp[1] = 'b';
	tmp[2] = 'z';
	tmp[3] = I2D(savefile_compress);
	tmp[4] = '\0';
	fff = my_fopen(name, tmp);

	if (!fff)
	{
		if (wizard)
			cmsg_format(TERM_VIOLET,
						"save_player_aux() couldn't open file '%s': %s",
						name, PHYSFS_getLastError());
		return(FALSE);
	}

	/* File is okay */
	if (fff)
	{
		/* Successful open */
		if (fff)
		{
			/* Write the savefile */
			if (do_savefile_aux(LS_SAVE)) ok = TRUE;

			/* Attempt to close it */
			if (my_fclose(fff)) ok = FALSE;
		}

		/* "broken" savefile */
		if (!ok)
		{
			/* Remove "broken" files */
			PHYSFS_delete(name);
		}
	}

	/* Failure */
	if (!ok) return (FALSE);
	process_hooks(HOOK_SAVE_GAME_END, "()");

	/* Successful save */
	character_saved = TRUE;

	/* Success */
	return (TRUE);
}

/*
 * Attempt to save the player in a savefile
 */
bool save_player(void)
{
	s32b result = FALSE;
	char safe[1024];
	char panicsave[1024];

	/* Make sure the directory structure exists */
	check_savefile_dir(savefile);

	if (panic_save)
	{
		/*
		 * Not sure how to do this so it's nicely portable to brain-damaged
		 * OS's with short filenames
		 */
		path_build(panicsave, 1024, savefile, "panic");

		/* Remove any old panic saves */
		fd_kill(panicsave);

		/* Save character */
		save_player_aux(panicsave);

		return TRUE;
	}

	/* New savefile */
	path_build(safe, 1024, savefile, "savefile");

	/* Remove it */
	fd_kill(safe);

	/* Attempt to save the player */
	if (save_player_aux(safe))
	{
		/* Hack -- Pretend the character was loaded */
		character_loaded = TRUE;

		/* Success */
		result = TRUE;
	}

	/* Save permanent data */
	save_perm_data();

	save_savefile_names();

	/* Return the result */
	return (result);
}

/*
 * Attempt to save the player in a quickstart savefile
 */
bool save_player_quickstart(void)
{
	char safe[1024];

	/* Make sure the directory structure exists */
	check_savefile_dir(savefile);

	/* New savefile */
	path_build(safe, 1024, savefile, "quickstart");

	/* Remove it */
	fd_kill(safe);

	/* Attempt to save the player */
	if (save_player_aux(safe))
	{
		/* Success */
		return TRUE;
	}

	/* Return the result */
	return FALSE;
}

void set_loadsave_stream(PHYSFS_file *f)
{
	fff = f;
}

bool file_exist(cptr buf)
{
	bool result;

	result = (PHYSFS_exists(buf) != 0) ? TRUE : FALSE;

	return result;
}

/*
 * Attempt to Load a "savefile"
 *
 * On multi-user systems, you may only "read" a savefile if you will be
 * allowed to "write" it later, this prevents painful situations in which
 * the player loads a savefile belonging to someone else, and then is not
 * allowed to save his game when he quits.
 *
 * We return "TRUE" if the savefile was usable, and we set the global
 * flag "character_loaded" if a real, living, character was loaded.
 *
 * Note that we always try to load the "current" savefile, even if
 * there is no such file, so we must check for "empty" savefile names.
 */
bool load_player(bool quickstart)
{
	char magic_header[SAVEFILE_MAGIC_LEN + 1];
	errr err = 0;
	bool testfff = FALSE;
	char safe[1024]; 		/* Filename for normal savefiles */
	char panic_fname[1024]; 	/* Filename for panic savefiles */

	cptr what = "generic";

	panicload = FALSE;

	if (quickstart)
		path_build(safe, 1024, savefile, "quickstart");
	else
		path_build(safe, 1024, savefile, "savefile");
	path_build(panic_fname, 1024, savefile, "panic");

	/* Paranoia */
	turn = 0;

	/* Paranoia */
	death = FALSE;

	/* Allow empty savefile name */
	if (!savefile[0]) return (TRUE);

	/* XXX XXX XXX Fix this */

	/* Verify the existance of the savefile */
	if ((!file_exist(savefile)) || ((!file_exist(safe)) && (!file_exist(panic_fname))))
	{
		/* Give a message */
		/* Dont
		msg_format("Savefile does not exist: %s", savefile);
		msg_print(NULL);
		*/

		/* Allow this */
		return (TRUE);
	}

	/* Okay */
	if (!err)
	{
		/* Open the savefile */
		fff = my_fopen(safe, "rbz");

		/* No file */
		if (fff == NULL) err = -1;

		/* Message (below) */
		if (err) what = "Cannot open savefile";
	}

	/* Open panic save file */
	testfff = file_exist(panic_fname);

	/* A panic save exists, which is not normally the case */
	if (testfff)
	{
		panicload = 1;

		/* Close the normal save file */
		my_fclose(fff);

		/* Prefer panic saves over real saves */
		fff = my_fopen(panic_fname, "rbz");

		/* This is not the error if we're at this pt */
		what = "";
		err = 0;
	}

	/* Process file */
	if (!err)
	{
		/* Read the magic header and the first four bytes */
		do_string(magic_header, SAVEFILE_MAGIC_LEN + 1, LS_LOAD);
		do_u32b(&vernum, LS_LOAD);
		do_byte(&sf_extra, LS_LOAD);
	}

	my_fclose(fff);

	/* Process file */
	if (!err)
	{

		/* Extract version */
		sf_major = VERSION_MAJOR;
		sf_minor = VERSION_MINOR;
		sf_patch = VERSION_PATCH;
		sf_extra = VERSION_EXTRA;

		/* Clear screen */
		Term_clear();

		/* Attempt to load */
		err = rd_savefile(quickstart);

		/* Message (below) */
		if (err) what = "Cannot parse savefile";
	}

	/* Paranoia */
	if (!err)
	{
		/* Invalid turn */
		if (!turn) err = -1;

		/* Message (below) */
		if (err) what = "Broken savefile";
	}

	/* Okay */
	if (!err)
	{
		/* Maybe the scripts want to resurrect char */
		if (process_hooks_ret(HOOK_LOAD_GAME_END, "d", "(d)", death))
		{
			character_loaded = process_hooks_return[0].num;
			death = process_hooks_return[1].num;
			return TRUE;
		}

		/* Player is dead */
		if (death)
		{
			/* Player is no longer "dead" */
			death = FALSE;

			/* Cheat death (unless the character retired) */
			if (arg_wizard && !total_winner)
			{
				/* A character was loaded */
				character_loaded = TRUE;

				/* Done */
				return (TRUE);
			}

			/* Count lives */
			sf_lives++;

			/* Forget turns */
			turn = old_turn = 0;

			/* Done */
			return (TRUE);
		}

		/* A character was loaded */
		character_loaded = TRUE;

		if (panicload)
		{
			/*
			 * Done loading, it'll either immediately panic and re-save, or
			 * we don't need the panicsave file anymore. Either way, it's safe
			 * to zap the original panicsave
			 */
			fd_kill(panic_fname);
		}

		/* Success */
		return (TRUE);
	}

	/* Message */
	msg_format("Error (%s) reading %d.%d.%d savefile.",
	           what, sf_major, sf_minor, sf_patch);
	msg_print(NULL);

	/* Oops */
	return (FALSE);
}


/*
 * Size-aware read/write routines for the savefile, do all their
 * work through sf_get and sf_put.
 */

void do_bool(bool *v, s32b flag)
{
	if (flag == LS_LOAD)
	{
		*v = sf_get();
		return;
	}
	if (flag == LS_SAVE)
	{
		byte val = *v;
		sf_put(val);
		return;
	}
	/* We should never reach here, so bail out */
	printf("FATAL: do_byte passed %ld\n", flag);
	exit(0);
}

void do_byte(byte *v, s32b flag)
{
	if (flag == LS_LOAD)
	{
		*v = sf_get();
		return;
	}
	if (flag == LS_SAVE)
	{
		byte val = *v;
		sf_put(val);
		return;
	}
	/* We should never reach here, so bail out */
	printf("FATAL: do_byte passed %ld\n", flag);
	exit(0);
}

/*
 * Size-aware read/write routines for the savefile, do all their
 * work through sf_get and sf_put.
 */

void do_char(char *v, s32b flag)
{
	if (flag == LS_LOAD)
	{
		*v = (char)sf_get();
		return;
	}
	if (flag == LS_SAVE)
	{
		char val = *v;
		sf_put(val);
		return;
	}
	/* We should never reach here, so bail out */
	printf("FATAL: do_byte passed %ld\n", flag);
	exit(0);
}

void do_u16b(u16b *v, s32b flag)
{
	if (flag == LS_LOAD)
	{
		(*v) = sf_get();
		(*v) |= ((u16b)(sf_get()) << 8);
		return;
	}
	if (flag == LS_SAVE)
	{
		u16b val;
		val = *v;
		sf_put((byte)(val & 0xFF));
		sf_put((byte)((val >> 8) & 0xFF));
		return;
	}
	/* Never should reach here, bail out */
	printf("FATAL: do_u16b passed %ld\n", flag);
	exit(0);
}

void do_s16b(s16b *ip, s32b flag)
{
	byte sign;
	u16b v;

	/*
	 * To quiet Valgrind warnings, don't look at *ip if loading, since
	 * *ip will be an unitialized value.
	 */
	if (flag == LS_SAVE)
	{
		sign = (*ip < 0) ? TRUE : FALSE;
		v = (sign) ? -(*ip) : *ip;
	}

	do_byte(&sign, flag);
	do_u16b(&v, flag);
	if (flag == LS_LOAD)
	{
		*ip = (sign) ? -v : v;
		return;
	}
	if (flag == LS_SAVE)
	{
		return;
	}
	/* Raus! Schnell! */
	printf("FATAL: do_s16b passed %ld\n", flag);
	exit(0);
}

void do_u32b(u32b *ip, s32b flag)
{
	if (flag == LS_LOAD)
	{
		(*ip) = sf_get();
		(*ip) |= ((u32b)(sf_get()) << 8);
		(*ip) |= ((u32b)(sf_get()) << 16);
		(*ip) |= ((u32b)(sf_get()) << 24);
		return;
	}
	if (flag == LS_SAVE)
	{
		u32b val = *ip;
		sf_put((byte)(val & 0xFF));
		sf_put((byte)((val >> 8) & 0xFF));
		sf_put((byte)((val >> 16) & 0xFF));
		sf_put((byte)((val >> 24) & 0xFF));
		return;
	}
	/* Bad news if you're here, because you're going down */
	printf("FATAL: do_u32b passed %ld\n", flag);
	exit(0);
}

void do_s32b(s32b *ip, s32b flag)
{
	byte sign;
	u32b v;

	/*
	 * To quiet Valgrind warnings, don't look at *ip if loading, since
	 * *ip will be an unitialized value.
	 */
	if (flag == LS_SAVE)
	{
		sign = (*ip < 0) ? TRUE : FALSE;
		v = (sign) ? -(*ip) : *ip;
	}

	do_byte(&sign, flag);
	do_u32b(&v, flag);
	if (flag == LS_LOAD)
	{
		*ip = (sign) ? -v : v;
		return;
	}
	if (flag == LS_SAVE)
	{
		return;
	}
	/* Raus! Schnell! */
	printf("FATAL: do_s32b passed %ld\n", flag);
	exit(0);
}

static void do_string(char *str, s32b max, s32b flag)
/* Max is ignored for writing */
{
	if (flag == LS_LOAD)
	{
		s32b i;

		/* Read the string */
		for (i = 0; TRUE; i++)
		{
			char tmp8u;

			/* Read a byte */
			do_char(&tmp8u, LS_LOAD);

			/* Collect string while legal */
			if (i < max) str[i] = tmp8u;

			/* End of string */
			if (!tmp8u) break;
		}
		/* Terminate */
		str[max - 1] = '\0';
		return;
	}
	if (flag == LS_SAVE)
	{
		while (*str)
		{
			do_char(str, flag);
			str++;
		}
		do_char(str, flag); 		/* Output a terminator */
		return;
	}
	printf("FATAL: do_string passed flag %ld\n", flag);
	exit(0);
}

void loadsave_do_string_nolen(byte type, char **str, s32b flag)
{
	do_string_nolen(str, flag);
}
void do_string_nolen(char **str, s32b flag)
{
	if (flag == LS_SAVE)
	{
		if (*str != NULL)
		{
			u16b len = strlen(*str);
			do_u16b(&len, flag);
			do_string(*str, len + 1, flag);
		}
		else
		{
			u16b len = 0;
			do_u16b(&len, flag);
			do_string("", len + 1, flag);
		}
	}
	else
	{
		u16b len;
		do_u16b(&len, flag);
		C_MAKE(*str, len + 1, char);
		do_string(*str, len + 1, flag);
	}
}

#if USE_COMPATIBILITY_FUNCS /* Unused, uncomment when needed */

static void do_ver_byte(byte *, u32b, byte, s32b);
static void do_ver_u16b(u16b *, u32b, u16b, s32b);
static void do_ver_s16b(s16b *, u32b, s16b, s32b);
static void do_ver_u32b(u32b *, u32b, u32b, s32b);
static void do_ver_s32b(s32b *, u32b, s32b, s32b);
static void do_ver_string(char *, s32b, u32b, char *, s32b);

static void skip_ver_byte(u32b, s32b);
static void skip_ver_u16b(u32b, s32b);
static void skip_ver_s16b(u32b, s32b);
static void skip_ver_u32b(u32b, s32b);
static void skip_ver_s32b(u32b, s32b);
static void skip_ver_string(u32b, s32b);

/*
 * Periodically, to reduce overhead and code clutter, we'll probably
 * want to convert all calls to do_ver_??? with direct do_??? calls, and
 * break the backwards compatibility. How often this needs to happen
 * remains to be seen, as the rate of accumulation isn't very
 * predictable. -- Improv
 */
static void do_ver_byte(byte *v, u32b version, byte defval, s32b flag)
{
	if ((flag == LS_LOAD) && (vernum < version))
	{
		*v = defval; 			/* Use the default value, and DO NOT READ */
		return;
	}

	do_byte(v, flag); 			/* Otherwise, go as normal */
}

static void skip_ver_byte(u32b version, s32b flag)
/* Reads and discards a byte if the savefile is as old as/older than version */
{
	if ((flag == LS_LOAD) && (vernum <= version))
	{
		byte forget;
		do_byte(&forget, flag);
	}
	return;
}

static void do_ver_u16b(u16b *v, u32b version, u16b defval, s32b flag)
{
	if ((flag == LS_LOAD) && (vernum < version))
	{
		*v = defval;
		return;
	}
	do_u16b(v, flag);
}

static void skip_ver_u16b(u32b version, s32b flag)
{
	if ((flag == LS_LOAD) && (vernum <= version))
	{
		u16b forget;
		do_u16b(&forget, flag);
	}
	return;
}

static void do_ver_s16b(s16b *v, u32b version, s16b defval, s32b flag)
{
	if ((flag == LS_LOAD) && (vernum < version))
	{
		*v = defval;
		return;
	}
	do_s16b(v, flag);
}

static void skip_ver_s16b(u32b version, s32b flag)
{
	if ((flag == LS_LOAD) && (vernum <= version))
	{
		s16b forget;
		do_s16b(&forget, flag);
	}
	return;
}

static void do_ver_u32b(u32b *v, u32b version, u32b defval, s32b flag)
{
	if ((flag == LS_LOAD) && (vernum < version))
	{
		*v = defval;
		return;
	}
	do_u32b(v, flag);
}

static void skip_ver_u32b(u32b version, s32b flag)
{
	if ((flag == LS_LOAD) && (vernum <= version))
	{
		u32b forget;
		do_u32b(&forget, flag);
	}
	return;
}

static void do_ver_s32b(s32b *v, u32b version, s32b defval, s32b flag)
{
	if ((flag == LS_LOAD) && (vernum < version))
	{
		*v = defval;
		return;
	}
	do_s32b(v, flag);
}

static void skip_ver_s32b(u32b version, s32b flag)
{
	if ((flag == LS_LOAD) && (vernum <= version))
	{
		s32b forget;
		do_s32b(&forget, flag);
	}
	return;
}

static void do_ver_string(char *str, s32b max, u32b version, char *defval,
                          s32b flag)
/* Careful, remember the argument order here */
{
	if ((flag == LS_LOAD) && (vernum < version))
	{
		strncpy(str, defval, max);
		str[max - 1] = '\0'; 	/* Ensure that whatever happens, the result string is term'd */
		return;
	}
	do_string(str, max, flag);
}

static void skip_ver_string(u32b version, s32b flag)
/* This function is slow and bulky */
{
	if ((flag == LS_LOAD) && (vernum <= version))
	{
		char forget[1000];
		do_string(forget, 999, flag);
	}
	return;
}

#endif

/*
 * Show information on the screen, one line at a time.
 *
 * Avoid the top two lines, to avoid interference with "msg_print()".
 */
static void note(cptr msg)
{
	static s32b y = 2;

	/* Draw the message */
	prt(msg, y, 0);

	/* Advance one line (wrap if needed) */
	if (++y >= 24) y = 2;

	/* Flush it */
	Term_fresh();
}



/*
 * rd/wr an object
 *
 * FIXME! This code probably has a lot of cruft from the old Z/V codebase.
 *
 */
void loadsave_do_item(byte type, object_type **o_ptr, s32b flag)
{
	do_item(type, *o_ptr, flag);
}
void do_item(byte type, object_type *o_ptr, s32b flag)
{
	s32b i;
	byte old_dd;
	byte old_ds;

	if (type != FLAG_TYPE_OBJ) return;

	/* Kind */
	do_s16b(&o_ptr->k_idx, flag);

	/* Location */
	do_byte(&o_ptr->iy, flag);
	do_byte(&o_ptr->ix, flag);

	/* Type/Subtype */
	do_byte(&o_ptr->tval, flag);
	do_byte(&o_ptr->sval, flag);

	do_byte(&o_ptr->discount, flag);
	do_byte(&o_ptr->number, flag);
	do_s32b(&o_ptr->weight, flag);

	do_s16b(&o_ptr->artifact_id, flag);
	for (i = 0; i < MAX_EGO_PER_OBJ; i++)
		do_s16b(&o_ptr->ego_id[i], flag);
	do_s16b(&o_ptr->timeout, flag);

	do_s16b(&o_ptr->to_h, flag);
	do_s16b(&o_ptr->to_d, flag);
	do_s16b(&o_ptr->to_a, flag);

	do_s16b(&o_ptr->ac, flag);

    /* We do special processing of this flag when reading */
    if (flag == LS_LOAD)
    {
        do_byte(&old_dd, LS_LOAD);
        do_byte(&old_ds, LS_LOAD);
    }
    if (flag == LS_SAVE)
    {
        do_byte(&o_ptr->dd, LS_SAVE);
        do_byte(&o_ptr->ds, LS_SAVE);
    }

	do_byte(&o_ptr->ident, flag);

	do_byte(&o_ptr->marked, flag);

	/* flags */
	do_flags(&o_ptr->flags, flag);

	/* Monster holding object */
	do_s16b(&o_ptr->held_m_idx, flag);

	/* Special powers */
	do_byte(&o_ptr->xtra1, flag);
	do_s16b(&o_ptr->xtra2, flag);

	do_byte(&o_ptr->elevel, flag);
	do_s32b(&o_ptr->exp, flag);

	/* Per-object colour */
	do_byte(&o_ptr->d_attr, flag);
	do_byte(&o_ptr->x_attr, flag);

	/* Read the pseudo-id */
	do_byte(&o_ptr->sense, flag);

	/* Read the found info */
	do_byte(&o_ptr->found, flag);
	do_s16b(&o_ptr->found_aux1, flag);
	do_s16b(&o_ptr->found_aux2, flag);
	do_s16b(&o_ptr->found_aux3, flag);
	do_s16b(&o_ptr->found_aux4, flag);

	if (flag == LS_LOAD)
	{
		char buf[128];
		/* Inscription */
		do_string(buf, 128, LS_LOAD);
		/* Save the inscription */
		if (buf[0]) o_ptr->note = quark_add(buf);

		do_string(buf, 128, LS_LOAD);
		if (buf[0]) o_ptr->art_name = quark_add(buf);
	}
	if (flag == LS_SAVE)
	{
		/* Save the inscription (if any) */
		if (o_ptr->note)
		{
			do_string((char *)quark_str(o_ptr->note), 0, LS_SAVE);
		}
		else
		{
			do_string("", 0, LS_SAVE);
		}
		if (o_ptr->art_name)
		{
			do_string((char *)quark_str(o_ptr->art_name), 0, LS_SAVE);
		}
		else
		{
			do_string("", 0, LS_SAVE);
		}
	}

	if (flag == LS_SAVE) return ; 	/* Stick any more shared code before this. The rest
										   of this function is reserved for LS_LOAD's
										   cleanup functions */
	/*********** END OF LS_SAVE ***************/

}




/*
 * Read a monster
 */
void loadsave_do_monster(byte type, monster_type **m_ptr, s32b flag)
{
	do_monster(type, *m_ptr, flag);
}
void do_monster(byte type, monster_type *m_ptr, s32b flag)
{
	bool tmp;

	type = type;

	/* Read the monster race */
	do_s16b(&m_ptr->r_idx, flag);

	do_char(&m_ptr->d_char, flag);
	do_s16b(&m_ptr->sval, flag);

	do_u16b(&m_ptr->ego, flag);

	/* Read the other information */
	do_byte(&m_ptr->fy, flag);
	do_byte(&m_ptr->fx, flag);

	do_s32b(&m_ptr->hp, flag);
	do_s32b(&m_ptr->maxhp, flag);

	/* AI */
	do_u32b(&m_ptr->ai, flag);
	do_flags(&m_ptr->ai_states, flag);

	/* Flags & Spells*/
	do_flags(&m_ptr->flags, flag);
	do_flags(&m_ptr->spells, flag);
	do_byte(&m_ptr->freq_inate, flag);
	do_byte(&m_ptr->freq_spell, flag);

	do_s16b(&m_ptr->csleep, flag);
	do_byte(&m_ptr->mspeed, flag);
	do_s32b(&m_ptr->energy, flag);
	do_byte(&m_ptr->stunned, flag);
	do_byte(&m_ptr->confused, flag);
	do_byte(&m_ptr->monfear, flag);
	do_u32b(&m_ptr->smart, flag);
	do_u32b(&m_ptr->faction, flag);
	do_s16b(&m_ptr->possessor, flag);
	do_byte(&m_ptr->speed, flag);
	do_byte(&m_ptr->level, flag);
	do_s16b(&m_ptr->ac, flag);
	do_u32b(&m_ptr->exp, flag);
	do_s16b(&m_ptr->target, flag);

	do_s16b(&m_ptr->bleeding, flag);
	do_s16b(&m_ptr->poisoned, flag);

	do_s32b(&m_ptr->mflag, flag);

	do_flags(&m_ptr->inventory, flag);

	if (flag == LS_LOAD) m_ptr->mflag &= PERM_MFLAG_MASK;

	/* Attacks */
	do_flags(&m_ptr->blow, flag);

	/* Mind */
	tmp = (m_ptr->mind) ? TRUE : FALSE;
	do_char(&tmp, flag);
	if (tmp)
	{
		if (flag == LS_LOAD)
		{
			MAKE(m_ptr->mind, monster_mind);
		}
	}

	/* Special race */
	tmp = (m_ptr->sr_ptr) ? TRUE : FALSE;
	do_char(&tmp, flag);
	if (tmp)
	{
		if (flag == LS_LOAD)
		{
			MAKE(m_ptr->sr_ptr, monster_race);
		}
		do_string_nolen((char**)&m_ptr->sr_ptr->name, flag);
		do_string_nolen((char**)&m_ptr->sr_ptr->text, flag);

		do_u16b(&m_ptr->sr_ptr->hdice, flag);
		do_u16b(&m_ptr->sr_ptr->hside, flag);

		do_s16b(&m_ptr->sr_ptr->ac, flag);

		do_s16b(&m_ptr->sr_ptr->sleep, flag);
		do_byte(&m_ptr->sr_ptr->aaf, flag);
		do_byte(&m_ptr->sr_ptr->speed, flag);

		do_s32b(&m_ptr->sr_ptr->mexp, flag);

		do_s32b(&m_ptr->sr_ptr->weight, flag);

		do_byte(&m_ptr->sr_ptr->freq_inate, flag);
		do_byte(&m_ptr->sr_ptr->freq_spell, flag);

		do_flags(&m_ptr->sr_ptr->flags, flag);

		/* Attacks */
		do_flags(&m_ptr->sr_ptr->blow, flag);

		do_flags(&m_ptr->sr_ptr->body_parts, flag);

		do_byte(&m_ptr->sr_ptr->level, flag);
		do_byte(&m_ptr->sr_ptr->rarity, flag);

		do_char(&m_ptr->sr_ptr->d_char, flag);
		do_byte(&m_ptr->sr_ptr->d_attr, flag);

		do_char(&m_ptr->sr_ptr->x_char, flag);
		do_byte(&m_ptr->sr_ptr->x_attr, flag);

		do_s16b(&m_ptr->sr_ptr->max_num, flag);
		do_s16b(&m_ptr->sr_ptr->cur_num, flag);
	}

	/* Find the right monster */
	if (flag == LS_LOAD)
	{
		if (m_ptr->r_idx != 0)
			m_ptr->r_idx = lookup_race(m_ptr->d_char, m_ptr->sval);
	}
}





/*
 * Handle monster lore
 */
static void do_lore(s32b r_idx, s32b flag)
{
	monster_race *r_ptr = &r_info[r_idx];

	/* Count sights/deaths/kills */
	do_s16b(&r_ptr->r_sights, flag);
	do_s16b(&r_ptr->r_deaths, flag);
	do_s16b(&r_ptr->r_pkills, flag);
	do_s16b(&r_ptr->r_tkills, flag);

	/* Count wakes and ignores */
	do_byte(&r_ptr->r_wake, flag);
	do_byte(&r_ptr->r_ignore, flag);

	/* Extra stuff */
	do_byte(&r_ptr->r_xtra1, flag);
	do_byte(&r_ptr->r_xtra2, flag);

	/* Count drops */
	do_byte(&r_ptr->r_drop_gold, flag);
	do_byte(&r_ptr->r_drop_item, flag);

	/* Count spells */
	do_byte(&r_ptr->r_cast_inate, flag);
	do_byte(&r_ptr->r_cast_spell, flag);

	/* Count blows of each type */
	do_byte(&r_ptr->r_blows[0], flag);
	do_byte(&r_ptr->r_blows[1], flag);
	do_byte(&r_ptr->r_blows[2], flag);
	do_byte(&r_ptr->r_blows[3], flag);

	/* Memorize flags */
	do_flags(&r_ptr->r_flags, flag);

	/* Read the "Racial" monster tmp16b per level */
	do_s16b(&r_ptr->max_num, flag);
}




/*
 * Read a store
 */
void loadsave_do_store(byte type, store_type **str, s32b flag)
{
	do_store(type, *str, flag);
}
void do_store(byte type, store_type *str, s32b flag)
{
	/* Some basic info */
	do_u16b(&str->st_idx, flag);

	do_s32b(&str->store_open, flag);
	do_s16b(&str->insult_cur, flag);
	do_u16b(&str->owner, flag);

	do_s16b(&str->good_buy, flag);
	do_s16b(&str->bad_buy, flag);

	/* Last visit */
	do_s32b(&str->last_visit, flag);

	/* Items */
	do_flags(&str->stock, flag);
}

/*
 * RNG state
 */
static void do_randomizer(s32b flag)
{
	s32b i;

	/* Place */
	do_u16b(&Rand_place, flag);

	/* State */
	for (i = 0; i < RAND_DEG; i++)
	{
		do_u32b(&Rand_state[i], flag);
	}

	/* Accept */
	if (flag == LS_LOAD)
	{
		Rand_quick = FALSE;
	}
}

/*
 * Handle options
 *
 * Normal options are stored as a set of 256 bit flags,
 * plus a set of 256 bit masks to indicate which bit flags were defined
 * at the time the savefile was created.  This will allow new options
 * to be added, and old options to be removed, at any time, without
 * hurting old savefiles.
 *
 * The window options are stored in the same way, but note that each
 * window gets 32 options, and their order is fixed by certain defines.
 */
static void do_options(s32b flag)
{
	s32b i;

	/*** Special info */

	/* Read "delay_factor" */
	do_byte(&delay_factor, flag);

	/* Read "hitpoint_warn" */
	do_byte(&hitpoint_warn, flag);

	/*** Cheating options ***/
	if (flag == LS_LOAD)		/* There *MUST* be some nice way to unify this! */
	{
		u16b c;
		do_u16b(&c, LS_LOAD);
		if (c & 0x0002) wizard = TRUE;
		cheat_peek = (c & 0x0100) ? TRUE : FALSE;
		cheat_hear = (c & 0x0200) ? TRUE : FALSE;
		cheat_room = (c & 0x0400) ? TRUE : FALSE;
		cheat_xtra = (c & 0x0800) ? TRUE : FALSE;
		cheat_know = (c & 0x1000) ? TRUE : FALSE;
		cheat_live = (c & 0x2000) ? TRUE : FALSE;
	}
	if (flag == LS_SAVE)
	{
		u16b c = 0;
		if (wizard) c |= 0x0002;
		if (cheat_peek) c |= 0x0100;
		if (cheat_hear) c |= 0x0200;
		if (cheat_room) c |= 0x0400;
		if (cheat_xtra) c |= 0x0800;
		if (cheat_know) c |= 0x1000;
		if (cheat_live) c |= 0x2000;
		do_u16b(&c, LS_SAVE);
	}

	do_char(&autosave_l, flag);
	do_char(&autosave_t, flag);
	do_s16b(&autosave_freq, flag);

	for (i = 0; i < ANGBAND_TERM_MAX; i++)
		do_flags(&window_flag[i], flag);
}


/*
 * Handle player inventory
 *
 * FIXME! This function probably could be unified better
 * Note that the inventory is "re-sorted" later by "dungeon()".
 */
static bool do_inventory(s32b flag)
{
	do_flags(&p_ptr->inventory, flag);

	if (flag == LS_LOAD)
	{
		for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
		{
			if (inven_nb >= INVEN_PACK)
			{
				/* One more item */
				equip_cnt++;
			}
			else
				inven_cnt++;
		}
		end_inventory();
	}
	/* Success */
	return (TRUE);
}



/*
 * Read the saved messages
 */
static void do_messages(s32b flag)   /* FIXME! We should be able to unify this better */
{
	s32b i;
	char buf[128];
	byte color, type;

	s16b num;

	if (flag == LS_SAVE) num = (compress_savefile &&
		                            (message_num() > 40)) ? 40 : message_num();

	/* Total */
	do_s16b(&num, flag);

	/* Read the messages */
	if (flag == LS_LOAD)
	{
		for (i = 0; i < num; i++)
		{
			/* Read the message */
			do_string(buf, 128, LS_LOAD);
			do_byte(&color, flag);
			do_byte(&type, flag);

			/* Save the message */
			message_add(type, buf, color);
		}
	}
	if (flag == LS_SAVE)
	{
		byte holder;
		for (i = num - 1; i >= 0; i--)
		{
			do_string((char *)message_str((s16b)i), 0, LS_SAVE);
			holder = message_color((s16b)i);
			do_byte(&holder, flag);
			holder = message_type((s16b)i);
			do_byte(&holder, flag);
		}
	}
}

/*
 * Handle dungeon
 *
 * The monsters/objects must be loaded in the same order
 * that they were stored, since the actual indexes matter.
 */
static bool do_dungeon(s32b flag)
{
	s32b i;

	cave_type *c_ptr;

	/* Read specific */
	u16b tmp16b = 0;

	my_sentinel("Before do_dungeon", 324, flag);

	/* Header info */
	do_s16b(&dun_level, flag);
	do_byte(&dungeon_type, flag);
	do_s16b(&num_repro, flag);
	do_s16b(&p_ptr->py, flag);
	do_s16b(&p_ptr->px, flag);
	do_s16b(&cur_hgt, flag);
	do_s16b(&cur_wid, flag);
	do_s16b(&max_panel_rows, flag);
	do_s16b(&max_panel_cols, flag);

	do_flags(&dungeon_flags->flags, flag);

	/* Last teleportation */
	do_s16b(&last_teleportation_y, flag);
	do_s16b(&last_teleportation_y, flag);

	/* Spell effects */
	tmp16b = MAX_EFFECTS;
	do_u16b(&tmp16b, flag);

	if ((flag == LS_LOAD) && (tmp16b > MAX_EFFECTS))
	{
		quit("Too many spell effects");
	}

	for (i = 0; i < tmp16b; ++i)
	{
		do_s16b(&effects[i].who, flag);
		do_s16b(&effects[i].type, flag);
		do_s16b(&effects[i].dam, flag);
		do_s16b(&effects[i].time, flag);
		do_u32b(&effects[i].flags, flag);
		do_s16b(&effects[i].cx, flag);
		do_s16b(&effects[i].cy, flag);
		do_s16b(&effects[i].rad, flag);
	}

	/* TO prevent bugs with evolving dungeons */
	for (i = 0; i < 100; i++)
	{
		do_s16b(&floor_type[i], flag);
		do_s16b(&fill_type[i], flag);
	}

	do_grid(flag);

	/*** Monsters ***/
	/* Read the monsters */
	do_flags(&monst_list, flag);

	if (flag == LS_LOAD)
	{
		for_flags(&monst_list);
		{
			s32b m_idx = __key;
			monster_type *m_ptr;
			monster_race *r_ptr;

			/* Acquire monster */
			m_ptr = get_monster(m_idx);

			/* Access grid */
			c_ptr = &cave[m_ptr->fy][m_ptr->fx];

			/* Mark the location */
			c_ptr->m_idx = m_idx;

			/* Controlled ? */
			if (m_ptr->mflag & MFLAG_CONTROL)
				p_ptr->control = m_idx;

			/* Access race */
			r_ptr = &r_info[m_ptr->r_idx];

			/* Count XXX XXX XXX */
			r_ptr->cur_num++;
		}
		end_for_flags();
	}

	do_flags(&keep_monst_list, flag);

	/*** Success ***/

	/* The dungeon is ready */
	if (flag == LS_LOAD) character_dungeon = TRUE;

	/* Success */
	return (TRUE);
}

/* Returns TRUE if we successfully load the dungeon */
bool load_dungeon(s32b dungeon, s32b level, s32b *saved_turn)
{
	char ext[30];
	char name[1024];
	byte old_dungeon_type = dungeon_type;
	s16b old_dun = dun_level;

	/* Construct name */
	if (level)
		sprintf(ext, "dun%ld.%ld", dungeon, level);
	else
		sprintf(ext, "wild%ld.%ld", p_ptr->wilderness_y, p_ptr->wilderness_x);
	path_build(name, 1024, savefile, ext);

	/* Open the file */
	fff = my_fopen(name, "rbz");

	if (fff == NULL)
	{
		dun_level = old_dun;
		dungeon_type = old_dungeon_type;

		if (wizard)
			cmsg_format(TERM_VIOLET,
						"load_dungeon() couldn't open file '%s': %s",
						name, PHYSFS_getLastError());

		return (FALSE);
	}

	/* Read the dungeon */
	if (!do_dungeon(LS_LOAD))
	{
		dun_level = old_dun;
		dungeon_type = old_dungeon_type;

		my_fclose(fff);
		return (FALSE);
	}
	do_s32b(saved_turn, LS_LOAD);

	dun_level = old_dun;
	dungeon_type = old_dungeon_type;

	/* Done */
	my_fclose(fff);
	return (TRUE);
}

void do_blocks(s32b flag)
/* Handle blocked-allocation stuff for quests and lua stuff
   This depends on a dyn_tosave array of s32b's. Adjust as needed
   if other data structures are desirable. This also is not hooked
   in yet. Ideally, plug it near the end of the savefile.
 */
{
	s16b numblks, n_iter = 0; 	/* How many blocks do we have? */
	do_s16b(&numblks, flag);
	while (n_iter < numblks)
	{
		/*	do_s32b(dyn_tosave[n_iter], flag); */
		n_iter++;
	}
	my_sentinel("In blocked-allocation area", 37, flag);
}

/*
 * Actually read the savefile
 */
static bool do_savefile_aux(s32b flag)
{
	s32b i, j;

	byte tmp8u;
	u16b tmp16u;
	u32b tmp32u;

	/* Mention the savefile version */
	if (flag == LS_LOAD)
	{
		if (vernum < 100)
		{
			note(format("Savefile version %lu too old! ", vernum));
			return FALSE;
		}
		else
		{
			note(format("Loading version %lu savefile... ", vernum));
		}
	}
	if (flag == LS_SAVE)
	{
		sf_when = time((time_t *) 0); 	/* Note when file was saved */
		sf_xtra = 0L; 			/* What the hell is this? */
		sf_saves++; 				/* Increment the saves ctr */
	}

	/* Handle version bytes. FIXME! DG wants me to change this all around */
	if (flag == LS_LOAD)
	{
		u32b mt32b;
		byte mtbyte;
		char buf[SAVEFILE_MAGIC_LEN + 1];

		/* Discard all this, we've already read it */
		do_string(buf, SAVEFILE_MAGIC_LEN + 1, flag);
		do_u32b(&mt32b, flag);
		do_byte(&mtbyte, flag);
	}
	if (flag == LS_SAVE)
	{
		u32b saver;
		saver = SAVEFILE_VERSION;
		do_string(SAVEFILE_MAGIC, SAVEFILE_MAGIC_LEN + 1, flag);
		do_u32b(&saver, flag);
		tmp8u = (byte)rand_int(256);
		do_byte(&tmp8u, flag); 	/* 'encryption' */
	}

	/* Operating system info? Not really. This is just set to 0L */
	do_u32b(&sf_xtra, flag);

	/* Time of last save */
	do_u32b(&sf_when, flag);

	/* Number of past lives */
	do_u16b(&sf_lives, flag);

	/* Number of times saved */
	do_u16b(&sf_saves, flag);

	/* Game module */
	if (flag == LS_SAVE)
		strcpy(loaded_game_module, game_module);
	do_string(loaded_game_module, 80, flag);

	/* Read RNG state */
	do_randomizer(flag);
	if ((flag == LS_LOAD) && (arg_fiddle)) note("Loaded Randomizer Info");

	/* Automatizer state */
	do_char(&automatizer_enabled, flag);

	/* Then the options */
	do_options(flag);
	if ((flag == LS_LOAD) && (arg_fiddle)) note("Loaded Option Flags");

	/* Then the "messages" */
	do_messages(flag);
	if ((flag == LS_LOAD) && (arg_fiddle)) note("Loaded Messages");

	/* Monster Memory */
	if (flag == LS_SAVE) tmp16u = max_r_idx;
	do_u16b(&tmp16u, flag);

	/* Incompatible save files */
	if ((flag == LS_LOAD) && (tmp16u > max_r_idx))
	{
		note(format("Too many (%u) monster races!", tmp16u));
		return (FALSE);
	}

	/* Read the available records */
	for (i = 0; i < tmp16u; i++)
	{
		monster_race *r_ptr;

		/* Read the lore */
		do_lore(i, flag);

		/* Access that monster */
		r_ptr = &r_info[i]; 		/* FIXME! Why the hell are we doing this? */
	}
	if ((flag == LS_LOAD) && (arg_fiddle)) note("Loaded Monster Memory");

	/* Object Memory */
	if (flag == LS_SAVE) tmp16u = max_k_idx;
	do_u16b(&tmp16u, flag);

	/* Read the object memory */
	for (i = 0; i < tmp16u; i++) do_xtra(i, flag);
	if ((flag == LS_LOAD) && (arg_fiddle)) note("Loaded Object Memory");


	{
		u16b max_towns_ldsv;
		u16b max_quests_ldsv;
		if (flag == LS_SAVE) max_towns_ldsv = max_towns;
		/* Number of towns */
		do_u16b(&max_towns_ldsv, flag);
		/* Incompatible save files */
		if ((flag == LS_LOAD) && (max_towns_ldsv > max_towns))
		{
			note(format("Too many (%u) towns!", max_towns_ldsv));
			return (FALSE);
		}
		/* Min of random towns */
		if (flag == LS_SAVE) max_towns_ldsv = TOWN_RANDOM;
		do_u16b(&max_towns_ldsv, flag);
		/* Incompatible save files */
		if ((flag == LS_LOAD) && (max_towns_ldsv != TOWN_RANDOM))
		{
			note(format("Different random towns base (%u)!", max_towns_ldsv));
			return (FALSE);
		}

		for (i = 0; i < max_towns; i++)
		{
			do_char(&town_info[i].destroyed, flag);
			do_flags(&town_info[i].stores, flag);

			if (i >= TOWN_RANDOM)
			{
				do_u32b(&town_info[i].seed, flag);
				do_byte(&town_info[i].numstores, flag);
				do_byte(&town_info[i].flags, flag);
			}
		}

		/* Number of dungeon */
		if (flag == LS_SAVE) max_towns_ldsv = max_d_idx;
		do_u16b(&max_towns_ldsv, flag);

		/* Incompatible save files */
		if ((flag == LS_LOAD) && (max_towns_ldsv > max_d_idx))
		{
			note(format("Too many dungeon types (%u)!", max_towns_ldsv));
			return (FALSE);
		}

		/* Number of towns per dungeon */
		if (flag == LS_SAVE) max_quests_ldsv = TOWN_DUNGEON;
		do_u16b(&max_quests_ldsv, flag);
		/* Incompatible save files */
		if ((flag == LS_LOAD) && (max_quests_ldsv > TOWN_DUNGEON))
		{
			note(format("Too many town per dungeons (%u)!", max_quests_ldsv));
			return (FALSE);
		}

		for (i = 0; i < max_towns_ldsv; i++)
		{
			for (j = 0; j < max_quests_ldsv; j++)
			{
				do_s16b(&(d_info[i].t_idx[j]), flag);
				do_s16b(&(d_info[i].t_level[j]), flag);
			}
			do_s16b(&(d_info[i].t_num), flag);
		}

		if (flag == LS_SAVE) max_quests_ldsv = MAX_Q_IDX_INIT;
		/* Number of quests */
		do_u16b(&max_quests_ldsv, flag);

		/* Incompatible save files */
		if ((flag == LS_LOAD) && (max_quests_ldsv > MAX_Q_IDX_INIT))
		{
			note(format("Too many (%u) quests!", max_quests_ldsv));
			return (FALSE);
		}

		for (i = 0; i < max_quests_ldsv; i++)
		{
			do_s16b(&quest[i].status, flag);
			for (j = 0; j < 4; j++)
			{
				do_s32b(&(quest[i].data[j]), flag);
			}

			/* Init the hooks */
			if ((flag == LS_LOAD) && (quest[i].type == HOOK_TYPE_C)) quest[i].init(i);
		}

		/* Position in the wilderness */
		do_s32b(&p_ptr->wilderness_x, flag);
		do_s32b(&p_ptr->wilderness_y, flag);
		do_char(&p_ptr->wild_mode, flag);
		do_char(&p_ptr->old_wild_mode, flag);

		{
			s32b wild_x_size, wild_y_size;
			if (flag == LS_SAVE)
			{
				wild_x_size = max_wild_x;
				wild_y_size = max_wild_y;
			}
			/* Size of the wilderness */
			do_s32b(&wild_x_size, flag);
			do_s32b(&wild_y_size, flag);
			/* Incompatible save files */
			if ((flag == LS_LOAD) &&
			                ((wild_x_size > max_wild_x) || (wild_y_size > max_wild_y)))
			{
				note(format("Wilderness is too big (%u/%u)!",
				            wild_x_size, wild_y_size));
				return (FALSE);
			}
			/* Wilderness seeds */
			for (i = 0; i < wild_x_size; i++)
			{
				for (j = 0; j < wild_y_size; j++)
				{
					do_s16b (&wild_map[j][i].mimic,    flag);
					do_u32b (&wild_map[j][i].seed,     flag);
					do_u16b (&wild_map[j][i].entrance, flag);
					do_char (&wild_map[j][i].known,    flag);
					do_flags(&wild_map[j][i].flags,    flag);
				}
			}
		}
	}
	if ((flag == LS_LOAD) && (arg_fiddle)) note("Loaded Quests");

	/* Load the random artifacts. */
	if (flag == LS_SAVE) tmp16u = MAX_RANDARTS;
	do_u16b(&tmp16u, flag);
	if ((flag == LS_LOAD) && (tmp16u > MAX_RANDARTS))
	{
		note(format("Too many (%u) random artifacts!", tmp16u));
		return (FALSE);
	}
	for (i = 0; i < tmp16u; i++)
	{
		random_artifact *ra_ptr = &random_artifacts[i];

		do_string(ra_ptr->name_full, 80, flag);
		do_string(ra_ptr->name_short, 80, flag);
		do_byte(&ra_ptr->level, flag);
		do_byte(&ra_ptr->attr, flag);
		do_u32b(&ra_ptr->cost, flag);
		do_byte(&ra_ptr->activation, flag);
		do_byte(&ra_ptr->generated, flag);
	}

	/* Load the Artifacts */
	if (flag == LS_SAVE) tmp16u = max_a_idx;
	do_u16b(&tmp16u, flag);
	/* Incompatible save files */
	if ((flag == LS_LOAD) && (tmp16u > max_a_idx))
	{
		note(format("Too many (%u) artifacts!", tmp16u));
		return (FALSE);
	}

	/* Read the artifact flags */
	for (i = 0; i < tmp16u; i++)
	{
		do_s16b(&(&a_info[i])->cur_num, flag);
		do_bool(&(&a_info[i])->known, flag);
	}
	if ((flag == LS_LOAD) && arg_fiddle) note("Loaded Artifacts");

	/* Load the Traps */
	if (flag == LS_SAVE) tmp16u = max_t_idx;
	do_u16b(&tmp16u, flag);

	/* Incompatible save files */
	if ((flag == LS_LOAD) && (tmp16u > max_t_idx))
	{
		note(format("Too many (%u) traps!", tmp16u));
		return (FALSE);
	}

	/* Read the extra stuff */
	if (!do_extra(flag))
		return FALSE;
	if ((flag == LS_LOAD) && arg_fiddle) note("Loaded extra information");


	/* player_hp array */
	if (flag == LS_SAVE) tmp16u = PY_MAX_LEVEL;
	do_u16b(&tmp16u, flag);
	/* Incompatible save files */
	if ((flag == LS_LOAD) && (tmp16u > PY_MAX_LEVEL))
	{
		note(format("Too many (%u) hitpoint entries!", tmp16u));
		return (FALSE);
	}

	/* Read the player_hp array */
	for (i = 0; i < tmp16u; i++)
	{
		do_s16b(&player_hp[i], flag);
	}

	/* Read the pet command settings */
	do_byte(&p_ptr->pet_follow_distance, flag);
	do_byte(&p_ptr->pet_open_doors, flag);
	do_byte(&p_ptr->pet_pickup_items, flag);

	/* Read the inventory */
	if (!do_inventory(flag) && (flag == LS_LOAD))	/* do NOT reverse this ordering */
	{
		note("Unable to read inventory");
		return (FALSE);
	}


	if (flag == LS_SAVE)
	{
		/* Ask the hooks how much space they want to save */
		extra_savefile_parts = 0;
		process_hooks(HOOK_SAVE_REGISTER, "()");
		tmp32u = extra_savefile_parts;
	}
	do_u32b(&tmp32u, flag);
	if (flag == LS_SAVE)
	{
		/* Save the stuff */
		process_hooks(HOOK_SAVE_GAME, "()");
	}

	if (flag == LS_LOAD)
	{
		u32b len = tmp32u;

		while (len)
		{
			char key_buf[200];

			/* Load a key */
			switch(load_key(key_buf))
			{
			case SAVEFILE_NIL:
				{
					process_hooks(HOOK_LOAD_GAME, "(s,n)", key_buf);
					break;
				}
			case SAVEFILE_NUMBER:
				{
					s32b val;
					do_s32b(&val, LS_LOAD);
					process_hooks(HOOK_LOAD_GAME, "(s,d)", key_buf, val);
					break;
				}
			case SAVEFILE_STRING:
				{
					char *str;
					u16b len;

					/* Get length */
					do_u16b(&len, LS_LOAD);

					/* Allocate & get */
					C_MAKE(str, len + 1, char);
					do_string(str, len + 1, LS_LOAD);

					process_hooks(HOOK_LOAD_GAME, "(s,s)", key_buf, str);

					/* Get rid of it now */
					C_FREE(str, len + 1, char);
					break;
				}
			case SAVEFILE_FLAGS:
				{
					flags_type *flags = flag_new(2);

					do_flags(flags, LS_LOAD);

					process_hooks(HOOK_LOAD_GAME, "(s,F)", key_buf, flags);

					break;
				}
			}
			len--;
		}
	}

	/* I'm not dead yet... */
	if (!death)
	{
		/* Dead players have no dungeon */
		if (flag == LS_LOAD) note("Restoring Dungeon...");
		if ((flag == LS_LOAD) && (!do_dungeon(LS_LOAD)))
		{
			note("Error reading dungeon data");
			return (FALSE);
		}
		if (flag == LS_SAVE) do_dungeon(LS_SAVE);
		my_sentinel("Before ghost data", 435, flag);
		my_sentinel("After ghost data", 320, flag);
	}

	{
		byte foo = 0;
		if (flag == LS_SAVE)
		{
			/*
			 * Safety Padding. It's there
			 * for a good reason. Trust me on
			 * this. Keep this at the *END*
			 * of the file, and do *NOT* try to
			 * read it. Insert any new stuff before
			 * this position.
			 */
			do_byte(&foo, LS_SAVE);
		}
	}

	/* Success */
	return (TRUE);
}


/*
 * Actually read the savefile
 */
errr rd_savefile(bool quickstart)
{
	errr err = 0;

	char safe[1024];
	char panic_fname[1024];

	if (quickstart)
		path_build(safe, 1024, savefile, "quickstart");
	else
		path_build(safe, 1024, savefile, "savefile");

	if (!panicload)
	{
		/* The savefile is a binary file */
		fff = my_fopen(safe, "rbz");
	}
	else
	{
		path_build(panic_fname, 1024, savefile, "panic");

		/* Open panic save file */
		fff = my_fopen(panic_fname, "rbz");
	}

	/* Paranoia */
	if (!fff)
	{
		if (wizard)
			cmsg_format(TERM_VIOLET,
						"rd_savefile() couldn't open file '%s': %s",
						panic_fname, PHYSFS_getLastError());

		return ( -1);
	}

	/* Call the sub-function */
	err = !do_savefile_aux(LS_LOAD);

	/* Close the file */
	my_fclose(fff);

	/* Result */
	return (err);
}

static void do_grid(s32b flag)
{
	s32b y = 0, x = 0;
	cave_type *c_ptr;
	s32b ymax = cur_hgt, xmax = cur_wid;

	my_sentinel("Before grid", 17, flag);

	for (y = 0; y < ymax; y++)
	{
		for (x = 0; x < xmax; x++)
		{
			c_ptr = &cave[y][x];

			do_u16b(&c_ptr->info, flag);
			do_s16b(&c_ptr->feat, flag);
			do_s16b(&c_ptr->mimic, flag);
			do_flags(&c_ptr->flags, flag);
			do_flags(&c_ptr->inventory, flag);
			do_flags(&c_ptr->activations, flag);
			do_s16b(&c_ptr->mana, flag);
			do_s16b(&c_ptr->effect, flag);
			do_u32b(&c_ptr->los_turn, flag);
			do_byte(&c_ptr->los_dist, flag);
		}
	}
	my_sentinel("In grid", 36, flag);
}

static void my_sentinel(char *place, u16b value, s32b flag)
/* This function lets us know exactly where a savefile is
   broken by reading/writing conveniently a sentinel at this
   spot */
{
	if (flag == LS_SAVE)
	{
		do_u16b(&value, flag);
		return;
	}
	if (flag == LS_LOAD)
	{
		u16b found;
		do_u16b(&found, flag);
		if (found == value)		/* All is good */
			return;
		/* All is bad */
		note(format("Savefile broken %s", place));
		return;
	}
	note(format("Impossible has occurred")); 	/* Programmer error */
	exit(0);
}

/********** Variable savefile stuff **************/

/*
 * Add num slots to the savefile
 */
void register_savefile(s32b num)
{
	extra_savefile_parts += (num > 0) ? num : 0;
}

static void save_key(char *key, byte type)
{
	byte len = strlen(key);

	do_byte(&len, LS_SAVE);
	while (*key)
	{
		do_char(key, LS_SAVE);
		key++;
	}
	do_byte(&type, LS_SAVE);
}

void save_nil_key(char *key)
{
	save_key(key, SAVEFILE_NIL);
}

void save_number_key(char *key, s32b val)
{
	save_key(key, SAVEFILE_NUMBER);
	do_s32b(&val, LS_SAVE);
}

void save_string_key(char *key, cptr val)
{
	u16b len = strlen(val);

	save_key(key, SAVEFILE_STRING);
	do_u16b(&len, LS_SAVE);
	do_string((char*)val, len + 1, LS_SAVE);
}

void save_flagset_key(char *key, flags_type *f)
{
	save_key(key, SAVEFILE_FLAGS);
	do_flags(f, LS_SAVE);
}

/* Load a key and its type */
byte load_key(char *key)
{
	byte len, i = 0;
	byte type;

	do_byte(&len, LS_LOAD);
	while (i < len)
	{
		do_char(&key[i], LS_LOAD);
		i++;
	}
	key[i] = '\0';
	do_byte(&type, LS_LOAD);

	return type;
}
