/* File: power.c */

/* Purpose: Powers */

/*
 * Copyright (c) 2001 James E. Wilson, Robert A. Koeneke, DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

/*
 * Note: return value indicates the amount of mana to use
 */
bool power_chance(power_type *x_ptr)
{
#if 0 // DGDGDGDG -- mana is no more hardcoded
	bool use_hp = FALSE;
	s32b diff = x_ptr->diff;

	/* Always true ? */
	if (!x_ptr->cost) return TRUE;

	/* Not enough mana - use hp */
	if (p_ptr->csp < x_ptr->cost) return FALSE;

	/* Power is not available yet */
	if (p_ptr->lev < x_ptr->level)
	{
		msg_format("You need to attain level %d to use this power.", x_ptr->level);
		energy_use = 0;
		return (FALSE);
	}

	/* Too confused */
	else if (intrinsic(CONFUSED))
	{
		msg_print("You are too confused to use this power.");
		energy_use = 0;
		return (FALSE);
	}

	/* Else attempt to do it! */
	if (p_ptr->lev > x_ptr->level)
	{
		s32b lev_adj = ((p_ptr->lev - x_ptr->level) / 3);
		if (lev_adj > 10) lev_adj = 10;
		diff -= lev_adj;
	}

	if (diff < 5) diff = 5;

	/* take time and pay the price */
	p_ptr->csp -= (x_ptr->cost / 2 ) + (randint(x_ptr->cost / 2));

	energy_use = get_player_energy(SPEED_POWER);

	/* Redraw mana and hp */
	flag_bool(&p_ptr->redraw, FLAG_PR_HP);
	flag_bool(&p_ptr->redraw, FLAG_PR_MANA);

	/* Window stuff */
	p_ptr->window |= (PW_PLAYER);

	/* Success? */
	// DGDGDGDGDG this was stat_cur, needs adjusting
	// DGDGDGDGDG dont bother, jsut change the formula to be sane anyway
	if (randint(get_stat(x_ptr->stat, ind)) >=
	                ((diff / 2) + randint(diff / 2)))
	{
		return (TRUE);
	}

	if (flush_failure) flush();
	msg_print("You've failed to concentrate hard enough.");
#endif
	return (FALSE);
}

static void power_activate(s32b power)
{
	power_type *x_ptr = &powers_type[power];

	if (!power_chance(x_ptr)) return;

	switch (power)
	{
	default:
		if (!process_hooks(HOOK_ACTIVATE_POWER, "(d)", power))
		{
			msg_format("Warning power_activate() called with invalid power(%d).", power);
			energy_use = 0;
		}
		break;
	}

	flag_bool(&p_ptr->redraw, FLAG_PR_HP);
	flag_bool(&p_ptr->redraw, FLAG_PR_MANA);
}

/*
 * Print a batch of power.
 */
static void print_power_batch(s32b *p, s32b start, s32b max, bool mode)
{
	char buff[80];
	power_type* spell;
	s32b i = start, j = 0;

	if (mode) prt(format("         %-31s Level Mana Fail", "Name"), 1, 20);

	for (i = start; i < (start + 20); i++)
	{
		if (i >= max) break;

		spell = &powers_type[p[i]];

		sprintf(buff, "  %c-%3ld) %-30s  %5d %4d %s@%d", (char) I2A(j),
				p[i] + 1, spell->name,
		        spell->level, spell->cost, stat_names[spell->stat], spell->diff);

		if (mode) prt(buff, 2 + j, 20);
		j++;
	}
	if (mode) prt("", 2 + j, 20);
	prt(format("Select a power (a-%c), +/- to scroll:", I2A(j - 1)), 0, 0);
}


/*
 * List powers and ask to pick one.
 */

static power_type* select_power(s32b *x_idx)
{
	s32b which;
	s32b max = 0, i, start = 0;
	power_type* ret;
	bool mode = easy_inven;
	s32b *p;

	C_MAKE(p, power_max, s32b);

	/* Count the max */
	for (i = 0; i < power_max; i++)
	{
		if (flag_get(&p_ptr->powers, i))
		{
			p[max++] = i;
		}
	}

	/* Exit if there aren't powers */
	if (max == 0)
	{
		*x_idx = -1;
		ret = NULL;
		msg_print("You don't have any special powers.");
	}
	else
	{
		character_icky++;
		Term_save();

		while (1)
		{
			print_power_batch(p, start, max, mode);
			which = inkey();

			if (which == ESCAPE)
			{
				*x_idx = -1;
				ret = NULL;
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

				*x_idx = p[start + A2I(which)];
				ret = &powers_type[p[start + A2I(which)]];
				break;
			}
		}
		Term_load();
		character_icky--;
	}

	C_FREE(p, power_max, s32b);

	return ret;
}

/* Ask & execute a power */
void do_cmd_power()
{
	s32b x_idx;
	power_type *x_ptr;
	bool push = TRUE;

	/* Get the skill, if available */
	if (repeat_pull(&x_idx))
	{
		if ((x_idx < 0) || (x_idx >= power_max)) return;
		x_ptr = &powers_type[x_idx];
		push = FALSE;
	}
	else if (!command_arg) x_ptr = select_power(&x_idx);
	else
	{
		x_idx = command_arg - 1;
		if ((x_idx < 0) || (x_idx >= power_max)) return;
		x_ptr = &powers_type[x_idx];
	}

	if (x_ptr == NULL) return;

	if (push) repeat_push(x_idx);

	if (flag_get(&p_ptr->powers, x_idx))
		power_activate(x_idx);
	else
		msg_print("You do not have access to this power.");
}
