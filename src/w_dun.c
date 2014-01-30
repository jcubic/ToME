/*
** Lua binding: dungeon
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:36 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_dungeon_open (lua_State* tolua_S);
void tolua_dungeon_close (lua_State* tolua_S);

#include "angband.h"
static wilderness_map* lua_get_wild_map(s32b y, s32b x) { return &wild_map[y][x]; }
static char *lua_get_dungeon_name() { static char buf[20]; return (get_dungeon_name(buf) == TRUE) ? buf : ""; }
bool special_level_get(s32b dungeon, s32b level, bool set) { if (set == -1) return special_lvl[level][dungeon]; else { special_lvl[level][dungeon] = set; return set; } }

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"rule_type");
 tolua_usertype(tolua_S,"border_type");
 tolua_usertype(tolua_S,"vault_type");
 tolua_usertype(tolua_S,"wilderness_type_info");
 tolua_usertype(tolua_S,"feature_type");
 tolua_usertype(tolua_S,"owner_type");
 tolua_usertype(tolua_S,"wilderness_map");
 tolua_usertype(tolua_S,"flags_holder_type");
 tolua_usertype(tolua_S,"store_info_type");
 tolua_usertype(tolua_S,"town_type");
 tolua_usertype(tolua_S,"flags_type");
 tolua_usertype(tolua_S,"store_type");
 tolua_usertype(tolua_S,"dungeon_info_type");
 tolua_usertype(tolua_S,"store_action_type");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* get function: SCREEN_HGT */
static int toluaI_get_dungeon_SCREEN_HGT(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)SCREEN_HGT);
 return 1;
}

/* set function: SCREEN_HGT */
static int toluaI_set_dungeon_SCREEN_HGT(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  SCREEN_HGT = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: SCREEN_WID */
static int toluaI_get_dungeon_SCREEN_WID(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)SCREEN_WID);
 return 1;
}

/* set function: SCREEN_WID */
static int toluaI_set_dungeon_SCREEN_WID(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  SCREEN_WID = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_WAY_LESS */
static int toluaI_get_dungeon_FEAT_WAY_LESS(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_WAY_LESS);
 return 1;
}

/* set function: FEAT_WAY_LESS */
static int toluaI_set_dungeon_FEAT_WAY_LESS(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_WAY_LESS = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_WAY_MORE */
static int toluaI_get_dungeon_FEAT_WAY_MORE(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_WAY_MORE);
 return 1;
}

/* set function: FEAT_WAY_MORE */
static int toluaI_set_dungeon_FEAT_WAY_MORE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_WAY_MORE = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_SHAFT_DOWN */
static int toluaI_get_dungeon_FEAT_SHAFT_DOWN(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_SHAFT_DOWN);
 return 1;
}

/* set function: FEAT_SHAFT_DOWN */
static int toluaI_set_dungeon_FEAT_SHAFT_DOWN(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_SHAFT_DOWN = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_SHAFT_UP */
static int toluaI_get_dungeon_FEAT_SHAFT_UP(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_SHAFT_UP);
 return 1;
}

/* set function: FEAT_SHAFT_UP */
static int toluaI_set_dungeon_FEAT_SHAFT_UP(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_SHAFT_UP = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_LESS */
static int toluaI_get_dungeon_FEAT_LESS(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_LESS);
 return 1;
}

/* set function: FEAT_LESS */
static int toluaI_set_dungeon_FEAT_LESS(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_LESS = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_MORE */
static int toluaI_get_dungeon_FEAT_MORE(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_MORE);
 return 1;
}

/* set function: FEAT_MORE */
static int toluaI_set_dungeon_FEAT_MORE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_MORE = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_DOOR */
static int toluaI_get_dungeon_FEAT_DOOR(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_DOOR);
 return 1;
}

/* set function: FEAT_DOOR */
static int toluaI_set_dungeon_FEAT_DOOR(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_DOOR = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_OPEN */
static int toluaI_get_dungeon_FEAT_OPEN(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_OPEN);
 return 1;
}

/* set function: FEAT_OPEN */
static int toluaI_set_dungeon_FEAT_OPEN(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_OPEN = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_WALL_INNER */
static int toluaI_get_dungeon_FEAT_WALL_INNER(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_WALL_INNER);
 return 1;
}

/* set function: FEAT_WALL_INNER */
static int toluaI_set_dungeon_FEAT_WALL_INNER(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_WALL_INNER = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_WALL_OUTER */
static int toluaI_get_dungeon_FEAT_WALL_OUTER(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_WALL_OUTER);
 return 1;
}

/* set function: FEAT_WALL_OUTER */
static int toluaI_set_dungeon_FEAT_WALL_OUTER(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_WALL_OUTER = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_WALL_EXTRA */
static int toluaI_get_dungeon_FEAT_WALL_EXTRA(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_WALL_EXTRA);
 return 1;
}

/* set function: FEAT_WALL_EXTRA */
static int toluaI_set_dungeon_FEAT_WALL_EXTRA(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_WALL_EXTRA = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_WALL_SOLID */
static int toluaI_get_dungeon_FEAT_WALL_SOLID(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_WALL_SOLID);
 return 1;
}

/* set function: FEAT_WALL_SOLID */
static int toluaI_set_dungeon_FEAT_WALL_SOLID(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_WALL_SOLID = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_PERM_INNER */
static int toluaI_get_dungeon_FEAT_PERM_INNER(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_PERM_INNER);
 return 1;
}

/* set function: FEAT_PERM_INNER */
static int toluaI_set_dungeon_FEAT_PERM_INNER(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_PERM_INNER = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_PERM_OUTER */
static int toluaI_get_dungeon_FEAT_PERM_OUTER(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_PERM_OUTER);
 return 1;
}

/* set function: FEAT_PERM_OUTER */
static int toluaI_set_dungeon_FEAT_PERM_OUTER(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_PERM_OUTER = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_PERM_SOLID */
static int toluaI_get_dungeon_FEAT_PERM_SOLID(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_PERM_SOLID);
 return 1;
}

/* set function: FEAT_PERM_SOLID */
static int toluaI_set_dungeon_FEAT_PERM_SOLID(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_PERM_SOLID = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_WORLD_BORDER */
static int toluaI_get_dungeon_FEAT_WORLD_BORDER(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_WORLD_BORDER);
 return 1;
}

/* set function: FEAT_WORLD_BORDER */
static int toluaI_set_dungeon_FEAT_WORLD_BORDER(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_WORLD_BORDER = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_RUBBLE */
static int toluaI_get_dungeon_FEAT_RUBBLE(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_RUBBLE);
 return 1;
}

/* set function: FEAT_RUBBLE */
static int toluaI_set_dungeon_FEAT_RUBBLE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_RUBBLE = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_SECRET */
static int toluaI_get_dungeon_FEAT_SECRET(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_SECRET);
 return 1;
}

/* set function: FEAT_SECRET */
static int toluaI_set_dungeon_FEAT_SECRET(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_SECRET = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_BROKEN */
static int toluaI_get_dungeon_FEAT_BROKEN(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_BROKEN);
 return 1;
}

/* set function: FEAT_BROKEN */
static int toluaI_set_dungeon_FEAT_BROKEN(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_BROKEN = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_DOOR_LOCKED_HEAD */
static int toluaI_get_dungeon_FEAT_DOOR_LOCKED_HEAD(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_DOOR_LOCKED_HEAD);
 return 1;
}

/* set function: FEAT_DOOR_LOCKED_HEAD */
static int toluaI_set_dungeon_FEAT_DOOR_LOCKED_HEAD(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_DOOR_LOCKED_HEAD = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_DOOR_LOCKED_TAIL */
static int toluaI_get_dungeon_FEAT_DOOR_LOCKED_TAIL(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_DOOR_LOCKED_TAIL);
 return 1;
}

/* set function: FEAT_DOOR_LOCKED_TAIL */
static int toluaI_set_dungeon_FEAT_DOOR_LOCKED_TAIL(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_DOOR_LOCKED_TAIL = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_DOOR_JAMMED_HEAD */
static int toluaI_get_dungeon_FEAT_DOOR_JAMMED_HEAD(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_DOOR_JAMMED_HEAD);
 return 1;
}

/* set function: FEAT_DOOR_JAMMED_HEAD */
static int toluaI_set_dungeon_FEAT_DOOR_JAMMED_HEAD(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_DOOR_JAMMED_HEAD = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_DOOR_JAMMED_TAIL */
static int toluaI_get_dungeon_FEAT_DOOR_JAMMED_TAIL(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_DOOR_JAMMED_TAIL);
 return 1;
}

/* set function: FEAT_DOOR_JAMMED_TAIL */
static int toluaI_set_dungeon_FEAT_DOOR_JAMMED_TAIL(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_DOOR_JAMMED_TAIL = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: FEAT_SHOP */
static int toluaI_get_dungeon_FEAT_SHOP(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)FEAT_SHOP);
 return 1;
}

/* set function: FEAT_SHOP */
static int toluaI_set_dungeon_FEAT_SHOP(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  FEAT_SHOP = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: dungeon_flags */
static int toluaI_get_dungeon_level_flags(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)dungeon_flags,tolua_tag(tolua_S,"flags_holder_type"));
 return 1;
}

/* set function: dungeon_flags */
static int toluaI_set_dungeon_level_flags(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_holder_type"),0))
 TOLUA_ERR_ASSIGN;
  dungeon_flags = ((flags_holder_type*)  tolua_getusertype(tolua_S,1,0));
 return 0;
}

/* get function: north of class  border_type */
static int toluaI_get_dungeon_border_type_north(lua_State* tolua_S)
{
 int toluaI_index;
  border_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (border_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_WID)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->north[toluaI_index]);
 return 1;
}

/* set function: north of class  border_type */
static int toluaI_set_dungeon_border_type_north(lua_State* tolua_S)
{
 int toluaI_index;
  border_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (border_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_WID)
 tolua_error(tolua_S,"array indexing out of range.");
  self->north[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: south of class  border_type */
static int toluaI_get_dungeon_border_type_south(lua_State* tolua_S)
{
 int toluaI_index;
  border_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (border_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_WID)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->south[toluaI_index]);
 return 1;
}

/* set function: south of class  border_type */
static int toluaI_set_dungeon_border_type_south(lua_State* tolua_S)
{
 int toluaI_index;
  border_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (border_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_WID)
 tolua_error(tolua_S,"array indexing out of range.");
  self->south[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: east of class  border_type */
static int toluaI_get_dungeon_border_type_east(lua_State* tolua_S)
{
 int toluaI_index;
  border_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (border_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_HGT)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->east[toluaI_index]);
 return 1;
}

/* set function: east of class  border_type */
static int toluaI_set_dungeon_border_type_east(lua_State* tolua_S)
{
 int toluaI_index;
  border_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (border_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_HGT)
 tolua_error(tolua_S,"array indexing out of range.");
  self->east[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: west of class  border_type */
static int toluaI_get_dungeon_border_type_west(lua_State* tolua_S)
{
 int toluaI_index;
  border_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (border_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_HGT)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->west[toluaI_index]);
 return 1;
}

/* set function: west of class  border_type */
static int toluaI_set_dungeon_border_type_west(lua_State* tolua_S)
{
 int toluaI_index;
  border_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (border_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_HGT)
 tolua_error(tolua_S,"array indexing out of range.");
  self->west[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: north_west of class  border_type */
static int toluaI_get_dungeon_border_type_north_west(lua_State* tolua_S)
{
  border_type* self = (border_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->north_west);
 return 1;
}

/* set function: north_west of class  border_type */
static int toluaI_set_dungeon_border_type_north_west(lua_State* tolua_S)
{
  border_type* self = (border_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->north_west = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: north_east of class  border_type */
static int toluaI_get_dungeon_border_type_north_east(lua_State* tolua_S)
{
  border_type* self = (border_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->north_east);
 return 1;
}

/* set function: north_east of class  border_type */
static int toluaI_set_dungeon_border_type_north_east(lua_State* tolua_S)
{
  border_type* self = (border_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->north_east = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: south_west of class  border_type */
static int toluaI_get_dungeon_border_type_south_west(lua_State* tolua_S)
{
  border_type* self = (border_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->south_west);
 return 1;
}

/* set function: south_west of class  border_type */
static int toluaI_set_dungeon_border_type_south_west(lua_State* tolua_S)
{
  border_type* self = (border_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->south_west = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: south_east of class  border_type */
static int toluaI_get_dungeon_border_type_south_east(lua_State* tolua_S)
{
  border_type* self = (border_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->south_east);
 return 1;
}

/* set function: south_east of class  border_type */
static int toluaI_set_dungeon_border_type_south_east(lua_State* tolua_S)
{
  border_type* self = (border_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->south_east = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_name(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_name(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: text of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_text(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->text);
 return 1;
}

/* set function: text of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_text(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->text = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: entrance of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_entrance(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->entrance);
 return 1;
}

/* set function: entrance of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_entrance(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->entrance = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: road of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_road(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->road);
 return 1;
}

/* set function: road of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_road(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->road = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: level of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_level(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->level);
 return 1;
}

/* set function: level of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_level(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->level = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_flags(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_flags(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: feat of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_feat(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->feat);
 return 1;
}

/* set function: feat of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_feat(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->feat = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_char of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_d_char(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_char);
 return 1;
}

/* set function: d_char of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_d_char(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_char = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: terrain_idx of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_terrain_idx(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->terrain_idx);
 return 1;
}

/* set function: terrain_idx of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_terrain_idx(lua_State* tolua_S)
{
  wilderness_type_info* self = (wilderness_type_info*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->terrain_idx = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: terrain of class  wilderness_type_info */
static int toluaI_get_dungeon_wilderness_type_info_terrain(lua_State* tolua_S)
{
 int toluaI_index;
  wilderness_type_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (wilderness_type_info*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_WILD_TERRAIN)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->terrain[toluaI_index]);
 return 1;
}

/* set function: terrain of class  wilderness_type_info */
static int toluaI_set_dungeon_wilderness_type_info_terrain(lua_State* tolua_S)
{
 int toluaI_index;
  wilderness_type_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (wilderness_type_info*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_WILD_TERRAIN)
 tolua_error(tolua_S,"array indexing out of range.");
  self->terrain[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: feat of class  wilderness_map */
static int toluaI_get_dungeon_wilderness_map_feat(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->feat);
 return 1;
}

/* set function: feat of class  wilderness_map */
static int toluaI_set_dungeon_wilderness_map_feat(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->feat = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mimic of class  wilderness_map */
static int toluaI_get_dungeon_wilderness_map_mimic(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mimic);
 return 1;
}

/* set function: mimic of class  wilderness_map */
static int toluaI_set_dungeon_wilderness_map_mimic(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mimic = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: seed of class  wilderness_map */
static int toluaI_get_dungeon_wilderness_map_seed(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->seed);
 return 1;
}

/* set function: seed of class  wilderness_map */
static int toluaI_set_dungeon_wilderness_map_seed(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->seed = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: entrance of class  wilderness_map */
static int toluaI_get_dungeon_wilderness_map_entrance(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->entrance);
 return 1;
}

/* set function: entrance of class  wilderness_map */
static int toluaI_set_dungeon_wilderness_map_entrance(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->entrance = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: known of class  wilderness_map */
static int toluaI_get_dungeon_wilderness_map_known(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->known);
 return 1;
}

/* set function: known of class  wilderness_map */
static int toluaI_set_dungeon_wilderness_map_known(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->known = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  wilderness_map */
static int toluaI_get_dungeon_wilderness_map_flags(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  wilderness_map */
static int toluaI_set_dungeon_wilderness_map_flags(lua_State* tolua_S)
{
  wilderness_map* self = (wilderness_map*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: max_st_idx */
static int toluaI_get_dungeon_max_st_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_st_idx);
 return 1;
}

/* set function: max_st_idx */
static int toluaI_set_dungeon_max_st_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_st_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: name of class  town_type */
static int toluaI_get_dungeon_town_type_name(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  town_type */
static int toluaI_set_dungeon_town_type_name(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: seed of class  town_type */
static int toluaI_get_dungeon_town_type_seed(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->seed);
 return 1;
}

/* set function: seed of class  town_type */
static int toluaI_set_dungeon_town_type_seed(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->seed = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: stores of class  town_type */
static int toluaI_get_dungeon_town_type_stores(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->stores,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: stores of class  town_type */
static int toluaI_set_dungeon_town_type_stores(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->stores = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: numstores of class  town_type */
static int toluaI_get_dungeon_town_type_numstores(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->numstores);
 return 1;
}

/* set function: numstores of class  town_type */
static int toluaI_set_dungeon_town_type_numstores(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->numstores = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  town_type */
static int toluaI_get_dungeon_town_type_flags(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->flags);
 return 1;
}

/* set function: flags of class  town_type */
static int toluaI_set_dungeon_town_type_flags(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->flags = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: stocked of class  town_type */
static int toluaI_get_dungeon_town_type_stocked(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->stocked);
 return 1;
}

/* set function: stocked of class  town_type */
static int toluaI_set_dungeon_town_type_stocked(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->stocked = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: destroyed of class  town_type */
static int toluaI_get_dungeon_town_type_destroyed(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->destroyed);
 return 1;
}

/* set function: destroyed of class  town_type */
static int toluaI_set_dungeon_town_type_destroyed(lua_State* tolua_S)
{
  town_type* self = (town_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->destroyed = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: max_towns */
static int toluaI_get_dungeon_max_towns(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_towns);
 return 1;
}

/* set function: max_towns */
static int toluaI_set_dungeon_max_towns(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_towns = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_real_towns */
static int toluaI_get_dungeon_max_real_towns(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_real_towns);
 return 1;
}

/* set function: max_real_towns */
static int toluaI_set_dungeon_max_real_towns(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_real_towns = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: town_info */
static int toluaI_get_dungeon_town_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_towns)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&town_info[toluaI_index],tolua_tag(tolua_S,"town_type"));
 return 1;
}

/* set function: town_info */
static int toluaI_set_dungeon_town_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_towns)
 tolua_error(tolua_S,"array indexing out of range.");
  town_info[toluaI_index] = *((town_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: mode of class  rule_type */
static int toluaI_get_dungeon_rule_type_mode(lua_State* tolua_S)
{
  rule_type* self = (rule_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mode);
 return 1;
}

/* set function: mode of class  rule_type */
static int toluaI_set_dungeon_rule_type_mode(lua_State* tolua_S)
{
  rule_type* self = (rule_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mode = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: percent of class  rule_type */
static int toluaI_get_dungeon_rule_type_percent(lua_State* tolua_S)
{
  rule_type* self = (rule_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->percent);
 return 1;
}

/* set function: percent of class  rule_type */
static int toluaI_set_dungeon_rule_type_percent(lua_State* tolua_S)
{
  rule_type* self = (rule_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->percent = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  rule_type */
static int toluaI_get_dungeon_rule_type_flags(lua_State* tolua_S)
{
  rule_type* self = (rule_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  rule_type */
static int toluaI_set_dungeon_rule_type_flags(lua_State* tolua_S)
{
  rule_type* self = (rule_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: r_char of class  rule_type */
static int toluaI_get_dungeon_rule_type_r_char(lua_State* tolua_S)
{
 int toluaI_index;
  rule_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (rule_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=5)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->r_char[toluaI_index]);
 return 1;
}

/* set function: r_char of class  rule_type */
static int toluaI_set_dungeon_rule_type_r_char(lua_State* tolua_S)
{
 int toluaI_index;
  rule_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (rule_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=5)
 tolua_error(tolua_S,"array indexing out of range.");
  self->r_char[toluaI_index] = ((char)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: name of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_name(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_name(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: text of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_text(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->text);
 return 1;
}

/* set function: text of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_text(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->text = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: short_name of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_short_name(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=3)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->short_name[toluaI_index]);
 return 1;
}

/* set function: short_name of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_short_name(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=3)
 tolua_error(tolua_S,"array indexing out of range.");
  self->short_name[toluaI_index] = ((char)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: floor1 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_floor1(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->floor1);
 return 1;
}

/* set function: floor1 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_floor1(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->floor1 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: floor_percent1 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_floor_percent1(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->floor_percent1[toluaI_index]);
 return 1;
}

/* set function: floor_percent1 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_floor_percent1(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
  self->floor_percent1[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: floor2 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_floor2(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->floor2);
 return 1;
}

/* set function: floor2 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_floor2(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->floor2 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: floor_percent2 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_floor_percent2(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->floor_percent2[toluaI_index]);
 return 1;
}

/* set function: floor_percent2 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_floor_percent2(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
  self->floor_percent2[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: floor3 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_floor3(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->floor3);
 return 1;
}

/* set function: floor3 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_floor3(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->floor3 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: floor_percent3 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_floor_percent3(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->floor_percent3[toluaI_index]);
 return 1;
}

/* set function: floor_percent3 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_floor_percent3(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
  self->floor_percent3[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: outer_wall of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_outer_wall(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->outer_wall);
 return 1;
}

/* set function: outer_wall of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_outer_wall(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->outer_wall = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: inner_wall of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_inner_wall(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->inner_wall);
 return 1;
}

/* set function: inner_wall of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_inner_wall(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->inner_wall = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: fill_type1 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_fill_type1(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->fill_type1);
 return 1;
}

/* set function: fill_type1 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_fill_type1(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->fill_type1 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: fill_percent1 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_fill_percent1(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->fill_percent1[toluaI_index]);
 return 1;
}

/* set function: fill_percent1 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_fill_percent1(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
  self->fill_percent1[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: fill_type2 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_fill_type2(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->fill_type2);
 return 1;
}

/* set function: fill_type2 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_fill_type2(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->fill_type2 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: fill_percent2 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_fill_percent2(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->fill_percent2[toluaI_index]);
 return 1;
}

/* set function: fill_percent2 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_fill_percent2(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
  self->fill_percent2[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: fill_type3 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_fill_type3(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->fill_type3);
 return 1;
}

/* set function: fill_type3 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_fill_type3(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->fill_type3 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: fill_percent3 of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_fill_percent3(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->fill_percent3[toluaI_index]);
 return 1;
}

/* set function: fill_percent3 of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_fill_percent3(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
  self->fill_percent3[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: fill_method of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_fill_method(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->fill_method);
 return 1;
}

/* set function: fill_method of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_fill_method(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->fill_method = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mindepth of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_mindepth(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mindepth);
 return 1;
}

/* set function: mindepth of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_mindepth(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mindepth = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: maxdepth of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_maxdepth(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->maxdepth);
 return 1;
}

/* set function: maxdepth of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_maxdepth(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->maxdepth = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: next of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_next(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->next);
 return 1;
}

/* set function: next of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_next(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->next = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: min_plev of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_min_plev(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->min_plev);
 return 1;
}

/* set function: min_plev of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_min_plev(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->min_plev = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: min_m_alloc_level of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_min_m_alloc_level(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->min_m_alloc_level);
 return 1;
}

/* set function: min_m_alloc_level of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_min_m_alloc_level(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->min_m_alloc_level = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_m_alloc_chance of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_max_m_alloc_chance(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_m_alloc_chance);
 return 1;
}

/* set function: max_m_alloc_chance of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_max_m_alloc_chance(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_m_alloc_chance = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_flags(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_flags(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: size_x of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_size_x(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->size_x);
 return 1;
}

/* set function: size_x of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_size_x(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->size_x = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: size_y of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_size_y(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->size_y);
 return 1;
}

/* set function: size_y of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_size_y(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->size_y = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: rule_percents of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_rule_percents(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=100)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->rule_percents[toluaI_index]);
 return 1;
}

/* set function: rule_percents of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_rule_percents(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=100)
 tolua_error(tolua_S,"array indexing out of range.");
  self->rule_percents[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: rules of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_rules(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=5)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&self->rules[toluaI_index],tolua_tag(tolua_S,"rule_type"));
 return 1;
}

/* set function: rules of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_rules(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=5)
 tolua_error(tolua_S,"array indexing out of range.");
  self->rules[toluaI_index] = *((rule_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: ix of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_ix(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ix);
 return 1;
}

/* set function: ix of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_ix(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ix = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: iy of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_iy(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->iy);
 return 1;
}

/* set function: iy of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_iy(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->iy = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ox of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_ox(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ox);
 return 1;
}

/* set function: ox of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_ox(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ox = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: oy of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_oy(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->oy);
 return 1;
}

/* set function: oy of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_oy(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->oy = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_dice of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_d_dice(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->d_dice[toluaI_index]);
 return 1;
}

/* set function: d_dice of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_d_dice(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->d_dice[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: d_side of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_d_side(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->d_side[toluaI_index]);
 return 1;
}

/* set function: d_side of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_d_side(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->d_side[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: d_frequency of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_d_frequency(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->d_frequency[toluaI_index]);
 return 1;
}

/* set function: d_frequency of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_d_frequency(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->d_frequency[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: d_type of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_d_type(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->d_type[toluaI_index]);
 return 1;
}

/* set function: d_type of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_d_type(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->d_type[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: t_idx of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_t_idx(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=TOWN_DUNGEON)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->t_idx[toluaI_index]);
 return 1;
}

/* set function: t_idx of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_t_idx(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=TOWN_DUNGEON)
 tolua_error(tolua_S,"array indexing out of range.");
  self->t_idx[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: t_level of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_t_level(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=TOWN_DUNGEON)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->t_level[toluaI_index]);
 return 1;
}

/* set function: t_level of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_t_level(lua_State* tolua_S)
{
 int toluaI_index;
  dungeon_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (dungeon_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=TOWN_DUNGEON)
 tolua_error(tolua_S,"array indexing out of range.");
  self->t_level[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: t_num of class  dungeon_info_type */
static int toluaI_get_dungeon_dungeon_info_type_t_num(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->t_num);
 return 1;
}

/* set function: t_num of class  dungeon_info_type */
static int toluaI_set_dungeon_dungeon_info_type_t_num(lua_State* tolua_S)
{
  dungeon_info_type* self = (dungeon_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->t_num = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_d_idx */
static int toluaI_get_dungeon_max_d_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_d_idx);
 return 1;
}

/* set function: max_d_idx */
static int toluaI_set_dungeon_max_d_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_d_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: d_info */
static int toluaI_get_dungeon_d_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_d_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&d_info[toluaI_index],tolua_tag(tolua_S,"dungeon_info_type"));
 return 1;
}

/* set function: d_info */
static int toluaI_set_dungeon_d_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_d_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  d_info[toluaI_index] = *((dungeon_info_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: max_wild_x */
static int toluaI_get_dungeon_max_wild_x(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_wild_x);
 return 1;
}

/* set function: max_wild_x */
static int toluaI_set_dungeon_max_wild_x(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_wild_x = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_wild_y */
static int toluaI_get_dungeon_max_wild_y(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_wild_y);
 return 1;
}

/* set function: max_wild_y */
static int toluaI_set_dungeon_max_wild_y(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_wild_y = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_wf_idx */
static int toluaI_get_dungeon_max_wf_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_wf_idx);
 return 1;
}

/* set function: max_wf_idx */
static int toluaI_set_dungeon_max_wf_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_wf_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: wf_info */
static int toluaI_get_dungeon_wf_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_wf_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&wf_info[toluaI_index],tolua_tag(tolua_S,"wilderness_type_info"));
 return 1;
}

/* set function: wf_info */
static int toluaI_set_dungeon_wf_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_wf_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  wf_info[toluaI_index] = *((wilderness_type_info*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: dungeon_type */
static int toluaI_get_dungeon_current_dungeon_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)dungeon_type);
 return 1;
}

/* set function: dungeon_type */
static int toluaI_set_dungeon_current_dungeon_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  dungeon_type = ((byte)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: lua_get_wild_map */
static int toluaI_dungeon_wild_map00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  wilderness_map* toluaI_ret = (wilderness_map*)  lua_get_wild_map(y,x);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"wilderness_map"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wild_map'.");
 return 0;
}

/* function: place_floor */
static int toluaI_dungeon_place_floor00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  place_floor(y,x);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_floor'.");
 return 0;
}

/* function: place_filler */
static int toluaI_dungeon_place_filler00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  place_filler(y,x);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_filler'.");
 return 0;
}

/* function: add_scripted_generator */
static int toluaI_dungeon_add_scripted_generator00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  cptr name = ((cptr)  tolua_getstring(tolua_S,1,0));
  bool stairs = ((bool)  tolua_getbool(tolua_S,2,0));
  bool monsters = ((bool)  tolua_getbool(tolua_S,3,0));
  bool objects = ((bool)  tolua_getbool(tolua_S,4,0));
  bool miscs = ((bool)  tolua_getbool(tolua_S,5,0));
 {
  add_scripted_generator(name,stairs,monsters,objects,miscs);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_scripted_generator'.");
 return 0;
}

/* function: new_player_spot */
static int toluaI_dungeon_new_player_spot00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b branch = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  new_player_spot(branch);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new_player_spot'.");
 return 0;
}

/* function: get_level_desc */
static int toluaI_dungeon_get_level_desc00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  char* buf = ((char*)  tolua_getstring(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  get_level_desc(buf);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_level_desc'.");
 return 0;
}

/* function: get_level_flags */
static int toluaI_dungeon_get_level_flags00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  get_level_flags();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_level_flags'.");
 return 0;
}

/* function: lua_get_dungeon_name */
static int toluaI_dungeon_get_dungeon_name00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  char* toluaI_ret = (char*)  lua_get_dungeon_name();
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_dungeon_name'.");
 return 0;
}

/* function: get_dungeon_special */
static int toluaI_dungeon_get_dungeon_special00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  char* buf = ((char*)  tolua_getstring(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  get_dungeon_special(buf);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_dungeon_special'.");
 return 0;
}

/* function: get_branch */
static int toluaI_dungeon_get_branch00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  s32b toluaI_ret = (s32b)  get_branch();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_branch'.");
 return 0;
}

/* function: get_fbranch */
static int toluaI_dungeon_get_fbranch00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  s32b toluaI_ret = (s32b)  get_fbranch();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_fbranch'.");
 return 0;
}

/* function: get_flevel */
static int toluaI_dungeon_get_flevel00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  s32b toluaI_ret = (s32b)  get_flevel();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_flevel'.");
 return 0;
}

/* function: prt_map */
static int toluaI_dungeon_prt_map00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  prt_map();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'prt_map'.");
 return 0;
}

/* function: verify_panel */
static int toluaI_dungeon_verify_panel00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  verify_panel();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'verify_panel'.");
 return 0;
}

/* function: change_panel */
static int toluaI_dungeon_change_panel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b dy = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dx = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  change_panel(dy,dx);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'change_panel'.");
 return 0;
}

/* function: player_can_enter */
static int toluaI_dungeon_player_can_enter00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  byte feature = ((byte)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  player_can_enter(feature);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'player_can_enter'.");
 return 0;
}

/* get function: max_dlv */
static int toluaI_get_dungeon_max_dlv(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_d_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)max_dlv[toluaI_index]);
 return 1;
}

/* set function: max_dlv */
static int toluaI_set_dungeon_max_dlv(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_d_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  max_dlv[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: wildc2i */
static int toluaI_get_dungeon_wildc2i(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=256)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)wildc2i[toluaI_index]);
 return 1;
}

/* set function: wildc2i */
static int toluaI_set_dungeon_wildc2i(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=256)
 tolua_error(tolua_S,"array indexing out of range.");
  wildc2i[toluaI_index] = ((s32b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* function: reveal_wilderness_around_player */
static int toluaI_dungeon_reveal_wilderness_around_player00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b h = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b w = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  reveal_wilderness_around_player(y,x,h,w);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reveal_wilderness_around_player'.");
 return 0;
}

/* function: special_level_get */
static int toluaI_dungeon_special_lvl00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  s32b dungeon = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b level = ((s32b)  tolua_getnumber(tolua_S,2,0));
  bool set = ((bool)  tolua_getbool(tolua_S,3,-1));
 {
  bool toluaI_ret = (bool)  special_level_get(dungeon,level,set);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'special_lvl'.");
 return 0;
}

/* get function: generate_special_feeling */
static int toluaI_get_dungeon_generate_special_feeling(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)generate_special_feeling);
 return 1;
}

/* set function: generate_special_feeling */
static int toluaI_set_dungeon_generate_special_feeling(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  generate_special_feeling = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: store_type_new */
static int toluaI_dungeon_store_type_new00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  store_type* toluaI_ret = (store_type*)  store_type_new();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"store_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'store_type_new'.");
 return 0;
}

/* function: store_type_del */
static int toluaI_dungeon_store_type_del00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"store_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  store_type* store = ((store_type*)  tolua_getusertype(tolua_S,1,0));
 {
  store_type_del(store);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'store_type_del'.");
 return 0;
}

/* function: store_type_dup */
static int toluaI_dungeon_store_type_dup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"store_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  store_type* store = ((store_type*)  tolua_getusertype(tolua_S,1,0));
 {
  store_type* toluaI_ret = (store_type*)  store_type_dup(store);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"store_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'store_type_dup'.");
 return 0;
}

/* get function: st_idx of class  store_type */
static int toluaI_get_dungeon_store_type_st_idx(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->st_idx);
 return 1;
}

/* set function: st_idx of class  store_type */
static int toluaI_set_dungeon_store_type_st_idx(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->st_idx = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: owner of class  store_type */
static int toluaI_get_dungeon_store_type_owner(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->owner);
 return 1;
}

/* set function: owner of class  store_type */
static int toluaI_set_dungeon_store_type_owner(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->owner = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: insult_cur of class  store_type */
static int toluaI_get_dungeon_store_type_insult_cur(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->insult_cur);
 return 1;
}

/* set function: insult_cur of class  store_type */
static int toluaI_set_dungeon_store_type_insult_cur(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->insult_cur = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: good_buy of class  store_type */
static int toluaI_get_dungeon_store_type_good_buy(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->good_buy);
 return 1;
}

/* set function: good_buy of class  store_type */
static int toluaI_set_dungeon_store_type_good_buy(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->good_buy = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: bad_buy of class  store_type */
static int toluaI_get_dungeon_store_type_bad_buy(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->bad_buy);
 return 1;
}

/* set function: bad_buy of class  store_type */
static int toluaI_set_dungeon_store_type_bad_buy(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->bad_buy = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: store_open of class  store_type */
static int toluaI_get_dungeon_store_type_store_open(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->store_open);
 return 1;
}

/* set function: store_open of class  store_type */
static int toluaI_set_dungeon_store_type_store_open(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->store_open = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: last_visit of class  store_type */
static int toluaI_get_dungeon_store_type_last_visit(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->last_visit);
 return 1;
}

/* set function: last_visit of class  store_type */
static int toluaI_set_dungeon_store_type_last_visit(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->last_visit = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: stock of class  store_type */
static int toluaI_get_dungeon_store_type_stock(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->stock,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: stock of class  store_type */
static int toluaI_set_dungeon_store_type_stock(lua_State* tolua_S)
{
  store_type* self = (store_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->stock = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: name of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_name(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_name(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: table of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_table(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->table,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: table of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_table(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->table = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: max_obj of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_max_obj(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_obj);
 return 1;
}

/* set function: max_obj of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_max_obj(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_obj = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: buy of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_buy(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->buy);
 return 1;
}

/* set function: buy of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_buy(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->buy = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: store_maker of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_store_maker(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->store_maker);
 return 1;
}

/* set function: store_maker of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_store_maker(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->store_maker = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: store_make_num of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_store_make_num(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->store_make_num);
 return 1;
}

/* set function: store_make_num of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_store_make_num(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->store_make_num = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: owners of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_owners(lua_State* tolua_S)
{
 int toluaI_index;
  store_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (store_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->owners[toluaI_index]);
 return 1;
}

/* set function: owners of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_owners(lua_State* tolua_S)
{
 int toluaI_index;
  store_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (store_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->owners[toluaI_index] = ((u16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: actions of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_actions(lua_State* tolua_S)
{
 int toluaI_index;
  store_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (store_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=6)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->actions[toluaI_index]);
 return 1;
}

/* set function: actions of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_actions(lua_State* tolua_S)
{
 int toluaI_index;
  store_info_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (store_info_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=6)
 tolua_error(tolua_S,"array indexing out of range.");
  self->actions[toluaI_index] = ((u16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: d_attr of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_d_attr(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_attr);
 return 1;
}

/* set function: d_attr of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_d_attr(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_char of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_d_char(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_char);
 return 1;
}

/* set function: d_char of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_d_char(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_attr of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_x_attr(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_attr);
 return 1;
}

/* set function: x_attr of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_x_attr(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_char of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_x_char(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_char);
 return 1;
}

/* set function: x_char of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_x_char(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  store_info_type */
static int toluaI_get_dungeon_store_info_type_flags(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  store_info_type */
static int toluaI_set_dungeon_store_info_type_flags(lua_State* tolua_S)
{
  store_info_type* self = (store_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: st_info */
static int toluaI_get_dungeon_st_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_st_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&st_info[toluaI_index],tolua_tag(tolua_S,"store_info_type"));
 return 1;
}

/* set function: st_info */
static int toluaI_set_dungeon_st_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_st_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  st_info[toluaI_index] = *((store_info_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: name of class  store_action_type */
static int toluaI_get_dungeon_store_action_type_name(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  store_action_type */
static int toluaI_set_dungeon_store_action_type_name(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: costs of class  store_action_type */
static int toluaI_get_dungeon_store_action_type_costs(lua_State* tolua_S)
{
 int toluaI_index;
  store_action_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (store_action_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=3)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->costs[toluaI_index]);
 return 1;
}

/* set function: costs of class  store_action_type */
static int toluaI_set_dungeon_store_action_type_costs(lua_State* tolua_S)
{
 int toluaI_index;
  store_action_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (store_action_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=3)
 tolua_error(tolua_S,"array indexing out of range.");
  self->costs[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: letter of class  store_action_type */
static int toluaI_get_dungeon_store_action_type_letter(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->letter);
 return 1;
}

/* set function: letter of class  store_action_type */
static int toluaI_set_dungeon_store_action_type_letter(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->letter = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: letter_aux of class  store_action_type */
static int toluaI_get_dungeon_store_action_type_letter_aux(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->letter_aux);
 return 1;
}

/* set function: letter_aux of class  store_action_type */
static int toluaI_set_dungeon_store_action_type_letter_aux(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->letter_aux = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: action of class  store_action_type */
static int toluaI_get_dungeon_store_action_type_action(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->action);
 return 1;
}

/* set function: action of class  store_action_type */
static int toluaI_set_dungeon_store_action_type_action(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->action = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: action_restr of class  store_action_type */
static int toluaI_get_dungeon_store_action_type_action_restr(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->action_restr);
 return 1;
}

/* set function: action_restr of class  store_action_type */
static int toluaI_set_dungeon_store_action_type_action_restr(lua_State* tolua_S)
{
  store_action_type* self = (store_action_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->action_restr = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ba_info */
static int toluaI_get_dungeon_ba_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_ba_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&ba_info[toluaI_index],tolua_tag(tolua_S,"store_action_type"));
 return 1;
}

/* set function: ba_info */
static int toluaI_set_dungeon_ba_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_ba_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  ba_info[toluaI_index] = *((store_action_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: name of class  owner_type */
static int toluaI_get_dungeon_owner_type_name(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  owner_type */
static int toluaI_set_dungeon_owner_type_name(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: max_cost of class  owner_type */
static int toluaI_get_dungeon_owner_type_max_cost(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_cost);
 return 1;
}

/* set function: max_cost of class  owner_type */
static int toluaI_set_dungeon_owner_type_max_cost(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_cost = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_inflate of class  owner_type */
static int toluaI_get_dungeon_owner_type_max_inflate(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_inflate);
 return 1;
}

/* set function: max_inflate of class  owner_type */
static int toluaI_set_dungeon_owner_type_max_inflate(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_inflate = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: min_inflate of class  owner_type */
static int toluaI_get_dungeon_owner_type_min_inflate(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->min_inflate);
 return 1;
}

/* set function: min_inflate of class  owner_type */
static int toluaI_set_dungeon_owner_type_min_inflate(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->min_inflate = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: haggle_per of class  owner_type */
static int toluaI_get_dungeon_owner_type_haggle_per(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->haggle_per);
 return 1;
}

/* set function: haggle_per of class  owner_type */
static int toluaI_set_dungeon_owner_type_haggle_per(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->haggle_per = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: insult_max of class  owner_type */
static int toluaI_get_dungeon_owner_type_insult_max(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->insult_max);
 return 1;
}

/* set function: insult_max of class  owner_type */
static int toluaI_set_dungeon_owner_type_insult_max(lua_State* tolua_S)
{
  owner_type* self = (owner_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->insult_max = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: races of class  owner_type */
static int toluaI_get_dungeon_owner_type_races(lua_State* tolua_S)
{
 int toluaI_index;
  owner_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (owner_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&self->races[toluaI_index],tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: races of class  owner_type */
static int toluaI_set_dungeon_owner_type_races(lua_State* tolua_S)
{
 int toluaI_index;
  owner_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (owner_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
  self->races[toluaI_index] = *((flags_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: classes of class  owner_type */
static int toluaI_get_dungeon_owner_type_classes(lua_State* tolua_S)
{
 int toluaI_index;
  owner_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (owner_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&self->classes[toluaI_index],tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: classes of class  owner_type */
static int toluaI_set_dungeon_owner_type_classes(lua_State* tolua_S)
{
 int toluaI_index;
  owner_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (owner_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=2)
 tolua_error(tolua_S,"array indexing out of range.");
  self->classes[toluaI_index] = *((flags_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: costs of class  owner_type */
static int toluaI_get_dungeon_owner_type_costs(lua_State* tolua_S)
{
 int toluaI_index;
  owner_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (owner_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=3)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->costs[toluaI_index]);
 return 1;
}

/* set function: costs of class  owner_type */
static int toluaI_set_dungeon_owner_type_costs(lua_State* tolua_S)
{
 int toluaI_index;
  owner_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (owner_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=3)
 tolua_error(tolua_S,"array indexing out of range.");
  self->costs[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: ow_info */
static int toluaI_get_dungeon_ow_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_ow_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&ow_info[toluaI_index],tolua_tag(tolua_S,"owner_type"));
 return 1;
}

/* set function: ow_info */
static int toluaI_set_dungeon_ow_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_ow_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  ow_info[toluaI_index] = *((owner_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: seed_flavor */
static int toluaI_get_dungeon_seed_flavor(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)seed_flavor);
 return 1;
}

/* set function: seed_flavor */
static int toluaI_set_dungeon_seed_flavor(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  seed_flavor = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: seed_town */
static int toluaI_get_dungeon_seed_town(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)seed_town);
 return 1;
}

/* set function: seed_town */
static int toluaI_set_dungeon_seed_town(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  seed_town = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: seed_dungeon */
static int toluaI_get_dungeon_seed_dungeon(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)seed_dungeon);
 return 1;
}

/* set function: seed_dungeon */
static int toluaI_set_dungeon_seed_dungeon(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  seed_dungeon = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: name of class  feature_type */
static int toluaI_get_dungeon_feature_type_name(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  feature_type */
static int toluaI_set_dungeon_feature_type_name(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: text of class  feature_type */
static int toluaI_get_dungeon_feature_type_text(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->text);
 return 1;
}

/* set function: text of class  feature_type */
static int toluaI_set_dungeon_feature_type_text(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->text = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: block of class  feature_type */
static int toluaI_get_dungeon_feature_type_block(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->block);
 return 1;
}

/* set function: block of class  feature_type */
static int toluaI_set_dungeon_feature_type_block(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->block = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: can_enter of class  feature_type */
static int toluaI_get_dungeon_feature_type_can_enter(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->can_enter);
 return 1;
}

/* set function: can_enter of class  feature_type */
static int toluaI_set_dungeon_feature_type_can_enter(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->can_enter = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mimic of class  feature_type */
static int toluaI_get_dungeon_feature_type_mimic(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mimic);
 return 1;
}

/* set function: mimic of class  feature_type */
static int toluaI_set_dungeon_feature_type_mimic(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mimic = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  feature_type */
static int toluaI_get_dungeon_feature_type_flags(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  feature_type */
static int toluaI_set_dungeon_feature_type_flags(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: extra of class  feature_type */
static int toluaI_get_dungeon_feature_type_extra(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->extra);
 return 1;
}

/* set function: extra of class  feature_type */
static int toluaI_set_dungeon_feature_type_extra(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->extra = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: unused of class  feature_type */
static int toluaI_get_dungeon_feature_type_unused(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->unused);
 return 1;
}

/* set function: unused of class  feature_type */
static int toluaI_set_dungeon_feature_type_unused(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->unused = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_attr of class  feature_type */
static int toluaI_get_dungeon_feature_type_d_attr(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_attr);
 return 1;
}

/* set function: d_attr of class  feature_type */
static int toluaI_set_dungeon_feature_type_d_attr(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_char of class  feature_type */
static int toluaI_get_dungeon_feature_type_d_char(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_char);
 return 1;
}

/* set function: d_char of class  feature_type */
static int toluaI_set_dungeon_feature_type_d_char(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_attr of class  feature_type */
static int toluaI_get_dungeon_feature_type_x_attr(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_attr);
 return 1;
}

/* set function: x_attr of class  feature_type */
static int toluaI_set_dungeon_feature_type_x_attr(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_char of class  feature_type */
static int toluaI_get_dungeon_feature_type_x_char(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_char);
 return 1;
}

/* set function: x_char of class  feature_type */
static int toluaI_set_dungeon_feature_type_x_char(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: shimmer of class  feature_type */
static int toluaI_get_dungeon_feature_type_shimmer(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=7)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->shimmer[toluaI_index]);
 return 1;
}

/* set function: shimmer of class  feature_type */
static int toluaI_set_dungeon_feature_type_shimmer(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=7)
 tolua_error(tolua_S,"array indexing out of range.");
  self->shimmer[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: d_dice of class  feature_type */
static int toluaI_get_dungeon_feature_type_d_dice(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->d_dice[toluaI_index]);
 return 1;
}

/* set function: d_dice of class  feature_type */
static int toluaI_set_dungeon_feature_type_d_dice(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->d_dice[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: d_side of class  feature_type */
static int toluaI_get_dungeon_feature_type_d_side(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->d_side[toluaI_index]);
 return 1;
}

/* set function: d_side of class  feature_type */
static int toluaI_set_dungeon_feature_type_d_side(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->d_side[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: d_frequency of class  feature_type */
static int toluaI_get_dungeon_feature_type_d_frequency(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->d_frequency[toluaI_index]);
 return 1;
}

/* set function: d_frequency of class  feature_type */
static int toluaI_set_dungeon_feature_type_d_frequency(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->d_frequency[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: d_type of class  feature_type */
static int toluaI_get_dungeon_feature_type_d_type(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->d_type[toluaI_index]);
 return 1;
}

/* set function: d_type of class  feature_type */
static int toluaI_set_dungeon_feature_type_d_type(lua_State* tolua_S)
{
 int toluaI_index;
  feature_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (feature_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->d_type[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: priority of class  feature_type */
static int toluaI_get_dungeon_feature_type_priority(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->priority);
 return 1;
}

/* set function: priority of class  feature_type */
static int toluaI_set_dungeon_feature_type_priority(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->priority = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: load_script of class  feature_type */
static int toluaI_get_dungeon_feature_type_load_script(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->load_script);
 return 1;
}

/* set function: load_script of class  feature_type */
static int toluaI_set_dungeon_feature_type_load_script(lua_State* tolua_S)
{
  feature_type* self = (feature_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->load_script = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: max_f_idx */
static int toluaI_get_dungeon_max_f_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_f_idx);
 return 1;
}

/* set function: max_f_idx */
static int toluaI_set_dungeon_max_f_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_f_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: f_info */
static int toluaI_get_dungeon_f_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_f_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&f_info[toluaI_index],tolua_tag(tolua_S,"feature_type"));
 return 1;
}

/* set function: f_info */
static int toluaI_set_dungeon_f_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_f_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  f_info[toluaI_index] = *((feature_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: name of class  vault_type */
static int toluaI_get_dungeon_vault_type_name(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  vault_type */
static int toluaI_set_dungeon_vault_type_name(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: text of class  vault_type */
static int toluaI_get_dungeon_vault_type_text(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->text);
 return 1;
}

/* set function: text of class  vault_type */
static int toluaI_set_dungeon_vault_type_text(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->text = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: typ of class  vault_type */
static int toluaI_get_dungeon_vault_type_typ(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->typ);
 return 1;
}

/* set function: typ of class  vault_type */
static int toluaI_set_dungeon_vault_type_typ(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->typ = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: rat of class  vault_type */
static int toluaI_get_dungeon_vault_type_rat(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->rat);
 return 1;
}

/* set function: rat of class  vault_type */
static int toluaI_set_dungeon_vault_type_rat(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->rat = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: hgt of class  vault_type */
static int toluaI_get_dungeon_vault_type_hgt(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->hgt);
 return 1;
}

/* set function: hgt of class  vault_type */
static int toluaI_set_dungeon_vault_type_hgt(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->hgt = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: wid of class  vault_type */
static int toluaI_get_dungeon_vault_type_wid(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->wid);
 return 1;
}

/* set function: wid of class  vault_type */
static int toluaI_set_dungeon_vault_type_wid(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->wid = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: lvl of class  vault_type */
static int toluaI_get_dungeon_vault_type_lvl(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->lvl);
 return 1;
}

/* set function: lvl of class  vault_type */
static int toluaI_set_dungeon_vault_type_lvl(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->lvl = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dun_type of class  vault_type */
static int toluaI_get_dungeon_vault_type_dun_type(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dun_type);
 return 1;
}

/* set function: dun_type of class  vault_type */
static int toluaI_set_dungeon_vault_type_dun_type(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dun_type = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mon of class  vault_type */
static int toluaI_get_dungeon_vault_type_mon(lua_State* tolua_S)
{
 int toluaI_index;
  vault_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (vault_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=10)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->mon[toluaI_index]);
 return 1;
}

/* set function: mon of class  vault_type */
static int toluaI_set_dungeon_vault_type_mon(lua_State* tolua_S)
{
 int toluaI_index;
  vault_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (vault_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=10)
 tolua_error(tolua_S,"array indexing out of range.");
  self->mon[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: item of class  vault_type */
static int toluaI_get_dungeon_vault_type_item(lua_State* tolua_S)
{
 int toluaI_index;
  vault_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (vault_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=3)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->item[toluaI_index]);
 return 1;
}

/* set function: item of class  vault_type */
static int toluaI_set_dungeon_vault_type_item(lua_State* tolua_S)
{
 int toluaI_index;
  vault_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (vault_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=3)
 tolua_error(tolua_S,"array indexing out of range.");
  self->item[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: flags of class  vault_type */
static int toluaI_get_dungeon_vault_type_flags(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  vault_type */
static int toluaI_set_dungeon_vault_type_flags(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: load_script of class  vault_type */
static int toluaI_get_dungeon_vault_type_load_script(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->load_script);
 return 1;
}

/* set function: load_script of class  vault_type */
static int toluaI_set_dungeon_vault_type_load_script(lua_State* tolua_S)
{
  vault_type* self = (vault_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->load_script = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: max_v_idx */
static int toluaI_get_dungeon_max_v_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_v_idx);
 return 1;
}

/* set function: max_v_idx */
static int toluaI_set_dungeon_max_v_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_v_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: v_info */
static int toluaI_get_dungeon_v_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_v_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&v_info[toluaI_index],tolua_tag(tolua_S,"vault_type"));
 return 1;
}

/* set function: v_info */
static int toluaI_set_dungeon_v_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_v_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  v_info[toluaI_index] = *((vault_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: wild_see_through */
static int toluaI_get_dungeon_wild_see_through(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)wild_see_through);
 return 1;
}

/* set function: wild_see_through */
static int toluaI_set_dungeon_wild_see_through(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  wild_see_through = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: use_corpse_decay */
static int toluaI_get_dungeon_use_corpse_decay(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)use_corpse_decay);
 return 1;
}

/* set function: use_corpse_decay */
static int toluaI_set_dungeon_use_corpse_decay(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  use_corpse_decay = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: recursive_river */
static int toluaI_dungeon_dungeon_generator_recursive_river00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,8)
 )
 goto tolua_lerror;
 else
 {
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b x2 = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b y2 = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b feat1 = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b feat2 = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b width = ((s32b)  tolua_getnumber(tolua_S,7,0));
 {
  recursive_river(x1,y1,x2,y2,feat1,feat2,width);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'recursive_river'.");
 return 0;
}

/* function: room_build */
static int toluaI_dungeon_dungeon_generator_room_build00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  room_build(y,x,typ);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'room_build'.");
 return 0;
}

/* function: dungeon_set_level_size */
static int toluaI_dungeon_dungeon_generator_set_level_size00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b hgt = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b wid = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  dungeon_set_level_size(hgt,wid);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_level_size'.");
 return 0;
}

/* function: realtime_mode */
static int toluaI_dungeon_realtime___switch00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  u32b millis_per_turn = ((u32b)  tolua_getnumber(tolua_S,1,0));
 {
  realtime_mode(millis_per_turn);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__switch'.");
 return 0;
}

/* get function: realtime_freq */
static int toluaI_get_dungeon_realtime_millis_per_turn(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)realtime_freq);
 return 1;
}

/* set function: realtime_freq */
static int toluaI_set_dungeon_realtime_millis_per_turn(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  realtime_freq = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: realtime_run_factor */
static int toluaI_get_dungeon_realtime_run_factor(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)realtime_run_factor);
 return 1;
}

/* set function: realtime_run_factor */
static int toluaI_set_dungeon_realtime_run_factor(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  realtime_run_factor = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* Open function */
int tolua_dungeon_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_globalvar(tolua_S,"SCREEN_HGT",toluaI_get_dungeon_SCREEN_HGT,toluaI_set_dungeon_SCREEN_HGT);
 tolua_globalvar(tolua_S,"SCREEN_WID",toluaI_get_dungeon_SCREEN_WID,toluaI_set_dungeon_SCREEN_WID);
 tolua_constant(tolua_S,NULL,"CAVE_MARK",CAVE_MARK);
 tolua_constant(tolua_S,NULL,"CAVE_GLOW",CAVE_GLOW);
 tolua_constant(tolua_S,NULL,"CAVE_ICKY",CAVE_ICKY);
 tolua_constant(tolua_S,NULL,"CAVE_ROOM",CAVE_ROOM);
 tolua_constant(tolua_S,NULL,"CAVE_SEEN",CAVE_SEEN);
 tolua_constant(tolua_S,NULL,"CAVE_VIEW",CAVE_VIEW);
 tolua_constant(tolua_S,NULL,"CAVE_TEMP",CAVE_TEMP);
 tolua_constant(tolua_S,NULL,"CAVE_WALL",CAVE_WALL);
 tolua_constant(tolua_S,NULL,"CAVE_TRDT",CAVE_TRDT);
 tolua_constant(tolua_S,NULL,"CAVE_IDNT",CAVE_IDNT);
 tolua_constant(tolua_S,NULL,"CAVE_SPEC",CAVE_SPEC);
 tolua_constant(tolua_S,NULL,"CAVE_FREE",CAVE_FREE);
 tolua_constant(tolua_S,NULL,"CAVE_DETECT",CAVE_DETECT);
 tolua_constant(tolua_S,NULL,"CAVE_PLIT",CAVE_PLIT);
 tolua_constant(tolua_S,NULL,"CAVE_MLIT",CAVE_MLIT);
 tolua_globalvar(tolua_S,"FEAT_WAY_LESS",toluaI_get_dungeon_FEAT_WAY_LESS,toluaI_set_dungeon_FEAT_WAY_LESS);
 tolua_globalvar(tolua_S,"FEAT_WAY_MORE",toluaI_get_dungeon_FEAT_WAY_MORE,toluaI_set_dungeon_FEAT_WAY_MORE);
 tolua_globalvar(tolua_S,"FEAT_SHAFT_DOWN",toluaI_get_dungeon_FEAT_SHAFT_DOWN,toluaI_set_dungeon_FEAT_SHAFT_DOWN);
 tolua_globalvar(tolua_S,"FEAT_SHAFT_UP",toluaI_get_dungeon_FEAT_SHAFT_UP,toluaI_set_dungeon_FEAT_SHAFT_UP);
 tolua_globalvar(tolua_S,"FEAT_LESS",toluaI_get_dungeon_FEAT_LESS,toluaI_set_dungeon_FEAT_LESS);
 tolua_globalvar(tolua_S,"FEAT_MORE",toluaI_get_dungeon_FEAT_MORE,toluaI_set_dungeon_FEAT_MORE);
 tolua_globalvar(tolua_S,"FEAT_DOOR",toluaI_get_dungeon_FEAT_DOOR,toluaI_set_dungeon_FEAT_DOOR);
 tolua_globalvar(tolua_S,"FEAT_OPEN",toluaI_get_dungeon_FEAT_OPEN,toluaI_set_dungeon_FEAT_OPEN);
 tolua_globalvar(tolua_S,"FEAT_WALL_INNER",toluaI_get_dungeon_FEAT_WALL_INNER,toluaI_set_dungeon_FEAT_WALL_INNER);
 tolua_globalvar(tolua_S,"FEAT_WALL_OUTER",toluaI_get_dungeon_FEAT_WALL_OUTER,toluaI_set_dungeon_FEAT_WALL_OUTER);
 tolua_globalvar(tolua_S,"FEAT_WALL_EXTRA",toluaI_get_dungeon_FEAT_WALL_EXTRA,toluaI_set_dungeon_FEAT_WALL_EXTRA);
 tolua_globalvar(tolua_S,"FEAT_WALL_SOLID",toluaI_get_dungeon_FEAT_WALL_SOLID,toluaI_set_dungeon_FEAT_WALL_SOLID);
 tolua_globalvar(tolua_S,"FEAT_PERM_INNER",toluaI_get_dungeon_FEAT_PERM_INNER,toluaI_set_dungeon_FEAT_PERM_INNER);
 tolua_globalvar(tolua_S,"FEAT_PERM_OUTER",toluaI_get_dungeon_FEAT_PERM_OUTER,toluaI_set_dungeon_FEAT_PERM_OUTER);
 tolua_globalvar(tolua_S,"FEAT_PERM_SOLID",toluaI_get_dungeon_FEAT_PERM_SOLID,toluaI_set_dungeon_FEAT_PERM_SOLID);
 tolua_globalvar(tolua_S,"FEAT_WORLD_BORDER",toluaI_get_dungeon_FEAT_WORLD_BORDER,toluaI_set_dungeon_FEAT_WORLD_BORDER);
 tolua_globalvar(tolua_S,"FEAT_RUBBLE",toluaI_get_dungeon_FEAT_RUBBLE,toluaI_set_dungeon_FEAT_RUBBLE);
 tolua_globalvar(tolua_S,"FEAT_SECRET",toluaI_get_dungeon_FEAT_SECRET,toluaI_set_dungeon_FEAT_SECRET);
 tolua_globalvar(tolua_S,"FEAT_BROKEN",toluaI_get_dungeon_FEAT_BROKEN,toluaI_set_dungeon_FEAT_BROKEN);
 tolua_globalvar(tolua_S,"FEAT_DOOR_LOCKED_HEAD",toluaI_get_dungeon_FEAT_DOOR_LOCKED_HEAD,toluaI_set_dungeon_FEAT_DOOR_LOCKED_HEAD);
 tolua_globalvar(tolua_S,"FEAT_DOOR_LOCKED_TAIL",toluaI_get_dungeon_FEAT_DOOR_LOCKED_TAIL,toluaI_set_dungeon_FEAT_DOOR_LOCKED_TAIL);
 tolua_globalvar(tolua_S,"FEAT_DOOR_JAMMED_HEAD",toluaI_get_dungeon_FEAT_DOOR_JAMMED_HEAD,toluaI_set_dungeon_FEAT_DOOR_JAMMED_HEAD);
 tolua_globalvar(tolua_S,"FEAT_DOOR_JAMMED_TAIL",toluaI_get_dungeon_FEAT_DOOR_JAMMED_TAIL,toluaI_set_dungeon_FEAT_DOOR_JAMMED_TAIL);
 tolua_globalvar(tolua_S,"FEAT_SHOP",toluaI_get_dungeon_FEAT_SHOP,toluaI_set_dungeon_FEAT_SHOP);
 tolua_globalvar(tolua_S,"level_flags",toluaI_get_dungeon_level_flags,toluaI_set_dungeon_level_flags);
 tolua_constant(tolua_S,NULL,"MAX_HGT",MAX_HGT);
 tolua_constant(tolua_S,NULL,"MAX_WID",MAX_WID);
 tolua_constant(tolua_S,NULL,"TOWN_REAL",TOWN_REAL);
 tolua_constant(tolua_S,NULL,"TOWN_KNOWN",TOWN_KNOWN);
 tolua_constant(tolua_S,NULL,"TOWN_RANDOM",TOWN_RANDOM);
 tolua_constant(tolua_S,NULL,"TOWN_DUNGEON",TOWN_DUNGEON);
 tolua_constant(tolua_S,NULL,"TOWN_CHANCE",TOWN_CHANCE);
 tolua_constant(tolua_S,NULL,"TERRAIN_EDGE",TERRAIN_EDGE);
 tolua_constant(tolua_S,NULL,"TERRAIN_TOWN",TERRAIN_TOWN);
 tolua_constant(tolua_S,NULL,"TERRAIN_DEEP_WATER",TERRAIN_DEEP_WATER);
 tolua_constant(tolua_S,NULL,"TERRAIN_SHALLOW_WATER",TERRAIN_SHALLOW_WATER);
 tolua_constant(tolua_S,NULL,"TERRAIN_SWAMP",TERRAIN_SWAMP);
 tolua_constant(tolua_S,NULL,"TERRAIN_DIRT",TERRAIN_DIRT);
 tolua_constant(tolua_S,NULL,"TERRAIN_GRASS",TERRAIN_GRASS);
 tolua_constant(tolua_S,NULL,"TERRAIN_TREES",TERRAIN_TREES);
 tolua_constant(tolua_S,NULL,"TERRAIN_DESERT",TERRAIN_DESERT);
 tolua_constant(tolua_S,NULL,"TERRAIN_SHALLOW_LAVA",TERRAIN_SHALLOW_LAVA);
 tolua_constant(tolua_S,NULL,"TERRAIN_DEEP_LAVA",TERRAIN_DEEP_LAVA);
 tolua_constant(tolua_S,NULL,"TERRAIN_MOUNTAIN",TERRAIN_MOUNTAIN);
 tolua_constant(tolua_S,NULL,"MAX_WILD_TERRAIN",MAX_WILD_TERRAIN);
 tolua_constant(tolua_S,NULL,"ALLOC_SET_CORR",ALLOC_SET_CORR);
 tolua_constant(tolua_S,NULL,"ALLOC_SET_ROOM",ALLOC_SET_ROOM);
 tolua_constant(tolua_S,NULL,"ALLOC_SET_BOTH",ALLOC_SET_BOTH);
 tolua_cclass(tolua_S,"border_type","");
 tolua_tablearray(tolua_S,"border_type","north",toluaI_get_dungeon_border_type_north,toluaI_set_dungeon_border_type_north);
 tolua_tablearray(tolua_S,"border_type","south",toluaI_get_dungeon_border_type_south,toluaI_set_dungeon_border_type_south);
 tolua_tablearray(tolua_S,"border_type","east",toluaI_get_dungeon_border_type_east,toluaI_set_dungeon_border_type_east);
 tolua_tablearray(tolua_S,"border_type","west",toluaI_get_dungeon_border_type_west,toluaI_set_dungeon_border_type_west);
 tolua_tablevar(tolua_S,"border_type","north_west",toluaI_get_dungeon_border_type_north_west,toluaI_set_dungeon_border_type_north_west);
 tolua_tablevar(tolua_S,"border_type","north_east",toluaI_get_dungeon_border_type_north_east,toluaI_set_dungeon_border_type_north_east);
 tolua_tablevar(tolua_S,"border_type","south_west",toluaI_get_dungeon_border_type_south_west,toluaI_set_dungeon_border_type_south_west);
 tolua_tablevar(tolua_S,"border_type","south_east",toluaI_get_dungeon_border_type_south_east,toluaI_set_dungeon_border_type_south_east);
 tolua_cclass(tolua_S,"wilderness_type_info","");
 tolua_tablevar(tolua_S,"wilderness_type_info","name",toluaI_get_dungeon_wilderness_type_info_name,toluaI_set_dungeon_wilderness_type_info_name);
 tolua_tablevar(tolua_S,"wilderness_type_info","text",toluaI_get_dungeon_wilderness_type_info_text,toluaI_set_dungeon_wilderness_type_info_text);
 tolua_tablevar(tolua_S,"wilderness_type_info","entrance",toluaI_get_dungeon_wilderness_type_info_entrance,toluaI_set_dungeon_wilderness_type_info_entrance);
 tolua_tablevar(tolua_S,"wilderness_type_info","road",toluaI_get_dungeon_wilderness_type_info_road,toluaI_set_dungeon_wilderness_type_info_road);
 tolua_tablevar(tolua_S,"wilderness_type_info","level",toluaI_get_dungeon_wilderness_type_info_level,toluaI_set_dungeon_wilderness_type_info_level);
 tolua_tablevar(tolua_S,"wilderness_type_info","flags",toluaI_get_dungeon_wilderness_type_info_flags,toluaI_set_dungeon_wilderness_type_info_flags);
 tolua_tablevar(tolua_S,"wilderness_type_info","feat",toluaI_get_dungeon_wilderness_type_info_feat,toluaI_set_dungeon_wilderness_type_info_feat);
 tolua_tablevar(tolua_S,"wilderness_type_info","d_char",toluaI_get_dungeon_wilderness_type_info_d_char,toluaI_set_dungeon_wilderness_type_info_d_char);
 tolua_tablevar(tolua_S,"wilderness_type_info","terrain_idx",toluaI_get_dungeon_wilderness_type_info_terrain_idx,toluaI_set_dungeon_wilderness_type_info_terrain_idx);
 tolua_tablearray(tolua_S,"wilderness_type_info","terrain",toluaI_get_dungeon_wilderness_type_info_terrain,toluaI_set_dungeon_wilderness_type_info_terrain);
 tolua_cclass(tolua_S,"wilderness_map","");
 tolua_tablevar(tolua_S,"wilderness_map","feat",toluaI_get_dungeon_wilderness_map_feat,toluaI_set_dungeon_wilderness_map_feat);
 tolua_tablevar(tolua_S,"wilderness_map","mimic",toluaI_get_dungeon_wilderness_map_mimic,toluaI_set_dungeon_wilderness_map_mimic);
 tolua_tablevar(tolua_S,"wilderness_map","seed",toluaI_get_dungeon_wilderness_map_seed,toluaI_set_dungeon_wilderness_map_seed);
 tolua_tablevar(tolua_S,"wilderness_map","entrance",toluaI_get_dungeon_wilderness_map_entrance,toluaI_set_dungeon_wilderness_map_entrance);
 tolua_tablevar(tolua_S,"wilderness_map","known",toluaI_get_dungeon_wilderness_map_known,toluaI_set_dungeon_wilderness_map_known);
 tolua_tablevar(tolua_S,"wilderness_map","flags",toluaI_get_dungeon_wilderness_map_flags,toluaI_set_dungeon_wilderness_map_flags);
 tolua_globalvar(tolua_S,"max_st_idx",toluaI_get_dungeon_max_st_idx,toluaI_set_dungeon_max_st_idx);
 tolua_cclass(tolua_S,"town_type","");
 tolua_tablevar(tolua_S,"town_type","name",toluaI_get_dungeon_town_type_name,toluaI_set_dungeon_town_type_name);
 tolua_tablevar(tolua_S,"town_type","seed",toluaI_get_dungeon_town_type_seed,toluaI_set_dungeon_town_type_seed);
 tolua_tablevar(tolua_S,"town_type","stores",toluaI_get_dungeon_town_type_stores,toluaI_set_dungeon_town_type_stores);
 tolua_tablevar(tolua_S,"town_type","numstores",toluaI_get_dungeon_town_type_numstores,toluaI_set_dungeon_town_type_numstores);
 tolua_tablevar(tolua_S,"town_type","flags",toluaI_get_dungeon_town_type_flags,toluaI_set_dungeon_town_type_flags);
 tolua_tablevar(tolua_S,"town_type","stocked",toluaI_get_dungeon_town_type_stocked,toluaI_set_dungeon_town_type_stocked);
 tolua_tablevar(tolua_S,"town_type","destroyed",toluaI_get_dungeon_town_type_destroyed,toluaI_set_dungeon_town_type_destroyed);
 tolua_globalvar(tolua_S,"max_towns",toluaI_get_dungeon_max_towns,toluaI_set_dungeon_max_towns);
 tolua_globalvar(tolua_S,"max_real_towns",toluaI_get_dungeon_max_real_towns,toluaI_set_dungeon_max_real_towns);
 tolua_globalarray(tolua_S,"town_info",toluaI_get_dungeon_town_info,toluaI_set_dungeon_town_info);
 tolua_cclass(tolua_S,"rule_type","");
 tolua_tablevar(tolua_S,"rule_type","mode",toluaI_get_dungeon_rule_type_mode,toluaI_set_dungeon_rule_type_mode);
 tolua_tablevar(tolua_S,"rule_type","percent",toluaI_get_dungeon_rule_type_percent,toluaI_set_dungeon_rule_type_percent);
 tolua_tablevar(tolua_S,"rule_type","flags",toluaI_get_dungeon_rule_type_flags,toluaI_set_dungeon_rule_type_flags);
 tolua_tablearray(tolua_S,"rule_type","r_char",toluaI_get_dungeon_rule_type_r_char,toluaI_set_dungeon_rule_type_r_char);
 tolua_cclass(tolua_S,"dungeon_info_type","");
 tolua_tablevar(tolua_S,"dungeon_info_type","name",toluaI_get_dungeon_dungeon_info_type_name,toluaI_set_dungeon_dungeon_info_type_name);
 tolua_tablevar(tolua_S,"dungeon_info_type","text",toluaI_get_dungeon_dungeon_info_type_text,toluaI_set_dungeon_dungeon_info_type_text);
 tolua_tablearray(tolua_S,"dungeon_info_type","short_name",toluaI_get_dungeon_dungeon_info_type_short_name,toluaI_set_dungeon_dungeon_info_type_short_name);
 tolua_tablevar(tolua_S,"dungeon_info_type","floor1",toluaI_get_dungeon_dungeon_info_type_floor1,toluaI_set_dungeon_dungeon_info_type_floor1);
 tolua_tablearray(tolua_S,"dungeon_info_type","floor_percent1",toluaI_get_dungeon_dungeon_info_type_floor_percent1,toluaI_set_dungeon_dungeon_info_type_floor_percent1);
 tolua_tablevar(tolua_S,"dungeon_info_type","floor2",toluaI_get_dungeon_dungeon_info_type_floor2,toluaI_set_dungeon_dungeon_info_type_floor2);
 tolua_tablearray(tolua_S,"dungeon_info_type","floor_percent2",toluaI_get_dungeon_dungeon_info_type_floor_percent2,toluaI_set_dungeon_dungeon_info_type_floor_percent2);
 tolua_tablevar(tolua_S,"dungeon_info_type","floor3",toluaI_get_dungeon_dungeon_info_type_floor3,toluaI_set_dungeon_dungeon_info_type_floor3);
 tolua_tablearray(tolua_S,"dungeon_info_type","floor_percent3",toluaI_get_dungeon_dungeon_info_type_floor_percent3,toluaI_set_dungeon_dungeon_info_type_floor_percent3);
 tolua_tablevar(tolua_S,"dungeon_info_type","outer_wall",toluaI_get_dungeon_dungeon_info_type_outer_wall,toluaI_set_dungeon_dungeon_info_type_outer_wall);
 tolua_tablevar(tolua_S,"dungeon_info_type","inner_wall",toluaI_get_dungeon_dungeon_info_type_inner_wall,toluaI_set_dungeon_dungeon_info_type_inner_wall);
 tolua_tablevar(tolua_S,"dungeon_info_type","fill_type1",toluaI_get_dungeon_dungeon_info_type_fill_type1,toluaI_set_dungeon_dungeon_info_type_fill_type1);
 tolua_tablearray(tolua_S,"dungeon_info_type","fill_percent1",toluaI_get_dungeon_dungeon_info_type_fill_percent1,toluaI_set_dungeon_dungeon_info_type_fill_percent1);
 tolua_tablevar(tolua_S,"dungeon_info_type","fill_type2",toluaI_get_dungeon_dungeon_info_type_fill_type2,toluaI_set_dungeon_dungeon_info_type_fill_type2);
 tolua_tablearray(tolua_S,"dungeon_info_type","fill_percent2",toluaI_get_dungeon_dungeon_info_type_fill_percent2,toluaI_set_dungeon_dungeon_info_type_fill_percent2);
 tolua_tablevar(tolua_S,"dungeon_info_type","fill_type3",toluaI_get_dungeon_dungeon_info_type_fill_type3,toluaI_set_dungeon_dungeon_info_type_fill_type3);
 tolua_tablearray(tolua_S,"dungeon_info_type","fill_percent3",toluaI_get_dungeon_dungeon_info_type_fill_percent3,toluaI_set_dungeon_dungeon_info_type_fill_percent3);
 tolua_tablevar(tolua_S,"dungeon_info_type","fill_method",toluaI_get_dungeon_dungeon_info_type_fill_method,toluaI_set_dungeon_dungeon_info_type_fill_method);
 tolua_tablevar(tolua_S,"dungeon_info_type","mindepth",toluaI_get_dungeon_dungeon_info_type_mindepth,toluaI_set_dungeon_dungeon_info_type_mindepth);
 tolua_tablevar(tolua_S,"dungeon_info_type","maxdepth",toluaI_get_dungeon_dungeon_info_type_maxdepth,toluaI_set_dungeon_dungeon_info_type_maxdepth);
 tolua_tablevar(tolua_S,"dungeon_info_type","next",toluaI_get_dungeon_dungeon_info_type_next,toluaI_set_dungeon_dungeon_info_type_next);
 tolua_tablevar(tolua_S,"dungeon_info_type","min_plev",toluaI_get_dungeon_dungeon_info_type_min_plev,toluaI_set_dungeon_dungeon_info_type_min_plev);
 tolua_tablevar(tolua_S,"dungeon_info_type","min_m_alloc_level",toluaI_get_dungeon_dungeon_info_type_min_m_alloc_level,toluaI_set_dungeon_dungeon_info_type_min_m_alloc_level);
 tolua_tablevar(tolua_S,"dungeon_info_type","max_m_alloc_chance",toluaI_get_dungeon_dungeon_info_type_max_m_alloc_chance,toluaI_set_dungeon_dungeon_info_type_max_m_alloc_chance);
 tolua_tablevar(tolua_S,"dungeon_info_type","flags",toluaI_get_dungeon_dungeon_info_type_flags,toluaI_set_dungeon_dungeon_info_type_flags);
 tolua_tablevar(tolua_S,"dungeon_info_type","size_x",toluaI_get_dungeon_dungeon_info_type_size_x,toluaI_set_dungeon_dungeon_info_type_size_x);
 tolua_tablevar(tolua_S,"dungeon_info_type","size_y",toluaI_get_dungeon_dungeon_info_type_size_y,toluaI_set_dungeon_dungeon_info_type_size_y);
 tolua_tablearray(tolua_S,"dungeon_info_type","rule_percents",toluaI_get_dungeon_dungeon_info_type_rule_percents,toluaI_set_dungeon_dungeon_info_type_rule_percents);
 tolua_tablearray(tolua_S,"dungeon_info_type","rules",toluaI_get_dungeon_dungeon_info_type_rules,toluaI_set_dungeon_dungeon_info_type_rules);
 tolua_tablevar(tolua_S,"dungeon_info_type","ix",toluaI_get_dungeon_dungeon_info_type_ix,toluaI_set_dungeon_dungeon_info_type_ix);
 tolua_tablevar(tolua_S,"dungeon_info_type","iy",toluaI_get_dungeon_dungeon_info_type_iy,toluaI_set_dungeon_dungeon_info_type_iy);
 tolua_tablevar(tolua_S,"dungeon_info_type","ox",toluaI_get_dungeon_dungeon_info_type_ox,toluaI_set_dungeon_dungeon_info_type_ox);
 tolua_tablevar(tolua_S,"dungeon_info_type","oy",toluaI_get_dungeon_dungeon_info_type_oy,toluaI_set_dungeon_dungeon_info_type_oy);
 tolua_tablearray(tolua_S,"dungeon_info_type","d_dice",toluaI_get_dungeon_dungeon_info_type_d_dice,toluaI_set_dungeon_dungeon_info_type_d_dice);
 tolua_tablearray(tolua_S,"dungeon_info_type","d_side",toluaI_get_dungeon_dungeon_info_type_d_side,toluaI_set_dungeon_dungeon_info_type_d_side);
 tolua_tablearray(tolua_S,"dungeon_info_type","d_frequency",toluaI_get_dungeon_dungeon_info_type_d_frequency,toluaI_set_dungeon_dungeon_info_type_d_frequency);
 tolua_tablearray(tolua_S,"dungeon_info_type","d_type",toluaI_get_dungeon_dungeon_info_type_d_type,toluaI_set_dungeon_dungeon_info_type_d_type);
 tolua_tablearray(tolua_S,"dungeon_info_type","t_idx",toluaI_get_dungeon_dungeon_info_type_t_idx,toluaI_set_dungeon_dungeon_info_type_t_idx);
 tolua_tablearray(tolua_S,"dungeon_info_type","t_level",toluaI_get_dungeon_dungeon_info_type_t_level,toluaI_set_dungeon_dungeon_info_type_t_level);
 tolua_tablevar(tolua_S,"dungeon_info_type","t_num",toluaI_get_dungeon_dungeon_info_type_t_num,toluaI_set_dungeon_dungeon_info_type_t_num);
 tolua_globalvar(tolua_S,"max_d_idx",toluaI_get_dungeon_max_d_idx,toluaI_set_dungeon_max_d_idx);
 tolua_globalarray(tolua_S,"d_info",toluaI_get_dungeon_d_info,toluaI_set_dungeon_d_info);
 tolua_globalvar(tolua_S,"max_wild_x",toluaI_get_dungeon_max_wild_x,toluaI_set_dungeon_max_wild_x);
 tolua_globalvar(tolua_S,"max_wild_y",toluaI_get_dungeon_max_wild_y,toluaI_set_dungeon_max_wild_y);
 tolua_globalvar(tolua_S,"max_wf_idx",toluaI_get_dungeon_max_wf_idx,toluaI_set_dungeon_max_wf_idx);
 tolua_globalarray(tolua_S,"wf_info",toluaI_get_dungeon_wf_info,toluaI_set_dungeon_wf_info);
 tolua_globalvar(tolua_S,"current_dungeon_idx",toluaI_get_dungeon_current_dungeon_idx,toluaI_set_dungeon_current_dungeon_idx);
 tolua_function(tolua_S,NULL,"wild_map",toluaI_dungeon_wild_map00);
 tolua_function(tolua_S,NULL,"place_floor",toluaI_dungeon_place_floor00);
 tolua_function(tolua_S,NULL,"place_filler",toluaI_dungeon_place_filler00);
 tolua_function(tolua_S,NULL,"add_scripted_generator",toluaI_dungeon_add_scripted_generator00);
 tolua_function(tolua_S,NULL,"new_player_spot",toluaI_dungeon_new_player_spot00);
 tolua_function(tolua_S,NULL,"get_level_desc",toluaI_dungeon_get_level_desc00);
 tolua_function(tolua_S,NULL,"get_level_flags",toluaI_dungeon_get_level_flags00);
 tolua_function(tolua_S,NULL,"get_dungeon_name",toluaI_dungeon_get_dungeon_name00);
 tolua_function(tolua_S,NULL,"get_dungeon_special",toluaI_dungeon_get_dungeon_special00);
 tolua_function(tolua_S,NULL,"get_branch",toluaI_dungeon_get_branch00);
 tolua_function(tolua_S,NULL,"get_fbranch",toluaI_dungeon_get_fbranch00);
 tolua_function(tolua_S,NULL,"get_flevel",toluaI_dungeon_get_flevel00);
 tolua_function(tolua_S,NULL,"prt_map",toluaI_dungeon_prt_map00);
 tolua_function(tolua_S,NULL,"verify_panel",toluaI_dungeon_verify_panel00);
 tolua_function(tolua_S,NULL,"change_panel",toluaI_dungeon_change_panel00);
 tolua_function(tolua_S,NULL,"player_can_enter",toluaI_dungeon_player_can_enter00);
 tolua_constant(tolua_S,NULL,"DUNGEON_MODE_NONE",DUNGEON_MODE_NONE);
 tolua_constant(tolua_S,NULL,"DUNGEON_MODE_AND",DUNGEON_MODE_AND);
 tolua_constant(tolua_S,NULL,"DUNGEON_MODE_NAND",DUNGEON_MODE_NAND);
 tolua_constant(tolua_S,NULL,"DUNGEON_MODE_OR",DUNGEON_MODE_OR);
 tolua_constant(tolua_S,NULL,"DUNGEON_MODE_NOR",DUNGEON_MODE_NOR);
 tolua_globalarray(tolua_S,"max_dlv",toluaI_get_dungeon_max_dlv,toluaI_set_dungeon_max_dlv);
 tolua_globalarray(tolua_S,"wildc2i",toluaI_get_dungeon_wildc2i,toluaI_set_dungeon_wildc2i);
 tolua_function(tolua_S,NULL,"reveal_wilderness_around_player",toluaI_dungeon_reveal_wilderness_around_player00);
 tolua_function(tolua_S,NULL,"special_lvl",toluaI_dungeon_special_lvl00);
 tolua_globalvar(tolua_S,"generate_special_feeling",toluaI_get_dungeon_generate_special_feeling,toluaI_set_dungeon_generate_special_feeling);
 tolua_function(tolua_S,NULL,"store_type_new",toluaI_dungeon_store_type_new00);
 tolua_function(tolua_S,NULL,"store_type_del",toluaI_dungeon_store_type_del00);
 tolua_function(tolua_S,NULL,"store_type_dup",toluaI_dungeon_store_type_dup00);
 tolua_cclass(tolua_S,"store_type","");
 tolua_tablevar(tolua_S,"store_type","st_idx",toluaI_get_dungeon_store_type_st_idx,toluaI_set_dungeon_store_type_st_idx);
 tolua_tablevar(tolua_S,"store_type","owner",toluaI_get_dungeon_store_type_owner,toluaI_set_dungeon_store_type_owner);
 tolua_tablevar(tolua_S,"store_type","insult_cur",toluaI_get_dungeon_store_type_insult_cur,toluaI_set_dungeon_store_type_insult_cur);
 tolua_tablevar(tolua_S,"store_type","good_buy",toluaI_get_dungeon_store_type_good_buy,toluaI_set_dungeon_store_type_good_buy);
 tolua_tablevar(tolua_S,"store_type","bad_buy",toluaI_get_dungeon_store_type_bad_buy,toluaI_set_dungeon_store_type_bad_buy);
 tolua_tablevar(tolua_S,"store_type","store_open",toluaI_get_dungeon_store_type_store_open,toluaI_set_dungeon_store_type_store_open);
 tolua_tablevar(tolua_S,"store_type","last_visit",toluaI_get_dungeon_store_type_last_visit,toluaI_set_dungeon_store_type_last_visit);
 tolua_tablevar(tolua_S,"store_type","stock",toluaI_get_dungeon_store_type_stock,toluaI_set_dungeon_store_type_stock);
 tolua_cclass(tolua_S,"store_info_type","");
 tolua_tablevar(tolua_S,"store_info_type","name",toluaI_get_dungeon_store_info_type_name,toluaI_set_dungeon_store_info_type_name);
 tolua_tablevar(tolua_S,"store_info_type","table",toluaI_get_dungeon_store_info_type_table,toluaI_set_dungeon_store_info_type_table);
 tolua_tablevar(tolua_S,"store_info_type","max_obj",toluaI_get_dungeon_store_info_type_max_obj,toluaI_set_dungeon_store_info_type_max_obj);
 tolua_tablevar(tolua_S,"store_info_type","buy",toluaI_get_dungeon_store_info_type_buy,toluaI_set_dungeon_store_info_type_buy);
 tolua_tablevar(tolua_S,"store_info_type","store_maker",toluaI_get_dungeon_store_info_type_store_maker,toluaI_set_dungeon_store_info_type_store_maker);
 tolua_tablevar(tolua_S,"store_info_type","store_make_num",toluaI_get_dungeon_store_info_type_store_make_num,toluaI_set_dungeon_store_info_type_store_make_num);
 tolua_tablearray(tolua_S,"store_info_type","owners",toluaI_get_dungeon_store_info_type_owners,toluaI_set_dungeon_store_info_type_owners);
 tolua_tablearray(tolua_S,"store_info_type","actions",toluaI_get_dungeon_store_info_type_actions,toluaI_set_dungeon_store_info_type_actions);
 tolua_tablevar(tolua_S,"store_info_type","d_attr",toluaI_get_dungeon_store_info_type_d_attr,toluaI_set_dungeon_store_info_type_d_attr);
 tolua_tablevar(tolua_S,"store_info_type","d_char",toluaI_get_dungeon_store_info_type_d_char,toluaI_set_dungeon_store_info_type_d_char);
 tolua_tablevar(tolua_S,"store_info_type","x_attr",toluaI_get_dungeon_store_info_type_x_attr,toluaI_set_dungeon_store_info_type_x_attr);
 tolua_tablevar(tolua_S,"store_info_type","x_char",toluaI_get_dungeon_store_info_type_x_char,toluaI_set_dungeon_store_info_type_x_char);
 tolua_tablevar(tolua_S,"store_info_type","flags",toluaI_get_dungeon_store_info_type_flags,toluaI_set_dungeon_store_info_type_flags);
 tolua_globalarray(tolua_S,"st_info",toluaI_get_dungeon_st_info,toluaI_set_dungeon_st_info);
 tolua_cclass(tolua_S,"store_action_type","");
 tolua_tablevar(tolua_S,"store_action_type","name",toluaI_get_dungeon_store_action_type_name,toluaI_set_dungeon_store_action_type_name);
 tolua_tablearray(tolua_S,"store_action_type","costs",toluaI_get_dungeon_store_action_type_costs,toluaI_set_dungeon_store_action_type_costs);
 tolua_tablevar(tolua_S,"store_action_type","letter",toluaI_get_dungeon_store_action_type_letter,toluaI_set_dungeon_store_action_type_letter);
 tolua_tablevar(tolua_S,"store_action_type","letter_aux",toluaI_get_dungeon_store_action_type_letter_aux,toluaI_set_dungeon_store_action_type_letter_aux);
 tolua_tablevar(tolua_S,"store_action_type","action",toluaI_get_dungeon_store_action_type_action,toluaI_set_dungeon_store_action_type_action);
 tolua_tablevar(tolua_S,"store_action_type","action_restr",toluaI_get_dungeon_store_action_type_action_restr,toluaI_set_dungeon_store_action_type_action_restr);
 tolua_globalarray(tolua_S,"ba_info",toluaI_get_dungeon_ba_info,toluaI_set_dungeon_ba_info);
 tolua_cclass(tolua_S,"owner_type","");
 tolua_tablevar(tolua_S,"owner_type","name",toluaI_get_dungeon_owner_type_name,toluaI_set_dungeon_owner_type_name);
 tolua_tablevar(tolua_S,"owner_type","max_cost",toluaI_get_dungeon_owner_type_max_cost,toluaI_set_dungeon_owner_type_max_cost);
 tolua_tablevar(tolua_S,"owner_type","max_inflate",toluaI_get_dungeon_owner_type_max_inflate,toluaI_set_dungeon_owner_type_max_inflate);
 tolua_tablevar(tolua_S,"owner_type","min_inflate",toluaI_get_dungeon_owner_type_min_inflate,toluaI_set_dungeon_owner_type_min_inflate);
 tolua_tablevar(tolua_S,"owner_type","haggle_per",toluaI_get_dungeon_owner_type_haggle_per,toluaI_set_dungeon_owner_type_haggle_per);
 tolua_tablevar(tolua_S,"owner_type","insult_max",toluaI_get_dungeon_owner_type_insult_max,toluaI_set_dungeon_owner_type_insult_max);
 tolua_tablearray(tolua_S,"owner_type","races",toluaI_get_dungeon_owner_type_races,toluaI_set_dungeon_owner_type_races);
 tolua_tablearray(tolua_S,"owner_type","classes",toluaI_get_dungeon_owner_type_classes,toluaI_set_dungeon_owner_type_classes);
 tolua_tablearray(tolua_S,"owner_type","costs",toluaI_get_dungeon_owner_type_costs,toluaI_set_dungeon_owner_type_costs);
 tolua_globalarray(tolua_S,"ow_info",toluaI_get_dungeon_ow_info,toluaI_set_dungeon_ow_info);
 tolua_constant(tolua_S,NULL,"STORE_HATED",STORE_HATED);
 tolua_constant(tolua_S,NULL,"STORE_LIKED",STORE_LIKED);
 tolua_constant(tolua_S,NULL,"STORE_NORMAL",STORE_NORMAL);
 tolua_globalvar(tolua_S,"seed_flavor",toluaI_get_dungeon_seed_flavor,toluaI_set_dungeon_seed_flavor);
 tolua_globalvar(tolua_S,"seed_town",toluaI_get_dungeon_seed_town,toluaI_set_dungeon_seed_town);
 tolua_globalvar(tolua_S,"seed_dungeon",toluaI_get_dungeon_seed_dungeon,toluaI_set_dungeon_seed_dungeon);
 tolua_cclass(tolua_S,"feature_type","");
 tolua_tablevar(tolua_S,"feature_type","name",toluaI_get_dungeon_feature_type_name,toluaI_set_dungeon_feature_type_name);
 tolua_tablevar(tolua_S,"feature_type","text",toluaI_get_dungeon_feature_type_text,toluaI_set_dungeon_feature_type_text);
 tolua_tablevar(tolua_S,"feature_type","block",toluaI_get_dungeon_feature_type_block,toluaI_set_dungeon_feature_type_block);
 tolua_tablevar(tolua_S,"feature_type","can_enter",toluaI_get_dungeon_feature_type_can_enter,toluaI_set_dungeon_feature_type_can_enter);
 tolua_tablevar(tolua_S,"feature_type","mimic",toluaI_get_dungeon_feature_type_mimic,toluaI_set_dungeon_feature_type_mimic);
 tolua_tablevar(tolua_S,"feature_type","flags",toluaI_get_dungeon_feature_type_flags,toluaI_set_dungeon_feature_type_flags);
 tolua_tablevar(tolua_S,"feature_type","extra",toluaI_get_dungeon_feature_type_extra,toluaI_set_dungeon_feature_type_extra);
 tolua_tablevar(tolua_S,"feature_type","unused",toluaI_get_dungeon_feature_type_unused,toluaI_set_dungeon_feature_type_unused);
 tolua_tablevar(tolua_S,"feature_type","d_attr",toluaI_get_dungeon_feature_type_d_attr,toluaI_set_dungeon_feature_type_d_attr);
 tolua_tablevar(tolua_S,"feature_type","d_char",toluaI_get_dungeon_feature_type_d_char,toluaI_set_dungeon_feature_type_d_char);
 tolua_tablevar(tolua_S,"feature_type","x_attr",toluaI_get_dungeon_feature_type_x_attr,toluaI_set_dungeon_feature_type_x_attr);
 tolua_tablevar(tolua_S,"feature_type","x_char",toluaI_get_dungeon_feature_type_x_char,toluaI_set_dungeon_feature_type_x_char);
 tolua_tablearray(tolua_S,"feature_type","shimmer",toluaI_get_dungeon_feature_type_shimmer,toluaI_set_dungeon_feature_type_shimmer);
 tolua_tablearray(tolua_S,"feature_type","d_dice",toluaI_get_dungeon_feature_type_d_dice,toluaI_set_dungeon_feature_type_d_dice);
 tolua_tablearray(tolua_S,"feature_type","d_side",toluaI_get_dungeon_feature_type_d_side,toluaI_set_dungeon_feature_type_d_side);
 tolua_tablearray(tolua_S,"feature_type","d_frequency",toluaI_get_dungeon_feature_type_d_frequency,toluaI_set_dungeon_feature_type_d_frequency);
 tolua_tablearray(tolua_S,"feature_type","d_type",toluaI_get_dungeon_feature_type_d_type,toluaI_set_dungeon_feature_type_d_type);
 tolua_tablevar(tolua_S,"feature_type","priority",toluaI_get_dungeon_feature_type_priority,toluaI_set_dungeon_feature_type_priority);
 tolua_tablevar(tolua_S,"feature_type","load_script",toluaI_get_dungeon_feature_type_load_script,toluaI_set_dungeon_feature_type_load_script);
 tolua_globalvar(tolua_S,"max_f_idx",toluaI_get_dungeon_max_f_idx,toluaI_set_dungeon_max_f_idx);
 tolua_globalarray(tolua_S,"f_info",toluaI_get_dungeon_f_info,toluaI_set_dungeon_f_info);
 tolua_cclass(tolua_S,"vault_type","");
 tolua_tablevar(tolua_S,"vault_type","name",toluaI_get_dungeon_vault_type_name,toluaI_set_dungeon_vault_type_name);
 tolua_tablevar(tolua_S,"vault_type","text",toluaI_get_dungeon_vault_type_text,toluaI_set_dungeon_vault_type_text);
 tolua_tablevar(tolua_S,"vault_type","typ",toluaI_get_dungeon_vault_type_typ,toluaI_set_dungeon_vault_type_typ);
 tolua_tablevar(tolua_S,"vault_type","rat",toluaI_get_dungeon_vault_type_rat,toluaI_set_dungeon_vault_type_rat);
 tolua_tablevar(tolua_S,"vault_type","hgt",toluaI_get_dungeon_vault_type_hgt,toluaI_set_dungeon_vault_type_hgt);
 tolua_tablevar(tolua_S,"vault_type","wid",toluaI_get_dungeon_vault_type_wid,toluaI_set_dungeon_vault_type_wid);
 tolua_tablevar(tolua_S,"vault_type","lvl",toluaI_get_dungeon_vault_type_lvl,toluaI_set_dungeon_vault_type_lvl);
 tolua_tablevar(tolua_S,"vault_type","dun_type",toluaI_get_dungeon_vault_type_dun_type,toluaI_set_dungeon_vault_type_dun_type);
 tolua_tablearray(tolua_S,"vault_type","mon",toluaI_get_dungeon_vault_type_mon,toluaI_set_dungeon_vault_type_mon);
 tolua_tablearray(tolua_S,"vault_type","item",toluaI_get_dungeon_vault_type_item,toluaI_set_dungeon_vault_type_item);
 tolua_tablevar(tolua_S,"vault_type","flags",toluaI_get_dungeon_vault_type_flags,toluaI_set_dungeon_vault_type_flags);
 tolua_tablevar(tolua_S,"vault_type","load_script",toluaI_get_dungeon_vault_type_load_script,toluaI_set_dungeon_vault_type_load_script);
 tolua_globalvar(tolua_S,"max_v_idx",toluaI_get_dungeon_max_v_idx,toluaI_set_dungeon_max_v_idx);
 tolua_globalarray(tolua_S,"v_info",toluaI_get_dungeon_v_info,toluaI_set_dungeon_v_info);
 tolua_globalvar(tolua_S,"wild_see_through",toluaI_get_dungeon_wild_see_through,toluaI_set_dungeon_wild_see_through);
 tolua_globalvar(tolua_S,"use_corpse_decay",toluaI_get_dungeon_use_corpse_decay,toluaI_set_dungeon_use_corpse_decay);
 tolua_module(tolua_S,"dungeon_generator");
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_NORMAL",ROOM_TYPE_NORMAL);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_OVERLAPPING",ROOM_TYPE_OVERLAPPING);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_CROSS",ROOM_TYPE_CROSS);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_LARGE",ROOM_TYPE_LARGE);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_NEST",ROOM_TYPE_NEST);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_PIT",ROOM_TYPE_PIT);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_LESSER_VAULT",ROOM_TYPE_LESSER_VAULT);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_GREATER_VAULT",ROOM_TYPE_GREATER_VAULT);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_CIRCULAR",ROOM_TYPE_CIRCULAR);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_FRACTAL",ROOM_TYPE_FRACTAL);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_RANDOM_VAULT",ROOM_TYPE_RANDOM_VAULT);
 tolua_constant(tolua_S,"dungeon_generator","ROOM_TYPE_CRYPT",ROOM_TYPE_CRYPT);
 tolua_function(tolua_S,"dungeon_generator","recursive_river",toluaI_dungeon_dungeon_generator_recursive_river00);
 tolua_function(tolua_S,"dungeon_generator","room_build",toluaI_dungeon_dungeon_generator_room_build00);
 tolua_function(tolua_S,"dungeon_generator","set_level_size",toluaI_dungeon_dungeon_generator_set_level_size00);
 tolua_module(tolua_S,"realtime");
 tolua_function(tolua_S,"realtime","__switch",toluaI_dungeon_realtime___switch00);
 tolua_tablevar(tolua_S,"realtime","millis_per_turn",toluaI_get_dungeon_realtime_millis_per_turn,toluaI_set_dungeon_realtime_millis_per_turn);
 tolua_tablevar(tolua_S,"realtime","run_factor",toluaI_get_dungeon_realtime_run_factor,toluaI_set_dungeon_realtime_run_factor);
 tolua_constant(tolua_S,NULL,"KILLWALL_FAILURE",KILLWALL_FAILURE);
 tolua_constant(tolua_S,NULL,"KILLWALL_WORKING",KILLWALL_WORKING);
 tolua_constant(tolua_S,NULL,"KILLWALL_DONE",KILLWALL_DONE);
 return 1;
}
/* Close function */
void tolua_dungeon_close (lua_State* tolua_S)
{
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"SCREEN_HGT"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"SCREEN_WID"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_MARK");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_GLOW");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_ICKY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_ROOM");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_SEEN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_VIEW");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_TEMP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_WALL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_TRDT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_IDNT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_SPEC");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_FREE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_DETECT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_PLIT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CAVE_MLIT");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_WAY_LESS"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_WAY_MORE"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_SHAFT_DOWN"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_SHAFT_UP"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_LESS"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_MORE"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_DOOR"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_OPEN"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_WALL_INNER"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_WALL_OUTER"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_WALL_EXTRA"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_WALL_SOLID"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_PERM_INNER"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_PERM_OUTER"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_PERM_SOLID"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_WORLD_BORDER"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_RUBBLE"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_SECRET"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_BROKEN"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_DOOR_LOCKED_HEAD"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_DOOR_LOCKED_TAIL"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_DOOR_JAMMED_HEAD"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_DOOR_JAMMED_TAIL"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"FEAT_SHOP"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"level_flags"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MAX_HGT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MAX_WID");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TOWN_REAL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TOWN_KNOWN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TOWN_RANDOM");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TOWN_DUNGEON");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TOWN_CHANCE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_EDGE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_TOWN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_DEEP_WATER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_SHALLOW_WATER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_SWAMP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_DIRT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_GRASS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_TREES");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_DESERT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_SHALLOW_LAVA");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_DEEP_LAVA");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERRAIN_MOUNTAIN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MAX_WILD_TERRAIN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ALLOC_SET_CORR");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ALLOC_SET_ROOM");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ALLOC_SET_BOTH");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"border_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wilderness_type_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wilderness_map");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_st_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"town_type");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_towns"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_real_towns"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"town_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"rule_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"dungeon_info_type");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_d_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"d_info");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_wild_x"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_wild_y"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_wf_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wf_info");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"current_dungeon_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wild_map");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_floor");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_filler");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"add_scripted_generator");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"new_player_spot");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_level_desc");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_level_flags");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_dungeon_name");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_dungeon_special");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_branch");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_fbranch");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_flevel");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"prt_map");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"verify_panel");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"change_panel");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"player_can_enter");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DUNGEON_MODE_NONE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DUNGEON_MODE_AND");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DUNGEON_MODE_NAND");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DUNGEON_MODE_OR");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DUNGEON_MODE_NOR");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"max_dlv");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wildc2i");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"reveal_wilderness_around_player");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"special_lvl");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"generate_special_feeling"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"store_type_new");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"store_type_del");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"store_type_dup");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"store_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"store_info_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"st_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"store_action_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ba_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"owner_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ow_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"STORE_HATED");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"STORE_LIKED");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"STORE_NORMAL");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"seed_flavor"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"seed_town"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"seed_dungeon"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"feature_type");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_f_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"f_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"vault_type");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_v_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"v_info");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"wild_see_through"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"use_corpse_decay"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"dungeon_generator");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"realtime");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"KILLWALL_FAILURE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"KILLWALL_WORKING");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"KILLWALL_DONE");
}
