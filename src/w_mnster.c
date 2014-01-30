/*
** Lua binding: monster
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:39 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_monster_open (lua_State* tolua_S);
void tolua_monster_close (lua_State* tolua_S);

#include "angband.h"
static monster_type lua_monster_forge;
static monster_type *lua_get_monster(s32b m_idx){return flag_get_monster(&monst_list, m_idx);}
static char *lua_monster_desc(monster_type *m_ptr, s32b mode){static char buf[200]; monster_desc(buf, m_ptr, mode); return buf;}
static char *lua_monster_race_desc(s32b r_idx, s32b ego){static char buf[200]; monster_race_desc(buf, r_idx, ego); return buf;}

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"flags_type");
 tolua_usertype(tolua_S,"monster_type");
 tolua_usertype(tolua_S,"monster_blow");
 tolua_usertype(tolua_S,"monster_ego");
 tolua_usertype(tolua_S,"object_type");
 tolua_usertype(tolua_S,"monster_race");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* get function: lua_monster_forge */
static int toluaI_get_monster_monster_forge(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&lua_monster_forge,tolua_tag(tolua_S,"monster_type"));
 return 1;
}

/* set function: lua_monster_forge */
static int toluaI_set_monster_monster_forge(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"monster_type"),0))
 TOLUA_ERR_ASSIGN;
  lua_monster_forge = *((monster_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: method of class  monster_blow */
static int toluaI_get_monster_monster_blow_method(lua_State* tolua_S)
{
  monster_blow* self = (monster_blow*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->method);
 return 1;
}

/* set function: method of class  monster_blow */
static int toluaI_set_monster_monster_blow_method(lua_State* tolua_S)
{
  monster_blow* self = (monster_blow*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->method = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: effect of class  monster_blow */
static int toluaI_get_monster_monster_blow_effect(lua_State* tolua_S)
{
  monster_blow* self = (monster_blow*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->effect);
 return 1;
}

/* set function: effect of class  monster_blow */
static int toluaI_set_monster_monster_blow_effect(lua_State* tolua_S)
{
  monster_blow* self = (monster_blow*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->effect = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_dice of class  monster_blow */
static int toluaI_get_monster_monster_blow_d_dice(lua_State* tolua_S)
{
  monster_blow* self = (monster_blow*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_dice);
 return 1;
}

/* set function: d_dice of class  monster_blow */
static int toluaI_set_monster_monster_blow_d_dice(lua_State* tolua_S)
{
  monster_blow* self = (monster_blow*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_dice = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_side of class  monster_blow */
static int toluaI_get_monster_monster_blow_d_side(lua_State* tolua_S)
{
  monster_blow* self = (monster_blow*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_side);
 return 1;
}

/* set function: d_side of class  monster_blow */
static int toluaI_set_monster_monster_blow_d_side(lua_State* tolua_S)
{
  monster_blow* self = (monster_blow*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_side = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  monster_race */
static int toluaI_get_monster_monster_race_name(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  monster_race */
static int toluaI_set_monster_monster_race_name(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: text of class  monster_race */
static int toluaI_get_monster_monster_race_text(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->text);
 return 1;
}

/* set function: text of class  monster_race */
static int toluaI_set_monster_monster_race_text(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->text = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: hdice of class  monster_race */
static int toluaI_get_monster_monster_race_hdice(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->hdice);
 return 1;
}

/* set function: hdice of class  monster_race */
static int toluaI_set_monster_monster_race_hdice(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->hdice = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: hside of class  monster_race */
static int toluaI_get_monster_monster_race_hside(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->hside);
 return 1;
}

/* set function: hside of class  monster_race */
static int toluaI_set_monster_monster_race_hside(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->hside = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ac of class  monster_race */
static int toluaI_get_monster_monster_race_ac(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ac);
 return 1;
}

/* set function: ac of class  monster_race */
static int toluaI_set_monster_monster_race_ac(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ac = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sleep of class  monster_race */
static int toluaI_get_monster_monster_race_sleep(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sleep);
 return 1;
}

/* set function: sleep of class  monster_race */
static int toluaI_set_monster_monster_race_sleep(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sleep = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: aaf of class  monster_race */
static int toluaI_get_monster_monster_race_aaf(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->aaf);
 return 1;
}

/* set function: aaf of class  monster_race */
static int toluaI_set_monster_monster_race_aaf(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->aaf = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: speed of class  monster_race */
static int toluaI_get_monster_monster_race_speed(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->speed);
 return 1;
}

/* set function: speed of class  monster_race */
static int toluaI_set_monster_monster_race_speed(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->speed = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mexp of class  monster_race */
static int toluaI_get_monster_monster_race_mexp(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mexp);
 return 1;
}

/* set function: mexp of class  monster_race */
static int toluaI_set_monster_monster_race_mexp(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mexp = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: weight of class  monster_race */
static int toluaI_get_monster_monster_race_weight(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->weight);
 return 1;
}

/* set function: weight of class  monster_race */
static int toluaI_set_monster_monster_race_weight(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->weight = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: freq_inate of class  monster_race */
static int toluaI_get_monster_monster_race_freq_inate(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->freq_inate);
 return 1;
}

/* set function: freq_inate of class  monster_race */
static int toluaI_set_monster_monster_race_freq_inate(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->freq_inate = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: freq_spell of class  monster_race */
static int toluaI_get_monster_monster_race_freq_spell(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->freq_spell);
 return 1;
}

/* set function: freq_spell of class  monster_race */
static int toluaI_set_monster_monster_race_freq_spell(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->freq_spell = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  monster_race */
static int toluaI_get_monster_monster_race_flags(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  monster_race */
static int toluaI_set_monster_monster_race_flags(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: spells of class  monster_race */
static int toluaI_get_monster_monster_race_spells(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->spells,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: spells of class  monster_race */
static int toluaI_set_monster_monster_race_spells(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->spells = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: blow of class  monster_race */
static int toluaI_get_monster_monster_race_blow(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->blow,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: blow of class  monster_race */
static int toluaI_set_monster_monster_race_blow(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->blow = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: body_parts of class  monster_race */
static int toluaI_get_monster_monster_race_body_parts(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->body_parts,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: body_parts of class  monster_race */
static int toluaI_set_monster_monster_race_body_parts(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->body_parts = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: level of class  monster_race */
static int toluaI_get_monster_monster_race_level(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->level);
 return 1;
}

/* set function: level of class  monster_race */
static int toluaI_set_monster_monster_race_level(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->level = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: rarity of class  monster_race */
static int toluaI_get_monster_monster_race_rarity(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->rarity);
 return 1;
}

/* set function: rarity of class  monster_race */
static int toluaI_set_monster_monster_race_rarity(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->rarity = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_attr of class  monster_race */
static int toluaI_get_monster_monster_race_d_attr(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_attr);
 return 1;
}

/* set function: d_attr of class  monster_race */
static int toluaI_set_monster_monster_race_d_attr(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_char of class  monster_race */
static int toluaI_get_monster_monster_race_d_char(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_char);
 return 1;
}

/* set function: d_char of class  monster_race */
static int toluaI_set_monster_monster_race_d_char(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sval of class  monster_race */
static int toluaI_get_monster_monster_race_sval(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sval);
 return 1;
}

/* set function: sval of class  monster_race */
static int toluaI_set_monster_monster_race_sval(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sval = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_attr of class  monster_race */
static int toluaI_get_monster_monster_race_x_attr(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_attr);
 return 1;
}

/* set function: x_attr of class  monster_race */
static int toluaI_set_monster_monster_race_x_attr(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_char of class  monster_race */
static int toluaI_get_monster_monster_race_x_char(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_char);
 return 1;
}

/* set function: x_char of class  monster_race */
static int toluaI_set_monster_monster_race_x_char(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_num of class  monster_race */
static int toluaI_get_monster_monster_race_max_num(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_num);
 return 1;
}

/* set function: max_num of class  monster_race */
static int toluaI_set_monster_monster_race_max_num(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_num = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: cur_num of class  monster_race */
static int toluaI_get_monster_monster_race_cur_num(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->cur_num);
 return 1;
}

/* set function: cur_num of class  monster_race */
static int toluaI_set_monster_monster_race_cur_num(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->cur_num = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_sights of class  monster_race */
static int toluaI_get_monster_monster_race_r_sights(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_sights);
 return 1;
}

/* set function: r_sights of class  monster_race */
static int toluaI_set_monster_monster_race_r_sights(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_sights = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_deaths of class  monster_race */
static int toluaI_get_monster_monster_race_r_deaths(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_deaths);
 return 1;
}

/* set function: r_deaths of class  monster_race */
static int toluaI_set_monster_monster_race_r_deaths(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_deaths = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_pkills of class  monster_race */
static int toluaI_get_monster_monster_race_r_pkills(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_pkills);
 return 1;
}

/* set function: r_pkills of class  monster_race */
static int toluaI_set_monster_monster_race_r_pkills(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_pkills = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_tkills of class  monster_race */
static int toluaI_get_monster_monster_race_r_tkills(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_tkills);
 return 1;
}

/* set function: r_tkills of class  monster_race */
static int toluaI_set_monster_monster_race_r_tkills(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_tkills = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_wake of class  monster_race */
static int toluaI_get_monster_monster_race_r_wake(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_wake);
 return 1;
}

/* set function: r_wake of class  monster_race */
static int toluaI_set_monster_monster_race_r_wake(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_wake = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_ignore of class  monster_race */
static int toluaI_get_monster_monster_race_r_ignore(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_ignore);
 return 1;
}

/* set function: r_ignore of class  monster_race */
static int toluaI_set_monster_monster_race_r_ignore(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_ignore = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_xtra1 of class  monster_race */
static int toluaI_get_monster_monster_race_r_xtra1(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_xtra1);
 return 1;
}

/* set function: r_xtra1 of class  monster_race */
static int toluaI_set_monster_monster_race_r_xtra1(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_xtra1 = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_xtra2 of class  monster_race */
static int toluaI_get_monster_monster_race_r_xtra2(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_xtra2);
 return 1;
}

/* set function: r_xtra2 of class  monster_race */
static int toluaI_set_monster_monster_race_r_xtra2(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_xtra2 = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_drop_gold of class  monster_race */
static int toluaI_get_monster_monster_race_r_drop_gold(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_drop_gold);
 return 1;
}

/* set function: r_drop_gold of class  monster_race */
static int toluaI_set_monster_monster_race_r_drop_gold(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_drop_gold = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_drop_item of class  monster_race */
static int toluaI_get_monster_monster_race_r_drop_item(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_drop_item);
 return 1;
}

/* set function: r_drop_item of class  monster_race */
static int toluaI_set_monster_monster_race_r_drop_item(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_drop_item = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_cast_inate of class  monster_race */
static int toluaI_get_monster_monster_race_r_cast_inate(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_cast_inate);
 return 1;
}

/* set function: r_cast_inate of class  monster_race */
static int toluaI_set_monster_monster_race_r_cast_inate(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_cast_inate = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_cast_spell of class  monster_race */
static int toluaI_get_monster_monster_race_r_cast_spell(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_cast_spell);
 return 1;
}

/* set function: r_cast_spell of class  monster_race */
static int toluaI_set_monster_monster_race_r_cast_spell(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_cast_spell = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_blows of class  monster_race */
static int toluaI_get_monster_monster_race_r_blows(lua_State* tolua_S)
{
 int toluaI_index;
  monster_race* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (monster_race*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->r_blows[toluaI_index]);
 return 1;
}

/* set function: r_blows of class  monster_race */
static int toluaI_set_monster_monster_race_r_blows(lua_State* tolua_S)
{
 int toluaI_index;
  monster_race* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (monster_race*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=4)
 tolua_error(tolua_S,"array indexing out of range.");
  self->r_blows[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: r_flags of class  monster_race */
static int toluaI_get_monster_monster_race_r_flags(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->r_flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: r_flags of class  monster_race */
static int toluaI_set_monster_monster_race_r_flags(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->r_flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: total_visible of class  monster_race */
static int toluaI_get_monster_monster_race_total_visible(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->total_visible);
 return 1;
}

/* set function: total_visible of class  monster_race */
static int toluaI_set_monster_monster_race_total_visible(lua_State* tolua_S)
{
  monster_race* self = (monster_race*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->total_visible = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  monster_ego */
static int toluaI_get_monster_monster_ego_name(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  monster_ego */
static int toluaI_set_monster_monster_ego_name(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: before of class  monster_ego */
static int toluaI_get_monster_monster_ego_before(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->before);
 return 1;
}

/* set function: before of class  monster_ego */
static int toluaI_set_monster_monster_ego_before(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->before = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: blow of class  monster_ego */
static int toluaI_get_monster_monster_ego_blow(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->blow,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: blow of class  monster_ego */
static int toluaI_set_monster_monster_ego_blow(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->blow = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: blowm_dice of class  monster_ego */
static int toluaI_get_monster_monster_ego_blowm_dice(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->blowm_dice,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: blowm_dice of class  monster_ego */
static int toluaI_set_monster_monster_ego_blowm_dice(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->blowm_dice = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: blowm_side of class  monster_ego */
static int toluaI_get_monster_monster_ego_blowm_side(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->blowm_side,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: blowm_side of class  monster_ego */
static int toluaI_set_monster_monster_ego_blowm_side(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->blowm_side = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: hdice of class  monster_ego */
static int toluaI_get_monster_monster_ego_hdice(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->hdice);
 return 1;
}

/* set function: hdice of class  monster_ego */
static int toluaI_set_monster_monster_ego_hdice(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->hdice = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: hside of class  monster_ego */
static int toluaI_get_monster_monster_ego_hside(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->hside);
 return 1;
}

/* set function: hside of class  monster_ego */
static int toluaI_set_monster_monster_ego_hside(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->hside = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ac of class  monster_ego */
static int toluaI_get_monster_monster_ego_ac(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ac);
 return 1;
}

/* set function: ac of class  monster_ego */
static int toluaI_set_monster_monster_ego_ac(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ac = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sleep of class  monster_ego */
static int toluaI_get_monster_monster_ego_sleep(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sleep);
 return 1;
}

/* set function: sleep of class  monster_ego */
static int toluaI_set_monster_monster_ego_sleep(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sleep = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: aaf of class  monster_ego */
static int toluaI_get_monster_monster_ego_aaf(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->aaf);
 return 1;
}

/* set function: aaf of class  monster_ego */
static int toluaI_set_monster_monster_ego_aaf(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->aaf = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: speed of class  monster_ego */
static int toluaI_get_monster_monster_ego_speed(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->speed);
 return 1;
}

/* set function: speed of class  monster_ego */
static int toluaI_set_monster_monster_ego_speed(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->speed = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mexp of class  monster_ego */
static int toluaI_get_monster_monster_ego_mexp(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mexp);
 return 1;
}

/* set function: mexp of class  monster_ego */
static int toluaI_set_monster_monster_ego_mexp(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mexp = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: weight of class  monster_ego */
static int toluaI_get_monster_monster_ego_weight(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->weight);
 return 1;
}

/* set function: weight of class  monster_ego */
static int toluaI_set_monster_monster_ego_weight(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->weight = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: freq_inate of class  monster_ego */
static int toluaI_get_monster_monster_ego_freq_inate(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->freq_inate);
 return 1;
}

/* set function: freq_inate of class  monster_ego */
static int toluaI_set_monster_monster_ego_freq_inate(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->freq_inate = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: freq_spell of class  monster_ego */
static int toluaI_get_monster_monster_ego_freq_spell(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->freq_spell);
 return 1;
}

/* set function: freq_spell of class  monster_ego */
static int toluaI_set_monster_monster_ego_freq_spell(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->freq_spell = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  monster_ego */
static int toluaI_get_monster_monster_ego_flags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  monster_ego */
static int toluaI_set_monster_monster_ego_flags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: hflags of class  monster_ego */
static int toluaI_get_monster_monster_ego_hflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->hflags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: hflags of class  monster_ego */
static int toluaI_set_monster_monster_ego_hflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->hflags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: mflags of class  monster_ego */
static int toluaI_get_monster_monster_ego_mflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->mflags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: mflags of class  monster_ego */
static int toluaI_set_monster_monster_ego_mflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->mflags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: nflags of class  monster_ego */
static int toluaI_get_monster_monster_ego_nflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->nflags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: nflags of class  monster_ego */
static int toluaI_set_monster_monster_ego_nflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->nflags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: msflags of class  monster_ego */
static int toluaI_get_monster_monster_ego_msflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->msflags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: msflags of class  monster_ego */
static int toluaI_set_monster_monster_ego_msflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->msflags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: nsflags of class  monster_ego */
static int toluaI_get_monster_monster_ego_nsflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->nsflags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: nsflags of class  monster_ego */
static int toluaI_set_monster_monster_ego_nsflags(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->nsflags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: remove_all_spells of class  monster_ego */
static int toluaI_get_monster_monster_ego_remove_all_spells(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->remove_all_spells);
 return 1;
}

/* set function: remove_all_spells of class  monster_ego */
static int toluaI_set_monster_monster_ego_remove_all_spells(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->remove_all_spells = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: level of class  monster_ego */
static int toluaI_get_monster_monster_ego_level(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->level);
 return 1;
}

/* set function: level of class  monster_ego */
static int toluaI_set_monster_monster_ego_level(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->level = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: rarity of class  monster_ego */
static int toluaI_get_monster_monster_ego_rarity(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->rarity);
 return 1;
}

/* set function: rarity of class  monster_ego */
static int toluaI_set_monster_monster_ego_rarity(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->rarity = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_attr of class  monster_ego */
static int toluaI_get_monster_monster_ego_d_attr(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_attr);
 return 1;
}

/* set function: d_attr of class  monster_ego */
static int toluaI_set_monster_monster_ego_d_attr(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_char of class  monster_ego */
static int toluaI_get_monster_monster_ego_d_char(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_char);
 return 1;
}

/* set function: d_char of class  monster_ego */
static int toluaI_set_monster_monster_ego_d_char(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: g_attr of class  monster_ego */
static int toluaI_get_monster_monster_ego_g_attr(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->g_attr);
 return 1;
}

/* set function: g_attr of class  monster_ego */
static int toluaI_set_monster_monster_ego_g_attr(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->g_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: g_char of class  monster_ego */
static int toluaI_get_monster_monster_ego_g_char(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->g_char);
 return 1;
}

/* set function: g_char of class  monster_ego */
static int toluaI_set_monster_monster_ego_g_char(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->g_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: r_char of class  monster_ego */
static int toluaI_get_monster_monster_ego_r_char(lua_State* tolua_S)
{
 int toluaI_index;
  monster_ego* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (monster_ego*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=5)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->r_char[toluaI_index]);
 return 1;
}

/* set function: r_char of class  monster_ego */
static int toluaI_set_monster_monster_ego_r_char(lua_State* tolua_S)
{
 int toluaI_index;
  monster_ego* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (monster_ego*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=5)
 tolua_error(tolua_S,"array indexing out of range.");
  self->r_char[toluaI_index] = ((char)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: nr_char of class  monster_ego */
static int toluaI_get_monster_monster_ego_nr_char(lua_State* tolua_S)
{
 int toluaI_index;
  monster_ego* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (monster_ego*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=5)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->nr_char[toluaI_index]);
 return 1;
}

/* set function: nr_char of class  monster_ego */
static int toluaI_set_monster_monster_ego_nr_char(lua_State* tolua_S)
{
 int toluaI_index;
  monster_ego* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (monster_ego*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=5)
 tolua_error(tolua_S,"array indexing out of range.");
  self->nr_char[toluaI_index] = ((char)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: load_script of class  monster_ego */
static int toluaI_get_monster_monster_ego_load_script(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->load_script);
 return 1;
}

/* set function: load_script of class  monster_ego */
static int toluaI_set_monster_monster_ego_load_script(lua_State* tolua_S)
{
  monster_ego* self = (monster_ego*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->load_script = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: r_idx of class  monster_type */
static int toluaI_get_monster_monster_type_r_idx(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->r_idx);
 return 1;
}

/* set function: r_idx of class  monster_type */
static int toluaI_set_monster_monster_type_r_idx(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->r_idx = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_char of class  monster_type */
static int toluaI_get_monster_monster_type_d_char(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_char);
 return 1;
}

/* set function: d_char of class  monster_type */
static int toluaI_set_monster_monster_type_d_char(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sval of class  monster_type */
static int toluaI_get_monster_monster_type_sval(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sval);
 return 1;
}

/* set function: sval of class  monster_type */
static int toluaI_set_monster_monster_type_sval(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sval = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ego of class  monster_type */
static int toluaI_get_monster_monster_type_ego(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ego);
 return 1;
}

/* set function: ego of class  monster_type */
static int toluaI_set_monster_monster_type_ego(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ego = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: fy of class  monster_type */
static int toluaI_get_monster_monster_type_fy(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->fy);
 return 1;
}

/* set function: fy of class  monster_type */
static int toluaI_set_monster_monster_type_fy(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->fy = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: fx of class  monster_type */
static int toluaI_get_monster_monster_type_fx(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->fx);
 return 1;
}

/* set function: fx of class  monster_type */
static int toluaI_set_monster_monster_type_fx(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->fx = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: hp of class  monster_type */
static int toluaI_get_monster_monster_type_hp(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->hp);
 return 1;
}

/* set function: hp of class  monster_type */
static int toluaI_set_monster_monster_type_hp(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->hp = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: maxhp of class  monster_type */
static int toluaI_get_monster_monster_type_maxhp(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->maxhp);
 return 1;
}

/* set function: maxhp of class  monster_type */
static int toluaI_set_monster_monster_type_maxhp(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->maxhp = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: blow of class  monster_type */
static int toluaI_get_monster_monster_type_blow(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->blow,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: blow of class  monster_type */
static int toluaI_set_monster_monster_type_blow(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->blow = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: speed of class  monster_type */
static int toluaI_get_monster_monster_type_speed(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->speed);
 return 1;
}

/* set function: speed of class  monster_type */
static int toluaI_set_monster_monster_type_speed(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->speed = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: level of class  monster_type */
static int toluaI_get_monster_monster_type_level(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->level);
 return 1;
}

/* set function: level of class  monster_type */
static int toluaI_set_monster_monster_type_level(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->level = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ac of class  monster_type */
static int toluaI_get_monster_monster_type_ac(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ac);
 return 1;
}

/* set function: ac of class  monster_type */
static int toluaI_set_monster_monster_type_ac(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ac = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: exp of class  monster_type */
static int toluaI_get_monster_monster_type_exp(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->exp);
 return 1;
}

/* set function: exp of class  monster_type */
static int toluaI_set_monster_monster_type_exp(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->exp = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: csleep of class  monster_type */
static int toluaI_get_monster_monster_type_csleep(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->csleep);
 return 1;
}

/* set function: csleep of class  monster_type */
static int toluaI_set_monster_monster_type_csleep(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->csleep = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mspeed of class  monster_type */
static int toluaI_get_monster_monster_type_mspeed(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mspeed);
 return 1;
}

/* set function: mspeed of class  monster_type */
static int toluaI_set_monster_monster_type_mspeed(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mspeed = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: energy of class  monster_type */
static int toluaI_get_monster_monster_type_energy(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->energy);
 return 1;
}

/* set function: energy of class  monster_type */
static int toluaI_set_monster_monster_type_energy(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->energy = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: stunned of class  monster_type */
static int toluaI_get_monster_monster_type_stunned(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->stunned);
 return 1;
}

/* set function: stunned of class  monster_type */
static int toluaI_set_monster_monster_type_stunned(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->stunned = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: confused of class  monster_type */
static int toluaI_get_monster_monster_type_confused(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->confused);
 return 1;
}

/* set function: confused of class  monster_type */
static int toluaI_set_monster_monster_type_confused(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->confused = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: monfear of class  monster_type */
static int toluaI_get_monster_monster_type_monfear(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->monfear);
 return 1;
}

/* set function: monfear of class  monster_type */
static int toluaI_set_monster_monster_type_monfear(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->monfear = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: freq_inate of class  monster_type */
static int toluaI_get_monster_monster_type_freq_inate(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->freq_inate);
 return 1;
}

/* set function: freq_inate of class  monster_type */
static int toluaI_set_monster_monster_type_freq_inate(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->freq_inate = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: freq_spell of class  monster_type */
static int toluaI_get_monster_monster_type_freq_spell(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->freq_spell);
 return 1;
}

/* set function: freq_spell of class  monster_type */
static int toluaI_set_monster_monster_type_freq_spell(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->freq_spell = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  monster_type */
static int toluaI_get_monster_monster_type_flags(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  monster_type */
static int toluaI_set_monster_monster_type_flags(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: spells of class  monster_type */
static int toluaI_get_monster_monster_type_spells(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->spells,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: spells of class  monster_type */
static int toluaI_set_monster_monster_type_spells(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->spells = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: bleeding of class  monster_type */
static int toluaI_get_monster_monster_type_bleeding(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->bleeding);
 return 1;
}

/* set function: bleeding of class  monster_type */
static int toluaI_set_monster_monster_type_bleeding(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->bleeding = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: poisoned of class  monster_type */
static int toluaI_get_monster_monster_type_poisoned(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->poisoned);
 return 1;
}

/* set function: poisoned of class  monster_type */
static int toluaI_set_monster_monster_type_poisoned(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->poisoned = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: cdis of class  monster_type */
static int toluaI_get_monster_monster_type_cdis(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->cdis);
 return 1;
}

/* set function: cdis of class  monster_type */
static int toluaI_set_monster_monster_type_cdis(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->cdis = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mflag of class  monster_type */
static int toluaI_get_monster_monster_type_mflag(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mflag);
 return 1;
}

/* set function: mflag of class  monster_type */
static int toluaI_set_monster_monster_type_mflag(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mflag = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ml of class  monster_type */
static int toluaI_get_monster_monster_type_ml(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->ml);
 return 1;
}

/* set function: ml of class  monster_type */
static int toluaI_set_monster_monster_type_ml(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->ml = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: inventory of class  monster_type */
static int toluaI_get_monster_monster_type_inventory(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->inventory,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: inventory of class  monster_type */
static int toluaI_set_monster_monster_type_inventory(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->inventory = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: ai of class  monster_type */
static int toluaI_get_monster_monster_type_ai(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ai);
 return 1;
}

/* set function: ai of class  monster_type */
static int toluaI_set_monster_monster_type_ai(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ai = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ai_states of class  monster_type */
static int toluaI_get_monster_monster_type_ai_states(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->ai_states,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: ai_states of class  monster_type */
static int toluaI_set_monster_monster_type_ai_states(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->ai_states = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: ai_speed of class  monster_type */
static int toluaI_get_monster_monster_type_ai_speed(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ai_speed);
 return 1;
}

/* set function: ai_speed of class  monster_type */
static int toluaI_set_monster_monster_type_ai_speed(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ai_speed = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ai_action of class  monster_type */
static int toluaI_get_monster_monster_type_ai_action(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ai_action);
 return 1;
}

/* set function: ai_action of class  monster_type */
static int toluaI_set_monster_monster_type_ai_action(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ai_action = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ai_move_y of class  monster_type */
static int toluaI_get_monster_monster_type_ai_move_y(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ai_move_y);
 return 1;
}

/* set function: ai_move_y of class  monster_type */
static int toluaI_set_monster_monster_type_ai_move_y(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ai_move_y = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ai_move_x of class  monster_type */
static int toluaI_get_monster_monster_type_ai_move_x(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ai_move_x);
 return 1;
}

/* set function: ai_move_x of class  monster_type */
static int toluaI_set_monster_monster_type_ai_move_x(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ai_move_x = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: smart of class  monster_type */
static int toluaI_get_monster_monster_type_smart(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->smart);
 return 1;
}

/* set function: smart of class  monster_type */
static int toluaI_set_monster_monster_type_smart(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->smart = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: faction of class  monster_type */
static int toluaI_get_monster_monster_type_faction(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->faction);
 return 1;
}

/* set function: faction of class  monster_type */
static int toluaI_set_monster_monster_type_faction(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->faction = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: target of class  monster_type */
static int toluaI_get_monster_monster_type_target(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->target);
 return 1;
}

/* set function: target of class  monster_type */
static int toluaI_set_monster_monster_type_target(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->target = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: possessor of class  monster_type */
static int toluaI_get_monster_monster_type_possessor(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->possessor);
 return 1;
}

/* set function: possessor of class  monster_type */
static int toluaI_set_monster_monster_type_possessor(lua_State* tolua_S)
{
  monster_type* self = (monster_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->possessor = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* function: lua_get_monster */
static int toluaI_monster_monster00(lua_State* tolua_S)
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
  monster_type* toluaI_ret = (monster_type*)  lua_get_monster(m_idx);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"monster_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster'.");
 return 0;
}

/* get function: monst_list */
static int toluaI_get_monster_monst_list(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&monst_list,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: monst_list */
static int toluaI_set_monster_monst_list(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  monst_list = *((flags_type*)  tolua_getusertype(tolua_S,1,0));
 return 0;
}

/* function: race_info_idx */
static int toluaI_monster_race_info_idx00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b r_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b ego = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  monster_race* toluaI_ret = (monster_race*)  race_info_idx(r_idx,ego);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"monster_race"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'race_info_idx'.");
 return 0;
}

/* function: delete_monster_idx */
static int toluaI_monster_delete_monster_idx00(lua_State* tolua_S)
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
  delete_monster_idx(i);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete_monster_idx'.");
 return 0;
}

/* function: m_pop */
static int toluaI_monster_m_pop00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  s16b toluaI_ret = (s16b)  m_pop();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'm_pop'.");
 return 0;
}

/* function: set_mon_num_hook */
static int toluaI_monster_set_mon_num_hook00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  set_mon_num_hook();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_mon_num_hook'.");
 return 0;
}

/* function: get_mon_num_prep */
static int toluaI_monster_get_mon_num_prep00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  errr toluaI_ret = (errr)  get_mon_num_prep();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_mon_num_prep'.");
 return 0;
}

/* function: get_mon_num */
static int toluaI_monster_get_mon_num00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b level = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  s16b toluaI_ret = (s16b)  get_mon_num(level);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_mon_num'.");
 return 0;
}

/* function: lua_monster_desc */
static int toluaI_monster_monster_desc00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  s32b mode = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  char* toluaI_ret = (char*)  lua_monster_desc(m_ptr,mode);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_desc'.");
 return 0;
}

/* function: lua_monster_race_desc */
static int toluaI_monster_monster_race_desc00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b r_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b ego = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  char* toluaI_ret = (char*)  lua_monster_race_desc(r_idx,ego);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_race_desc'.");
 return 0;
}

/* function: monster_race_desc */
static int toluaI_monster_monster_race_desc01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  char* desc = ((char*)  tolua_getstring(tolua_S,1,0));
  s32b r_idx = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b ego = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  monster_race_desc(desc,r_idx,ego);
 }
 }
 return 0;
tolua_lerror:
 return toluaI_monster_monster_race_desc00(tolua_S);
}

/* function: monster_carry */
static int toluaI_monster_monster_carry00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  s32b m_idx = ((s32b)  tolua_getnumber(tolua_S,2,0));
  object_type* q_ptr = ((object_type*)  tolua_getusertype(tolua_S,3,0));
 {
  monster_carry(m_ptr,m_idx,q_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_carry'.");
 return 0;
}

/* get function: monster_level */
static int toluaI_get_monster_monster_level(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)monster_level);
 return 1;
}

/* set function: monster_level */
static int toluaI_set_monster_monster_level(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  monster_level = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: place_monster_aux */
static int toluaI_monster_place_monster_aux00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b r_idx = ((s32b)  tolua_getnumber(tolua_S,3,0));
  bool slp = ((bool)  tolua_getbool(tolua_S,4,0));
  bool grp = ((bool)  tolua_getbool(tolua_S,5,0));
  s32b status = ((s32b)  tolua_getnumber(tolua_S,6,0));
 {
  bool toluaI_ret = (bool)  place_monster_aux(y,x,r_idx,slp,grp,status);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_monster_aux'.");
 return 0;
}

/* function: place_monster_aux_ego */
static int toluaI_monster_place_monster_aux_ego00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,6,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,8)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b r_idx = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b ego = ((s32b)  tolua_getnumber(tolua_S,4,0));
  bool slp = ((bool)  tolua_getbool(tolua_S,5,0));
  bool grp = ((bool)  tolua_getbool(tolua_S,6,0));
  s32b status = ((s32b)  tolua_getnumber(tolua_S,7,0));
 {
  s16b toluaI_ret = (s16b)  place_monster_aux_ego(y,x,r_idx,ego,slp,grp,status);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_monster_aux_ego'.");
 return 0;
}

/* function: place_monster */
static int toluaI_monster_place_monster00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  bool slp = ((bool)  tolua_getbool(tolua_S,3,0));
  bool grp = ((bool)  tolua_getbool(tolua_S,4,0));
 {
  bool toluaI_ret = (bool)  place_monster(y,x,slp,grp);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_monster'.");
 return 0;
}

/* function: place_monster_type */
static int toluaI_monster_place_monster_type00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,3,0));
 {
  s16b toluaI_ret = (s16b)  place_monster_type(y,x,m_ptr);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_monster_type'.");
 return 0;
}

/* function: monster_prep */
static int toluaI_monster_monster_prep00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  s32b r_idx = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b ego = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  monster_prep(m_ptr,r_idx,ego);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_prep'.");
 return 0;
}

/* get function: placed_monsters */
static int toluaI_get_monster_placed_monsters(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=PLACED_MONSTERS_MAX)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)placed_monsters[toluaI_index],tolua_tag(tolua_S,"monster_type"));
 return 1;
}

/* set function: placed_monsters */
static int toluaI_set_monster_placed_monsters(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=PLACED_MONSTERS_MAX)
 tolua_error(tolua_S,"array indexing out of range.");
  placed_monsters[toluaI_index] = ((monster_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: placed_monster_ids */
static int toluaI_get_monster_placed_monster_ids(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=PLACED_MONSTERS_MAX)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)placed_monster_ids[toluaI_index]);
 return 1;
}

/* set function: placed_monster_ids */
static int toluaI_set_monster_placed_monster_ids(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=PLACED_MONSTERS_MAX)
 tolua_error(tolua_S,"array indexing out of range.");
  placed_monster_ids[toluaI_index] = ((s32b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: placed_monsters_num */
static int toluaI_get_monster_placed_monsters_num(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)placed_monsters_num);
 return 1;
}

/* set function: placed_monsters_num */
static int toluaI_set_monster_placed_monsters_num(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  placed_monsters_num = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: placed_monster_id */
static int toluaI_get_monster_placed_monster_id(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)placed_monster_id);
 return 1;
}

/* set function: placed_monster_id */
static int toluaI_set_monster_placed_monster_id(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  placed_monster_id = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: place_monster_one */
static int toluaI_monster_place_monster_one00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b r_idx = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b ego = ((s32b)  tolua_getnumber(tolua_S,4,0));
  bool slp = ((bool)  tolua_getbool(tolua_S,5,0));
  s32b status = ((s32b)  tolua_getnumber(tolua_S,6,0));
 {
  s16b toluaI_ret = (s16b)  place_monster_one(y,x,r_idx,ego,slp,status);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_monster_one'.");
 return 0;
}

/* get function: place_monster_one_no_drop */
static int toluaI_get_monster_place_monster_one_no_drop(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)place_monster_one_no_drop);
 return 1;
}

/* set function: place_monster_one_no_drop */
static int toluaI_set_monster_place_monster_one_no_drop(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  place_monster_one_no_drop = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: alloc_monster */
static int toluaI_monster_alloc_monster00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b dis = ((s32b)  tolua_getnumber(tolua_S,1,0));
  bool slp = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  alloc_monster(dis,slp);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'alloc_monster'.");
 return 0;
}

/* function: is_friend */
static int toluaI_monster_is_friend00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  is_friend(m_ptr);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_friend'.");
 return 0;
}

/* function: is_enemy */
static int toluaI_monster_is_enemy00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  monster_type* t_ptr = ((monster_type*)  tolua_getusertype(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  is_enemy(m_ptr,t_ptr);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_enemy'.");
 return 0;
}

/* function: find_position */
static int toluaI_monster_find_position00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b yy = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b xx = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  find_position(y,x,&yy,&xx);
 tolua_pushnumber(tolua_S,(long)yy);
 tolua_pushnumber(tolua_S,(long)xx);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_position'.");
 return 0;
}

/* function: get_rand_r_idx */
static int toluaI_monster_get_rand_r_idx00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,6,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,7,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,8,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,9,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,10,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,11)
 )
 goto tolua_lerror;
 else
 {
  s32b lev = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b faction = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b min_friend = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b max_friend = ((s32b)  tolua_getnumber(tolua_S,4,0));
  flags_type* flags = ((flags_type*)  tolua_getusertype(tolua_S,5,0));
  flags_type* flags_forbid = ((flags_type*)  tolua_getusertype(tolua_S,6,0));
  cptr chars = ((cptr)  tolua_getstring(tolua_S,7,0));
  cptr chars_forbid = ((cptr)  tolua_getstring(tolua_S,8,0));
  cptr name = ((cptr)  tolua_getstring(tolua_S,9,0));
  cptr name_forbid = ((cptr)  tolua_getstring(tolua_S,10,0));
 {
  s32b toluaI_ret = (s32b)  get_rand_r_idx(lev,faction,min_friend,max_friend,flags,flags_forbid,chars,chars_forbid,name,name_forbid);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_rand_r_idx'.");
 return 0;
}

/* get function: summon_specific_level */
static int toluaI_get_monster_summon_specific_level(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)summon_specific_level);
 return 1;
}

/* set function: summon_specific_level */
static int toluaI_set_monster_summon_specific_level(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  summon_specific_level = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: summon_specific */
static int toluaI_monster_summon_specific00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,7,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,8,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,9,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,10,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,11,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,12,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,13,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,14)
 )
 goto tolua_lerror;
 else
 {
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b lev = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b faction = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b min_friend = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b max_friend = ((s32b)  tolua_getnumber(tolua_S,6,0));
  bool group_ok = ((bool)  tolua_getbool(tolua_S,7,0));
  flags_type* flags = ((flags_type*)  tolua_getusertype(tolua_S,8,0));
  flags_type* flags_forbid = ((flags_type*)  tolua_getusertype(tolua_S,9,0));
  cptr chars = ((cptr)  tolua_getstring(tolua_S,10,0));
  cptr chars_forbid = ((cptr)  tolua_getstring(tolua_S,11,0));
  cptr name = ((cptr)  tolua_getstring(tolua_S,12,0));
  cptr name_forbide = ((cptr)  tolua_getstring(tolua_S,13,0));
 {
  bool toluaI_ret = (bool)  summon_specific(y1,x1,lev,faction,min_friend,max_friend,group_ok,flags,flags_forbid,chars,chars_forbid,name,name_forbide);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'summon_specific'.");
 return 0;
}

/* function: lua_summon_monster */
static int toluaI_monster_summon_monster_aux00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,6,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b lev = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b faction = ((s32b)  tolua_getnumber(tolua_S,4,0));
  bool group_ok = ((bool)  tolua_getbool(tolua_S,5,0));
  char* fct = ((char*)  tolua_getstring(tolua_S,6,0));
 {
  bool toluaI_ret = (bool)  lua_summon_monster(y,x,lev,faction,group_ok,fct);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'summon_monster_aux'.");
 return 0;
}

/* function: can_create_companion */
static int toluaI_monster_can_create_companion00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  can_create_companion();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'can_create_companion'.");
 return 0;
}

/* function: monster_set_level */
static int toluaI_monster_monster_set_level00(lua_State* tolua_S)
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
  s32b level = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  monster_set_level(m_idx,level);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_set_level'.");
 return 0;
}

/* function: can_pass_aux */
static int toluaI_monster_can_pass_aux00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest_flags = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* passer_flags = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
  u32b how = ((u32b)  tolua_getnumber(tolua_S,3,NULL));
 {
  bool toluaI_ret = (bool)  can_pass_aux(dest_flags,passer_flags,&how);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)how);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'can_pass_aux'.");
 return 0;
}

/* function: monst_can_pass_square */
static int toluaI_monster_monst_can_pass_square00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,3,0));
  u32b how = ((u32b)  tolua_getnumber(tolua_S,4,NULL));
 {
  bool toluaI_ret = (bool)  monst_can_pass_square(m_ptr,y,x,&how);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)how);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monst_can_pass_square'.");
 return 0;
}

/* function: do_control_reconnect */
static int toluaI_monster_do_control_reconnect00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  do_control_reconnect();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_control_reconnect'.");
 return 0;
}

/* get function: max_m_idx */
static int toluaI_get_monster_max_monsters(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_m_idx);
 return 1;
}

/* set function: max_m_idx */
static int toluaI_set_monster_max_monsters(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_m_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_r_idx */
static int toluaI_get_monster_max_r_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_r_idx);
 return 1;
}

/* set function: max_r_idx */
static int toluaI_set_monster_max_r_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_r_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_re_idx */
static int toluaI_get_monster_max_re_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_re_idx);
 return 1;
}

/* set function: max_re_idx */
static int toluaI_set_monster_max_re_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_re_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: r_info */
static int toluaI_get_monster_r_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_r_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&r_info[toluaI_index],tolua_tag(tolua_S,"monster_race"));
 return 1;
}

/* set function: r_info */
static int toluaI_set_monster_r_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_r_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  r_info[toluaI_index] = *((monster_race*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: re_info */
static int toluaI_get_monster_re_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_re_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&re_info[toluaI_index],tolua_tag(tolua_S,"monster_ego"));
 return 1;
}

/* set function: re_info */
static int toluaI_set_monster_re_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_re_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  re_info[toluaI_index] = *((monster_ego*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* function: mon_take_hit */
static int toluaI_monster_mon_take_hit00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_istype(tolua_S,4,LUA_TSTRING,1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b m_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,2,0));
  bool fear = ((bool)  tolua_getbool(tolua_S,3,0));
  cptr note = ((cptr)  tolua_getstring(tolua_S,4,NULL));
 {
  bool toluaI_ret = (bool)  mon_take_hit(m_idx,dam,&fear,note);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushbool(tolua_S,(int)fear);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'mon_take_hit'.");
 return 0;
}

/* get function: health_who */
static int toluaI_get_monster_health_who(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)health_who);
 return 1;
}

/* set function: health_who */
static int toluaI_set_monster_health_who(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  health_who = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: health_track */
static int toluaI_monster_health_track00(lua_State* tolua_S)
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
  health_track(m_idx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'health_track'.");
 return 0;
}

/* function: monster_race_track */
static int toluaI_monster_monster_race_track00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b r_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b ego = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  monster_race_track(r_idx,ego);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_race_track'.");
 return 0;
}

/* function: multiply_monster */
static int toluaI_monster_multiply_monster00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  s32b m_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  bool charm = ((bool)  tolua_getbool(tolua_S,2,0));
  bool clone = ((bool)  tolua_getbool(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  multiply_monster(m_idx,charm,clone);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'multiply_monster'.");
 return 0;
}

/* function: ai_multiply */
static int toluaI_monster_ai_multiply00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  ai_multiply(m_idx);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ai_multiply'.");
 return 0;
}

/* function: do_poly_monster */
static int toluaI_monster_do_poly_monster00(lua_State* tolua_S)
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
  s16b toluaI_ret = (s16b)  do_poly_monster(y,x);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_poly_monster'.");
 return 0;
}

/* function: message_pain */
static int toluaI_monster_message_pain00(lua_State* tolua_S)
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
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  message_pain(m_idx,dam);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'message_pain'.");
 return 0;
}

/* function: lookup_race */
static int toluaI_monster_lookup_race00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  char d_char = ((char)  tolua_getnumber(tolua_S,1,0));
  s16b sval = ((s16b)  tolua_getnumber(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  lookup_race(d_char,sval);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lookup_race'.");
 return 0;
}

/* get function: faction_names */
static int toluaI_get_monster_factions_faction_names(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&faction_names,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: faction_names */
static int toluaI_set_monster_factions_faction_names(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  faction_names = *((flags_type*)  tolua_getusertype(tolua_S,1,0));
 return 0;
}

/* function: set_faction_friendliness */
static int toluaI_monster_factions_set_friendliness00(lua_State* tolua_S)
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
  u32b faction = ((u32b)  tolua_getnumber(tolua_S,1,0));
  u32b friend_faction = ((u32b)  tolua_getnumber(tolua_S,2,0));
  s32b percent = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  set_faction_friendliness(faction,friend_faction,percent);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_friendliness'.");
 return 0;
}

/* function: get_faction_friendliness */
static int toluaI_monster_factions_get_friendliness00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  u32b faction = ((u32b)  tolua_getnumber(tolua_S,1,0));
  u32b friend_faction = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  get_faction_friendliness(faction,friend_faction);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_friendliness'.");
 return 0;
}

/* function: change_monst_faction */
static int toluaI_monster_factions_change_faction00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  u32b new_faction = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  change_monst_faction(m_ptr,new_faction);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'change_faction'.");
 return 0;
}

/* function: get_hostile_faction */
static int toluaI_monster_factions_get_hostile_faction00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  u32b hostile_to = ((u32b)  tolua_getnumber(tolua_S,2,FACTION_PLAYER));
  u32b default_faction = ((u32b)  tolua_getnumber(tolua_S,3,FACTION_DUNGEON));
 {
  u32b toluaI_ret = (u32b)  get_hostile_faction(m_ptr,hostile_to,default_faction);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_hostile_faction'.");
 return 0;
}

/* function: monster_blow_new */
static int toluaI_monster_monster_blow_new00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  monster_blow* toluaI_ret = (monster_blow*)  monster_blow_new();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"monster_blow"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_blow_new'.");
 return 0;
}

/* function: monster_blow_del */
static int toluaI_monster_monster_blow_del00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_blow"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  monster_blow* blow = ((monster_blow*)  tolua_getusertype(tolua_S,1,0));
 {
  monster_blow_del(blow);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_blow_del'.");
 return 0;
}

/* function: monster_blow_dup */
static int toluaI_monster_monster_blow_dup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_blow"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  monster_blow* blow = ((monster_blow*)  tolua_getusertype(tolua_S,1,0));
 {
  monster_blow* toluaI_ret = (monster_blow*)  monster_blow_dup(blow);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"monster_blow"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_blow_dup'.");
 return 0;
}

/* function: monster_type_new */
static int toluaI_monster_monster_type_new00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  monster_type* toluaI_ret = (monster_type*)  monster_type_new();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"monster_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_type_new'.");
 return 0;
}

/* function: delete_monster_type */
static int toluaI_monster_delete_monster_type00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  bool del = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  delete_monster_type(m_ptr,del);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete_monster_type'.");
 return 0;
}

/* function: monster_type_free_memory */
static int toluaI_monster_monster_type_free_memory00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
 {
  monster_type_free_memory(m_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_type_free_memory'.");
 return 0;
}

/* function: monster_type_dup */
static int toluaI_monster_monster_type_dup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
 {
  monster_type* toluaI_ret = (monster_type*)  monster_type_dup(m_ptr);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"monster_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_type_dup'.");
 return 0;
}

/* function: monster_type_copy */
static int toluaI_monster_monster_type_copy00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  monster_type* dest = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  monster_type* src = ((monster_type*)  tolua_getusertype(tolua_S,2,0));
 {
  monster_type_copy(dest,src);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'monster_type_copy'.");
 return 0;
}

/* function: mon_take_hit_mon */
static int toluaI_monster_mon_take_hit_mon00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,5,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b s_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b m_idx = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dam = ((s32b)  tolua_getnumber(tolua_S,3,0));
  bool fear = ((bool)  tolua_getbool(tolua_S,4,0));
  cptr note = ((cptr)  tolua_getstring(tolua_S,5,0));
 {
  bool toluaI_ret = (bool)  mon_take_hit_mon(s_idx,m_idx,dam,&fear,note);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushbool(tolua_S,(int)fear);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'mon_take_hit_mon'.");
 return 0;
}

/* function: set_monster_ai */
static int toluaI_monster_set_monster_ai00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  u32b ai = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  set_monster_ai(m_ptr,ai);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_monster_ai'.");
 return 0;
}

/* function: find_safety */
static int toluaI_monster_find_safety00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  s32b yp = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b xp = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  find_safety(m_ptr,&yp,&xp);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)yp);
 tolua_pushnumber(tolua_S,(long)xp);
 }
 }
 return 3;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_safety'.");
 return 0;
}

/* function: find_hiding */
static int toluaI_monster_find_hiding00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  s32b yp = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b xp = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  find_hiding(m_ptr,&yp,&xp);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)yp);
 tolua_pushnumber(tolua_S,(long)xp);
 }
 }
 return 3;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_hiding'.");
 return 0;
}

/* function: get_monster_energy */
static int toluaI_monster_get_monster_energy00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  s32b speed_id = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  u16b toluaI_ret = (u16b)  get_monster_energy(m_ptr,speed_id);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_monster_energy'.");
 return 0;
}

/* function: is_obj_mimic_at */
static int toluaI_monster_is_obj_mimic_at00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  is_obj_mimic_at(y,x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_obj_mimic_at'.");
 return 0;
}

/* function: get_obj_mimic_obj_at */
static int toluaI_monster_get_obj_mimic_obj_at00(lua_State* tolua_S)
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
  object_type* toluaI_ret = (object_type*)  get_obj_mimic_obj_at(y,x);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"object_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_obj_mimic_obj_at'.");
 return 0;
}

/* function: is_feat_mimic_at */
static int toluaI_monster_is_feat_mimic_at00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  is_feat_mimic_at(y,x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_feat_mimic_at'.");
 return 0;
}

/* function: get_feat_mimic_feat_at */
static int toluaI_monster_get_feat_mimic_feat_at00(lua_State* tolua_S)
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
  s16b toluaI_ret = (s16b)  get_feat_mimic_feat_at(y,x);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_feat_mimic_feat_at'.");
 return 0;
}

/* function: player_monster_swap */
static int toluaI_monster_player_monster_swap00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  player_monster_swap(m_ptr);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'player_monster_swap'.");
 return 0;
}

/* function: talk_to_monster */
static int toluaI_monster_talk_to_monster00(lua_State* tolua_S)
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
  talk_to_monster(m_idx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'talk_to_monster'.");
 return 0;
}

/* get function: repair_monsters */
static int toluaI_get_monster_repair_monsters(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)repair_monsters);
 return 1;
}

/* set function: repair_monsters */
static int toluaI_set_monster_repair_monsters(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  repair_monsters = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: wipe_m_list */
static int toluaI_monster_wipe_m_list00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  wipe_m_list();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wipe_m_list'.");
 return 0;
}

/* Open function */
int tolua_monster_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_globalvar(tolua_S,"monster_forge",toluaI_get_monster_monster_forge,toluaI_set_monster_monster_forge);
 tolua_constant(tolua_S,NULL,"FACTION_PLAYER",FACTION_PLAYER);
 tolua_constant(tolua_S,NULL,"FACTION_TOWN",FACTION_TOWN);
 tolua_constant(tolua_S,NULL,"FACTION_DUNGEON",FACTION_DUNGEON);
 tolua_constant(tolua_S,NULL,"MFLAG_VIEW",MFLAG_VIEW);
 tolua_constant(tolua_S,NULL,"MFLAG_QUEST",MFLAG_QUEST);
 tolua_constant(tolua_S,NULL,"MFLAG_PARTIAL",MFLAG_PARTIAL);
 tolua_constant(tolua_S,NULL,"MFLAG_CONTROL",MFLAG_CONTROL);
 tolua_constant(tolua_S,NULL,"MFLAG_BORN",MFLAG_BORN);
 tolua_constant(tolua_S,NULL,"MFLAG_NICE",MFLAG_NICE);
 tolua_constant(tolua_S,NULL,"MFLAG_SHOW",MFLAG_SHOW);
 tolua_constant(tolua_S,NULL,"MFLAG_MARK",MFLAG_MARK);
 tolua_constant(tolua_S,NULL,"MFLAG_NO_DROP",MFLAG_NO_DROP);
 tolua_constant(tolua_S,NULL,"MFLAG_QUEST2",MFLAG_QUEST2);
 tolua_cclass(tolua_S,"monster_blow","");
 tolua_tablevar(tolua_S,"monster_blow","method",toluaI_get_monster_monster_blow_method,toluaI_set_monster_monster_blow_method);
 tolua_tablevar(tolua_S,"monster_blow","effect",toluaI_get_monster_monster_blow_effect,toluaI_set_monster_monster_blow_effect);
 tolua_tablevar(tolua_S,"monster_blow","d_dice",toluaI_get_monster_monster_blow_d_dice,toluaI_set_monster_monster_blow_d_dice);
 tolua_tablevar(tolua_S,"monster_blow","d_side",toluaI_get_monster_monster_blow_d_side,toluaI_set_monster_monster_blow_d_side);
 tolua_cclass(tolua_S,"monster_race","");
 tolua_tablevar(tolua_S,"monster_race","name",toluaI_get_monster_monster_race_name,toluaI_set_monster_monster_race_name);
 tolua_tablevar(tolua_S,"monster_race","text",toluaI_get_monster_monster_race_text,toluaI_set_monster_monster_race_text);
 tolua_tablevar(tolua_S,"monster_race","hdice",toluaI_get_monster_monster_race_hdice,toluaI_set_monster_monster_race_hdice);
 tolua_tablevar(tolua_S,"monster_race","hside",toluaI_get_monster_monster_race_hside,toluaI_set_monster_monster_race_hside);
 tolua_tablevar(tolua_S,"monster_race","ac",toluaI_get_monster_monster_race_ac,toluaI_set_monster_monster_race_ac);
 tolua_tablevar(tolua_S,"monster_race","sleep",toluaI_get_monster_monster_race_sleep,toluaI_set_monster_monster_race_sleep);
 tolua_tablevar(tolua_S,"monster_race","aaf",toluaI_get_monster_monster_race_aaf,toluaI_set_monster_monster_race_aaf);
 tolua_tablevar(tolua_S,"monster_race","speed",toluaI_get_monster_monster_race_speed,toluaI_set_monster_monster_race_speed);
 tolua_tablevar(tolua_S,"monster_race","mexp",toluaI_get_monster_monster_race_mexp,toluaI_set_monster_monster_race_mexp);
 tolua_tablevar(tolua_S,"monster_race","weight",toluaI_get_monster_monster_race_weight,toluaI_set_monster_monster_race_weight);
 tolua_tablevar(tolua_S,"monster_race","freq_inate",toluaI_get_monster_monster_race_freq_inate,toluaI_set_monster_monster_race_freq_inate);
 tolua_tablevar(tolua_S,"monster_race","freq_spell",toluaI_get_monster_monster_race_freq_spell,toluaI_set_monster_monster_race_freq_spell);
 tolua_tablevar(tolua_S,"monster_race","flags",toluaI_get_monster_monster_race_flags,toluaI_set_monster_monster_race_flags);
 tolua_tablevar(tolua_S,"monster_race","spells",toluaI_get_monster_monster_race_spells,toluaI_set_monster_monster_race_spells);
 tolua_tablevar(tolua_S,"monster_race","blow",toluaI_get_monster_monster_race_blow,toluaI_set_monster_monster_race_blow);
 tolua_tablevar(tolua_S,"monster_race","body_parts",toluaI_get_monster_monster_race_body_parts,toluaI_set_monster_monster_race_body_parts);
 tolua_tablevar(tolua_S,"monster_race","level",toluaI_get_monster_monster_race_level,toluaI_set_monster_monster_race_level);
 tolua_tablevar(tolua_S,"monster_race","rarity",toluaI_get_monster_monster_race_rarity,toluaI_set_monster_monster_race_rarity);
 tolua_tablevar(tolua_S,"monster_race","d_attr",toluaI_get_monster_monster_race_d_attr,toluaI_set_monster_monster_race_d_attr);
 tolua_tablevar(tolua_S,"monster_race","d_char",toluaI_get_monster_monster_race_d_char,toluaI_set_monster_monster_race_d_char);
 tolua_tablevar(tolua_S,"monster_race","sval",toluaI_get_monster_monster_race_sval,toluaI_set_monster_monster_race_sval);
 tolua_tablevar(tolua_S,"monster_race","x_attr",toluaI_get_monster_monster_race_x_attr,toluaI_set_monster_monster_race_x_attr);
 tolua_tablevar(tolua_S,"monster_race","x_char",toluaI_get_monster_monster_race_x_char,toluaI_set_monster_monster_race_x_char);
 tolua_tablevar(tolua_S,"monster_race","max_num",toluaI_get_monster_monster_race_max_num,toluaI_set_monster_monster_race_max_num);
 tolua_tablevar(tolua_S,"monster_race","cur_num",toluaI_get_monster_monster_race_cur_num,toluaI_set_monster_monster_race_cur_num);
 tolua_tablevar(tolua_S,"monster_race","r_sights",toluaI_get_monster_monster_race_r_sights,toluaI_set_monster_monster_race_r_sights);
 tolua_tablevar(tolua_S,"monster_race","r_deaths",toluaI_get_monster_monster_race_r_deaths,toluaI_set_monster_monster_race_r_deaths);
 tolua_tablevar(tolua_S,"monster_race","r_pkills",toluaI_get_monster_monster_race_r_pkills,toluaI_set_monster_monster_race_r_pkills);
 tolua_tablevar(tolua_S,"monster_race","r_tkills",toluaI_get_monster_monster_race_r_tkills,toluaI_set_monster_monster_race_r_tkills);
 tolua_tablevar(tolua_S,"monster_race","r_wake",toluaI_get_monster_monster_race_r_wake,toluaI_set_monster_monster_race_r_wake);
 tolua_tablevar(tolua_S,"monster_race","r_ignore",toluaI_get_monster_monster_race_r_ignore,toluaI_set_monster_monster_race_r_ignore);
 tolua_tablevar(tolua_S,"monster_race","r_xtra1",toluaI_get_monster_monster_race_r_xtra1,toluaI_set_monster_monster_race_r_xtra1);
 tolua_tablevar(tolua_S,"monster_race","r_xtra2",toluaI_get_monster_monster_race_r_xtra2,toluaI_set_monster_monster_race_r_xtra2);
 tolua_tablevar(tolua_S,"monster_race","r_drop_gold",toluaI_get_monster_monster_race_r_drop_gold,toluaI_set_monster_monster_race_r_drop_gold);
 tolua_tablevar(tolua_S,"monster_race","r_drop_item",toluaI_get_monster_monster_race_r_drop_item,toluaI_set_monster_monster_race_r_drop_item);
 tolua_tablevar(tolua_S,"monster_race","r_cast_inate",toluaI_get_monster_monster_race_r_cast_inate,toluaI_set_monster_monster_race_r_cast_inate);
 tolua_tablevar(tolua_S,"monster_race","r_cast_spell",toluaI_get_monster_monster_race_r_cast_spell,toluaI_set_monster_monster_race_r_cast_spell);
 tolua_tablearray(tolua_S,"monster_race","r_blows",toluaI_get_monster_monster_race_r_blows,toluaI_set_monster_monster_race_r_blows);
 tolua_tablevar(tolua_S,"monster_race","r_flags",toluaI_get_monster_monster_race_r_flags,toluaI_set_monster_monster_race_r_flags);
 tolua_tablevar(tolua_S,"monster_race","total_visible",toluaI_get_monster_monster_race_total_visible,toluaI_set_monster_monster_race_total_visible);
 tolua_cclass(tolua_S,"monster_ego","");
 tolua_tablevar(tolua_S,"monster_ego","name",toluaI_get_monster_monster_ego_name,toluaI_set_monster_monster_ego_name);
 tolua_tablevar(tolua_S,"monster_ego","before",toluaI_get_monster_monster_ego_before,toluaI_set_monster_monster_ego_before);
 tolua_tablevar(tolua_S,"monster_ego","blow",toluaI_get_monster_monster_ego_blow,toluaI_set_monster_monster_ego_blow);
 tolua_tablevar(tolua_S,"monster_ego","blowm_dice",toluaI_get_monster_monster_ego_blowm_dice,toluaI_set_monster_monster_ego_blowm_dice);
 tolua_tablevar(tolua_S,"monster_ego","blowm_side",toluaI_get_monster_monster_ego_blowm_side,toluaI_set_monster_monster_ego_blowm_side);
 tolua_tablevar(tolua_S,"monster_ego","hdice",toluaI_get_monster_monster_ego_hdice,toluaI_set_monster_monster_ego_hdice);
 tolua_tablevar(tolua_S,"monster_ego","hside",toluaI_get_monster_monster_ego_hside,toluaI_set_monster_monster_ego_hside);
 tolua_tablevar(tolua_S,"monster_ego","ac",toluaI_get_monster_monster_ego_ac,toluaI_set_monster_monster_ego_ac);
 tolua_tablevar(tolua_S,"monster_ego","sleep",toluaI_get_monster_monster_ego_sleep,toluaI_set_monster_monster_ego_sleep);
 tolua_tablevar(tolua_S,"monster_ego","aaf",toluaI_get_monster_monster_ego_aaf,toluaI_set_monster_monster_ego_aaf);
 tolua_tablevar(tolua_S,"monster_ego","speed",toluaI_get_monster_monster_ego_speed,toluaI_set_monster_monster_ego_speed);
 tolua_tablevar(tolua_S,"monster_ego","mexp",toluaI_get_monster_monster_ego_mexp,toluaI_set_monster_monster_ego_mexp);
 tolua_tablevar(tolua_S,"monster_ego","weight",toluaI_get_monster_monster_ego_weight,toluaI_set_monster_monster_ego_weight);
 tolua_tablevar(tolua_S,"monster_ego","freq_inate",toluaI_get_monster_monster_ego_freq_inate,toluaI_set_monster_monster_ego_freq_inate);
 tolua_tablevar(tolua_S,"monster_ego","freq_spell",toluaI_get_monster_monster_ego_freq_spell,toluaI_set_monster_monster_ego_freq_spell);
 tolua_tablevar(tolua_S,"monster_ego","flags",toluaI_get_monster_monster_ego_flags,toluaI_set_monster_monster_ego_flags);
 tolua_tablevar(tolua_S,"monster_ego","hflags",toluaI_get_monster_monster_ego_hflags,toluaI_set_monster_monster_ego_hflags);
 tolua_tablevar(tolua_S,"monster_ego","mflags",toluaI_get_monster_monster_ego_mflags,toluaI_set_monster_monster_ego_mflags);
 tolua_tablevar(tolua_S,"monster_ego","nflags",toluaI_get_monster_monster_ego_nflags,toluaI_set_monster_monster_ego_nflags);
 tolua_tablevar(tolua_S,"monster_ego","msflags",toluaI_get_monster_monster_ego_msflags,toluaI_set_monster_monster_ego_msflags);
 tolua_tablevar(tolua_S,"monster_ego","nsflags",toluaI_get_monster_monster_ego_nsflags,toluaI_set_monster_monster_ego_nsflags);
 tolua_tablevar(tolua_S,"monster_ego","remove_all_spells",toluaI_get_monster_monster_ego_remove_all_spells,toluaI_set_monster_monster_ego_remove_all_spells);
 tolua_tablevar(tolua_S,"monster_ego","level",toluaI_get_monster_monster_ego_level,toluaI_set_monster_monster_ego_level);
 tolua_tablevar(tolua_S,"monster_ego","rarity",toluaI_get_monster_monster_ego_rarity,toluaI_set_monster_monster_ego_rarity);
 tolua_tablevar(tolua_S,"monster_ego","d_attr",toluaI_get_monster_monster_ego_d_attr,toluaI_set_monster_monster_ego_d_attr);
 tolua_tablevar(tolua_S,"monster_ego","d_char",toluaI_get_monster_monster_ego_d_char,toluaI_set_monster_monster_ego_d_char);
 tolua_tablevar(tolua_S,"monster_ego","g_attr",toluaI_get_monster_monster_ego_g_attr,toluaI_set_monster_monster_ego_g_attr);
 tolua_tablevar(tolua_S,"monster_ego","g_char",toluaI_get_monster_monster_ego_g_char,toluaI_set_monster_monster_ego_g_char);
 tolua_tablearray(tolua_S,"monster_ego","r_char",toluaI_get_monster_monster_ego_r_char,toluaI_set_monster_monster_ego_r_char);
 tolua_tablearray(tolua_S,"monster_ego","nr_char",toluaI_get_monster_monster_ego_nr_char,toluaI_set_monster_monster_ego_nr_char);
 tolua_tablevar(tolua_S,"monster_ego","load_script",toluaI_get_monster_monster_ego_load_script,toluaI_set_monster_monster_ego_load_script);
 tolua_cclass(tolua_S,"monster_type","");
 tolua_tablevar(tolua_S,"monster_type","r_idx",toluaI_get_monster_monster_type_r_idx,toluaI_set_monster_monster_type_r_idx);
 tolua_tablevar(tolua_S,"monster_type","d_char",toluaI_get_monster_monster_type_d_char,toluaI_set_monster_monster_type_d_char);
 tolua_tablevar(tolua_S,"monster_type","sval",toluaI_get_monster_monster_type_sval,toluaI_set_monster_monster_type_sval);
 tolua_tablevar(tolua_S,"monster_type","ego",toluaI_get_monster_monster_type_ego,toluaI_set_monster_monster_type_ego);
 tolua_tablevar(tolua_S,"monster_type","fy",toluaI_get_monster_monster_type_fy,toluaI_set_monster_monster_type_fy);
 tolua_tablevar(tolua_S,"monster_type","fx",toluaI_get_monster_monster_type_fx,toluaI_set_monster_monster_type_fx);
 tolua_tablevar(tolua_S,"monster_type","hp",toluaI_get_monster_monster_type_hp,toluaI_set_monster_monster_type_hp);
 tolua_tablevar(tolua_S,"monster_type","maxhp",toluaI_get_monster_monster_type_maxhp,toluaI_set_monster_monster_type_maxhp);
 tolua_tablevar(tolua_S,"monster_type","blow",toluaI_get_monster_monster_type_blow,toluaI_set_monster_monster_type_blow);
 tolua_tablevar(tolua_S,"monster_type","speed",toluaI_get_monster_monster_type_speed,toluaI_set_monster_monster_type_speed);
 tolua_tablevar(tolua_S,"monster_type","level",toluaI_get_monster_monster_type_level,toluaI_set_monster_monster_type_level);
 tolua_tablevar(tolua_S,"monster_type","ac",toluaI_get_monster_monster_type_ac,toluaI_set_monster_monster_type_ac);
 tolua_tablevar(tolua_S,"monster_type","exp",toluaI_get_monster_monster_type_exp,toluaI_set_monster_monster_type_exp);
 tolua_tablevar(tolua_S,"monster_type","csleep",toluaI_get_monster_monster_type_csleep,toluaI_set_monster_monster_type_csleep);
 tolua_tablevar(tolua_S,"monster_type","mspeed",toluaI_get_monster_monster_type_mspeed,toluaI_set_monster_monster_type_mspeed);
 tolua_tablevar(tolua_S,"monster_type","energy",toluaI_get_monster_monster_type_energy,toluaI_set_monster_monster_type_energy);
 tolua_tablevar(tolua_S,"monster_type","stunned",toluaI_get_monster_monster_type_stunned,toluaI_set_monster_monster_type_stunned);
 tolua_tablevar(tolua_S,"monster_type","confused",toluaI_get_monster_monster_type_confused,toluaI_set_monster_monster_type_confused);
 tolua_tablevar(tolua_S,"monster_type","monfear",toluaI_get_monster_monster_type_monfear,toluaI_set_monster_monster_type_monfear);
 tolua_tablevar(tolua_S,"monster_type","freq_inate",toluaI_get_monster_monster_type_freq_inate,toluaI_set_monster_monster_type_freq_inate);
 tolua_tablevar(tolua_S,"monster_type","freq_spell",toluaI_get_monster_monster_type_freq_spell,toluaI_set_monster_monster_type_freq_spell);
 tolua_tablevar(tolua_S,"monster_type","flags",toluaI_get_monster_monster_type_flags,toluaI_set_monster_monster_type_flags);
 tolua_tablevar(tolua_S,"monster_type","spells",toluaI_get_monster_monster_type_spells,toluaI_set_monster_monster_type_spells);
 tolua_tablevar(tolua_S,"monster_type","bleeding",toluaI_get_monster_monster_type_bleeding,toluaI_set_monster_monster_type_bleeding);
 tolua_tablevar(tolua_S,"monster_type","poisoned",toluaI_get_monster_monster_type_poisoned,toluaI_set_monster_monster_type_poisoned);
 tolua_tablevar(tolua_S,"monster_type","cdis",toluaI_get_monster_monster_type_cdis,toluaI_set_monster_monster_type_cdis);
 tolua_tablevar(tolua_S,"monster_type","mflag",toluaI_get_monster_monster_type_mflag,toluaI_set_monster_monster_type_mflag);
 tolua_tablevar(tolua_S,"monster_type","ml",toluaI_get_monster_monster_type_ml,toluaI_set_monster_monster_type_ml);
 tolua_tablevar(tolua_S,"monster_type","inventory",toluaI_get_monster_monster_type_inventory,toluaI_set_monster_monster_type_inventory);
 tolua_tablevar(tolua_S,"monster_type","ai",toluaI_get_monster_monster_type_ai,toluaI_set_monster_monster_type_ai);
 tolua_tablevar(tolua_S,"monster_type","ai_states",toluaI_get_monster_monster_type_ai_states,toluaI_set_monster_monster_type_ai_states);
 tolua_tablevar(tolua_S,"monster_type","ai_speed",toluaI_get_monster_monster_type_ai_speed,toluaI_set_monster_monster_type_ai_speed);
 tolua_tablevar(tolua_S,"monster_type","ai_action",toluaI_get_monster_monster_type_ai_action,toluaI_set_monster_monster_type_ai_action);
 tolua_tablevar(tolua_S,"monster_type","ai_move_y",toluaI_get_monster_monster_type_ai_move_y,toluaI_set_monster_monster_type_ai_move_y);
 tolua_tablevar(tolua_S,"monster_type","ai_move_x",toluaI_get_monster_monster_type_ai_move_x,toluaI_set_monster_monster_type_ai_move_x);
 tolua_tablevar(tolua_S,"monster_type","smart",toluaI_get_monster_monster_type_smart,toluaI_set_monster_monster_type_smart);
 tolua_tablevar(tolua_S,"monster_type","faction",toluaI_get_monster_monster_type_faction,toluaI_set_monster_monster_type_faction);
 tolua_tablevar(tolua_S,"monster_type","target",toluaI_get_monster_monster_type_target,toluaI_set_monster_monster_type_target);
 tolua_tablevar(tolua_S,"monster_type","possessor",toluaI_get_monster_monster_type_possessor,toluaI_set_monster_monster_type_possessor);
 tolua_function(tolua_S,NULL,"monster",toluaI_monster_monster00);
 tolua_globalvar(tolua_S,"monst_list",toluaI_get_monster_monst_list,toluaI_set_monster_monst_list);
 tolua_function(tolua_S,NULL,"race_info_idx",toluaI_monster_race_info_idx00);
 tolua_function(tolua_S,NULL,"delete_monster_idx",toluaI_monster_delete_monster_idx00);
 tolua_function(tolua_S,NULL,"m_pop",toluaI_monster_m_pop00);
 tolua_function(tolua_S,NULL,"set_mon_num_hook",toluaI_monster_set_mon_num_hook00);
 tolua_function(tolua_S,NULL,"get_mon_num_prep",toluaI_monster_get_mon_num_prep00);
 tolua_function(tolua_S,NULL,"get_mon_num",toluaI_monster_get_mon_num00);
 tolua_function(tolua_S,NULL,"monster_desc",toluaI_monster_monster_desc00);
 tolua_function(tolua_S,NULL,"monster_race_desc",toluaI_monster_monster_race_desc00);
 tolua_function(tolua_S,NULL,"monster_race_desc",toluaI_monster_monster_race_desc01);
 tolua_function(tolua_S,NULL,"monster_carry",toluaI_monster_monster_carry00);
 tolua_globalvar(tolua_S,"monster_level",toluaI_get_monster_monster_level,toluaI_set_monster_monster_level);
 tolua_function(tolua_S,NULL,"place_monster_aux",toluaI_monster_place_monster_aux00);
 tolua_function(tolua_S,NULL,"place_monster_aux_ego",toluaI_monster_place_monster_aux_ego00);
 tolua_function(tolua_S,NULL,"place_monster",toluaI_monster_place_monster00);
 tolua_function(tolua_S,NULL,"place_monster_type",toluaI_monster_place_monster_type00);
 tolua_function(tolua_S,NULL,"monster_prep",toluaI_monster_monster_prep00);
 tolua_globalarray(tolua_S,"placed_monsters",toluaI_get_monster_placed_monsters,toluaI_set_monster_placed_monsters);
 tolua_globalarray(tolua_S,"placed_monster_ids",toluaI_get_monster_placed_monster_ids,toluaI_set_monster_placed_monster_ids);
 tolua_globalvar(tolua_S,"placed_monsters_num",toluaI_get_monster_placed_monsters_num,toluaI_set_monster_placed_monsters_num);
 tolua_globalvar(tolua_S,"placed_monster_id",toluaI_get_monster_placed_monster_id,toluaI_set_monster_placed_monster_id);
 tolua_function(tolua_S,NULL,"place_monster_one",toluaI_monster_place_monster_one00);
 tolua_globalvar(tolua_S,"place_monster_one_no_drop",toluaI_get_monster_place_monster_one_no_drop,toluaI_set_monster_place_monster_one_no_drop);
 tolua_function(tolua_S,NULL,"alloc_monster",toluaI_monster_alloc_monster00);
 tolua_function(tolua_S,NULL,"is_friend",toluaI_monster_is_friend00);
 tolua_function(tolua_S,NULL,"is_enemy",toluaI_monster_is_enemy00);
 tolua_function(tolua_S,NULL,"find_position",toluaI_monster_find_position00);
 tolua_function(tolua_S,NULL,"get_rand_r_idx",toluaI_monster_get_rand_r_idx00);
 tolua_globalvar(tolua_S,"summon_specific_level",toluaI_get_monster_summon_specific_level,toluaI_set_monster_summon_specific_level);
 tolua_function(tolua_S,NULL,"summon_specific",toluaI_monster_summon_specific00);
 tolua_function(tolua_S,NULL,"summon_monster_aux",toluaI_monster_summon_monster_aux00);
 tolua_function(tolua_S,NULL,"can_create_companion",toluaI_monster_can_create_companion00);
 tolua_function(tolua_S,NULL,"monster_set_level",toluaI_monster_monster_set_level00);
 tolua_function(tolua_S,NULL,"can_pass_aux",toluaI_monster_can_pass_aux00);
 tolua_function(tolua_S,NULL,"monst_can_pass_square",toluaI_monster_monst_can_pass_square00);
 tolua_function(tolua_S,NULL,"do_control_reconnect",toluaI_monster_do_control_reconnect00);
 tolua_globalvar(tolua_S,"max_monsters",toluaI_get_monster_max_monsters,toluaI_set_monster_max_monsters);
 tolua_globalvar(tolua_S,"max_r_idx",toluaI_get_monster_max_r_idx,toluaI_set_monster_max_r_idx);
 tolua_globalvar(tolua_S,"max_re_idx",toluaI_get_monster_max_re_idx,toluaI_set_monster_max_re_idx);
 tolua_globalarray(tolua_S,"r_info",toluaI_get_monster_r_info,toluaI_set_monster_r_info);
 tolua_globalarray(tolua_S,"re_info",toluaI_get_monster_re_info,toluaI_set_monster_re_info);
 tolua_function(tolua_S,NULL,"mon_take_hit",toluaI_monster_mon_take_hit00);
 tolua_globalvar(tolua_S,"health_who",toluaI_get_monster_health_who,toluaI_set_monster_health_who);
 tolua_function(tolua_S,NULL,"health_track",toluaI_monster_health_track00);
 tolua_function(tolua_S,NULL,"monster_race_track",toluaI_monster_monster_race_track00);
 tolua_function(tolua_S,NULL,"multiply_monster",toluaI_monster_multiply_monster00);
 tolua_function(tolua_S,NULL,"ai_multiply",toluaI_monster_ai_multiply00);
 tolua_function(tolua_S,NULL,"do_poly_monster",toluaI_monster_do_poly_monster00);
 tolua_function(tolua_S,NULL,"message_pain",toluaI_monster_message_pain00);
 tolua_function(tolua_S,NULL,"lookup_race",toluaI_monster_lookup_race00);
 tolua_module(tolua_S,"factions");
 tolua_tablevar(tolua_S,"factions","faction_names",toluaI_get_monster_factions_faction_names,toluaI_set_monster_factions_faction_names);
 tolua_function(tolua_S,"factions","set_friendliness",toluaI_monster_factions_set_friendliness00);
 tolua_function(tolua_S,"factions","get_friendliness",toluaI_monster_factions_get_friendliness00);
 tolua_function(tolua_S,"factions","change_faction",toluaI_monster_factions_change_faction00);
 tolua_function(tolua_S,"factions","get_hostile_faction",toluaI_monster_factions_get_hostile_faction00);
 tolua_function(tolua_S,NULL,"monster_blow_new",toluaI_monster_monster_blow_new00);
 tolua_function(tolua_S,NULL,"monster_blow_del",toluaI_monster_monster_blow_del00);
 tolua_function(tolua_S,NULL,"monster_blow_dup",toluaI_monster_monster_blow_dup00);
 tolua_function(tolua_S,NULL,"monster_type_new",toluaI_monster_monster_type_new00);
 tolua_function(tolua_S,NULL,"delete_monster_type",toluaI_monster_delete_monster_type00);
 tolua_function(tolua_S,NULL,"monster_type_free_memory",toluaI_monster_monster_type_free_memory00);
 tolua_function(tolua_S,NULL,"monster_type_dup",toluaI_monster_monster_type_dup00);
 tolua_function(tolua_S,NULL,"monster_type_copy",toluaI_monster_monster_type_copy00);
 tolua_function(tolua_S,NULL,"mon_take_hit_mon",toluaI_monster_mon_take_hit_mon00);
 tolua_function(tolua_S,NULL,"set_monster_ai",toluaI_monster_set_monster_ai00);
 tolua_function(tolua_S,NULL,"find_safety",toluaI_monster_find_safety00);
 tolua_function(tolua_S,NULL,"find_hiding",toluaI_monster_find_hiding00);
 tolua_function(tolua_S,NULL,"get_monster_energy",toluaI_monster_get_monster_energy00);
 tolua_function(tolua_S,NULL,"is_obj_mimic_at",toluaI_monster_is_obj_mimic_at00);
 tolua_function(tolua_S,NULL,"get_obj_mimic_obj_at",toluaI_monster_get_obj_mimic_obj_at00);
 tolua_function(tolua_S,NULL,"is_feat_mimic_at",toluaI_monster_is_feat_mimic_at00);
 tolua_function(tolua_S,NULL,"get_feat_mimic_feat_at",toluaI_monster_get_feat_mimic_feat_at00);
 tolua_module(tolua_S,"__mego__");
 tolua_constant(tolua_S,"__mego__","MEGO_CHAR_ANY",MEGO_CHAR_ANY);
 tolua_constant(tolua_S,"__mego__","MEGO_ADD",MEGO_ADD);
 tolua_constant(tolua_S,"__mego__","MEGO_SUB",MEGO_SUB);
 tolua_constant(tolua_S,"__mego__","MEGO_FIX",MEGO_FIX);
 tolua_constant(tolua_S,"__mego__","MEGO_PRC",MEGO_PRC);
 tolua_constant(tolua_S,NULL,"RT_NONE",RT_NONE);
 tolua_constant(tolua_S,NULL,"RT_MISC",RT_MISC);
 tolua_constant(tolua_S,NULL,"RT_RESIST",RT_RESIST);
 tolua_constant(tolua_S,NULL,"RT_SPELL",RT_SPELL);
 tolua_constant(tolua_S,NULL,"RT_BLOWS",RT_BLOWS);
 tolua_constant(tolua_S,NULL,"RT_MAX",RT_MAX);
 tolua_function(tolua_S,NULL,"player_monster_swap",toluaI_monster_player_monster_swap00);
 tolua_function(tolua_S,NULL,"talk_to_monster",toluaI_monster_talk_to_monster00);
 tolua_globalvar(tolua_S,"repair_monsters",toluaI_get_monster_repair_monsters,toluaI_set_monster_repair_monsters);
 tolua_function(tolua_S,NULL,"wipe_m_list",toluaI_monster_wipe_m_list00);
 return 1;
}
/* Close function */
void tolua_monster_close (lua_State* tolua_S)
{
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"monster_forge"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FACTION_PLAYER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FACTION_TOWN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FACTION_DUNGEON");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_VIEW");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_QUEST");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_PARTIAL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_CONTROL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_BORN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_NICE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_SHOW");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_MARK");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_NO_DROP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MFLAG_QUEST2");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_blow");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_race");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_ego");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"monst_list"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"race_info_idx");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"delete_monster_idx");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"m_pop");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"set_mon_num_hook");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_mon_num_prep");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_mon_num");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_desc");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_race_desc");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_race_desc");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_carry");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"monster_level"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_monster_aux");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_monster_aux_ego");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_monster");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_monster_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_prep");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"placed_monsters");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"placed_monster_ids");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"placed_monsters_num"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"placed_monster_id"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_monster_one");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"place_monster_one_no_drop"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"alloc_monster");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_friend");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_enemy");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_position");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_rand_r_idx");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"summon_specific_level"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"summon_specific");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"summon_monster_aux");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"can_create_companion");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_set_level");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"can_pass_aux");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monst_can_pass_square");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_control_reconnect");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_monsters"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_r_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_re_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"r_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"re_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"mon_take_hit");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"health_who"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"health_track");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_race_track");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"multiply_monster");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ai_multiply");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_poly_monster");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"message_pain");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lookup_race");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"factions");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_blow_new");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_blow_del");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_blow_dup");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_type_new");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"delete_monster_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_type_free_memory");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_type_dup");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"monster_type_copy");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"mon_take_hit_mon");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"set_monster_ai");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_safety");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_hiding");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_monster_energy");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_obj_mimic_at");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_obj_mimic_obj_at");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_feat_mimic_at");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_feat_mimic_feat_at");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"__mego__");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"RT_NONE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"RT_MISC");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"RT_RESIST");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"RT_SPELL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"RT_BLOWS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"RT_MAX");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"player_monster_swap");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"talk_to_monster");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"repair_monsters"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wipe_m_list");
}
