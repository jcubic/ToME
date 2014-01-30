/*
** Lua binding: spells
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:50 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_spells_open (lua_State* tolua_S);
void tolua_spells_close (lua_State* tolua_S);

#include "angband.h"
#include "lua.h"

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"effect_type");
 tolua_usertype(tolua_S,"object_type");
 tolua_usertype(tolua_S,"PHYSFS_file");
 tolua_usertype(tolua_S,"magic_power");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* get function: project_range */
static int toluaI_get_spells_project_range(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)project_range);
 return 1;
}

/* set function: project_range */
static int toluaI_set_spells_project_range(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  project_range = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: project_time */
static int toluaI_get_spells_project_time(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)project_time);
 return 1;
}

/* set function: project_time */
static int toluaI_set_spells_project_time(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  project_time = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: project_time_speed */
static int toluaI_get_spells_project_time_speed(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)project_time_speed);
 return 1;
}

/* set function: project_time_speed */
static int toluaI_set_spells_project_time_speed(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  project_time_speed = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: project_time_effect */
static int toluaI_get_spells_project_time_effect(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)project_time_effect);
 return 1;
}

/* set function: project_time_effect */
static int toluaI_set_spells_project_time_effect(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  project_time_effect = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: teleport_player_directed */
static int toluaI_spells_teleport_player_directed00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  teleport_player_directed(rad,dir);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'teleport_player_directed'.");
 return 0;
}

/* function: teleport_away */
static int toluaI_spells_teleport_away00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b m_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dis = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  teleport_away(m_idx,dis);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'teleport_away'.");
 return 0;
}

/* function: teleport_player */
static int toluaI_spells_teleport_player00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b dis = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  teleport_player(dis);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'teleport_player'.");
 return 0;
}

/* function: teleport_player_to */
static int toluaI_spells_teleport_player_to00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b ny = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b nx = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  teleport_player_to(ny,nx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'teleport_player_to'.");
 return 0;
}

/* function: teleport_monster_to */
static int toluaI_spells_teleport_monster_to00(lua_State* tolua_S)
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
  s32b m_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b ny = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b nx = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  teleport_monster_to(m_idx,ny,nx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'teleport_monster_to'.");
 return 0;
}

/* function: teleport_player_level */
static int toluaI_spells_teleport_player_level00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  teleport_player_level();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'teleport_player_level'.");
 return 0;
}

/* function: fetch */
static int toluaI_spells_fetch00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b wgt = ((s32b)  tolua_getnumber(tolua_S,2,0));
  bool require_los = ((bool)  tolua_getbool(tolua_S,3,0));
 {
  fetch(dir,wgt,require_los);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fetch'.");
 return 0;
}

/* function: recall_player */
static int toluaI_spells_recall_player00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b d = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b f = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  recall_player(d,f);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'recall_player'.");
 return 0;
}

/* function: project */
static int toluaI_spells_project00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,9)
 )
 goto tolua_lerror;
 else
 {
  s32b who = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b flg = ((s32b)  tolua_getnumber(tolua_S,7,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,8,-1));
 {
  bool toluaI_ret = (bool)  project(who,rad,y,x,dam,typ,flg,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'project'.");
 return 0;
}

/* function: corrupt_player */
static int toluaI_spells_corrupt_player00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  corrupt_player();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'corrupt_player'.");
 return 0;
}

/* function: grow_things */
static int toluaI_spells_grow_things00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s16b type = ((s16b)  tolua_getnumber(tolua_S,1,0));
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  grow_things(type,rad);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'grow_things'.");
 return 0;
}

/* function: grow_grass */
static int toluaI_spells_grow_grass00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  grow_grass(rad);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'grow_grass'.");
 return 0;
}

/* function: inc_stat */
static int toluaI_spells_inc_stat00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b stat = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  inc_stat(stat);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'inc_stat'.");
 return 0;
}

/* function: dec_stat */
static int toluaI_spells_dec_stat00(lua_State* tolua_S)
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
  s32b stat = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b amount = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b mode = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  dec_stat(stat,amount,mode);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dec_stat'.");
 return 0;
}

/* function: res_stat */
static int toluaI_spells_res_stat00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b stat = ((s32b)  tolua_getnumber(tolua_S,1,0));
  bool full = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  res_stat(stat,full);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'res_stat'.");
 return 0;
}

/* function: do_dec_stat */
static int toluaI_spells_do_dec_stat00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b stat = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b mode = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  do_dec_stat(stat,mode);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_dec_stat'.");
 return 0;
}

/* function: do_res_stat */
static int toluaI_spells_do_res_stat00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b stat = ((s32b)  tolua_getnumber(tolua_S,1,0));
  bool full = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  do_res_stat(stat,full);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_res_stat'.");
 return 0;
}

/* function: do_inc_stat */
static int toluaI_spells_do_inc_stat00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b stat = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  do_inc_stat(stat);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_inc_stat'.");
 return 0;
}

/* function: identify_pack */
static int toluaI_spells_identify_pack00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  identify_pack();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'identify_pack'.");
 return 0;
}

/* function: remove_curse */
static int toluaI_spells_remove_curse00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  remove_curse();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove_curse'.");
 return 0;
}

/* function: remove_all_curse */
static int toluaI_spells_remove_all_curse00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  remove_all_curse();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove_all_curse'.");
 return 0;
}

/* function: restore_level */
static int toluaI_spells_restore_level00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  restore_level();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'restore_level'.");
 return 0;
}

/* function: self_knowledge */
static int toluaI_spells_self_knowledge00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"PHYSFS_file"),1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  PHYSFS_file* fff = ((PHYSFS_file*)  tolua_getusertype(tolua_S,1,NULL));
 {
  self_knowledge(fff);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'self_knowledge'.");
 return 0;
}

/* function: lose_all_info */
static int toluaI_spells_lose_all_info00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  lose_all_info();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lose_all_info'.");
 return 0;
}

/* function: tgt_pt */
static int toluaI_spells_tgt_pt00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b x = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  tgt_pt(&x,&y);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)x);
 tolua_pushnumber(tolua_S,(long)y);
 }
 }
 return 3;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tgt_pt'.");
 return 0;
}

/* function: tgt_pt_prompt */
static int toluaI_spells_tgt_pt_prompt00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  cptr prompt = ((cptr)  tolua_getstring(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  tgt_pt_prompt(prompt,&x,&y);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)x);
 tolua_pushnumber(tolua_S,(long)y);
 }
 }
 return 3;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tgt_pt_prompt'.");
 return 0;
}

/* function: create_artifact */
static int toluaI_spells_create_artifact00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  bool a_scroll = ((bool)  tolua_getbool(tolua_S,2,0));
  bool get_name = ((bool)  tolua_getbool(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  create_artifact(o_ptr,a_scroll,get_name);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_artifact'.");
 return 0;
}

/* function: ident_spell */
static int toluaI_spells_ident_spell00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  ident_spell();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ident_spell'.");
 return 0;
}

/* function: identify_fully */
static int toluaI_spells_identify_fully00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  identify_fully();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'identify_fully'.");
 return 0;
}

/* function: aggravate_monsters */
static int toluaI_spells_aggravate_monsters00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b who = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  aggravate_monsters(who);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'aggravate_monsters'.");
 return 0;
}

/* function: wake_monsters */
static int toluaI_spells_wake_monsters00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,3,0));
  u32b power = ((u32b)  tolua_getnumber(tolua_S,4,0));
 {
  wake_monsters(y,x,rad,power);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wake_monsters'.");
 return 0;
}

/* function: genocide_aux */
static int toluaI_spells_genocide_aux00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  bool player_cast = ((bool)  tolua_getbool(tolua_S,1,0));
  char typ = ((char)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  genocide_aux(player_cast,typ);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'genocide_aux'.");
 return 0;
}

/* function: genocide */
static int toluaI_spells_genocide00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  bool player_cast = ((bool)  tolua_getbool(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  genocide(player_cast);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'genocide'.");
 return 0;
}

/* function: mass_genocide */
static int toluaI_spells_mass_genocide00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  bool player_cast = ((bool)  tolua_getbool(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  mass_genocide(player_cast);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'mass_genocide'.");
 return 0;
}

/* function: probing */
static int toluaI_spells_probing00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  probing();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'probing'.");
 return 0;
}

/* function: wipe */
static int toluaI_spells_wipe00(lua_State* tolua_S)
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
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b r = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  wipe(y1,x1,r);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wipe'.");
 return 0;
}

/* function: destroy_area */
static int toluaI_spells_destroy_area00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b r = ((s32b)  tolua_getnumber(tolua_S,3,0));
  bool full = ((bool)  tolua_getbool(tolua_S,4,0));
  bool bypass = ((bool)  tolua_getbool(tolua_S,5,0));
 {
  destroy_area(y1,x1,r,full,bypass);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'destroy_area'.");
 return 0;
}

/* function: earthquake */
static int toluaI_spells_earthquake00(lua_State* tolua_S)
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
  s32b cy = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b cx = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b r = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  earthquake(cy,cx,r);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'earthquake'.");
 return 0;
}

/* function: lite_room */
static int toluaI_spells_lite_room00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  lite_room(y1,x1);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lite_room'.");
 return 0;
}

/* function: unlite_room */
static int toluaI_spells_unlite_room00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  unlite_room(y1,x1);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'unlite_room'.");
 return 0;
}

/* function: fire_ball_beam */
static int toluaI_spells_fire_ball_beam00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,5,-1));
 {
  bool toluaI_ret = (bool)  fire_ball_beam(typ,dir,dam,rad,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_ball_beam'.");
 return 0;
}

/* function: make_wish */
static int toluaI_spells_make_wish00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  make_wish();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_wish'.");
 return 0;
}

/* function: fire_wave */
static int toluaI_spells_fire_wave00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,9)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b time = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b speed = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b eff = ((s32b)  tolua_getnumber(tolua_S,7,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,8,-1));
 {
  bool toluaI_ret = (bool)  fire_wave(typ,dir,dam,rad,time,speed,eff,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_wave'.");
 return 0;
}

/* function: fire_cloud */
static int toluaI_spells_fire_cloud00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,8)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b time = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b speed = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,7,-1));
 {
  bool toluaI_ret = (bool)  fire_cloud(typ,dir,dam,rad,time,speed,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_cloud'.");
 return 0;
}

/* function: fire_wall */
static int toluaI_spells_fire_wall00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b time = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b speed = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,6,-1));
 {
  bool toluaI_ret = (bool)  fire_wall(typ,dir,dam,time,speed,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_wall'.");
 return 0;
}

/* function: fire_ball */
static int toluaI_spells_fire_ball00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,5,-1));
 {
  bool toluaI_ret = (bool)  fire_ball(typ,dir,dam,rad,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_ball'.");
 return 0;
}

/* function: fire_cone */
static int toluaI_spells_fire_cone00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,5,-1));
 {
  bool toluaI_ret = (bool)  fire_cone(typ,dir,dam,rad,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_cone'.");
 return 0;
}

/* function: fire_bolt */
static int toluaI_spells_fire_bolt00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,4,-1));
 {
  bool toluaI_ret = (bool)  fire_bolt(typ,dir,dam,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_bolt'.");
 return 0;
}

/* function: fire_beam */
static int toluaI_spells_fire_beam00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,4,-1));
 {
  bool toluaI_ret = (bool)  fire_beam(typ,dir,dam,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_beam'.");
 return 0;
}

/* function: fire_druid_ball */
static int toluaI_spells_fire_druid_ball00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b rad = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,5,-1));
 {
  bool toluaI_ret = (bool)  fire_druid_ball(typ,dir,dam,rad,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_druid_ball'.");
 return 0;
}

/* function: fire_druid_bolt */
static int toluaI_spells_fire_druid_bolt00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,4,-1));
 {
  bool toluaI_ret = (bool)  fire_druid_bolt(typ,dir,dam,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_druid_bolt'.");
 return 0;
}

/* function: fire_druid_beam */
static int toluaI_spells_fire_druid_beam00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,4,-1));
 {
  bool toluaI_ret = (bool)  fire_druid_beam(typ,dir,dam,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_druid_beam'.");
 return 0;
}

/* function: fire_bolt_or_beam */
static int toluaI_spells_fire_bolt_or_beam00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b prob = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,5,-1));
 {
  bool toluaI_ret = (bool)  fire_bolt_or_beam(prob,typ,dir,dam,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fire_bolt_or_beam'.");
 return 0;
}

/* function: alchemy */
static int toluaI_spells_alchemy00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  alchemy();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'alchemy'.");
 return 0;
}

/* function: alter_reality */
static int toluaI_spells_alter_reality00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  alter_reality();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'alter_reality'.");
 return 0;
}

/* function: swap_position */
static int toluaI_spells_swap_position00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b lty = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b ltx = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  swap_position(lty,ltx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'swap_position'.");
 return 0;
}

/* function: teleport_swap */
static int toluaI_spells_teleport_swap00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  teleport_swap(dir);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'teleport_swap'.");
 return 0;
}

/* function: project_meteor */
static int toluaI_spells_project_meteor00(lua_State* tolua_S)
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
  s32b radius = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  u32b flg = ((u32b)  tolua_getnumber(tolua_S,4,0));
 {
  project_meteor(radius,typ,dam,flg);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'project_meteor'.");
 return 0;
}

/* function: passwall */
static int toluaI_spells_passwall00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,1,0));
  bool safe = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  passwall(dir,safe);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'passwall'.");
 return 0;
}

/* function: project_hook */
static int toluaI_spells_project_through00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b flg = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b max_range = ((s32b)  tolua_getnumber(tolua_S,5,0));
 {
  bool toluaI_ret = (bool)  project_hook(typ,dir,dam,flg,max_range);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'project_through'.");
 return 0;
}

/* function: reset_recall */
static int toluaI_spells_reset_recall00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  bool no_trepas_max_depth = ((bool)  tolua_getbool(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  reset_recall(no_trepas_max_depth);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reset_recall'.");
 return 0;
}

/* function: get_aim_dir */
static int toluaI_spells_get_aim_dir00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b dp = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  get_aim_dir(&dp);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)dp);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_aim_dir'.");
 return 0;
}

/* function: get_rep_dir */
static int toluaI_spells_get_rep_dir00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b dp = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  get_rep_dir(&dp);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)dp);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_rep_dir'.");
 return 0;
}

/* function: project_hack */
static int toluaI_spells_project_los00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  project_hack(typ,dam);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'project_los'.");
 return 0;
}

/* function: map_area */
static int toluaI_spells_map_area00(lua_State* tolua_S)
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
  s32b cy = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b cx = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b radius = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  map_area(cy,cx,radius);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'map_area'.");
 return 0;
}

/* function: lite_level */
static int toluaI_spells_lite_level00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  bool mark = ((bool)  tolua_getbool(tolua_S,1,0));
 {
  lite_level(mark);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lite_level'.");
 return 0;
}

/* function: wiz_lite */
static int toluaI_spells_wiz_lite00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  bool mark = ((bool)  tolua_getbool(tolua_S,1,0));
 {
  wiz_lite(mark);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wiz_lite'.");
 return 0;
}

/* function: wiz_lite_extra */
static int toluaI_spells_wiz_lite_extra00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  wiz_lite_extra();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wiz_lite_extra'.");
 return 0;
}

/* function: forget_level */
static int toluaI_spells_forget_level00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  forget_level();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'forget_level'.");
 return 0;
}

/* function: darken_level */
static int toluaI_spells_darken_level00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  bool all = ((bool)  tolua_getbool(tolua_S,1,0));
 {
  darken_level(all);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'darken_level'.");
 return 0;
}

/* function: wiz_dark */
static int toluaI_spells_wiz_dark00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  wiz_dark();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wiz_dark'.");
 return 0;
}

/* function: create_between_gate */
static int toluaI_spells_create_between_gate00(lua_State* tolua_S)
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
  s32b dist = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  create_between_gate(dist,y,x);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_between_gate'.");
 return 0;
}

/* get function: min_lev of class  magic_power */
static int toluaI_get_spells_magic_power_min_lev(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->min_lev);
 return 1;
}

/* set function: min_lev of class  magic_power */
static int toluaI_set_spells_magic_power_min_lev(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->min_lev = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mana_cost of class  magic_power */
static int toluaI_get_spells_magic_power_mana_cost(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mana_cost);
 return 1;
}

/* set function: mana_cost of class  magic_power */
static int toluaI_set_spells_magic_power_mana_cost(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mana_cost = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: fail of class  magic_power */
static int toluaI_get_spells_magic_power_fail(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->fail);
 return 1;
}

/* set function: fail of class  magic_power */
static int toluaI_set_spells_magic_power_fail(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->fail = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  magic_power */
static int toluaI_get_spells_magic_power_name(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  magic_power */
static int toluaI_set_spells_magic_power_name(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: desc of class  magic_power */
static int toluaI_get_spells_magic_power_desc(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->desc);
 return 1;
}

/* set function: desc of class  magic_power */
static int toluaI_set_spells_magic_power_desc(lua_State* tolua_S)
{
  magic_power* self = (magic_power*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->desc = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* function: new_magic_power */
static int toluaI_spells_new_magic_power00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b num = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  magic_power* toluaI_ret = (magic_power*)  new_magic_power(num);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"magic_power"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new_magic_power'.");
 return 0;
}

/* function: grab_magic_power */
static int toluaI_spells_get_magic_power00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"magic_power"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  magic_power* m_ptr = ((magic_power*)  tolua_getusertype(tolua_S,1,0));
  s32b num = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  magic_power* toluaI_ret = (magic_power*)  grab_magic_power(m_ptr,num);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"magic_power"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_magic_power'.");
 return 0;
}

/* function: get_magic_power_lua */
static int toluaI_spells_select_magic_power00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"magic_power"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  s32b sn = ((s32b)  tolua_getnumber(tolua_S,1,0));
  magic_power* powers = ((magic_power*)  tolua_getusertype(tolua_S,2,0));
  s32b max_powers = ((s32b)  tolua_getnumber(tolua_S,3,0));
  char* info_fct = ((char*)  tolua_getstring(tolua_S,4,0));
  s32b plev = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b cast_stat = ((s32b)  tolua_getnumber(tolua_S,6,0));
 {
  bool toluaI_ret = (bool)  get_magic_power_lua(&sn,powers,max_powers,info_fct,plev,cast_stat);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)sn);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'select_magic_power'.");
 return 0;
}

/* function: add_new_power */
static int toluaI_spells_add_new_power00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,4,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,9)
 )
 goto tolua_lerror;
 else
 {
  cptr name = ((cptr)  tolua_getstring(tolua_S,1,0));
  cptr desc = ((cptr)  tolua_getstring(tolua_S,2,0));
  cptr gain = ((cptr)  tolua_getstring(tolua_S,3,0));
  cptr lose = ((cptr)  tolua_getstring(tolua_S,4,0));
  byte level = ((byte)  tolua_getnumber(tolua_S,5,0));
  byte cost = ((byte)  tolua_getnumber(tolua_S,6,0));
  byte stat = ((byte)  tolua_getnumber(tolua_S,7,0));
  byte diff = ((byte)  tolua_getnumber(tolua_S,8,0));
 {
  s16b toluaI_ret = (s16b)  add_new_power(name,desc,gain,lose,level,cost,stat,diff);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_new_power'.");
 return 0;
}

/* get function: power_max */
static int toluaI_get_spells_power_max(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)power_max);
 return 1;
}

/* set function: power_max */
static int toluaI_set_spells_power_max(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  power_max = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: lua_spell_chance */
static int toluaI_spells_lua_spell_chance00(lua_State* tolua_S)
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
  s32b chance = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b level = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b skill_level = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b mana = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b cur_mana = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b fail_adj = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b fail_min = ((s32b)  tolua_getnumber(tolua_S,7,0));
 {
  s32b toluaI_ret = (s32b)  lua_spell_chance(chance,level,skill_level,mana,cur_mana,fail_adj,fail_min);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_spell_chance'.");
 return 0;
}

/* function: lua_spell_device_chance */
static int toluaI_spells_lua_spell_device_chance00(lua_State* tolua_S)
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
  s32b skill = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b chance = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b level = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b base_level = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  s32b toluaI_ret = (s32b)  lua_spell_device_chance(skill,chance,level,base_level);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_spell_device_chance'.");
 return 0;
}

/* function: get_school_spell */
static int toluaI_spells_get_school_spell00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"object_type"),1) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  cptr do_what = ((cptr)  tolua_getstring(tolua_S,1,0));
  cptr check_fct = ((cptr)  tolua_getstring(tolua_S,2,0));
  object_type* force_book = ((object_type*)  tolua_getusertype(tolua_S,3,0));
  bool browse = ((bool)  tolua_getbool(tolua_S,4,FALSE));
 {
  s32b toluaI_ret = (s32b)  get_school_spell(do_what,check_fct,force_book,browse);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_school_spell'.");
 return 0;
}

/* get function: last_teleportation_y */
static int toluaI_get_spells_last_teleportation_y(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)last_teleportation_y);
 return 1;
}

/* set function: last_teleportation_y */
static int toluaI_set_spells_last_teleportation_y(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  last_teleportation_y = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: last_teleportation_x */
static int toluaI_get_spells_last_teleportation_x(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)last_teleportation_x);
 return 1;
}

/* set function: last_teleportation_x */
static int toluaI_set_spells_last_teleportation_x(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  last_teleportation_x = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: get_pos_player */
static int toluaI_spells_get_pos_player00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  s32b dis = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b ny = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b nx = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  get_pos_player(dis,&ny,&nx);
 tolua_pushnumber(tolua_S,(long)ny);
 tolua_pushnumber(tolua_S,(long)nx);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_pos_player'.");
 return 0;
}

/* get function: spell_caster */
static int toluaI_get_spells_spell_caster(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)spell_caster);
 return 1;
}

/* set function: spell_caster */
static int toluaI_set_spells_spell_caster(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  spell_caster = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: do_probe */
static int toluaI_spells_do_probe00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b m_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  do_probe(m_idx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_probe'.");
 return 0;
}

/* function: project_m */
static int toluaI_spells_project_m00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,9)
 )
 goto tolua_lerror;
 else
 {
  s32b who = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b r = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b flg = ((s32b)  tolua_getnumber(tolua_S,7,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,8,0));
 {
  bool toluaI_ret = (bool)  project_m(who,r,y,x,dam,typ,flg,dir);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'project_m'.");
 return 0;
}

/* function: project_o */
static int toluaI_spells_project_o00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,9)
 )
 goto tolua_lerror;
 else
 {
  s32b who = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b r = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b flg = ((s32b)  tolua_getnumber(tolua_S,7,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,8,0));
 {
  bool toluaI_ret = (bool)  project_o(who,r,y,x,dam,typ,flg,dir);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'project_o'.");
 return 0;
}

/* function: project_f */
static int toluaI_spells_project_f00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,9)
 )
 goto tolua_lerror;
 else
 {
  s32b who = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b r = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b flg = ((s32b)  tolua_getnumber(tolua_S,7,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,8,0));
 {
  bool toluaI_ret = (bool)  project_f(who,r,y,x,dam,typ,flg,dir);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'project_f'.");
 return 0;
}

/* function: project_p */
static int toluaI_spells_project_p00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,10)
 )
 goto tolua_lerror;
 else
 {
  s32b who = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b r = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b typ = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b a_rad = ((s32b)  tolua_getnumber(tolua_S,7,0));
  s32b flg = ((s32b)  tolua_getnumber(tolua_S,8,0));
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,9,0));
 {
  bool toluaI_ret = (bool)  project_p(who,r,y,x,dam,typ,a_rad,flg,dir);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'project_p'.");
 return 0;
}

/* function: projectable */
static int toluaI_spells_projectable00(lua_State* tolua_S)
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
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b y2 = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b x2 = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  bool toluaI_ret = (bool)  projectable(y1,x1,y2,x2);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'projectable'.");
 return 0;
}

/* get function: who of class  effect_type */
static int toluaI_get_spells_effect_type_who(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->who);
 return 1;
}

/* set function: who of class  effect_type */
static int toluaI_set_spells_effect_type_who(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->who = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: time of class  effect_type */
static int toluaI_get_spells_effect_type_time(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->time);
 return 1;
}

/* set function: time of class  effect_type */
static int toluaI_set_spells_effect_type_time(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->time = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dam of class  effect_type */
static int toluaI_get_spells_effect_type_dam(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dam);
 return 1;
}

/* set function: dam of class  effect_type */
static int toluaI_set_spells_effect_type_dam(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dam = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: type of class  effect_type */
static int toluaI_get_spells_effect_type_type(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->type);
 return 1;
}

/* set function: type of class  effect_type */
static int toluaI_set_spells_effect_type_type(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->type = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: cy of class  effect_type */
static int toluaI_get_spells_effect_type_cy(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->cy);
 return 1;
}

/* set function: cy of class  effect_type */
static int toluaI_set_spells_effect_type_cy(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->cy = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: cx of class  effect_type */
static int toluaI_get_spells_effect_type_cx(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->cx);
 return 1;
}

/* set function: cx of class  effect_type */
static int toluaI_set_spells_effect_type_cx(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->cx = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: rad of class  effect_type */
static int toluaI_get_spells_effect_type_rad(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->rad);
 return 1;
}

/* set function: rad of class  effect_type */
static int toluaI_set_spells_effect_type_rad(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->rad = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: speed of class  effect_type */
static int toluaI_get_spells_effect_type_speed(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->speed);
 return 1;
}

/* set function: speed of class  effect_type */
static int toluaI_set_spells_effect_type_speed(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->speed = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  effect_type */
static int toluaI_get_spells_effect_type_flags(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->flags);
 return 1;
}

/* set function: flags of class  effect_type */
static int toluaI_set_spells_effect_type_flags(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->flags = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: energy of class  effect_type */
static int toluaI_get_spells_effect_type_energy(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->energy);
 return 1;
}

/* set function: energy of class  effect_type */
static int toluaI_set_spells_effect_type_energy(lua_State* tolua_S)
{
  effect_type* self = (effect_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->energy = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: effects */
static int toluaI_get_spells_lasting_effects(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_EFFECTS)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&effects[toluaI_index],tolua_tag(tolua_S,"effect_type"));
 return 1;
}

/* set function: effects */
static int toluaI_set_spells_lasting_effects(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_EFFECTS)
 tolua_error(tolua_S,"array indexing out of range.");
  effects[toluaI_index] = *((effect_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* Open function */
int tolua_spells_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_constant(tolua_S,NULL,"DEFAULT_RADIUS",DEFAULT_RADIUS);
 tolua_constant(tolua_S,NULL,"WHO_PLAYER",WHO_PLAYER);
 tolua_constant(tolua_S,NULL,"WHO_TRAP",WHO_TRAP);
 tolua_constant(tolua_S,NULL,"WHO_DUNGEON",WHO_DUNGEON);
 tolua_constant(tolua_S,NULL,"WHO_FLOOR",WHO_FLOOR);
 tolua_constant(tolua_S,NULL,"PROJECT_JUMP",PROJECT_JUMP);
 tolua_constant(tolua_S,NULL,"PROJECT_BEAM",PROJECT_BEAM);
 tolua_constant(tolua_S,NULL,"PROJECT_THRU",PROJECT_THRU);
 tolua_constant(tolua_S,NULL,"PROJECT_STOP",PROJECT_STOP);
 tolua_constant(tolua_S,NULL,"PROJECT_GRID",PROJECT_GRID);
 tolua_constant(tolua_S,NULL,"PROJECT_ITEM",PROJECT_ITEM);
 tolua_constant(tolua_S,NULL,"PROJECT_KILL",PROJECT_KILL);
 tolua_constant(tolua_S,NULL,"PROJECT_HIDE",PROJECT_HIDE);
 tolua_constant(tolua_S,NULL,"PROJECT_HIDE_BLAST",PROJECT_HIDE_BLAST);
 tolua_constant(tolua_S,NULL,"PROJECT_VIEWABLE",PROJECT_VIEWABLE);
 tolua_constant(tolua_S,NULL,"PROJECT_METEOR_SHOWER",PROJECT_METEOR_SHOWER);
 tolua_constant(tolua_S,NULL,"PROJECT_BLAST",PROJECT_BLAST);
 tolua_constant(tolua_S,NULL,"PROJECT_PANEL",PROJECT_PANEL);
 tolua_constant(tolua_S,NULL,"PROJECT_ALL",PROJECT_ALL);
 tolua_constant(tolua_S,NULL,"PROJECT_WALL",PROJECT_WALL);
 tolua_constant(tolua_S,NULL,"PROJECT_MANA_PATH",PROJECT_MANA_PATH);
 tolua_constant(tolua_S,NULL,"PROJECT_ABSORB_MANA",PROJECT_ABSORB_MANA);
 tolua_constant(tolua_S,NULL,"PROJECT_STAY",PROJECT_STAY);
 tolua_constant(tolua_S,NULL,"PROJECT_CASTER",PROJECT_CASTER);
 tolua_constant(tolua_S,NULL,"PROJECT_CONE",PROJECT_CONE);
 tolua_constant(tolua_S,NULL,"PROJECT_NO_REFLECT",PROJECT_NO_REFLECT);
 tolua_constant(tolua_S,NULL,"PROJECT_SILENT",PROJECT_SILENT);
 tolua_constant(tolua_S,NULL,"PROJECT_NO_RADIUS_DAM",PROJECT_NO_RADIUS_DAM);
 tolua_globalvar(tolua_S,"project_range",toluaI_get_spells_project_range,toluaI_set_spells_project_range);
 tolua_globalvar(tolua_S,"project_time",toluaI_get_spells_project_time,toluaI_set_spells_project_time);
 tolua_globalvar(tolua_S,"project_time_speed",toluaI_get_spells_project_time_speed,toluaI_set_spells_project_time_speed);
 tolua_globalvar(tolua_S,"project_time_effect",toluaI_get_spells_project_time_effect,toluaI_set_spells_project_time_effect);
 tolua_function(tolua_S,NULL,"teleport_player_directed",toluaI_spells_teleport_player_directed00);
 tolua_function(tolua_S,NULL,"teleport_away",toluaI_spells_teleport_away00);
 tolua_function(tolua_S,NULL,"teleport_player",toluaI_spells_teleport_player00);
 tolua_function(tolua_S,NULL,"teleport_player_to",toluaI_spells_teleport_player_to00);
 tolua_function(tolua_S,NULL,"teleport_monster_to",toluaI_spells_teleport_monster_to00);
 tolua_function(tolua_S,NULL,"teleport_player_level",toluaI_spells_teleport_player_level00);
 tolua_function(tolua_S,NULL,"fetch",toluaI_spells_fetch00);
 tolua_function(tolua_S,NULL,"recall_player",toluaI_spells_recall_player00);
 tolua_function(tolua_S,NULL,"project",toluaI_spells_project00);
 tolua_function(tolua_S,NULL,"corrupt_player",toluaI_spells_corrupt_player00);
 tolua_function(tolua_S,NULL,"grow_things",toluaI_spells_grow_things00);
 tolua_function(tolua_S,NULL,"grow_grass",toluaI_spells_grow_grass00);
 tolua_constant(tolua_S,NULL,"STAT_DEC_TEMPORARY",STAT_DEC_TEMPORARY);
 tolua_constant(tolua_S,NULL,"STAT_DEC_NORMAL",STAT_DEC_NORMAL);
 tolua_constant(tolua_S,NULL,"STAT_DEC_PERMANENT",STAT_DEC_PERMANENT);
 tolua_function(tolua_S,NULL,"inc_stat",toluaI_spells_inc_stat00);
 tolua_function(tolua_S,NULL,"dec_stat",toluaI_spells_dec_stat00);
 tolua_function(tolua_S,NULL,"res_stat",toluaI_spells_res_stat00);
 tolua_function(tolua_S,NULL,"do_dec_stat",toluaI_spells_do_dec_stat00);
 tolua_function(tolua_S,NULL,"do_res_stat",toluaI_spells_do_res_stat00);
 tolua_function(tolua_S,NULL,"do_inc_stat",toluaI_spells_do_inc_stat00);
 tolua_function(tolua_S,NULL,"identify_pack",toluaI_spells_identify_pack00);
 tolua_function(tolua_S,NULL,"remove_curse",toluaI_spells_remove_curse00);
 tolua_function(tolua_S,NULL,"remove_all_curse",toluaI_spells_remove_all_curse00);
 tolua_function(tolua_S,NULL,"restore_level",toluaI_spells_restore_level00);
 tolua_function(tolua_S,NULL,"self_knowledge",toluaI_spells_self_knowledge00);
 tolua_function(tolua_S,NULL,"lose_all_info",toluaI_spells_lose_all_info00);
 tolua_function(tolua_S,NULL,"tgt_pt",toluaI_spells_tgt_pt00);
 tolua_function(tolua_S,NULL,"tgt_pt_prompt",toluaI_spells_tgt_pt_prompt00);
 tolua_function(tolua_S,NULL,"create_artifact",toluaI_spells_create_artifact00);
 tolua_function(tolua_S,NULL,"ident_spell",toluaI_spells_ident_spell00);
 tolua_function(tolua_S,NULL,"identify_fully",toluaI_spells_identify_fully00);
 tolua_function(tolua_S,NULL,"aggravate_monsters",toluaI_spells_aggravate_monsters00);
 tolua_function(tolua_S,NULL,"wake_monsters",toluaI_spells_wake_monsters00);
 tolua_function(tolua_S,NULL,"genocide_aux",toluaI_spells_genocide_aux00);
 tolua_function(tolua_S,NULL,"genocide",toluaI_spells_genocide00);
 tolua_function(tolua_S,NULL,"mass_genocide",toluaI_spells_mass_genocide00);
 tolua_function(tolua_S,NULL,"probing",toluaI_spells_probing00);
 tolua_function(tolua_S,NULL,"wipe",toluaI_spells_wipe00);
 tolua_function(tolua_S,NULL,"destroy_area",toluaI_spells_destroy_area00);
 tolua_function(tolua_S,NULL,"earthquake",toluaI_spells_earthquake00);
 tolua_function(tolua_S,NULL,"lite_room",toluaI_spells_lite_room00);
 tolua_function(tolua_S,NULL,"unlite_room",toluaI_spells_unlite_room00);
 tolua_function(tolua_S,NULL,"fire_ball_beam",toluaI_spells_fire_ball_beam00);
 tolua_function(tolua_S,NULL,"make_wish",toluaI_spells_make_wish00);
 tolua_function(tolua_S,NULL,"fire_wave",toluaI_spells_fire_wave00);
 tolua_constant(tolua_S,NULL,"EFF_WAVE",EFF_WAVE);
 tolua_constant(tolua_S,NULL,"EFF_LAST",EFF_LAST);
 tolua_constant(tolua_S,NULL,"EFF_STORM",EFF_STORM);
 tolua_constant(tolua_S,NULL,"EFF_DIR1",EFF_DIR1);
 tolua_constant(tolua_S,NULL,"EFF_DIR2",EFF_DIR2);
 tolua_constant(tolua_S,NULL,"EFF_DIR3",EFF_DIR3);
 tolua_constant(tolua_S,NULL,"EFF_DIR4",EFF_DIR4);
 tolua_constant(tolua_S,NULL,"EFF_DIR6",EFF_DIR6);
 tolua_constant(tolua_S,NULL,"EFF_DIR7",EFF_DIR7);
 tolua_constant(tolua_S,NULL,"EFF_DIR8",EFF_DIR8);
 tolua_constant(tolua_S,NULL,"EFF_DIR9",EFF_DIR9);
 tolua_function(tolua_S,NULL,"fire_cloud",toluaI_spells_fire_cloud00);
 tolua_function(tolua_S,NULL,"fire_wall",toluaI_spells_fire_wall00);
 tolua_function(tolua_S,NULL,"fire_ball",toluaI_spells_fire_ball00);
 tolua_function(tolua_S,NULL,"fire_cone",toluaI_spells_fire_cone00);
 tolua_function(tolua_S,NULL,"fire_bolt",toluaI_spells_fire_bolt00);
 tolua_function(tolua_S,NULL,"fire_beam",toluaI_spells_fire_beam00);
 tolua_function(tolua_S,NULL,"fire_druid_ball",toluaI_spells_fire_druid_ball00);
 tolua_function(tolua_S,NULL,"fire_druid_bolt",toluaI_spells_fire_druid_bolt00);
 tolua_function(tolua_S,NULL,"fire_druid_beam",toluaI_spells_fire_druid_beam00);
 tolua_function(tolua_S,NULL,"fire_bolt_or_beam",toluaI_spells_fire_bolt_or_beam00);
 tolua_function(tolua_S,NULL,"alchemy",toluaI_spells_alchemy00);
 tolua_function(tolua_S,NULL,"alter_reality",toluaI_spells_alter_reality00);
 tolua_function(tolua_S,NULL,"swap_position",toluaI_spells_swap_position00);
 tolua_function(tolua_S,NULL,"teleport_swap",toluaI_spells_teleport_swap00);
 tolua_function(tolua_S,NULL,"project_meteor",toluaI_spells_project_meteor00);
 tolua_function(tolua_S,NULL,"passwall",toluaI_spells_passwall00);
 tolua_function(tolua_S,NULL,"project_through",toluaI_spells_project_through00);
 tolua_function(tolua_S,NULL,"reset_recall",toluaI_spells_reset_recall00);
 tolua_function(tolua_S,NULL,"get_aim_dir",toluaI_spells_get_aim_dir00);
 tolua_function(tolua_S,NULL,"get_rep_dir",toluaI_spells_get_rep_dir00);
 tolua_function(tolua_S,NULL,"project_los",toluaI_spells_project_los00);
 tolua_function(tolua_S,NULL,"map_area",toluaI_spells_map_area00);
 tolua_function(tolua_S,NULL,"lite_level",toluaI_spells_lite_level00);
 tolua_function(tolua_S,NULL,"wiz_lite",toluaI_spells_wiz_lite00);
 tolua_function(tolua_S,NULL,"wiz_lite_extra",toluaI_spells_wiz_lite_extra00);
 tolua_function(tolua_S,NULL,"forget_level",toluaI_spells_forget_level00);
 tolua_function(tolua_S,NULL,"darken_level",toluaI_spells_darken_level00);
 tolua_function(tolua_S,NULL,"wiz_dark",toluaI_spells_wiz_dark00);
 tolua_function(tolua_S,NULL,"create_between_gate",toluaI_spells_create_between_gate00);
 tolua_cclass(tolua_S,"magic_power","");
 tolua_tablevar(tolua_S,"magic_power","min_lev",toluaI_get_spells_magic_power_min_lev,toluaI_set_spells_magic_power_min_lev);
 tolua_tablevar(tolua_S,"magic_power","mana_cost",toluaI_get_spells_magic_power_mana_cost,toluaI_set_spells_magic_power_mana_cost);
 tolua_tablevar(tolua_S,"magic_power","fail",toluaI_get_spells_magic_power_fail,toluaI_set_spells_magic_power_fail);
 tolua_tablevar(tolua_S,"magic_power","name",toluaI_get_spells_magic_power_name,toluaI_set_spells_magic_power_name);
 tolua_tablevar(tolua_S,"magic_power","desc",toluaI_get_spells_magic_power_desc,toluaI_set_spells_magic_power_desc);
 tolua_function(tolua_S,NULL,"new_magic_power",toluaI_spells_new_magic_power00);
 tolua_function(tolua_S,NULL,"get_magic_power",toluaI_spells_get_magic_power00);
 tolua_function(tolua_S,NULL,"select_magic_power",toluaI_spells_select_magic_power00);
 tolua_function(tolua_S,NULL,"add_new_power",toluaI_spells_add_new_power00);
 tolua_globalvar(tolua_S,"power_max",toluaI_get_spells_power_max,toluaI_set_spells_power_max);
 tolua_function(tolua_S,NULL,"lua_spell_chance",toluaI_spells_lua_spell_chance00);
 tolua_function(tolua_S,NULL,"lua_spell_device_chance",toluaI_spells_lua_spell_device_chance00);
 tolua_function(tolua_S,NULL,"get_school_spell",toluaI_spells_get_school_spell00);
 tolua_globalvar(tolua_S,"last_teleportation_y",toluaI_get_spells_last_teleportation_y,toluaI_set_spells_last_teleportation_y);
 tolua_globalvar(tolua_S,"last_teleportation_x",toluaI_get_spells_last_teleportation_x,toluaI_set_spells_last_teleportation_x);
 tolua_function(tolua_S,NULL,"get_pos_player",toluaI_spells_get_pos_player00);
 tolua_globalvar(tolua_S,"spell_caster",toluaI_get_spells_spell_caster,toluaI_set_spells_spell_caster);
 tolua_function(tolua_S,NULL,"do_probe",toluaI_spells_do_probe00);
 tolua_function(tolua_S,NULL,"project_m",toluaI_spells_project_m00);
 tolua_function(tolua_S,NULL,"project_o",toluaI_spells_project_o00);
 tolua_function(tolua_S,NULL,"project_f",toluaI_spells_project_f00);
 tolua_function(tolua_S,NULL,"project_p",toluaI_spells_project_p00);
 tolua_function(tolua_S,NULL,"projectable",toluaI_spells_projectable00);
 tolua_cclass(tolua_S,"effect_type","");
 tolua_tablevar(tolua_S,"effect_type","who",toluaI_get_spells_effect_type_who,toluaI_set_spells_effect_type_who);
 tolua_tablevar(tolua_S,"effect_type","time",toluaI_get_spells_effect_type_time,toluaI_set_spells_effect_type_time);
 tolua_tablevar(tolua_S,"effect_type","dam",toluaI_get_spells_effect_type_dam,toluaI_set_spells_effect_type_dam);
 tolua_tablevar(tolua_S,"effect_type","type",toluaI_get_spells_effect_type_type,toluaI_set_spells_effect_type_type);
 tolua_tablevar(tolua_S,"effect_type","cy",toluaI_get_spells_effect_type_cy,toluaI_set_spells_effect_type_cy);
 tolua_tablevar(tolua_S,"effect_type","cx",toluaI_get_spells_effect_type_cx,toluaI_set_spells_effect_type_cx);
 tolua_tablevar(tolua_S,"effect_type","rad",toluaI_get_spells_effect_type_rad,toluaI_set_spells_effect_type_rad);
 tolua_tablevar(tolua_S,"effect_type","speed",toluaI_get_spells_effect_type_speed,toluaI_set_spells_effect_type_speed);
 tolua_tablevar(tolua_S,"effect_type","flags",toluaI_get_spells_effect_type_flags,toluaI_set_spells_effect_type_flags);
 tolua_tablevar(tolua_S,"effect_type","energy",toluaI_get_spells_effect_type_energy,toluaI_set_spells_effect_type_energy);
 tolua_constant(tolua_S,NULL,"MAX_EFFECTS",MAX_EFFECTS);
 tolua_globalarray(tolua_S,"lasting_effects",toluaI_get_spells_lasting_effects,toluaI_set_spells_lasting_effects);
 return 1;
}
/* Close function */
void tolua_spells_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DEFAULT_RADIUS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"WHO_PLAYER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"WHO_TRAP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"WHO_DUNGEON");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"WHO_FLOOR");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_JUMP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_BEAM");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_THRU");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_STOP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_GRID");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_ITEM");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_KILL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_HIDE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_HIDE_BLAST");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_VIEWABLE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_METEOR_SHOWER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_BLAST");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_PANEL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_ALL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_WALL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_MANA_PATH");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_ABSORB_MANA");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_STAY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_CASTER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_CONE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_NO_REFLECT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_SILENT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"PROJECT_NO_RADIUS_DAM");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"project_range"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"project_time"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"project_time_speed"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"project_time_effect"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"teleport_player_directed");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"teleport_away");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"teleport_player");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"teleport_player_to");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"teleport_monster_to");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"teleport_player_level");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fetch");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"recall_player");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"project");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"corrupt_player");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"grow_things");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"grow_grass");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"STAT_DEC_TEMPORARY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"STAT_DEC_NORMAL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"STAT_DEC_PERMANENT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"inc_stat");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"dec_stat");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"res_stat");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_dec_stat");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_res_stat");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_inc_stat");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"identify_pack");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"remove_curse");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"remove_all_curse");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"restore_level");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"self_knowledge");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lose_all_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"tgt_pt");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"tgt_pt_prompt");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"create_artifact");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ident_spell");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"identify_fully");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"aggravate_monsters");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wake_monsters");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"genocide_aux");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"genocide");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"mass_genocide");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"probing");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wipe");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"destroy_area");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"earthquake");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lite_room");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"unlite_room");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_ball_beam");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"make_wish");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_wave");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_WAVE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_LAST");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_STORM");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_DIR1");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_DIR2");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_DIR3");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_DIR4");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_DIR6");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_DIR7");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_DIR8");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"EFF_DIR9");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_cloud");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_wall");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_ball");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_cone");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_bolt");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_beam");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_druid_ball");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_druid_bolt");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_druid_beam");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fire_bolt_or_beam");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"alchemy");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"alter_reality");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"swap_position");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"teleport_swap");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"project_meteor");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"passwall");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"project_through");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"reset_recall");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_aim_dir");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_rep_dir");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"project_los");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"map_area");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lite_level");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wiz_lite");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wiz_lite_extra");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"forget_level");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"darken_level");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wiz_dark");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"create_between_gate");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"magic_power");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"new_magic_power");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_magic_power");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"select_magic_power");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"add_new_power");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"power_max"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lua_spell_chance");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lua_spell_device_chance");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_school_spell");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"last_teleportation_y"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"last_teleportation_x"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_pos_player");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"spell_caster"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_probe");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"project_m");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"project_o");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"project_f");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"project_p");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"projectable");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"effect_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MAX_EFFECTS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lasting_effects");
}
