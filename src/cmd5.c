/* File: cmd5.c */

/* Purpose: Class commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */


#include "angband.h"
#include "lua/lua.h"
#include "tolua.h"

extern lua_State *L;


/* Maximum number of tries for teleporting */
#define MAX_TRIES 300

bool is_school_book(object_type *o_ptr)
{
	return has_flag(o_ptr, FLAG_SPELLBOOK);
}

/* Does it contains a schooled spell ? */
static bool hook_school_spellable(object_type *o_ptr, s32b item)
{
	if (is_school_book(o_ptr))
		return TRUE;
	else
	{
		if (has_flag(o_ptr, FLAG_SPELL_CONTAIN) && (get_flag(o_ptr, FLAG_SPELL_CONTAIN) != -1))
			return TRUE;
	}
	return FALSE;
}

/* Is it a book */
bool item_tester_hook_browsable(object_type *o_ptr, s32b item)
{
	if (hook_school_spellable(o_ptr, item)) return TRUE;
	if (has_flag(o_ptr, FLAG_SPELLBOOK)) return TRUE;
	return FALSE;
}

/*
 * Peruse the spells/prayers in a book
 *
 * Note that *all* spells in the book are listed
 *
 * Note that browsing is allowed while confused or blind,
 * and in the dark, primarily to allow browsing in stores.
 */

extern void do_cmd_browse_aux(object_type *o_ptr)
{
	browse_school_spell(o_ptr);
}

void do_cmd_browse(void)
{
	s32b item;

	cptr q, s;

	object_type *o_ptr;

	/* Restrict choices to "useful" books */
	item_tester_hook = item_tester_hook_browsable;

	/* Get an item */
	q = "Browse which book? ";
	s = "You have no books that you can read.";
	if (!get_item(&item, q, s, (USE_INVEN | USE_EQUIP | USE_FLOOR))) return;

	/* Get the item (in the pack) */
	o_ptr = get_object(item);

	do_cmd_browse_aux(o_ptr);
}


/*
 * Fetch an item (teleport it right underneath the caster)
 */
void fetch(s32b dir, s32b wgt, bool require_los)
{
	s32b ty, tx;

	bool flag;

	cave_type *c_ptr;

	/* Check to see if an object is already there */
	if (!flag_used(&cave[p_ptr->py][p_ptr->px].inventory))
	{
		msg_print("You can't fetch when you're already standing on something.");
		return;
	}

	/* Use a target */
	if ((dir == 5) && target_okay())
	{
		tx = target_col;
		ty = target_row;

		if (distance(p_ptr->py, p_ptr->px, ty, tx) > MAX_RANGE)
		{
			msg_print("You can't fetch something that far away!");
			return;
		}

		c_ptr = &cave[ty][tx];

		if (!flag_used(&c_ptr->inventory))
		{
			msg_print("There is no object at this place.");
			return;
		}

		if (require_los && (!player_has_los_bold(ty, tx)))
		{
			msg_print("You have no direct line of sight to that location.");
			return;
		}
	}
	else
	{
		/* Use a direction */
		ty = p_ptr->py;  /* Where to drop the item */
		tx = p_ptr->px;
		flag = FALSE;

		while (1)
		{
			ty += ddy[dir];
			tx += ddx[dir];
			c_ptr = &cave[ty][tx];

			if ((distance(p_ptr->py, p_ptr->px, ty, tx) > MAX_RANGE) ||
			                !cave_floor_bold(ty, tx)) return;

			if (flag_used(&c_ptr->inventory)) break;
		}
	}
#if 0 //DGDGDGDG
	o_ptr = o_list[c_ptr->o_idx];

	if (o_ptr->weight > wgt)
	{
		/* Too heavy to 'fetch' */
		msg_print("The object is too heavy.");
		return;
	}

	i = c_ptr->o_idx;
	c_ptr->o_idx = o_ptr->next_o_idx;
	cave[p_ptr->py][p_ptr->px].o_idx = i;  /* 'move' it */
	o_ptr->next_o_idx = 0;
	o_ptr->iy = p_ptr->py;
	o_ptr->ix = p_ptr->px;

	object_desc(o_name, o_ptr, TRUE, 0);
	msg_format("%^s flies through the air to your feet.", o_name);

	note_spot(p_ptr->py, p_ptr->px);
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);
#endif
}

/*
 * Schooled magic
 */

/*
 * Find a spell in any books/objects
 */
static s32b hack_force_spell = -1;
static object_type *hack_force_spell_obj = NULL;
bool get_item_hook_find_spell(s32b *item)
{
	s32b spell;
	char buf[80];
	s32b ret;

	strcpy(buf, "Manathrust");
	if (!get_string("Spell name? ", buf, 79))
		return FALSE;

	spell = find_spell(buf);
	if (spell == -1) return FALSE;

	for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		/* Must we wield it ? */
		if ((wield_slot(o_ptr) != -1) && (inven_nb < INVEN_PACK) && ((has_flag(o_ptr, FLAG_WIELD_CAST)))) continue;

		call_lua("spell_in_book", "(O,d)", "d", o_ptr, spell, &ret);
		if (ret)
		{
			*item = item_slot;
			hack_force_spell = spell;
			hack_force_spell_obj = o_ptr;
			return TRUE;
		}
	}
	end_inventory();
	return FALSE;
}

/*
 * Get a spell from a book
 */
s32b get_school_spell(cptr do_what, cptr check_fct, object_type *force_book, bool browse)
{
	s32b i, item;
	s32b spell = -1;
	s32b num = 0;
	s32b where = 1;
	s32b ask;
	bool flag, redraw, no_icky;
	s32b choice;
	char out_val[160];
	char buf2[40];
	char buf3[40];
	object_type *o_ptr;
	s32b tmp;
	bool used_get_item = FALSE;
	term_win *save_screen;

	hack_force_spell = -1;
	hack_force_spell_obj = NULL;

	if (repeat_pull(&tmp))
	{
		s32b ret;
		for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
		{
			/* Must we wield it ? */
			if ((wield_slot(o_ptr) != -1) && (inven_nb < INVEN_PACK) && ((has_flag(o_ptr, FLAG_WIELD_CAST)))) continue;

			call_lua("spell_in_book", "(O,d)", "d", o_ptr, tmp, &ret);
			if (ret)
			{
				return tmp;
			}
		}
		end_inventory();
	}

	/* Ok do we need to ask for a book ? */
	if (force_book == NULL)
	{
		get_item_extra_hook = get_item_hook_find_spell;
		item_tester_hook = hook_school_spellable;
		sprintf(buf2, "You have no book to %s from", do_what);
		sprintf(buf3, "%s from which book?", do_what);
		if (!get_item(&item, buf3, buf2, USE_INVEN | USE_EQUIP | USE_EXTRA )) return -1;

		/* Get the item (in the pack) */
		o_ptr = get_object(item);

		/* If it can be wielded, it must */
		if ((wield_slot(o_ptr) != -1) && (item_slot_to_inven(item) < INVEN_PACK) && ((has_flag(o_ptr, FLAG_WIELD_CAST))))
		{
			msg_format("You cannot %s from that object, it must be wielded first.", do_what);
			return -1;
		}

		used_get_item = TRUE;
	}
	else
	{
		o_ptr = force_book;
	}

	/* Nothing chosen yet */
	flag = FALSE;

	/* No redraw yet */
	redraw = easy_inven;
	no_icky = ((character_icky != 0) || hack_force_spell != -1) ? TRUE : FALSE;
	save_screen = Term_save_to();

	/* Show choices */
	if (show_choices)
	{
		/* Window stuff */
		window_stuff();
	}

	/* No spell to cast by default */
	spell = -1;

	if (hack_force_spell == -1)
	{
		call_lua("book_spells_num", "(O)", "d", o_ptr, &num);

		/* Build a prompt (accept all spells) */
		strnfmt(out_val, 78, "(Spells %c-%c, Descs %c-%c, *=List, ESC=exit) %^s which spell? ",
			I2A(0), I2A(num - 1), I2A(0) - 'a' + 'A', I2A(num - 1) - 'a' + 'A', do_what);

		if (redraw)
		{
			/* Save the screen */
			if (!no_icky) character_icky++;

			/* Display a list of spells */
			call_lua("print_book", "(O)", "d", o_ptr, &where);
		}

		/* Get a spell from the user */
		while (!flag && get_com(out_val, &choice))
		{
			/* Request redraw */
			if (((choice == ' ') || (choice == '*') || (choice == '?')))
			{
				/* Show the list */
				if (!redraw)
				{
					/* Show list */
					redraw = TRUE;

					/* Save the screen */
					if (!no_icky) character_icky++;

					/* Display a list of spells */
					call_lua("print_book", "(O)", "d", o_ptr, &where);
				}

				/* Hide the list */
				else
				{
					/* Hide list */
					redraw = FALSE;
					where = 1;

					/* Restore the screen */
					Term_load_from(save_screen, FALSE);
					if (!no_icky) character_icky--;
				}

				/* Redo asking */
				continue;
			}

			if (choice == '@')
			{
				char buf[80];
				s32b ok;

				strcpy(buf, "");
				if (!get_string("Which spell? ", buf, 79))
					break;

				/* Get spell idx from name (if spell is in object at all) */
				call_lua("spell_from_book_by_name", "(O,s)", "d",
						 o_ptr, buf, &spell);

				if (spell == -1)
					break;

				/* Do we need to do some pre test */
				call_lua(check_fct, "(d,O)", "d", spell, o_ptr, &ok);

				/* Require "okay" spells */
				if (!ok)
				{
					bell();
					msg_format("You may not %s that spell.", do_what);
					spell = -1;
					break;
				}
				flag = TRUE;
				break;
			} /* if (choice == '@') */

			/* Note verify */
			ask = (isupper(choice));

			/* Lowercase */
			if (ask) choice = tolower(choice);

			/* Extract request */
			i = (islower(choice) ? A2I(choice) : -1);

			/* Totally Illegal */
			if ((i < 0) || (i >= num))
			{
				bell();
				continue;
			}

			/* Verify it */
			if (ask || browse)
			{
				/* Show the list */
				if (!redraw)
				{
					/* Show list */
					redraw = TRUE;

					/* Save the screen */
					if (!no_icky) character_icky++;
					Term_load_from(save_screen, FALSE);
				}
				/* Rstore the screen */
				else
				{
					/* Restore the screen */
					Term_load_from(save_screen, FALSE);
				}

				/* Display a list of spells */
				call_lua("print_book", "(O)", "d", o_ptr, &where);
				call_lua("print_spell_desc_spell_x", "(O,d,d)", "", o_ptr, i, where);
			}
			else
			{
				s32b ok;

				/* Save the spell index */
				call_lua("spell_x", "(O,d)", "d", o_ptr, i, &spell);

				/* Do we need to do some pre test */
				call_lua(check_fct, "(d,O)", "d", spell, o_ptr, &ok);

				/* Require "okay" spells */
				if (!ok)
				{
					bell();
					msg_format("You may not %s that spell.", do_what);
					spell = -1;
					continue;
				}

				/* Stop the loop */
				flag = TRUE;
			}
		}
	}
	else
	{
		s32b ok;

		/* Require "okay" spells */
		call_lua(check_fct, "(d, O)", "d", hack_force_spell, hack_force_spell_obj, &ok);
		if (ok)
		{
			flag = TRUE;
			spell = hack_force_spell;
		}
		else
		{
			bell();
			msg_format("You may not %s that spell.", do_what);
			spell = -1;
		}
	}


	/* Restore the screen */
	if (redraw)
	{
		if (!no_icky) character_icky--;
	}

	Term_load_from(save_screen, TRUE);


	/* Show choices */
	if (show_choices)
	{
		/* Window stuff */
		window_stuff();
	}

	/* Abort if needed */
	if (!flag) return -1;

	tmp = spell;

	/*
	 * If we used get_item, we remove what it pushed on the command stack as it is not needed
	 * now taht we know which spell to cast
	 */
	if (used_get_item)
	{
		repeat_pop();
	}
	repeat_push(tmp);
	return spell;
}

void cast_school_spell()
{
	s32b spell;

	/* No magic */
	if (p_ptr->antimagic)
	{
		msg_print("Your anti-magic field disrupts any magic attempts.");
		return;
	}

	/* No magic */
	if (intrinsic(NO_MAGIC))
	{
		msg_print("Your anti-magic shell disrupts any magic attempts.");
		return;
	}

	spell = get_school_spell("cast", "is_ok_spell", NULL, FALSE);

	/* Actualy cast the choice */
	if (spell != -1)
	{
		exec_lua(format("cast_school_spell(%d, spell(%d))", spell, spell));
	}
}

void browse_school_spell(object_type *o_ptr)
{
	s32b i;
	s32b num = 0, where = 1;
	s32b ask;
	s32b choice;
	char out_val[160];

	/* Show choices */
	if (show_choices)
	{
		/* Window stuff */
		window_stuff();
	}

	call_lua("book_spells_num", "(O)", "d", o_ptr, &num);

	/* Build a prompt (accept all spells) */
	strnfmt(out_val, 78, "(Spells %c-%c, ESC=exit) cast which spell? ",
	        I2A(0), I2A(num - 1));

	/* Save the screen */
	character_icky++;
	Term_save();

	/* Display a list of spells */
	call_lua("print_book", "(O)", "d", o_ptr, &where);

	/* Get a spell from the user */
	while (get_com(out_val, &choice))
	{
		/* Display a list of spells */
		call_lua("print_book", "(O)", "d", o_ptr, &where);

		/* Note verify */
		ask = (isupper(choice));

		/* Lowercase */
		if (ask) choice = tolower(choice);

		/* Extract request */
		i = (islower(choice) ? A2I(choice) : -1);

		/* Totally Illegal */
		if ((i < 0) || (i >= num))
		{
			bell();
			continue;
		}

		/* Restore the screen */
		Term_load();

		/* Display a list of spells */
		call_lua("print_book", "(O)", "d", o_ptr, &where);
		call_lua("print_spell_desc_spell_x", "(O,d,d)", "", o_ptr, i, where);
	}


	/* Restore the screen */
	Term_load();
	character_icky--;

	/* Show choices */
	if (show_choices)
	{
		/* Window stuff */
		window_stuff();
	}
}

/* Can it contains a schooled spell ? */
static bool hook_school_can_spellable(object_type *o_ptr)
{
	if (((has_flag(o_ptr, FLAG_SPELL_CONTAIN))) && (!has_flag(o_ptr, FLAG_SPELL_IDX)))
		return TRUE;
	return FALSE;
}

/*
 * Finds a spell by name, optimized for speed
 */
s32b find_spell(char *name)
{
	s32b spell;
	call_lua("find_spell", "(s)", "d", name, &spell);
	return spell;
}
