/* File: cmd6.c */

/* Purpose: Object commands */

/*
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "tolua.h"
extern lua_State* L;



/*
 * General function to find an item by its name
 */
cptr get_item_hook_find_obj_what;
bool get_item_hook_find_obj(s32b *item)
{
	char buf[80];
	char buf2[100];

	strcpy(buf, "");
	if (!get_string(get_item_hook_find_obj_what, buf, 79))
		return FALSE;

	for_inventory(p_ptr, o_ptr, 0, INVEN_TOTAL);
	{
		if (!item_tester_okay(o_ptr, item_slot)) continue;

		object_desc(buf2, o_ptr, -1, 0);
		if (!strcmp(buf, buf2))
		{
			*item = item_slot;
			return TRUE;
		}
	}
	end_inventory();

	return FALSE;
}
