/* File: store.c */

/* Purpose: Store commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

store_type *store_type_new()
{
	store_type *store;

	MAKE(store, store_type);
	flag_init(&store->stock, 2);
	flag_set(&store->stock, INVEN_LIMIT_KEY, 0);

	return store;
}
void store_type_del(store_type *store)
{
	flag_free(&store->stock, FALSE);
	FREE(store, store_type);
}

store_type *store_type_dup(store_type *store)
{
	flags_type save;
	store_type *store2 = store_type_new();

	flag_copy(&store2->stock, &store->stock);
	save = store2->stock;
	*store2 = *store;
	store2->stock = save;

	return store2;
}

/*
 * Enter a store, and interact with it.
 *
 * Note that we use the standard "request_command()" function
 * to get a command, allowing us to use "command_arg" and all
 * command macros and other nifty stuff, but we use the special
 * "shopping" argument, to force certain commands to be converted
 * into other commands, normally, we convert "p" (pray) and "m"
 * (cast magic) into "g" (get), and "s" (search) into "d" (drop).
 */
void do_cmd_store(void)
{
	s32b which;
	s32b maintain_num;
	s32b i;
	bool recreate = FALSE;
	store_type *st_ptr;
	store_info_type *sti_ptr;

	cave_type *c_ptr;

	/* Access the player grid */
	c_ptr = &cave[p_ptr->py][p_ptr->px];

	/* Verify a store */
	if (!cave_feat_is(c_ptr, FLAG_CONTAINS_BUILDING))
	{
		msg_print("You see no store here.");
		return;
	}

	/* Extract the store code */
	which = get_flag(c_ptr, FLAG_CONTAINS_BUILDING);

	/* Get the store */
	st_ptr = flag_get_store(&town_info[p_ptr->town_num].stores, which);

	/* If it does not exists yet, create it */
	if (!st_ptr)
	{
		call_lua("store.create_for_town", "(d,d)", "", p_ptr->town_num, which);
		st_ptr = flag_get_store(&town_info[p_ptr->town_num].stores, which);

		if (!st_ptr) quit(format("Could not create store %d town %d! BAD BAD BAD!", which, p_ptr->town_num));
	}

	/* Hack -- Check the "locked doors" */
	if (st_ptr->store_open > turn)
	{
		msg_print("The doors are locked.");
		return;
	}
	sti_ptr = &st_info[st_ptr->st_idx];

	/* Calculate the number of store maintainances since the last visit */
	if (has_flag(sti_ptr, FLAG_STORE_MAINTAIN_TURNS))
	{
		if (get_flag(sti_ptr, FLAG_STORE_MAINTAIN_TURNS) >= 1)
			maintain_num = (turn - st_ptr->last_visit) / (10L * get_flag(sti_ptr, FLAG_STORE_MAINTAIN_TURNS));
		else
			maintain_num = 0;
	}
	else
		maintain_num = (turn - st_ptr->last_visit) / (10L * STORE_TURNS);

	/* Maintain the store max. 10 times */
	if (maintain_num > 10) maintain_num = 10;

	if (maintain_num)
	{
		/* Maintain the store */
		for (i = 0; i < maintain_num; i++)
			call_lua("store.maintain", "(S)", "", st_ptr);

		/* Save the visit */
		st_ptr->last_visit = turn;
	}

	/* Forget the lite */
	/* forget_lite(); */

	/* Forget the view */
	forget_view();

	call_lua("store.display", "(S)", "", st_ptr);

	/* Free turn XXX XXX XXX */
	energy_use = 0;

	/* Recreate the level only when needed */
	if (recreate)
	{
		/* Reinit wilderness to activate quests ... */
		p_ptr->oldpx = p_ptr->px;
		p_ptr->oldpy = p_ptr->py;

		p_ptr->leaving = TRUE;
	}

	/* Hack -- Cancel automatic command */
	command_new = 0;

	/* Hack -- Cancel "see" mode */
	command_see = FALSE;


	/* Flush messages XXX XXX XXX */
	msg_print(NULL);


	/* Clear the screen */
	Term_clear();

	/* Update everything */
	p_ptr->update |= (PU_VIEW | PU_MON_LITE);
	p_ptr->update |= (PU_MONSTERS);

	/* Redraw entire screen */
	flag_bool(&p_ptr->redraw, FLAG_PR_BASIC);
	flag_bool(&p_ptr->redraw, FLAG_PR_EXTRA);

	/* Redraw map */
	flag_bool(&p_ptr->redraw, FLAG_PR_MAP);

	/* Window stuff */
	flag_bool(&p_ptr->window, FLAG_PW_OVERHEAD);
}
