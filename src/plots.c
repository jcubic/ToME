/* File: plots.c */

/* Purpose: plots & quests */

/*
 * Copyright (c) 2001 James E. Wilson, Robert A. Koeneke, DarkGod
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

#include "lua/lua.h"
#include "tolua.h"
extern lua_State* L;

/* #define DEBUG_HOOK */

/******** Hooks stuff *********/
PHYSFS_file *hook_file;

#define MAX_ARGS        50

hooks_chain *hooks_heads[MAX_HOOKS];

/* Wipe hooks and init them with quest hooks */
void wipe_hooks()
{
	s32b i;

	for (i = 0; i < MAX_HOOKS; i++)
	{
		hooks_heads[i] = NULL;
	}
}
void init_hooks()
{
	s32b i;

	for (i = 0; i < MAX_Q_IDX_INIT; i++)
	{
		if ((quest[i].type == HOOK_TYPE_C) && (quest[i].init != NULL)) quest[i].init(i);
	}
}

void dump_hooks(s32b h_idx)
{
	s32b min = 0, max = MAX_HOOKS, i;

	if (h_idx != -1)
	{
		min = h_idx;
		max = h_idx + 1;
	}

	for (i = min; i < max; i++)
	{
		hooks_chain *c = hooks_heads[i];

		/* Find it */
		while (c != NULL)
		{
			msg_format("%s(%s)", c->name, (c->type == HOOK_TYPE_C) ? "C" : "Lua");

			c = c->next;
		}
	}
}

/* Check a hook */
bool check_hook(s32b h_idx)
{
	hooks_chain *c = hooks_heads[h_idx];

	return (c != NULL);
}

/* Add a hook */
hooks_chain* add_hook(s32b h_idx, hook_type hook, cptr name)
{
	hooks_chain *new, *c = hooks_heads[h_idx];

	/* Find it */
	while ((c != NULL) && (strcmp(c->name, name)))
	{
		c = c->next;
	}

	/* If not already in the list, add it */
	if (c == NULL)
	{
		MAKE(new, hooks_chain);
		new->hook = hook;
		new->name = string_make(name);
#ifdef DEBUG_HOOK
		if (wizard) cmsg_format(TERM_VIOLET, "HOOK ADD: %s", name);
		if (take_notes) add_note(format("HOOK ADD: %s", name), 'D');
#endif
		new->next = hooks_heads[h_idx];
		hooks_heads[h_idx] = new;
		return (new);
	}
	else return (c);
}

void add_hook_script(s32b h_idx, char *script, cptr name)
{
	hooks_chain *c = add_hook(h_idx, NULL, name);
#ifdef DEBUG_HOOK
	if (wizard) cmsg_format(TERM_VIOLET, "HOOK LUA ADD: %s : %s", name, script);
#endif
	c->script = string_make(script);
	c->type = HOOK_TYPE_LUA;
}

/* Remove a hook */
void del_hook(s32b h_idx, hook_type hook)
{
	hooks_chain *c = hooks_heads[h_idx], *p = NULL;

	/* Find it */
	while ((c != NULL) && (c->hook != hook))
	{
		p = c;
		c = c->next;
	}

	/* Remove it */
	if (c != NULL)
	{
		if (p == NULL)
		{
#ifdef DEBUG_HOOK
			if (wizard) cmsg_format(TERM_VIOLET, "HOOK DEL: %s", c->name);
			if (take_notes) add_note(format("HOOK DEL: %s", c->name), 'D');
#endif
			hooks_heads[h_idx] = c->next;
			string_free(c->name);
			string_free(c->script);
			FREE(c, hooks_chain);
		}
		else
		{
#ifdef DEBUG_HOOK
			if (wizard) cmsg_format(TERM_VIOLET, "HOOK DEL: %s", c->name);
			if (take_notes) add_note(format("HOOK DEL: %s", c->name), 'D');
#endif
			p->next = c->next;
			string_free(c->name);
			string_free(c->script);
			FREE(c, hooks_chain);
		}
	}
}

void del_hook_name(s32b h_idx, cptr name)
{
	hooks_chain *c = hooks_heads[h_idx], *p = NULL;

	/* Find it */
	while ((c != NULL) && (strcmp(c->name, name)))
	{
		p = c;
		c = c->next;
	}

	/* Remove it */
	if (c != NULL)
	{
		if (p == NULL)
		{
#ifdef DEBUG_HOOK
			if (wizard) cmsg_format(TERM_VIOLET, "HOOK DEL: %s", c->name);
			if (take_notes) add_note(format("HOOK DEL: %s", c->name), 'D');
#endif
			hooks_heads[h_idx] = c->next;
			FREE(c, hooks_chain);
		}
		else
		{
#ifdef DEBUG_HOOK
			if (wizard) cmsg_format(TERM_VIOLET, "HOOK DEL: %s", c->name);
			if (take_notes) add_note(format("HOOK DEL: %s", c->name), 'D');
#endif
			p->next = c->next;
			FREE(c, hooks_chain);
		}
	}
}

/* get the next argument */
static hook_return param_pile[MAX_ARGS];
static s32b get_next_arg_pos = 0;
static s32b get_next_arg_pile_pos = 0;
s32b get_next_arg(char *fmt)
{
	while (TRUE)
	{
		switch (fmt[get_next_arg_pos++])
		{
		case 'd':
		case 'l':
			return (param_pile[get_next_arg_pile_pos++].num);
		case ')':
			get_next_arg_pos--;
			return 0;
		case '(':
		case ',':
			break;
		}
	}
}
char* get_next_arg_str(char *fmt)
{
	while (TRUE)
	{
		switch (fmt[get_next_arg_pos++])
		{
		case 's':
			return (char*)(param_pile[get_next_arg_pile_pos++].str);
		case ')':
			get_next_arg_pos--;
			return 0;
		case '(':
		case ',':
			break;
		}
	}
}

/* Actually process the hooks */
s32b process_hooks_restart = FALSE;
hook_return process_hooks_return[20];
static bool vprocess_hooks_return (s32b h_idx, char *ret, char *fmt, va_list *ap)
{
	hooks_chain *c = hooks_heads[h_idx];
	va_list real_ap;

	while (c != NULL)
	{
#ifdef DEBUG_HOOK
		if (wizard) cmsg_format(TERM_VIOLET, "HOOK: %s", c->name);
		if (take_notes) add_note(format("HOOK PROCESS: %s", c->name), 'D');
#endif
		if (c->type == HOOK_TYPE_C)
		{
			s32b i = 0, nb = 0;

			/* Push all args in the pile */
			i = 0;
			(void) COPY(&real_ap, ap, va_list);
			while (fmt[i])
			{
				switch (fmt[i])
				{
				case 'O':
					param_pile[nb++].o_ptr = va_arg(real_ap, object_type *);
					break;
				case 'M':
					param_pile[nb++].m_ptr = va_arg(real_ap, monster_type *);
					break;
				case 'F':
					param_pile[nb++].flags = va_arg(real_ap, flags_type *);
					break;
				case 'R':
					param_pile[nb++].r_ptr = va_arg(real_ap, monster_race *);
					break;
				case 's':
					param_pile[nb++].str = va_arg(real_ap, char *);
					break;
				case 'b':
					param_pile[nb++].boolean = va_arg(real_ap, s32b);
					break;
				case 'd':
				case 'l':
					param_pile[nb++].num = va_arg(real_ap, s32b);
					break;
				case 'n':
					cmsg_print(TERM_VIOLET, "Oops no nil type in C...");
					break;
				case '(':
				case ')':
				case ',':
				case ' ':
					break;
				default:
					cmsg_format(TERM_VIOLET,
							   "Unknown process_hooks_ret() format '%c'...",
							   fmt[i]);
					break;
				}
				i++;
			}

			get_next_arg_pos = 0;
			get_next_arg_pile_pos = 0;
			if (c->hook(fmt))
			{
				return TRUE;
			}

			/* Should we restart ? */
			if (process_hooks_restart)
			{
				c = hooks_heads[h_idx];
				process_hooks_restart = FALSE;
			}
			else
			{
				c = c->next;
			}
		}
		else if (c->type == HOOK_TYPE_LUA)
		{
			s32b i = 0, nb = 0, nbr = 0, num_rets = 1;
			s32b oldtop = lua_gettop(L), size;

			/* Push the function */
			lua_getglobal(L, c->script);

			/* Push and count the arguments */
			(void) COPY(&real_ap, ap, va_list);
			while (fmt[i])
			{
				switch (fmt[i++])
				{
				case 'b':
					tolua_pushbool(L, va_arg(real_ap, s32b));
					nb++;
					break;
				case 'd':
				case 'l':
					tolua_pushnumber(L, va_arg(real_ap, s32b));
					nb++;
					break;
				case 's':
					tolua_pushstring(L, va_arg(real_ap, char*));
					nb++;
					break;
				case 'O':
					tolua_pushusertype(L, (void*)va_arg(real_ap, object_type*), tolua_tag(L, "object_type"));
					nb++;
					break;
				case 'R':
					tolua_pushusertype(L, (void*)va_arg(real_ap, monster_race*), tolua_tag(L, "monster_race"));
					nb++;
					break;
				case 'M':
					tolua_pushusertype(L, (void*)va_arg(real_ap, monster_type*), tolua_tag(L, "monster_type"));
					nb++;
					break;
				case 'F':
					tolua_pushusertype(L, (void*)va_arg(real_ap, flags_type*), tolua_tag(L, "flags_type"));
					nb++;
					break;
				case 'n':
					lua_pushnil(L);
					nb++;
					break;
				case '(':
				case ')':
				case ',':
				case ' ':
					break;
				default:
					cmsg_format(TERM_VIOLET,
							   "Unknown process_hooks_ret() format '%c'...",
							   fmt[i - 1]);
					break;
				}
			}

			/* Count returns */
			num_rets += strlen(ret);

			/* Call the function */
			if (lua_call(L, nb, num_rets))
			{
				cmsg_format(TERM_VIOLET, "ERROR in lua_call while calling '%s' lua hook script. Breaking the hook chain now.", c->script);
				return FALSE;
			}

			/* Number of returned values, SHOULD be the same as nbr, but I'm paranoid */
			size = lua_gettop(L) - oldtop;

			/* get the extra returns if needed */
			for (i = 0; i < num_rets - 1; i++)
			{
				if ((ret[i] == 'd') || (ret[i] == 'l'))
				{
					if (lua_isnumber(L, ( -size) + 1 + nbr)) process_hooks_return[nbr].num = tolua_getnumber(L, ( -size) + 1 + nbr, 0);
					else process_hooks_return[nbr].num = 0;
				}
				else if (ret[i] == 'b')
				{
					if (lua_isnumber(L, ( -size) + 1 + nbr)) process_hooks_return[nbr].boolean = tolua_getbool(L, ( -size) + 1 + nbr, 0);
					else process_hooks_return[nbr].boolean = FALSE;
				}
				else if (ret[i] == 's')
				{
					if (lua_isstring(L, ( -size) + 1 + nbr)) process_hooks_return[nbr].str = tolua_getstring(L, ( -size) + 1 + nbr, "");
					else process_hooks_return[nbr].str = NULL;
				}
				else if (ret[i] == 'O')
				{
					if (tolua_istype(L, ( -size) + 1 + nbr, tolua_tag(L, "object_type"), 0))
						process_hooks_return[nbr].o_ptr = (object_type*)tolua_getuserdata(L, ( -size) + 1 + nbr, NULL);
					else
						process_hooks_return[nbr].o_ptr = NULL;
				}
				else if (ret[i] == 'M')
				{
					if (tolua_istype(L, ( -size) + 1 + nbr, tolua_tag(L, "monster_type"), 0))
						process_hooks_return[nbr].m_ptr = (monster_type*)tolua_getuserdata(L, ( -size) + 1 + nbr, NULL);
					else
						process_hooks_return[nbr].m_ptr = NULL;
				}
				else if (ret[i] == 'F')
				{
					if (tolua_istype(L, ( -size) + 1 + nbr, tolua_tag(L, "flags_type"), 0))
						process_hooks_return[nbr].flags = (flags_type*)tolua_getuserdata(L, ( -size) + 1 + nbr, NULL);
					else
						process_hooks_return[nbr].flags = NULL;
				}
				else if (ret[i] == '(' || ret[i] == ')' || ret[i] == ',' ||
						 ret[i] == ' ')
					nbr--;
				else
				{
					cmsg_format(TERM_VIOLET,
							   "Unknown process_hooks_ret() return"
							   "format '%c'...", ret[i]);
					process_hooks_return[nbr].num = 0;
				}
				nbr++;
			}

			/* Get the basic return(continue or stop the hook chain) */
			if (tolua_getnumber(L, -size, 0))
			{
				lua_settop(L, oldtop);
				return (TRUE);
			}
			if (process_hooks_restart)
			{
				c = hooks_heads[h_idx];
				process_hooks_restart = FALSE;
			}
			else
				c = c->next;
			lua_settop(L, oldtop);
		}
		else
		{
			msg_format("Unknown hook type %d, name %s", c->type, c->name);
		}
	}

	return FALSE;
}

bool process_hooks_ret(s32b h_idx, char *ret, char *fmt, ...)
{
	va_list ap;
	bool r;

	va_start(ap, fmt);
	r = vprocess_hooks_return (h_idx, ret, fmt, &ap);
	va_end(ap);
	return (r);
}

bool process_hooks(s32b h_idx, char *fmt, ...)
{
	va_list ap;
	bool ret;

	va_start(ap, fmt);
	ret = vprocess_hooks_return (h_idx, "", fmt, &ap);
	va_end(ap);
	return (ret);
}

/******** Plots & Quest stuff ********/

/* Catch-all quest hook */
bool quest_null_hook(s32b q)
{
	/* Do nothing */
	return (FALSE);
}
