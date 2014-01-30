/*
** Lua binding: player_c
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:45 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_player_c_open (lua_State* tolua_S);
void tolua_player_c_close (lua_State* tolua_S);

#include "angband.h"

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"player_descriptor");
 tolua_usertype(tolua_S,"flags_type");
 tolua_usertype(tolua_S,"skill_type");
 tolua_usertype(tolua_S,"ability_type");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* get function: name of class  skill_type */
static int toluaI_get_player_c_skill_type_name(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  skill_type */
static int toluaI_set_player_c_skill_type_name(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: desc of class  skill_type */
static int toluaI_get_player_c_skill_type_desc(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->desc);
 return 1;
}

/* set function: desc of class  skill_type */
static int toluaI_set_player_c_skill_type_desc(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->desc = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: action_desc of class  skill_type */
static int toluaI_get_player_c_skill_type_action_desc(lua_State* tolua_S)
{
 int toluaI_index;
  skill_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (skill_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_SKILL_ACTION)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushstring(tolua_S,(const char*)self->action_desc[toluaI_index]);
 return 1;
}

/* set function: action_desc of class  skill_type */
static int toluaI_set_player_c_skill_type_action_desc(lua_State* tolua_S)
{
 int toluaI_index;
  skill_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (skill_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_SKILL_ACTION)
 tolua_error(tolua_S,"array indexing out of range.");
  self->action_desc[toluaI_index] = ((cptr)  tolua_getstring(tolua_S,3,0));
 return 0;
}

/* get function: action_mkey of class  skill_type */
static int toluaI_get_player_c_skill_type_action_mkey(lua_State* tolua_S)
{
 int toluaI_index;
  skill_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (skill_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_SKILL_ACTION)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->action_mkey[toluaI_index]);
 return 1;
}

/* set function: action_mkey of class  skill_type */
static int toluaI_set_player_c_skill_type_action_mkey(lua_State* tolua_S)
{
 int toluaI_index;
  skill_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (skill_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_SKILL_ACTION)
 tolua_error(tolua_S,"array indexing out of range.");
  self->action_mkey[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: i_value of class  skill_type */
static int toluaI_get_player_c_skill_type_i_value(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->i_value);
 return 1;
}

/* set function: i_value of class  skill_type */
static int toluaI_set_player_c_skill_type_i_value(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->i_value = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: i_mod of class  skill_type */
static int toluaI_get_player_c_skill_type_i_mod(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->i_mod);
 return 1;
}

/* set function: i_mod of class  skill_type */
static int toluaI_set_player_c_skill_type_i_mod(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->i_mod = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: value of class  skill_type */
static int toluaI_get_player_c_skill_type_value(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->value);
 return 1;
}

/* set function: value of class  skill_type */
static int toluaI_set_player_c_skill_type_value(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->value = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mod of class  skill_type */
static int toluaI_get_player_c_skill_type_mod(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mod);
 return 1;
}

/* set function: mod of class  skill_type */
static int toluaI_set_player_c_skill_type_mod(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mod = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: rate of class  skill_type */
static int toluaI_get_player_c_skill_type_rate(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->rate);
 return 1;
}

/* set function: rate of class  skill_type */
static int toluaI_set_player_c_skill_type_rate(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->rate = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: uses of class  skill_type */
static int toluaI_get_player_c_skill_type_uses(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->uses);
 return 1;
}

/* set function: uses of class  skill_type */
static int toluaI_set_player_c_skill_type_uses(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->uses = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: action of class  skill_type */
static int toluaI_get_player_c_skill_type_action(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->action,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: action of class  skill_type */
static int toluaI_set_player_c_skill_type_action(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->action = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: father of class  skill_type */
static int toluaI_get_player_c_skill_type_father(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->father);
 return 1;
}

/* set function: father of class  skill_type */
static int toluaI_set_player_c_skill_type_father(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->father = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dev of class  skill_type */
static int toluaI_get_player_c_skill_type_dev(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->dev);
 return 1;
}

/* set function: dev of class  skill_type */
static int toluaI_set_player_c_skill_type_dev(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->dev = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: order of class  skill_type */
static int toluaI_get_player_c_skill_type_order(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->order);
 return 1;
}

/* set function: order of class  skill_type */
static int toluaI_set_player_c_skill_type_order(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->order = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: hidden of class  skill_type */
static int toluaI_get_player_c_skill_type_hidden(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->hidden);
 return 1;
}

/* set function: hidden of class  skill_type */
static int toluaI_set_player_c_skill_type_hidden(lua_State* tolua_S)
{
  skill_type* self = (skill_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->hidden = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: old_max_s_idx */
static int toluaI_get_player_c_old_max_s_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)old_max_s_idx);
 return 1;
}

/* set function: old_max_s_idx */
static int toluaI_set_player_c_old_max_s_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  old_max_s_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_s_idx */
static int toluaI_get_player_c_max_s_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_s_idx);
 return 1;
}

/* set function: max_s_idx */
static int toluaI_set_player_c_max_s_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_s_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: s_info */
static int toluaI_get_player_c_s_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_s_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&s_info[toluaI_index],tolua_tag(tolua_S,"skill_type"));
 return 1;
}

/* set function: s_info */
static int toluaI_set_player_c_s_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_s_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  s_info[toluaI_index] = *((skill_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* function: get_skill_raw */
static int toluaI_player_c_get_skill_raw00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b skill = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  get_skill_raw(skill);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_skill_raw'.");
 return 0;
}

/* function: get_skill */
static int toluaI_player_c_get_skill00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b skill = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  get_skill(skill);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_skill'.");
 return 0;
}

/* function: get_skill_scale */
static int toluaI_player_c_get_skill_scale00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b skill = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b scale = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  get_skill_scale(skill,scale);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_skill_scale'.");
 return 0;
}

/* function: do_get_new_skill */
static int toluaI_player_c_do_get_new_skill00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  do_get_new_skill();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_get_new_skill'.");
 return 0;
}

/* function: find_skill */
static int toluaI_player_c_find_skill00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr name = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  s16b toluaI_ret = (s16b)  find_skill(name);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_skill'.");
 return 0;
}

/* function: find_skill_i */
static int toluaI_player_c_find_skill_i00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr name = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  s16b toluaI_ret = (s16b)  find_skill_i(name);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_skill_i'.");
 return 0;
}

/* get function: name of class  ability_type */
static int toluaI_get_player_c_ability_type_name(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  ability_type */
static int toluaI_set_player_c_ability_type_name(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: desc of class  ability_type */
static int toluaI_get_player_c_ability_type_desc(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->desc);
 return 1;
}

/* set function: desc of class  ability_type */
static int toluaI_set_player_c_ability_type_desc(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->desc = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: action_mkey of class  ability_type */
static int toluaI_get_player_c_ability_type_action_mkey(lua_State* tolua_S)
{
 int toluaI_index;
  ability_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ability_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_SKILL_ACTION)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->action_mkey[toluaI_index]);
 return 1;
}

/* set function: action_mkey of class  ability_type */
static int toluaI_set_player_c_ability_type_action_mkey(lua_State* tolua_S)
{
 int toluaI_index;
  ability_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ability_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_SKILL_ACTION)
 tolua_error(tolua_S,"array indexing out of range.");
  self->action_mkey[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: cost of class  ability_type */
static int toluaI_get_player_c_ability_type_cost(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->cost);
 return 1;
}

/* set function: cost of class  ability_type */
static int toluaI_set_player_c_ability_type_cost(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->cost = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: acquired of class  ability_type */
static int toluaI_get_player_c_ability_type_acquired(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->acquired);
 return 1;
}

/* set function: acquired of class  ability_type */
static int toluaI_set_player_c_ability_type_acquired(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->acquired = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: hidden of class  ability_type */
static int toluaI_get_player_c_ability_type_hidden(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->hidden);
 return 1;
}

/* set function: hidden of class  ability_type */
static int toluaI_set_player_c_ability_type_hidden(lua_State* tolua_S)
{
  ability_type* self = (ability_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->hidden = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* function: find_ability */
static int toluaI_player_c_find_ability00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr name = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  s16b toluaI_ret = (s16b)  find_ability(name);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_ability'.");
 return 0;
}

/* function: has_ability */
static int toluaI_player_c_has_ability00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b ab = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  has_ability(ab);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'has_ability'.");
 return 0;
}

/* get function: max_ab_idx */
static int toluaI_get_player_c_max_ab_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_ab_idx);
 return 1;
}

/* set function: max_ab_idx */
static int toluaI_set_player_c_max_ab_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_ab_idx = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: ab_info */
static int toluaI_get_player_c_ab_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_ab_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&ab_info[toluaI_index],tolua_tag(tolua_S,"ability_type"));
 return 1;
}

/* set function: ab_info */
static int toluaI_set_player_c_ab_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_ab_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  ab_info[toluaI_index] = *((ability_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* function: find_player_descriptor */
static int toluaI_player_c_find_player_descriptor00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr name = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  find_player_descriptor(name);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_player_descriptor'.");
 return 0;
}

/* function: find_power */
static int toluaI_player_c_find_power00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr name = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  find_power(name);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_power'.");
 return 0;
}

/* get function: id of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_id(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->id);
 return 1;
}

/* set function: id of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_id(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->id = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: type of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_type(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->type);
 return 1;
}

/* set function: type of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_type(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->type = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: title of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_title(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->title);
 return 1;
}

/* set function: title of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_title(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->title = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: desc of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_desc(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->desc);
 return 1;
}

/* set function: desc of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_desc(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->desc = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: place of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_place(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->place);
 return 1;
}

/* set function: place of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_place(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->place = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: adj of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_adj(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->adj,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: adj of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_adj(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->adj = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: luck of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_luck(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->luck);
 return 1;
}

/* set function: luck of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_luck(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->luck = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mana of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_mana(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mana);
 return 1;
}

/* set function: mana of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_mana(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mana = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: blow_num of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_blow_num(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->blow_num);
 return 1;
}

/* set function: blow_num of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_blow_num(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->blow_num = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: blow_wgt of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_blow_wgt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->blow_wgt);
 return 1;
}

/* set function: blow_wgt of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_blow_wgt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->blow_wgt = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: blow_mul of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_blow_mul(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->blow_mul);
 return 1;
}

/* set function: blow_mul of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_blow_mul(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->blow_mul = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: extra_blows of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_extra_blows(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->extra_blows);
 return 1;
}

/* set function: extra_blows of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_extra_blows(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->extra_blows = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dis of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_dis(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dis);
 return 1;
}

/* set function: dis of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_dis(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dis = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dev of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_dev(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dev);
 return 1;
}

/* set function: dev of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_dev(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dev = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sav of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_sav(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sav);
 return 1;
}

/* set function: sav of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_sav(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sav = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: stl of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_stl(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->stl);
 return 1;
}

/* set function: stl of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_stl(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->stl = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: srh of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_srh(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->srh);
 return 1;
}

/* set function: srh of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_srh(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->srh = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: fos of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_fos(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->fos);
 return 1;
}

/* set function: fos of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_fos(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->fos = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: thn of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_thn(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->thn);
 return 1;
}

/* set function: thn of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_thn(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->thn = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: thb of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_thb(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->thb);
 return 1;
}

/* set function: thb of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_thb(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->thb = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mhp of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_mhp(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mhp);
 return 1;
}

/* set function: mhp of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_mhp(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mhp = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: exp of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_exp(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->exp);
 return 1;
}

/* set function: exp of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_exp(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->exp = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: b_age of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_b_age(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->b_age);
 return 1;
}

/* set function: b_age of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_b_age(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->b_age = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: m_age of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_m_age(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->m_age);
 return 1;
}

/* set function: m_age of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_m_age(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->m_age = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: m_b_ht of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_m_b_ht(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->m_b_ht);
 return 1;
}

/* set function: m_b_ht of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_m_b_ht(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->m_b_ht = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: m_m_ht of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_m_m_ht(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->m_m_ht);
 return 1;
}

/* set function: m_m_ht of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_m_m_ht(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->m_m_ht = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: m_b_wt of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_m_b_wt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->m_b_wt);
 return 1;
}

/* set function: m_b_wt of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_m_b_wt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->m_b_wt = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: m_m_wt of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_m_m_wt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->m_m_wt);
 return 1;
}

/* set function: m_m_wt of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_m_m_wt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->m_m_wt = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: f_b_ht of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_f_b_ht(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->f_b_ht);
 return 1;
}

/* set function: f_b_ht of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_f_b_ht(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->f_b_ht = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: f_m_ht of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_f_m_ht(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->f_m_ht);
 return 1;
}

/* set function: f_m_ht of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_f_m_ht(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->f_m_ht = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: f_b_wt of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_f_b_wt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->f_b_wt);
 return 1;
}

/* set function: f_b_wt of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_f_b_wt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->f_b_wt = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: f_m_wt of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_f_m_wt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->f_m_wt);
 return 1;
}

/* set function: f_m_wt of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_f_m_wt(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->f_m_wt = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: infra of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_infra(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->infra);
 return 1;
}

/* set function: infra of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_infra(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->infra = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: choice of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_choice(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->choice,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: choice of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_choice(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->choice = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: powers of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_powers(lua_State* tolua_S)
{
 int toluaI_index;
  player_descriptor* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (player_descriptor*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->powers[toluaI_index]);
 return 1;
}

/* set function: powers of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_powers(lua_State* tolua_S)
{
 int toluaI_index;
  player_descriptor* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (player_descriptor*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->powers[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: body_parts of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_body_parts(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->body_parts,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: body_parts of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_body_parts(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->body_parts = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: chart of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_chart(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->chart);
 return 1;
}

/* set function: chart of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_chart(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->chart = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_flags(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_flags(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: oflags of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_oflags(lua_State* tolua_S)
{
 int toluaI_index;
  player_descriptor* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (player_descriptor*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=200)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&self->oflags[toluaI_index],tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: oflags of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_oflags(lua_State* tolua_S)
{
 int toluaI_index;
  player_descriptor* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (player_descriptor*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=200)
 tolua_error(tolua_S,"array indexing out of range.");
  self->oflags[toluaI_index] = *((flags_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: skill_base of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_skill_base(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->skill_base,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: skill_base of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_skill_base(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->skill_base = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: skill_mod of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_skill_mod(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->skill_mod,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: skill_mod of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_skill_mod(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->skill_mod = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: objects of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_objects(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->objects,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: objects of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_objects(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->objects = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: abilities of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_abilities(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->abilities,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: abilities of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_abilities(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->abilities = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: load_script of class  player_descriptor */
static int toluaI_get_player_c_player_descriptor_load_script(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->load_script);
 return 1;
}

/* set function: load_script of class  player_descriptor */
static int toluaI_set_player_c_player_descriptor_load_script(lua_State* tolua_S)
{
  player_descriptor* self = (player_descriptor*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->load_script = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* Open function */
int tolua_player_c_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_constant(tolua_S,NULL,"SKILL_EXCLUSIVE",SKILL_EXCLUSIVE);
 tolua_cclass(tolua_S,"skill_type","");
 tolua_tablevar(tolua_S,"skill_type","name",toluaI_get_player_c_skill_type_name,toluaI_set_player_c_skill_type_name);
 tolua_tablevar(tolua_S,"skill_type","desc",toluaI_get_player_c_skill_type_desc,toluaI_set_player_c_skill_type_desc);
 tolua_tablearray(tolua_S,"skill_type","action_desc",toluaI_get_player_c_skill_type_action_desc,toluaI_set_player_c_skill_type_action_desc);
 tolua_tablearray(tolua_S,"skill_type","action_mkey",toluaI_get_player_c_skill_type_action_mkey,toluaI_set_player_c_skill_type_action_mkey);
 tolua_tablevar(tolua_S,"skill_type","i_value",toluaI_get_player_c_skill_type_i_value,toluaI_set_player_c_skill_type_i_value);
 tolua_tablevar(tolua_S,"skill_type","i_mod",toluaI_get_player_c_skill_type_i_mod,toluaI_set_player_c_skill_type_i_mod);
 tolua_tablevar(tolua_S,"skill_type","value",toluaI_get_player_c_skill_type_value,toluaI_set_player_c_skill_type_value);
 tolua_tablevar(tolua_S,"skill_type","mod",toluaI_get_player_c_skill_type_mod,toluaI_set_player_c_skill_type_mod);
 tolua_tablevar(tolua_S,"skill_type","rate",toluaI_get_player_c_skill_type_rate,toluaI_set_player_c_skill_type_rate);
 tolua_tablevar(tolua_S,"skill_type","uses",toluaI_get_player_c_skill_type_uses,toluaI_set_player_c_skill_type_uses);
 tolua_tablevar(tolua_S,"skill_type","action",toluaI_get_player_c_skill_type_action,toluaI_set_player_c_skill_type_action);
 tolua_tablevar(tolua_S,"skill_type","father",toluaI_get_player_c_skill_type_father,toluaI_set_player_c_skill_type_father);
 tolua_tablevar(tolua_S,"skill_type","dev",toluaI_get_player_c_skill_type_dev,toluaI_set_player_c_skill_type_dev);
 tolua_tablevar(tolua_S,"skill_type","order",toluaI_get_player_c_skill_type_order,toluaI_set_player_c_skill_type_order);
 tolua_tablevar(tolua_S,"skill_type","hidden",toluaI_get_player_c_skill_type_hidden,toluaI_set_player_c_skill_type_hidden);
 tolua_globalvar(tolua_S,"old_max_s_idx",toluaI_get_player_c_old_max_s_idx,toluaI_set_player_c_old_max_s_idx);
 tolua_globalvar(tolua_S,"max_s_idx",toluaI_get_player_c_max_s_idx,toluaI_set_player_c_max_s_idx);
 tolua_globalarray(tolua_S,"s_info",toluaI_get_player_c_s_info,toluaI_set_player_c_s_info);
 tolua_function(tolua_S,NULL,"get_skill_raw",toluaI_player_c_get_skill_raw00);
 tolua_function(tolua_S,NULL,"get_skill",toluaI_player_c_get_skill00);
 tolua_function(tolua_S,NULL,"get_skill_scale",toluaI_player_c_get_skill_scale00);
 tolua_function(tolua_S,NULL,"do_get_new_skill",toluaI_player_c_do_get_new_skill00);
 tolua_function(tolua_S,NULL,"find_skill",toluaI_player_c_find_skill00);
 tolua_function(tolua_S,NULL,"find_skill_i",toluaI_player_c_find_skill_i00);
 tolua_constant(tolua_S,NULL,"ACTIVATE_SKILL",ACTIVATE_SKILL);
 tolua_constant(tolua_S,NULL,"BROWSE_SKILL",BROWSE_SKILL);
 tolua_constant(tolua_S,NULL,"MAX_SKILL_ACTION",MAX_SKILL_ACTION);
 tolua_cclass(tolua_S,"ability_type","");
 tolua_tablevar(tolua_S,"ability_type","name",toluaI_get_player_c_ability_type_name,toluaI_set_player_c_ability_type_name);
 tolua_tablevar(tolua_S,"ability_type","desc",toluaI_get_player_c_ability_type_desc,toluaI_set_player_c_ability_type_desc);
 tolua_tablearray(tolua_S,"ability_type","action_mkey",toluaI_get_player_c_ability_type_action_mkey,toluaI_set_player_c_ability_type_action_mkey);
 tolua_tablevar(tolua_S,"ability_type","cost",toluaI_get_player_c_ability_type_cost,toluaI_set_player_c_ability_type_cost);
 tolua_tablevar(tolua_S,"ability_type","acquired",toluaI_get_player_c_ability_type_acquired,toluaI_set_player_c_ability_type_acquired);
 tolua_tablevar(tolua_S,"ability_type","hidden",toluaI_get_player_c_ability_type_hidden,toluaI_set_player_c_ability_type_hidden);
 tolua_function(tolua_S,NULL,"find_ability",toluaI_player_c_find_ability00);
 tolua_function(tolua_S,NULL,"has_ability",toluaI_player_c_has_ability00);
 tolua_globalvar(tolua_S,"max_ab_idx",toluaI_get_player_c_max_ab_idx,toluaI_set_player_c_max_ab_idx);
 tolua_globalarray(tolua_S,"ab_info",toluaI_get_player_c_ab_info,toluaI_set_player_c_ab_info);
 tolua_function(tolua_S,NULL,"find_player_descriptor",toluaI_player_c_find_player_descriptor00);
 tolua_function(tolua_S,NULL,"find_power",toluaI_player_c_find_power00);
 tolua_cclass(tolua_S,"player_descriptor","");
 tolua_tablevar(tolua_S,"player_descriptor","id",toluaI_get_player_c_player_descriptor_id,toluaI_set_player_c_player_descriptor_id);
 tolua_tablevar(tolua_S,"player_descriptor","type",toluaI_get_player_c_player_descriptor_type,toluaI_set_player_c_player_descriptor_type);
 tolua_tablevar(tolua_S,"player_descriptor","title",toluaI_get_player_c_player_descriptor_title,toluaI_set_player_c_player_descriptor_title);
 tolua_tablevar(tolua_S,"player_descriptor","desc",toluaI_get_player_c_player_descriptor_desc,toluaI_set_player_c_player_descriptor_desc);
 tolua_tablevar(tolua_S,"player_descriptor","place",toluaI_get_player_c_player_descriptor_place,toluaI_set_player_c_player_descriptor_place);
 tolua_tablevar(tolua_S,"player_descriptor","adj",toluaI_get_player_c_player_descriptor_adj,toluaI_set_player_c_player_descriptor_adj);
 tolua_tablevar(tolua_S,"player_descriptor","luck",toluaI_get_player_c_player_descriptor_luck,toluaI_set_player_c_player_descriptor_luck);
 tolua_tablevar(tolua_S,"player_descriptor","mana",toluaI_get_player_c_player_descriptor_mana,toluaI_set_player_c_player_descriptor_mana);
 tolua_tablevar(tolua_S,"player_descriptor","blow_num",toluaI_get_player_c_player_descriptor_blow_num,toluaI_set_player_c_player_descriptor_blow_num);
 tolua_tablevar(tolua_S,"player_descriptor","blow_wgt",toluaI_get_player_c_player_descriptor_blow_wgt,toluaI_set_player_c_player_descriptor_blow_wgt);
 tolua_tablevar(tolua_S,"player_descriptor","blow_mul",toluaI_get_player_c_player_descriptor_blow_mul,toluaI_set_player_c_player_descriptor_blow_mul);
 tolua_tablevar(tolua_S,"player_descriptor","extra_blows",toluaI_get_player_c_player_descriptor_extra_blows,toluaI_set_player_c_player_descriptor_extra_blows);
 tolua_tablevar(tolua_S,"player_descriptor","dis",toluaI_get_player_c_player_descriptor_dis,toluaI_set_player_c_player_descriptor_dis);
 tolua_tablevar(tolua_S,"player_descriptor","dev",toluaI_get_player_c_player_descriptor_dev,toluaI_set_player_c_player_descriptor_dev);
 tolua_tablevar(tolua_S,"player_descriptor","sav",toluaI_get_player_c_player_descriptor_sav,toluaI_set_player_c_player_descriptor_sav);
 tolua_tablevar(tolua_S,"player_descriptor","stl",toluaI_get_player_c_player_descriptor_stl,toluaI_set_player_c_player_descriptor_stl);
 tolua_tablevar(tolua_S,"player_descriptor","srh",toluaI_get_player_c_player_descriptor_srh,toluaI_set_player_c_player_descriptor_srh);
 tolua_tablevar(tolua_S,"player_descriptor","fos",toluaI_get_player_c_player_descriptor_fos,toluaI_set_player_c_player_descriptor_fos);
 tolua_tablevar(tolua_S,"player_descriptor","thn",toluaI_get_player_c_player_descriptor_thn,toluaI_set_player_c_player_descriptor_thn);
 tolua_tablevar(tolua_S,"player_descriptor","thb",toluaI_get_player_c_player_descriptor_thb,toluaI_set_player_c_player_descriptor_thb);
 tolua_tablevar(tolua_S,"player_descriptor","mhp",toluaI_get_player_c_player_descriptor_mhp,toluaI_set_player_c_player_descriptor_mhp);
 tolua_tablevar(tolua_S,"player_descriptor","exp",toluaI_get_player_c_player_descriptor_exp,toluaI_set_player_c_player_descriptor_exp);
 tolua_tablevar(tolua_S,"player_descriptor","b_age",toluaI_get_player_c_player_descriptor_b_age,toluaI_set_player_c_player_descriptor_b_age);
 tolua_tablevar(tolua_S,"player_descriptor","m_age",toluaI_get_player_c_player_descriptor_m_age,toluaI_set_player_c_player_descriptor_m_age);
 tolua_tablevar(tolua_S,"player_descriptor","m_b_ht",toluaI_get_player_c_player_descriptor_m_b_ht,toluaI_set_player_c_player_descriptor_m_b_ht);
 tolua_tablevar(tolua_S,"player_descriptor","m_m_ht",toluaI_get_player_c_player_descriptor_m_m_ht,toluaI_set_player_c_player_descriptor_m_m_ht);
 tolua_tablevar(tolua_S,"player_descriptor","m_b_wt",toluaI_get_player_c_player_descriptor_m_b_wt,toluaI_set_player_c_player_descriptor_m_b_wt);
 tolua_tablevar(tolua_S,"player_descriptor","m_m_wt",toluaI_get_player_c_player_descriptor_m_m_wt,toluaI_set_player_c_player_descriptor_m_m_wt);
 tolua_tablevar(tolua_S,"player_descriptor","f_b_ht",toluaI_get_player_c_player_descriptor_f_b_ht,toluaI_set_player_c_player_descriptor_f_b_ht);
 tolua_tablevar(tolua_S,"player_descriptor","f_m_ht",toluaI_get_player_c_player_descriptor_f_m_ht,toluaI_set_player_c_player_descriptor_f_m_ht);
 tolua_tablevar(tolua_S,"player_descriptor","f_b_wt",toluaI_get_player_c_player_descriptor_f_b_wt,toluaI_set_player_c_player_descriptor_f_b_wt);
 tolua_tablevar(tolua_S,"player_descriptor","f_m_wt",toluaI_get_player_c_player_descriptor_f_m_wt,toluaI_set_player_c_player_descriptor_f_m_wt);
 tolua_tablevar(tolua_S,"player_descriptor","infra",toluaI_get_player_c_player_descriptor_infra,toluaI_set_player_c_player_descriptor_infra);
 tolua_tablevar(tolua_S,"player_descriptor","choice",toluaI_get_player_c_player_descriptor_choice,toluaI_set_player_c_player_descriptor_choice);
 tolua_tablearray(tolua_S,"player_descriptor","powers",toluaI_get_player_c_player_descriptor_powers,toluaI_set_player_c_player_descriptor_powers);
 tolua_tablevar(tolua_S,"player_descriptor","body_parts",toluaI_get_player_c_player_descriptor_body_parts,toluaI_set_player_c_player_descriptor_body_parts);
 tolua_tablevar(tolua_S,"player_descriptor","chart",toluaI_get_player_c_player_descriptor_chart,toluaI_set_player_c_player_descriptor_chart);
 tolua_tablevar(tolua_S,"player_descriptor","flags",toluaI_get_player_c_player_descriptor_flags,toluaI_set_player_c_player_descriptor_flags);
 tolua_tablearray(tolua_S,"player_descriptor","oflags",toluaI_get_player_c_player_descriptor_oflags,toluaI_set_player_c_player_descriptor_oflags);
 tolua_tablevar(tolua_S,"player_descriptor","skill_base",toluaI_get_player_c_player_descriptor_skill_base,toluaI_set_player_c_player_descriptor_skill_base);
 tolua_tablevar(tolua_S,"player_descriptor","skill_mod",toluaI_get_player_c_player_descriptor_skill_mod,toluaI_set_player_c_player_descriptor_skill_mod);
 tolua_tablevar(tolua_S,"player_descriptor","objects",toluaI_get_player_c_player_descriptor_objects,toluaI_set_player_c_player_descriptor_objects);
 tolua_tablevar(tolua_S,"player_descriptor","abilities",toluaI_get_player_c_player_descriptor_abilities,toluaI_set_player_c_player_descriptor_abilities);
 tolua_tablevar(tolua_S,"player_descriptor","load_script",toluaI_get_player_c_player_descriptor_load_script,toluaI_set_player_c_player_descriptor_load_script);
 return 1;
}
/* Close function */
void tolua_player_c_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SKILL_EXCLUSIVE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"skill_type");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"old_max_s_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_s_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"s_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_skill_raw");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_skill");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_skill_scale");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_get_new_skill");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_skill");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_skill_i");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ACTIVATE_SKILL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"BROWSE_SKILL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MAX_SKILL_ACTION");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ability_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_ability");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"has_ability");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_ab_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ab_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_player_descriptor");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_power");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"player_descriptor");
}
