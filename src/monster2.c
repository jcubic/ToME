/* File: monster2.c */

/* Purpose: misc code for monsters */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

#define MAX_HORROR 20
#define MAX_FUNNY 22
#define MAX_COMMENT 5

/* Monster gain a few levels ? */
void monster_check_experience(s32b m_idx, bool silent)
{
	monster_type *m_ptr = get_monster(m_idx);
	monster_race *r_ptr = &r_info[m_ptr->r_idx];
	char m_name[80];

	/* Get the name */
	monster_desc(m_name, m_ptr, 0);

	/* Gain levels while possible */
	while ((m_ptr->level < MONSTER_LEVEL_MAX) &&
	                (m_ptr->exp >= (MONSTER_EXP(m_ptr->level + 1))))
	{
		/* Gain a level */
		m_ptr->level++;

		if (m_ptr->ml && (!silent)) cmsg_format(TERM_L_BLUE, "%^s gains a level.", m_name);

		/* Gain hp */
		if (magik(80))
		{
			m_ptr->maxhp += r_ptr->hside;
			m_ptr->hp += r_ptr->hside;
		}

		/* Gain speed */
		if (magik(40))
		{
			s32b speed = randint(2);
			m_ptr->speed += speed;
			m_ptr->mspeed += speed;
		}

		/* Gain ac */
		if (magik(50))
		{
			m_ptr->ac += (r_ptr->ac / 10) ? r_ptr->ac / 10 : 1;
		}

		/* Gain melee power */
		if (magik(30))
		{
/*DGDGDGDG			s32b i = rand_int(3), try = 20;

			while ((try--) && !m_ptr->blow[i].d_dice)
			{
				i = rand_int(3);
			}

			m_ptr->blow[i].d_dice++;*/
		}
	}
}

/* Monster gain some xp */
void monster_gain_exp(s32b m_idx, u32b exp, bool silent)
{
	monster_type *m_ptr = get_monster(m_idx);

	m_ptr->exp += exp;
	if (wizard)
	{
		char m_name[80];

		/* Get the name */
		monster_desc(m_name, m_ptr, 0x80);

		if (!silent) msg_format("%^s gains %ld exp.", m_name, exp);
	}

	monster_check_experience(m_idx, silent);
}

void monster_set_level(s32b m_idx, s32b level)
{
	monster_type *m_ptr = get_monster(m_idx);

	if (level > 150) level = 150;

	if (m_ptr->level < level)
	{
		m_ptr->exp = MONSTER_EXP(level);
		monster_check_experience(m_idx, TRUE);
	}
}

/* Will add, sub, .. */
s32b modify_aux(s32b a, s32b b, char mod)
{
	switch (mod)
	{
	case MEGO_ADD:
		return (a + b);
		break;
	case MEGO_SUB:
		return (a - b);
		break;
	case MEGO_FIX:
		return (b);
		break;
	case MEGO_PRC:
		return (a * b / 100);
		break;
	default:
		msg_format("WARNING, unmatching MEGO(%d).", mod);
		return (0);
	}
}

/* Is this ego ok for this monster ? */
bool mego_ok(s32b r_idx, s32b ego)
{
	monster_ego *re_ptr = &re_info[ego];
	monster_race *r_ptr = &r_info[r_idx];
	bool ok = FALSE;
	s32b i;

	if (!r_idx && !ego) return FALSE;

	/* needed flags */
	if (flag_used(&re_ptr->flags) && (!flag_contains(&r_ptr->flags, &re_ptr->flags))) return FALSE;

	/* unwanted flags */
	if (flag_used(&re_ptr->hflags) && (flag_contains(&r_ptr->flags, &re_ptr->hflags))) return FALSE;

	/* Need good race -- IF races are specified */
	if (re_ptr->r_char[0])
	{
		for (i = 0; i < 5; i++)
		{
			if (r_ptr->d_char == re_ptr->r_char[i]) ok = TRUE;
		}
		if (!ok) return FALSE;
	}
	if (re_ptr->nr_char[0])
	{
		for (i = 0; i < 5; i++)
		{
			if (r_ptr->d_char == re_ptr->nr_char[i]) return (FALSE);
		}
	}

	/* Passed all tests ? */
	return TRUE;
}

/* Choose an ego type */
static s32b pick_ego_monster(s32b r_idx, s32b force_faction)
{
	/* Assume no ego */
	s32b ego = 0, lvl;
	s32b tries = max_re_idx + 10;
	monster_ego *re_ptr;

	/* No monster ego to choose from */
	if (max_re_idx <= 1) return 0;

	/* If we need to for an ego, bypass all restrictions */
	if (has_flag(dungeon_flags, FLAG_DUNGEON_FORCE_MONSTER_EGO))
	{
		open_lua_functions_registry(flag_get(&dungeon_flags->flags, FLAG_DUNGEON_FORCE_MONSTER_EGO), flag_get2(&dungeon_flags->flags, FLAG_DUNGEON_FORCE_MONSTER_EGO));
		call_lua(NULL, "(d)", "d", r_idx, &ego);
		close_lua_functions_registry();

		/* Still not accept invalid ones */
		if (!mego_ok(r_idx, ego)) return 0;
		return ego;
	}

	/* No townspeople ego */
	if ((!has_flag(&r_info[r_idx], FLAG_ALWAYS_EGO)) && (!r_info[r_idx].level)) return 0;

	/* First are we allowed to find an ego */
	if ((!has_flag(&r_info[r_idx], FLAG_ALWAYS_EGO)) && (!magik(MEGO_CHANCE))) return 0;

	/* Lets look for one */
	while (tries--)
	{
		/* Pick one */
		ego = rand_range(1, max_re_idx - 1);
		re_ptr = &re_info[ego];

		if (force_faction != -1)
		{
			s32b faction = FACTION_DUNGEON;

			if (has_flag(re_ptr, FLAG_FACTION)) faction = get_flag(re_ptr, FLAG_FACTION);
			else if (has_flag(&r_info[r_idx], FLAG_FACTION)) faction = get_flag(&r_info[r_idx], FLAG_FACTION);

			if (get_faction_friendliness(force_faction, faction) <= 0) continue;
		}

		/*  No hope so far */
		if (!mego_ok(r_idx, ego)) continue;

		/* Not too much OoD */
		lvl = r_info[r_idx].level;
		MODIFY(lvl, re_ptr->level, 0);
		lvl -= ((dun_level / 2) + (rand_int(dun_level / 2)));
		if (lvl < 1) lvl = 1;
		if (rand_int(lvl)) continue;

		/* Each ego types have a rarity */
		if (rand_int(re_ptr->rarity)) continue;

		/* We finally got one ? GREAT */
		return ego;
	}

	/* Found none ? so sad, well no ego for the time being */
	return 0;
}

/*
 * Return a (monster_race*) with the combination of the monster
 * properties and the ego type
 */
static monster_race __race;
monster_race* race_info_idx(s32b r_idx, s32b ego)
{
	monster_ego *re_ptr = &re_info[ego];
	monster_race *r_ptr = &r_info[r_idx], *nr_ptr = &__race;
	s32b i;

	/* No work needed */
	if (!ego) return r_ptr;

	/* Copy the base monster */
	(void) COPY(nr_ptr, r_ptr, monster_race);

	/* Make sure to not mess up flags */
	flag_init(&nr_ptr->flags,2);
	flag_copy(&nr_ptr->flags, &r_ptr->flags);
	flag_init(&nr_ptr->spells,2);
	flag_copy(&nr_ptr->spells, &r_ptr->spells);
	flag_init(&nr_ptr->r_flags,2);
	flag_copy(&nr_ptr->r_flags, &r_ptr->r_flags);

	/* Adjust the values */
	for (i = 0; i <= flag_max_key(&re_ptr->blow); i++)
	{
		s32b j, k;
		monster_blow *blow_r = flag_get_blow(&nr_ptr->blow, i);
		monster_blow *blow_e = flag_get_blow(&re_ptr->blow, i);

		/* DGDGDGDG add the blow if not present on the monster */
		if ((blow_r != NULL) && (blow_e != NULL))
		{
			j = modify_aux(blow_r->d_dice, blow_e->d_dice, flag_get(&re_ptr->blowm_dice, i));
			if (j < 0) j = 0;
			k = modify_aux(blow_r->d_side, blow_e->d_side, flag_get(&re_ptr->blowm_side, i));
			if (k < 0) k = 0;

			blow_r->d_dice = j;
			blow_r->d_side = k;

			if (blow_e->method) blow_r->method = blow_e->method;
			if (blow_e->effect) blow_r->effect = blow_e->effect;
		}
	}

	MODIFY(nr_ptr->hdice, re_ptr->hdice, 1);
	MODIFY(nr_ptr->hside, re_ptr->hside, 1);

	MODIFY(nr_ptr->ac, re_ptr->ac, 0);

	MODIFY(nr_ptr->sleep, re_ptr->sleep, 0);

	MODIFY(nr_ptr->aaf, re_ptr->aaf, 1);

	MODIFY(nr_ptr->speed, re_ptr->speed, 50);
	MODIFY(nr_ptr->mexp, re_ptr->mexp, 0);

	MODIFY(nr_ptr->weight, re_ptr->weight, 10);

	nr_ptr->freq_inate = (nr_ptr->freq_inate > re_ptr->freq_inate)
	                     ? nr_ptr->freq_inate : re_ptr->freq_inate;
	nr_ptr->freq_spell = (nr_ptr->freq_spell > re_ptr->freq_spell)
	                     ? nr_ptr->freq_spell : re_ptr->freq_spell;

	MODIFY(nr_ptr->level, re_ptr->level, 1);

	/* Take off some flags */
	flag_del(&nr_ptr->flags, &re_ptr->nflags);

	/* Add some flags */
	flag_add(&nr_ptr->flags, &re_ptr->mflags);

	/* Take off some spells */
	if (re_ptr->remove_all_spells)
		flag_empty(&nr_ptr->spells);
	else
		flag_del(&nr_ptr->spells, &re_ptr->nsflags);

	/* Add some spells */
	flag_add(&nr_ptr->spells, &re_ptr->msflags);

	/* Change the char/attr is needed */
	if (re_ptr->d_char != MEGO_CHAR_ANY)
	{
		nr_ptr->d_char = re_ptr->d_char;
		nr_ptr->x_char = re_ptr->d_char;
	}
	if (re_ptr->d_attr != MEGO_CHAR_ANY)
	{
		nr_ptr->d_attr = re_ptr->d_attr;
		nr_ptr->x_attr = re_ptr->d_attr;
	}

	/* And finanly return a pointer to a fully working monster race */
	return nr_ptr;
}

static cptr horror_desc[MAX_HORROR] =
{
	"abominable",
	"abysmal",
	"appalling",
	"baleful",
	"blasphemous",

	"disgusting",
	"dreadful",
	"filthy",
	"grisly",
	"hideous",

	"hellish",
	"horrible",
	"infernal",
	"loathsome",
	"nightmarish",

	"repulsive",
	"sacrilegious",
	"terrible",
	"unclean",
	"unspeakable",
};

static cptr funny_desc[MAX_FUNNY] =
{
	"silly",
	"hilarious",
	"absurd",
	"insipid",
	"ridiculous",

	"laughable",
	"ludicrous",
	"far-out",
	"groovy",
	"postmodern",

	"fantastic",
	"dadaistic",
	"cubistic",
	"cosmic",
	"awesome",

	"incomprehensible",
	"fabulous",
	"amazing",
	"incredible",
	"chaotic",

	"wild",
	"preposterous",
};

static cptr funny_comments[MAX_COMMENT] =
{
	"Wow, cosmic, man!",
	"Rad!",
	"Groovy!",
	"Cool!",
	"Far out!"
};

static void monster_gone(monster_type *m_ptr)
{
	monster_race *r_ptr = race_inf(m_ptr);

	char m_name[80];
	monster_desc(m_name, m_ptr, 0x80);

	/* Hack -- Reduce the racial counter */
	r_ptr->cur_num--;

	/* Hack -- count the number of "reproducers" */
	if (has_flag(m_ptr, FLAG_MULTIPLY)) num_repro--;

	/* Delete objects */
	if (p_ptr->preserve)
	{
		for_inventory_ro(m_ptr, o_ptr, 0, INVEN_TOTAL);
		{
			/* Hack -- Preserve unknown artifacts */
			if (artifact_p(o_ptr) && !object_known_p(o_ptr))
			{
				/* Mega-Hack -- Preserve the artifact */
				if (has_flag(&k_info[o_ptr->k_idx], FLAG_NORM_ART))
				{
					k_info[o_ptr->k_idx].artifact = FALSE;
				}
				else
				{
					a_info[o_ptr->artifact_id].cur_num = 0;
				}
			}
		}
		end_inventory();
	}
}

void delete_monster_type(monster_type *m_ptr, bool del)
{
	char m_name[80];
	monster_desc(m_name, m_ptr, 0x80);

	/* Reduce the monster race counter and preserve artifacts */ 
	monster_gone(m_ptr);

	flag_free(&m_ptr->inventory, FALSE);

	/* Delete mind & special race if needed */
	if (m_ptr->sr_ptr)
	{
		flag_free(&m_ptr->sr_ptr->flags, FALSE);
		flag_free(&m_ptr->sr_ptr->r_flags, FALSE);
		flag_free(&m_ptr->sr_ptr->spells, FALSE);
		flag_free(&m_ptr->sr_ptr->blow, FALSE);
		flag_free(&m_ptr->sr_ptr->body_parts, FALSE);
		KILL(m_ptr->sr_ptr, monster_race);
	}
	if (m_ptr->mind)
		KILL(m_ptr->mind, monster_mind);

	/* Delete monster blows */
	flag_free(&m_ptr->blow, FALSE);
	
	/* Delete monster spells */
	flag_free(&m_ptr->spells, FALSE);
	
	/* Delete monster flags */
	flag_free(&m_ptr->flags, FALSE);
	
	/* Delete AI states */
	flag_free(&m_ptr->ai_states, FALSE);

	/* Wipe the Monster */
	if (del)
		KILL(m_ptr, monster_type);
	else
		(void) WIPE(m_ptr, monster_type);
}

void monster_type_free_memory(monster_type *m_ptr)
{
	flag_free(&m_ptr->inventory, FALSE);
	/* Delete mind & special race if needed */
	if (m_ptr->sr_ptr)
	{
		flag_free(&m_ptr->sr_ptr->flags, FALSE);
		flag_free(&m_ptr->sr_ptr->r_flags, FALSE);
		flag_free(&m_ptr->sr_ptr->spells, FALSE);
		flag_free(&m_ptr->sr_ptr->blow, FALSE);
		flag_free(&m_ptr->sr_ptr->body_parts, FALSE);
		KILL(m_ptr->sr_ptr, monster_race);
	}
	if (m_ptr->mind)
		KILL(m_ptr->mind, monster_mind);
	/* Delete monster blows */
	flag_free(&m_ptr->blow, FALSE);
	
	/* Delete monster spells */
	flag_free(&m_ptr->spells, FALSE);
	
	/* Delete monster flags */
	flag_free(&m_ptr->flags, FALSE);
	
	/* Delete AI states */
	flag_free(&m_ptr->ai_states, FALSE);

	KILL(m_ptr, monster_type);
}

/*
 * Delete a monster by index.
 *
 * When a monster is deleted, all of its objects are deleted.
 */
void delete_monster_idx(s32b i)
{
	s32b x, y;

	monster_type *m_ptr = get_monster(i);

	bool had_lite = FALSE;

	process_hooks(HOOK_MONSTER_REMOVED, "(d,M)", i, m_ptr);

	/* Get location */
	y = m_ptr->fy;
	x = m_ptr->fx;

	/* XXX XXX XXX remove monster light source */
	if (has_flag(m_ptr, FLAG_HAS_LITE)) had_lite = TRUE;

	/* Hack -- remove target monster */
	if (i == target_who) target_who = -1;

	/* Hack -- remove tracked monster */
	if (i == health_who) health_track(0);

	/* Hack -- remove tracked monster */
	if (i == p_ptr->control) p_ptr->control = 0;

	/* Remove targets */
	for_flags(&monst_list);
	{
		monster_type *m_ptr = get_monster(__key);
		if (m_ptr->target == i) m_ptr->target = -1;
	}
	end_for_flags();

	/* Monster is gone */
	cave[y][x].m_idx = 0;
	delete_monster_type(m_ptr, FALSE);

	/* Count monsters */
	m_cnt--;

	/* Update monster light */
	if (had_lite) p_ptr->update |= (PU_MON_LITE);

	/* Update monster list window */
	flag_bool(&p_ptr->window, FLAG_PW_M_LIST);

	/* Visual update */
	lite_spot(y, x);
}


/*
 * Delete the monster, if any, at a given location
 */
void delete_monster(s32b y, s32b x)
{
	cave_type *c_ptr;

	/* Paranoia */
	if (!in_bounds(y, x)) return;

	/* Check the grid */
	c_ptr = &cave[y][x];

	/* Delete the monster (if any) */
	if (c_ptr->m_idx) delete_monster_idx(c_ptr->m_idx);
}

/*
 * Delete/Remove all the monsters when the player leaves the level
 *
 * This is an efficient method of simulating multiple calls to the
 * "delete_monster()" function, with no visual effects.
 */
void wipe_m_list(void)
{
	s32b i;

	/* Uncount every monsters */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);

		/* Skip dead monsters */
		if (!m_ptr->r_idx) continue;

		/* Monster is gone */
		cave[m_ptr->fy][m_ptr->fx].m_idx = 0;

		/* Delete mind & special race if needed */
		if (m_ptr->sr_ptr)
			KILL(m_ptr->sr_ptr, monster_race);
		if (m_ptr->mind)
			KILL(m_ptr->mind, monster_mind);
	}
	end_for_flags();

	/* Delete them */
	flag_empty(&monst_list);

	/* Reset "m_cnt" */
	m_cnt = 0;

	/* Hack -- reset "reproducer" count */
	num_repro = 0;

	/* Hack -- no more target */
	target_who = 0;

	/* Reset control */
	p_ptr->control = 0;

	/* Hack -- no more tracking */
	health_track(0);
}


/*
 * Acquires and returns the index of a "free" monster.
 */
s16b m_pop(void)
{
	return flag_first_free_key(&monst_list, 1);
}


/*
 * Apply a "monster restriction function" to the "monster allocation table"
 */
errr get_mon_num_prep(void)
{
	s32b i;

	/* Scan the allocation table */
	for (i = 0; i < alloc_race_size; i++)
	{
		/* Get the entry */
		alloc_entry *entry = &alloc_race_table[i];

		/* Accept monsters which pass the restriction, if any */
		if ((!get_mon_num_hook || (*get_mon_num_hook)(entry->index)) &&
		                (!get_mon_num2_hook || (*get_mon_num2_hook)(entry->index)))
		{
			/* Check if something in the Lua code wants to forbid it anyway. */
			if (!process_hooks(HOOK_MONSTER_FORBID, "(d)", entry->index))
			{
				/* Accept this monster */
				entry->prob2 = entry->prob1;
			}
			else
			{
				/* Reject this monster */
				entry->prob2 = 0;
			}
		}

		/* Do not use this monster */
		else
		{
			/* Decline this monster */
			entry->prob2 = 0;
		}
	}

	/* Success */
	return (0);
}

/*
 * Select the correct ruleset
 */
static void select_ruleset(byte *rule_percents[], rule_type *rules[])
{
	/* When in wilderness, use the terrain info, if it exisst */
	if ((dungeon_type == 0) && (wf_info[wild_map[p_ptr->wilderness_y][p_ptr->wilderness_x].feat].rules_set))
	{
		*rule_percents = wf_info[wild_map[p_ptr->wilderness_y][p_ptr->wilderness_x].feat].rule_percents;
		*rules = wf_info[wild_map[p_ptr->wilderness_y][p_ptr->wilderness_x].feat].rules;
	}
	/* Simply use dungeon ruleset */
	else
	{
		*rule_percents = d_info[dungeon_type].rule_percents;
		*rules = d_info[dungeon_type].rules;
	}
}

/*
 * Some dungeon types restrict the possible monsters.
 * Return TRUE is the monster is OK and FALSE otherwise
 */
bool apply_rule(flags_type *flags, s32b chr, byte rule)
{
	byte *rule_percents;
	rule_type *rules;

	select_ruleset(&rule_percents, &rules);

	if (rules[rule].mode == DUNGEON_MODE_NONE)
	{
		return TRUE;
	}
	else if ((rules[rule].mode == DUNGEON_MODE_AND) || (rules[rule].mode == DUNGEON_MODE_NAND))
	{
		s32b a;

		if (flag_used(&rules[rule].flags))
		{
			if (!flag_contains(flags, &rules[rule].flags))
				return FALSE;
		}
		for (a = 0; a < 5; a++)
		{
			if (rules[rule].r_char[a] && (rules[rule].r_char[a] != chr)) return FALSE;
		}

		/* All checks passed ? lets go ! */
		return TRUE;
	}
	else if ((rules[rule].mode == DUNGEON_MODE_OR) || (rules[rule].mode == DUNGEON_MODE_NOR))
	{
		s32b a;

		if (flag_used(&rules[rule].flags))
		{
			if (flag_contains_nb(flags, &rules[rule].flags) > 0)
				return TRUE;
		}

		for (a = 0; a < 5; a++)
		{
			if (rules[rule].r_char[a] == chr) return TRUE;
		}

		/* All checks failled ? Sorry ... */
		return FALSE;
	}
	/* Should NEVER happen */
	return FALSE;
}

bool restrict_to_dungeon(flags_type *flags, s32b chr)
{
	byte *rule_percents;
	rule_type *rules;
	byte rule;
	bool rule_ret;

	select_ruleset(&rule_percents, &rules);

	/* Select a random rule */
	rule = rule_percents[rand_int(100)];

	/* Apply the rule */
	rule_ret = apply_rule(flags, chr, rule);

	/* Should the rule be right or not ? */
	if ((rules[rule].mode == DUNGEON_MODE_NAND) || (rules[rule].mode == DUNGEON_MODE_NOR)) rule_ret = !rule_ret;

	/* Rule ok ? */
	if (rule_ret) return TRUE;

	/* Not allowed */
	return FALSE;
}

/* Ugly hack, let summon unappropriate monsters */
bool summon_hack = FALSE;

/*
 * Choose a monster race that seems "appropriate" to the given level
 *
 * This function uses the "prob2" field of the "monster allocation table",
 * and various local information, to calculate the "prob3" field of the
 * same table, which is then used to choose an "appropriate" monster, in
 * a relatively efficient manner.
 *
 * Note that "town" monsters will *only* be created in the town, and
 * "normal" monsters will *never* be created in the town, unless the
 * "level" is "modified", for example, by polymorph or summoning.
 *
 * There is a small chance (1/50) of "boosting" the given depth by
 * a small amount (up to four levels), except in the town.
 *
 * It is (slightly) more likely to acquire a monster of the given level
 * than one of a lower level.  This is done by choosing several monsters
 * appropriate to the given level and keeping the "hardest" one.
 *
 * Note that if no monsters are "appropriate", then this function will
 * fail, and return zero, but this should *almost* never happen.
 */
s16b get_mon_num(s32b level)
{
	s32b	i, j, p;

	s32b	r_idx;

	long	value, total;

	monster_race	*r_ptr;

	alloc_entry	*table = alloc_race_table;


	/* Boost the level */
	if (level > 0)
	{
		/* Occasional "nasty" monster */
		if (rand_int(NASTY_MON) == 0)
		{
			/* Pick a level bonus */
			s32b d = level / 4 + 2;

			/* Boost the level */
			level += ((d < 5) ? d : 5);
		}

		/* Occasional "nasty" monster */
		if (rand_int(NASTY_MON) == 0)
		{
			/* Pick a level bonus */
			s32b d = level / 4 + 2;

			/* Boost the level */
			level += ((d < 5) ? d : 5);
		}
	}


	/* Reset total */
	total = 0L;

	/* Process probabilities */
	for (i = 0; i < alloc_race_size; i++)
	{
		/* Monsters are sorted by depth */
		if (table[i].level > level) break;

		/* Default */
		table[i].prob3 = 0;

		/* No chance of it occuring, don't waste time on other tests.*/
		if (table[i].prob2 <= 0)
			continue;

		/* Access the "r_idx" of the chosen monster */
		r_idx = table[i].index;

		/* Access the actual race */
		r_ptr = &r_info[r_idx];

		/* Hack -- "unique" monsters must be "unique" */
		if ((has_flag(r_ptr, FLAG_UNIQUE)) &&
		                (r_ptr->cur_num >= r_ptr->max_num))
		{
			continue;
		}

		/* Depth Monsters never appear out of depth */
		if ((has_flag(r_ptr, FLAG_FORCE_DEPTH)) && (r_ptr->level > dun_level))
			continue;

		/* Depth Monsters never appear out of their depth */
		if ((has_flag(r_ptr, FLAG_ONLY_DEPTH)) && (r_ptr->level != dun_level))
			continue;

		/* Joke monsters allowed ? or not ? */
		if (!joke_monsters && has_flag(r_ptr, FLAG_JOKEANGBAND)) continue;

		/* Some dungeon types restrict the possible monsters */
		if (!summon_hack && !restrict_to_dungeon(&r_ptr->flags, r_ptr->d_char))
			continue;

		/* Accept */
		table[i].prob3 = table[i].prob2;

		/* Total */
		total += table[i].prob3;
	}

	/* No legal monsters */
	if (total <= 0) return (0);


	/* Pick a monster */
	value = rand_int(total);

	/* Find the monster */
	for (i = 0; i < alloc_race_size; i++)
	{
		/* Found the entry */
		if (value < table[i].prob3) break;

		/* Decrement */
		value = value - table[i].prob3;
	}


	/* Power boost */
	p = rand_int(100);

	/* Try for a "harder" monster once (50%) or twice (10%) */
	if (p < 60)
	{
		/* Save old */
		j = i;

		/* Pick a monster */
		value = rand_int(total);

		/* Find the monster */
		for (i = 0; i < alloc_race_size; i++)
		{
			/* Found the entry */
			if (value < table[i].prob3) break;

			/* Decrement */
			value = value - table[i].prob3;
		}

		/* Keep the "best" one */
		if (table[i].level < table[j].level) i = j;
	}

	/* Try for a "harder" monster twice (10%) */
	if (p < 10)
	{
		/* Save old */
		j = i;

		/* Pick a monster */
		value = rand_int(total);

		/* Find the monster */
		for (i = 0; i < alloc_race_size; i++)
		{
			/* Found the entry */
			if (value < table[i].prob3) break;

			/* Decrement */
			value = value - table[i].prob3;
		}

		/* Keep the "best" one */
		if (table[i].level < table[j].level) i = j;
	}

	/* Result */
	return (table[i].index);
}





/*
 * Build a string describing a monster in some way.
 *
 * We can correctly describe monsters based on their visibility.
 * We can force all monsters to be treated as visible or invisible.
 * We can build nominatives, objectives, possessives, or reflexives.
 * We can selectively pronominalize hidden, visible, or all monsters.
 * We can use definite or indefinite descriptions for hidden monsters.
 * We can use definite or indefinite descriptions for visible monsters.
 *
 * Pronominalization involves the gender whenever possible and allowed,
 * so that by cleverly requesting pronominalization / visibility, you
 * can get messages like "You hit someone.  She screams in agony!".
 *
 * Reflexives are acquired by requesting Objective plus Possessive.
 *
 * If no m_ptr arg is given (?), the monster is assumed to be hidden,
 * unless the "Assume Visible" mode is requested.
 *
 * If no r_ptr arg is given, it is extracted from m_ptr and r_info
 * If neither m_ptr nor r_ptr is given, the monster is assumed to
 * be neuter, singular, and hidden (unless "Assume Visible" is set),
 * in which case you may be in trouble... :-)
 *
 * I am assuming that no monster name is more than 70 characters long,
 * so that "char desc[80];" is sufficiently large for any result.
 *
 * Mode Flags:
 *   0x01 --> Objective (or Reflexive)
 *   0x02 --> Possessive (or Reflexive)
 *   0x04 --> Use indefinites for hidden monsters ("something")
 *   0x08 --> Use indefinites for visible monsters ("a kobold")
 *   0x10 --> Pronominalize hidden monsters
 *   0x20 --> Pronominalize visible monsters
 *   0x40 --> Assume the monster is hidden
 *   0x80 --> Assume the monster is visible
 *  0x100 --> Ignore insanity
 *  0x200 --> No article
 *
 * Useful Modes:
 *   0x00 --> Full nominative name ("the kobold") or "it"
 *   0x04 --> Full nominative name ("the kobold") or "something"
 *   0x80 --> Genocide resistance name ("the kobold")
 *   0x88 --> Killing name ("a kobold")
 *   0x22 --> Possessive, genderized if visible ("his") or "its"
 *   0x23 --> Reflexive, genderized if visible ("himself") or "itself"
 */
void monster_desc(char *desc, monster_type *m_ptr, s32b mode)
{
	cptr res;
	monster_race *r_ptr = race_inf(m_ptr);
	cptr b_name = (r_ptr->name);
	char silly_name[80], name[100];
	bool seen, pron;
	/* Only use sanity if sanity exists */


	if (m_ptr->ego)
	{
		if (re_info[m_ptr->ego].before) sprintf(name, "%s %s", re_info[m_ptr->ego].name, b_name);
		else sprintf(name, "%s %s", b_name, re_info[m_ptr->ego].name);
	}
	else
	{
		sprintf(name, "%s", b_name);
	}

	/*
	 * Are we hallucinating? (Idea from Nethack...)
	 * insanity roll added by pelpel
	 */
	if (!(mode & 0x100) && (intrinsic(HALLUCINATE)))
	{
		if (rand_int(2) == 0)
		{
			monster_race *hallu_race;

			do
			{
				hallu_race = &r_info[randint(max_r_idx - 2)];
			}
			while (has_flag(hallu_race, FLAG_UNIQUE));

			strcpy(silly_name, (hallu_race->name));
		}
		else
		{
			get_rnd_line("silly.txt", silly_name);
		}

		strcpy(name, silly_name);
	}

	/* Can we "see" it (exists + forced, or visible + not unforced) */
	seen = (m_ptr && ((mode & 0x80) || (!(mode & 0x40) && m_ptr->ml)));

	/* Sexed Pronouns (seen and allowed, or unseen and allowed) */
	pron = (m_ptr && ((seen && (mode & 0x20)) || (!seen && (mode & 0x10))));


	/* First, try using pronouns, or describing hidden monsters */
	if (!seen || pron)
	{
		/* an encoding of the monster "sex" */
		s32b kind = 0x00;

		/* Extract the gender (if applicable) */
		if (has_flag(m_ptr, FLAG_FEMALE)) kind = 0x20;
		else if (has_flag(m_ptr, FLAG_MALE)) kind = 0x10;

		/* Ignore the gender (if desired) */
		if (!m_ptr || !pron) kind = 0x00;


		/* Assume simple result */
		res = "it";

		/* Brute force: split on the possibilities */
		switch (kind | (mode & 0x07))
		{
			/* Neuter, or unknown */
		case 0x00:
			res = "it";
			break;
		case 0x01:
			res = "it";
			break;
		case 0x02:
			res = "its";
			break;
		case 0x03:
			res = "itself";
			break;
		case 0x04:
			res = "something";
			break;
		case 0x05:
			res = "something";
			break;
		case 0x06:
			res = "something's";
			break;
		case 0x07:
			res = "itself";
			break;

			/* Male (assume human if vague) */
		case 0x10:
			res = "he";
			break;
		case 0x11:
			res = "him";
			break;
		case 0x12:
			res = "his";
			break;
		case 0x13:
			res = "himself";
			break;
		case 0x14:
			res = "someone";
			break;
		case 0x15:
			res = "someone";
			break;
		case 0x16:
			res = "someone's";
			break;
		case 0x17:
			res = "himself";
			break;

			/* Female (assume human if vague) */
		case 0x20:
			res = "she";
			break;
		case 0x21:
			res = "her";
			break;
		case 0x22:
			res = "her";
			break;
		case 0x23:
			res = "herself";
			break;
		case 0x24:
			res = "someone";
			break;
		case 0x25:
			res = "someone";
			break;
		case 0x26:
			res = "someone's";
			break;
		case 0x27:
			res = "herself";
			break;
		}

		/* Copy the result */
		(void)strcpy(desc, res);
	}


	/* Handle visible monsters, "reflexive" request */
	else if ((mode & 0x02) && (mode & 0x01))
	{
		/* The monster is visible, so use its gender */
		if (has_flag(m_ptr, FLAG_FEMALE)) strcpy(desc, "herself");
		else if (has_flag(m_ptr, FLAG_MALE)) strcpy(desc, "himself");
		else strcpy(desc, "itself");
	}


	/* Handle all other visible monster requests */
	else
	{
		/* Ignore mode if FORCE_MONST_NAME present. */
		if (has_flag(m_ptr, FLAG_FORCE_MONST_NAME))
		{
			(void)strcpy(desc, flag_get_string(&m_ptr->flags,
											   FLAG_FORCE_MONST_NAME));
		}
		/* It could be a Unique, or the mode requests no articles */
		else if ((mode & 0x200) ||
			(has_flag(m_ptr, FLAG_UNIQUE) && !intrinsic(HALLUCINATE)))
		{
			/* Start with the name (thus nominative and objective) */
			(void)strcpy(desc, name);
		}

		/* It could be an indefinite monster */
		else if (mode & 0x08)
		{
			/* XXX Check plurality for "some" */

			/* Indefinite monsters need an indefinite article */
			(void)strcpy(desc, is_a_vowel(name[0]) ? "an " : "a ");
			(void)strcat(desc, name);
		}

		/* It could be a normal, definite, monster */
		else
		{
			/* Definite monsters need a definite article */
			if (is_friend(m_ptr) == 1)
				(void)strcpy(desc, "your ");
			else
				(void)strcpy(desc, "the ");

			(void)strcat(desc, name);
		}

		/* Handle the Possessive as a special afterthought */
		if (mode & 0x02)
		{
			/* XXX Check for trailing "s" */

			/* Simply append "apostrophe" and "s" */
			(void)strcat(desc, "'s");
		}
	}
}

void monster_race_desc(char *desc, s32b r_idx, s32b ego)
{
	monster_race *r_ptr = &r_info[r_idx];
	cptr b_name = (r_ptr->name);
	char name[80];

	if (ego)
	{
		if (re_info[ego].before) sprintf(name, "%s %s", re_info[ego].name, b_name);
		else sprintf(name, "%s %s", b_name, re_info[ego].name);
	}
	else
	{
		sprintf(name, "%s", b_name);
	}

	/* It could be a Unique */
	if (has_flag(r_ptr, FLAG_UNIQUE))
	{
		/* Start with the name (thus nominative and objective) */
		(void)strcpy(desc, name);
	}

	/* It could be a normal, definite, monster */
	else
	{
		/* Definite monsters need a definite article */
		(void)strcpy(desc, is_a_vowel(name[0]) ? "an " : "a ");

		(void)strcat(desc, name);
	}
}



/*
 * Learn about a monster (by "probing" it)
 */
void lore_do_probe(s32b m_idx)
{
	monster_type *m_ptr = get_monster(m_idx);

	monster_race *r_ptr = &r_info[m_ptr->r_idx];

	/* Hack -- Memorize some flags */
	flag_copy(&r_ptr->r_flags, &r_ptr->flags);

	/* Update monster recall window */
	if (monster_race_idx == m_ptr->r_idx)
	{
		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_MONSTER);
	}
}


/*
 * Take note that the given monster just dropped some treasure
 *
 * Note that learning the "GOOD"/"GREAT" flags gives information
 * about the treasure (even when the monster is killed for the first
 * time, such as uniques, and the treasure has not been examined yet).
 *
 * This "indirect" method is used to prevent the player from learning
 * exactly how much treasure a monster can drop from observing only
 * a single example of a drop.  This method actually observes how much
 * gold and items are dropped, and remembers that information to be
 * described later by the monster recall code.
 */
void lore_treasure(s32b m_idx, s32b num_item, s32b num_gold)
{
	monster_type *m_ptr = get_monster(m_idx);

	monster_race *r_ptr = &r_info[m_ptr->r_idx];

	/* Note the number of things dropped */
	if (num_item > r_ptr->r_drop_item) r_ptr->r_drop_item = num_item;
	if (num_gold > r_ptr->r_drop_gold) r_ptr->r_drop_gold = num_gold;

	/* Hack -- memorize the good/great flags */
	if (has_flag(r_ptr, FLAG_DROP_GOOD))
		monstmem_add(RT_MISC, FLAG_DROP_GOOD, m_ptr, 0);

	if (has_flag(r_ptr, FLAG_DROP_GREAT))
		monstmem_add(RT_MISC, FLAG_DROP_GREAT, m_ptr, 0);

	/* Update monster recall window */
	if (monster_race_idx == m_ptr->r_idx)
	{
		/* Window stuff */
		flag_bool(&p_ptr->window, FLAG_PW_MONSTER);
	}
}


/*
 * This function updates the monster record of the given monster
 *
 * This involves extracting the distance to the player, checking
 * for visibility (natural, infravision, see-invis, telepathy),
 * updating the monster visibility flag, redrawing or erasing the
 * monster when the visibility changes, and taking note of any
 * "visual" features of the monster (cold-blooded, invisible, etc).
 *
 * The only monster fields that are changed here are "cdis" (the
 * distance from the player), "los" (clearly visible to player),
 * and "ml" (visible to the player in any way).
 *
 * There are a few cases where the calling routine knows that the
 * distance from the player to the monster has not changed, and so
 * we have a special parameter "full" to request distance computation.
 * This lets many calls to this function run very quickly.
 *
 * Note that every time a monster moves, we must call this function
 * for that monster, and update distance.  Note that every time the
 * player moves, we must call this function for every monster, and
 * update distance.  Note that every time the player "state" changes
 * in certain ways (including "blindness", "infravision", "telepathy",
 * and "see invisible"), we must call this function for every monster.
 *
 * The routines that actually move the monsters call this routine
 * directly, and the ones that move the player, or notice changes
 * in the player state, call "update_monsters()".
 *
 * Routines that change the "illumination" of grids must also call
 * this function, since the "visibility" of some monsters may be
 * based on the illumination of their grid.
 *
 * Note that this function is called once per monster every time the
 * player moves, so it is important to optimize it for monsters which
 * are far away.  Note the optimization which skips monsters which
 * are far away and were completely invisible last turn.
 *
 * Note the optimized "inline" version of the "distance()" function.
 *
 * Note that only monsters on the current panel can be "visible",
 * and then only if they are (1) in line of sight and illuminated
 * by light or infravision, or (2) nearby and detected by telepathy.
 *
 * The player can choose to be disturbed by several things, including
 * "disturb_move" (monster which is viewable moves in some way), and
 * "disturb_near" (monster which is "easily" viewable moves in some
 * way).  Note that "moves" includes "appears" and "disappears".
 *
 * Note the new "xtra" field which encodes several state flags such
 * as "detected last turn", and "detected this turn", and "currently
 * in line of sight", all of which are used for visibility testing.
 */
void update_mon(s32b m_idx, bool full)
{
	monster_type *m_ptr;
	monster_race *r_ptr;

	/* The current monster location */
	s32b fy;
	s32b fx;

	/* Seen at all */
	bool flag = FALSE;

	/* Seen by vision */
	bool easy = FALSE;

	/* Seen by telepathy */
	bool can_esp = FALSE;
	bool hard    = FALSE;

	/* Various extra flags */
	bool do_empty_mind = FALSE;
	bool do_weird_mind = FALSE;
	bool do_invisible = FALSE;
	bool do_cold_blood = FALSE;

	if (!m_idx) return;

	m_ptr = get_monster(m_idx);

	/* Is monster is dead? */
	if (m_ptr->r_idx == 0)
		return;

	r_ptr = race_inf(m_ptr);

	fy = m_ptr->fy;
	fx = m_ptr->fx;

	/* Calculate distance */
	if (full)
	{
		s32b d, dy, dx;

		/* Distance components */
		dy = (p_ptr->py > fy) ? (p_ptr->py - fy) : (fy - p_ptr->py);
		dx = (p_ptr->px > fx) ? (p_ptr->px - fx) : (fx - p_ptr->px);

		/* Approximate distance */
		d = (dy > dx) ? (dy + (dx >> 1)) : (dx + (dy >> 1));

		/* Save the distance (in a byte) */
		m_ptr->cdis = (d < 255) ? d : 255;
	}


#if 0

	/* Process monster light for a monster within MAX_SIGHT + max radius */
	if (m_ptr->cdis <= MAX_SIGHT + 1)
	{
		if (m_ptr->flags9 & RF9_HAS_LITE)
		{
			p_ptr->update |= (PU_MON_LITE);
		}
	}

#endif

	/* Process "distant" monsters */
	if (m_ptr->cdis > MAX_SIGHT)
	{
		/* Ignore unseen monsters */
		if (!m_ptr->ml) return;

		/* Detected */
		if (m_ptr->mflag & (MFLAG_MARK)) flag = TRUE;
	}


	/* Process "nearby" monsters on the current "panel" */
	else if (panel_contains(fy, fx))
	{
		/* Normal line of sight, and player is not blind */
		if (player_has_los_bold(fy, fx) && !intrinsic(BLIND))
		{
			/* Use "infravision" */
			if (m_ptr->cdis <= (byte)(p_ptr->see_infra))
			{
				/* Infravision only works on "warm" creatures */
				/* Below, we will need to know that infravision failed */
				if (has_flag(m_ptr, FLAG_COLD_BLOOD)) do_cold_blood = TRUE;

				/* Infravision works */
				if (!do_cold_blood) easy = flag = TRUE;
			}

			/* Use "illumination" */
			if (player_can_see_bold(fy, fx))
			{
				/* Take note of invisibility */
				if (has_flag(m_ptr, FLAG_INVISIBLE)) do_invisible = TRUE;

				/* Visible, or detectable, monsters get seen */
				if ((!do_invisible) || intrinsic(SEE_INVIS))
					easy = flag = TRUE;
			}
		}

		if (flag_exists(&p_ptr->intrinsic, FLAG_ESP))
		{
			flags_type *esp_flags = flag_get_flags(&p_ptr->intrinsic,
												   FLAG_ESP);
			s32b i;

			/* By monster-flag ESP */
			for (i = 0; i < esp_flags->size; i++)
			{
				flag_node* n = &esp_flags->node[i];

				if (!(n->flags & FLAG_FLAG_USED)) continue;

				if (has_flag(m_ptr, n->key) &&
					(m_ptr->cdis <= n->vals.vals.val))
				{
					can_esp = TRUE;
					break;
				}
			}

			/* Full ESP */
			if (!can_esp && (m_ptr->cdis <= flag_get(esp_flags, FLAG_ESP)))
				can_esp = TRUE;

			if (!can_esp)
			{
				s32b friend = get_faction_friendliness(m_ptr->faction,
													  FACTION_PLAYER);

				if ((friend < 0) &&
					(m_ptr->cdis <= flag_get(esp_flags, FLAG_ESP_HOSTILE)))
					can_esp = TRUE;
				else if ((friend == 0) &&
						 (m_ptr->cdis <= flag_get(esp_flags,
												  FLAG_ESP_NEUTRAL)))
					can_esp = TRUE;
				else if ((friend > 0) &&
						 (m_ptr->cdis <= flag_get(esp_flags,
												  FLAG_ESP_FRIENDLY)))
					can_esp = TRUE;
			}

			if (!can_esp)
			{
				if (m_ptr->csleep > 0)
				{
					if (m_ptr->cdis <= flag_get(esp_flags, FLAG_ESP_SLEEPING))
						can_esp = TRUE;
				}
				else if (m_ptr->cdis <= flag_get(esp_flags, FLAG_ESP_AWAKE))
					can_esp = TRUE;
			}

			/* Only do ESP when we can really detect monster */
			if (can_esp)
			{
				/* Empty mind, no telepathy */
				if (has_flag(m_ptr, FLAG_EMPTY_MIND))
				{
					can_esp = FALSE;
				}

				/* Weird mind, occasional telepathy */
				else if (rand_int(100) < get_flag(m_ptr, FLAG_WEIRD_MIND))
				{
					do_weird_mind = TRUE;
					hard          = TRUE;
					flag          = TRUE;
				}

				/* Normal mind, allow telepathy */
				else
				{
					hard = TRUE;
					flag = TRUE;
				}
			}
		}

		/* Apply "detection" spells */
		if (m_ptr->mflag & (MFLAG_MARK)) flag = TRUE;

		/* Hack -- Wizards have "perfect telepathy" */
		if (wizard) flag = TRUE;
	}

	/* The monster is now visible */
	if (flag)
	{
		/* It was previously unseen */
		if (!m_ptr->ml)
		{
			monster_race *base_r_ptr = &r_info[m_ptr->r_idx];

			/* Mark as visible */
			m_ptr->ml = TRUE;

			/* Draw the monster */
			lite_spot(fy, fx);

			/* Update health bar as needed */
			if (health_who == m_idx) flag_bool(&p_ptr->redraw, FLAG_PR_HEALTH);

			/* Update monster list window */
			flag_bool(&p_ptr->window, FLAG_PW_M_LIST);

			/* Hack -- Count "fresh" sightings */
			if (base_r_ptr->r_sights < MAX_SHORT) base_r_ptr->r_sights++;

			/* Disturb on appearance */
			if (disturb_move)
			{
				if (disturb_pets || (is_friend(m_ptr) <= 0)) disturb(1, 0);
			}
		}

		/* Apply telepathy */
		if (hard)
		{
			/* Hack -- Memorize mental flags */
			if (has_flag(m_ptr, FLAG_SMART))
				monstmem_add(RT_MISC, FLAG_SMART, m_ptr, 0);
			if (has_flag(m_ptr, FLAG_STUPID))
				monstmem_add(RT_MISC, FLAG_STUPID, m_ptr, 0);
		}

		/* Memorize various observable flags */
		if (do_empty_mind)
			monstmem_add(RT_MISC, FLAG_EMPTY_MIND, m_ptr, 0);
		if (do_weird_mind)
			monstmem_add(RT_MISC, FLAG_WEIRD_MIND, m_ptr, 0);
		if (do_cold_blood)
			monstmem_add(RT_MISC, FLAG_COLD_BLOOD, m_ptr, 0);
		if (do_invisible)
			monstmem_add(RT_MISC, FLAG_INVISIBLE, m_ptr, 0);
	}

	/* The monster is not visible */
	else
	{
		/* It was previously seen */
		if (m_ptr->ml)
		{
			/* Mark as not visible */
			m_ptr->ml = FALSE;

			/* Erase the monster */
			lite_spot(fy, fx);

			/* Update health bar as needed */
			if (health_who == m_idx) flag_bool(&p_ptr->redraw, FLAG_PR_HEALTH);

			/* Update monster list window */
			flag_bool(&p_ptr->window, FLAG_PW_M_LIST);

                        /* Disturb on disappearance*/
			if (disturb_move)
			{
				if (disturb_pets || (is_friend(m_ptr) <= 0)) disturb(1, 0);
			}
		}
	}


	/* The monster is now easily visible */
	if (easy)
	{
		/* Change */
		if (!(m_ptr->mflag & (MFLAG_VIEW)))
		{
			/* Mark as easily visible */
			m_ptr->mflag |= (MFLAG_VIEW);

			/* Disturb on appearance */
			if (disturb_near)
			{
				if (disturb_pets || (is_friend(m_ptr) <= 0)) disturb(1, 0);
			}

		}
	}

	/* The monster is not easily visible */
	else
	{
		/* Change */
		if (m_ptr->mflag & (MFLAG_VIEW))
		{
			/* Mark as not easily visible */
			m_ptr->mflag &= ~(MFLAG_VIEW);

			/* Update monster list window */
			flag_bool(&p_ptr->window, FLAG_PW_M_LIST);

			/* Disturb on disappearance */
			if (disturb_near)
			{
				if (disturb_pets || (is_friend(m_ptr) <= 0)) disturb(1, 0);
			}
		}
	}
}




/*
 * This function simply updates all the (non-dead) monsters (see above).
 */
void update_monsters(bool full)
{
	s32b i;

	/* Update each (live) monster */
	for_flags(&monst_list);
	{
		i = __key;
		monster_type *m_ptr = get_monster(i);

		/* Skip dead monsters */
		if (!m_ptr || !m_ptr->r_idx) continue;

		/* Update the monster */
		update_mon(i, full);
	}
	end_for_flags();
}


void monster_carry(monster_type *m_ptr, s32b m_idx, object_type *o_ptr)
{
	o_ptr->held_m_idx = m_idx;
	o_ptr->ix = 0;
	o_ptr->iy = 0;
	o_ptr->next_o_idx = 0;

	inven_carry_inven(get_inven(m_ptr, INVEN_INVEN), o_ptr, FALSE);
}

/*
 * Check if a monster has space for an item in the pack without overflow
 * Consider unifying it with inven_carry_okay. But. Players have special
 * handling for gold and monsters don't I think. (JMLB)
 */

bool monster_carry_okay(monster_type *m_ptr, object_type *o_ptr)
{
	flags_type *inven = get_inven(m_ptr,INVEN_INVEN);

	/* DGDGDG some HOOK here would be a sensible way to implement the
	   FOO can't pick up SLAY_FOO type restrictions -- JMLB */

	/* Empty slot? */
	if (inventory_limit_inven(inven) > flag_max_key(inven)) return (TRUE);

	/* Similar slot? */
	for_inventory_slot(inven, j_ptr);
	{
		/* Check if the two items can be combined */
		if (object_similar(j_ptr, o_ptr)) return (TRUE);
	}
	end_inventory_slot();

	/* Nope */
	return (FALSE);
}


static s32b possible_randart[] =
{
	-1,
};


bool kind_is_randart(s32b k_idx)
{
	s32b max;
	object_kind *k_ptr = &k_info[k_idx];

#if 0 /* MATT XXX XXX */
/* Shouldn't this whole function be in Lua? */
	if (!kind_is_legal(k_idx)) return (FALSE);
#endif

	for (max = 0; possible_randart[max] != -1; max++)
	{
		if (k_ptr->tval == possible_randart[max]) return (TRUE);
	}
	return (FALSE);
}

/*
 * Sets up a monster_type struct as appriate for the given race and
 * ego, but without placing it in the dungeon or invoking any hooks,
 * not even the ON_MAKE hooks.
 */
static void monster_prep_aux(monster_type* m_ptr, s32b r_idx, 
							 monster_race* r_ptr, s32b ego)
{
	/* MATT */
	/* Save the race */
	m_ptr->r_idx  = r_idx;
	m_ptr->ego    = ego;
	m_ptr->d_char = r_ptr->d_char;
	m_ptr->sval   = r_ptr->sval;

	/* Not flowed yet */
	m_ptr->los_flow_last_turn = FALSE;

	/* No special, no mind */
	m_ptr->sr_ptr = NULL;
	m_ptr->mind   = NULL;

	/* NO AI */
	m_ptr->ai = 0L;
	flag_empty(&m_ptr->ai_states);

	/* Place the monster at the location */
	m_ptr->fy = -1;
	m_ptr->fx = -1;

	/* No "damage" yet */
	m_ptr->stunned = 0;
	m_ptr->confused = 0;
	m_ptr->monfear = 0;

	/* No target yet */
	m_ptr->target = -1;

	/* Unknown distance */
	m_ptr->cdis = 0;

	/* No flags */
	m_ptr->mflag = 0;

	/* Not visible */
	m_ptr->ml = FALSE;

	/* No objects yet */
	prepare_inventory(&m_ptr->inventory);

	/* Do we need to force the faction? */
	if (has_flag(r_ptr, FLAG_FACTION))
		m_ptr->faction = get_flag(r_ptr, FLAG_FACTION);
	else
		m_ptr->faction = -1;

	/* Assign maximal hitpoints */
	if (has_flag(r_ptr, FLAG_FORCE_MAXHP))
	{
		m_ptr->maxhp = maxroll(r_ptr->hdice, r_ptr->hside);
	}
	else
	{
		m_ptr->maxhp = damroll(r_ptr->hdice, r_ptr->hside);
	}

	/* And start out fully healthy */
	m_ptr->hp = m_ptr->maxhp;

	/* Some basic info */
	flag_copy(&m_ptr->blow, &r_ptr->blow);
	m_ptr->ac = r_ptr->ac;
	m_ptr->level = r_ptr->level;
	m_ptr->speed = r_ptr->speed;
	m_ptr->exp = MONSTER_EXP(m_ptr->level);

	/* Extract the monster base speed */
	m_ptr->mspeed = m_ptr->speed;

	/* Flags and spells */
	flag_copy(&m_ptr->flags, &r_ptr->flags);
	flag_copy(&m_ptr->spells, &r_ptr->spells);
	m_ptr->freq_spell = r_ptr->freq_spell;
	m_ptr->freq_inate = r_ptr->freq_inate;

	/* If no resists then add an empty flagset */
	if (!has_flag(m_ptr, FLAG_RESIST))
	{
		flag_set_flags(&m_ptr->flags, FLAG_RESIST, flag_new(2));
	}

	/* If no absorbs then add an empty flagset */
	if (!has_flag(m_ptr, FLAG_ABSORB))
	{
		flag_set_flags(&m_ptr->flags, FLAG_ABSORB, flag_new(2));
	}

	/* Give a random starting energy */
	m_ptr->energy = rand_int(100) + 900;

	/* Ok do we need some AI setup ? (probably yes) */
	if (has_flag(m_ptr, FLAG_AI_STATES))
	{
		flag_copy(&m_ptr->ai_states,
				  flag_get_flags(&m_ptr->flags, FLAG_AI_STATES));
		flag_remove_zap(&m_ptr->flags, FLAG_AI_STATES);
	}
	if (has_flag(m_ptr, FLAG_AI))
		set_monster_ai(m_ptr, get_flag(m_ptr, FLAG_AI));

	/* Force monster to wait for player */
	if (has_flag(r_ptr, FLAG_FORCE_SLEEP))
	{
		/* Monster is still being nice */
		m_ptr->mflag |= (MFLAG_NICE);

		/* Must repair monsters */
		repair_monsters = TRUE;
	}

	/* Assume no sleeping */
	m_ptr->csleep = 0;
}

void monster_prep(monster_type* m_ptr, s32b r_idx, s32b ego)
{
	monster_prep_aux(m_ptr, r_idx, &r_info[r_idx], ego);
}

/*
 * Attempt to place a monster of the given race at the given location.
 *
 * To give the player a sporting chance, any monster that appears in
 * line-of-sight and is extremely dangerous can be marked as
 * "FORCE_SLEEP", which will cause them to be placed with low energy,
 * which often (but not always) lets the player move before they do.
 *
 * This routine refuses to place out-of-depth "FORCE_DEPTH" monsters.
 *
 * XXX XXX XXX Use special "here" and "dead" flags for unique monsters,
 * remove old "cur_num" and "max_num" fields.
 *
 * XXX XXX XXX Actually, do something similar for artifacts, to simplify
 * the "preserve" mode, and to make the "what artifacts" flag more useful.
 *
 * This is the only function which may place a monster in the dungeon,
 * except for the savefile loading code.
 */
bool bypass_r_ptr_max_num = FALSE;
s32b place_monster_result = 0;
bool place_monster_one_no_drop = FALSE;
monster_race *place_monster_one_race = NULL;
monster_type *placed_monsters[PLACED_MONSTERS_MAX];
s32b          placed_monster_ids[PLACED_MONSTERS_MAX];
s32b          placed_monsters_num = 0;
s32b          placed_monster_id  = 0;
s16b place_monster_one(s32b y, s32b x, s32b r_idx, s32b ego, bool slp, s32b status)
{
	s32b min_level = 0, max_level = 0;

	cave_type    *c_ptr = &cave[y][x];
	feature_type *f_ptr = &f_info[c_ptr->feat];

	monster_type *m_ptr;

	monster_race *r_ptr = &r_info[r_idx];
	monster_ego *e_ptr;

	cptr name = (r_ptr->name);

	/* Grab the special race if needed */
	if (place_monster_one_race)
	{
		r_ptr = place_monster_one_race;
	}

	/* DO NOT PLACE A MONSTER IN THE SMALL SCALE WILDERNESS !!! */
	if (p_ptr->wild_mode)
	{
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Verify location */
	if (!in_bounds(y, x))
	{
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Require empty space */
	if (!cave_empty_bold(y, x))
	{
		if (wizard) cmsg_format(TERM_L_RED, "WARNING: Refused monster(%d): EMPTY BOLD", r_idx);
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Require no monster free grid, or special permission */
	if ((c_ptr->info & CAVE_FREE) && (!m_allow_special[r_idx]))
	{
		if (wizard) cmsg_format(TERM_L_RED, "WARNING: Refused monster(%d): CAVE_FREE", r_idx);
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Don't place any monsters on this type of terrain? */
	if (has_flag(f_ptr, FLAG_MONST_NO_PLACE))
	{
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Paranoia */
	if (!r_idx)
	{
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Paranoia */
	if (!r_ptr->name)
	{
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Are we allowed to continue ? */
	if (process_hooks(HOOK_NEW_MONSTER_PRE, "(d,d,d,d,d,d)", y, x, r_idx,
					  ego, slp, status))
	{
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Ego Uniques are NOT to be created */
	if (has_flag(r_ptr, FLAG_UNIQUE) && ego)
	{
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Now could we generate an Ego Monster */
	/* Grab the special race if needed */
	if (place_monster_one_race)
	{
		r_ptr = place_monster_one_race;
	}
	else
	{
		r_ptr = race_info_idx(r_idx, ego);
	}

	if (!monster_can_cross_terrain(c_ptr->feat, &r_ptr->flags))
	{
		if (wizard) cmsg_print(TERM_L_RED, "WARNING: Refused monster: cannot cross terrain");
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Unallow some uniques to be generated outside of their quests/special levels/dungeons */
	if (has_flag(r_ptr, FLAG_SPECIAL_GENE) && (!m_allow_special[r_idx]))
	{
		if (wizard) cmsg_format(TERM_L_RED, "WARNING: Refused monster(%d): SPECIAL_GENE", r_idx);
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Fully forbid it */
	if (has_flag(r_ptr, FLAG_NEVER_GENE))
	{
		if (wizard) cmsg_print(TERM_L_RED, "WARNING: Refused monster: NEVER_GENE");
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Hack -- "unique" monsters must be "unique" */
	if ((has_flag(r_ptr, FLAG_UNIQUE)) && (r_ptr->max_num == -1) && (!m_allow_special[r_idx]))
	{
		/* Cannot create */
		if (wizard) cmsg_format(TERM_L_RED, "WARNING: Refused monster %d: unique not unique", r_idx);
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Anyway that doesn't work .... hum... TO FIX -- DG */
#if 0
	/* Hack -- non "town" monsters are NEVER generated in town */
	if ((!(r_ptr->flags8 & (RF8_WILD_TOWN))) && (wf_info[wild_map[p_ptr->wilderness_y][p_ptr->wilderness_x].feat].terrain_idx == TERRAIN_TOWN) && !dun_level)
	{
		/* Cannot create */
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return (0);
	}
#endif
	/* Hack -- "unique" monsters must be "unique" */
#if 0
	if ((r_ptr->flags1 & (RF1_UNIQUE)) && (r_ptr->cur_num >= r_ptr->max_num) && (!m_allow_special[r_idx]))
#else
#if 0
	if ((r_ptr->flags1 & (RF1_UNIQUE)) && (r_ptr->cur_num >= r_ptr->max_num) && (!m_allow_special[r_idx]) && (r_ptr->max_num != -1))
#else
		if ((has_flag(r_ptr, FLAG_UNIQUE)) && (r_ptr->cur_num >= r_ptr->max_num) && (r_ptr->max_num != -1) && (!bypass_r_ptr_max_num))
#endif
#endif
	{
		/* Cannot create */
		if (wizard) cmsg_format(TERM_L_RED, "WARNING: Refused monster %d: cur_num >= max_num", r_idx);
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Depth monsters may NOT be created out of depth */
	if ((has_flag(r_ptr, FLAG_FORCE_DEPTH)) && (dun_level < r_ptr->level))
	{
		/* Cannot create */
		if (wizard) cmsg_print(TERM_L_RED, "WARNING: FORCE_DEPTH");
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Powerful monster */
	if (r_ptr->level > dun_level)
	{
		/* Unique monsters */
		if (has_flag(r_ptr, FLAG_UNIQUE))
		{
			/* Message for cheaters */
			if ((cheat_hear) || (p_ptr->precognition)) msg_format("Deep Unique (%s).", name);

			/* Boost rating by twice delta-depth */
			rating += (r_ptr->level - dun_level) * 2;
		}

		/* Normal monsters */
		else
		{
			/* Message for cheaters */
			if ((cheat_hear) || (p_ptr->precognition)) msg_format("Deep Monster (%s).", name);

			/* Boost rating by delta-depth */
			rating += (r_ptr->level - dun_level);
		}
	}

	/* Note the monster */
	else if (has_flag(r_ptr, FLAG_UNIQUE))
	{
		/* Unique monsters induce message */
		if ((cheat_hear) || (p_ptr->precognition)) msg_format("Unique (%s).", name);
	}


	/* Access the location */
	c_ptr = &cave[y][x];

	/* Make a new monster */
	c_ptr->m_idx = m_pop();
	hack_m_idx_ii = c_ptr->m_idx;

	/* Mega-Hack -- catch "failure" */
	if (!c_ptr->m_idx)
	{
		if (place_monster_one_race) KILL(place_monster_one_race, monster_race);
		return 0;
	}

	/* Get a new monster record */
	m_ptr = monster_type_new();
	flag_set_monster(&monst_list, c_ptr->m_idx, m_ptr);

	/* Set up most of the monster's fields */
	monster_prep_aux(m_ptr, r_idx, r_ptr, ego);

	/* Enforce sleeping if needed */
	if (slp && r_ptr->sleep)
	{
		s32b val = r_ptr->sleep;
		m_ptr->csleep = ((val * 2) + randint(val * 10));

		/* Setup the sleep AI */
		call_lua("ai.setup_sleep", "(M)", "", m_ptr);
	}

	/* No special, no mind */
	m_ptr->sr_ptr = place_monster_one_race;

	/* Place the monster at the location */
	m_ptr->fy = y;
	m_ptr->fx = x;

	/* Do we need to force the faction? */
	if (m_ptr->faction == -1)
		m_ptr->faction = status;

	/* Generate the monster's inventory(if any) */
	/* Only if not fated to die */
	if (!place_monster_one_no_drop)
		process_hooks(HOOK_CREATE_DROP, "(d,M)", c_ptr->m_idx, m_ptr);

	place_monster_one_no_drop = FALSE;


	/* Hack -- small racial variety */
	if (!(has_flag(r_ptr, FLAG_UNIQUE)))
	{
#if 0 // DGDGDGDG - fixme with code from Cthband
		/* Allow some small variation per monster */
		i = extract_energy[m_ptr->speed] / 10;
		if (i) m_ptr->mspeed += rand_spread(0, i);
#endif
	}

	/* Need to match dungeon level ? */
	if (has_flag(dungeon_flags, FLAG_ADJUST_LEVEL))
	{
		call_lua("get_adjusted_level", "(d)", "dd", get_flag(dungeon_flags, FLAG_ADJUST_LEVEL), &min_level, &max_level);
		monster_set_level(c_ptr->m_idx, rand_range(min_level, max_level));
	}

	/* Hack -- see "process_monsters()" */
	if (c_ptr->m_idx < hack_m_idx)
	{
		/* Monster is still being born */
		m_ptr->mflag |= (MFLAG_BORN);
	}

	/* Update the monster */
	update_mon(c_ptr->m_idx, TRUE);

	/* Hack -- Count the number of "reproducers" */
	if (has_flag(r_ptr, FLAG_MULTIPLY)) num_repro++;

	/* Hack -- Notice new multi-hued monsters */
	if (has_flag(r_ptr, FLAG_ATTR_MULTI)) shimmer_monsters = TRUE;

	/* Hack -- we need to modify the REAL r_info, not the fake one */
	r_ptr = &r_info[r_idx];

	/* Hack -- Count the monsters on the level */
	r_ptr->cur_num++;
	m_cnt++;

	place_monster_one_race = NULL;

	if (flag_exists(&r_ptr->flags, FLAG_ON_MAKE))
	{
		open_lua_functions_registry(flag_get(&r_ptr->flags, FLAG_ON_MAKE), flag_get2(&r_ptr->flags, FLAG_ON_MAKE));
		call_lua(NULL, "(d,M)", "", c_ptr->m_idx, m_ptr);
		close_lua_functions_registry();
	}
	if (ego)
	{
		e_ptr = &re_info[ego];
		if (flag_exists(&e_ptr->flags, FLAG_ON_MAKE))
		{
			open_lua_functions_registry(flag_get(&e_ptr->flags, FLAG_ON_MAKE), flag_get2(&e_ptr->flags, FLAG_ON_MAKE));
			call_lua(NULL, "(d,M)", "", c_ptr->m_idx, m_ptr);
			close_lua_functions_registry();
		}
	}

	/* Success */
	if (placed_monsters_num < PLACED_MONSTERS_MAX)
	{
		placed_monsters[placed_monsters_num]     = m_ptr;
		placed_monster_ids[placed_monsters_num] = placed_monster_id;
		placed_monsters_num++;
	}

	place_monster_result = c_ptr->m_idx;
	process_hooks(HOOK_NEW_MONSTER_POST, "(d,d,d,d,d,d)", y, x, r_idx,
				  ego, slp, status);
	return c_ptr->m_idx;
}

/*
 * Place excisting monster on the map(it really should NOT already be on it)
 */
s16b place_monster_type(s32b y, s32b x, monster_type *m_ptr)
{
	cave_type    *c_ptr = &cave[y][x];
	feature_type *f_ptr = &f_info[c_ptr->feat];
	monster_race *r_ptr = &r_info[m_ptr->r_idx];

	/* DO NOT PLACE A MONSTER IN THE SMALL SCALE WILDERNESS !!! */
	if (p_ptr->wild_mode)
	{
		return 0;
	}

	/* Verify location */
	if (!in_bounds(y, x))
	{
		return 0;
	}

	/* Require empty space */
	if (!cave_empty_bold(y, x))
	{
		if (wizard) cmsg_format(TERM_L_RED, "WARNING: Refused monster(%d): EMPTY BOLD", m_ptr->r_idx);
		return 0;
	}

	/* Don't place any monsters on this type of terrain? */
	if (has_flag(f_ptr, FLAG_MONST_NO_PLACE))
	{
		if (wizard) cmsg_format(TERM_L_RED,
								"WARNING: Refused monster(%d): MONST_NO_PLACE",
								m_ptr->r_idx);

		return 0;
	}

	/* Paranoia */
	if (!m_ptr)
	{
		return 0;
	}

	if (!monster_can_cross_terrain(cave[y][x].feat, &m_ptr->flags))
	{
		if (wizard) cmsg_print(TERM_L_RED, "WARNING: Refused monster: cannot cross terrain");
		return 0;
	}

	/* Access the location */
	c_ptr = &cave[y][x];

	/* Make a new monster */
	c_ptr->m_idx = m_pop();
	hack_m_idx_ii = c_ptr->m_idx;

	/* Mega-Hack -- catch "failure" */
	if (!c_ptr->m_idx)
	{
		return 0;
	}

	/* Save it */
	flag_set_monster(&monst_list, c_ptr->m_idx, m_ptr);

	/* Place the monster at the location */
	m_ptr->fy = y;
	m_ptr->fx = x;

	/* No target yet */
	m_ptr->target = -1;

	/* Unknown distance */
	m_ptr->cdis = 0;

	/* No flags */
	m_ptr->mflag = 0;

	/* Not visible */
	m_ptr->ml = FALSE;

	/* Hack -- see "process_monsters()" */
	if (c_ptr->m_idx < hack_m_idx)
	{
		/* Monster is still being born */
		m_ptr->mflag |= (MFLAG_BORN);
	}

	/* Update the monster */
	update_mon(c_ptr->m_idx, TRUE);

	/* Hack -- Count the number of "reproducers" */
	if (has_flag(r_ptr, FLAG_MULTIPLY)) num_repro++;

	/* Hack -- Notice new multi-hued monsters */
	if (has_flag(r_ptr, FLAG_ATTR_MULTI)) shimmer_monsters = TRUE;

	/* Hack -- we need to modify the REAL r_info, not the fake one */
	r_ptr = &r_info[m_ptr->r_idx];

	/* Hack -- Count the monsters on the level */
	r_ptr->cur_num++;

	/* Success */
	place_monster_result = c_ptr->m_idx;
	return c_ptr->m_idx;
}

/*
 * Maximum size of a group of monsters
 */
#define GROUP_MAX	32


/*
 * Attempt to place a "group" of monsters around the given location
 */
static bool place_monster_group(s32b y, s32b x, s32b r_idx, s32b ego_idx, s32b max_nb, bool slp, s32b status)
{
	monster_race *r_ptr = &r_info[r_idx];

	s32b old, n, i;
	s32b total = 0;

	s32b hack_n = 0;

	byte hack_y[GROUP_MAX];
	byte hack_x[GROUP_MAX];

	s32b faction = -1;

	if (has_flag(r_ptr, FLAG_FACTION)) faction = get_flag(r_ptr, FLAG_FACTION);

	/* Pick a group size between 1 and max_nb */
	total = randint(max_nb);

	/* Maximum size */
	if (total > GROUP_MAX) total = GROUP_MAX;

	/* Save the rating */
	old = rating;

	/* Start on the monster */
	hack_n = 1;
	hack_x[0] = x;
	hack_y[0] = y;

	/* Puddle monsters, breadth first, up to total */
	for (n = 0; (n < hack_n) && (hack_n < total); n++)
	{
		/* Grab the location */
		s32b hx = hack_x[n];
		s32b hy = hack_y[n];

		/* Check each direction, up to total */
		for (i = 0; (i < 8) && (hack_n < total); i++)
		{
			s32b mx = hx + ddx_ddd[i];
			s32b my = hy + ddy_ddd[i];

			/* Walls and Monsters block flow */
			if (!cave_empty_bold(my, mx)) continue;

			/* Attempt to place another monster */
			if (place_monster_one(my, mx, r_idx, (ego_idx <= 0) ? pick_ego_monster(r_idx, faction) : ego_idx, slp, status))
			{
				/* Add it to the "hack" set */
				hack_y[hack_n] = my;
				hack_x[hack_n] = mx;
				hack_n++;
			}
		}
	}

	/* Hack -- restore the rating */
	rating = old;


	/* Success */
	return (TRUE);
}


/*
 * Hack -- help pick an escort type
 */
static s32b place_monster_idx = 0;

/*
 * Hack -- help pick an escort type
 */
static bool rand_r_idx_okay(s32b r_idx);
static s32b escort_min_level;
static s32b escort_max_level;
static s32b escort_faction;
static bool place_escort_okay(s32b r_idx)
{
	monster_race *z_ptr    = &r_info[r_idx];
	s32b          z_faction;

	bool debug = FALSE;

	/* Paranoia -- Skip identical monsters */
	if (place_monster_idx == r_idx)
	{
		if (debug)
			printf("%s not escorting: identical race\n", z_ptr->name);
		return (FALSE);
	}


	/* Skip more advanced monsters */
	if (z_ptr->level > escort_max_level)
	{
		if (debug)
			printf("%s not escorting: level too high\n", z_ptr->name);
		return (FALSE);
	}

	/* Skip too weak monsters */
	if (z_ptr->level < escort_min_level)
	{
		if (debug)
			printf("%s not escorting: level too low\n", z_ptr->name);
		return (FALSE);
	}

	/* Skip unique monsters */
	if (has_flag(z_ptr, FLAG_UNIQUE))
	{
		if (debug)
			printf("%s not escorting: unique\n", z_ptr->name);
		return (FALSE);
	}

	/* Hack - Escorts have to have the same dungeon flag */
	if (monster_dungeon(place_monster_idx) != monster_dungeon(r_idx))
	{
		if (debug)
			printf("%s not escorting: unsuitable dungeon type\n", z_ptr->name);
		return (FALSE);
	}

	/* Friendliness has to go both ways */
	if (has_flag(z_ptr, FLAG_FACTION))
		z_faction = get_flag(z_ptr, FLAG_FACTION);
	else
		z_faction = FACTION_DUNGEON;
	if (get_faction_friendliness(z_faction, escort_faction) < 1)
	{
		if (debug)
			printf("%s not escorting: not friendly\n", z_ptr->name);
		return (FALSE);
	}

	/* Re-use the get_rand_r_idx() code */
	if (!rand_r_idx_okay(r_idx))
	{
		if (debug)
			printf("%s not escorting: rand_r_idx_okay() failed\n",
				   z_ptr->name);
		return (FALSE);
	}

	if (debug)
		printf("%s escorting\n",
			   z_ptr->name);

	/* Okay */
	return (TRUE);
} /* place_escort_okay() */


static void set_rand_r_idx_vars(s32b lev, s32b faction, s32b min_friend,
								s32b max_friend, flags_type* flags,
								flags_type* flags_forbid, cptr chars,
								cptr chars_forbid, cptr name,
								cptr name_forbid);

/*
 * Attempt to place a monster of the given race at the given location
 *
 * Note that certain monsters are now marked as requiring "friends".
 * These monsters, if successfully placed, and if the "grp" parameter
 * is TRUE, will be surrounded by a "group" of identical monsters.
 *
 * Note that certain monsters are now marked as requiring an "escort",
 * which is a collection of monsters with similar "race" but lower level.
 *
 * Some monsters induce a fake "group" flag on their escorts.
 *
 * Note the "bizarre" use of non-recursion to prevent annoying output
 * when running a code profiler.
 *
 * Note the use of the new "monster allocation table" code to restrict
 * the "get_mon_num()" function to "legal" escort types.
 */
static bool recent_escort_cascade = FALSE;
static int  escort_cascade_level  = 0;

extern s16b place_monster_aux_ego(s32b y, s32b x, s32b r_idx, s32b ego,
								  bool slp, bool grp, s32b status)
{
	s32b i;
	monster_race *r_ptr = &r_info[r_idx];
	bool (*old_get_mon_num_hook)(s32b r_idx);
	s32b faction = -1;
	s16b m_idx = place_monster_one(y, x, r_idx, ego, slp, status);

	/* Place one monster, or fail */
	if (m_idx <= 0 ) return(m_idx);

	/* Require the "group" flag */
	if (!grp) return (m_idx);

	if (has_flag(&re_info[ego], FLAG_FACTION))
		faction = get_flag(&re_info[ego], FLAG_FACTION);
	else if (has_flag(&r_info[r_idx], FLAG_FACTION))
		faction = get_flag(&r_info[r_idx], FLAG_FACTION);

	/* Friends for certain monsters */
	if (has_flag(r_ptr, FLAG_FRIENDS) && magik(get_flag(r_ptr, FLAG_FRIENDS)))
	{
		/* Attempt to place a group */
		(void)place_monster_group(y, x, r_idx, ego, get_flag2(r_ptr, FLAG_FRIENDS), slp, status);
	}

	/* Escorts for certain monsters */
	if (has_flag(r_ptr, FLAG_ESCORT))
	{
		s32b        num, min_num, max_num, tries, placed;
		char        default_chars[2];
		flags_type* escort_flags = flag_get_flags(&r_ptr->flags, FLAG_ESCORT_FLAGS);
		cptr        escort_chars = flag_get_string(&r_ptr->flags, FLAG_ESCORT_CHARS);
		cptr        escort_name  = flag_get_string(&r_ptr->flags, FLAG_ESCORT_NAME);

		flags_type* escort_dont         = flag_new(2);
		flags_type* escort_flags_forbid =
			flag_get_flags(&r_ptr->flags, FLAG_ESCORT_FLAGS_FORBID);

		escort_cascade_level++;

		if (faction == -1)
			faction = FACTION_DUNGEON;

		old_get_mon_num_hook = get_mon_num_hook;

		/*
		 * Setup variables for get_rand_r_idx() code we're re-using.
		 * Make sure that the escort monsters are friendly to the
		 * monster being escorted.
		 */
		if (!escort_chars && !escort_name && !escort_flags &&
			!escort_flags_forbid)
		{
			/* 
			 * Only use the default escort definition of "same display
			 * char as escorted" if no escort definition info is
			 * supplied.
			 */
			default_chars[0] = r_ptr->d_char;
			default_chars[1] = '\0';
			escort_chars     = default_chars;
		}

		flag_set(escort_dont, FLAG_ESCORT_DONT, TRUE);
		if (escort_flags_forbid)
			flag_copy(escort_dont, escort_flags_forbid);

		min_num = get_flag(r_ptr,  FLAG_ESCORT);
		max_num = get_flag2(r_ptr, FLAG_ESCORT);

		if (min_num == 1 && max_num == 0)
		{
			/* Monster definition simply says "ESCORT=true" */
			num   = 50;
			tries = 50;
		}
		else
		{
			max_num = MAX(min_num, max_num);

			num   = min_num + rand_int(1 + max_num - min_num);
			tries = num * 2;
		}

		/* Try to place several "escorts" */
		placed = 0;
		for (i = 0; i < tries && placed < num; i++)
		{
			s32b nx, ny, z, d = 3;

			if (i == 0 || recent_escort_cascade)
			{
				recent_escort_cascade = FALSE;
				/*
				 * Re-do (or do for first time) monster allocation table
				 * for escorts.
				 */
				escort_min_level = get_flag(r_ptr, FLAG_ESCORT_MIN_LEVEL);
				escort_max_level = get_flag(r_ptr, FLAG_ESCORT_MAX_LEVEL);
				escort_faction   = faction;

				if (escort_max_level == 0)
					escort_max_level = r_ptr->level;

				set_rand_r_idx_vars(r_ptr->level, faction, 1, 32000, escort_flags, escort_dont, escort_chars, NULL, escort_name, NULL);

				/* Set the escort index */
				place_monster_idx = r_idx;

				/* Set the escort hook */
				get_mon_num_hook = place_escort_okay;

				/* Prepare allocation table */
				get_mon_num_prep();
			} /* if (i == 0 || recent_escort_cascade) */

			/* Pick a location */
			scatter(&ny, &nx, y, x, d, 0);

			/* Require empty grids */
			if (!cave_empty_bold(ny, nx))
				continue;

			/* Ignore the dungeon restrictions */
			if (has_flag(r_ptr, FLAG_ESCORT_IGNORE_DUNGEON))
				summon_hack = TRUE;
			/* Pick a random race */
			z = get_mon_num(r_ptr->level);
			summon_hack = FALSE;

			/* Handle failure */
			if (!z)
				break;

			/* Attempt to place a single escort */
			if (!place_monster_aux_ego(ny, nx, z, pick_ego_monster(z, faction), slp, TRUE, status))
				continue;

			placed++;

			/* Place a "group" of escorts if needed */
			if ((has_flag(&r_info[z], FLAG_FRIENDS) &&
				 magik(get_flag(&r_info[z], FLAG_FRIENDS))) ||
				(has_flag(r_ptr, FLAG_ESCORTED_BY_GROUPS)))
			{
				/* Place a group of monsters */
				s32b num = MAX(get_flag2(&r_info[z], FLAG_FRIENDS), get_flag(r_ptr, FLAG_ESCORTED_BY_GROUPS));

				/* Default number in T2 was 13 */
				if (num <= 1)
					num = 13;

				(void)place_monster_group(ny, nx, z, -1, num, slp, status);
			}
		} /* for (i = 0; i < tries && placed < num; i++) */

		recent_escort_cascade = TRUE;
		escort_cascade_level--;

		if (escort_cascade_level == 0)
			recent_escort_cascade = FALSE;

		/* Reset restriction */
		get_mon_num_hook = old_get_mon_num_hook;

		/* Prepare allocation table */
		if ((get_mon_num_hook != place_escort_okay) &&
			(get_mon_num_hook != rand_r_idx_okay))
			get_mon_num_prep();

		flag_free(escort_dont, TRUE);
	} /* if (has_flag(r_ptr, FLAG_ESCORT)) */

	/* Success */
	return m_idx;
}

bool place_monster_aux(s32b y, s32b x, s32b r_idx, bool slp, bool grp,
					   s32b status)
{
	s32b ego_idx = pick_ego_monster(r_idx, -1);
	s16b m_idx = 
		place_monster_aux_ego(y, x, r_idx, ego_idx, slp, grp, status);

	return (m_idx > 0);
}

/*
 * Hack -- attempt to place a monster at the given location
 *
 * Attempt to find a monster appropriate to the "monster_level"
 */
bool place_monster(s32b y, s32b x, bool slp, bool grp)
{
	s32b r_idx;

	/* Set monster restriction */
	set_mon_num2_hook(y, x);

	/* Prepare allocation table */
	get_mon_num_prep();

	/* Pick a monster */
	r_idx = get_mon_num(monster_level);

	/* Reset restriction */
	get_mon_num2_hook = NULL;

	/* Prepare allocation table */
	get_mon_num_prep();

	/* Handle failure */
	if (!r_idx) return (FALSE);

	/* Attempt to place the monster */
	if (place_monster_aux(y, x, r_idx, slp, grp, FACTION_DUNGEON)) return (TRUE);

	/* Oops */
	return (FALSE);
}


bool alloc_horde(s32b y, s32b x)
{
	s32b           r_idx    = 0;
	monster_race *r_ptr    = NULL;
	monster_type *m_ptr;
	s32b           attempts = 1000;
	char          summon_kin_type[2];
	flags_type   *no_uniques;

	set_mon_num2_hook(y, x);

	/* Prepare allocation table */
	get_mon_num_prep();

	while (--attempts)
	{
		/* Pick a monster */
		r_idx = get_mon_num(monster_level);

		/* Handle failure */
		if (!r_idx) return (FALSE);

		r_ptr = &r_info[r_idx];

		if (!(has_flag(r_ptr, FLAG_UNIQUE))
		                && !(has_flag(r_ptr, FLAG_ESCORT)))
			break;
	}

	get_mon_num2_hook = NULL;

	/* Prepare allocation table */
	get_mon_num_prep();

	if (attempts < 1) return FALSE;

	attempts = 1000;

	while (--attempts)
	{
		/* Attempt to place the monster */
		if (place_monster_aux(y, x, r_idx, FALSE, FALSE, FACTION_DUNGEON)) break;
	}

	if (attempts < 1) return FALSE;


	m_ptr = get_monster(hack_m_idx_ii);

	summon_kin_type[0] = r_ptr->d_char;
	summon_kin_type[1] = '\0';

	no_uniques = flag_new(2);
	flag_set(no_uniques, FLAG_UNIQUE, TRUE);

	for (attempts = randint(10) + 5; attempts; attempts--)
	{
		(void) summon_specific(m_ptr->fy, m_ptr->fx, dun_level,
							   m_ptr->faction, 0, INT_MAX, TRUE, NULL,
							   no_uniques, summon_kin_type, NULL,
							   NULL, NULL);
	}

	flag_free(no_uniques, TRUE);

	return TRUE;
}

/*
 * Summon any random monster, except for uniques.
 */
extern bool summon_random(s32b y1, s32b x1, s32b lev, s32b faction)
{
	flags_type *no_uniques = flag_new(2);
	flag_set(no_uniques, FLAG_UNIQUE, TRUE);

	(void) summon_specific(y1, x1, lev, faction, 0, INT_MAX, TRUE,
						   NULL, no_uniques, NULL, NULL, NULL, NULL);

	flag_free(no_uniques, TRUE);

	return(TRUE);
} /* summon_random() */


/*
 * Attempt to allocate a random monster in the dungeon.
 *
 * Place the monster at least "dis" distance from the player.
 *
 * Use "slp" to choose the initial "sleep" status
 *
 * Use "monster_level" for the monster level
 */
bool alloc_monster(s32b dis, bool slp)
{
	s32b	y, x;
	s32b attempts_left = 10000;

	/* Find a legal, distant, unoccupied, space */
	while (attempts_left--)
	{
		/* Pick a location */
		y = rand_int(cur_hgt);
		x = rand_int(cur_wid);

		/* Require empty floor grid (was "naked") */
		if (!cave_empty_bold(y, x)) continue;

		/* Accept far away grids */
		if (distance(y, x, p_ptr->py, p_ptr->px) > dis) break;
	}

	if (!attempts_left)
	{
		if (cheat_xtra || cheat_hear || wizard)
		{
			msg_print("Warning! Could not allocate a new monster. Small level?");
		}

		return (FALSE);
	}


	if (has_flag(dungeon_flags, FLAG_MONSTER_HORDE) && (randint(get_flag(dungeon_flags, FLAG_MONSTER_HORDE)) <= dun_level))
	{
		if (alloc_horde(y, x))
		{
			if ((cheat_hear) || (p_ptr->precognition)) msg_print("Monster horde.");
			return (TRUE);
		}
	}
	else
	{
		/* Attempt to place the monster, allow groups */
		if (place_monster(y, x, slp, TRUE)) return (TRUE);
	}

	/* Nope */
	return (FALSE);
}

/*
 * Hack -- the "type" of the current "summon specific"
 */
static flags_type* rand_r_idx_flags        = NULL;
static flags_type* rand_r_idx_flags_forbid = NULL;
static cptr        rand_r_idx_chars        = NULL;
static cptr        rand_r_idx_chars_forbid = NULL;
static cptr        rand_r_idx_name         = NULL;
static cptr        rand_r_idx_name_forbid  = NULL;
static u32b        rand_r_idx_faction      = 0;
static s32b        rand_r_idx_min_friend   = 1;
static s32b        rand_r_idx_max_friend   = -1;

/*
 * Hack -- help decide if a monster race is "okay" to summon.
 */
static bool rand_r_idx_okay(s32b r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	s32b          friend;
	s32b          faction;

	bool debug = FALSE;

	if (has_flag(r_ptr, FLAG_SPECIAL_GENE))
	{
		if (debug)
			printf("%s: SPECIAL GENE\n", r_ptr->name);
		return(FALSE);
	}

	if (has_flag(r_ptr, FLAG_NEVER_GENE))
	{
		if (debug)
			printf("%s: NEVER GENE\n", r_ptr->name);
		return(FALSE);
	}

	if (rand_r_idx_flags)
		if (!flag_contains(&r_ptr->flags, rand_r_idx_flags))
	{
		if (debug)
			printf("%s: doesn't have right flags\n", r_ptr->name);
			return (FALSE);
	}

	if (rand_r_idx_flags_forbid)
		if (flag_intersects(&r_ptr->flags, rand_r_idx_flags_forbid))
	{
		if (debug)
			printf("%s: has wrong flags\n", r_ptr->name);
			return (FALSE);
	}

	if (rand_r_idx_chars)
	{
		s32b  i;
		bool found = FALSE;

		for (i = 0; rand_r_idx_chars[i]; i++)
			if (r_ptr->d_char == rand_r_idx_chars[i])
			{
				found = TRUE;
				break;
			}

		if (!found)
		{
			if (debug)
				printf("%s: doesn't match race characters\n", r_ptr->name);
			return(FALSE);
		}
	}

	if (rand_r_idx_chars_forbid)
	{
		s32b  i;
		bool found = FALSE;

		for (i = 0; rand_r_idx_chars_forbid[i]; i++)
			if (r_ptr->d_char == rand_r_idx_chars_forbid[i])
			{
				found = TRUE;
				break;
			}

		if (found)
		{
			if (debug)
				printf("%s: has wrong race charactrs\n", r_ptr->name);
			return(FALSE);
		}
	}

	if (rand_r_idx_name)
		if (!strstr(r_ptr->name, rand_r_idx_name))
		{
			if (debug)
				printf("%s: wrong name\n", r_ptr->name);
			return(FALSE);
		}

	if (rand_r_idx_name_forbid)
		if (strstr(r_ptr->name, rand_r_idx_name_forbid))
		{
			if (debug)
				printf("%s: wrong name 2\n", r_ptr->name);
			return(FALSE);
		}

	/*
	 * Don't use a unique if it's alive on this level or dead.
	 */
	if (has_flag(r_ptr, FLAG_UNIQUE))
		if (r_ptr->cur_num > 0 || r_ptr->max_num == 0)
		{
			if (debug)
				printf("%s: already existing uniqe\n", r_ptr->name);
			return(FALSE);
		}

	/* HACK: Do we care even about faction friendlyness? */
	if (rand_r_idx_min_friend > rand_r_idx_max_friend)
		return(TRUE);

	if (has_flag(r_ptr, FLAG_FACTION))
		faction = get_flag(r_ptr, FLAG_FACTION);
	else
		faction = FACTION_DUNGEON;

	friend = get_faction_friendliness(rand_r_idx_faction, faction);

	if (friend > rand_r_idx_max_friend)
	{
		if (debug)
			printf("%s: too friendly (%ld > %ld)\n", r_ptr->name,
				   friend, rand_r_idx_max_friend);
		return(FALSE);
	}

	if (friend < rand_r_idx_min_friend)
	{
		if (debug)
			printf("%s: not friendly enough (%ld < %ld)\n", r_ptr->name,
				   friend, rand_r_idx_min_friend);
		return(FALSE);
	}

	return(TRUE);
}

static void set_rand_r_idx_vars(s32b lev, s32b faction, s32b min_friend,
								s32b max_friend, flags_type* flags,
								flags_type* flags_forbid, cptr chars,
								cptr chars_forbid, cptr name,
								cptr name_forbid)
{
	rand_r_idx_flags        = flags;
	rand_r_idx_flags_forbid = flags_forbid;
	rand_r_idx_chars        = chars;
	rand_r_idx_chars_forbid = chars_forbid;
	rand_r_idx_name         = name;
	rand_r_idx_name_forbid  = name_forbid;
	rand_r_idx_faction      = faction;
	rand_r_idx_min_friend   = min_friend;
	rand_r_idx_max_friend   = max_friend;
} 

/*
 * Get a random race index that matches the given conditions.
 *
 * Note that we use the new "monster allocation table" creation code
 * to restrict the "get_mon_num()" function to the set of "legal"
 * monsters, making this function much faster and more reliable.
 */
s32b get_rand_r_idx(s32b lev, s32b faction, s32b min_friend, s32b max_friend,
				   flags_type* flags, flags_type* flags_forbid,
				   cptr chars, cptr chars_forbid,
				   cptr name, cptr name_forbid)
{
	bool (*old_get_mon_num_hook)(s32b r_idx);
	s32b  r_idx;

	/* Backup the old hook */
	old_get_mon_num_hook = get_mon_num_hook;

	/* Require "okay" monsters */
	get_mon_num_hook = rand_r_idx_okay;

	rand_r_idx_flags        = flags;
	rand_r_idx_flags_forbid = flags_forbid;
	rand_r_idx_chars        = chars;
	rand_r_idx_chars_forbid = chars_forbid;
	rand_r_idx_name         = name;
	rand_r_idx_name_forbid  = name_forbid;
	rand_r_idx_faction      = faction;
	rand_r_idx_min_friend   = min_friend;
	rand_r_idx_max_friend   = max_friend;

	/* Prepare allocation table */
	get_mon_num_prep();

	/* Pick a monster, using the level calculation */
	summon_hack = TRUE;
	r_idx = get_mon_num(lev);
	summon_hack = FALSE;

#ifdef R_IDX_TESTING_HACK
	r_idx = 356;
#endif

	/* Reset restriction */
	get_mon_num_hook = old_get_mon_num_hook;

	/* Prepare allocation table */
	get_mon_num_prep();

	return r_idx;
}


/*
 * Place a monster (of the specified "type") near the given
 * location.  Return TRUE if a monster was actually summoned.
 *
 * We will attempt to place the monster up to 20 times before giving up.
 *
 * This function has been changed.  We now take the "monster level"
 * of the summoning monster as a parameter, and use that, along with
 * the current dungeon level, to help determine the level of the
 * desired monster.  Note that this is an upper bound, and also
 * tends to "prefer" monsters of that level.  Currently, we use
 * the average of the dungeon and monster levels, and then add
 * five to allow slight increases in monster power.
 *
 * Note that this function may not succeed, though this is very rare.
 */
s32b summon_specific_level = 0;
bool summon_specific(s32b y1, s32b x1, s32b lev, s32b faction,
					 s32b min_friend, s32b max_friend, bool group_ok,
					 flags_type* flags, flags_type* flags_forbid,
					 cptr chars, cptr chars_forbid,
					 cptr name, cptr name_forbid)
{
	s32b i, x, y, r_idx;

	/* Look for a location */
	for (i = 0; i < 20; ++i)
	{
		cave_type    *c_ptr;
		feature_type *f_ptr;
		/* Pick a distance */
		s32b d = (i / 15) + 1;

		/* Pick a location */
		scatter(&y, &x, y1, x1, d, 0);

		c_ptr = &cave[y][x];
		f_ptr = &f_info[c_ptr->feat];

		/* Require "empty" floor grid */
		if (!cave_empty_bold(y, x)) continue;

		/* Hack -- no summon on glyph of warding */

		/* Don't place any monsters on this type of terrain? */
		if (has_flag(f_ptr, FLAG_MONST_NO_PLACE))
			continue;

		/* Okay */
		break;
	}

	/* Failure */
	if (i == 20) return (FALSE);

	r_idx = get_rand_r_idx((dun_level + lev) / 2 + 5, faction,
						   min_friend, max_friend,
						   flags, flags_forbid, chars, chars_forbid,
						   name, name_forbid);

	/* Handle failure */
	if (!r_idx) {
		if (wizard)
			cmsg_format(TERM_VIOLET, "ERROR: summon_specific()'s monster "
						"hook excluded all monster races.");
		return (FALSE);
	}


	/* Attempt to place the monster (awake, allow groups) */
	if (!place_monster_aux(y, x, r_idx, FALSE, group_ok, faction))
		return (FALSE);

	if (summon_specific_level)
	{
		monster_set_level(place_monster_result, summon_specific_level);
		summon_specific_level = 0;
	}

	/* Success */
	return (TRUE);
}

/*
 * Swap the players/monsters (if any) at two locations XXX XXX XXX
 */
void monster_swap(s32b y1, s32b x1, s32b y2, s32b x2)
{
	s32b m1, m2;

	monster_type *m_ptr;
	cave_type *c_ptr1, *c_ptr2;

	c_ptr1 = &cave[y1][x1];
	c_ptr2 = &cave[y2][x2];

	/* Monsters */
	m1 = c_ptr1->m_idx;
	m2 = c_ptr2->m_idx;


	/* Update grids */
	c_ptr1->m_idx = m2;
	c_ptr2->m_idx = m1;


	/* Monster 1 */
	if (m1 > 0)
	{
		m_ptr = get_monster(m1);

		/* Move monster */
		m_ptr->fy = y2;
		m_ptr->fx = x2;

		/* Update monster */
		update_mon(m1, TRUE);
	}

	/* Player 1 */
	else if (m1 < 0)
	{
		/* Move player */
		p_ptr->py = y2;
		p_ptr->px = x2;

		/* Check for new panel (redraw map) */
		verify_panel();

		/* Update stuff */
		p_ptr->update |= (PU_VIEW | PU_FLOW | PU_MON_LITE);

		/* Update the monsters */
		p_ptr->update |= (PU_DISTANCE);

		/* Window stuff */
		/* It's probably not a good idea to recalculate the
		 * overhead view each turn.

		 p_ptr->window |= (PW_OVERHEAD);
		*/
	}

	/* Monster 2 */
	if (m2 > 0)
	{
		m_ptr = get_monster(m2);

		/* Move monster */
		m_ptr->fy = y1;
		m_ptr->fx = x1;

		/* Update monster */
		update_mon(m2, TRUE);
	}

	/* Player 2 */
	else if (m2 > 0)
	{
		/* Move player */
		p_ptr->py = y1;
		p_ptr->px = x1;

		/* Check for new panel (redraw map) */
		verify_panel();

		/* Update stuff */
		p_ptr->update |= (PU_VIEW | PU_FLOW | PU_MON_LITE);

		/* Update the monsters */
		p_ptr->update |= (PU_DISTANCE);

		/* Window stuff */
		/* It's probably not a good idea to recalculate the
		 * overhead view each turn.

		 p_ptr->window |= (PW_OVERHEAD);
		*/
	}


	/* Redraw */
	lite_spot(y1, x1);
	lite_spot(y2, x2);
}


/*
 * Hack -- help decide if a monster race is "okay" to summon
 */
static bool mutate_monster_okay(s32b r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	bool okay = FALSE;

	/* Hack - Only summon dungeon monsters */
	if (!monster_dungeon(r_idx)) return (FALSE);

	okay = ((r_ptr->d_char == summon_kin_type) && !(has_flag(r_ptr, FLAG_UNIQUE))
	        && (r_ptr->level >= dun_level));

	return okay;
}


/*
 * Let the given monster attempt to reproduce.
 *
 * Note that "reproduction" REQUIRES empty space.
 */
bool multiply_monster(s32b m_idx, bool charm, bool clone)
{
	monster_type	*m_ptr = get_monster(m_idx);
	monster_race *r_ptr = race_inf(m_ptr);

	s32b i, y, x, new_race;

	bool result = FALSE;

	if (no_breeds)
	{
		msg_print("It tries to breed but he fails!");
		return FALSE;
	}

	/* Try up to 18 times */
	for (i = 0; i < 18; i++)
	{
		s32b d = 1;


		/* Pick a location */
		scatter(&y, &x, m_ptr->fy, m_ptr->fx, d, 0);

		/* Require an "empty" floor grid */
		if (!cave_empty_bold(y, x)) continue;

		new_race = m_ptr->r_idx;

		/* It can mutate into a nastier monster */
		if ((rand_int(100) < 3) && (!clone))
		{
			bool (*old_get_mon_num_hook)(s32b r_idx);

			/* Backup the old hook */
			old_get_mon_num_hook = get_mon_num_hook;

			/* Require "okay" monsters */
			get_mon_num_hook = mutate_monster_okay;

			/* Prepare allocation table */
			get_mon_num_prep();

			summon_kin_type = r_ptr->d_char;

			/* Pick a monster, using the level calculation */
			new_race = get_mon_num(dun_level + 5);

			/* Reset restriction */
			get_mon_num_hook = old_get_mon_num_hook;

			/* Prepare allocation table */
			get_mon_num_prep();
		}

		/* Create a new monster (awake, no groups) */
		result = place_monster_aux(y, x, new_race, FALSE, FALSE, (charm) ? FACTION_PLAYER : FACTION_DUNGEON);

		/* Done */
		break;
	}

	m_ptr = get_monster(hack_m_idx_ii);
	if (clone && result) m_ptr->smart |= SM_CLONED;

	/* Result */
	return (result);
}





/*
 * Dump a message describing a monster's reaction to damage
 *
 * Technically should attempt to treat "Beholder"'s as jelly's
 */
bool hack_message_pain_may_silent = FALSE;
void message_pain_hook(cptr fmt, ...)
{
	va_list vp;

	char buf[1024];

	/* Begin the Varargs Stuff */
	va_start(vp, fmt);

	/* Format the args, save the length */
	(void)vstrnfmt(buf, 1024, fmt, vp);

	/* End the Varargs Stuff */
	va_end(vp);

	if (hack_message_pain_may_silent)
		monster_msg(buf);
	else
		msg_print(buf);
}

void message_pain(s32b m_idx, s32b dam)
{
	long oldhp, newhp, tmp;
	s32b percentage;
	monster_type *m_ptr = get_monster(m_idx);
	monster_race *r_ptr = race_inf(m_ptr);
	char m_name[80];

#if 0
	if (!(player_can_see_bold(m_ptr->fy, m_ptr->fx)))
		return;
#endif

	/* Get the monster name */
	monster_desc(m_name, m_ptr, 0);

	/* Notice non-damage */
	if (dam == 0)
	{
		message_pain_hook("%^s is unharmed.", m_name);
		return;
	}

	/* Note -- subtle fix -CFT */
	newhp = (long)(m_ptr->hp);
	oldhp = newhp + (long)(dam);
	tmp = (newhp * 100L) / oldhp;
	percentage = (s32b)(tmp);


	/* Jelly's, Mold's, Vortex's, Quthl's */
	if (strchr("jmvQ", r_ptr->d_char))
	{
		if (percentage > 95)
			message_pain_hook("%^s barely notices.", m_name);
		else if (percentage > 75)
			message_pain_hook("%^s flinches.", m_name);
		else if (percentage > 50)
			message_pain_hook("%^s squelches.", m_name);
		else if (percentage > 35)
			message_pain_hook("%^s quivers in pain.", m_name);
		else if (percentage > 20)
			message_pain_hook("%^s writhes about.", m_name);
		else if (percentage > 10)
			message_pain_hook("%^s writhes in agony.", m_name);
		else
			message_pain_hook("%^s jerks limply.", m_name);
	}

	/* Dogs and Hounds */
	else if (strchr("CZ", r_ptr->d_char))
	{
		if (percentage > 95)
			message_pain_hook("%^s shrugs off the attack.", m_name);
		else if (percentage > 75)
			message_pain_hook("%^s snarls with pain.", m_name);
		else if (percentage > 50)
			message_pain_hook("%^s yelps in pain.", m_name);
		else if (percentage > 35)
			message_pain_hook("%^s howls in pain.", m_name);
		else if (percentage > 20)
			message_pain_hook("%^s howls in agony.", m_name);
		else if (percentage > 10)
			message_pain_hook("%^s writhes in agony.", m_name);
		else
			message_pain_hook("%^s yelps feebly.", m_name);
	}

	/* One type of monsters (ignore,squeal,shriek) */
	else if (strchr("FIKMRSXabclqrst", r_ptr->d_char))
	{
		if (percentage > 95)
			message_pain_hook("%^s ignores the attack.", m_name);
		else if (percentage > 75)
			message_pain_hook("%^s grunts with pain.", m_name);
		else if (percentage > 50)
			message_pain_hook("%^s squeals in pain.", m_name);
		else if (percentage > 35)
			message_pain_hook("%^s shrieks in pain.", m_name);
		else if (percentage > 20)
			message_pain_hook("%^s shrieks in agony.", m_name);
		else if (percentage > 10)
			message_pain_hook("%^s writhes in agony.", m_name);
		else
			message_pain_hook("%^s cries out feebly.", m_name);
	}

	/* Another type of monsters (shrug,cry,scream) */
	else
	{
		if (percentage > 95)
			message_pain_hook("%^s shrugs off the attack.", m_name);
		else if (percentage > 75)
			message_pain_hook("%^s grunts with pain.", m_name);
		else if (percentage > 50)
			message_pain_hook("%^s cries out in pain.", m_name);
		else if (percentage > 35)
			message_pain_hook("%^s screams in pain.", m_name);
		else if (percentage > 20)
			message_pain_hook("%^s screams in agony.", m_name);
		else if (percentage > 10)
			message_pain_hook("%^s writhes in agony.", m_name);
		else
			message_pain_hook("%^s cries out feebly.", m_name);
	}
}



/*
 * Learn about an "observed" resistance.
 */
void update_smart_learn(s32b m_idx, s32b what)
{
	monster_type *m_ptr = get_monster(m_idx);


	/* Not allowed to learn */
	if (!smart_learn) return;

	/* Too stupid to learn anything */
	if (has_flag(m_ptr, FLAG_STUPID)) return;

	/* Not intelligent, only learn sometimes */
	if (!(has_flag(m_ptr, FLAG_SMART)) && (rand_int(100) < 50)) return;


	/* XXX XXX XXX */

#if 0 // DGDGDGDG
	/* Analyze the knowledge */
	switch (what)
	{
	case DRS_ACID:
		if (intrinsic(RES_ACID)) m_ptr->smart |= (SM_RES_ACID);
		if (p_ptr->oppose_acid) m_ptr->smart |= (SM_OPP_ACID);
		if (intrinsic(IM_ACID)) m_ptr->smart |= (SM_IMM_ACID);
		break;

	case DRS_ELEC:
		if (intrinsic(RES_ELEC)) m_ptr->smart |= (SM_RES_ELEC);
		if (p_ptr->oppose_elec) m_ptr->smart |= (SM_OPP_ELEC);
		if (intrinsic(IM_ELEC)) m_ptr->smart |= (SM_IMM_ELEC);
		break;

	case DRS_FIRE:
		if (intrinsic(RES_FIRE)) m_ptr->smart |= (SM_RES_FIRE);
		if (p_ptr->oppose_fire) m_ptr->smart |= (SM_OPP_FIRE);
		if (intrinsic(IM_FIRE)) m_ptr->smart |= (SM_IMM_FIRE);
		break;

	case DRS_COLD:
		if (intrinsic(RES_COLD)) m_ptr->smart |= (SM_RES_COLD);
		if (p_ptr->oppose_cold) m_ptr->smart |= (SM_OPP_COLD);
		if (intrinsic(IM_COLD)) m_ptr->smart |= (SM_IMM_COLD);
		break;

	case DRS_POIS:
		if (intrinsic(RES_POIS)) m_ptr->smart |= (SM_RES_POIS);
		if (p_ptr->oppose_pois) m_ptr->smart |= (SM_OPP_POIS);
		break;


	case DRS_NETH:
		if (intrinsic(RES_NETH)) m_ptr->smart |= (SM_RES_NETH);
		break;

	case DRS_LITE:
		if (intrinsic(RES_LITE)) m_ptr->smart |= (SM_RES_LITE);
		break;

	case DRS_DARK:
		if (intrinsic(RES_DARK)) m_ptr->smart |= (SM_RES_DARK);
		break;

	case DRS_FEAR:
		if (intrinsic(RES_FEAR)) m_ptr->smart |= (SM_RES_FEAR);
		break;

	case DRS_CONF:
		if (intrinsic(RES_CONF)) m_ptr->smart |= (SM_RES_CONF);
		break;

	case DRS_CHAOS:
		if (intrinsic(RES_CHAOS)) m_ptr->smart |= (SM_RES_CHAOS);
		break;

	case DRS_DISEN:
		if (intrinsic(RES_DISEN)) m_ptr->smart |= (SM_RES_DISEN);
		break;

	case DRS_BLIND:
		if (intrinsic(RES_BLIND)) m_ptr->smart |= (SM_RES_BLIND);
		break;

	case DRS_NEXUS:
		if (intrinsic(RES_NEXUS)) m_ptr->smart |= (SM_RES_NEXUS);
		break;

	case DRS_SOUND:
		if (intrinsic(RES_SOUND)) m_ptr->smart |= (SM_RES_SOUND);
		break;

	case DRS_SHARD:
		if (intrinsic(RES_SHARDS)) m_ptr->smart |= (SM_RES_SHARD);
		break;


	case DRS_FREE:
		if (intrinsic(FREE_ACT)) m_ptr->smart |= (SM_IMM_FREE);
		break;

	case DRS_MANA:
		if (!p_ptr->msp) m_ptr->smart |= (SM_IMM_MANA);
		break;

	case DRS_REFLECT:
		if (intrinsic(REFLECT)) m_ptr-> smart |= (SM_IMM_REFLECT);
		break;
	}
#endif
}


/*
 * Place the player in the dungeon XXX XXX
 */
s16b player_place(s32b y, s32b x)
{
	/* Paranoia XXX XXX */
	if (cave[y][x].m_idx != 0) return (0);

	/* Save player location */
	p_ptr->py = y;
	p_ptr->px = x;

	/* Success */
	return ( -1);
}

/*
 * Drop all items carried by a monster
 */
void monster_drop_carried_objects(monster_type *m_ptr)
{
	/* Drop objects being carried */
	for_inventory(m_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		/* Remove from the inventory, but do not free the object */
		flag_remove_zap(get_inven(m_ptr, inven_nb), item_nb);

		/* Paranoia */
		o_ptr->held_m_idx = 0;

		/* Drop it */
		drop_near(o_ptr, -1, m_ptr->fy, m_ptr->fx);
	}
	end_inventory();
}

bool is_obj_mimic_at(s32b y, s32b x)
{
	return (get_obj_mimic_obj_at(y, x) != NULL);
}

object_type* get_obj_mimic_obj_at(s32b y, s32b x)
{
	cave_type *c_ptr = &cave[y][x];

	if (c_ptr->m_idx)
	{
		monster_type *m_ptr = get_monster(c_ptr->m_idx);
		if (has_flag(m_ptr, FLAG_MIMIC))
		{
			object_type *o_ptr;

			/* Acquire object */
			o_ptr = flag_get_obj(get_inven(m_ptr, INVEN_INVEN), 1);

			return(o_ptr);
		}
	}

	return NULL;
}

extern bool is_feat_mimic_at(s32b y, s32b x)
{
	return (get_feat_mimic_feat_at(y, x) != -1);
}

extern s16b get_feat_mimic_feat_at(s32b y, s32b x)
{
	cave_type *c_ptr = &cave[y][x];

	if (c_ptr->m_idx)
	{
		monster_type *m_ptr = get_monster(c_ptr->m_idx);
		if (has_flag(m_ptr, FLAG_MIMIC))
		{
			object_type *o_ptr;

			/* Acquire object */
			o_ptr = flag_get_obj(get_inven(m_ptr, INVEN_INVEN), 1);

			if (o_ptr != NULL)
				return(-1);

			return(get_flag(m_ptr, FLAG_MIMIC));
		}
	}

	return(-1);
}

