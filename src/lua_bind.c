/* File: lua_bind.c */

/* Purpose: various lua bindings */

/*
 * Copyright (c) 2001 DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"
#include "sdl-defines.h"
#include "SDL.h"

#include "lua.h"
#include "tolua.h"
extern lua_State *L;

/*
 * Get a new magic type
 */
magic_power *new_magic_power(s32b num)
{
	magic_power *m_ptr;
	C_MAKE(m_ptr, num, magic_power);
	return (m_ptr);
}
magic_power *grab_magic_power(magic_power *m_ptr, s32b num)
{
	return (&m_ptr[num]);
}
static char *magic_power_info_lua_fct;
static void magic_power_info_lua(char *p, s32b power)
{
	s32b oldtop = lua_gettop(L);

	lua_getglobal(L, magic_power_info_lua_fct);
	tolua_pushnumber(L, power);
	lua_call(L, 1, 1);
	strcpy(p, lua_tostring(L, -1));
	lua_settop(L, oldtop);
}
bool get_magic_power_lua(s32b *sn, magic_power *powers, s32b max_powers, char *info_fct, s32b plev, s32b cast_stat)
{
	magic_power_info_lua_fct = info_fct;
	return (get_magic_power(sn, powers, max_powers, magic_power_info_lua, plev, cast_stat));
}

/*
 * Powers
 */
s16b add_new_power(cptr name, cptr desc, cptr gain, cptr lose, byte level, byte cost, byte stat, byte diff)
{
	/* Increase the size */
	reinit_powers_type(power_max + 1);

	/* Copy the strings */
	C_MAKE(powers_type[power_max - 1].name, strlen(name) + 1, char);
	strcpy(powers_type[power_max - 1].name, name);
	C_MAKE(powers_type[power_max - 1].desc_text, strlen(desc) + 1, char);
	strcpy(powers_type[power_max - 1].desc_text, desc);
	C_MAKE(powers_type[power_max - 1].gain_text, strlen(gain) + 1, char);
	strcpy(powers_type[power_max - 1].gain_text, gain);
	C_MAKE(powers_type[power_max - 1].lose_text, strlen(lose) + 1, char);
	strcpy(powers_type[power_max - 1].lose_text, lose);

	/* Copy the other stuff */
	powers_type[power_max - 1].level = level;
	powers_type[power_max - 1].cost = cost;
	powers_type[power_max - 1].stat = stat;
	powers_type[power_max - 1].diff = diff;

	return (power_max - 1);
}

/*
 * Inventory slots
 */
s16b add_new_inventory_slot(cptr desc, cptr name, cptr describe, cptr wear, cptr takeoff)
{
	/* Increase the size */
	reinit_inventory_slot_names(INVEN_TOTAL + 1);

	/* Copy the strings */
	C_MAKE(inventory_slot_names[INVEN_TOTAL - 1].desc, strlen(desc) + 1, char);
	strcpy((char*)inventory_slot_names[INVEN_TOTAL - 1].desc, desc);

	C_MAKE(inventory_slot_names[INVEN_TOTAL - 1].name, strlen(name) + 1, char);
	strcpy((char*)inventory_slot_names[INVEN_TOTAL - 1].name, name);

	C_MAKE(inventory_slot_names[INVEN_TOTAL - 1].describe, strlen(describe) + 1, char);
	strcpy((char*)inventory_slot_names[INVEN_TOTAL - 1].describe, describe);

	C_MAKE(inventory_slot_names[INVEN_TOTAL - 1].wear, strlen(wear) + 1, char);
	strcpy((char*)inventory_slot_names[INVEN_TOTAL - 1].wear, wear);

	C_MAKE(inventory_slot_names[INVEN_TOTAL - 1].takeoff, strlen(takeoff) + 1, char);
	strcpy((char*)inventory_slot_names[INVEN_TOTAL - 1].takeoff, takeoff);

	return (INVEN_TOTAL - 1);
}

static char *lua_item_tester_fct;
static bool lua_item_tester(object_type* o_ptr, s32b item)
{
	s32b oldtop = lua_gettop(L);
	bool ret;

	lua_getglobal(L, lua_item_tester_fct);
	tolua_pushusertype(L, o_ptr, tolua_tag(L, "object_type"));
	tolua_pushnumber(L, item);
	lua_call(L, 2, 1);
	ret = lua_tonumber(L, -1);
	lua_settop(L, oldtop);
	return (ret);
}

void lua_set_item_tester(s32b tval, char *fct)
{
	if (tval == -1)
	{
		item_tester_tval = 0;
		item_tester_hook = NULL;
	}
	else if (tval)
	{
		item_tester_tval = tval;
	}
	else
	{
		lua_item_tester_fct = fct;
		item_tester_hook = lua_item_tester;
	}
}

char *lua_object_desc_store(object_type *o_ptr, s32b pref, s32b mode)
{
	static char buf[200];

	object_desc_store(buf, o_ptr, pref, mode);
	return (buf);
}


/*
 * Monsters
 */

void find_position(s32b y, s32b x, s32b *yy, s32b *xx)
{
	s32b attempts = 500;

	do
	{
		scatter(yy, xx, y, x, 6, 0);
	}
	while (!(in_bounds(*yy, *xx) && cave_floor_bold(*yy, *xx)) && --attempts);
}

static char *summon_lua_okay_fct;
bool summon_lua_okay(s32b r_idx)
{
	s32b oldtop = lua_gettop(L);
	bool ret;

	lua_getglobal(L, summon_lua_okay_fct);
	tolua_pushnumber(L, r_idx);
	lua_call(L, 1, 1);
	ret = lua_tonumber(L, -1);
	lua_settop(L, oldtop);
	return (ret);
}

bool lua_summon_monster(s32b y1, s32b x1, s32b lev, s32b faction,
						bool group_ok, char *fct)
{
	s32b y, x, i, r_idx;
	bool (*old_get_mon_num_hook)(s32b r_idx);

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

		/* Don't place any monsters on this type of terrain? */
		if (has_flag(f_ptr, FLAG_MONST_NO_PLACE))
			continue;

		/* Okay */
		break;
	}

	/* Failure */
	if (i == 20) return (FALSE);

	/* Backup the old hook */
	old_get_mon_num_hook = get_mon_num_hook;
	summon_lua_okay_fct = fct;

	/* Prepare allocation table */
	get_mon_num_prep();


	/* Pick a monster, using the level calculation */
	summon_hack = TRUE;
	r_idx = get_mon_num((dun_level + lev) / 2 + 5);
	summon_hack = FALSE;

	/* Reset restriction */
	get_mon_num_hook = old_get_mon_num_hook;

	/* Prepare allocation table */
	get_mon_num_prep();


	/* Handle failure */
	if (!r_idx) {
		if (wizard)
			cmsg_format(TERM_VIOLET, "ERROR: lua_summon_monster()'s monster "
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
} /* lua_summon_monster() */

/*
 * Quests
 */
s16b add_new_quest(char *name)
{
	s32b i;

	/* Increase the size */
	reinit_quests(max_q_idx + 1);
	quest[max_q_idx - 1].type = HOOK_TYPE_LUA;
	strncpy(quest[max_q_idx - 1].name, name, 39);

	for (i = 0; i < 10; i++)
		strncpy(quest[max_q_idx - 1].desc[i], "", 39);

	return (max_q_idx - 1);
}

void quest_set_desc(s32b q_idx, s32b d, char *desc)
{
	if (d >= 0 && d < 10)
		strncpy(quest[q_idx].desc[d], desc, 79);
}


cptr quest_get_desc(s32b q_idx, s32b line)
{
	if (line >= 1 && line <= 10)
		return(quest[q_idx].desc[line - 1]);
	else
		return NULL;
}

/*
 * Misc
 */
bool get_com_lua(cptr prompt, s32b *com, bool keep_cursor)
{
	s32b c;

	if (!get_com_aux(prompt, &c, keep_cursor)) return (FALSE);
	*com = c;
	return (TRUE);
}

/* Spell schools */

s32b lua_spell_chance(s32b chance, s32b level, s32b skill_level, s32b mana, s32b cur_mana, s32b fail_adj, s32b fail_min)
{
	s32b minfail;
	/* Reduce failure rate by "effective" level adjustment */
	chance -= 3 * (level - 1);

	/* Reduce failure rate by INT/WIS adjustment */
	chance -= 3 * (fail_adj - 1);

	/* Not enough mana to cast */
	if (chance < 0) chance = 0;
	if (mana > cur_mana)
	{
		chance += 15 * (mana - cur_mana);
	}

	/* Extract the minimum failure rate */
	minfail = fail_min;

	/*
	        * Non mage characters never get too good
	 */
	if (!(has_ability(AB_PERFECT_CASTING)))
	{
		if (minfail < 5) minfail = 5;
	}

	/* Minimum failure rate */
	if (chance < minfail) chance = minfail;

	/* Always a 5 percent chance of working */
	if (chance > 95) chance = 95;

	/* Return the chance */
	return (chance);
}

s32b lua_spell_device_chance(s32b skill, s32b chance, s32b level, s32b base_level)
{
	s32b minfail;

	/* Reduce failure rate by "effective" level adjustment */
	chance -= (level - 1);

	/* Extract the minimum failure rate */
	minfail = 15 - get_skill_scale(skill, 25);
	if (minfail < 0) minfail = 0;

	/* Minimum failure rate */
	if (chance < minfail) chance = minfail;

	/* Always a 5 percent chance of working */
	if (chance > 95) chance = 95;

	/* Return the chance */
	return (chance);
}

/* Cave */
cave_type *lua_get_cave(s32b y, s32b x)
{
	return (&(cave[y][x]));
}

void set_target(s32b y, s32b x)
{
	target_who = -1;
	target_col = x;
	target_row = y;
}

void get_target(s32b dir, s32b *y, s32b *x)
{
	s32b ty, tx;

	/* Use the given direction */
	tx = p_ptr->px + (ddx[dir] * 100);
	ty = p_ptr->py + (ddy[dir] * 100);

	/* Hack -- Use an actual "target" */
	if ((dir == 5) && target_okay())
	{
		tx = target_col;
		ty = target_row;
	}
	*y = ty;
	*x = tx;
}

/* Allow lua to use a temporary file */
static char lua_temp_name[1025];
static bool lua_temp_file_alloc = FALSE;
bool lua_make_temp_file()
{
	if (lua_temp_file_alloc)
		return FALSE;

	if (path_temp(lua_temp_name, 1024))
		return FALSE;

	/* Open a new file */
	hook_file = my_fopen(lua_temp_name, "w");
	lua_temp_file_alloc = TRUE;

	return TRUE;
}

errr lua_write_temp_file(cptr str)
{
	if (!lua_temp_file_alloc)
		return FALSE;

	if (str == NULL)
		str = "";

	return my_fputs(hook_file, str);
}


void lua_close_temp_file()
{
	/* Close the file */
	my_fclose(hook_file);
}

void lua_end_temp_file()
{
	/* Remove the file */
	fd_kill(lua_temp_name);
	lua_temp_file_alloc = FALSE;
}

cptr lua_get_temp_name()
{
	if (lua_temp_file_alloc)
		return lua_temp_name;
	else
		return "";
}

bool alloc_room(s32b by0, s32b bx0, s32b ysize, s32b xsize, s32b *y1, s32b *x1, s32b *y2, s32b *x2)
{
	s32b xval, yval, x, y;

	/* Try to allocate space for room.  If fails, exit */
	if (!room_alloc(xsize + 2, ysize + 2, FALSE, by0, bx0, &xval, &yval)) return FALSE;

	/* Get corner values */
	*y1 = yval - ysize / 2;
	*x1 = xval - xsize / 2;
	*y2 = yval + (ysize) / 2;
	*x2 = xval + (xsize) / 2;

	/* Place a full floor under the room */
	for (y = *y1 - 1; y <= *y2 + 1; y++)
	{
		for (x = *x1 - 1; x <= *x2 + 1; x++)
		{
			cave_type *c_ptr = &cave[y][x];
			cave_set_feat(y, x, floor_type[rand_int(100)]);
			c_ptr->info |= (CAVE_ROOM);
			c_ptr->info |= (CAVE_GLOW);
		}
	}
	return TRUE;
}


/* Files */
void lua_print_hook(cptr str)
{
	fprintf(hook_file, str);
}

/*
 * Some misc functions
 */
char *lua_input_box(cptr title, s32b max, cptr def)
{
	static char buf[80];
	s32b wid, hgt;

	strcpy(buf, def);
	Term_get_size(&wid, &hgt);
	if (!input_box(title, hgt / 2, wid / 2, buf, (max > 79) ? 79 : max))
		return "";
	return buf;
}

s32b lua_msg_box(cptr title)
{
	s32b wid, hgt;

	Term_get_size(&wid, &hgt);
	return msg_box(title, hgt / 2, wid / 2);
}

list_type *lua_create_list(s32b size)
{
	list_type *l;
	cptr *list;

	MAKE(l, list_type);
	C_MAKE(list, size, cptr);
	l->list = list;
	return l;
}

void lua_delete_list(list_type *l, s32b size)
{
	s32b i;

	for (i = 0; i < size; i++)
		string_free(l->list[i]);
	C_FREE(l->list, size, cptr);
	FREE(l, list_type);
}

void lua_add_to_list(list_type *l, s32b idx, cptr str)
{
	l->list[idx] = string_make(str);
}

void lua_display_list(s32b y, s32b x, s32b h, s32b w, cptr title,
					  list_type* list, s32b max, s32b begin, s32b sel,
					  byte sel_color, bool show_scrollbar)
{
	display_list(y, x, h, w, title, list->list, max, begin, sel, sel_color,
				 show_scrollbar);
}

/*
 * Level generators
 */
bool level_generate_script(cptr name)
{
	s32b ret = FALSE;

	call_lua("level_generate", "(s)", "d", name, &ret);

	return ret;
}

void add_scripted_generator(cptr name, bool stairs, bool monsters, bool objects, bool miscs)
{
	add_level_generator(name, level_generate_script, stairs, monsters, objects, miscs);
}

/*
 * RT timer
 */

/* SDL timer callback */
static Uint32 sdl_timer_callback(Uint32 interval, void *param)
{
        SDL_Event event;
	SDL_UserEvent userevent;

        userevent.type = SDL_USEREVENT;
        userevent.code = EVENT_GAME_RT_TIMER;
        userevent.data1 = (void*)interval;
        userevent.data2 = param;

        event.type = SDL_USEREVENT;
        event.user = userevent;

        SDL_PushEvent(&event);
        return(interval);
}

SDL_TimerID rt_timer_add(s32b interval, void *c_callback)
{
	if (!strcmp(ANGBAND_SYS, "sdl"))
		return SDL_AddTimer(interval, sdl_timer_callback, c_callback);
	else
		return 0;
}

void rt_timer_del(SDL_TimerID id)
{
	if (!strcmp(ANGBAND_SYS, "sdl"))
		SDL_RemoveTimer(id);
}

cptr lua_apply_keymap(s32b ch)
{
	s32b mode;

	if ((ch < 0) || (ch > 255))
		return NULL;

	/* Roguelike */
	if (rogue_like_commands)
		mode = KEYMAP_MODE_ROGUE;
	/* Original */
	else
		mode = KEYMAP_MODE_ORIG;


	return keymap_act[mode][(byte)ch];
}
