/* File: script.c */

/* Purpose: scripting in lua */

/*
 * Copyright (c) 2001 Dark God
 *
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

#include "angband.h"

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/luadebug.h"
#include "lauxlib.h"
#include "tolua.h"
#include "lobject.h"
#include "SDL.h"
#include "SDL_net.h"

#ifdef RISCOS
extern char *riscosify_name(const char *path);
#endif


s32b tolua_monster_open (lua_State *L);
s32b tolua_player_open (lua_State *L);
s32b tolua_player_c_open (lua_State *L);
s32b tolua_util_open (lua_State *L);
s32b tolua_z_pack_open (lua_State *L);
s32b tolua_object_open (lua_State *L);
s32b tolua_spells_open (lua_State *L);
s32b tolua_quest_open (lua_State *L);
s32b tolua_dungeon_open (lua_State *L);
s32b tolua_flags_open (lua_State *L);
s32b tolua_speeds_open (lua_State* tolua_S);
s32b tolua_physfs_open (lua_State *L);
s32b tolua_net_open (lua_State* tolua_S);
s32b tolua_sound_open (lua_State* tolua_S);

/*
 * Lua state
 */
lua_State* L = NULL;

/* ToME Lua error message handler */
static s32b tome_errormessage(lua_State *L)
{
	char buf[200];
	cptr str = luaL_check_string(L, 1);
	s32b i = 0, j = 0;

	while (str[i])
	{
		if (str[i] == '#')
		{
			buf[j++] = '$';
		}
		else if (str[i] != '\n')
		{
			buf[j++] = str[i];
		}
		else
		{
			buf[j] = '\0';
			cmsg_format(TERM_VIOLET, "LUA: %s", buf);
			j = 0;
		}
		i++;
	}
	buf[j] = '\0';
	cmsg_format(TERM_VIOLET, "LUA: %s", buf);
	return (0);
}

#ifndef LUA_ERROR_TO_TERMINAL
static struct luaL_reg tome_iolib[] =
{
	{ "_ALERT", tome_errormessage },
};
#endif

#define luaL_check_bit(L, n)  ((long)luaL_check_number(L, n))
#define luaL_check_ubit(L, n) ((unsigned long)luaL_check_bit(L, n))

/*
 * Dyadic integer modulus operation
 * DYADIC(mod,      %)
 */
static int int_mod(lua_State* L)
{
	lua_pushnumber(L, luaL_check_bit(L, 1) % luaL_check_bit(L, 2));
	return 1;
}

/*
 * Binary left shift operation
 * TDYADIC(lshift,  <<, , u)
 */
static int int_lshift(lua_State* L)
{
	lua_pushnumber(L, luaL_check_bit(L, 1) << luaL_check_ubit(L, 2));
	return 1;
}

/*
 * Binary logical right shift operation
 * TDYADIC(rshift,  >>, u, u)
 */
static int int_rshift(lua_State* L)
{
	lua_pushnumber(L, luaL_check_ubit(L, 1) >> luaL_check_ubit(L, 2));
	return 1;
}

/*
 * Binary arithmetic right shift operation
 * TDYADIC(arshift, >>, , u)
 */
static int int_arshift(lua_State* L)
{
	lua_pushnumber(L, luaL_check_bit(L, 1) >> luaL_check_ubit(L, 2));
	return 1;
}

/*
 * Integer exponentiation operation
 */
static int int_pow(lua_State* L)
{
	long x = luaL_check_bit(L, 1);
	long y = luaL_check_bit(L, 2);
	s32b result = 1;
	s32b i;
	for(i = 0; i < y; ++i)
		result *= x;
	lua_pushnumber(L, result);
	return 1;
}

/*
 * Integer square root operation
 */
static int int_sqrt(lua_State* L)
{
	lua_pushnumber(L, sroot(luaL_check_bit(L, 1)));
	return 1;
}

static const struct luaL_reg bitlib[] =
{
	{"imod", int_mod},   /* "mod" already in Lua math library */
	{"lshift", int_lshift},
	{"rshift", int_rshift},
	{"arshift", int_arshift},
	{"ipow", int_pow},
	{"isqrt", int_sqrt},
};

static int lua_net_send_string(lua_State* L)
{
	if (!tolua_istype(L, 1, tolua_tag(L, "TCPsocket"), 0) ||
		!tolua_istype(L, 2, LUA_TSTRING, 0) ||
		!tolua_isnoobj(L, 3)
	   )
	{
		tolua_error(L, "#ferror in function 'send_string'.");
		return 0;
	}
	else
	{
		TCPsocket conn = ((TCPsocket) tolua_getusertype(L, 1, 0));
		size_t len = lua_strlen(L, 2);
		cptr buf_src = tolua_getstring(L, 2, 0);
		long sent_len;

		sent_len = SDLNet_TCP_Send(conn, (char*)buf_src, len);
		tolua_pushnumber(L, (long)sent_len);

		return 1;
	}
}

static int lua_net_recv_string(lua_State* L)
{
	if (!tolua_istype(L, 1, tolua_tag(L, "TCPsocket"), 0) ||
		!tolua_istype(L, 2, LUA_TNUMBER, 0) ||
		!tolua_isnoobj(L, 3)
	   )
	{
		tolua_error(L, "#ferror in function 'recv_string'.");
		return 0;
	}
	else
	{
		TCPsocket conn = ((TCPsocket) tolua_getusertype(L, 1, 0));
		long len = tolua_getnumber(L, 2, 0);
		long read_len;
		char *buf;

		C_MAKE(buf, len, char);

		read_len = SDLNet_TCP_Recv(conn, (char*)buf, len);
		tolua_pushnumber(L, read_len);
		if (read_len > 0)
			lua_pushlstring(L, buf, read_len);

		C_FREE(buf, len, char);

		return (read_len > 0) ? 2 : 1;
	}
}

/*
 * List files/dirs of a directory
 */
static int enumerate_files(lua_State* L)
{
	if (!tolua_istype(L, 1, LUA_TSTRING, 0) ||
		!tolua_istype(L,2,tolua_tag(L,"bool"),0) ||
		!tolua_isnoobj(L, 3)
	   )
	{
		tolua_error(L, "#ferror in function 'enumerate_files'.");
		return 0;
	}
	else
	{
		bool only_dir = (bool)  tolua_getbool(L,2,0);
		cptr dir = tolua_getstring(L, 1, 0);
		char **rc = PHYSFS_enumerateFiles(dir);
		char **i;
		s32b nb = 0;
		char buf[2048];

		lua_newtable(L);
		for (i = rc; *i != NULL; i++)
		{
			path_build(buf, 2048, dir, *i);
			if (only_dir && (!PHYSFS_isDirectory(buf)))
				continue;

			lua_pushnumber(L, nb + 1);
			lua_pushstring(L, *i);
			lua_settable(L, -3);
			nb++;
		}

		PHYSFS_freeList(rc);

		return 1;
	}
}

/*
 * List dirs in the search path
 */
static int get_search_path(lua_State* L)
{
	char **i, **f;
	s32b nb;

	lua_newtable(L);

	f = PHYSFS_getSearchPath();
	for (i = f, nb = 1; *i != NULL; i++, nb++)
	{
		lua_pushnumber(L, nb);
		lua_pushstring(L, *i);
		lua_settable(L, -3);
	}

	PHYSFS_freeList(f);

	return 1;
}

/* Bytecode dumper/loader */
extern char *luaU_dumpchunk_buffer(const Proto* Main, size_t *len);
extern Proto* lua_toluafunction (lua_State *L, s32b index);

s32b lua_bytecode_dump(lua_State *L)
{
	if (!tolua_istype(L, 1, LUA_TFUNCTION, 0) ||
		!tolua_isnoobj(L, 2)
	   )
	{
		tolua_error(L, "#ferror in function 'bytecode.dump'.");
		return 0;
	}
	else
	{
		char *buf;
		size_t len;
		Proto *l = lua_toluafunction(L, -1); lua_pop(L, 1);

		buf = luaU_dumpchunk_buffer(l, &len);
		lua_pushlstring(L, buf, len);
		C_FREE(buf, len, char);

		return 1;
	}
}

static int lua_bytecode_dump_to_data_buffer(lua_State *L)
{
	if (!tolua_istype(L, 1, LUA_TFUNCTION, 0) ||
		!tolua_isnoobj(L, 2)
	   )
	{
		tolua_error(L, "#ferror in function 'bytecode.dump'.");
		return 0;
	}
	else
	{
		size_t len;
		buffer_type *buf;
		Proto *l = lua_toluafunction(L, -1); lua_pop(L, 1);
		buf = data_buffer_alloc();

		buf->buf = luaU_dumpchunk_buffer(l, &len);
		buf->len = len;

		tolua_pushusertype(L, (void*)buf, tolua_tag(L, "buffer_type"));

		return 1;
	}
}

static int lua_bytecode_load(lua_State *L)
{
	if (!(tolua_istype(L, 1, LUA_TSTRING, 0) ||
		  tolua_istype(L, 1, tolua_tag(L, "buffer_type"), 0)) ||
		!tolua_isnoobj(L, 2)
	   )
	{
		tolua_error(L, "#ferror in function 'bytecode.load'.");
		return 0;
	}
	else
	{
		size_t len;
		cptr code;

		if (tolua_istype(L, 1, LUA_TSTRING, 0))
		{
			len = lua_strlen(L, -1);
			code = lua_tostring(L, -1);
		}
		else
		{
			buffer_type* buffer =
				(buffer_type*) tolua_getuserdata(L, ( -1), NULL);
			len = buffer->len;
			code = buffer->buf;
		}

		lua_pop(L, 1);

		lua_loadbuffer(L, code, len, "(bytecode loaded)");
		return 1;
	}
}

/*
 * Get a string from the user.  Takes default value as argument.
 */
static int lua_askfor_aux(lua_State* L)
{
	char  buf[1024];
	cptr default_str;
	bool  ret_val;

	buf[0] = '\0';

	default_str = lua_tostring(L, 1);

	if (default_str != NULL)
		strcpy(buf, default_str);

	ret_val = askfor_aux(buf, 1024);

	/* Return nil if the user pressed escape */
	if (!ret_val)
		lua_pushnil(L);
	else
		lua_pushstring(L, buf);

	return 1;
}

/*
 * From an allocation table, choose a random item_kind index.
 * Implemented in C, since nested tables in Lua are very slow.
 */
static int lua_get_rand_k_idx_aux(lua_State* L)
{
	alloc_entry* table = (alloc_entry*) lua_touserdata(L, 1);

	s32b size          = (s32b) lua_tonumber(L, 2);
	s32b obj_level     = (s32b) lua_tonumber(L, 3);
	s32b obj_level_min = (s32b) lua_tonumber(L, 4);

	s32b min_total = 0;
	s32b min_idx   = 0;
	s32b max_total = 0;
	s32b max_idx   = -1;

	s32b i, choice;

	if (obj_level_min > obj_level)
		obj_level_min = obj_level;

	if (obj_level_min > table[size - 1].level)
		obj_level_min =  table[size - 1].level;

	for(i = 0; i < size; i++) {
		/* Find the last item of too low level in the list */
		if (table[i].level < obj_level_min) {
			min_idx = i + 1;
			min_total = table[i].total;
		}

		if (table[i].level > obj_level)
			break;

		max_total = table[i].total;
		max_idx   = i;
	}

	if ((max_total == 0) || (max_idx == -1)) {
		lua_pushnil(L);
		return 1;
	}

	choice = rand_int(max_total - min_total);

	for (i = min_idx; i <= max_idx; i++)
		if ((choice + min_total) < table[i].total) {
			lua_pushnumber(L, table[i].index);
			return 1;
		}
	lua_pushnil(L);

	if (wizard)
		plog("lua_get_rand_k_idx_aux: nothing matching in table?");

	return 1;
} /* lua_get_rand_k_idx_aux() */

static int lua_flag_get_obj(lua_State* L)
{
	flags_type*  t   = (flags_type*) lua_touserdata(L, 1);
	u32b         key = lua_tonumber(L, 2);
	object_type* o_ptr;

	o_ptr = (object_type*) flag_get_ptr(t, key, FLAG_TYPE_OBJ);

	tolua_pushusertype(L, o_ptr, tolua_tag(L, "object_type"));

	return 1;
} /* lua_flag_get_obj() */

static int lua_flag_set_obj(lua_State* L)
{
	flags_type*  t     = (flags_type*)  lua_touserdata(L, 1);
	u32b         key   = lua_tonumber(L, 2);
	object_type* o_ptr = (object_type*) lua_touserdata(L, 3);

	flag_set_ptr(t, key, FLAG_TYPE_OBJ, o_ptr);

	return 1;
} /* lua_flag_set_obj() */

/*
 * Stuff a lua nested table into an array of C structures so
 * lua_get_rand_k_idx_aux() can handle it.
 */
static int lua_make_c_alloc_table(lua_State* L)
{
	s32b          size  = lua_getn(L, -1);
	alloc_entry* table = calloc(size, sizeof(alloc_entry));
	s32b          i;

	for(i = 0; i < size; i++) {
		/* Get Ith nested table */
		lua_pushnumber(L, i + 1);
		lua_gettable(L, -2);

		lua_pushstring(L, "index");
		lua_gettable(L, -2);
		table[i].index = (s16b) lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "level");
		lua_gettable(L, -2);
		table[i].level = (byte) lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "total");
		lua_gettable(L, -2);
		table[i].total = (s16b) lua_tonumber(L, -1);
		lua_pop(L, 1);

		/* Remove Ith nested table */
		lua_pop(L, 1);
	} /* while (lua_next(L, 1) != 0) */

	lua_pushuserdata(L, (void*) table);

	return 1;
} /* lua_make_c_alloc_table() */

static int lua_scatter(lua_State* L)
{
	s32b yp, xp;

/*
	s32b y = (s32b) tolua_getnumber(L, 1, 0);
	s32b x = (s32b) tolua_getnumber(L, 2, 0);
	s32b d = (s32b) tolua_getnumber(L, 3, 0);
	s32b m = (s32b) tolua_getnumber(L, 4, 0);
*/

	s32b y = (s32b) lua_tonumber(L, 1);
	s32b x = (s32b) lua_tonumber(L, 2);
	s32b d = (s32b) lua_tonumber(L, 3);
	s32b m = (s32b) lua_tonumber(L, 4);

	scatter(&yp, &xp, y, x, d, m);

	lua_pushnumber(L, yp);
	lua_pushnumber(L, xp);

	return 2;
}

/* function: Term_what */
static int z_pack_term_Term_what(lua_State* tolua_S)
{
	if (
		!tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
		!tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
		!tolua_isnoobj(tolua_S,3)
		)
	{
		tolua_error(tolua_S,"#ferror in function 'Term_what'.");
		return 0;
	}
	else
	{
		s32b x = ((s32b)  tolua_getnumber(tolua_S,1,0));
		s32b y = ((s32b)  tolua_getnumber(tolua_S,2,0));
		byte a;
		char c;
		errr toluaI_ret = (errr)  Term_what(x,y,&a,&c);
		tolua_pushnumber(tolua_S,(long)toluaI_ret);
		tolua_pushnumber(tolua_S,(long)a);
		tolua_pushnumber(tolua_S,(long)c);
		return 3;
	}
}

static int lua_birth_process_player_name(lua_State *L)
{
	if (!tolua_istype(L, 1, LUA_TSTRING, 0) || !tolua_isnoobj(L, 2))
	{
		tolua_error(L, "#ferror in function 'birth.process_player_name'.");
		return 0;
	}
	else
	{
		cptr name = lua_tostring(L, -1);
		lua_pop(L, 1);

		strnfmt(player_base, 127, "%s", name);
		process_player_name(TRUE);
		return 1;
	}
}

static int lua_find_naked_bold(lua_State *L)
{
	s32b y, x;
	s32b set = (s32b) lua_tonumber(L, 1);

	if (find_naked_bold(set, &y, &x))
	{
		lua_pushnumber(L, y);
		lua_pushnumber(L, x);
	}
	else
	{
		lua_pushnil(L);
		lua_pushnil(L);
	}

	return 2;
}

/*
 * Initialize lua scripting
 */
static byte init_lua_done = 0;
void init_lua()
{
	/* Hack -- Do not initialize more than once */
	if (init_lua_done) return;
	init_lua_done = 1;

	/* Start the interpreter with default stack size */
	L = lua_open(0);

	/* Register the Lua base libraries */
	lua_baselibopen(L);
	lua_strlibopen(L);
	lua_iolibopen(L);
	lua_dblibopen(L);

	/* Tolua */
	tolua_open(L);
#ifndef LUA_ERROR_TO_TERMINAL
	/* Register tome lua debug library */
	luaL_openl(L, tome_iolib);
#endif

	/* Register the bitlib */
	luaL_openl(L, bitlib);

	/* Load the data fillers wrappers */
	load_cache_wrappers();

	/* Register the ToME main APIs */
	tolua_player_open(L);
	tolua_player_c_open(L);
	tolua_util_open(L);
	tolua_z_pack_open(L);
	tolua_object_open(L);
	tolua_monster_open(L);
	tolua_spells_open(L);
	tolua_quest_open(L);
	tolua_dungeon_open(L);
	tolua_physfs_open(L);
	tolua_net_open(L);
	tolua_sound_open(L);

	/* Register some special wrappers */
	tolua_module(L,   "birth");
	tolua_function(L, "birth",    "process_player_name", lua_birth_process_player_name);
	tolua_function(L, "net",      "send", lua_net_send_string);
	tolua_function(L, "net",      "read", lua_net_recv_string);
	tolua_function(L, "fs",       "enumerate_files", enumerate_files);
	tolua_function(L, "fs",       "get_search_path", get_search_path);
	tolua_module(L,   "bytecode");
	tolua_function(L, "bytecode", "dump_to_data_buffer", lua_bytecode_dump_to_data_buffer);
	tolua_function(L, "bytecode", "dump", lua_bytecode_dump);
	tolua_function(L, "bytecode", "load", lua_bytecode_load);
	tolua_function(L, "term",     "askfor_aux", lua_askfor_aux);
	tolua_function(L, "term",     "Term_what", z_pack_term_Term_what);

	tolua_function(L, NULL,       "flag_get_obj", lua_flag_get_obj);
	tolua_function(L, NULL,       "flag_set_obj", lua_flag_set_obj);

	tolua_function(L, NULL,       "scatter",         lua_scatter);
	tolua_function(L, NULL,       "find_naked_bold", lua_find_naked_bold);

	/*
	 * Functions implemented in C because they're too slow in Lua.
	 */
	tolua_module(L,   "rand_k_idx");
	tolua_function(L, "rand_k_idx", "make_c_table",
				   lua_make_c_alloc_table);
	tolua_function(L, "rand_k_idx", "get", lua_get_rand_k_idx_aux);
}

void load_perm_data()
{
	call_lua("load_perm_data", "()", "");
}

void save_perm_data()
{
	call_lua("save_perm_data", "()", "");
}

void init_lua_init()
{
	/* Cant be done before, because they arent defined yet */
	tolua_flags_open(L);
	tolua_speeds_open(L);

	/* Load the first lua file */
	tome_dofile_anywhere(TENGINE_DIR_ENGINE, "init.lua", TRUE, FALSE);

	/* Load permanent data */
	load_perm_data();

	/* Finished */
	init_lua_done = 2;
}

#ifdef COMPILE_LUA
#define LUA_USE_SOURCE          1
#define LUA_USE_BINARY          2
#define LUA_USE_RECOMPILE       4

/* Checks if file needs recompilation */
static byte must_recompile(char *src)
{
	char obj[1024];

	/* Make up the destination file .lua => .luo */
	strncpy(obj, src, 1024);
	obj[strlen(obj) - 2] = 'b';
	obj[strlen(obj) - 1] = '\0';

	/* Make sure physfs is suficiently initialized to allow compiling */
	if (!PHYSFS_getWriteDir()) return (LUA_USE_SOURCE);

	/* Access stats on source file */
	if (!file_exist(src))
	{
		/* Error */
		return (LUA_USE_BINARY);
	}

	/* Access stats on object file */
	if (!file_exist(obj))
	{
		/* Error */
		return (LUA_USE_RECOMPILE | LUA_USE_BINARY);
	}

	/* Ensure text file is not newer than raw file */
	if (PHYSFS_getLastModTime(src) > PHYSFS_getLastModTime(obj))
	{
		/* Reprocess text file */
		return (LUA_USE_RECOMPILE | LUA_USE_BINARY);
	}

	return (LUA_USE_BINARY);
}
#endif /* COMPILE_LUA */


/* Is lua initliazed yet */
byte is_lua_started()
{
	return init_lua_done;
}

/* Loads a lua file from lib/scpt*/
bool tome_compile_lua_ok = TRUE;
extern bool module_initialized;
bool tome_dofile(char *file, bool bypass_redirection)
{
	return tome_dofile_anywhere(TENGINE_DIR_SCPT, file, TRUE, bypass_redirection);
}

/* Loads a lua file from anywhere */
bool tome_dofile_anywhere(cptr dir, char *file, bool test_exist, bool bypass_redirection)
{
	cptr tmp;
	char buf[2048];
	s32b oldtop = lua_gettop(L);

	/* Build the filename */
	path_build(buf, sizeof(buf), dir, file);
	if (module_initialized && (!bypass_redirection))
	{
		/* Is that location rehooked to somewhere else? */
		call_lua("get_real_file_path", "(s)", "s", buf, &tmp);
		strncpy(buf, tmp, 2047);
	}

        /* Need to recompile ? */
#ifdef COMPILE_LUA
	if (tome_compile_lua_ok)
	{
		byte res = LUA_USE_SOURCE;
		if (file_exist(buf)) res = must_recompile(buf);

		if (res & LUA_USE_RECOMPILE)
		{
			tome_compile_lua(buf);
		}

		/* Now load the .luo file instead of the .lua file */
		if (res & LUA_USE_BINARY)
		{
			buf[strlen(buf) - 2] = 'b';
			buf[strlen(buf) - 1] = '\0';
		}

	}
#endif

	if (!file_exist(buf))
	{
		if (test_exist)
			cmsg_format(TERM_VIOLET,
				"tome_dofile_anywhere(): file %s(%s) doesn't exist in %s.", dir, file, buf);
		return (FALSE);
	}

#ifdef RISCOS
	{
		char *realname = riscosify_name(buf);
		lua_dofile(L, realname);
	}
#else /* RISCOS */
	lua_dofile(L, buf);
#endif /* RISCOS */

	lua_settop(L, oldtop);

	return (TRUE);
}

/* Execute a lua string */
s32b exec_lua(cptr file)
{
	s32b oldtop = lua_gettop(L);
	s32b res;

	if (!lua_dostring(L, file))
	{
		s32b size = lua_gettop(L) - oldtop;
		if (size != 0)
			res = tolua_getnumber(L, -size, 0);
		else
			res = 0;
	}
	else
		res = 0;

	lua_settop(L, oldtop);
	return (res);
}

/* Execute  a lua string and gets a string as return */
cptr string_exec_lua(cptr file)
{
	s32b oldtop = lua_gettop(L);
	cptr res;

	if (!lua_dostring(L, file))
	{
		s32b size = lua_gettop(L) - oldtop;
		res = tolua_getstring(L, -size, "");
	}
	else
		res = "";
	lua_settop(L, oldtop);
	return (res);
}

void dump_lua_stack(s32b min, s32b max)
{
	s32b i;

	cmsg_print(TERM_YELLOW, "lua_stack:");
	for (i = min; i <= max; i++)
	{
		if (lua_isnumber(L, i)) cmsg_format(TERM_YELLOW, "%d [n] = %d", i, tolua_getnumber(L, i, 0));
		else if (lua_isstring(L, i)) cmsg_format(TERM_YELLOW, "%d [s] = '%s'", i, tolua_getstring(L, i, 0));
	}
	cmsg_print(TERM_YELLOW, "END lua_stack");
}

/* Grabs a variable */
static s32b push_variable(cptr name, s32b oldtop)
{
	s32b i;
	s32b offset = 0;
	s32b level = 0;
	char *table;

	C_MAKE(table, strlen(name) + 1, char);

	for (i = 0; i < strlen(name); i++)
	{
		if (name[i] == '.') {
			table[i - offset] = '\0';
			if (offset == 0)
				lua_getglobal(L, table);
			else
			{
				lua_pushstring(L, table);
				lua_gettable(L, -2);
			}
			offset = i + 1;
		}
		else
			table[i - offset] = name[i];
	}
	table[i - offset] = '\0';

	if (offset == 0)
		lua_getglobal(L, table);
	else
	{
		lua_pushstring(L, table);
		lua_gettable(L, -2);
		level = 1;
	}
	C_FREE(table, strlen(name) + 1, char);

	return oldtop + level;
}

/* Push a function from the registry into the stack */
void open_lua_functions_registry(s32b domain, s32b ref)
{
	lua_getglobal(L, "__functions_registry");
	lua_pushnumber(L, domain);
	lua_gettable(L, -2);
	lua_pushnumber(L, ref);
	lua_gettable(L, -2);
}
void close_lua_functions_registry()
{
	lua_pop(L, 3);
}

s32b get_lua_functions_registry_domain(cptr name)
{
	s32b ret;

	lua_getglobal(L, "__info_cache");
	lua_pushstring(L, name);
	lua_gettable(L, -2);
	ret = lua_tonumber(L, -1);
	lua_pop(L, 2);
	return ret;
}

/* Call a lua function */
bool call_lua(cptr function, cptr args, cptr ret, ...)
{
	s32b i = 0, nb = 0, nbr = 0;
	s32b oldtop = lua_gettop(L), size, size_start;
	va_list ap;

	va_start(ap, ret);

	/* Push the function */
	if (function != NULL)
		size_start = push_variable(function, oldtop);
	else
		size_start = oldtop - 1;

	/* Push and count the arguments */
	while (args[i])
	{
		switch (args[i++])
		{
		case 'b':
			/*
			 * NOTE: ToME crashes-and-burns if you give va-arg()
			 * "bool" instead of "s32b" (at least on ix86 Linux).
			 */
			tolua_pushbool(L, va_arg(ap, s32b));
			nb++;
			break;
		case 'd':
		case 'l':
			tolua_pushnumber(L, va_arg(ap, s32b));
			nb++;
			break;
		case 's':
			tolua_pushstring(L, va_arg(ap, char*));
			nb++;
			break;
		case 'O':
			tolua_pushusertype(L, (void*)va_arg(ap, object_type*), tolua_tag(L, "object_type"));
			nb++;
			break;
		case 'M':
			tolua_pushusertype(L, (void*)va_arg(ap, monster_type*), tolua_tag(L, "monster_type"));
			nb++;
			break;
		case 'R':
			tolua_pushusertype(L, (void*)va_arg(ap, monster_race*), tolua_tag(L, "monster_race"));
			nb++;
			break;
		case 'F':
			tolua_pushusertype(L, (void*)va_arg(ap, flags_type*), tolua_tag(L, "flags_type"));
			nb++;
			break;
		case 'S':
			tolua_pushusertype(L, (void*)va_arg(ap, store_type*), tolua_tag(L, "store_type"));
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
		}
	}

	/* Count returns */
	nbr = 0;
	for (i = 0; ret[i]; i++)
	{
		switch(ret[i])
		{
		case '(':
		case ')':
		case ',':
		case ' ':
			break;
		default:
			nbr++;
		}
	}

	/* Call the function */
	if (lua_call(L, nb, nbr))
	{
		cmsg_format(TERM_VIOLET, "ERROR in lua_call while calling '%s' from call_lua. Things should start breaking up from now on!", function);
		return FALSE;
	}

	/* Number of returned values, SHOULD be the same as nbr, but I'm paranoid */
	size = lua_gettop(L) - size_start;

	/* Get the returns */
	for (i = 0, nbr = 0; ret[i]; i++, nbr++)
	{
		switch (ret[i])
		{
		case 'b':
			{
				bool *tmp = va_arg(ap, bool*);

				if (lua_isnumber(L, ( -size) + nbr)) *tmp = tolua_getnumber(L, ( -size) + nbr, 0);
				else *tmp = FALSE;
				break;
			}
		case 'd':
		case 'l':
			{
				s32b *tmp = va_arg(ap, s32b*);

				if (lua_isnumber(L, ( -size) + nbr)) *tmp = tolua_getnumber(L, ( -size) + nbr, 0);
				else *tmp = 0;
				break;
			}

		case 's':
			{
				cptr *tmp = va_arg(ap, cptr*);

				if (lua_isstring(L, ( -size) + nbr)) *tmp = tolua_getstring(L, ( -size) + nbr, "");
				else *tmp = NULL;
				break;
			}

		case 'O':
			{
				object_type **tmp = va_arg(ap, object_type**);

				if (tolua_istype(L, ( -size) + nbr, tolua_tag(L, "object_type"), 0))
					*tmp = (object_type*)tolua_getuserdata(L, ( -size) + nbr, NULL);
				else
					*tmp = NULL;
				break;
			}

		case 'M':
			{
				monster_type **tmp = va_arg(ap, monster_type**);

				if (tolua_istype(L, ( -size) + nbr, tolua_tag(L, "monster_type"), 0))
					*tmp = (monster_type*)tolua_getuserdata(L, ( -size) + nbr, NULL);
				else
					*tmp = NULL;
				break;
			}

		case 'R':
			{
				monster_race **tmp = va_arg(ap, monster_race**);

				if (tolua_istype(L, ( -size) + nbr, tolua_tag(L, "monster_race"), 0))
					*tmp = (monster_race*)tolua_getuserdata(L, ( -size) + nbr, NULL);
				else
					*tmp = NULL;
				break;
			}

		case 'F':
			{
				flags_type **tmp = va_arg(ap, flags_type**);

				if (tolua_istype(L, ( -size) + nbr, tolua_tag(L, "flags_type"), 0))
					*tmp = (flags_type*)tolua_getuserdata(L, ( -size) + nbr, NULL);
				else
					*tmp = NULL;
				break;
			}

		case 'S':
			{
				store_type **tmp = va_arg(ap, store_type**);

				if (tolua_istype(L, ( -size) + nbr, tolua_tag(L, "store_type"), 0))
					*tmp = (store_type*)tolua_getuserdata(L, ( -size) + nbr, NULL);
				else
					*tmp = NULL;
				break;
			}

		case '(':
		case ')':
		case ',':
		case ' ':
			nbr--;
			break;

		default:
			cmsg_format(TERM_VIOLET, "ERROR in lua_call while calling '%s' from call_lua: Unknown return type '%c'", function, ret[i]);
			return FALSE;
		}
	}

	lua_settop(L, oldtop);

	va_end(ap);

	return TRUE;
}

/* Grab a lua variable */
bool get_lua_var(cptr name, char type, void *arg)
{
	s32b oldtop = lua_gettop(L);

	/* Push the function */
	push_variable(name, oldtop);

	switch (type)
	{
	case 'b':
		{
			bool *tmp = (bool*)arg;

			*tmp = tolua_getbool(L, ( -1), FALSE);
			break;
		}

	case 'd':
	case 'l':
		{
			s32b *tmp = (s32b*)arg;

			if (lua_isnumber(L, ( -1))) *tmp = tolua_getnumber(L, ( -1), 0);
			else *tmp = 0;
			break;
		}

	case 's':
		{
			cptr *tmp = (cptr*)arg;

			if (lua_isstring(L, ( -1))) *tmp = tolua_getstring(L, ( -1), "");
			else *tmp = NULL;
			break;
		}

	case 'O':
		{
			object_type **tmp = (object_type**)arg;

			if (tolua_istype(L, ( -1), tolua_tag(L, "object_type"), 0))
				*tmp = (object_type*)tolua_getuserdata(L, ( -1), NULL);
			else
				*tmp = NULL;
			break;
		}

	case 'M':
		{
			monster_type **tmp = (monster_type**)arg;

			if (tolua_istype(L, ( -1), tolua_tag(L, "monster_type"), 0))
				*tmp = (monster_type*)tolua_getuserdata(L, ( -1), NULL);
			else
				*tmp = NULL;
			break;
		}
	case 'R':
		{
			monster_race **tmp = (monster_race**)arg;

			if (tolua_istype(L, ( -1), tolua_tag(L, "monster_race"), 0))
				*tmp = (monster_race*)tolua_getuserdata(L, ( -1), NULL);
			else
				*tmp = NULL;
			break;
		}

	case 'F':
		{
			flags_type **tmp = (flags_type**)arg;

			if (tolua_istype(L, ( -1), tolua_tag(L, "flags_type"), 0))
				*tmp = (flags_type*)tolua_getuserdata(L, ( -1), NULL);
			else
				*tmp = NULL;
			break;
		}

	case 'S':
		{
			store_type **tmp = (store_type**)arg;

			if (tolua_istype(L, ( -1), tolua_tag(L, "store_type"), 0))
				*tmp = (store_type*)tolua_getuserdata(L, ( -1), NULL);
			else
				*tmp = NULL;
			break;
		}

	default:
		cmsg_format(TERM_VIOLET, "ERROR in get_lua_var while calling '%s': Unknown return type '%c'", name, type);
		return FALSE;
	}

	lua_settop(L, oldtop);

	return TRUE;
}

/*
 * Can be called from within a debugger to look at the current Lua
 * call stack.
 */
void print_lua_stack(void)
{
	struct lua_Debug dbg;
	s32b              i;

	i = 0;

	printf("\n");
	while (lua_getstack(L, i++, &dbg) == 1)
	{
		lua_getinfo(L, "lnuS", &dbg);
		printf("%s , function %s, line %d\n", dbg.short_src,
		       dbg.name, dbg.currentline);
	}

	printf("\n");
}
