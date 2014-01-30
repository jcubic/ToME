/* File: init1.c */

/* Purpose: Initialization and settings */

#include "angband.h"

#include "lua/lua.h"
#include "tolua.h"
extern lua_State* L;

void esettings_set_str(cptr key, cptr val)
{
	call_lua("esettings.set", "(s,s)", "", key, val);
}

void esettings_set_int(cptr key, s32b val)
{
	call_lua("esettings.set", "(s,d)", "", key, val);
}

cptr esettings_get_str(cptr key, cptr default_val)
{
	cptr ret;
	call_lua("esettings.get", "(s,s)", "s", key, default_val, &ret);
	return ret;
}

s32b esettings_get_int(cptr key, s32b default_val)
{
	s32b ret;
	call_lua("esettings.get", "(s,d)", "d", key, default_val, &ret);
	return ret;
}

void esettings_load()
{
	lua_dofile(L, "/engine/settings.lua");
	call_lua("esettings.load", "()", "");
}

void esettings_save()
{
	call_lua("esettings.save", "()", "");
}
