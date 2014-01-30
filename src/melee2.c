/* File: melee2.c */

/* Purpose: Monster spells and movement */

/*
* Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
*
* This software may be copied and distributed for educational, research, and
* not for profit purposes provided that this copyright and statement are
* included in all such copies.
*/

/*
* This file has several additions to it by Keldon Jones (keldon@umr.edu)
* to improve the general quality of the AI (version 0.1.1).
*/

#include "angband.h"
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lauxlib.h"
#include "tolua.h"

#define SPEAK_CHANCE 200

#define FOLLOW_DISTANCE 6

#define MAX_MOVEMENTS_TRIES 9

/*
 * Lua state
 */
extern lua_State* L;

/*
 * Based on mon_take_hit... all monster attacks on
 * other monsters should use
 */
bool mon_take_hit_mon(s32b s_idx, s32b m_idx, s32b dam, bool *fear, cptr note)
{
	monster_type *m_ptr = get_monster(m_idx), *s_ptr = NULL;

	monster_race *r_ptr = race_inf(m_ptr);

	s32b div, new_exp, new_exp_frac;

	if (m_ptr->r_idx == 0)
	{
		s32b y = m_ptr->fy;
		s32b x = m_ptr->fx;

		cmsg_format(TERM_L_RED, "ERROR: mon_take_hit_mon()'s m_ptr is "
					"already dead");

		if (in_bounds(y, x))
			cave[y][x].m_idx = 0;

		return FALSE;
	}

	/*
	 * mon_take_hit_mon() can be called when the attacking "monster" is
	 * the dungeon/trap/floor/etc.
	 */
	if (s_idx > 0)
		s_ptr = get_monster(s_idx);

	/* Redraw (later) if needed */
	if (health_who == m_idx) flag_bool(&p_ptr->redraw, FLAG_PR_HEALTH);

	/* Some mosnters are immune to death */
	if (has_flag(m_ptr, FLAG_NO_DEATH)) return FALSE;

	if (process_hooks_ret(HOOK_MONSTER_TAKE_HIT, "d", "(d,d,M,d)", s_idx, m_idx, m_ptr, dam))
		dam = process_hooks_return[0].num;

	if (dam == 0) return FALSE;

	/* Wake it up */
	m_ptr->csleep = 0;

	/* Hurt it */
	m_ptr->hp -= dam;

	/* Notice the player */

	if (wizard && m_ptr->ml) {
		char m_name[80];
		char s_name[80];

		monster_desc(m_name, m_ptr, 0x80);

		if (s_ptr != NULL)
			monster_desc(s_name, s_ptr, 0x80);
		else
		{
			if (s_idx == WHO_TRAP)
				strcpy(s_name, "a trap");
			else if (s_idx == WHO_DUNGEON)
				strcpy(s_name, "the dungeon");
			else if (s_idx == WHO_FLOOR)
				strcpy(s_name, "the floor");
			else
				strcpy(s_name, "unknown");
		}

		msg_format("%s takes hit for #G%d#W damage#w from %s.", m_name, dam,
				   s_name);
	}

	/* It is dead now... or is it? */
	if (m_ptr->hp < 0)
	{
#if 0 // DGDGDGDG Is this really usefull ?
		if ((has_flag(m_ptr, FLAG_UNIQUE) && (m_ptr->status <= MSTATUS_NEUTRAL_P)) ||
		                (m_ptr->mflag & MFLAG_QUEST))
		{
			m_ptr->hp = 1;
		}
		else
#endif
		{
			char m_name[80];

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
				cmonster_msg(TERM_L_RED, "%^s%s", m_name, note);
			}
			/* Death by Physical attack -- invisible monster */
			else if (!m_ptr->ml)
			{
				/* Do nothing */
			}
			/* Death by Physical attack -- special note */
			else if (has_flag(m_ptr, FLAG_NOTE_DIES))
			{
				cmonster_msg(TERM_L_RED, "%^s%s", m_name,
							 flag_get_string(&m_ptr->flags, FLAG_NOTE_DIES));
			}
			/* Death by Physical attack -- special verb */
			else if (has_flag(m_ptr, FLAG_KILLED_VERB))
			{
				cmonster_msg(TERM_L_RED, "%^s is %s.", m_name,
							 flag_get_string(&m_ptr->flags,
											 FLAG_KILLED_VERB));
			}
			/* Death by Physical attack -- normal note/verb */
			else
			{
				cmonster_msg(TERM_L_RED, "%^s is killed.", m_name);
			}

			if (s_idx > 0)
			{
				s32b dive = s_ptr->level;
				if (!dive) dive = 1;
				dive = r_ptr->mexp * m_ptr->level / dive;
				if (!dive) dive = 1;

				/* Monster gains some xp */
				monster_gain_exp(s_idx, dive, FALSE);

				/* Monster lore skill allows gaining xp from pets */
				if (get_skill(SKILL_LORE) &&
					(s_ptr->faction == FACTION_PLAYER))
				{
					/* Maximum player level */
					div = p_ptr->max_plv;

					/* Give some experience for the kill */
					new_exp = ((long)r_ptr->mexp * m_ptr->level) / div;

					/* Handle fractional experience */
					new_exp_frac = ((((long)r_ptr->mexp * m_ptr->level) % div)
									* 0x10000L / div) + p_ptr->exp_frac;

					/* Keep track of experience */
					if (new_exp_frac >= 0x10000L)
					{
						new_exp++;
						p_ptr->exp_frac = new_exp_frac - 0x10000;
					}
					else
					{
						p_ptr->exp_frac = new_exp_frac;
					}

					/*
					 * Factor the xp by the skill level Note that a
					 * score * of 50 in the skill makes the gain be
					 * 120% of the * exp
					 */
					new_exp = new_exp * get_skill_scale(SKILL_LORE, 120) / 100;

					/* Gain experience */
					gain_exp(new_exp);
				}
			} /* if (s_idx > 0) */

			/* When an Unique dies, it stays dead */
			if (has_flag(m_ptr, FLAG_UNIQUE))
			{
				r_ptr->max_num = 0;
			}

			/* Generate treasure */
			monster_death(m_idx, s_idx);

			/* Delete the monster */
			delete_monster_idx(m_idx);

			/* Not afraid */
			(*fear) = FALSE;

			/* Monster is dead */
			return (TRUE);
		}

	}

	/* Not dead yet */
	return (FALSE);
}





/*
* And now for Intelligent monster attacks (including spells).
*
* Original idea and code by "DRS" (David Reeves Sward).
* Major modifications by "BEN" (Ben Harrison).
*
* Give monsters more intelligent attack/spell selection based on
* observations of previous attacks on the player, and/or by allowing
* the monster to "cheat" and know the player status.
*
* Maintain an idea of the player status, and use that information
* to occasionally eliminate "ineffective" spell attacks.  We could
* also eliminate ineffective normal attacks, but there is no reason
* for the monster to do this, since he gains no benefit.
* Note that MINDLESS monsters are not allowed to use this code.
* And non-INTELLIGENT monsters only use it partially effectively.
*
* Actually learn what the player resists, and use that information
* to remove attacks or spells before using them.  This will require
* much less space, if I am not mistaken.  Thus, each monster gets a
* set of 32 bit flags, "smart", build from the various "SM_*" flags.
*
* This has the added advantage that attacks and spells are related.
* The "smart_learn" option means that the monster "learns" the flags
* that should be set, and "smart_cheat" means that he "knows" them.
* So "smart_cheat" means that the "smart" field is always up to date,
* while "smart_learn" means that the "smart" field is slowly learned.
* Both of them have the same effect on the "choose spell" routine.
*/



/*
 * Determine if a bolt spell will hit the player.
 *
 * This is exactly like "projectable", but it will return FALSE if a monster
 * is in the way.
 */
static bool clean_shot(s32b y1, s32b x1, s32b y2, s32b x2)
{
	s32b dist, y, x;

	/* Not too far away */
	if (distance(y1, x1, y2, x2) > MAX_RANGE) return FALSE;;

	/* Start at the initial location */
	y = y1, x = x1;

	/* See "project()" and "projectable()" */
	for (dist = 0; dist <= MAX_RANGE; dist++)
	{
		/* Never pass through walls */
		if (dist && (!cave_sight_bold(y, x) || cave_wall_bold(y, x))) break;

		/* Never pass through monsters */
		if (dist && cave[y][x].m_idx > 0)
		{
			if (is_friend(get_monster(cave[y][x].m_idx)) < 0) break;
		}

		/* Check for arrival at "final target" */
		if ((x == x2) && (y == y2)) return (TRUE);

		/* Calculate the new location */
		mmove2(&y, &x, y1, x1, y2, x2);
	}

	/* Assume obstruction */
	return (FALSE);
}



void monster_msg(cptr fmt, ...)
{
	va_list vp;

	char buf[1024];

	/* Begin the Varargs Stuff */
	va_start(vp, fmt);

	/* Format the args, save the length */
	(void)vstrnfmt(buf, 1024, fmt, vp);

	/* End the Varargs Stuff */
	va_end(vp);

	/* Display */
	if (disturb_other)
		msg_print(buf);
	else
	{
		message_add(MESSAGE_MSG, buf, TERM_WHITE);
		flag_bool(&p_ptr->window, FLAG_PW_MESSAGE);
	}
}

void cmonster_msg(char a, cptr fmt, ...)
{
	va_list vp;

	char buf[1024];

	/* Begin the Varargs Stuff */
	va_start(vp, fmt);

	/* Format the args, save the length */
	(void)vstrnfmt(buf, 1024, fmt, vp);

	/* End the Varargs Stuff */
	va_end(vp);

	/* Display */
	if (disturb_other)
		cmsg_print(a, buf);
	else
	{
		message_add(MESSAGE_MSG, buf, a);
		flag_bool(&p_ptr->window, FLAG_PW_MESSAGE);
	}
}


/*
 * Monster tries to 'cast a spell' (or breath, etc)
 * at another monster.
 */
s32b monst_spell_monst_spell = -1;
static bool monst_spell_monst(s32b m_idx)
{
	s32b chance, rlev;
	u32b thrown_spell;
	monster_type *m_ptr = get_monster(m_idx);
	monster_type *tm_ptr;
	char m_name[80];
	char m_poss[80];
	char ddesc[80];
	bool no_inate = FALSE;
	s32b x, y;

	/* Assume "normal" target */
	bool normal = TRUE;

	/* Target location */
	if (m_ptr->target > -1)
	{
		if (m_ptr->target > 0)
		{
			tm_ptr = get_monster(m_ptr->target);
			y = tm_ptr->fy;
			x = tm_ptr->fx;
		}
		else
		{
			return (FALSE);
		}
	}
	else return FALSE;

	/* Cannot cast spells when confused */
	if (m_ptr->confused) return (FALSE);

	/* Cannot cast spells when nice */
	if (m_ptr->mflag & (MFLAG_NICE)) return (FALSE);
/* No need	if (is_friend(m_ptr) >= 0) return (FALSE); */

	/* Hack -- Extract the spell probability */
	chance = (m_ptr->freq_inate + m_ptr->freq_spell) / 2;

	/* Not allowed to cast spells */
	if (!chance) return (FALSE);

	if (stupid_monsters)
	{
		/* Only do spells occasionally */
		if (rand_int(100) >= chance) return (FALSE);
	}
	else
	{
		if (rand_int(100) >= chance) return (FALSE);

		/* Sometimes forbid inate attacks (breaths) */
		if (rand_int(100) >= (chance * 2)) no_inate = TRUE;
	}

	/* Hack -- require projectable player */
	if (normal)
	{
		/* Check path */
		if (!projectable(m_ptr->fy, m_ptr->fx, y, x)) return (FALSE);
	}

	/* Extract the monster level */
	rlev = ((m_ptr->level >= 1) ? m_ptr->level : 1);

	/* No spells left */
	if (!flag_used(&m_ptr->spells)) return (FALSE);

	/* Stop if player is dead or gone */
	if (!alive || death) return (FALSE);

	/* Stop if player is leaving */
	if (p_ptr->leaving) return (FALSE);

	/* Get the monster name (or "it") */
	monster_desc(m_name, m_ptr, 0x00);

	/* Get the monster possessive ("his"/"her"/"its") */
	monster_desc(m_poss, m_ptr, 0x22);

	/* Hack -- Get the "died from" name */
	monster_desc(ddesc, m_ptr, 0x88);

	/* Choose a spell to cast */
	thrown_spell = flag_get_rand(&m_ptr->spells);

	/* Call upon the GUSS(this function will rewire various functions to make it monster casted */
	call_lua("cast_monster_spell", "(d,d,d,d,d,d)", "", m_idx, thrown_spell, flag_get(&m_ptr->spells, thrown_spell), flag_get2(&m_ptr->spells, thrown_spell), y, x);
	return TRUE;
}


/*
 * Creatures can cast spells, shoot missiles, and breathe.
 *
 * Returns "TRUE" if a spell (or whatever) was (successfully) cast.
 *
 * XXX XXX XXX This function could use some work, but remember to
 * keep it as optimized as possible, while retaining generic code.
 *
 * Verify the various "blind-ness" checks in the code.
 *
 * XXX XXX XXX Note that several effects should really not be "seen"
 * if the player is blind.  See also "effects.c" for other "mistakes".
 *
 * Perhaps monsters should breathe at locations *near* the player,
 * since this would allow them to inflict "partial" damage.
 *
 * Perhaps smart monsters should decline to use "bolt" spells if
 * there is a monster in the way, unless they wish to kill it.
 *
 * Note that, to allow the use of the "track_target" option at some
 * later time, certain non-optimal things are done in the code below,
 * including explicit checks against the "direct" variable, which is
 * currently always true by the time it is checked, but which should
 * really be set according to an explicit "projectable()" test, and
 * the use of generic "x,y" locations instead of the player location,
 * with those values being initialized with the player location.
 *
 * It will not be possible to "correctly" handle the case in which a
 * monster attempts to attack a location which is thought to contain
 * the player, but which in fact is nowhere near the player, since this
 * might induce all sorts of messages about the attack itself, and about
 * the effects of the attack, which the player might or might not be in
 * a position to observe.  Thus, for simplicity, it is probably best to
 * only allow "faulty" attacks by a monster if one of the important grids
 * (probably the initial or final grid) is in fact in view of the player.
 * It may be necessary to actually prevent spell attacks except when the
 * monster actually has line of sight to the player.  Note that a monster
 * could be left in a bizarre situation after the player ducked behind a
 * pillar and then teleported away, for example.
 *
 * Note that certain spell attacks do not use the "project()" function
 * but "simulate" it via the "direct" variable, which is always at least
 * as restrictive as the "project()" function.  This is necessary to
 * prevent "blindness" attacks and such from bending around walls, etc,
 * and to allow the use of the "track_target" option in the future.
 *
 * Note that this function attempts to optimize the use of spells for the
 * cases in which the monster has no spells, or has spells but cannot use
 * them, or has spells but they will have no "useful" effect.  Note that
 * this function has been an efficiency bottleneck in the past.
 *
 * Note the special "MFLAG_NICE" flag, which prevents a monster from using
 * any spell attacks until the player has had a single chance to move.
 */
bool make_attack_spell(s32b m_idx)
{
	s32b chance, rlev;
	u32b thrown_spell;
	monster_type *m_ptr = get_monster(m_idx);
	char m_name[80];
	char m_poss[80];
	char ddesc[80];
	bool no_inate = FALSE;
	s32b x, y;

	/* Assume "normal" target */
	bool normal = TRUE;

	/* Target location */
	if (m_ptr->target > -1)
	{
		if (!m_ptr->target)
		{
			y = p_ptr->py;
			x = p_ptr->px;
		}
		else
		{
			return monst_spell_monst(m_idx);
		}
	}
	else return FALSE;

	/* Cannot cast spells when confused */
	if (m_ptr->confused) return (FALSE);

	/* Cannot cast spells when nice */
	if (m_ptr->mflag & (MFLAG_NICE)) return (FALSE);
	if (is_friend(m_ptr) >= 0) return (FALSE);

	/* Cannot attack the player if mortal and player fated to never die by the ... */
	if (has_flag(m_ptr, FLAG_MORTAL) && (p_ptr->no_mortal)) return (FALSE);

	/* Hack -- Extract the spell probability */
	chance = (m_ptr->freq_inate + m_ptr->freq_spell) / 2;

	/* Not allowed to cast spells */
	if (!chance) return (FALSE);

	if (stupid_monsters)
	{
		/* Only do spells occasionally */
		if (rand_int(100) >= chance) return (FALSE);
	}
	else
	{
		if (rand_int(100) >= chance) return (FALSE);

		/* Sometimes forbid inate attacks (breaths) */
		if (rand_int(100) >= (chance * 2)) no_inate = TRUE;
	}

	/* Hack -- require projectable player */
	if (normal)
	{
		/* Check path */
		if (!projectable(m_ptr->fy, m_ptr->fx, y, x)) return (FALSE);
	}

	/* Extract the monster level */
	rlev = ((m_ptr->level >= 1) ? m_ptr->level : 1);

	/* No spells left */
	if (!flag_used(&m_ptr->spells)) return (FALSE);

	/* Stop if player is dead or gone */
	if (!alive || death) return (FALSE);

	/* Stop if player is leaving */
	if (p_ptr->leaving) return (FALSE);

	/* Get the monster name (or "it") */
	monster_desc(m_name, m_ptr, 0x00);

	/* Get the monster possessive ("his"/"her"/"its") */
	monster_desc(m_poss, m_ptr, 0x22);

	/* Hack -- Get the "died from" name */
	monster_desc(ddesc, m_ptr, 0x88);

	/* Choose a spell to cast */
	thrown_spell = flag_get_rand(&m_ptr->spells);

	/* Call upon the GUSS(this function will rewire various functions to make it monster casted */
	call_lua("cast_monster_spell", "(d,d,d,d,d,d)", "", m_idx, thrown_spell, flag_get(&m_ptr->spells, thrown_spell), flag_get2(&m_ptr->spells, thrown_spell), y, x);
	return TRUE;
}


/*
 * Returns whether a given monster will try to run from the player.
 *
 * Monsters will attempt to avoid very powerful players.  See below.
 *
 * Because this function is called so often, little details are important
 * for efficiency.  Like not using "mod" or "div" when possible.  And
 * attempting to check the conditions in an optimal order.  Note that
 * "(x << 2) == (x * 4)" if "x" has enough bits to hold the result.
 *
 * Note that this function is responsible for about one to five percent
 * of the processor use in normal conditions...
 */
static s32b mon_will_run(s32b m_idx)
{
	monster_type *m_ptr = get_monster(m_idx);

	/* Keep monsters from running too far away */
	if (m_ptr->cdis > MAX_SIGHT + 5) return (FALSE);

	/* Friends don't run away */
	if (is_friend(m_ptr) >= 0) return (FALSE);

	/* All "afraid" monsters will run away */
	if (m_ptr->monfear) return (TRUE);


#if 0 // DGDGDGDG erm life is no more a hardocded thing ..
#ifdef ALLOW_TERROR

	/* Nearby monsters will not become terrified */
	if (m_ptr->cdis <= 5) return (FALSE);

	/* Examine player power (level) */
	p_lev = p_ptr->lev;

	/* Examine monster power (level plus morale) */
	m_lev = m_ptr->level + (m_idx & 0x08) + 25;

	/* Optimize extreme cases below */
	if (m_lev > p_lev + 4) return (FALSE);
	if (m_lev + 4 <= p_lev) return (TRUE);

	/* Examine player health */
	p_chp = p_ptr->chp;
	p_mhp = p_ptr->mhp;

	/* Examine monster health */
	m_chp = m_ptr->hp;
	m_mhp = m_ptr->maxhp;

	/* Prepare to optimize the calculation */
	p_val = (p_lev * p_mhp) + (p_chp << 2);  /* div p_mhp */
	m_val = (m_lev * m_mhp) + (m_chp << 2);  /* div m_mhp */

	/* Strong players scare strong monsters */
	if (p_val * m_mhp > m_val * p_mhp) return (TRUE);

#endif
#endif
	/* Assume no terror */
	return (FALSE);
}


/*
* Choose a "safe" location near a monster for it to run toward.
*
* A location is "safe" if it can be reached quickly and the player
* is not able to fire into it (it isn't a "clean shot").  So, this will
* cause monsters to "duck" behind walls.  Hopefully, monsters will also
* try to run towards corridor openings if they are in a room.
*
* This function may take lots of CPU time if lots of monsters are
* fleeing.
*
* Return TRUE if a safe location is available.
*/
bool find_safety(monster_type *m_ptr, s32b *yp, s32b *xp)
{
	s32b fy = m_ptr->fy;
	s32b fx = m_ptr->fx;

	s32b y, x, d, dis;
	s32b gy = 0, gx = 0, gdis = 0;

	/* Start with adjacent locations, spread further */
	for (d = 1; d < 10; d++)
	{
		/* Check nearby locations */
		for (y = fy - d; y <= fy + d; y++)
		{
			for (x = fx - d; x <= fx + d; x++)
			{
				/* Skip illegal locations */
				if (!in_bounds(y, x)) continue;

				/* Skip locations in a wall */
				if (!cave_floor_bold(y, x)) continue;

				/* Check distance */
				if (distance(y, x, fy, fx) != d) continue;

				/* Check for absence of shot */
				if (!projectable(y, x, p_ptr->py, p_ptr->px))
				{
					/* Calculate distance from player */
					dis = distance(y, x, p_ptr->py, p_ptr->px);

					/* Remember if further than previous */
					if (dis > gdis)
					{
						gy = y;
						gx = x;
						gdis = dis;
					}
				}
			}
		}

		/* Check for success */
		if (gdis > 0)
		{
			/* Good location */
			(*yp) = gy;
			(*xp) = gx;

			/* Found safe place */
			return (TRUE);
		}
	}

	/* No safe place */
	return (FALSE);
}


/*
 * Choose a good hiding place near a monster for it to run toward.
 *
 * Pack monsters will use this to "ambush" the player and lure him out
 * of corridors into open space so they can swarm him.
 *
 * Return TRUE if a good location is available.
 */
bool find_hiding(monster_type *m_ptr, s32b *yp, s32b *xp)
{
	s32b fy = m_ptr->fy;
	s32b fx = m_ptr->fx;

	s32b y, x, d, dis;
	s32b gy = 0, gx = 0, gdis = 999, min;

	/* Closest distance to get */
	min = distance(p_ptr->py, p_ptr->px, fy, fx) * 3 / 4 + 2;

	/* Start with adjacent locations, spread further */
	for (d = 1; d < 10; d++)
	{
		/* Check nearby locations */
		for (y = fy - d; y <= fy + d; y++)
		{
			for (x = fx - d; x <= fx + d; x++)
			{
				/* Skip illegal locations */
				if (!in_bounds(y, x)) continue;

				/* Skip locations in a wall */
				if (!cave_floor_bold(y, x)) continue;

				/* Check distance */
				if (distance(y, x, fy, fx) != d) continue;

				/* Check for hidden, available grid */
				if (!player_can_see_bold(y, x) && clean_shot(fy, fx, y, x))
				{
					/* Calculate distance from player */
					dis = distance(y, x, p_ptr->py, p_ptr->px);

					/* Remember if closer than previous */
					if (dis < gdis && dis >= min)
					{
						gy = y;
						gx = x;
						gdis = dis;
					}
				}
			}
		}

		/* Check for success */
		if (gdis < 999)
		{
			/* Good location */
			(*yp) = fy;
			(*xp) = fx;

			/* Found good place */
			return (TRUE);
		}
	}

	/* No good place */
	return (FALSE);
}


/* Find an appropriate corpse */
void find_corpse(monster_type *m_ptr, s32b *y, s32b *x)
{
#if 0 // DGDGDGDG
	s32b k, last = -1;

	for (k = 0; k < max_o_idx; k++)
	{
		object_type *o_ptr = o_list[k];
		monster_race *rt_ptr, *rt2_ptr;

		if (!o_ptr->k_idx) continue;

		if (o_ptr->tval != TV_CORPSE) continue;
		if ((o_ptr->sval != SV_CORPSE_CORPSE) && (o_ptr->sval != SV_CORPSE_SKELETON)) continue;

		rt_ptr = &r_info[get_flag(o_ptr, FLAG_MONSTER_IDX)];

		/* Cannot incarnate into a higher level monster */
		if (rt_ptr->level > m_ptr->level) continue;

		/* Must be in LOS */
		if (!los(m_ptr->fy, m_ptr->fx, o_ptr->iy, o_ptr->ix)) continue;

		if (last != -1)
		{
			rt2_ptr = &r_info[get_flag(o_list[last], FLAG_MONSTER_IDX)];
			if (rt_ptr->level > rt2_ptr->level) last = k;
			else continue;
		}
		else
		{
			last = k;
		}
	}

	/* Must be ok now */
	if (last != -1)
	{
		*y = o_list[last]->iy;
		*x = o_list[last]->ix;
	}
#endif
}

/*
 * Choose target
 */
static void get_target_monster(s32b m_idx)
{
	monster_type *m_ptr = get_monster(m_idx);
	s32b i, t = -1, d = 9999;

	if (has_flag(m_ptr, FLAG_AI_SPECIAL_TARGET))
	{
		if (process_hooks_ret(HOOK_MONSTER_AI_TARGET, "d", "(d)", m_idx))
		{
			m_ptr->target = process_hooks_return[0].num;
		}
	}

	/* Process the monsters (backwards) */
	for_flags(&monst_list);
	{
		i = __key;
		/* Access the monster */
		monster_type *t_ptr = get_monster(i);
		/* hack should call the function for ego monsters ... but no_target i not meant to be added by ego and it speeds up the code */
		monster_race *rt_ptr = &r_info[t_ptr->r_idx];
		s32b dd;

		/* Ignore "dead" monsters */
		if (!t_ptr->r_idx) continue;

		if (m_idx == i) continue;

		/* Cannot be targeted */
		if (has_flag(rt_ptr, FLAG_NO_TARGET)) continue;

		if (is_enemy(m_ptr, t_ptr) && (los(m_ptr->fy, m_ptr->fx, t_ptr->fy, t_ptr->fx) &&
		                               ((dd = distance(m_ptr->fy, m_ptr->fx, t_ptr->fy, t_ptr->fx)) < d)))
		{
			t = i;
			d = dd;
		}
	}
	end_for_flags();
	/* Hack */
	if ((is_friend(m_ptr) < 0) && los(m_ptr->fy, m_ptr->fx, p_ptr->py, p_ptr->px) && (distance(m_ptr->fy, m_ptr->fx, p_ptr->py, p_ptr->px) < d)) t = 0;

	m_ptr->target = t;
}

/*
 * Uses the stored LOS info in the map to flow toward the last position the payer was seen
 */
static char flow_search_grid[8][2] =
{
	{ -1,  0 },
	{  0, -1 },
	{  1,  0 },
	{  0,  1 },

	{ -1, -1 },
	{  1, -1 },
	{  1,  1 },
	{ -1,  1 },
};
bool flow_to_last_seen_player_position(s32b start_y, s32b start_x, s32b *mm)
{
	s32b i, j, z, a;
	u32b best_turn = cave[start_y][start_x].los_turn;
	u16b best_dist = cave[start_y][start_x].los_dist;
	bool found = FALSE;
	bool done = FALSE;

	mm[0] = 5;
	mm[1] = 5;
	mm[2] = 5;
	mm[3] = 5;
	mm[4] = 5;
	mm[5] = 5;
	mm[6] = 5;
	mm[7] = 5;
	mm[8] = 0;

	/* printf("START at (%d,%d): %ld/%d\n", start_y, start_x, best_turn, best_dist); */
	for (z = 0; z < 8; z++)
	{
		found = FALSE;
		j = flow_search_grid[z][0];
		i = flow_search_grid[z][1];

		/* printf("Analizing (%d,%d)@%d: %ld/%d\n", start_y + j, start_x + i, dddir[j+1][i+1], cave[start_y + j][start_x + i].los_turn, cave[start_y + j][start_x + i].los_dist); */
		for (a = 0; a < MAX_MOVEMENTS_TRIES - 1; a++)
		{
			/* printf("?=? Testing %d=>%d\n", a, mm[a]); */
			if (mm[a])
			{
				s32b test_dir_y = ddy[mm[a]];
				s32b test_dir_x = ddx[mm[a]];
				best_turn = cave[start_y + test_dir_y][start_x + test_dir_x].los_turn;
				best_dist = cave[start_y + test_dir_y][start_x + test_dir_x].los_dist;
				/* printf("Dir %d,%d\n",test_dir_y,test_dir_x); */
				/* printf("Comparing to (%d,%d)@%d: %ld/%d\n", start_y + test_dir_y, start_x + test_dir_x, mm[a], best_turn, best_dist); */

				if (cave[start_y + j][start_x + i].los_turn > best_turn)
				{
					found = TRUE;
					/* printf(" => GOOD by turn!\n"); */
				}
				else if ((!found) && (cave[start_y + j][start_x + i].los_turn == best_turn) && (cave[start_y + j][start_x + i].los_dist < best_dist))
				{
					found = TRUE;
					/* printf(" => GOOD by dist!\n"); */
				}

				if (found)
				{
					s32b b;

					/* printf("Current mm: %d:%d:%d:%d:%d:%d:%d:%d:%d\n", mm[0], mm[1], mm[2], mm[3], mm[4], mm[5], mm[6], mm[7], mm[8]); */

					for (b = 8; b > a; b--)
					{
						mm[b] = mm[b - 1];
					}
					mm[a] = dddir[j + 1][i + 1];

					/* printf("New     mm: %d:%d:%d:%d:%d:%d:%d:%d;%d\n", mm[0], mm[1], mm[2], mm[3], mm[4], mm[5], mm[6], mm[7], mm[8]); */
					done = TRUE;
					break;
				}
			}
/*			else
			{
				mm[a] = dddir[j + 1][i + 1];
				break;
			}*/
		}
	}

	/* Replace all 5 to 0 to get back to a clean state */
	for (a = 0; a < MAX_MOVEMENTS_TRIES; a++)
		mm[a] = (mm[a] == 5) ? 0 : mm[a];

	return done;
}

s32b check_hit2(s32b power, s32b level, s32b ac)
{
	s32b i, k;

	/* Percentile dice */
	k = rand_int(100);

	/* Hack -- Always miss or hit */
	if (k < 10) return (k < 5);

	/* Calculate the "attack quality" */
	i = (power + (level * 3));

	/* Power and Level compete against Armor */
	if ((i > 0) && (randint(i) > ((ac * 3) / 4))) return (TRUE);

	/* Assume miss */
	return (FALSE);
}


/* Monster attacks monster */
static bool monst_attack_monst(s32b m_idx, s32b t_idx)
{
	s32b ret;
	monster_type *t_ptr = get_monster(t_idx);

	call_lua("monster_attack", "(d,d,d)", "d", m_idx, t_ptr->fy, t_ptr->fx, &ret);
	return ret;
}


/*
 * Hack -- local "player stealth" value (see below)
 */
static u32b noise = 0L;

/* Determine whether the player is invisible to a monster */
static bool player_invis(monster_type * m_ptr)
{
	s16b inv, mlv;

	inv = intrinsic(INVIS);

	mlv = (s16b) m_ptr->level;

	mlv += get_flag(m_ptr, FLAG_SEE_INVIS);

	if (has_flag(m_ptr, FLAG_NO_SLEEP))
		mlv += 10;
	if (has_flag(m_ptr, FLAG_STUPID))
		mlv /= 2;
	if (has_flag(m_ptr, FLAG_SMART))
		mlv = (mlv * 5) / 4;
	if (m_ptr->mflag & MFLAG_QUEST)
		inv = 0;
	if (has_flag(m_ptr, FLAG_INVISIBLE))
		inv = 0;
	if (m_ptr->mflag & MFLAG_CONTROL)
		inv = 0;
	if (mlv < 1)
		mlv = 1;
	return (inv >= randint(mlv*2));
}

/*
 * Process a monster
 *
 * The monster is known to be within 100 grids of the player
 *
 * In several cases, we directly update the monster lore
 *
 * Note that a monster is only allowed to "reproduce" if there
 * are a limited number of "reproducing" monsters on the current
 * level.  This should prevent the level from being "swamped" by
 * reproducing monsters.  It also allows a large mass of mice to
 * prevent a louse from multiplying, but this is a small price to
 * pay for a simple multiplication method.
 *
 * XXX Monster fear is slightly odd, in particular, monsters will
 * fixate on opening a door even if they cannot open it.  Actually,
 * the same thing happens to normal monsters when they hit a door
 *
 * XXX XXX XXX In addition, monsters which *cannot* open or bash
 * down a door will still stand there trying to open it...
 *
 * XXX Technically, need to check for monster in the way
 * combined with that monster being in a wall (or door?)
 *
 * A "direction" of "5" means "pick a random direction".
 */
/*
 * Choose "logical" directions for monster movement
 */
static bool get_moves(s32b m_idx, s32b *mm, s32b y2, s32b x2)
{
	monster_type *m_ptr = get_monster(m_idx);

	s32b y, ay, x, ax;

	s32b move_val = 0;

	/* Extract the "pseudo-direction" */
	y = m_ptr->fy - y2;
	x = m_ptr->fx - x2;

	/* Check for no move */
	if (!x && !y) return (FALSE);

	/* Extract the "absolute distances" */
	ax = ABS(x);
	ay = ABS(y);

	/* Do something weird */
	if (y < 0) move_val += 8;
	if (x > 0) move_val += 4;

	/* Prevent the diamond maneuvre */
	if (ay > (ax << 1))
	{
		move_val++;
		move_val++;
	}
	else if (ax > (ay << 1))
	{
		move_val++;
	}

	/* Extract some directions */
	switch (move_val)
	{
	case 0:
		mm[0] = 9;
		if (ay > ax)
		{
			mm[1] = 8;
			mm[2] = 6;
			mm[3] = 7;
			mm[4] = 3;
		}
		else
		{
			mm[1] = 6;
			mm[2] = 8;
			mm[3] = 3;
			mm[4] = 7;
		}
		break;
	case 1:
	case 9:
		mm[0] = 6;
		if (y < 0)
		{
			mm[1] = 3;
			mm[2] = 9;
			mm[3] = 2;
			mm[4] = 8;
		}
		else
		{
			mm[1] = 9;
			mm[2] = 3;
			mm[3] = 8;
			mm[4] = 2;
		}
		break;
	case 2:
	case 6:
		mm[0] = 8;
		if (x < 0)
		{
			mm[1] = 9;
			mm[2] = 7;
			mm[3] = 6;
			mm[4] = 4;
		}
		else
		{
			mm[1] = 7;
			mm[2] = 9;
			mm[3] = 4;
			mm[4] = 6;
		}
		break;
	case 4:
		mm[0] = 7;
		if (ay > ax)
		{
			mm[1] = 8;
			mm[2] = 4;
			mm[3] = 9;
			mm[4] = 1;
		}
		else
		{
			mm[1] = 4;
			mm[2] = 8;
			mm[3] = 1;
			mm[4] = 9;
		}
		break;
	case 5:
	case 13:
		mm[0] = 4;
		if (y < 0)
		{
			mm[1] = 1;
			mm[2] = 7;
			mm[3] = 2;
			mm[4] = 8;
		}
		else
		{
			mm[1] = 7;
			mm[2] = 1;
			mm[3] = 8;
			mm[4] = 2;
		}
		break;
	case 8:
		mm[0] = 3;
		if (ay > ax)
		{
			mm[1] = 2;
			mm[2] = 6;
			mm[3] = 1;
			mm[4] = 9;
		}
		else
		{
			mm[1] = 6;
			mm[2] = 2;
			mm[3] = 9;
			mm[4] = 1;
		}
		break;
	case 10:
	case 14:
		mm[0] = 2;
		if (x < 0)
		{
			mm[1] = 3;
			mm[2] = 1;
			mm[3] = 6;
			mm[4] = 4;
		}
		else
		{
			mm[1] = 1;
			mm[2] = 3;
			mm[3] = 4;
			mm[4] = 6;
		}
		break;
	case 12:
		mm[0] = 1;
		if (ay > ax)
		{
			mm[1] = 2;
			mm[2] = 4;
			mm[3] = 3;
			mm[4] = 7;
		}
		else
		{
			mm[1] = 4;
			mm[2] = 2;
			mm[3] = 7;
			mm[4] = 3;
		}
		break;
	}



	/* Wants to move... */
	return (TRUE);
}


static void process_monster(s32b m_idx, bool is_frien)
{
	/* Remember the tags, speed up a tad little bit */
	static s32b tag_flags = 0;
	static s32b tag_monster = 0;

	monster_type *m_ptr = get_monster(m_idx);
	monster_race *r_ptr = race_inf(m_ptr);
	cave_type    *c_ptr = &cave[m_ptr->fy][m_ptr->fx];
	feature_type *f_ptr;
	vector_type  *to_delete;
	flags_type   *killwall_flags;

	s32b i, d, oy, ox, ny, nx;

	s32b mm[MAX_MOVEMENTS_TRIES];

	monster_type *y_ptr = NULL;

	bool do_turn;
	bool do_move;
	bool do_view;

	bool did_open_door;
	bool did_bash_door;
	bool did_take_item;
	bool did_kill_item;
	bool did_move_body;
	bool did_kill_body;
	bool did_pass_wall;
	u32b pass_wall_how;
	bool did_pass_wall2;
	u32b pass_wall_how2;
	bool did_kill_wall;
	u32b kill_wall_how;
	bool inv;
	bool do_flow = FALSE;
	s32b oldtop;

	flags_type *state;
	s32b target_y, target_x;

	if (!tag_flags) tag_flags = tolua_tag(L, "flags_type");
	if (!tag_monster) tag_monster = tolua_tag(L, "monster_type");

	/* Default speed */
	m_ptr->ai_speed	= SPEED_GLOBAL;

	inv = player_invis(m_ptr);

	/* Handle "fear" */
	if (m_ptr->monfear)
	{
		/* Amount of "boldness" */
		s32b d = randint(m_ptr->level / 10 + 1);

		/* Still afraid */
		if (m_ptr->monfear > d)
		{
			/* Reduce the fear */
			m_ptr->monfear -= d;
		}

		/* Recover from fear, take note if seen */
		else
		{
			/* No longer afraid */
			m_ptr->monfear = 0;

			/* Visual note */
			if (m_ptr->ml)
			{
				char m_name[80];
				char m_poss[80];

				/* Acquire the monster name/poss */
				monster_desc(m_name, m_ptr, 0);
				monster_desc(m_poss, m_ptr, 0x22);

				/* Dump a message */
				msg_format("%^s recovers %s courage.", m_name, m_poss);
			}
		}
	}

	/* Get the origin */
	oy = m_ptr->fy;
	ox = m_ptr->fx;

	/* Need a new target ? */
	if ((m_ptr->target == -1) || ((!m_ptr->los_flow_last_turn) && magik(10)))
	{
		get_target_monster(m_idx);
	}

	/* No AI ? well no moves! */
	if (!m_ptr->ai) return;

	/*
	 * Build the calling code to the lua stuff
	 * do it with the lua API directly to avoid speed overheads
	 */
	oldtop = lua_gettop(L);

	state = &m_ptr->ai_states;

	/* Grab the function to call */
	lua_getglobal(L, "ai");
	lua_pushnumber(L, m_ptr->ai);
	lua_gettable(L, -2);
	lua_pushstring(L, "exec");
	lua_gettable(L, -2);

	/* monster idx, monster struct and current ai state */
	lua_pushnumber(L, m_idx);
	tolua_pushusertype(L, (void*)m_ptr, tag_monster);
	tolua_pushusertype(L, (void*)state, tag_flags);

	/* Call the function */
	if (lua_call(L, 3, 0))
	{
		cmsg_format(TERM_VIOLET, "ERROR in lua_call while calling 'process_monster'.");
		lua_settop(L, oldtop);
		return;
	}

	lua_settop(L, oldtop);

	/* Paranoia -- lua could delete it */
	if (!c_ptr->m_idx) return;

	/* Analyze the results */
	if (m_ptr->ai_action == 1) /* ai.MOVE */
	{
		target_y = m_ptr->ai_move_y;
		target_x = m_ptr->ai_move_x;
	}
	else if (m_ptr->ai_action == 2) /* ai.MOVE_DIR */
	{
		target_y = m_ptr->ai_move_y - m_ptr->fy;
		target_x = m_ptr->ai_move_x - m_ptr->fx;
	}
	else
		return;

	/* Hack -- Assume no movement */
	mm[0] = mm[1] = mm[2] = mm[3] = 0;
	mm[4] = mm[5] = mm[6] = mm[7] = 0;

	/* Confused -- 100% random */
	if (m_ptr->confused || (inv == TRUE && m_ptr->target == 0))
	{
		/* Try four "random" directions */
		mm[0] = mm[1] = mm[2] = mm[3] = 5;
	}

	/* random movement */
	else if ((has_flag(m_ptr, FLAG_RAND_MOVEMENT)) &&
	                (rand_int(100) < get_flag(m_ptr, FLAG_RAND_MOVEMENT)))
	{
		/* Try four "random" directions */
		mm[0] = mm[1] = mm[2] = mm[3] = 5;
	}

	/* Normal movement */
	else
	{
		get_moves(m_idx, mm, target_y, target_x);
	}
	m_ptr->los_flow_last_turn = do_flow;

	/* Assume nothing */
	do_turn     = FALSE;
	do_move     = FALSE;
	do_view     = FALSE;


	/* Assume nothing */
	did_open_door  = FALSE;
	did_bash_door  = FALSE;
	did_take_item  = FALSE;
	did_kill_item  = FALSE;
	did_move_body  = FALSE;
	did_kill_body  = FALSE;
	did_pass_wall  = FALSE;
	did_pass_wall2 = FALSE;
	did_kill_wall  = FALSE;

	killwall_flags = flag_get_flags(&m_ptr->flags, FLAG_KILL_WALL);

	/* Take a zero-terminated array of "directions" */
	for (i = 0; mm[i]; i++)
	{
		bool do_attack    = FALSE;
		bool do_killwall  = FALSE;
		bool do_kill_body = FALSE;

		monster_race *z_ptr = NULL;

		/* Get the direction */
		d = mm[i];

		/* Hack -- allow "randomized" motion */
		if (d == 5) d = ddd[rand_int(8)];

		/* Get the destination */
		ny = oy + ddy[d];
		nx = ox + ddx[d];

		/* Access that cave grid */
		c_ptr = &cave[ny][nx];
		f_ptr = &f_info[c_ptr->feat];

		/* Access that cave grid's contents */
		if (c_ptr->m_idx)
		{
			y_ptr = get_monster(c_ptr->m_idx);

			if (!y_ptr->r_idx)
				y_ptr = NULL;
			else
				z_ptr = race_inf(y_ptr);
		}
		else
		{
			y_ptr = NULL;
			z_ptr = NULL;
		}

		/* Hack -- player 'in' wall */
		if ((ny == p_ptr->py) && (nx == p_ptr->px))
		{
			s32b friendly;
			friendly = get_faction_friendliness(FACTION_PLAYER,
												m_ptr->faction);

			if (!has_flag(m_ptr, FLAG_NEVER_BLOW) &&
				(m_ptr->target == 0 || friendly < 0))
				do_attack = TRUE;

			do_move = FALSE;
		}

		else if (y_ptr)
		{
			/* Possibly a monster to attack */
			if (!has_flag(m_ptr, FLAG_NEVER_BLOW) &&
				(m_ptr->target == c_ptr->m_idx || is_enemy(m_ptr, y_ptr)))
				do_attack = TRUE;

			/*
			 * Monster might want to do KILL_BODY (which moves onto the
			 * square the victim was on), or might want to try to
			 * push past the other monster if it can't attack.
			 */
			if (!has_flag(m_ptr, FLAG_NEVER_MOVE))
				do_move = TRUE;
		}

		/* Some monsters never move. */
		else if (has_flag(m_ptr, FLAG_NEVER_MOVE))
		{
			do_move = FALSE;
		}

		/* Floor is open and passable? */
		else if (cave_floor_bold(ny, nx) &&
				 monster_can_cross_terrain(c_ptr->feat, &m_ptr->flags))
		{
			/* Go ahead and move */
			do_move = TRUE;
		}

		/* Some monsters can fly over or pass through terrain */
		else if (monst_can_pass_square(m_ptr, ny, nx, &pass_wall_how))
		{
			/* Pass through or fly over walls/doors/rubble */
			do_move = TRUE;

			/* Only learn if player knows what the monster passed over. */
			if (c_ptr->info & CAVE_MARK)
				did_pass_wall = TRUE;
		}
		/* Monster destroys walls (and doors) */
		else if (killwall_flags && !has_flag(f_ptr, FLAG_PERMANENT) &&
				 flag_intersects_where(killwall_flags, &f_ptr->flags,
									   &kill_wall_how))
		{
			do_killwall = TRUE;
			do_move     = TRUE;
		}

		/* Handle doors and secret doors */
		else if (has_flag(f_ptr, FLAG_DOOR) && has_flag(f_ptr, FLAG_CLOSED))
		{
			bool may_bash = TRUE;

			/* Take a turn */
			do_turn = TRUE;
			if (has_flag(m_ptr, FLAG_OPEN_DOOR) &&
				 has_flag(f_ptr, FLAG_OPEN_DOOR))
			{
				/* Closed doors and secret doors */
				if (!has_flag(f_ptr, FLAG_LOCKED))
				{
					/* The door is open */
					did_open_door = TRUE;

					/* Do not bash the door */
					may_bash = FALSE;
				}

				/* Locked doors (not jammed) */
				else
				{
					/* Door power */
					s32b k = get_flag(f_ptr, FLAG_LOCKED);

					/* Try to unlock it XXX XXX XXX */
					if (rand_int(m_ptr->hp / 10) > k)
					{
						/* Unlock the door */
						did_open_door = TRUE;

						/* Do not bash the door */
						may_bash = FALSE;
					}
				}
			}

			/* Stuck doors -- attempt to bash them down if allowed */
			if (may_bash && has_flag(m_ptr, FLAG_BASH_DOOR) &&
				has_flag(f_ptr, FLAG_BASH_DOOR))
			{
				/* Door power */
				s32b k = get_flag(f_ptr, FLAG_BASH_DOOR);

				/* Attempt to Bash XXX XXX XXX */
				if (rand_int(m_ptr->hp / 10) > k)
				{
					/* Message */
					if (player_can_see_bold(ny, nx) && !intrinsic(BLIND))
						msg_format("You see the %s burst open!",
								  f_ptr->name);
					else
						msg_print("You hear a door burst open!");

					/* Disturb (sometimes) */
					if (disturb_minor) disturb(0, 0);

					/* The door was bashed open */
					did_bash_door = TRUE;
					did_open_door = FALSE;

					/* Hack -- fall into doorway */
					do_move = TRUE;
				}
				else if (player_can_see_bold(ny, nx) && !intrinsic(BLIND))
					msg_format("You see the %s shake!",
							   f_ptr->name);
			}

			/* Deal with doors in the way */
			if (did_open_door || did_bash_door)
			{
				/* It's no longer hidden */
				cave[ny][nx].mimic = 0;

				/* Break down the door */
				if (did_bash_door && (rand_int(100) < 50))
				{
					s32b feat = get_flag(f_ptr, FLAG_DESTROY_INTO);

					if (feat <= 0)
						feat = FEAT_BROKEN;

					if (feat > 0)
						cave_set_feat(ny, nx, feat);
					else
						place_floor(ny, nx);
				}

				/* Open the door */
				else
				{
					s32b feat = get_flag(f_ptr, FLAG_OPEN);

					if (get_flag(f_ptr, FLAG_OPEN_DOOR) > 1)
						feat = get_flag(f_ptr, FLAG_OPEN_DOOR);

					if (feat <= 0)
						feat = FEAT_OPEN;
					cave_set_feat(ny, nx, feat);
				}

				/* Handle viewable doors */
				if (player_has_los_bold(ny, nx)) do_view = TRUE;
			}
		}

		if (do_move && has_flag(f_ptr, FLAG_MONST_CAN_ENTER)) {
			s32b domain = flag_get( &f_ptr->flags, FLAG_MONST_CAN_ENTER);
			s32b ref    = flag_get2(&f_ptr->flags, FLAG_MONST_CAN_ENTER);

			open_lua_functions_registry(domain, ref);
			call_lua(NULL, "(d, d, b, M)", "b", ny, nx, do_attack, m_ptr,
					 &do_move);
			close_lua_functions_registry();
		}

		if (do_move && y_ptr && has_flag(m_ptr, FLAG_KILL_BODY) &&
			(r_ptr->mexp > z_ptr->mexp) &&
			!has_flag(m_ptr, FLAG_NEVER_BLOW) &&
			/* Friends don't kill friends... */
			!((is_friend(y_ptr) > 0) && (is_friend(y_ptr) > 0)) &&
			/* Don't punish summoners for relying on their friends */
			(is_friend(y_ptr) <= 0) &&
			monst_can_pass_square(m_ptr, ny, nx, &pass_wall_how))
		{
			do_attack    = TRUE;
			do_kill_body = TRUE;
		}

		if (do_attack && has_flag(f_ptr, FLAG_MONST_CAN_ATTACK)) {
			s32b domain = flag_get( &f_ptr->flags, FLAG_MONST_CAN_ATTACK);
			s32b ref    = flag_get2(&f_ptr->flags, FLAG_MONST_CAN_ATTACK);

			open_lua_functions_registry(domain, ref);
			call_lua(NULL, "(d, d, b, M)", "b", ny, nx, do_move, m_ptr,
					 &do_attack);
			close_lua_functions_registry();

			if (!do_attack)
				do_kill_body = FALSE;
		}

		/* The player is in the way.  Attack him. */
		if (do_attack && (ny == p_ptr->py) && (nx == p_ptr->px))
		{
			/* Do the attack */
			m_ptr->ai_speed = SPEED_WEAPON;
			if (make_attack_normal(m_idx, 1)) return;

			/* Do not move */
			do_move = FALSE;

			/* Took a turn */
			do_turn = TRUE;
		}

		/* A monster is in the way */
		else if ((do_attack || do_move) && y_ptr)
		{
			monster_type *m2_ptr = y_ptr;

			/* Can destroy monster bodies. */
			if (do_kill_body)
			{
				/* Monster ate another monster */
				if (m_ptr->ml && m2_ptr->ml)
				{
					char name1[80], name2[80];
					did_kill_body = TRUE;

					monster_desc(name1, m_ptr, 0);
					monster_desc(name2, m2_ptr, 0);
					monster_msg("%^s destroys %s!", name1, name2);
				}
				else if (m2_ptr->ml) {
					char name[80];

					monster_desc(name, m2_ptr, 0);
					monster_msg("%^s is destroyed by something!", name);
				}


				/* Kill the monster */
				delete_monster(ny, nx);

				/* Hack -- get the empty monster */
				y_ptr = get_monster(c_ptr->m_idx);

				/* Did player see us pass through a wall? */
				if (!cave_floor_bold(ny, nx) &&	(c_ptr->info & CAVE_MARK))
					did_pass_wall = TRUE;
			} /* if (do_kill_body) */

			/* Attack 'enemies' */
			else if (do_attack)
			{
				do_move = FALSE;
				/* attack */
				if (m2_ptr->r_idx && (m2_ptr->hp >= 0))
				{
					m_ptr->ai_speed = SPEED_WEAPON;

					hack_message_pain_may_silent = TRUE;
					if (monst_attack_monst(m_idx, c_ptr->m_idx))
					{
						hack_message_pain_may_silent = FALSE;
						return;
					}
					hack_message_pain_may_silent = FALSE;
				}
			}
			/* Push past weaker monsters. */
			else if (do_move && has_flag(m_ptr, FLAG_MOVE_BODY) &&
					 (r_ptr->mexp > z_ptr->mexp) && (!has_flag(z_ptr, FLAG_NO_PUSHBACK)) &&
					 monst_can_pass_square(m_ptr, ny, nx, &pass_wall_how) &&
					 monst_can_pass_square(m2_ptr, m_ptr->fy, m_ptr->fx,
										   &pass_wall_how2))
			{
				cave_type    *c2_ptr = &cave[m_ptr->fy][m_ptr->fx];
				feature_type *f2_ptr = &f_info[c2_ptr->feat];

				do_attack = FALSE;

				/*
				 * Check if pushed monster can enter the square just
				 * vacated by the pusher.
				 */
				if (has_flag(f2_ptr, FLAG_MONST_CAN_ENTER)) {
					s32b domain = flag_get(&f2_ptr->flags,
										   FLAG_MONST_CAN_ENTER);
					s32b ref    = flag_get2(&f2_ptr->flags,
											FLAG_MONST_CAN_ENTER);

					open_lua_functions_registry(domain, ref);
					call_lua(NULL, "(d, d, b, M)", "b",
							 m_ptr->fy, m_ptr->fx, FALSE, m2_ptr,
							 &do_move);
					close_lua_functions_registry();
				}

				if (do_move) {
					/* Monster pushed past another monster */

					/* Did player see us pass through a wall? */
					if (!cave_floor_bold(ny, nx) && (c_ptr->info & CAVE_MARK))
						did_pass_wall = TRUE;
					if (!cave_floor_bold(m_ptr->fy, m_ptr->fx) &&
						(c2_ptr->info & CAVE_MARK))
						did_pass_wall2 = TRUE;

					if (m_ptr->ml && m2_ptr->ml)
					{
						char name1[80], name2[80];
						did_move_body = TRUE;

						monster_desc(name1, m_ptr, 0);
						monster_desc(name2, m2_ptr, 0);
						monster_msg("%^s pushes past %s.", name1, name2);
					}
					else if (m2_ptr->ml) {
						char name[80];

						monster_desc(name, m2_ptr, 0);
						monster_msg("%^s is displaced by something.", name);
					}
				} /* if (do_move) */
			} /* push past weaker monster? */
			else
			{
				do_move   = FALSE;
				do_attack = FALSE;
			}
		}
		else if (do_attack)
		{
			cmsg_format(TERM_L_RED, "ERROR: do_attack set but nothing "
						"there to attack.");
		}
		/* Only killwall if there is no monster or player in it. */
		else if (do_move && do_killwall)
		{
			/*
			 * Killing a wall *AND* walking onto the square where it
			 * used to be takes more energy than just walking.
			 */
			m_ptr->energy -= get_monster_energy(m_ptr, SPEED_DIG);

			/*
			 * Monster destroyed a wall, but only learn if player knows
			 * what used to be there.  Set now, since c_ptr->info might
			 * be changed later.
			 */
			if (c_ptr->info & CAVE_MARK)
				did_kill_wall = TRUE;

			if (mon_killwall(ny, nx, m_idx) == KILLWALL_DONE)
			{
				do_move = TRUE;

				/* Note changes to viewable region */
				if (player_has_los_bold(ny, nx))
					do_view = TRUE;
			}
			else
				/* Didn't kill it after all. */
				did_kill_wall = FALSE;
		}

		/* Creature has been allowed move. */
		if (do_move)
		{
			/* Take a turn */
			do_turn = TRUE;
			m_ptr->ai_speed = SPEED_WALK;

			/* Hack -- Update the old location */
			cave[oy][ox].m_idx = c_ptr->m_idx;

			/* Mega-Hack -- move the old monster, if any */
			if (c_ptr->m_idx)
			{
				monster_type *t = get_monster(c_ptr->m_idx);

				/* Move the old monster */
				if (y_ptr)
				{
					y_ptr->fy = oy;
					y_ptr->fx = ox;
				}

				/* Update the old monster */
				update_mon(c_ptr->m_idx, TRUE);

				/* Wake up the moved monster */
				t->csleep = 0;

				/*
				 * Update monster light -- I'm too lazy to check flags
				 * here, and those ego monster_race functions aren't
				 * re-entrant XXX XXX XXX
				 */
				p_ptr->update |= (PU_MON_LITE);
			}

			/* Hack -- Update the new location */
			c_ptr->m_idx = m_idx;

			/* Move the monster */
			m_ptr->fy = ny;
			m_ptr->fx = nx;

			/* Update the monster */
			update_mon(m_idx, TRUE);

			/* Redraw the old grid */
			lite_spot(oy, ox);

			/* Redraw the new grid */
			lite_spot(ny, nx);

			/* Update LOS info if needed */
			if (do_flow)
			{
//				cave[oy][ox].los_dist = 9999;
//				cave[oy][ox].los_turn = cave[ny][nx].los_turn - 1;
			}

			/* Possible disturb */
			if (m_ptr->ml && (disturb_move ||
			                  ((m_ptr->mflag & (MFLAG_VIEW)) &&
			                   disturb_near)))
			{
				/* Disturb */
				if ((is_friend(m_ptr) < 0) || disturb_pets)
					disturb(0, 0);
			}

			/* Check for monster trap */
			if (mon_hit_trap(m_idx, ny, nx)) return;

			/* Scan all objects in the grid */
			to_delete = vector_new();
			for_inventory_slot(&c_ptr->inventory, o_ptr);
			{
				/* Skip gold */
				if (has_flag(o_ptr, FLAG_GOLD_VALUE)) continue;

				/* Take or Kill objects on the floor */
				/* rr9: Pets will no longer pick up/destroy items */
				if ((((has_flag(m_ptr, FLAG_TAKE_ITEM)) &&
					((is_friend(m_ptr) <= 0) || p_ptr->pet_pickup_items)) ||
					(has_flag(m_ptr, FLAG_KILL_ITEM))) &&
					(is_friend(m_ptr) <= 0))
				{
					char m_name[80];
					char o_name[80];

					/* Acquire the object name */
					object_desc(o_name, o_ptr, TRUE, 3);

					/* Acquire the monster name */
					monster_desc(m_name, m_ptr, 0x04);
#if 0 // DGDGDGDG: mhh ..
					/* React to objects that hurt the monster */
					if (f5 & (TR5_KILL_DEMON)) flg3 |= (RF3_DEMON);
					if (f5 & (TR5_KILL_UNDEAD)) flg3 |= (RF3_UNDEAD);
					if (f1 & (TR1_SLAY_DRAGON)) flg3 |= (RF3_DRAGON);
					if (f1 & (TR1_SLAY_TROLL)) flg3 |= (RF3_TROLL);
					if (f1 & (TR1_SLAY_GIANT)) flg3 |= (RF3_GIANT);
					if (f1 & (TR1_SLAY_ORC)) flg3 |= (RF3_ORC);
					if (f1 & (TR1_SLAY_DEMON)) flg3 |= (RF3_DEMON);
					if (f1 & (TR1_SLAY_UNDEAD)) flg3 |= (RF3_UNDEAD);
					if (f1 & (TR1_SLAY_ANIMAL)) flg3 |= (RF3_ANIMAL);
					if (f1 & (TR1_SLAY_EVIL)) flg3 |= (RF3_EVIL);
#endif
					/* The object cannot be picked up by the monster */
					if (artifact_p(o_ptr))
					{
						/* Only give a message for "take_item" */
						if (has_flag(m_ptr, FLAG_TAKE_ITEM))
						{
							/* Take note */
							did_take_item = TRUE;

							/* Describe observable situations */
							if (m_ptr->ml && player_has_los_bold(ny, nx))
							{
								/* Dump a message */
								msg_format("%^s tries to pick up %s, but fails.",
									m_name, o_name);
							}
						}
					}

					/* Pick up the item */
					else if (has_flag(m_ptr, FLAG_TAKE_ITEM))
					{
						s32b this_o_idx;

						if (monster_carry_okay(m_ptr,o_ptr))
						{
							/* Take note */
							did_take_item = TRUE;

							/* Describe observable situations */
							if (player_has_los_bold(ny, nx))
							{
								/* Dump a message */
								msg_format("%^s picks up %s.", m_name, o_name);
							}

							/* Get an object index */
							call_lua("generate_item_floor_index", "(d,d,d)", "d",
								 ny, nx,
								 item_nb,
								 &this_o_idx);

							/* Delete the object */
							vector_push(to_delete, this_o_idx);
						}
					}

					/* Destroy the item */
					else
					{
						s32b this_o_idx;

						/* Get an object index */
						call_lua("generate_item_floor_index", "(d,d,d)", "d",
							 ny, nx,
							 item_nb,
							 &this_o_idx);

						/* Take note */
						did_kill_item = TRUE;

						/* Describe observable situations */
						if (player_has_los_bold(ny, nx))
						{
							/* Dump a message */
							msg_format("%^s crushes %s.", m_name, o_name);
						}

						/* Delete the object */
						vector_push(to_delete, this_o_idx);
					}
				}
			}
			end_inventory_slot();

			/*
			 * Now delete all marked items, in FILO(First In Last Out) way to ensure
			 * integrity checking
			 */
			while (!vector_is_empty(to_delete))
			{
				s32b item = vector_pop(to_delete);
				object_type *o_ptr;

				if (has_flag(m_ptr, FLAG_TAKE_ITEM))
				{
					o_ptr = object_dup(get_object(item));

					/* Forget mark */
					o_ptr->marked = FALSE;

					/* Give to monster */
					monster_carry(m_ptr, m_idx, o_ptr);
				}

				/* Now delete it */
				item_increase(item, -255);
				item_optimize(item);
			}

			vector_free(to_delete);

			/* Update monster light */
			if (has_flag(m_ptr, FLAG_HAS_LITE)) p_ptr->update |= (PU_MON_LITE);
		}

		/* Stop when done */
		if (do_turn) break;
	}

	/* Notice changes in view */
	if (do_view)
	{
		/* Update some things */
		p_ptr->update |= (PU_VIEW | PU_FLOW | PU_MONSTERS | PU_MON_LITE);
	}


	/* Learn things from observable monster */
	if (m_ptr->ml)
	{
		/* Monster opened a door */
		if (did_open_door)
			monstmem_add(RT_MISC, FLAG_OPEN_DOOR, m_ptr, 0);

		/* Monster bashed a door */
		if (did_bash_door)
			monstmem_add(RT_MISC, FLAG_BASH_DOOR, m_ptr, 0);

		/* Monster tried to pick something up */
		if (did_take_item)
			monstmem_add(RT_MISC, FLAG_TAKE_ITEM, m_ptr, 0);

		/* Monster tried to crush something */
		if (did_kill_item)
			monstmem_add(RT_MISC, FLAG_KILL_ITEM, m_ptr, 0);

		/* Monster pushed past another monster */
		if (did_move_body)
			monstmem_add(RT_MISC, FLAG_MOVE_BODY, m_ptr, 0);

		/* Monster ate another monster */
		if (did_kill_body)
			monstmem_add(RT_MISC, FLAG_KILL_BODY, m_ptr, 0);

		/* Monster destroyed a wall. */
		if (did_kill_wall)
			monstmem_add2(RT_MISC, FLAG_KILL_WALL, kill_wall_how, m_ptr, 0);

		/*
		 * Monster passed through a wall.  Only learn if player knows what
		 * the wall is.
		 */
		if (did_pass_wall && pass_wall_how != FLAG_CAN_FLY)
			monstmem_add2(RT_MISC, FLAG_PASS_WALL, pass_wall_how, m_ptr, 0);
	}

	if (y_ptr && y_ptr->r_idx && y_ptr->ml)
	{
		/*
		 * Monster passed through a wall.  Only learn if player knows what
		 * the wall is.
		 */
		if (did_pass_wall2 && pass_wall_how2 != FLAG_CAN_FLY)
			monstmem_add2(RT_MISC, FLAG_PASS_WALL, pass_wall_how2, y_ptr, 0);
	}
} /* process_monster() */


void summon_maint(s32b m_idx)
{
#if 0 // DGDGDGDG -- mana is no more hardcoded
	monster_type *m_ptr = get_monster(m_idx);

	/* Can you pay? */
	if ((p_ptr->maintain_sum / 100) > p_ptr->csp)
	{
		char m_name[80];

		monster_desc(m_name, m_ptr, 0);

		msg_format("You lose control of %s.", m_name);

		/* Well, then, I guess I'm dead. */
		delete_monster_idx(m_idx);
	}
	else
	{
		s32b s = get_skill(SKILL_SUMMON);

		s = (!s) ? 1 : s;
		/* Well, then I'll take my wages from you. */
		p_ptr->maintain_sum += (m_ptr->level * 100 / s);
	}
#endif
	return;
}


/*
 * Process all the "live" monsters, once per game turn.
 *
 * During each game turn, we scan through the list of all the "live" monsters,
 * (backwards, so we can excise any "freshly dead" monsters), energizing each
 * monster, and allowing fully energized monsters to move, attack, pass, etc.
 *
 * Note that monsters can never move in the monster array (except when the
 * "compact_monsters()" function is called by "dungeon()" or "save_player()").
 *
 * This function is responsible for at least half of the processor time
 * on a normal system with a "normal" amount of monsters and a player doing
 * normal things.
 *
 * When the player is resting, virtually 90% of the processor time is spent
 * in this function, and its children, "process_monster()" and "make_move()".
 *
 * Most of the rest of the time is spent in "update_view()" and "lite_spot()",
 * especially when the player is running.
 *
 * Note the special "MFLAG_BORN" flag, which allows us to ignore "fresh"
 * monsters while they are still being "born".  A monster is "fresh" only
 * during the turn in which it is created, and we use the "hack_m_idx" to
 * determine if the monster is yet to be processed during the current turn.
 *
 * Note the special "MFLAG_NICE" flag, which allows the player to get one
 * move before any "nasty" monsters get to use their spell attacks.
 *
 * Note that when the "knowledge" about the currently tracked monster
 * changes (flags, attacks, spells), we induce a redraw of the monster
 * recall window.
 */
void process_monsters(void)
{
	s32b i;
	s32b fx, fy;

	bool test;
	bool is_frien = FALSE;

	monster_type *m_ptr;
	monster_race *r_ptr;

	s32b old_monster_race_idx;
	s32b old_monster_ego_idx;

	flags_type old_r_flags;

	byte old_r_blows0 = 0;
	byte old_r_blows1 = 0;
	byte old_r_blows2 = 0;
	byte old_r_blows3 = 0;

	byte old_r_cast_inate = 0;
	byte old_r_cast_spell = 0;

	/* Memorize old race */
	old_monster_race_idx = monster_race_idx;
	old_monster_ego_idx = monster_ego_idx;
	p_ptr->maintain_sum = 0;

	/* Acquire knowledge */
	old_r_flags.node = NULL;
	old_r_flags.size = 0;
	if (monster_race_idx)
	{
		/* Acquire current monster */
		r_ptr = &r_info[monster_race_idx];

		/* Memorize flags */
		flag_init(&old_r_flags, 2);
		flag_copy(&old_r_flags, &r_ptr->r_flags);

		/* Memorize blows */
		old_r_blows0 = r_ptr->r_blows[0];
		old_r_blows1 = r_ptr->r_blows[1];
		old_r_blows2 = r_ptr->r_blows[2];
		old_r_blows3 = r_ptr->r_blows[3];

		/* Memorize castings */
		old_r_cast_inate = r_ptr->r_cast_inate;
		old_r_cast_spell = r_ptr->r_cast_spell;
	}


	/* Hack -- calculate the "player noise" */
	noise = (1L << (30 - p_ptr->skill_stl));


	/* Process the monsters (backwards) */
	for_flags(&monst_list);
	{
		i = __key;
		/* Access the monster */
		m_ptr = get_monster(i);

		/* Handle "leaving" */
		if (p_ptr->leaving) break;

		/* Ignore "dead" monsters */
		if (!m_ptr || !m_ptr->r_idx) continue;

		/* Reset placed monsters array to empty. */
		placed_monsters_num = 0;
		placed_monster_id  = 0;

		/* Handle "fresh" monsters */
		if (m_ptr->mflag & (MFLAG_BORN))
		{
			/* No longer "fresh" */
			m_ptr->mflag &= ~(MFLAG_BORN);

			/* Skip */
			continue;
		}

		/* Give this monster some energy */
		m_ptr->energy += TICK_ENERGY;

		/* Not enough energy to move */
		if (m_ptr->energy < 1000) continue;

		/* Access the race */
		r_ptr = race_inf(m_ptr);

		/* Access the location */
		fx = m_ptr->fx;
		fy = m_ptr->fy;


		/* Assume no move */
		test = FALSE;

		/* Control monster aint affected by distance */
		if (p_ptr->control == i)
		{
			test = TRUE;
		}

		/* Handle "sensing radius" */
		else if (m_ptr->cdis <= r_ptr->aaf)
		{
			/* We can "sense" the player */
			test = TRUE;
		}

		/* Handle "sight" and "aggravation" */
		else if ((m_ptr->cdis <= MAX_SIGHT) &&
		                (player_has_los_bold(fy, fx) ||
		                 intrinsic(AGGRAVATE)))
		{
			/* We can "see" or "feel" the player */
			test = TRUE;
		}
		test = TRUE;

		/* Running away wont save them ! */
		if (m_ptr->poisoned || m_ptr->bleeding) test = TRUE;

		/* Do nothing */
		if (!test)
		{
			/* Use up "some" energy */
			m_ptr->energy -= get_monster_energy(m_ptr, SPEED_GLOBAL);
			continue;
		}

		/* Save global index */
		hack_m_idx = i;

		if (is_friend(m_ptr) > 0) is_frien = TRUE;

		if (m_ptr->mflag & MFLAG_PARTIAL) summon_maint(i);

		/* Process the monster */
		process_monster(i, is_frien);

		/* Did monster die/disappear during processing? */
		if (!get_monster(i))
			continue;

		/* Use up "some" energy */
		m_ptr->energy -= get_monster_energy(m_ptr, m_ptr->ai_speed);

		/* Hack -- notice death or departure */
		if (!alive || death) break;

		/* Notice leaving */
		if (p_ptr->leaving) break;
	}
	end_for_flags();

	/* Reset global index */
	hack_m_idx = 0;


	/* Tracking a monster race (the same one we were before) */
	if (monster_race_idx && (monster_race_idx == old_monster_race_idx))
	{
		/* Acquire monster race */
		r_ptr = &r_info[monster_race_idx];

		/* Check for knowledge change */
		if (flag_equal(&old_r_flags, &r_ptr->r_flags) ||
		                (old_r_blows0 != r_ptr->r_blows[0]) ||
		                (old_r_blows1 != r_ptr->r_blows[1]) ||
		                (old_r_blows2 != r_ptr->r_blows[2]) ||
		                (old_r_blows3 != r_ptr->r_blows[3]) ||
		                (old_r_cast_inate != r_ptr->r_cast_inate) ||
		                (old_r_cast_spell != r_ptr->r_cast_spell))
		{
			/* Window stuff */
			flag_bool(&p_ptr->window, FLAG_PW_MONSTER);
		}
	}

	if (monster_race_idx)
		flag_free(&old_r_flags, FALSE);
}

bool can_pass_aux(flags_type *dest_flags, flags_type *passer_flags,
				  u32b *how)
{
	flags_type   *can_pass;
	flags_type   *pass_wall;

	s32b  i;
	u32b fake;

	if (how == NULL)
		how = &fake;

	*how = 0;

	if (flag_exists(dest_flags, FLAG_FLOOR) &&
		!flag_exists(dest_flags, FLAG_NO_WALK))
		return TRUE;

	if (flag_exists(dest_flags, FLAG_CAN_FLY))
		if (flag_get(dest_flags, FLAG_CAN_FLY) <=
			flag_get(passer_flags, FLAG_CAN_FLY))
		{
			*how = FLAG_CAN_FLY;
			return TRUE;
		}

	if (!flag_exists(dest_flags, FLAG_CAN_PASS) ||
		!flag_exists(passer_flags, FLAG_PASS_WALL))
		return FALSE;

	can_pass  = flag_get_flags(dest_flags, FLAG_CAN_PASS);
	pass_wall = flag_get_flags(passer_flags, FLAG_PASS_WALL);

	if (!can_pass) return FALSE;
	if (!pass_wall) return FALSE;

	/*
	 * If any of the flags in pass_wall are present in can_pass, and
	 * is of a sufficient value, the monster/player can pass.  Different
	 * terrains can have different "power" levels for their can_pass,
	 * like forest with denser woods being harder to pass through.
	 */
	for (i = 0; i < pass_wall->size; i++)
	{
		flag_node*  n = &pass_wall->node[i];

		u32b key;
		s32b val;

		if (!(n->flags & FLAG_FLAG_USED)) continue;

		key = n->key;
		val = n->vals.vals.val;

		if (flag_exists(can_pass, key) && flag_get(can_pass, key) <= val)
		{
			*how = key;
			return TRUE;
		}
	}

	return FALSE;
} /* can_pass_aux() */

bool monst_can_pass_square(monster_type *m_ptr, s32b y, s32b x, u32b *how)
{
	cave_type    *c_ptr = &cave[y][x];
	feature_type *f_ptr = &f_info[c_ptr->feat];

	if (!monster_can_cross_terrain(c_ptr->feat, &m_ptr->flags))
		return FALSE;

	return can_pass_aux(&f_ptr->flags, &m_ptr->flags, how);
} /* monst_can_pass_square() */

bool monst_can_enter_square(monster_type *m_ptr, s32b y, s32b x, u32b *how)
{
	cave_type    *c_ptr    = &cave[y][x];
	feature_type *f_ptr    = &f_info[c_ptr->feat];
	bool         can_enter = TRUE;

	/*
	 * Can't enter square if it's already occupied by another monster
	 * or the player.
	 */
	if (((p_ptr->py == y) && (p_ptr->px == x)) || (c_ptr->m_idx != 0))
		return FALSE;

	if (!monster_can_cross_terrain(c_ptr->feat, &m_ptr->flags))
		return FALSE;

	if (!can_pass_aux(&f_ptr->flags, &m_ptr->flags, how))
		return FALSE;

	if (has_flag(f_ptr, FLAG_MONST_CAN_ENTER)) {
		s32b domain = flag_get( &f_ptr->flags, FLAG_MONST_CAN_ENTER);
		s32b ref    = flag_get2(&f_ptr->flags, FLAG_MONST_CAN_ENTER);

		open_lua_functions_registry(domain, ref);
		call_lua(NULL, "(d, d, b, M)", "b", y, x, FALSE, m_ptr,
				 &can_enter);
		close_lua_functions_registry();
	}

	return can_enter;
}

