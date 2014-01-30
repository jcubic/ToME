/*
** Lua binding: player
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:46 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_player_open (lua_State* tolua_S);
void tolua_player_close (lua_State* tolua_S);

#include "angband.h"
static void set_died_from(cptr from){strcpy(died_from, from);}
static char *lua_cnv_stat(s32b i) { static char buf[32]; cnv_stat(i, buf); return buf; }

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"player_descriptor");
 tolua_usertype(tolua_S,"monster_type");
 tolua_usertype(tolua_S,"move_info_type");
 tolua_usertype(tolua_S,"player_type");
 tolua_usertype(tolua_S,"tactic_info_type");
 tolua_usertype(tolua_S,"PHYSFS_file");
 tolua_usertype(tolua_S,"object_type");
 tolua_usertype(tolua_S,"flags_type");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* get function: PY_MAX_EXP */
static int toluaI_get_player_PY_MAX_EXP(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)PY_MAX_EXP);
 return 1;
}

/* set function: PY_MAX_EXP */
static int toluaI_set_player_PY_MAX_EXP(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  PY_MAX_EXP = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: PY_MAX_GOLD */
static int toluaI_get_player_PY_MAX_GOLD(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)PY_MAX_GOLD);
 return 1;
}

/* set function: PY_MAX_GOLD */
static int toluaI_set_player_PY_MAX_GOLD(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  PY_MAX_GOLD = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: PY_MAX_LEVEL */
static int toluaI_get_player_PY_MAX_LEVEL(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)PY_MAX_LEVEL);
 return 1;
}

/* set function: PY_MAX_LEVEL */
static int toluaI_set_player_PY_MAX_LEVEL(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  PY_MAX_LEVEL = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: player_exp */
static int toluaI_get_player_player_exp(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=PY_MAX_LEVEL)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)player_exp[toluaI_index]);
 return 1;
}

/* set function: player_exp */
static int toluaI_set_player_player_exp(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=PY_MAX_LEVEL)
 tolua_error(tolua_S,"array indexing out of range.");
  player_exp[toluaI_index] = ((s32b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: player_hp */
static int toluaI_get_player_player_hp(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=PY_MAX_LEVEL)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)player_hp[toluaI_index]);
 return 1;
}

/* set function: player_hp */
static int toluaI_set_player_player_hp(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=PY_MAX_LEVEL)
 tolua_error(tolua_S,"array indexing out of range.");
  player_hp[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: STATS_MAX */
static int toluaI_get_player_stats_MAX(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)STATS_MAX);
 return 1;
}

/* set function: STATS_MAX */
static int toluaI_set_player_stats_MAX(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  STATS_MAX = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: MIN_STAT_VALUE */
static int toluaI_get_player_stats_MIN_VALUE(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)MIN_STAT_VALUE);
 return 1;
}

/* set function: MIN_STAT_VALUE */
static int toluaI_set_player_stats_MIN_VALUE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  MIN_STAT_VALUE = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: MAX_STAT_VALUE */
static int toluaI_get_player_stats_MAX_VALUE(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)MAX_STAT_VALUE);
 return 1;
}

/* set function: MAX_STAT_VALUE */
static int toluaI_set_player_stats_MAX_VALUE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  MAX_STAT_VALUE = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: MAX_STAT_NATURAL */
static int toluaI_get_player_stats_MAX_NATURAL(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)MAX_STAT_NATURAL);
 return 1;
}

/* set function: MAX_STAT_NATURAL */
static int toluaI_set_player_stats_MAX_NATURAL(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  MAX_STAT_NATURAL = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: modify_stat_value */
static int toluaI_player_stats_modify_value00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b value = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b amount = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  modify_stat_value(value,amount);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'modify_value'.");
 return 0;
}

/* get function: SKILL_MAX */
static int toluaI_get_player_skills_MAX_VALUE(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)SKILL_MAX);
 return 1;
}

/* set function: SKILL_MAX */
static int toluaI_set_player_skills_MAX_VALUE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  SKILL_MAX = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: SKILL_STEP */
static int toluaI_get_player_skills_STEP_VALUE(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)SKILL_STEP);
 return 1;
}

/* set function: SKILL_STEP */
static int toluaI_set_player_skills_STEP_VALUE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  SKILL_STEP = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: dump_skills */
static int toluaI_player_skills_dump00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"PHYSFS_file"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  PHYSFS_file* fff = ((PHYSFS_file*)  tolua_getusertype(tolua_S,1,0));
 {
  dump_skills(fff);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dump'.");
 return 0;
}

/* get function: to_hit of class  tactic_info_type */
static int toluaI_get_player_tactic_info_type_to_hit(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_hit);
 return 1;
}

/* set function: to_hit of class  tactic_info_type */
static int toluaI_set_player_tactic_info_type_to_hit(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_hit = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_dam of class  tactic_info_type */
static int toluaI_get_player_tactic_info_type_to_dam(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_dam);
 return 1;
}

/* set function: to_dam of class  tactic_info_type */
static int toluaI_set_player_tactic_info_type_to_dam(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_dam = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_ac of class  tactic_info_type */
static int toluaI_get_player_tactic_info_type_to_ac(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_ac);
 return 1;
}

/* set function: to_ac of class  tactic_info_type */
static int toluaI_set_player_tactic_info_type_to_ac(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_ac = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_stealth of class  tactic_info_type */
static int toluaI_get_player_tactic_info_type_to_stealth(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_stealth);
 return 1;
}

/* set function: to_stealth of class  tactic_info_type */
static int toluaI_set_player_tactic_info_type_to_stealth(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_stealth = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_disarm of class  tactic_info_type */
static int toluaI_get_player_tactic_info_type_to_disarm(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_disarm);
 return 1;
}

/* set function: to_disarm of class  tactic_info_type */
static int toluaI_set_player_tactic_info_type_to_disarm(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_disarm = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_saving of class  tactic_info_type */
static int toluaI_get_player_tactic_info_type_to_saving(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_saving);
 return 1;
}

/* set function: to_saving of class  tactic_info_type */
static int toluaI_set_player_tactic_info_type_to_saving(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_saving = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  tactic_info_type */
static int toluaI_get_player_tactic_info_type_name(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  tactic_info_type */
static int toluaI_set_player_tactic_info_type_name(lua_State* tolua_S)
{
  tactic_info_type* self = (tactic_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: to_speed of class  move_info_type */
static int toluaI_get_player_move_info_type_to_speed(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_speed);
 return 1;
}

/* set function: to_speed of class  move_info_type */
static int toluaI_set_player_move_info_type_to_speed(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_speed = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_search of class  move_info_type */
static int toluaI_get_player_move_info_type_to_search(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_search);
 return 1;
}

/* set function: to_search of class  move_info_type */
static int toluaI_set_player_move_info_type_to_search(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_search = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_stealth of class  move_info_type */
static int toluaI_get_player_move_info_type_to_stealth(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_stealth);
 return 1;
}

/* set function: to_stealth of class  move_info_type */
static int toluaI_set_player_move_info_type_to_stealth(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_stealth = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_percep of class  move_info_type */
static int toluaI_get_player_move_info_type_to_percep(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_percep);
 return 1;
}

/* set function: to_percep of class  move_info_type */
static int toluaI_set_player_move_info_type_to_percep(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_percep = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  move_info_type */
static int toluaI_get_player_move_info_type_name(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  move_info_type */
static int toluaI_set_player_move_info_type_name(lua_State* tolua_S)
{
  move_info_type* self = (move_info_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: tactic_info */
static int toluaI_get_player_tactic_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&tactic_info[toluaI_index],tolua_tag(tolua_S,"tactic_info_type"));
 return 1;
}

/* set function: tactic_info */
static int toluaI_set_player_tactic_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
  tactic_info[toluaI_index] = *((tactic_info_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: move_info */
static int toluaI_get_player_move_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&move_info[toluaI_index],tolua_tag(tolua_S,"move_info_type"));
 return 1;
}

/* set function: move_info */
static int toluaI_set_player_move_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
  move_info[toluaI_index] = *((move_info_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: lives of class  player_type */
static int toluaI_get_player_player_type_lives(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->lives);
 return 1;
}

/* set function: lives of class  player_type */
static int toluaI_set_player_player_type_lives(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->lives = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: oldpy of class  player_type */
static int toluaI_get_player_player_type_oldpy(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->oldpy);
 return 1;
}

/* set function: oldpy of class  player_type */
static int toluaI_set_player_player_type_oldpy(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->oldpy = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: oldpx of class  player_type */
static int toluaI_get_player_player_type_oldpx(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->oldpx);
 return 1;
}

/* set function: oldpx of class  player_type */
static int toluaI_set_player_player_type_oldpx(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->oldpx = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: wild_force_side_y_ok of class  player_type */
static int toluaI_get_player_player_type_wild_force_side_y_ok(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->wild_force_side_y_ok);
 return 1;
}

/* set function: wild_force_side_y_ok of class  player_type */
static int toluaI_set_player_player_type_wild_force_side_y_ok(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->wild_force_side_y_ok = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: wild_force_side_x_ok of class  player_type */
static int toluaI_get_player_player_type_wild_force_side_x_ok(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->wild_force_side_x_ok);
 return 1;
}

/* set function: wild_force_side_x_ok of class  player_type */
static int toluaI_set_player_player_type_wild_force_side_x_ok(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->wild_force_side_x_ok = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: wild_force_side_y of class  player_type */
static int toluaI_get_player_player_type_wild_force_side_y(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->wild_force_side_y);
 return 1;
}

/* set function: wild_force_side_y of class  player_type */
static int toluaI_set_player_player_type_wild_force_side_y(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->wild_force_side_y = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: wild_force_side_x of class  player_type */
static int toluaI_get_player_player_type_wild_force_side_x(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->wild_force_side_x);
 return 1;
}

/* set function: wild_force_side_x of class  player_type */
static int toluaI_set_player_player_type_wild_force_side_x(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->wild_force_side_x = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: py of class  player_type */
static int toluaI_get_player_player_type_py(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->py);
 return 1;
}

/* set function: py of class  player_type */
static int toluaI_set_player_player_type_py(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->py = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: px of class  player_type */
static int toluaI_get_player_player_type_px(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->px);
 return 1;
}

/* set function: px of class  player_type */
static int toluaI_set_player_player_type_px(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->px = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: descriptor of class  player_type */
static int toluaI_get_player_player_type_descriptor(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->descriptor,tolua_tag(tolua_S,"player_descriptor"));
 return 1;
}

/* set function: descriptor of class  player_type */
static int toluaI_set_player_player_type_descriptor(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"player_descriptor"),0))
 TOLUA_ERR_ASSIGN;
  self->descriptor = *((player_descriptor*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: descriptors of class  player_type */
static int toluaI_get_player_player_type_descriptors(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->descriptors,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: descriptors of class  player_type */
static int toluaI_set_player_player_type_descriptors(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->descriptors = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: mimic_form of class  player_type */
static int toluaI_get_player_player_type_mimic_form(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mimic_form);
 return 1;
}

/* set function: mimic_form of class  player_type */
static int toluaI_set_player_player_type_mimic_form(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mimic_form = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mimic_level of class  player_type */
static int toluaI_get_player_player_type_mimic_level(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mimic_level);
 return 1;
}

/* set function: mimic_level of class  player_type */
static int toluaI_set_player_player_type_mimic_level(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mimic_level = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: oops of class  player_type */
static int toluaI_get_player_player_type_oops(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->oops);
 return 1;
}

/* set function: oops of class  player_type */
static int toluaI_set_player_player_type_oops(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->oops = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: inventory of class  player_type */
static int toluaI_get_player_player_type_inventory(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->inventory,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: inventory of class  player_type */
static int toluaI_set_player_player_type_inventory(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->inventory = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: hitdie of class  player_type */
static int toluaI_get_player_player_type_hitdie(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->hitdie);
 return 1;
}

/* set function: hitdie of class  player_type */
static int toluaI_set_player_player_type_hitdie(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->hitdie = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: expfact of class  player_type */
static int toluaI_get_player_player_type_expfact(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->expfact);
 return 1;
}

/* set function: expfact of class  player_type */
static int toluaI_set_player_player_type_expfact(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->expfact = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: maximize of class  player_type */
static int toluaI_get_player_player_type_maximize(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->maximize);
 return 1;
}

/* set function: maximize of class  player_type */
static int toluaI_set_player_player_type_maximize(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->maximize = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: preserve of class  player_type */
static int toluaI_get_player_player_type_preserve(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->preserve);
 return 1;
}

/* set function: preserve of class  player_type */
static int toluaI_set_player_player_type_preserve(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->preserve = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: special of class  player_type */
static int toluaI_get_player_player_type_special(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->special);
 return 1;
}

/* set function: special of class  player_type */
static int toluaI_set_player_player_type_special(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->special = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: allow_one_death of class  player_type */
static int toluaI_get_player_player_type_allow_one_death(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->allow_one_death);
 return 1;
}

/* set function: allow_one_death of class  player_type */
static int toluaI_set_player_player_type_allow_one_death(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->allow_one_death = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: age of class  player_type */
static int toluaI_get_player_player_type_age(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->age);
 return 1;
}

/* set function: age of class  player_type */
static int toluaI_set_player_player_type_age(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->age = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ht of class  player_type */
static int toluaI_get_player_player_type_ht(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ht);
 return 1;
}

/* set function: ht of class  player_type */
static int toluaI_set_player_player_type_ht(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ht = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: wt of class  player_type */
static int toluaI_get_player_player_type_wt(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->wt);
 return 1;
}

/* set function: wt of class  player_type */
static int toluaI_set_player_player_type_wt(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->wt = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sc of class  player_type */
static int toluaI_get_player_player_type_sc(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sc);
 return 1;
}

/* set function: sc of class  player_type */
static int toluaI_set_player_player_type_sc(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sc = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: au of class  player_type */
static int toluaI_get_player_player_type_au(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->au);
 return 1;
}

/* set function: au of class  player_type */
static int toluaI_set_player_player_type_au(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->au = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_exp of class  player_type */
static int toluaI_get_player_player_type_max_exp(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_exp);
 return 1;
}

/* set function: max_exp of class  player_type */
static int toluaI_set_player_player_type_max_exp(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_exp = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: exp of class  player_type */
static int toluaI_get_player_player_type_exp(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->exp);
 return 1;
}

/* set function: exp of class  player_type */
static int toluaI_set_player_player_type_exp(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->exp = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: exp_frac of class  player_type */
static int toluaI_get_player_player_type_exp_frac(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->exp_frac);
 return 1;
}

/* set function: exp_frac of class  player_type */
static int toluaI_set_player_player_type_exp_frac(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->exp_frac = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: lev of class  player_type */
static int toluaI_get_player_player_type_lev(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->lev);
 return 1;
}

/* set function: lev of class  player_type */
static int toluaI_set_player_player_type_lev(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->lev = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: town_num of class  player_type */
static int toluaI_get_player_player_type_town_num(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->town_num);
 return 1;
}

/* set function: town_num of class  player_type */
static int toluaI_set_player_player_type_town_num(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->town_num = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: inside_quest of class  player_type */
static int toluaI_get_player_player_type_inside_quest(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->inside_quest);
 return 1;
}

/* set function: inside_quest of class  player_type */
static int toluaI_set_player_player_type_inside_quest(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->inside_quest = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: exit_bldg of class  player_type */
static int toluaI_get_player_player_type_exit_bldg(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->exit_bldg);
 return 1;
}

/* set function: exit_bldg of class  player_type */
static int toluaI_set_player_player_type_exit_bldg(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->exit_bldg = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: wilderness_x of class  player_type */
static int toluaI_get_player_player_type_wilderness_x(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->wilderness_x);
 return 1;
}

/* set function: wilderness_x of class  player_type */
static int toluaI_set_player_player_type_wilderness_x(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->wilderness_x = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: wilderness_y of class  player_type */
static int toluaI_get_player_player_type_wilderness_y(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->wilderness_y);
 return 1;
}

/* set function: wilderness_y of class  player_type */
static int toluaI_set_player_player_type_wilderness_y(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->wilderness_y = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: wild_mode of class  player_type */
static int toluaI_get_player_player_type_wild_mode(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->wild_mode);
 return 1;
}

/* set function: wild_mode of class  player_type */
static int toluaI_set_player_player_type_wild_mode(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->wild_mode = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: old_wild_mode of class  player_type */
static int toluaI_get_player_player_type_old_wild_mode(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->old_wild_mode);
 return 1;
}

/* set function: old_wild_mode of class  player_type */
static int toluaI_set_player_player_type_old_wild_mode(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->old_wild_mode = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: hp_mod of class  player_type */
static int toluaI_get_player_player_type_hp_mod(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->hp_mod);
 return 1;
}

/* set function: hp_mod of class  player_type */
static int toluaI_set_player_player_type_hp_mod(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->hp_mod = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_plv of class  player_type */
static int toluaI_get_player_player_type_max_plv(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_plv);
 return 1;
}

/* set function: max_plv of class  player_type */
static int toluaI_set_player_player_type_max_plv(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_plv = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: luck_cur of class  player_type */
static int toluaI_get_player_player_type_luck_cur(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->luck_cur);
 return 1;
}

/* set function: luck_cur of class  player_type */
static int toluaI_set_player_player_type_luck_cur(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->luck_cur = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: luck_max of class  player_type */
static int toluaI_get_player_player_type_luck_max(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->luck_max);
 return 1;
}

/* set function: luck_max of class  player_type */
static int toluaI_set_player_player_type_luck_max(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->luck_max = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: luck_base of class  player_type */
static int toluaI_get_player_player_type_luck_base(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->luck_base);
 return 1;
}

/* set function: luck_base of class  player_type */
static int toluaI_set_player_player_type_luck_base(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->luck_base = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: cur_lite of class  player_type */
static int toluaI_get_player_player_type_cur_lite(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->cur_lite);
 return 1;
}

/* set function: cur_lite of class  player_type */
static int toluaI_set_player_player_type_cur_lite(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->cur_lite = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: pspeed of class  player_type */
static int toluaI_get_player_player_type_pspeed(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->pspeed);
 return 1;
}

/* set function: pspeed of class  player_type */
static int toluaI_set_player_player_type_pspeed(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->pspeed = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: speeds of class  player_type */
static int toluaI_get_player_player_type_speeds(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->speeds,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: speeds of class  player_type */
static int toluaI_set_player_player_type_speeds(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->speeds = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: notice of class  player_type */
static int toluaI_get_player_player_type_notice(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->notice);
 return 1;
}

/* set function: notice of class  player_type */
static int toluaI_set_player_player_type_notice(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->notice = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: update of class  player_type */
static int toluaI_get_player_player_type_update(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->update);
 return 1;
}

/* set function: update of class  player_type */
static int toluaI_set_player_player_type_update(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->update = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: redraw of class  player_type */
static int toluaI_get_player_player_type_redraw(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->redraw,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: redraw of class  player_type */
static int toluaI_set_player_player_type_redraw(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->redraw = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: window of class  player_type */
static int toluaI_get_player_player_type_window(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->window,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: window of class  player_type */
static int toluaI_set_player_player_type_window(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->window = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: recall_dungeon of class  player_type */
static int toluaI_get_player_player_type_recall_dungeon(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->recall_dungeon);
 return 1;
}

/* set function: recall_dungeon of class  player_type */
static int toluaI_set_player_player_type_recall_dungeon(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->recall_dungeon = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: word_recall of class  player_type */
static int toluaI_get_player_player_type_word_recall(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->word_recall);
 return 1;
}

/* set function: word_recall of class  player_type */
static int toluaI_set_player_player_type_word_recall(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->word_recall = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: energy of class  player_type */
static int toluaI_get_player_player_type_energy(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->energy);
 return 1;
}

/* set function: energy of class  player_type */
static int toluaI_set_player_player_type_energy(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->energy = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: stat_max of class  player_type */
static int toluaI_get_player_player_type_stat_max(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->stat_max,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: stat_max of class  player_type */
static int toluaI_set_player_player_type_stat_max(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->stat_max = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: stat_cur of class  player_type */
static int toluaI_get_player_player_type_stat_cur(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->stat_cur,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: stat_cur of class  player_type */
static int toluaI_set_player_player_type_stat_cur(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->stat_cur = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: stat_add of class  player_type */
static int toluaI_get_player_player_type_stat_add(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->stat_add,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: stat_add of class  player_type */
static int toluaI_set_player_player_type_stat_add(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->stat_add = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: stat_ind of class  player_type */
static int toluaI_get_player_player_type_stat_ind(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->stat_ind,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: stat_ind of class  player_type */
static int toluaI_set_player_player_type_stat_ind(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->stat_ind = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: stat_top of class  player_type */
static int toluaI_get_player_player_type_stat_top(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->stat_top,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: stat_top of class  player_type */
static int toluaI_set_player_player_type_stat_top(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->stat_top = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: stat_cnt of class  player_type */
static int toluaI_get_player_player_type_stat_cnt(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->stat_cnt,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: stat_cnt of class  player_type */
static int toluaI_set_player_player_type_stat_cnt(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->stat_cnt = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: stat_los of class  player_type */
static int toluaI_get_player_player_type_stat_los(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->stat_los,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: stat_los of class  player_type */
static int toluaI_set_player_player_type_stat_los(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->stat_los = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: intrinsic of class  player_type */
static int toluaI_get_player_player_type_intrinsic_flags(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->intrinsic,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: intrinsic of class  player_type */
static int toluaI_set_player_player_type_intrinsic_flags(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->intrinsic = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: resists of class  player_type */
static int toluaI_get_player_player_type_resists(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->resists,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: resists of class  player_type */
static int toluaI_set_player_player_type_resists(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->resists = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: absorbs of class  player_type */
static int toluaI_get_player_player_type_absorbs(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->absorbs,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: absorbs of class  player_type */
static int toluaI_set_player_player_type_absorbs(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->absorbs = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: skill_bonuses of class  player_type */
static int toluaI_get_player_player_type_skill_bonuses(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->skill_bonuses,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: skill_bonuses of class  player_type */
static int toluaI_set_player_player_type_skill_bonuses(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->skill_bonuses = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: cumber_armor of class  player_type */
static int toluaI_get_player_player_type_cumber_armor(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->cumber_armor);
 return 1;
}

/* set function: cumber_armor of class  player_type */
static int toluaI_set_player_player_type_cumber_armor(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->cumber_armor = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: cumber_glove of class  player_type */
static int toluaI_get_player_player_type_cumber_glove(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->cumber_glove);
 return 1;
}

/* set function: cumber_glove of class  player_type */
static int toluaI_set_player_player_type_cumber_glove(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->cumber_glove = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: heavy_wield of class  player_type */
static int toluaI_get_player_player_type_heavy_wield(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->heavy_wield);
 return 1;
}

/* set function: heavy_wield of class  player_type */
static int toluaI_set_player_player_type_heavy_wield(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->heavy_wield = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: heavy_shoot of class  player_type */
static int toluaI_get_player_player_type_heavy_shoot(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->heavy_shoot);
 return 1;
}

/* set function: heavy_shoot of class  player_type */
static int toluaI_set_player_player_type_heavy_shoot(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->heavy_shoot = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: icky_wield of class  player_type */
static int toluaI_get_player_player_type_icky_wield(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->icky_wield);
 return 1;
}

/* set function: icky_wield of class  player_type */
static int toluaI_set_player_player_type_icky_wield(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->icky_wield = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: dis_to_h of class  player_type */
static int toluaI_get_player_player_type_dis_to_h(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dis_to_h);
 return 1;
}

/* set function: dis_to_h of class  player_type */
static int toluaI_set_player_player_type_dis_to_h(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dis_to_h = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dis_to_d of class  player_type */
static int toluaI_get_player_player_type_dis_to_d(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dis_to_d);
 return 1;
}

/* set function: dis_to_d of class  player_type */
static int toluaI_set_player_player_type_dis_to_d(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dis_to_d = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dis_to_a of class  player_type */
static int toluaI_get_player_player_type_dis_to_a(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dis_to_a);
 return 1;
}

/* set function: dis_to_a of class  player_type */
static int toluaI_set_player_player_type_dis_to_a(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dis_to_a = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dis_ac of class  player_type */
static int toluaI_get_player_player_type_dis_ac(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dis_ac);
 return 1;
}

/* set function: dis_ac of class  player_type */
static int toluaI_set_player_player_type_dis_ac(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dis_ac = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_l of class  player_type */
static int toluaI_get_player_player_type_to_l(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_l);
 return 1;
}

/* set function: to_l of class  player_type */
static int toluaI_set_player_player_type_to_l(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_l = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_m of class  player_type */
static int toluaI_get_player_player_type_to_m(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_m);
 return 1;
}

/* set function: to_m of class  player_type */
static int toluaI_set_player_player_type_to_m(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_m = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_s of class  player_type */
static int toluaI_get_player_player_type_to_s(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_s);
 return 1;
}

/* set function: to_s of class  player_type */
static int toluaI_set_player_player_type_to_s(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_s = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_h of class  player_type */
static int toluaI_get_player_player_type_to_h(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_h);
 return 1;
}

/* set function: to_h of class  player_type */
static int toluaI_set_player_player_type_to_h(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_h = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_d of class  player_type */
static int toluaI_get_player_player_type_to_d(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_d);
 return 1;
}

/* set function: to_d of class  player_type */
static int toluaI_set_player_player_type_to_d(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_d = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_a of class  player_type */
static int toluaI_get_player_player_type_to_a(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_a);
 return 1;
}

/* set function: to_a of class  player_type */
static int toluaI_set_player_player_type_to_a(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_a = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: num_blow of class  player_type */
static int toluaI_get_player_player_type_num_blow(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->num_blow);
 return 1;
}

/* set function: num_blow of class  player_type */
static int toluaI_set_player_player_type_num_blow(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->num_blow = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: num_fire of class  player_type */
static int toluaI_get_player_player_type_num_fire(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->num_fire);
 return 1;
}

/* set function: num_fire of class  player_type */
static int toluaI_set_player_player_type_num_fire(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->num_fire = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: extra_blows of class  player_type */
static int toluaI_get_player_player_type_extra_blows(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->extra_blows);
 return 1;
}

/* set function: extra_blows of class  player_type */
static int toluaI_set_player_player_type_extra_blows(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->extra_blows = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: extra_shots of class  player_type */
static int toluaI_get_player_player_type_extra_shots(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->extra_shots);
 return 1;
}

/* set function: extra_shots of class  player_type */
static int toluaI_set_player_player_type_extra_shots(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->extra_shots = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ac of class  player_type */
static int toluaI_get_player_player_type_ac(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ac);
 return 1;
}

/* set function: ac of class  player_type */
static int toluaI_set_player_player_type_ac(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ac = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: antimagic of class  player_type */
static int toluaI_get_player_player_type_antimagic(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->antimagic);
 return 1;
}

/* set function: antimagic of class  player_type */
static int toluaI_set_player_player_type_antimagic(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->antimagic = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: antimagic_dis of class  player_type */
static int toluaI_get_player_player_type_antimagic_dis(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->antimagic_dis);
 return 1;
}

/* set function: antimagic_dis of class  player_type */
static int toluaI_set_player_player_type_antimagic_dis(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->antimagic_dis = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: see_infra of class  player_type */
static int toluaI_get_player_player_type_see_infra(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->see_infra);
 return 1;
}

/* set function: see_infra of class  player_type */
static int toluaI_set_player_player_type_see_infra(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->see_infra = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: skill_dis of class  player_type */
static int toluaI_get_player_player_type_skill_dis(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->skill_dis);
 return 1;
}

/* set function: skill_dis of class  player_type */
static int toluaI_set_player_player_type_skill_dis(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->skill_dis = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: skill_dev of class  player_type */
static int toluaI_get_player_player_type_skill_dev(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->skill_dev);
 return 1;
}

/* set function: skill_dev of class  player_type */
static int toluaI_set_player_player_type_skill_dev(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->skill_dev = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: skill_sav of class  player_type */
static int toluaI_get_player_player_type_skill_sav(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->skill_sav);
 return 1;
}

/* set function: skill_sav of class  player_type */
static int toluaI_set_player_player_type_skill_sav(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->skill_sav = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: skill_stl of class  player_type */
static int toluaI_get_player_player_type_skill_stl(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->skill_stl);
 return 1;
}

/* set function: skill_stl of class  player_type */
static int toluaI_set_player_player_type_skill_stl(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->skill_stl = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: skill_thn of class  player_type */
static int toluaI_get_player_player_type_skill_thn(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->skill_thn);
 return 1;
}

/* set function: skill_thn of class  player_type */
static int toluaI_set_player_player_type_skill_thn(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->skill_thn = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: skill_thb of class  player_type */
static int toluaI_get_player_player_type_skill_thb(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->skill_thb);
 return 1;
}

/* set function: skill_thb of class  player_type */
static int toluaI_set_player_player_type_skill_thb(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->skill_thb = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: skill_tht of class  player_type */
static int toluaI_get_player_player_type_skill_tht(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->skill_tht);
 return 1;
}

/* set function: skill_tht of class  player_type */
static int toluaI_set_player_player_type_skill_tht(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->skill_tht = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: skill_points of class  player_type */
static int toluaI_get_player_player_type_skill_points(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->skill_points);
 return 1;
}

/* set function: skill_points of class  player_type */
static int toluaI_set_player_player_type_skill_points(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->skill_points = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: combat_style of class  player_type */
static int toluaI_get_player_player_type_combat_style(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->combat_style);
 return 1;
}

/* set function: combat_style of class  player_type */
static int toluaI_set_player_player_type_combat_style(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->combat_style = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: use_piercing_shots of class  player_type */
static int toluaI_get_player_player_type_use_piercing_shots(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->use_piercing_shots);
 return 1;
}

/* set function: use_piercing_shots of class  player_type */
static int toluaI_set_player_player_type_use_piercing_shots(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->use_piercing_shots = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: control of class  player_type */
static int toluaI_get_player_player_type_control(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->control);
 return 1;
}

/* set function: control of class  player_type */
static int toluaI_set_player_player_type_control(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->control = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: control_dir of class  player_type */
static int toluaI_get_player_player_type_control_dir(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->control_dir);
 return 1;
}

/* set function: control_dir of class  player_type */
static int toluaI_set_player_player_type_control_dir(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->control_dir = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: companion_killed of class  player_type */
static int toluaI_get_player_player_type_companion_killed(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->companion_killed);
 return 1;
}

/* set function: companion_killed of class  player_type */
static int toluaI_set_player_player_type_companion_killed(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->companion_killed = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: black_breath of class  player_type */
static int toluaI_get_player_player_type_black_breath(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->black_breath);
 return 1;
}

/* set function: black_breath of class  player_type */
static int toluaI_set_player_player_type_black_breath(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->black_breath = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: body_monster of class  player_type */
static int toluaI_get_player_player_type_body_monster(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->body_monster,tolua_tag(tolua_S,"monster_type"));
 return 1;
}

/* set function: body_monster of class  player_type */
static int toluaI_set_player_player_type_body_monster(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"monster_type"),0))
 TOLUA_ERR_ASSIGN;
  self->body_monster = *((monster_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: extra_body_parts of class  player_type */
static int toluaI_get_player_player_type_extra_body_parts(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->extra_body_parts,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: extra_body_parts of class  player_type */
static int toluaI_set_player_player_type_extra_body_parts(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->extra_body_parts = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: powers of class  player_type */
static int toluaI_get_player_player_type_powers(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->powers,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: powers of class  player_type */
static int toluaI_set_player_player_type_powers(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->powers = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: powers_mod of class  player_type */
static int toluaI_get_player_player_type_powers_mod(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->powers_mod,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: powers_mod of class  player_type */
static int toluaI_set_player_player_type_powers_mod(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->powers_mod = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: necro_extra of class  player_type */
static int toluaI_get_player_player_type_necro_extra(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->necro_extra);
 return 1;
}

/* set function: necro_extra of class  player_type */
static int toluaI_set_player_player_type_necro_extra(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->necro_extra = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  player_type */
static int toluaI_get_player_player_type_flags(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  player_type */
static int toluaI_set_player_player_type_flags(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: tactic of class  player_type */
static int toluaI_get_player_player_type_tactic(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->tactic);
 return 1;
}

/* set function: tactic of class  player_type */
static int toluaI_set_player_player_type_tactic(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->tactic = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: movement of class  player_type */
static int toluaI_get_player_player_type_movement(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->movement);
 return 1;
}

/* set function: movement of class  player_type */
static int toluaI_set_player_player_type_movement(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->movement = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: leaving of class  player_type */
static int toluaI_get_player_player_type_leaving(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->leaving);
 return 1;
}

/* set function: leaving of class  player_type */
static int toluaI_set_player_player_type_leaving(lua_State* tolua_S)
{
  player_type* self = (player_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->leaving = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: energy_use */
static int toluaI_get_player_energy_use(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)energy_use);
 return 1;
}

/* set function: energy_use */
static int toluaI_set_player_energy_use(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  energy_use = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: alive */
static int toluaI_get_player_alive(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)alive);
 return 1;
}

/* set function: alive */
static int toluaI_set_player_alive(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  alive = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: death */
static int toluaI_get_player_death(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)death);
 return 1;
}

/* set function: death */
static int toluaI_set_player_death(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  death = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: set_died_from */
static int toluaI_player_set_died_from00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr from = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  set_died_from(from);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_died_from'.");
 return 0;
}

/* get function: p_ptr */
static int toluaI_get_player_player(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)p_ptr,tolua_tag(tolua_S,"player_type"));
 return 1;
}

/* set function: p_ptr */
static int toluaI_set_player_player(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"player_type"),0))
 TOLUA_ERR_ASSIGN;
  p_ptr = ((player_type*)  tolua_getusertype(tolua_S,1,0));
 return 0;
}

/* get function: max_dp_idx */
static int toluaI_get_player_max_dp_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_dp_idx);
 return 1;
}

/* set function: max_dp_idx */
static int toluaI_set_player_max_dp_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_dp_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: descriptor_info */
static int toluaI_get_player_descriptor_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_dp_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&descriptor_info[toluaI_index],tolua_tag(tolua_S,"player_descriptor"));
 return 1;
}

/* set function: descriptor_info */
static int toluaI_set_player_descriptor_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_dp_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  descriptor_info[toluaI_index] = *((player_descriptor*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: flush_failure */
static int toluaI_get_player_flush_failure(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)flush_failure);
 return 1;
}

/* set function: flush_failure */
static int toluaI_set_player_flush_failure(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  flush_failure = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: check_experience */
static int toluaI_player_check_experience00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  check_experience();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'check_experience'.");
 return 0;
}

/* function: check_experience_obj */
static int toluaI_player_check_experience_obj00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
 {
  check_experience_obj(o_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'check_experience_obj'.");
 return 0;
}

/* function: gain_exp */
static int toluaI_player_gain_exp00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b amount = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  gain_exp(amount);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gain_exp'.");
 return 0;
}

/* function: lose_exp */
static int toluaI_player_lose_exp00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b amount = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  lose_exp(amount);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lose_exp'.");
 return 0;
}

/* function: no_lite */
static int toluaI_player_no_lite00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  no_lite();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'no_lite'.");
 return 0;
}

/* function: do_cmd_debug */
static int toluaI_player_do_cmd_debug00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  do_cmd_debug();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_cmd_debug'.");
 return 0;
}

/* get function: dun_level */
static int toluaI_get_player_dun_level(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)dun_level);
 return 1;
}

/* set function: dun_level */
static int toluaI_set_player_dun_level(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  dun_level = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: old_dun_level */
static int toluaI_get_player_old_dun_level(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)old_dun_level);
 return 1;
}

/* set function: old_dun_level */
static int toluaI_set_player_old_dun_level(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  old_dun_level = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: change_wild_mode */
static int toluaI_player_change_wild_mode00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  change_wild_mode();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'change_wild_mode'.");
 return 0;
}

/* get function: wizard */
static int toluaI_get_player_wizard(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)wizard);
 return 1;
}

/* set function: wizard */
static int toluaI_set_player_wizard(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  wizard = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: noscore */
static int toluaI_get_player_noscore(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)noscore);
 return 1;
}

/* set function: noscore */
static int toluaI_set_player_noscore(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  noscore = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: total_winner */
static int toluaI_get_player_winner_state(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)total_winner);
 return 1;
}

/* set function: total_winner */
static int toluaI_set_player_winner_state(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  total_winner = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: has_won */
static int toluaI_get_player_has_won(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)has_won);
 return 1;
}

/* set function: has_won */
static int toluaI_set_player_has_won(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  has_won = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: joke_monsters */
static int toluaI_get_player_joke_monsters(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)joke_monsters);
 return 1;
}

/* set function: joke_monsters */
static int toluaI_set_player_joke_monsters(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  joke_monsters = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: exp_need */
static int toluaI_get_player_exp_need(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)exp_need);
 return 1;
}

/* set function: exp_need */
static int toluaI_set_player_exp_need(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  exp_need = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: linear_stats */
static int toluaI_get_player_linear_stats(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)linear_stats);
 return 1;
}

/* set function: linear_stats */
static int toluaI_set_player_linear_stats(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  linear_stats = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: player_char_health */
static int toluaI_get_player_player_char_health(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)player_char_health);
 return 1;
}

/* set function: player_char_health */
static int toluaI_set_player_player_char_health(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  player_char_health = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: hitpoint_warn */
static int toluaI_get_player_hitpoint_warn(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)hitpoint_warn);
 return 1;
}

/* set function: hitpoint_warn */
static int toluaI_set_player_hitpoint_warn(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  hitpoint_warn = ((byte)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: lua_cnv_stat */
static int toluaI_player_cnv_stat00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b i = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  char* toluaI_ret = (char*)  lua_cnv_stat(i);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cnv_stat'.");
 return 0;
}

/* get function: command_cmd */
static int toluaI_get_player_command_cmd(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)command_cmd);
 return 1;
}

/* set function: command_cmd */
static int toluaI_set_player_command_cmd(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  command_cmd = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: command_arg */
static int toluaI_get_player_command_arg(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)command_arg);
 return 1;
}

/* set function: command_arg */
static int toluaI_set_player_command_arg(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  command_arg = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: command_rep */
static int toluaI_get_player_command_rep(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)command_rep);
 return 1;
}

/* set function: command_rep */
static int toluaI_set_player_command_rep(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  command_rep = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: command_dir */
static int toluaI_get_player_command_dir(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)command_dir);
 return 1;
}

/* set function: command_dir */
static int toluaI_set_player_command_dir(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  command_dir = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: command_see */
static int toluaI_get_player_command_see(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)command_see);
 return 1;
}

/* set function: command_see */
static int toluaI_set_player_command_see(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  command_see = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: command_wrk */
static int toluaI_get_player_command_wrk(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)command_wrk);
 return 1;
}

/* set function: command_wrk */
static int toluaI_set_player_command_wrk(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  command_wrk = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: command_new */
static int toluaI_get_player_command_new(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)command_new);
 return 1;
}

/* set function: command_new */
static int toluaI_set_player_command_new(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  command_new = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: command_new_do_keymap */
static int toluaI_get_player_command_new_do_keymap(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)command_new_do_keymap);
 return 1;
}

/* set function: command_new_do_keymap */
static int toluaI_set_player_command_new_do_keymap(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  command_new_do_keymap = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: leaving_quest */
static int toluaI_get_player_leaving_quest(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)leaving_quest);
 return 1;
}

/* set function: leaving_quest */
static int toluaI_set_player_leaving_quest(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  leaving_quest = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: get_player_energy */
static int toluaI_player_get_player_energy00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b speed_id = ((s32b)  tolua_getnumber(tolua_S,1,-1));
 {
  u16b toluaI_ret = (u16b)  get_player_energy(speed_id);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_player_energy'.");
 return 0;
}

/* function: get_speed_name */
static int toluaI_player_get_speed_name00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  u32b index = ((u32b)  tolua_getnumber(tolua_S,1,0));
 {
  cptr toluaI_ret = (cptr)  get_speed_name(index);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_speed_name'.");
 return 0;
}

/* function: get_speed_desc */
static int toluaI_player_get_speed_desc00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  u32b index = ((u32b)  tolua_getnumber(tolua_S,1,0));
 {
  cptr toluaI_ret = (cptr)  get_speed_desc(index);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_speed_desc'.");
 return 0;
}

/* function: register_speed */
static int toluaI_player_register_speed00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cptr name = ((cptr)  tolua_getstring(tolua_S,1,0));
  cptr desc = ((cptr)  tolua_getstring(tolua_S,2,0));
 {
  u32b toluaI_ret = (u32b)  register_speed(name,desc);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'register_speed'.");
 return 0;
}

/* function: notice_stuff */
static int toluaI_player_notice_stuff00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  notice_stuff();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'notice_stuff'.");
 return 0;
}

/* function: update_stuff */
static int toluaI_player_update_stuff00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  update_stuff();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update_stuff'.");
 return 0;
}

/* function: redraw_stuff */
static int toluaI_player_redraw_stuff00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  redraw_stuff();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'redraw_stuff'.");
 return 0;
}

/* function: window_stuff */
static int toluaI_player_window_stuff00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  window_stuff();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'window_stuff'.");
 return 0;
}

/* function: handle_stuff */
static int toluaI_player_handle_stuff00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  handle_stuff();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'handle_stuff'.");
 return 0;
}

/* get function: history */
static int toluaI_get_player_history(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushstring(tolua_S,(const char*)history[toluaI_index]);
 return 1;
}

/* get function: character_generated */
static int toluaI_get_player_character_generated(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)character_generated);
 return 1;
}

/* set function: character_generated */
static int toluaI_set_player_character_generated(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  character_generated = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: character_dungeon */
static int toluaI_get_player_character_dungeon(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)character_dungeon);
 return 1;
}

/* set function: character_dungeon */
static int toluaI_set_player_character_dungeon(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  character_dungeon = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: character_loaded */
static int toluaI_get_player_character_loaded(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)character_loaded);
 return 1;
}

/* set function: character_loaded */
static int toluaI_set_player_character_loaded(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  character_loaded = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: character_saved */
static int toluaI_get_player_character_saved(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)character_saved);
 return 1;
}

/* set function: character_saved */
static int toluaI_set_player_character_saved(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  character_saved = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: character_xtra */
static int toluaI_get_player_character_xtra(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)character_xtra);
 return 1;
}

/* set function: character_xtra */
static int toluaI_set_player_character_xtra(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  character_xtra = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: browse_school_spell */
static int toluaI_player_browse_school_spell00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
 {
  browse_school_spell(o_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'browse_school_spell'.");
 return 0;
}

/* function: do_cmd_open_obj */
static int toluaI_player_do_cmd_open_obj00(lua_State* tolua_S)
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
  s16b o_idx = ((s16b)  tolua_getnumber(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  do_cmd_open_obj(y,x,o_idx);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_cmd_open_obj'.");
 return 0;
}

/* function: do_cmd_open_aux */
static int toluaI_player_do_cmd_open_aux00(lua_State* tolua_S)
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
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  do_cmd_open_aux(y,x,dir);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_cmd_open_aux'.");
 return 0;
}

/* get function: no_begin_screen */
static int toluaI_get_player_birth_no_begin_screen(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)no_begin_screen);
 return 1;
}

/* set function: no_begin_screen */
static int toluaI_set_player_birth_no_begin_screen(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  no_begin_screen = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: force_new_game */
static int toluaI_get_player_birth_force_new_game(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)force_new_game);
 return 1;
}

/* set function: force_new_game */
static int toluaI_set_player_birth_force_new_game(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  force_new_game = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: do_cmd_skill */
static int toluaI_player_do_cmd_skill00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  do_cmd_skill();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_cmd_skill'.");
 return 0;
}

/* function: player_birth_skill */
static int toluaI_player_player_birth_skill00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  player_birth_skill();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'player_birth_skill'.");
 return 0;
}

/* function: change_player_body */
static int toluaI_player_change_player_body00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"monster_type"),1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  monster_type* new_body = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  monster_type* old_body = ((monster_type*)  tolua_getusertype(tolua_S,2,NULL));
 {
  bool toluaI_ret = (bool)  change_player_body(new_body,old_body);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'change_player_body'.");
 return 0;
}

/* function: do_cmd_suicide */
static int toluaI_player_do_cmd_suicide00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  do_cmd_suicide();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_cmd_suicide'.");
 return 0;
}

/* Open function */
int tolua_player_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_constant(tolua_S,NULL,"SUBRACE_PLACE_AFTER",SUBRACE_PLACE_AFTER);
 tolua_constant(tolua_S,NULL,"SUBRACE_PLACE_BEFORE",SUBRACE_PLACE_BEFORE);
 tolua_constant(tolua_S,NULL,"SUBRACE_PLACE_REPLACE",SUBRACE_PLACE_REPLACE);
 tolua_globalvar(tolua_S,"PY_MAX_EXP",toluaI_get_player_PY_MAX_EXP,toluaI_set_player_PY_MAX_EXP);
 tolua_globalvar(tolua_S,"PY_MAX_GOLD",toluaI_get_player_PY_MAX_GOLD,toluaI_set_player_PY_MAX_GOLD);
 tolua_globalvar(tolua_S,"PY_MAX_LEVEL",toluaI_get_player_PY_MAX_LEVEL,toluaI_set_player_PY_MAX_LEVEL);
 tolua_globalarray(tolua_S,"player_exp",toluaI_get_player_player_exp,toluaI_set_player_player_exp);
 tolua_globalarray(tolua_S,"player_hp",toluaI_get_player_player_hp,toluaI_set_player_player_hp);
 tolua_module(tolua_S,"stats");
 tolua_tablevar(tolua_S,"stats","MAX",toluaI_get_player_stats_MAX,toluaI_set_player_stats_MAX);
 tolua_tablevar(tolua_S,"stats","MIN_VALUE",toluaI_get_player_stats_MIN_VALUE,toluaI_set_player_stats_MIN_VALUE);
 tolua_tablevar(tolua_S,"stats","MAX_VALUE",toluaI_get_player_stats_MAX_VALUE,toluaI_set_player_stats_MAX_VALUE);
 tolua_tablevar(tolua_S,"stats","MAX_NATURAL",toluaI_get_player_stats_MAX_NATURAL,toluaI_set_player_stats_MAX_NATURAL);
 tolua_function(tolua_S,"stats","modify_value",toluaI_player_stats_modify_value00);
 tolua_module(tolua_S,"skills");
 tolua_tablevar(tolua_S,"skills","MAX_VALUE",toluaI_get_player_skills_MAX_VALUE,toluaI_set_player_skills_MAX_VALUE);
 tolua_tablevar(tolua_S,"skills","STEP_VALUE",toluaI_get_player_skills_STEP_VALUE,toluaI_set_player_skills_STEP_VALUE);
 tolua_function(tolua_S,"skills","dump",toluaI_player_skills_dump00);
 tolua_constant(tolua_S,NULL,"SUBRACE_SAVE",SUBRACE_SAVE);
 tolua_constant(tolua_S,NULL,"SEX_FEMALE",SEX_FEMALE);
 tolua_constant(tolua_S,NULL,"SEX_MALE",SEX_MALE);
 tolua_constant(tolua_S,NULL,"SEX_NEUTER",SEX_NEUTER);
 tolua_constant(tolua_S,NULL,"PN_COMBINE",PN_COMBINE);
 tolua_constant(tolua_S,NULL,"PN_REORDER",PN_REORDER);
 tolua_constant(tolua_S,NULL,"PU_BONUS",PU_BONUS);
 tolua_constant(tolua_S,NULL,"PU_TORCH",PU_TORCH);
 tolua_constant(tolua_S,NULL,"PU_BODY",PU_BODY);
 tolua_constant(tolua_S,NULL,"PU_SANITY",PU_SANITY);
 tolua_constant(tolua_S,NULL,"PU_HP",PU_HP);
 tolua_constant(tolua_S,NULL,"PU_MANA",PU_MANA);
 tolua_constant(tolua_S,NULL,"PU_SPELLS",PU_SPELLS);
 tolua_constant(tolua_S,NULL,"PU_POWERS",PU_POWERS);
 tolua_constant(tolua_S,NULL,"PU_UN_VIEW",PU_UN_VIEW);
 tolua_constant(tolua_S,NULL,"PU_VIEW",PU_VIEW);
 tolua_constant(tolua_S,NULL,"PU_MON_LITE",PU_MON_LITE);
 tolua_constant(tolua_S,NULL,"PU_MONSTERS",PU_MONSTERS);
 tolua_constant(tolua_S,NULL,"PU_DISTANCE",PU_DISTANCE);
 tolua_constant(tolua_S,NULL,"PU_FLOW",PU_FLOW);
 tolua_constant(tolua_S,NULL,"CLASS_MANA_PATH_ERASE",CLASS_MANA_PATH_ERASE);
 tolua_constant(tolua_S,NULL,"CLASS_FLOOD_LEVEL",CLASS_FLOOD_LEVEL);
 tolua_constant(tolua_S,NULL,"CLASS_CANALIZE_MANA_EXTRA",CLASS_CANALIZE_MANA_EXTRA);
 tolua_constant(tolua_S,NULL,"CLASS_UNDEAD",CLASS_UNDEAD);
 tolua_constant(tolua_S,NULL,"CLASS_ANTIMAGIC",CLASS_ANTIMAGIC);
 tolua_constant(tolua_S,NULL,"CLASS_LEGS",CLASS_LEGS);
 tolua_constant(tolua_S,NULL,"CLASS_ARMS",CLASS_ARMS);
 tolua_constant(tolua_S,NULL,"CLASS_WALL",CLASS_WALL);
 tolua_cclass(tolua_S,"tactic_info_type","");
 tolua_tablevar(tolua_S,"tactic_info_type","to_hit",toluaI_get_player_tactic_info_type_to_hit,toluaI_set_player_tactic_info_type_to_hit);
 tolua_tablevar(tolua_S,"tactic_info_type","to_dam",toluaI_get_player_tactic_info_type_to_dam,toluaI_set_player_tactic_info_type_to_dam);
 tolua_tablevar(tolua_S,"tactic_info_type","to_ac",toluaI_get_player_tactic_info_type_to_ac,toluaI_set_player_tactic_info_type_to_ac);
 tolua_tablevar(tolua_S,"tactic_info_type","to_stealth",toluaI_get_player_tactic_info_type_to_stealth,toluaI_set_player_tactic_info_type_to_stealth);
 tolua_tablevar(tolua_S,"tactic_info_type","to_disarm",toluaI_get_player_tactic_info_type_to_disarm,toluaI_set_player_tactic_info_type_to_disarm);
 tolua_tablevar(tolua_S,"tactic_info_type","to_saving",toluaI_get_player_tactic_info_type_to_saving,toluaI_set_player_tactic_info_type_to_saving);
 tolua_tablevar(tolua_S,"tactic_info_type","name",toluaI_get_player_tactic_info_type_name,toluaI_set_player_tactic_info_type_name);
 tolua_cclass(tolua_S,"move_info_type","");
 tolua_tablevar(tolua_S,"move_info_type","to_speed",toluaI_get_player_move_info_type_to_speed,toluaI_set_player_move_info_type_to_speed);
 tolua_tablevar(tolua_S,"move_info_type","to_search",toluaI_get_player_move_info_type_to_search,toluaI_set_player_move_info_type_to_search);
 tolua_tablevar(tolua_S,"move_info_type","to_stealth",toluaI_get_player_move_info_type_to_stealth,toluaI_set_player_move_info_type_to_stealth);
 tolua_tablevar(tolua_S,"move_info_type","to_percep",toluaI_get_player_move_info_type_to_percep,toluaI_set_player_move_info_type_to_percep);
 tolua_tablevar(tolua_S,"move_info_type","name",toluaI_get_player_move_info_type_name,toluaI_set_player_move_info_type_name);
 tolua_globalarray(tolua_S,"tactic_info",toluaI_get_player_tactic_info,toluaI_set_player_tactic_info);
 tolua_globalarray(tolua_S,"move_info",toluaI_get_player_move_info,toluaI_set_player_move_info);
 tolua_cclass(tolua_S,"player_type","");
 tolua_tablevar(tolua_S,"player_type","lives",toluaI_get_player_player_type_lives,toluaI_set_player_player_type_lives);
 tolua_tablevar(tolua_S,"player_type","oldpy",toluaI_get_player_player_type_oldpy,toluaI_set_player_player_type_oldpy);
 tolua_tablevar(tolua_S,"player_type","oldpx",toluaI_get_player_player_type_oldpx,toluaI_set_player_player_type_oldpx);
 tolua_tablevar(tolua_S,"player_type","wild_force_side_y_ok",toluaI_get_player_player_type_wild_force_side_y_ok,toluaI_set_player_player_type_wild_force_side_y_ok);
 tolua_tablevar(tolua_S,"player_type","wild_force_side_x_ok",toluaI_get_player_player_type_wild_force_side_x_ok,toluaI_set_player_player_type_wild_force_side_x_ok);
 tolua_tablevar(tolua_S,"player_type","wild_force_side_y",toluaI_get_player_player_type_wild_force_side_y,toluaI_set_player_player_type_wild_force_side_y);
 tolua_tablevar(tolua_S,"player_type","wild_force_side_x",toluaI_get_player_player_type_wild_force_side_x,toluaI_set_player_player_type_wild_force_side_x);
 tolua_tablevar(tolua_S,"player_type","py",toluaI_get_player_player_type_py,toluaI_set_player_player_type_py);
 tolua_tablevar(tolua_S,"player_type","px",toluaI_get_player_player_type_px,toluaI_set_player_player_type_px);
 tolua_tablevar(tolua_S,"player_type","descriptor",toluaI_get_player_player_type_descriptor,toluaI_set_player_player_type_descriptor);
 tolua_tablevar(tolua_S,"player_type","descriptors",toluaI_get_player_player_type_descriptors,toluaI_set_player_player_type_descriptors);
 tolua_tablevar(tolua_S,"player_type","mimic_form",toluaI_get_player_player_type_mimic_form,toluaI_set_player_player_type_mimic_form);
 tolua_tablevar(tolua_S,"player_type","mimic_level",toluaI_get_player_player_type_mimic_level,toluaI_set_player_player_type_mimic_level);
 tolua_tablevar(tolua_S,"player_type","oops",toluaI_get_player_player_type_oops,toluaI_set_player_player_type_oops);
 tolua_tablevar(tolua_S,"player_type","inventory",toluaI_get_player_player_type_inventory,toluaI_set_player_player_type_inventory);
 tolua_tablevar(tolua_S,"player_type","hitdie",toluaI_get_player_player_type_hitdie,toluaI_set_player_player_type_hitdie);
 tolua_tablevar(tolua_S,"player_type","expfact",toluaI_get_player_player_type_expfact,toluaI_set_player_player_type_expfact);
 tolua_tablevar(tolua_S,"player_type","maximize",toluaI_get_player_player_type_maximize,toluaI_set_player_player_type_maximize);
 tolua_tablevar(tolua_S,"player_type","preserve",toluaI_get_player_player_type_preserve,toluaI_set_player_player_type_preserve);
 tolua_tablevar(tolua_S,"player_type","special",toluaI_get_player_player_type_special,toluaI_set_player_player_type_special);
 tolua_tablevar(tolua_S,"player_type","allow_one_death",toluaI_get_player_player_type_allow_one_death,toluaI_set_player_player_type_allow_one_death);
 tolua_tablevar(tolua_S,"player_type","age",toluaI_get_player_player_type_age,toluaI_set_player_player_type_age);
 tolua_tablevar(tolua_S,"player_type","ht",toluaI_get_player_player_type_ht,toluaI_set_player_player_type_ht);
 tolua_tablevar(tolua_S,"player_type","wt",toluaI_get_player_player_type_wt,toluaI_set_player_player_type_wt);
 tolua_tablevar(tolua_S,"player_type","sc",toluaI_get_player_player_type_sc,toluaI_set_player_player_type_sc);
 tolua_tablevar(tolua_S,"player_type","au",toluaI_get_player_player_type_au,toluaI_set_player_player_type_au);
 tolua_tablevar(tolua_S,"player_type","max_exp",toluaI_get_player_player_type_max_exp,toluaI_set_player_player_type_max_exp);
 tolua_tablevar(tolua_S,"player_type","exp",toluaI_get_player_player_type_exp,toluaI_set_player_player_type_exp);
 tolua_tablevar(tolua_S,"player_type","exp_frac",toluaI_get_player_player_type_exp_frac,toluaI_set_player_player_type_exp_frac);
 tolua_tablevar(tolua_S,"player_type","lev",toluaI_get_player_player_type_lev,toluaI_set_player_player_type_lev);
 tolua_tablevar(tolua_S,"player_type","town_num",toluaI_get_player_player_type_town_num,toluaI_set_player_player_type_town_num);
 tolua_tablevar(tolua_S,"player_type","inside_quest",toluaI_get_player_player_type_inside_quest,toluaI_set_player_player_type_inside_quest);
 tolua_tablevar(tolua_S,"player_type","exit_bldg",toluaI_get_player_player_type_exit_bldg,toluaI_set_player_player_type_exit_bldg);
 tolua_tablevar(tolua_S,"player_type","wilderness_x",toluaI_get_player_player_type_wilderness_x,toluaI_set_player_player_type_wilderness_x);
 tolua_tablevar(tolua_S,"player_type","wilderness_y",toluaI_get_player_player_type_wilderness_y,toluaI_set_player_player_type_wilderness_y);
 tolua_tablevar(tolua_S,"player_type","wild_mode",toluaI_get_player_player_type_wild_mode,toluaI_set_player_player_type_wild_mode);
 tolua_tablevar(tolua_S,"player_type","old_wild_mode",toluaI_get_player_player_type_old_wild_mode,toluaI_set_player_player_type_old_wild_mode);
 tolua_tablevar(tolua_S,"player_type","hp_mod",toluaI_get_player_player_type_hp_mod,toluaI_set_player_player_type_hp_mod);
 tolua_tablevar(tolua_S,"player_type","max_plv",toluaI_get_player_player_type_max_plv,toluaI_set_player_player_type_max_plv);
 tolua_tablevar(tolua_S,"player_type","luck_cur",toluaI_get_player_player_type_luck_cur,toluaI_set_player_player_type_luck_cur);
 tolua_tablevar(tolua_S,"player_type","luck_max",toluaI_get_player_player_type_luck_max,toluaI_set_player_player_type_luck_max);
 tolua_tablevar(tolua_S,"player_type","luck_base",toluaI_get_player_player_type_luck_base,toluaI_set_player_player_type_luck_base);
 tolua_tablevar(tolua_S,"player_type","cur_lite",toluaI_get_player_player_type_cur_lite,toluaI_set_player_player_type_cur_lite);
 tolua_tablevar(tolua_S,"player_type","pspeed",toluaI_get_player_player_type_pspeed,toluaI_set_player_player_type_pspeed);
 tolua_tablevar(tolua_S,"player_type","speeds",toluaI_get_player_player_type_speeds,toluaI_set_player_player_type_speeds);
 tolua_tablevar(tolua_S,"player_type","notice",toluaI_get_player_player_type_notice,toluaI_set_player_player_type_notice);
 tolua_tablevar(tolua_S,"player_type","update",toluaI_get_player_player_type_update,toluaI_set_player_player_type_update);
 tolua_tablevar(tolua_S,"player_type","redraw",toluaI_get_player_player_type_redraw,toluaI_set_player_player_type_redraw);
 tolua_tablevar(tolua_S,"player_type","window",toluaI_get_player_player_type_window,toluaI_set_player_player_type_window);
 tolua_tablevar(tolua_S,"player_type","recall_dungeon",toluaI_get_player_player_type_recall_dungeon,toluaI_set_player_player_type_recall_dungeon);
 tolua_tablevar(tolua_S,"player_type","word_recall",toluaI_get_player_player_type_word_recall,toluaI_set_player_player_type_word_recall);
 tolua_tablevar(tolua_S,"player_type","energy",toluaI_get_player_player_type_energy,toluaI_set_player_player_type_energy);
 tolua_tablevar(tolua_S,"player_type","stat_max",toluaI_get_player_player_type_stat_max,toluaI_set_player_player_type_stat_max);
 tolua_tablevar(tolua_S,"player_type","stat_cur",toluaI_get_player_player_type_stat_cur,toluaI_set_player_player_type_stat_cur);
 tolua_tablevar(tolua_S,"player_type","stat_add",toluaI_get_player_player_type_stat_add,toluaI_set_player_player_type_stat_add);
 tolua_tablevar(tolua_S,"player_type","stat_ind",toluaI_get_player_player_type_stat_ind,toluaI_set_player_player_type_stat_ind);
 tolua_tablevar(tolua_S,"player_type","stat_top",toluaI_get_player_player_type_stat_top,toluaI_set_player_player_type_stat_top);
 tolua_tablevar(tolua_S,"player_type","stat_cnt",toluaI_get_player_player_type_stat_cnt,toluaI_set_player_player_type_stat_cnt);
 tolua_tablevar(tolua_S,"player_type","stat_los",toluaI_get_player_player_type_stat_los,toluaI_set_player_player_type_stat_los);
 tolua_tablevar(tolua_S,"player_type","intrinsic_flags",toluaI_get_player_player_type_intrinsic_flags,toluaI_set_player_player_type_intrinsic_flags);
 tolua_tablevar(tolua_S,"player_type","resists",toluaI_get_player_player_type_resists,toluaI_set_player_player_type_resists);
 tolua_tablevar(tolua_S,"player_type","absorbs",toluaI_get_player_player_type_absorbs,toluaI_set_player_player_type_absorbs);
 tolua_tablevar(tolua_S,"player_type","skill_bonuses",toluaI_get_player_player_type_skill_bonuses,toluaI_set_player_player_type_skill_bonuses);
 tolua_tablevar(tolua_S,"player_type","cumber_armor",toluaI_get_player_player_type_cumber_armor,toluaI_set_player_player_type_cumber_armor);
 tolua_tablevar(tolua_S,"player_type","cumber_glove",toluaI_get_player_player_type_cumber_glove,toluaI_set_player_player_type_cumber_glove);
 tolua_tablevar(tolua_S,"player_type","heavy_wield",toluaI_get_player_player_type_heavy_wield,toluaI_set_player_player_type_heavy_wield);
 tolua_tablevar(tolua_S,"player_type","heavy_shoot",toluaI_get_player_player_type_heavy_shoot,toluaI_set_player_player_type_heavy_shoot);
 tolua_tablevar(tolua_S,"player_type","icky_wield",toluaI_get_player_player_type_icky_wield,toluaI_set_player_player_type_icky_wield);
 tolua_tablevar(tolua_S,"player_type","dis_to_h",toluaI_get_player_player_type_dis_to_h,toluaI_set_player_player_type_dis_to_h);
 tolua_tablevar(tolua_S,"player_type","dis_to_d",toluaI_get_player_player_type_dis_to_d,toluaI_set_player_player_type_dis_to_d);
 tolua_tablevar(tolua_S,"player_type","dis_to_a",toluaI_get_player_player_type_dis_to_a,toluaI_set_player_player_type_dis_to_a);
 tolua_tablevar(tolua_S,"player_type","dis_ac",toluaI_get_player_player_type_dis_ac,toluaI_set_player_player_type_dis_ac);
 tolua_tablevar(tolua_S,"player_type","to_l",toluaI_get_player_player_type_to_l,toluaI_set_player_player_type_to_l);
 tolua_tablevar(tolua_S,"player_type","to_m",toluaI_get_player_player_type_to_m,toluaI_set_player_player_type_to_m);
 tolua_tablevar(tolua_S,"player_type","to_s",toluaI_get_player_player_type_to_s,toluaI_set_player_player_type_to_s);
 tolua_tablevar(tolua_S,"player_type","to_h",toluaI_get_player_player_type_to_h,toluaI_set_player_player_type_to_h);
 tolua_tablevar(tolua_S,"player_type","to_d",toluaI_get_player_player_type_to_d,toluaI_set_player_player_type_to_d);
 tolua_tablevar(tolua_S,"player_type","to_a",toluaI_get_player_player_type_to_a,toluaI_set_player_player_type_to_a);
 tolua_tablevar(tolua_S,"player_type","num_blow",toluaI_get_player_player_type_num_blow,toluaI_set_player_player_type_num_blow);
 tolua_tablevar(tolua_S,"player_type","num_fire",toluaI_get_player_player_type_num_fire,toluaI_set_player_player_type_num_fire);
 tolua_tablevar(tolua_S,"player_type","extra_blows",toluaI_get_player_player_type_extra_blows,toluaI_set_player_player_type_extra_blows);
 tolua_tablevar(tolua_S,"player_type","extra_shots",toluaI_get_player_player_type_extra_shots,toluaI_set_player_player_type_extra_shots);
 tolua_tablevar(tolua_S,"player_type","ac",toluaI_get_player_player_type_ac,toluaI_set_player_player_type_ac);
 tolua_tablevar(tolua_S,"player_type","antimagic",toluaI_get_player_player_type_antimagic,toluaI_set_player_player_type_antimagic);
 tolua_tablevar(tolua_S,"player_type","antimagic_dis",toluaI_get_player_player_type_antimagic_dis,toluaI_set_player_player_type_antimagic_dis);
 tolua_tablevar(tolua_S,"player_type","see_infra",toluaI_get_player_player_type_see_infra,toluaI_set_player_player_type_see_infra);
 tolua_tablevar(tolua_S,"player_type","skill_dis",toluaI_get_player_player_type_skill_dis,toluaI_set_player_player_type_skill_dis);
 tolua_tablevar(tolua_S,"player_type","skill_dev",toluaI_get_player_player_type_skill_dev,toluaI_set_player_player_type_skill_dev);
 tolua_tablevar(tolua_S,"player_type","skill_sav",toluaI_get_player_player_type_skill_sav,toluaI_set_player_player_type_skill_sav);
 tolua_tablevar(tolua_S,"player_type","skill_stl",toluaI_get_player_player_type_skill_stl,toluaI_set_player_player_type_skill_stl);
 tolua_tablevar(tolua_S,"player_type","skill_thn",toluaI_get_player_player_type_skill_thn,toluaI_set_player_player_type_skill_thn);
 tolua_tablevar(tolua_S,"player_type","skill_thb",toluaI_get_player_player_type_skill_thb,toluaI_set_player_player_type_skill_thb);
 tolua_tablevar(tolua_S,"player_type","skill_tht",toluaI_get_player_player_type_skill_tht,toluaI_set_player_player_type_skill_tht);
 tolua_tablevar(tolua_S,"player_type","skill_points",toluaI_get_player_player_type_skill_points,toluaI_set_player_player_type_skill_points);
 tolua_tablevar(tolua_S,"player_type","combat_style",toluaI_get_player_player_type_combat_style,toluaI_set_player_player_type_combat_style);
 tolua_tablevar(tolua_S,"player_type","use_piercing_shots",toluaI_get_player_player_type_use_piercing_shots,toluaI_set_player_player_type_use_piercing_shots);
 tolua_tablevar(tolua_S,"player_type","control",toluaI_get_player_player_type_control,toluaI_set_player_player_type_control);
 tolua_tablevar(tolua_S,"player_type","control_dir",toluaI_get_player_player_type_control_dir,toluaI_set_player_player_type_control_dir);
 tolua_tablevar(tolua_S,"player_type","companion_killed",toluaI_get_player_player_type_companion_killed,toluaI_set_player_player_type_companion_killed);
 tolua_tablevar(tolua_S,"player_type","black_breath",toluaI_get_player_player_type_black_breath,toluaI_set_player_player_type_black_breath);
 tolua_tablevar(tolua_S,"player_type","body_monster",toluaI_get_player_player_type_body_monster,toluaI_set_player_player_type_body_monster);
 tolua_tablevar(tolua_S,"player_type","extra_body_parts",toluaI_get_player_player_type_extra_body_parts,toluaI_set_player_player_type_extra_body_parts);
 tolua_tablevar(tolua_S,"player_type","powers",toluaI_get_player_player_type_powers,toluaI_set_player_player_type_powers);
 tolua_tablevar(tolua_S,"player_type","powers_mod",toluaI_get_player_player_type_powers_mod,toluaI_set_player_player_type_powers_mod);
 tolua_tablevar(tolua_S,"player_type","necro_extra",toluaI_get_player_player_type_necro_extra,toluaI_set_player_player_type_necro_extra);
 tolua_tablevar(tolua_S,"player_type","flags",toluaI_get_player_player_type_flags,toluaI_set_player_player_type_flags);
 tolua_tablevar(tolua_S,"player_type","tactic",toluaI_get_player_player_type_tactic,toluaI_set_player_player_type_tactic);
 tolua_tablevar(tolua_S,"player_type","movement",toluaI_get_player_player_type_movement,toluaI_set_player_player_type_movement);
 tolua_tablevar(tolua_S,"player_type","leaving",toluaI_get_player_player_type_leaving,toluaI_set_player_player_type_leaving);
 tolua_globalvar(tolua_S,"energy_use",toluaI_get_player_energy_use,toluaI_set_player_energy_use);
 tolua_globalvar(tolua_S,"alive",toluaI_get_player_alive,toluaI_set_player_alive);
 tolua_globalvar(tolua_S,"death",toluaI_get_player_death,toluaI_set_player_death);
 tolua_function(tolua_S,NULL,"set_died_from",toluaI_player_set_died_from00);
 tolua_globalvar(tolua_S,"player",toluaI_get_player_player,toluaI_set_player_player);
 tolua_globalvar(tolua_S,"max_dp_idx",toluaI_get_player_max_dp_idx,toluaI_set_player_max_dp_idx);
 tolua_globalarray(tolua_S,"descriptor_info",toluaI_get_player_descriptor_info,toluaI_set_player_descriptor_info);
 tolua_globalvar(tolua_S,"flush_failure",toluaI_get_player_flush_failure,toluaI_set_player_flush_failure);
 tolua_function(tolua_S,NULL,"check_experience",toluaI_player_check_experience00);
 tolua_function(tolua_S,NULL,"check_experience_obj",toluaI_player_check_experience_obj00);
 tolua_function(tolua_S,NULL,"gain_exp",toluaI_player_gain_exp00);
 tolua_function(tolua_S,NULL,"lose_exp",toluaI_player_lose_exp00);
 tolua_function(tolua_S,NULL,"no_lite",toluaI_player_no_lite00);
 tolua_function(tolua_S,NULL,"do_cmd_debug",toluaI_player_do_cmd_debug00);
 tolua_globalvar(tolua_S,"dun_level",toluaI_get_player_dun_level,toluaI_set_player_dun_level);
 tolua_globalvar(tolua_S,"old_dun_level",toluaI_get_player_old_dun_level,toluaI_set_player_old_dun_level);
 tolua_function(tolua_S,NULL,"change_wild_mode",toluaI_player_change_wild_mode00);
 tolua_constant(tolua_S,NULL,"WINNER_NORMAL",WINNER_NORMAL);
 tolua_constant(tolua_S,NULL,"WINNER_ULTRA",WINNER_ULTRA);
 tolua_globalvar(tolua_S,"wizard",toluaI_get_player_wizard,toluaI_set_player_wizard);
 tolua_globalvar(tolua_S,"noscore",toluaI_get_player_noscore,toluaI_set_player_noscore);
 tolua_globalvar(tolua_S,"winner_state",toluaI_get_player_winner_state,toluaI_set_player_winner_state);
 tolua_globalvar(tolua_S,"has_won",toluaI_get_player_has_won,toluaI_set_player_has_won);
 tolua_globalvar(tolua_S,"joke_monsters",toluaI_get_player_joke_monsters,toluaI_set_player_joke_monsters);
 tolua_globalvar(tolua_S,"exp_need",toluaI_get_player_exp_need,toluaI_set_player_exp_need);
 tolua_globalvar(tolua_S,"linear_stats",toluaI_get_player_linear_stats,toluaI_set_player_linear_stats);
 tolua_globalvar(tolua_S,"player_char_health",toluaI_get_player_player_char_health,toluaI_set_player_player_char_health);
 tolua_globalvar(tolua_S,"hitpoint_warn",toluaI_get_player_hitpoint_warn,toluaI_set_player_hitpoint_warn);
 tolua_function(tolua_S,NULL,"cnv_stat",toluaI_player_cnv_stat00);
 tolua_globalvar(tolua_S,"command_cmd",toluaI_get_player_command_cmd,toluaI_set_player_command_cmd);
 tolua_globalvar(tolua_S,"command_arg",toluaI_get_player_command_arg,toluaI_set_player_command_arg);
 tolua_globalvar(tolua_S,"command_rep",toluaI_get_player_command_rep,toluaI_set_player_command_rep);
 tolua_globalvar(tolua_S,"command_dir",toluaI_get_player_command_dir,toluaI_set_player_command_dir);
 tolua_globalvar(tolua_S,"command_see",toluaI_get_player_command_see,toluaI_set_player_command_see);
 tolua_globalvar(tolua_S,"command_wrk",toluaI_get_player_command_wrk,toluaI_set_player_command_wrk);
 tolua_globalvar(tolua_S,"command_new",toluaI_get_player_command_new,toluaI_set_player_command_new);
 tolua_globalvar(tolua_S,"command_new_do_keymap",toluaI_get_player_command_new_do_keymap,toluaI_set_player_command_new_do_keymap);
 tolua_globalvar(tolua_S,"leaving_quest",toluaI_get_player_leaving_quest,toluaI_set_player_leaving_quest);
 tolua_function(tolua_S,NULL,"get_player_energy",toluaI_player_get_player_energy00);
 tolua_function(tolua_S,NULL,"get_speed_name",toluaI_player_get_speed_name00);
 tolua_function(tolua_S,NULL,"get_speed_desc",toluaI_player_get_speed_desc00);
 tolua_function(tolua_S,NULL,"register_speed",toluaI_player_register_speed00);
 tolua_function(tolua_S,NULL,"notice_stuff",toluaI_player_notice_stuff00);
 tolua_function(tolua_S,NULL,"update_stuff",toluaI_player_update_stuff00);
 tolua_function(tolua_S,NULL,"redraw_stuff",toluaI_player_redraw_stuff00);
 tolua_function(tolua_S,NULL,"window_stuff",toluaI_player_window_stuff00);
 tolua_function(tolua_S,NULL,"handle_stuff",toluaI_player_handle_stuff00);
 tolua_globalarray(tolua_S,"history",toluaI_get_player_history,NULL);
 tolua_globalvar(tolua_S,"character_generated",toluaI_get_player_character_generated,toluaI_set_player_character_generated);
 tolua_globalvar(tolua_S,"character_dungeon",toluaI_get_player_character_dungeon,toluaI_set_player_character_dungeon);
 tolua_globalvar(tolua_S,"character_loaded",toluaI_get_player_character_loaded,toluaI_set_player_character_loaded);
 tolua_globalvar(tolua_S,"character_saved",toluaI_get_player_character_saved,toluaI_set_player_character_saved);
 tolua_globalvar(tolua_S,"character_xtra",toluaI_get_player_character_xtra,toluaI_set_player_character_xtra);
 tolua_function(tolua_S,NULL,"browse_school_spell",toluaI_player_browse_school_spell00);
 tolua_function(tolua_S,NULL,"do_cmd_open_obj",toluaI_player_do_cmd_open_obj00);
 tolua_function(tolua_S,NULL,"do_cmd_open_aux",toluaI_player_do_cmd_open_aux00);
 tolua_module(tolua_S,"birth");
 tolua_tablevar(tolua_S,"birth","no_begin_screen",toluaI_get_player_birth_no_begin_screen,toluaI_set_player_birth_no_begin_screen);
 tolua_tablevar(tolua_S,"birth","force_new_game",toluaI_get_player_birth_force_new_game,toluaI_set_player_birth_force_new_game);
 tolua_function(tolua_S,NULL,"do_cmd_skill",toluaI_player_do_cmd_skill00);
 tolua_function(tolua_S,NULL,"player_birth_skill",toluaI_player_player_birth_skill00);
 tolua_function(tolua_S,NULL,"change_player_body",toluaI_player_change_player_body00);
 tolua_function(tolua_S,NULL,"do_cmd_suicide",toluaI_player_do_cmd_suicide00);
 return 1;
}
/* Close function */
void tolua_player_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SUBRACE_PLACE_AFTER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SUBRACE_PLACE_BEFORE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SUBRACE_PLACE_REPLACE");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"PY_MAX_EXP"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"PY_MAX_GOLD"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"PY_MAX_LEVEL"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"player_exp");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"player_hp");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"stats");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"skills");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SUBRACE_SAVE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SEX_FEMALE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SEX_MALE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SEX_NEUTER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PN_COMBINE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PN_REORDER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_BONUS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_TORCH");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_BODY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_SANITY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_HP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_MANA");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_SPELLS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_POWERS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_UN_VIEW");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_VIEW");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_MON_LITE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_MONSTERS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_DISTANCE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PU_FLOW");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CLASS_MANA_PATH_ERASE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CLASS_FLOOD_LEVEL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CLASS_CANALIZE_MANA_EXTRA");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CLASS_UNDEAD");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CLASS_ANTIMAGIC");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CLASS_LEGS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CLASS_ARMS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"CLASS_WALL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"tactic_info_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"move_info_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"tactic_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"move_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"player_type");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"energy_use"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"alive"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"death"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"set_died_from");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"player"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_dp_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"descriptor_info");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"flush_failure"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"check_experience");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"check_experience_obj");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"gain_exp");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lose_exp");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"no_lite");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_cmd_debug");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"dun_level"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"old_dun_level"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"change_wild_mode");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"WINNER_NORMAL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"WINNER_ULTRA");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"wizard"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"noscore"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"winner_state"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"has_won"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"joke_monsters"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"exp_need"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"linear_stats"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"player_char_health"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"hitpoint_warn"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cnv_stat");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"command_cmd"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"command_arg"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"command_rep"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"command_dir"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"command_see"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"command_wrk"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"command_new"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"command_new_do_keymap"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"leaving_quest"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_player_energy");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_speed_name");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_speed_desc");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"register_speed");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"notice_stuff");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"update_stuff");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"redraw_stuff");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"window_stuff");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"handle_stuff");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"history");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"character_generated"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"character_dungeon"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"character_loaded"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"character_saved"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"character_xtra"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"browse_school_spell");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_cmd_open_obj");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_cmd_open_aux");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"birth");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_cmd_skill");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"player_birth_skill");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"change_player_body");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_cmd_suicide");
}
