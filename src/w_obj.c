/*
** Lua binding: object
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:42 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_object_open (lua_State* tolua_S);
void tolua_object_close (lua_State* tolua_S);

#include "angband.h"
static bool lua_is_artifact(object_type *o_ptr) { return artifact_p(o_ptr); }
static bool lua_is_ego(object_type *o_ptr) { return ego_item_p(o_ptr); }
static bool lua_is_known(object_type *o_ptr) { return object_known_p(o_ptr); }
static void lua_set_known(object_type *o_ptr) { object_known(o_ptr); }
static bool lua_is_cursed(object_type *o_ptr) { return cursed_p(o_ptr); }

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"object_type");
 tolua_usertype(tolua_S,"__set_type_aux");
 tolua_usertype(tolua_S,"set_type");
 tolua_usertype(tolua_S,"object_kind");
 tolua_usertype(tolua_S,"flags_type");
 tolua_usertype(tolua_S,"artifact_type");
 tolua_usertype(tolua_S,"inventory_slot_type");
 tolua_usertype(tolua_S,"ego_item_type");
 tolua_usertype(tolua_S,"PHYSFS_file");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* get function: name of class  object_kind */
static int toluaI_get_object_object_kind_name(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  object_kind */
static int toluaI_set_object_object_kind_name(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: text of class  object_kind */
static int toluaI_get_object_object_kind_text(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->text);
 return 1;
}

/* set function: text of class  object_kind */
static int toluaI_set_object_object_kind_text(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->text = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: tval of class  object_kind */
static int toluaI_get_object_object_kind_tval(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->tval);
 return 1;
}

/* set function: tval of class  object_kind */
static int toluaI_set_object_object_kind_tval(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->tval = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sval of class  object_kind */
static int toluaI_get_object_object_kind_sval(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sval);
 return 1;
}

/* set function: sval of class  object_kind */
static int toluaI_set_object_object_kind_sval(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sval = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_h of class  object_kind */
static int toluaI_get_object_object_kind_to_h(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_h);
 return 1;
}

/* set function: to_h of class  object_kind */
static int toluaI_set_object_object_kind_to_h(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_h = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_d of class  object_kind */
static int toluaI_get_object_object_kind_to_d(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_d);
 return 1;
}

/* set function: to_d of class  object_kind */
static int toluaI_set_object_object_kind_to_d(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_d = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_a of class  object_kind */
static int toluaI_get_object_object_kind_to_a(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_a);
 return 1;
}

/* set function: to_a of class  object_kind */
static int toluaI_set_object_object_kind_to_a(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_a = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ac of class  object_kind */
static int toluaI_get_object_object_kind_ac(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ac);
 return 1;
}

/* set function: ac of class  object_kind */
static int toluaI_set_object_object_kind_ac(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ac = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dd of class  object_kind */
static int toluaI_get_object_object_kind_dd(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dd);
 return 1;
}

/* set function: dd of class  object_kind */
static int toluaI_set_object_object_kind_dd(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dd = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ds of class  object_kind */
static int toluaI_get_object_object_kind_ds(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ds);
 return 1;
}

/* set function: ds of class  object_kind */
static int toluaI_set_object_object_kind_ds(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ds = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: weight of class  object_kind */
static int toluaI_get_object_object_kind_weight(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->weight);
 return 1;
}

/* set function: weight of class  object_kind */
static int toluaI_set_object_object_kind_weight(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->weight = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: cost of class  object_kind */
static int toluaI_get_object_object_kind_cost(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->cost);
 return 1;
}

/* set function: cost of class  object_kind */
static int toluaI_set_object_object_kind_cost(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->cost = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  object_kind */
static int toluaI_get_object_object_kind_flags(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  object_kind */
static int toluaI_set_object_object_kind_flags(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: locale of class  object_kind */
static int toluaI_get_object_object_kind_locale(lua_State* tolua_S)
{
 int toluaI_index;
  object_kind* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (object_kind*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_KIND_ALLOCATIONS)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->locale[toluaI_index]);
 return 1;
}

/* set function: locale of class  object_kind */
static int toluaI_set_object_object_kind_locale(lua_State* tolua_S)
{
 int toluaI_index;
  object_kind* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (object_kind*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_KIND_ALLOCATIONS)
 tolua_error(tolua_S,"array indexing out of range.");
  self->locale[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: chance of class  object_kind */
static int toluaI_get_object_object_kind_chance(lua_State* tolua_S)
{
 int toluaI_index;
  object_kind* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (object_kind*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_KIND_ALLOCATIONS)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->chance[toluaI_index]);
 return 1;
}

/* set function: chance of class  object_kind */
static int toluaI_set_object_object_kind_chance(lua_State* tolua_S)
{
 int toluaI_index;
  object_kind* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (object_kind*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_KIND_ALLOCATIONS)
 tolua_error(tolua_S,"array indexing out of range.");
  self->chance[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: level of class  object_kind */
static int toluaI_get_object_object_kind_level(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->level);
 return 1;
}

/* set function: level of class  object_kind */
static int toluaI_set_object_object_kind_level(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->level = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: extra of class  object_kind */
static int toluaI_get_object_object_kind_extra(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->extra);
 return 1;
}

/* set function: extra of class  object_kind */
static int toluaI_set_object_object_kind_extra(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->extra = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_attr of class  object_kind */
static int toluaI_get_object_object_kind_d_attr(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_attr);
 return 1;
}

/* set function: d_attr of class  object_kind */
static int toluaI_set_object_object_kind_d_attr(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_char of class  object_kind */
static int toluaI_get_object_object_kind_d_char(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_char);
 return 1;
}

/* set function: d_char of class  object_kind */
static int toluaI_set_object_object_kind_d_char(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_attr of class  object_kind */
static int toluaI_get_object_object_kind_x_attr(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_attr);
 return 1;
}

/* set function: x_attr of class  object_kind */
static int toluaI_set_object_object_kind_x_attr(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_char of class  object_kind */
static int toluaI_get_object_object_kind_x_char(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_char);
 return 1;
}

/* set function: x_char of class  object_kind */
static int toluaI_set_object_object_kind_x_char(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_char = ((char)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flavor of class  object_kind */
static int toluaI_get_object_object_kind_flavor(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->flavor);
 return 1;
}

/* set function: flavor of class  object_kind */
static int toluaI_set_object_object_kind_flavor(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->flavor = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: easy_know of class  object_kind */
static int toluaI_get_object_object_kind_easy_know(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->easy_know);
 return 1;
}

/* set function: easy_know of class  object_kind */
static int toluaI_set_object_object_kind_easy_know(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->easy_know = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: aware of class  object_kind */
static int toluaI_get_object_object_kind_aware(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->aware);
 return 1;
}

/* set function: aware of class  object_kind */
static int toluaI_set_object_object_kind_aware(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->aware = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: tried of class  object_kind */
static int toluaI_get_object_object_kind_tried(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->tried);
 return 1;
}

/* set function: tried of class  object_kind */
static int toluaI_set_object_object_kind_tried(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->tried = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: know of class  object_kind */
static int toluaI_get_object_object_kind_know(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->know);
 return 1;
}

/* set function: know of class  object_kind */
static int toluaI_set_object_object_kind_know(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->know = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: btval of class  object_kind */
static int toluaI_get_object_object_kind_btval(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->btval);
 return 1;
}

/* set function: btval of class  object_kind */
static int toluaI_set_object_object_kind_btval(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->btval = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: bsval of class  object_kind */
static int toluaI_get_object_object_kind_bsval(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->bsval);
 return 1;
}

/* set function: bsval of class  object_kind */
static int toluaI_set_object_object_kind_bsval(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->bsval = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: artifact of class  object_kind */
static int toluaI_get_object_object_kind_artifact(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->artifact);
 return 1;
}

/* set function: artifact of class  object_kind */
static int toluaI_set_object_object_kind_artifact(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->artifact = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: power of class  object_kind */
static int toluaI_get_object_object_kind_power(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->power);
 return 1;
}

/* set function: power of class  object_kind */
static int toluaI_set_object_object_kind_power(lua_State* tolua_S)
{
  object_kind* self = (object_kind*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->power = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  artifact_type */
static int toluaI_get_object_artifact_type_name(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  artifact_type */
static int toluaI_set_object_artifact_type_name(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: unknown_name of class  artifact_type */
static int toluaI_get_object_artifact_type_unknown_name(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->unknown_name);
 return 1;
}

/* set function: unknown_name of class  artifact_type */
static int toluaI_set_object_artifact_type_unknown_name(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->unknown_name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: text of class  artifact_type */
static int toluaI_get_object_artifact_type_text(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->text);
 return 1;
}

/* set function: text of class  artifact_type */
static int toluaI_set_object_artifact_type_text(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->text = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: tval of class  artifact_type */
static int toluaI_get_object_artifact_type_tval(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->tval);
 return 1;
}

/* set function: tval of class  artifact_type */
static int toluaI_set_object_artifact_type_tval(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->tval = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sval of class  artifact_type */
static int toluaI_get_object_artifact_type_sval(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sval);
 return 1;
}

/* set function: sval of class  artifact_type */
static int toluaI_set_object_artifact_type_sval(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sval = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_attr of class  artifact_type */
static int toluaI_get_object_artifact_type_d_attr(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_attr);
 return 1;
}

/* set function: d_attr of class  artifact_type */
static int toluaI_set_object_artifact_type_d_attr(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_attr = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_attr of class  artifact_type */
static int toluaI_get_object_artifact_type_x_attr(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_attr);
 return 1;
}

/* set function: x_attr of class  artifact_type */
static int toluaI_set_object_artifact_type_x_attr(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_attr = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_h of class  artifact_type */
static int toluaI_get_object_artifact_type_to_h(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_h);
 return 1;
}

/* set function: to_h of class  artifact_type */
static int toluaI_set_object_artifact_type_to_h(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_h = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_d of class  artifact_type */
static int toluaI_get_object_artifact_type_to_d(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_d);
 return 1;
}

/* set function: to_d of class  artifact_type */
static int toluaI_set_object_artifact_type_to_d(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_d = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_a of class  artifact_type */
static int toluaI_get_object_artifact_type_to_a(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_a);
 return 1;
}

/* set function: to_a of class  artifact_type */
static int toluaI_set_object_artifact_type_to_a(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_a = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ac of class  artifact_type */
static int toluaI_get_object_artifact_type_ac(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ac);
 return 1;
}

/* set function: ac of class  artifact_type */
static int toluaI_set_object_artifact_type_ac(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ac = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dd of class  artifact_type */
static int toluaI_get_object_artifact_type_dd(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dd);
 return 1;
}

/* set function: dd of class  artifact_type */
static int toluaI_set_object_artifact_type_dd(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dd = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ds of class  artifact_type */
static int toluaI_get_object_artifact_type_ds(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ds);
 return 1;
}

/* set function: ds of class  artifact_type */
static int toluaI_set_object_artifact_type_ds(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ds = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: weight of class  artifact_type */
static int toluaI_get_object_artifact_type_weight(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->weight);
 return 1;
}

/* set function: weight of class  artifact_type */
static int toluaI_set_object_artifact_type_weight(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->weight = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: cost of class  artifact_type */
static int toluaI_get_object_artifact_type_cost(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->cost);
 return 1;
}

/* set function: cost of class  artifact_type */
static int toluaI_set_object_artifact_type_cost(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->cost = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  artifact_type */
static int toluaI_get_object_artifact_type_flags(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  artifact_type */
static int toluaI_set_object_artifact_type_flags(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: level of class  artifact_type */
static int toluaI_get_object_artifact_type_level(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->level);
 return 1;
}

/* set function: level of class  artifact_type */
static int toluaI_set_object_artifact_type_level(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->level = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: rarity of class  artifact_type */
static int toluaI_get_object_artifact_type_rarity(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->rarity);
 return 1;
}

/* set function: rarity of class  artifact_type */
static int toluaI_set_object_artifact_type_rarity(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->rarity = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: cur_num of class  artifact_type */
static int toluaI_get_object_artifact_type_cur_num(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->cur_num);
 return 1;
}

/* set function: cur_num of class  artifact_type */
static int toluaI_set_object_artifact_type_cur_num(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->cur_num = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_num of class  artifact_type */
static int toluaI_get_object_artifact_type_max_num(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_num);
 return 1;
}

/* set function: max_num of class  artifact_type */
static int toluaI_set_object_artifact_type_max_num(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_num = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: known of class  artifact_type */
static int toluaI_get_object_artifact_type_known(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->known);
 return 1;
}

/* set function: known of class  artifact_type */
static int toluaI_set_object_artifact_type_known(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->known = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: power of class  artifact_type */
static int toluaI_get_object_artifact_type_power(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->power);
 return 1;
}

/* set function: power of class  artifact_type */
static int toluaI_set_object_artifact_type_power(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->power = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: set of class  artifact_type */
static int toluaI_get_object_artifact_type_set(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->set);
 return 1;
}

/* set function: set of class  artifact_type */
static int toluaI_set_object_artifact_type_set(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->set = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: set_pos of class  artifact_type */
static int toluaI_get_object_artifact_type_set_pos(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->set_pos);
 return 1;
}

/* set function: set_pos of class  artifact_type */
static int toluaI_set_object_artifact_type_set_pos(lua_State* tolua_S)
{
  artifact_type* self = (artifact_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->set_pos = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  ego_item_type */
static int toluaI_get_object_ego_item_type_name(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  ego_item_type */
static int toluaI_set_object_ego_item_type_name(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: text of class  ego_item_type */
static int toluaI_get_object_ego_item_type_text(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->text);
 return 1;
}

/* set function: text of class  ego_item_type */
static int toluaI_set_object_ego_item_type_text(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->text = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: before of class  ego_item_type */
static int toluaI_get_object_ego_item_type_before(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->before);
 return 1;
}

/* set function: before of class  ego_item_type */
static int toluaI_set_object_ego_item_type_before(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->before = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: ego_group of class  ego_item_type */
static int toluaI_get_object_ego_item_type_ego_group(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->ego_group);
 return 1;
}

/* set function: ego_group of class  ego_item_type */
static int toluaI_set_object_ego_item_type_ego_group(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->ego_group = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: tval of class  ego_item_type */
static int toluaI_get_object_ego_item_type_tval(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_TVALS_PER_EGO)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->tval[toluaI_index]);
 return 1;
}

/* set function: tval of class  ego_item_type */
static int toluaI_set_object_ego_item_type_tval(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_TVALS_PER_EGO)
 tolua_error(tolua_S,"array indexing out of range.");
  self->tval[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: min_sval of class  ego_item_type */
static int toluaI_get_object_ego_item_type_min_sval(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_TVALS_PER_EGO)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->min_sval[toluaI_index]);
 return 1;
}

/* set function: min_sval of class  ego_item_type */
static int toluaI_set_object_ego_item_type_min_sval(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_TVALS_PER_EGO)
 tolua_error(tolua_S,"array indexing out of range.");
  self->min_sval[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: max_sval of class  ego_item_type */
static int toluaI_get_object_ego_item_type_max_sval(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_TVALS_PER_EGO)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->max_sval[toluaI_index]);
 return 1;
}

/* set function: max_sval of class  ego_item_type */
static int toluaI_set_object_ego_item_type_max_sval(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_TVALS_PER_EGO)
 tolua_error(tolua_S,"array indexing out of range.");
  self->max_sval[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: item_kinds_ok of class  ego_item_type */
static int toluaI_get_object_ego_item_type_item_kinds_ok(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->item_kinds_ok,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: item_kinds_ok of class  ego_item_type */
static int toluaI_set_object_ego_item_type_item_kinds_ok(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->item_kinds_ok = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: rating of class  ego_item_type */
static int toluaI_get_object_ego_item_type_rating(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->rating);
 return 1;
}

/* set function: rating of class  ego_item_type */
static int toluaI_set_object_ego_item_type_rating(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->rating = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: level of class  ego_item_type */
static int toluaI_get_object_ego_item_type_level(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->level);
 return 1;
}

/* set function: level of class  ego_item_type */
static int toluaI_set_object_ego_item_type_level(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->level = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: allocation of class  ego_item_type */
static int toluaI_get_object_ego_item_type_allocation(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->allocation);
 return 1;
}

/* set function: allocation of class  ego_item_type */
static int toluaI_set_object_ego_item_type_allocation(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->allocation = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_to_h of class  ego_item_type */
static int toluaI_get_object_ego_item_type_max_to_h(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_to_h);
 return 1;
}

/* set function: max_to_h of class  ego_item_type */
static int toluaI_set_object_ego_item_type_max_to_h(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_to_h = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_to_d of class  ego_item_type */
static int toluaI_get_object_ego_item_type_max_to_d(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_to_d);
 return 1;
}

/* set function: max_to_d of class  ego_item_type */
static int toluaI_set_object_ego_item_type_max_to_d(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_to_d = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: max_to_a of class  ego_item_type */
static int toluaI_get_object_ego_item_type_max_to_a(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->max_to_a);
 return 1;
}

/* set function: max_to_a of class  ego_item_type */
static int toluaI_set_object_ego_item_type_max_to_a(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->max_to_a = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: base_value_pct of class  ego_item_type */
static int toluaI_get_object_ego_item_type_base_value_pct(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->base_value_pct);
 return 1;
}

/* set function: base_value_pct of class  ego_item_type */
static int toluaI_set_object_ego_item_type_base_value_pct(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->base_value_pct = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: value_add of class  ego_item_type */
static int toluaI_get_object_ego_item_type_value_add(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->value_add);
 return 1;
}

/* set function: value_add of class  ego_item_type */
static int toluaI_set_object_ego_item_type_value_add(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->value_add = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: rar of class  ego_item_type */
static int toluaI_get_object_ego_item_type_rar(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_EGO_FLAG_GROUPS)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->rar[toluaI_index]);
 return 1;
}

/* set function: rar of class  ego_item_type */
static int toluaI_set_object_ego_item_type_rar(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_EGO_FLAG_GROUPS)
 tolua_error(tolua_S,"array indexing out of range.");
  self->rar[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: flags of class  ego_item_type */
static int toluaI_get_object_ego_item_type_flags(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_EGO_FLAG_GROUPS)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&self->flags[toluaI_index],tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  ego_item_type */
static int toluaI_set_object_ego_item_type_flags(lua_State* tolua_S)
{
 int toluaI_index;
  ego_item_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (ego_item_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_EGO_FLAG_GROUPS)
 tolua_error(tolua_S,"array indexing out of range.");
  self->flags[toluaI_index] = *((flags_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: need_flags of class  ego_item_type */
static int toluaI_get_object_ego_item_type_need_flags(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->need_flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: need_flags of class  ego_item_type */
static int toluaI_set_object_ego_item_type_need_flags(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->need_flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: forbid_flags of class  ego_item_type */
static int toluaI_get_object_ego_item_type_forbid_flags(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->forbid_flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: forbid_flags of class  ego_item_type */
static int toluaI_set_object_ego_item_type_forbid_flags(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->forbid_flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: power of class  ego_item_type */
static int toluaI_get_object_ego_item_type_power(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->power);
 return 1;
}

/* set function: power of class  ego_item_type */
static int toluaI_set_object_ego_item_type_power(lua_State* tolua_S)
{
  ego_item_type* self = (ego_item_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->power = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: k_idx of class  object_type */
static int toluaI_get_object_object_type_k_idx(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->k_idx);
 return 1;
}

/* set function: k_idx of class  object_type */
static int toluaI_set_object_object_type_k_idx(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->k_idx = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: iy of class  object_type */
static int toluaI_get_object_object_type_iy(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->iy);
 return 1;
}

/* set function: iy of class  object_type */
static int toluaI_set_object_object_type_iy(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->iy = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ix of class  object_type */
static int toluaI_get_object_object_type_ix(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ix);
 return 1;
}

/* set function: ix of class  object_type */
static int toluaI_set_object_object_type_ix(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ix = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: tval of class  object_type */
static int toluaI_get_object_object_type_tval(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->tval);
 return 1;
}

/* set function: tval of class  object_type */
static int toluaI_set_object_object_type_tval(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->tval = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sval of class  object_type */
static int toluaI_get_object_object_type_sval(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sval);
 return 1;
}

/* set function: sval of class  object_type */
static int toluaI_set_object_object_type_sval(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sval = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: discount of class  object_type */
static int toluaI_get_object_object_type_discount(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->discount);
 return 1;
}

/* set function: discount of class  object_type */
static int toluaI_set_object_object_type_discount(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->discount = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: number of class  object_type */
static int toluaI_get_object_object_type_number(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->number);
 return 1;
}

/* set function: number of class  object_type */
static int toluaI_set_object_object_type_number(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->number = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: weight of class  object_type */
static int toluaI_get_object_object_type_weight(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->weight);
 return 1;
}

/* set function: weight of class  object_type */
static int toluaI_set_object_object_type_weight(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->weight = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: elevel of class  object_type */
static int toluaI_get_object_object_type_elevel(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->elevel);
 return 1;
}

/* set function: elevel of class  object_type */
static int toluaI_set_object_object_type_elevel(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->elevel = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: exp of class  object_type */
static int toluaI_get_object_object_type_exp(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->exp);
 return 1;
}

/* set function: exp of class  object_type */
static int toluaI_set_object_object_type_exp(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->exp = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: d_attr of class  object_type */
static int toluaI_get_object_object_type_d_attr(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->d_attr);
 return 1;
}

/* set function: d_attr of class  object_type */
static int toluaI_set_object_object_type_d_attr(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->d_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: x_attr of class  object_type */
static int toluaI_get_object_object_type_x_attr(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->x_attr);
 return 1;
}

/* set function: x_attr of class  object_type */
static int toluaI_set_object_object_type_x_attr(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->x_attr = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: artifact_id of class  object_type */
static int toluaI_get_object_object_type_artifact_id(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->artifact_id);
 return 1;
}

/* set function: artifact_id of class  object_type */
static int toluaI_set_object_object_type_artifact_id(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->artifact_id = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ego_id of class  object_type */
static int toluaI_get_object_object_type_ego_id(lua_State* tolua_S)
{
 int toluaI_index;
  object_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (object_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_EGO_PER_OBJ)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->ego_id[toluaI_index]);
 return 1;
}

/* set function: ego_id of class  object_type */
static int toluaI_set_object_object_type_ego_id(lua_State* tolua_S)
{
 int toluaI_index;
  object_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (object_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=MAX_EGO_PER_OBJ)
 tolua_error(tolua_S,"array indexing out of range.");
  self->ego_id[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: xtra1 of class  object_type */
static int toluaI_get_object_object_type_xtra1(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->xtra1);
 return 1;
}

/* set function: xtra1 of class  object_type */
static int toluaI_set_object_object_type_xtra1(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->xtra1 = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: xtra2 of class  object_type */
static int toluaI_get_object_object_type_xtra2(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->xtra2);
 return 1;
}

/* set function: xtra2 of class  object_type */
static int toluaI_set_object_object_type_xtra2(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->xtra2 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_h of class  object_type */
static int toluaI_get_object_object_type_to_h(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_h);
 return 1;
}

/* set function: to_h of class  object_type */
static int toluaI_set_object_object_type_to_h(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_h = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_d of class  object_type */
static int toluaI_get_object_object_type_to_d(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_d);
 return 1;
}

/* set function: to_d of class  object_type */
static int toluaI_set_object_object_type_to_d(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_d = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: to_a of class  object_type */
static int toluaI_get_object_object_type_to_a(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->to_a);
 return 1;
}

/* set function: to_a of class  object_type */
static int toluaI_set_object_object_type_to_a(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->to_a = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ac of class  object_type */
static int toluaI_get_object_object_type_ac(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ac);
 return 1;
}

/* set function: ac of class  object_type */
static int toluaI_set_object_object_type_ac(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ac = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: dd of class  object_type */
static int toluaI_get_object_object_type_dd(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->dd);
 return 1;
}

/* set function: dd of class  object_type */
static int toluaI_set_object_object_type_dd(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->dd = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ds of class  object_type */
static int toluaI_get_object_object_type_ds(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ds);
 return 1;
}

/* set function: ds of class  object_type */
static int toluaI_set_object_object_type_ds(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ds = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: timeout of class  object_type */
static int toluaI_get_object_object_type_timeout(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->timeout);
 return 1;
}

/* set function: timeout of class  object_type */
static int toluaI_set_object_object_type_timeout(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->timeout = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: ident of class  object_type */
static int toluaI_get_object_object_type_ident(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->ident);
 return 1;
}

/* set function: ident of class  object_type */
static int toluaI_set_object_object_type_ident(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->ident = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: marked of class  object_type */
static int toluaI_get_object_object_type_marked(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->marked);
 return 1;
}

/* set function: marked of class  object_type */
static int toluaI_set_object_object_type_marked(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->marked = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: note of class  object_type */
static int toluaI_get_object_object_type_note(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->note);
 return 1;
}

/* set function: note of class  object_type */
static int toluaI_set_object_object_type_note(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->note = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: art_name of class  object_type */
static int toluaI_get_object_object_type_art_name(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->art_name);
 return 1;
}

/* set function: art_name of class  object_type */
static int toluaI_set_object_object_type_art_name(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->art_name = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  object_type */
static int toluaI_get_object_object_type_flags(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  object_type */
static int toluaI_set_object_object_type_flags(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: next_o_idx of class  object_type */
static int toluaI_get_object_object_type_next_o_idx(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->next_o_idx);
 return 1;
}

/* set function: next_o_idx of class  object_type */
static int toluaI_set_object_object_type_next_o_idx(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->next_o_idx = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: held_m_idx of class  object_type */
static int toluaI_get_object_object_type_held_m_idx(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->held_m_idx);
 return 1;
}

/* set function: held_m_idx of class  object_type */
static int toluaI_set_object_object_type_held_m_idx(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->held_m_idx = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: sense of class  object_type */
static int toluaI_get_object_object_type_sense(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->sense);
 return 1;
}

/* set function: sense of class  object_type */
static int toluaI_set_object_object_type_sense(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->sense = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: found of class  object_type */
static int toluaI_get_object_object_type_found(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->found);
 return 1;
}

/* set function: found of class  object_type */
static int toluaI_set_object_object_type_found(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->found = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: found_aux1 of class  object_type */
static int toluaI_get_object_object_type_found_aux1(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->found_aux1);
 return 1;
}

/* set function: found_aux1 of class  object_type */
static int toluaI_set_object_object_type_found_aux1(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->found_aux1 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: found_aux2 of class  object_type */
static int toluaI_get_object_object_type_found_aux2(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->found_aux2);
 return 1;
}

/* set function: found_aux2 of class  object_type */
static int toluaI_set_object_object_type_found_aux2(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->found_aux2 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: found_aux3 of class  object_type */
static int toluaI_get_object_object_type_found_aux3(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->found_aux3);
 return 1;
}

/* set function: found_aux3 of class  object_type */
static int toluaI_set_object_object_type_found_aux3(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->found_aux3 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: found_aux4 of class  object_type */
static int toluaI_get_object_object_type_found_aux4(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->found_aux4);
 return 1;
}

/* set function: found_aux4 of class  object_type */
static int toluaI_set_object_object_type_found_aux4(lua_State* tolua_S)
{
  object_type* self = (object_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->found_aux4 = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: present of class  __set_type_aux */
static int toluaI_get_object___set_type_aux_present(lua_State* tolua_S)
{
  __set_type_aux* self = (__set_type_aux*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->present);
 return 1;
}

/* set function: present of class  __set_type_aux */
static int toluaI_set_object___set_type_aux_present(lua_State* tolua_S)
{
  __set_type_aux* self = (__set_type_aux*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->present = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: a_idx of class  __set_type_aux */
static int toluaI_get_object___set_type_aux_a_idx(lua_State* tolua_S)
{
  __set_type_aux* self = (__set_type_aux*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->a_idx);
 return 1;
}

/* set function: a_idx of class  __set_type_aux */
static int toluaI_set_object___set_type_aux_a_idx(lua_State* tolua_S)
{
  __set_type_aux* self = (__set_type_aux*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->a_idx = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  __set_type_aux */
static int toluaI_get_object___set_type_aux_flags(lua_State* tolua_S)
{
 int toluaI_index;
  __set_type_aux* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (__set_type_aux*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=6)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&self->flags[toluaI_index],tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  __set_type_aux */
static int toluaI_set_object___set_type_aux_flags(lua_State* tolua_S)
{
 int toluaI_index;
  __set_type_aux* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (__set_type_aux*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=6)
 tolua_error(tolua_S,"array indexing out of range.");
  self->flags[toluaI_index] = *((flags_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: name of class  set_type */
static int toluaI_get_object_set_type_name(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  set_type */
static int toluaI_set_object_set_type_name(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: desc of class  set_type */
static int toluaI_get_object_set_type_desc(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->desc);
 return 1;
}

/* set function: desc of class  set_type */
static int toluaI_set_object_set_type_desc(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->desc = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: num of class  set_type */
static int toluaI_get_object_set_type_num(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->num);
 return 1;
}

/* set function: num of class  set_type */
static int toluaI_set_object_set_type_num(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->num = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: num_use of class  set_type */
static int toluaI_get_object_set_type_num_use(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->num_use);
 return 1;
}

/* set function: num_use of class  set_type */
static int toluaI_set_object_set_type_num_use(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->num_use = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: arts of class  set_type */
static int toluaI_get_object_set_type_arts(lua_State* tolua_S)
{
 int toluaI_index;
  set_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (set_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=6)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&self->arts[toluaI_index],tolua_tag(tolua_S,"__set_type_aux"));
 return 1;
}

/* set function: arts of class  set_type */
static int toluaI_set_object_set_type_arts(lua_State* tolua_S)
{
 int toluaI_index;
  set_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (set_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=6)
 tolua_error(tolua_S,"array indexing out of range.");
  self->arts[toluaI_index] = *((__set_type_aux*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: load_script of class  set_type */
static int toluaI_get_object_set_type_load_script(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->load_script);
 return 1;
}

/* set function: load_script of class  set_type */
static int toluaI_set_object_set_type_load_script(lua_State* tolua_S)
{
  set_type* self = (set_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->load_script = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: max_k_idx */
static int toluaI_get_object_max_k_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_k_idx);
 return 1;
}

/* set function: max_k_idx */
static int toluaI_set_object_max_k_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_k_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_a_idx */
static int toluaI_get_object_max_a_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_a_idx);
 return 1;
}

/* set function: max_a_idx */
static int toluaI_set_object_max_a_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_a_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_e_idx */
static int toluaI_get_object_max_e_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_e_idx);
 return 1;
}

/* set function: max_e_idx */
static int toluaI_set_object_max_e_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_e_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_ra_idx */
static int toluaI_get_object_max_ra_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_ra_idx);
 return 1;
}

/* set function: max_ra_idx */
static int toluaI_set_object_max_ra_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_ra_idx = ((u16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_set_idx */
static int toluaI_get_object_max_set_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_set_idx);
 return 1;
}

/* set function: max_set_idx */
static int toluaI_set_object_max_set_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_set_idx = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: k_info */
static int toluaI_get_object_k_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_k_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&k_info[toluaI_index],tolua_tag(tolua_S,"object_kind"));
 return 1;
}

/* set function: k_info */
static int toluaI_set_object_k_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_k_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  k_info[toluaI_index] = *((object_kind*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: a_info */
static int toluaI_get_object_a_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_a_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&a_info[toluaI_index],tolua_tag(tolua_S,"artifact_type"));
 return 1;
}

/* set function: a_info */
static int toluaI_set_object_a_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_a_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  a_info[toluaI_index] = *((artifact_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: e_info */
static int toluaI_get_object_e_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_e_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&e_info[toluaI_index],tolua_tag(tolua_S,"ego_item_type"));
 return 1;
}

/* set function: e_info */
static int toluaI_set_object_e_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_e_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  e_info[toluaI_index] = *((ego_item_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: set_info */
static int toluaI_get_object_set_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_set_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&set_info[toluaI_index],tolua_tag(tolua_S,"set_type"));
 return 1;
}

/* set function: set_info */
static int toluaI_set_object_set_info(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_set_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  set_info[toluaI_index] = *((set_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: object_level */
static int toluaI_get_object_object_level(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)object_level);
 return 1;
}

/* set function: object_level */
static int toluaI_set_object_object_level(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  object_level = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: m_bonus */
static int toluaI_object_m_bonus00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b max = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b level = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  m_bonus(max,level);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'm_bonus'.");
 return 0;
}

/* function: wield_slot_ideal */
static int toluaI_object_wield_slot_ideal00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  bool ideal = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  wield_slot_ideal(o_ptr,ideal);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wield_slot_ideal'.");
 return 0;
}

/* function: wield_slot */
static int toluaI_object_wield_slot00(lua_State* tolua_S)
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
  s16b toluaI_ret = (s16b)  wield_slot(o_ptr);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'wield_slot'.");
 return 0;
}

/* function: lua_object_desc_store */
static int toluaI_object_object_desc_store00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  s32b pref = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b mode = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  char* toluaI_ret = (char*)  lua_object_desc_store(o_ptr,pref,mode);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_desc_store'.");
 return 0;
}

/* function: item_describe */
static int toluaI_object_item_describe00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b item = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  item_describe(item);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'item_describe'.");
 return 0;
}

/* function: item_increase */
static int toluaI_object_item_increase00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b item = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b num = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  item_increase(item,num);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'item_increase'.");
 return 0;
}

/* function: item_optimize */
static int toluaI_object_item_optimize00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b item = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  item_optimize(item);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'item_optimize'.");
 return 0;
}

/* function: delete_object_idx */
static int toluaI_object_delete_object_idx00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b o_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  delete_object_idx(o_idx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete_object_idx'.");
 return 0;
}

/* function: ident_all */
static int toluaI_object_ident_all00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  ident_all();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ident_all'.");
 return 0;
}

/* function: lookup_kind */
static int toluaI_object_lookup_kind00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b tval = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b sval = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  lookup_kind(tval,sval);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lookup_kind'.");
 return 0;
}

/* function: object_wipe */
static int toluaI_object_object_wipe00(lua_State* tolua_S)
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
  object_wipe(o_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_wipe'.");
 return 0;
}

/* function: object_prep */
static int toluaI_object_object_prep00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  s32b k_idx = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  object_prep(o_ptr,k_idx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_prep'.");
 return 0;
}

/* function: object_copy */
static int toluaI_object_object_copy00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  object_type* j_ptr = ((object_type*)  tolua_getusertype(tolua_S,2,0));
 {
  object_copy(o_ptr,j_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_copy'.");
 return 0;
}

/* function: inven_carry_okay */
static int toluaI_object_inven_carry_okay00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  inven_carry_okay(o_ptr);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'inven_carry_okay'.");
 return 0;
}

/* function: apply_magic */
static int toluaI_object_apply_magic00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  s32b lev = ((s32b)  tolua_getnumber(tolua_S,2,0));
  bool okay = ((bool)  tolua_getbool(tolua_S,3,0));
  bool good = ((bool)  tolua_getbool(tolua_S,4,0));
  bool great = ((bool)  tolua_getbool(tolua_S,5,0));
 {
  apply_magic(o_ptr,lev,okay,good,great);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'apply_magic'.");
 return 0;
}

/* function: place_object */
static int toluaI_object_place_object00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  bool good = ((bool)  tolua_getbool(tolua_S,3,0));
  bool great = ((bool)  tolua_getbool(tolua_S,4,0));
  s32b where = ((s32b)  tolua_getnumber(tolua_S,5,0));
 {
  place_object(y,x,good,great,where);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_object'.");
 return 0;
}

/* function: drop_near */
static int toluaI_object_drop_near00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  s32b chance = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  s16b toluaI_ret = (s16b)  drop_near(o_ptr,chance,y,x);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'drop_near'.");
 return 0;
}

/* function: acquirement */
static int toluaI_object_acquirement00(lua_State* tolua_S)
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
  s32b num = ((s32b)  tolua_getnumber(tolua_S,3,0));
  bool great = ((bool)  tolua_getbool(tolua_S,4,0));
  bool known = ((bool)  tolua_getbool(tolua_S,5,0));
 {
  acquirement(y1,x1,num,great,known);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'acquirement'.");
 return 0;
}

/* function: get_object */
static int toluaI_object_get_object00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b item = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  object_type* toluaI_ret = (object_type*)  get_object(item);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"object_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_object'.");
 return 0;
}

/* function: set_object */
static int toluaI_object_set_object00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b item = ((s32b)  tolua_getnumber(tolua_S,1,0));
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,2,0));
 {
  set_object(item,o_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_object'.");
 return 0;
}

/* function: new_object */
static int toluaI_object_new_object00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  object_type* toluaI_ret = (object_type*)  new_object();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"object_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new_object'.");
 return 0;
}

/* function: object_dup */
static int toluaI_object_object_dup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  object_type* j_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
 {
  object_type* toluaI_ret = (object_type*)  object_dup(j_ptr);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"object_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_dup'.");
 return 0;
}

/* function: delete_object */
static int toluaI_object_delete_object00(lua_State* tolua_S)
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
  delete_object(o_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete_object'.");
 return 0;
}

/* function: item_tester_okay */
static int toluaI_object_item_tester_okay00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  s32b item = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  item_tester_okay(o_ptr,item);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'item_tester_okay'.");
 return 0;
}

/* function: get_item */
static int toluaI_object_get_item_aux00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b cp = ((s32b)  tolua_getnumber(tolua_S,1,0));
  cptr pmt = ((cptr)  tolua_getstring(tolua_S,2,0));
  cptr str = ((cptr)  tolua_getstring(tolua_S,3,0));
  s32b mode = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  bool toluaI_ret = (bool)  get_item(&cp,pmt,str,mode);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)cp);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_item_aux'.");
 return 0;
}

/* function: lua_set_item_tester */
static int toluaI_object_lua_set_item_tester00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b tval = ((s32b)  tolua_getnumber(tolua_S,1,0));
  char* fct = ((char*)  tolua_getstring(tolua_S,2,0));
 {
  lua_set_item_tester(tval,fct);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_set_item_tester'.");
 return 0;
}

/* function: make_item_fully_identified */
static int toluaI_object_make_item_fully_identified00(lua_State* tolua_S)
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
  make_item_fully_identified(o_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_item_fully_identified'.");
 return 0;
}

/* function: identify_pack_fully */
static int toluaI_object_identify_pack_fully00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  identify_pack_fully();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'identify_pack_fully'.");
 return 0;
}

/* function: inven_carry_inven */
static int toluaI_object_inven_carry_inven00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  flags_type* inven = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,2,0));
  bool final = ((bool)  tolua_getbool(tolua_S,3,0));
 {
  s16b toluaI_ret = (s16b)  inven_carry_inven(inven,o_ptr,final);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'inven_carry_inven'.");
 return 0;
}

/* function: calc_total_weight */
static int toluaI_object_calc_total_weight00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  s32b toluaI_ret = (s32b)  calc_total_weight();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'calc_total_weight'.");
 return 0;
}

/* function: get_slot */
static int toluaI_object_get_slot00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b slot = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  get_slot(slot);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_slot'.");
 return 0;
}

/* get function: sense_desc */
static int toluaI_get_object_sense_desc(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=1000)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushstring(tolua_S,(const char*)sense_desc[toluaI_index]);
 return 1;
}

/* set function: sense_desc */
static int toluaI_set_object_sense_desc(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=1000)
 tolua_error(tolua_S,"array indexing out of range.");
  sense_desc[toluaI_index] = ((cptr)  tolua_getstring(tolua_S,3,0));
 return 0;
}

/* function: object_pickup */
static int toluaI_object_object_pickup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b this_o_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  object_pickup(this_o_idx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_pickup'.");
 return 0;
}

/* function: inven_takeoff */
static int toluaI_object_inven_takeoff00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  s32b item = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b amt = ((s32b)  tolua_getnumber(tolua_S,2,0));
  bool force_drop = ((bool)  tolua_getbool(tolua_S,3,0));
  bool silent = ((bool)  tolua_getbool(tolua_S,4,FALSE));
 {
  s16b toluaI_ret = (s16b)  inven_takeoff(item,amt,force_drop,silent);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'inven_takeoff'.");
 return 0;
}

/* function: inven_drop */
static int toluaI_object_inven_drop00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b item = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b amt = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b dy = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b dx = ((s32b)  tolua_getnumber(tolua_S,4,0));
  bool silent = ((bool)  tolua_getbool(tolua_S,5,0));
 {
  s16b toluaI_ret = (s16b)  inven_drop(item,amt,dy,dx,silent);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'inven_drop'.");
 return 0;
}

/* function: make_gold */
static int toluaI_object_make_gold00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  object_type* j_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  make_gold(j_ptr);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_gold'.");
 return 0;
}

/* function: place_gold */
static int toluaI_object_place_gold00(lua_State* tolua_S)
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
  place_gold(y,x);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_gold'.");
 return 0;
}

/* function: object_out_desc */
static int toluaI_object_object_out_desc00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"PHYSFS_file"),1) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  object_type* q_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  PHYSFS_file* fff = ((PHYSFS_file*)  tolua_getusertype(tolua_S,2,NULL));
  bool trim_down = ((bool)  tolua_getbool(tolua_S,3,FALSE));
  bool wait_for_it = ((bool)  tolua_getbool(tolua_S,4,TRUE));
 {
  bool toluaI_ret = (bool)  object_out_desc(q_ptr,fff,trim_down,wait_for_it);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_out_desc'.");
 return 0;
}

/* function: lua_is_artifact */
static int toluaI_object_is_artifact00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_is_artifact(o_ptr);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_artifact'.");
 return 0;
}

/* function: lua_is_ego */
static int toluaI_object_is_ego00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_is_ego(o_ptr);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_ego'.");
 return 0;
}

/* function: lua_is_known */
static int toluaI_object_is_known00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_is_known(o_ptr);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_known'.");
 return 0;
}

/* function: lua_set_known */
static int toluaI_object_set_known00(lua_State* tolua_S)
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
  lua_set_known(o_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_known'.");
 return 0;
}

/* function: lua_is_cursed */
static int toluaI_object_is_cursed00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_is_cursed(o_ptr);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_cursed'.");
 return 0;
}

/* function: object_similar */
static int toluaI_object_object_similar00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  object_type* j_ptr = ((object_type*)  tolua_getusertype(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  object_similar(o_ptr,j_ptr);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_similar'.");
 return 0;
}

/* function: object_absorb */
static int toluaI_object_object_absorb00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  object_type* j_ptr = ((object_type*)  tolua_getusertype(tolua_S,2,0));
 {
  object_absorb(o_ptr,j_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_absorb'.");
 return 0;
}

/* function: object_unabsorb */
static int toluaI_object_object_unabsorb00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  object_type* j_ptr = ((object_type*)  tolua_getusertype(tolua_S,2,0));
 {
  object_unabsorb(o_ptr,j_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_unabsorb'.");
 return 0;
}

/* function: value_check_aux1 */
static int toluaI_object_value_check_aux100(lua_State* tolua_S)
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
  byte toluaI_ret = (byte)  value_check_aux1(o_ptr);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'value_check_aux1'.");
 return 0;
}

/* function: value_check_aux1_magic */
static int toluaI_object_value_check_aux1_magic00(lua_State* tolua_S)
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
  byte toluaI_ret = (byte)  value_check_aux1_magic(o_ptr);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'value_check_aux1_magic'.");
 return 0;
}

/* function: value_check_aux2 */
static int toluaI_object_value_check_aux200(lua_State* tolua_S)
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
  byte toluaI_ret = (byte)  value_check_aux2(o_ptr);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'value_check_aux2'.");
 return 0;
}

/* function: value_check_aux2_magic */
static int toluaI_object_value_check_aux2_magic00(lua_State* tolua_S)
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
  byte toluaI_ret = (byte)  value_check_aux2_magic(o_ptr);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'value_check_aux2_magic'.");
 return 0;
}

/* function: select_sense */
static int toluaI_object_select_sense00(lua_State* tolua_S)
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
  bool ok_combat = ((bool)  tolua_getbool(tolua_S,2,0));
  bool ok_magic = ((bool)  tolua_getbool(tolua_S,3,0));
 {
  byte toluaI_ret = (byte)  select_sense(o_ptr,ok_combat,ok_magic);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'select_sense'.");
 return 0;
}

/* function: remove_curse_object */
static int toluaI_object_remove_curse_object00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  bool all = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  remove_curse_object(o_ptr,all);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove_curse_object'.");
 return 0;
}

/* function: object_track */
static int toluaI_object_object_track00(lua_State* tolua_S)
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
  object_track(o_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_track'.");
 return 0;
}

/* function: get_item_extra_hook */
static int toluaI_object_get_item_extra_hook00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b cp = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  get_item_extra_hook(&cp);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)cp);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_item_extra_hook'.");
 return 0;
}

/* function: get_item_allow */
static int toluaI_object_get_item_allow00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b item = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  get_item_allow(item);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_item_allow'.");
 return 0;
}

/* function: get_tag */
static int toluaI_object_get_tag00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b cp = ((s32b)  tolua_getnumber(tolua_S,1,0));
  char tag = ((char)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  get_tag(&cp,tag);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)cp);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_tag'.");
 return 0;
}

/* function: object_value */
static int toluaI_object_object_value00(lua_State* tolua_S)
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
  s32b toluaI_ret = (s32b)  object_value(o_ptr);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_value'.");
 return 0;
}

/* function: object_value_real */
static int toluaI_object_object_value_real00(lua_State* tolua_S)
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
  s32b toluaI_ret = (s32b)  object_value_real(o_ptr);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'object_value_real'.");
 return 0;
}

/* get function: get_item_hook_find_obj_what */
static int toluaI_get_object_get_item_hook_find_obj_what(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)get_item_hook_find_obj_what);
 return 1;
}

/* set function: get_item_hook_find_obj_what */
static int toluaI_set_object_get_item_hook_find_obj_what(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  get_item_hook_find_obj_what = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* function: get_item_hook_find_obj */
static int toluaI_object_get_item_hook_find_obj00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b item = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  get_item_hook_find_obj(&item);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)item);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_item_hook_find_obj'.");
 return 0;
}

/* get function: tval_to_attr */
static int toluaI_get_object_tval_to_attr(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=128)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)tval_to_attr[toluaI_index]);
 return 1;
}

/* set function: tval_to_attr */
static int toluaI_set_object_tval_to_attr(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=128)
 tolua_error(tolua_S,"array indexing out of range.");
  tval_to_attr[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: desc of class  inventory_slot_type */
static int toluaI_get_object_inventory_slot_type_desc(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->desc);
 return 1;
}

/* set function: desc of class  inventory_slot_type */
static int toluaI_set_object_inventory_slot_type_desc(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->desc = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: name of class  inventory_slot_type */
static int toluaI_get_object_inventory_slot_type_name(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  inventory_slot_type */
static int toluaI_set_object_inventory_slot_type_name(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: describe of class  inventory_slot_type */
static int toluaI_get_object_inventory_slot_type_describe(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->describe);
 return 1;
}

/* set function: describe of class  inventory_slot_type */
static int toluaI_set_object_inventory_slot_type_describe(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->describe = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: wear of class  inventory_slot_type */
static int toluaI_get_object_inventory_slot_type_wear(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->wear);
 return 1;
}

/* set function: wear of class  inventory_slot_type */
static int toluaI_set_object_inventory_slot_type_wear(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->wear = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: takeoff of class  inventory_slot_type */
static int toluaI_get_object_inventory_slot_type_takeoff(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->takeoff);
 return 1;
}

/* set function: takeoff of class  inventory_slot_type */
static int toluaI_set_object_inventory_slot_type_takeoff(lua_State* tolua_S)
{
  inventory_slot_type* self = (inventory_slot_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->takeoff = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: inventory_slot_names */
static int toluaI_get_object_inventory_slot_names(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=INVEN_TOTAL)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&inventory_slot_names[toluaI_index],tolua_tag(tolua_S,"inventory_slot_type"));
 return 1;
}

/* set function: inventory_slot_names */
static int toluaI_set_object_inventory_slot_names(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=INVEN_TOTAL)
 tolua_error(tolua_S,"array indexing out of range.");
  inventory_slot_names[toluaI_index] = *((inventory_slot_type*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: INVEN_PACK */
static int toluaI_get_object_INVEN_PACK(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)INVEN_PACK);
 return 1;
}

/* set function: INVEN_PACK */
static int toluaI_set_object_INVEN_PACK(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  INVEN_PACK = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: INVEN_TOTAL */
static int toluaI_get_object_INVEN_TOTAL(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)INVEN_TOTAL);
 return 1;
}

/* set function: INVEN_TOTAL */
static int toluaI_set_object_INVEN_TOTAL(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  INVEN_TOTAL = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: repair_objects */
static int toluaI_get_object_repair_objects(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)repair_objects);
 return 1;
}

/* set function: repair_objects */
static int toluaI_set_object_repair_objects(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  repair_objects = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: mention_use */
static int toluaI_object_mention_use00(lua_State* tolua_S)
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
  cptr toluaI_ret = (cptr)  mention_use(i);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'mention_use'.");
 return 0;
}

/* function: describe_use */
static int toluaI_object_describe_use00(lua_State* tolua_S)
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
  cptr toluaI_ret = (cptr)  describe_use(i);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'describe_use'.");
 return 0;
}

/* function: preserve_artifacts_in_inven */
static int toluaI_object_preserve_artifacts_in_inven00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* inven = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  preserve_artifacts_in_inven(inven);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'preserve_artifacts_in_inven'.");
 return 0;
}

/* Open function */
int tolua_object_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_constant(tolua_S,NULL,"USE_EQUIP",USE_EQUIP);
 tolua_constant(tolua_S,NULL,"USE_INVEN",USE_INVEN);
 tolua_constant(tolua_S,NULL,"USE_FLOOR",USE_FLOOR);
 tolua_constant(tolua_S,NULL,"USE_EXTRA",USE_EXTRA);
 tolua_constant(tolua_S,NULL,"USE_AUTO",USE_AUTO);
 tolua_constant(tolua_S,NULL,"USE_DISPLAY",USE_DISPLAY);
 tolua_constant(tolua_S,NULL,"USE_SHOW_NO_ITEM",USE_SHOW_NO_ITEM);
 tolua_constant(tolua_S,NULL,"INVEN_INVEN",INVEN_INVEN);
 tolua_constant(tolua_S,NULL,"INVEN_MAX_SIZE",INVEN_MAX_SIZE);
 tolua_constant(tolua_S,NULL,"INVEN_LIMIT_KEY",INVEN_LIMIT_KEY);
 tolua_constant(tolua_S,NULL,"TV_CORPSE",TV_CORPSE);
 tolua_constant(tolua_S,NULL,"SV_CORPSE_CORPSE",SV_CORPSE_CORPSE);
 tolua_constant(tolua_S,NULL,"SV_CORPSE_SKELETON",SV_CORPSE_SKELETON);
 tolua_constant(tolua_S,NULL,"SV_CORPSE_HEAD",SV_CORPSE_HEAD);
 tolua_constant(tolua_S,NULL,"SV_CORPSE_SKULL",SV_CORPSE_SKULL);
 tolua_constant(tolua_S,NULL,"SV_CORPSE_MEAT",SV_CORPSE_MEAT);
 tolua_constant(tolua_S,NULL,"IDENT_SENSE",IDENT_SENSE);
 tolua_constant(tolua_S,NULL,"IDENT_FIXED",IDENT_FIXED);
 tolua_constant(tolua_S,NULL,"IDENT_EMPTY",IDENT_EMPTY);
 tolua_constant(tolua_S,NULL,"IDENT_KNOWN",IDENT_KNOWN);
 tolua_constant(tolua_S,NULL,"IDENT_STOREB",IDENT_STOREB);
 tolua_constant(tolua_S,NULL,"IDENT_MENTAL",IDENT_MENTAL);
 tolua_constant(tolua_S,NULL,"IDENT_CURSED",IDENT_CURSED);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_MONSTER",OBJ_FOUND_MONSTER);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_FLOOR",OBJ_FOUND_FLOOR);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_VAULT",OBJ_FOUND_VAULT);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_SPECIAL",OBJ_FOUND_SPECIAL);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_RUBBLE",OBJ_FOUND_RUBBLE);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_REWARD",OBJ_FOUND_REWARD);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_STORE",OBJ_FOUND_STORE);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_STOLEN",OBJ_FOUND_STOLEN);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_SELFMADE",OBJ_FOUND_SELFMADE);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_WISH",OBJ_FOUND_WISH);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_DEBUG",OBJ_FOUND_DEBUG);
 tolua_constant(tolua_S,NULL,"OBJ_FOUND_START",OBJ_FOUND_START);
 tolua_constant(tolua_S,NULL,"MAX_KIND_ALLOCATIONS",MAX_KIND_ALLOCATIONS);
 tolua_constant(tolua_S,NULL,"MAX_TVALS_PER_EGO",MAX_TVALS_PER_EGO);
 tolua_constant(tolua_S,NULL,"MAX_EGO_FLAG_GROUPS",MAX_EGO_FLAG_GROUPS);
 tolua_constant(tolua_S,NULL,"MAX_EGO_PER_OBJ",MAX_EGO_PER_OBJ);
 tolua_cclass(tolua_S,"object_kind","");
 tolua_tablevar(tolua_S,"object_kind","name",toluaI_get_object_object_kind_name,toluaI_set_object_object_kind_name);
 tolua_tablevar(tolua_S,"object_kind","text",toluaI_get_object_object_kind_text,toluaI_set_object_object_kind_text);
 tolua_tablevar(tolua_S,"object_kind","tval",toluaI_get_object_object_kind_tval,toluaI_set_object_object_kind_tval);
 tolua_tablevar(tolua_S,"object_kind","sval",toluaI_get_object_object_kind_sval,toluaI_set_object_object_kind_sval);
 tolua_tablevar(tolua_S,"object_kind","to_h",toluaI_get_object_object_kind_to_h,toluaI_set_object_object_kind_to_h);
 tolua_tablevar(tolua_S,"object_kind","to_d",toluaI_get_object_object_kind_to_d,toluaI_set_object_object_kind_to_d);
 tolua_tablevar(tolua_S,"object_kind","to_a",toluaI_get_object_object_kind_to_a,toluaI_set_object_object_kind_to_a);
 tolua_tablevar(tolua_S,"object_kind","ac",toluaI_get_object_object_kind_ac,toluaI_set_object_object_kind_ac);
 tolua_tablevar(tolua_S,"object_kind","dd",toluaI_get_object_object_kind_dd,toluaI_set_object_object_kind_dd);
 tolua_tablevar(tolua_S,"object_kind","ds",toluaI_get_object_object_kind_ds,toluaI_set_object_object_kind_ds);
 tolua_tablevar(tolua_S,"object_kind","weight",toluaI_get_object_object_kind_weight,toluaI_set_object_object_kind_weight);
 tolua_tablevar(tolua_S,"object_kind","cost",toluaI_get_object_object_kind_cost,toluaI_set_object_object_kind_cost);
 tolua_tablevar(tolua_S,"object_kind","flags",toluaI_get_object_object_kind_flags,toluaI_set_object_object_kind_flags);
 tolua_tablearray(tolua_S,"object_kind","locale",toluaI_get_object_object_kind_locale,toluaI_set_object_object_kind_locale);
 tolua_tablearray(tolua_S,"object_kind","chance",toluaI_get_object_object_kind_chance,toluaI_set_object_object_kind_chance);
 tolua_tablevar(tolua_S,"object_kind","level",toluaI_get_object_object_kind_level,toluaI_set_object_object_kind_level);
 tolua_tablevar(tolua_S,"object_kind","extra",toluaI_get_object_object_kind_extra,toluaI_set_object_object_kind_extra);
 tolua_tablevar(tolua_S,"object_kind","d_attr",toluaI_get_object_object_kind_d_attr,toluaI_set_object_object_kind_d_attr);
 tolua_tablevar(tolua_S,"object_kind","d_char",toluaI_get_object_object_kind_d_char,toluaI_set_object_object_kind_d_char);
 tolua_tablevar(tolua_S,"object_kind","x_attr",toluaI_get_object_object_kind_x_attr,toluaI_set_object_object_kind_x_attr);
 tolua_tablevar(tolua_S,"object_kind","x_char",toluaI_get_object_object_kind_x_char,toluaI_set_object_object_kind_x_char);
 tolua_tablevar(tolua_S,"object_kind","flavor",toluaI_get_object_object_kind_flavor,toluaI_set_object_object_kind_flavor);
 tolua_tablevar(tolua_S,"object_kind","easy_know",toluaI_get_object_object_kind_easy_know,toluaI_set_object_object_kind_easy_know);
 tolua_tablevar(tolua_S,"object_kind","aware",toluaI_get_object_object_kind_aware,toluaI_set_object_object_kind_aware);
 tolua_tablevar(tolua_S,"object_kind","tried",toluaI_get_object_object_kind_tried,toluaI_set_object_object_kind_tried);
 tolua_tablevar(tolua_S,"object_kind","know",toluaI_get_object_object_kind_know,toluaI_set_object_object_kind_know);
 tolua_tablevar(tolua_S,"object_kind","btval",toluaI_get_object_object_kind_btval,toluaI_set_object_object_kind_btval);
 tolua_tablevar(tolua_S,"object_kind","bsval",toluaI_get_object_object_kind_bsval,toluaI_set_object_object_kind_bsval);
 tolua_tablevar(tolua_S,"object_kind","artifact",toluaI_get_object_object_kind_artifact,toluaI_set_object_object_kind_artifact);
 tolua_tablevar(tolua_S,"object_kind","power",toluaI_get_object_object_kind_power,toluaI_set_object_object_kind_power);
 tolua_cclass(tolua_S,"artifact_type","");
 tolua_tablevar(tolua_S,"artifact_type","name",toluaI_get_object_artifact_type_name,toluaI_set_object_artifact_type_name);
 tolua_tablevar(tolua_S,"artifact_type","unknown_name",toluaI_get_object_artifact_type_unknown_name,toluaI_set_object_artifact_type_unknown_name);
 tolua_tablevar(tolua_S,"artifact_type","text",toluaI_get_object_artifact_type_text,toluaI_set_object_artifact_type_text);
 tolua_tablevar(tolua_S,"artifact_type","tval",toluaI_get_object_artifact_type_tval,toluaI_set_object_artifact_type_tval);
 tolua_tablevar(tolua_S,"artifact_type","sval",toluaI_get_object_artifact_type_sval,toluaI_set_object_artifact_type_sval);
 tolua_tablevar(tolua_S,"artifact_type","d_attr",toluaI_get_object_artifact_type_d_attr,toluaI_set_object_artifact_type_d_attr);
 tolua_tablevar(tolua_S,"artifact_type","x_attr",toluaI_get_object_artifact_type_x_attr,toluaI_set_object_artifact_type_x_attr);
 tolua_tablevar(tolua_S,"artifact_type","to_h",toluaI_get_object_artifact_type_to_h,toluaI_set_object_artifact_type_to_h);
 tolua_tablevar(tolua_S,"artifact_type","to_d",toluaI_get_object_artifact_type_to_d,toluaI_set_object_artifact_type_to_d);
 tolua_tablevar(tolua_S,"artifact_type","to_a",toluaI_get_object_artifact_type_to_a,toluaI_set_object_artifact_type_to_a);
 tolua_tablevar(tolua_S,"artifact_type","ac",toluaI_get_object_artifact_type_ac,toluaI_set_object_artifact_type_ac);
 tolua_tablevar(tolua_S,"artifact_type","dd",toluaI_get_object_artifact_type_dd,toluaI_set_object_artifact_type_dd);
 tolua_tablevar(tolua_S,"artifact_type","ds",toluaI_get_object_artifact_type_ds,toluaI_set_object_artifact_type_ds);
 tolua_tablevar(tolua_S,"artifact_type","weight",toluaI_get_object_artifact_type_weight,toluaI_set_object_artifact_type_weight);
 tolua_tablevar(tolua_S,"artifact_type","cost",toluaI_get_object_artifact_type_cost,toluaI_set_object_artifact_type_cost);
 tolua_tablevar(tolua_S,"artifact_type","flags",toluaI_get_object_artifact_type_flags,toluaI_set_object_artifact_type_flags);
 tolua_tablevar(tolua_S,"artifact_type","level",toluaI_get_object_artifact_type_level,toluaI_set_object_artifact_type_level);
 tolua_tablevar(tolua_S,"artifact_type","rarity",toluaI_get_object_artifact_type_rarity,toluaI_set_object_artifact_type_rarity);
 tolua_tablevar(tolua_S,"artifact_type","cur_num",toluaI_get_object_artifact_type_cur_num,toluaI_set_object_artifact_type_cur_num);
 tolua_tablevar(tolua_S,"artifact_type","max_num",toluaI_get_object_artifact_type_max_num,toluaI_set_object_artifact_type_max_num);
 tolua_tablevar(tolua_S,"artifact_type","known",toluaI_get_object_artifact_type_known,toluaI_set_object_artifact_type_known);
 tolua_tablevar(tolua_S,"artifact_type","power",toluaI_get_object_artifact_type_power,toluaI_set_object_artifact_type_power);
 tolua_tablevar(tolua_S,"artifact_type","set",toluaI_get_object_artifact_type_set,toluaI_set_object_artifact_type_set);
 tolua_tablevar(tolua_S,"artifact_type","set_pos",toluaI_get_object_artifact_type_set_pos,toluaI_set_object_artifact_type_set_pos);
 tolua_cclass(tolua_S,"ego_item_type","");
 tolua_tablevar(tolua_S,"ego_item_type","name",toluaI_get_object_ego_item_type_name,toluaI_set_object_ego_item_type_name);
 tolua_tablevar(tolua_S,"ego_item_type","text",toluaI_get_object_ego_item_type_text,toluaI_set_object_ego_item_type_text);
 tolua_tablevar(tolua_S,"ego_item_type","before",toluaI_get_object_ego_item_type_before,toluaI_set_object_ego_item_type_before);
 tolua_tablevar(tolua_S,"ego_item_type","ego_group",toluaI_get_object_ego_item_type_ego_group,toluaI_set_object_ego_item_type_ego_group);
 tolua_tablearray(tolua_S,"ego_item_type","tval",toluaI_get_object_ego_item_type_tval,toluaI_set_object_ego_item_type_tval);
 tolua_tablearray(tolua_S,"ego_item_type","min_sval",toluaI_get_object_ego_item_type_min_sval,toluaI_set_object_ego_item_type_min_sval);
 tolua_tablearray(tolua_S,"ego_item_type","max_sval",toluaI_get_object_ego_item_type_max_sval,toluaI_set_object_ego_item_type_max_sval);
 tolua_tablevar(tolua_S,"ego_item_type","item_kinds_ok",toluaI_get_object_ego_item_type_item_kinds_ok,toluaI_set_object_ego_item_type_item_kinds_ok);
 tolua_tablevar(tolua_S,"ego_item_type","rating",toluaI_get_object_ego_item_type_rating,toluaI_set_object_ego_item_type_rating);
 tolua_tablevar(tolua_S,"ego_item_type","level",toluaI_get_object_ego_item_type_level,toluaI_set_object_ego_item_type_level);
 tolua_tablevar(tolua_S,"ego_item_type","allocation",toluaI_get_object_ego_item_type_allocation,toluaI_set_object_ego_item_type_allocation);
 tolua_tablevar(tolua_S,"ego_item_type","max_to_h",toluaI_get_object_ego_item_type_max_to_h,toluaI_set_object_ego_item_type_max_to_h);
 tolua_tablevar(tolua_S,"ego_item_type","max_to_d",toluaI_get_object_ego_item_type_max_to_d,toluaI_set_object_ego_item_type_max_to_d);
 tolua_tablevar(tolua_S,"ego_item_type","max_to_a",toluaI_get_object_ego_item_type_max_to_a,toluaI_set_object_ego_item_type_max_to_a);
 tolua_tablevar(tolua_S,"ego_item_type","base_value_pct",toluaI_get_object_ego_item_type_base_value_pct,toluaI_set_object_ego_item_type_base_value_pct);
 tolua_tablevar(tolua_S,"ego_item_type","value_add",toluaI_get_object_ego_item_type_value_add,toluaI_set_object_ego_item_type_value_add);
 tolua_tablearray(tolua_S,"ego_item_type","rar",toluaI_get_object_ego_item_type_rar,toluaI_set_object_ego_item_type_rar);
 tolua_tablearray(tolua_S,"ego_item_type","flags",toluaI_get_object_ego_item_type_flags,toluaI_set_object_ego_item_type_flags);
 tolua_tablevar(tolua_S,"ego_item_type","need_flags",toluaI_get_object_ego_item_type_need_flags,toluaI_set_object_ego_item_type_need_flags);
 tolua_tablevar(tolua_S,"ego_item_type","forbid_flags",toluaI_get_object_ego_item_type_forbid_flags,toluaI_set_object_ego_item_type_forbid_flags);
 tolua_tablevar(tolua_S,"ego_item_type","power",toluaI_get_object_ego_item_type_power,toluaI_set_object_ego_item_type_power);
 tolua_cclass(tolua_S,"object_type","");
 tolua_tablevar(tolua_S,"object_type","k_idx",toluaI_get_object_object_type_k_idx,toluaI_set_object_object_type_k_idx);
 tolua_tablevar(tolua_S,"object_type","iy",toluaI_get_object_object_type_iy,toluaI_set_object_object_type_iy);
 tolua_tablevar(tolua_S,"object_type","ix",toluaI_get_object_object_type_ix,toluaI_set_object_object_type_ix);
 tolua_tablevar(tolua_S,"object_type","tval",toluaI_get_object_object_type_tval,toluaI_set_object_object_type_tval);
 tolua_tablevar(tolua_S,"object_type","sval",toluaI_get_object_object_type_sval,toluaI_set_object_object_type_sval);
 tolua_tablevar(tolua_S,"object_type","discount",toluaI_get_object_object_type_discount,toluaI_set_object_object_type_discount);
 tolua_tablevar(tolua_S,"object_type","number",toluaI_get_object_object_type_number,toluaI_set_object_object_type_number);
 tolua_tablevar(tolua_S,"object_type","weight",toluaI_get_object_object_type_weight,toluaI_set_object_object_type_weight);
 tolua_tablevar(tolua_S,"object_type","elevel",toluaI_get_object_object_type_elevel,toluaI_set_object_object_type_elevel);
 tolua_tablevar(tolua_S,"object_type","exp",toluaI_get_object_object_type_exp,toluaI_set_object_object_type_exp);
 tolua_tablevar(tolua_S,"object_type","d_attr",toluaI_get_object_object_type_d_attr,toluaI_set_object_object_type_d_attr);
 tolua_tablevar(tolua_S,"object_type","x_attr",toluaI_get_object_object_type_x_attr,toluaI_set_object_object_type_x_attr);
 tolua_tablevar(tolua_S,"object_type","artifact_id",toluaI_get_object_object_type_artifact_id,toluaI_set_object_object_type_artifact_id);
 tolua_tablearray(tolua_S,"object_type","ego_id",toluaI_get_object_object_type_ego_id,toluaI_set_object_object_type_ego_id);
 tolua_tablevar(tolua_S,"object_type","xtra1",toluaI_get_object_object_type_xtra1,toluaI_set_object_object_type_xtra1);
 tolua_tablevar(tolua_S,"object_type","xtra2",toluaI_get_object_object_type_xtra2,toluaI_set_object_object_type_xtra2);
 tolua_tablevar(tolua_S,"object_type","to_h",toluaI_get_object_object_type_to_h,toluaI_set_object_object_type_to_h);
 tolua_tablevar(tolua_S,"object_type","to_d",toluaI_get_object_object_type_to_d,toluaI_set_object_object_type_to_d);
 tolua_tablevar(tolua_S,"object_type","to_a",toluaI_get_object_object_type_to_a,toluaI_set_object_object_type_to_a);
 tolua_tablevar(tolua_S,"object_type","ac",toluaI_get_object_object_type_ac,toluaI_set_object_object_type_ac);
 tolua_tablevar(tolua_S,"object_type","dd",toluaI_get_object_object_type_dd,toluaI_set_object_object_type_dd);
 tolua_tablevar(tolua_S,"object_type","ds",toluaI_get_object_object_type_ds,toluaI_set_object_object_type_ds);
 tolua_tablevar(tolua_S,"object_type","timeout",toluaI_get_object_object_type_timeout,toluaI_set_object_object_type_timeout);
 tolua_tablevar(tolua_S,"object_type","ident",toluaI_get_object_object_type_ident,toluaI_set_object_object_type_ident);
 tolua_tablevar(tolua_S,"object_type","marked",toluaI_get_object_object_type_marked,toluaI_set_object_object_type_marked);
 tolua_tablevar(tolua_S,"object_type","note",toluaI_get_object_object_type_note,toluaI_set_object_object_type_note);
 tolua_tablevar(tolua_S,"object_type","art_name",toluaI_get_object_object_type_art_name,toluaI_set_object_object_type_art_name);
 tolua_tablevar(tolua_S,"object_type","flags",toluaI_get_object_object_type_flags,toluaI_set_object_object_type_flags);
 tolua_tablevar(tolua_S,"object_type","next_o_idx",toluaI_get_object_object_type_next_o_idx,toluaI_set_object_object_type_next_o_idx);
 tolua_tablevar(tolua_S,"object_type","held_m_idx",toluaI_get_object_object_type_held_m_idx,toluaI_set_object_object_type_held_m_idx);
 tolua_tablevar(tolua_S,"object_type","sense",toluaI_get_object_object_type_sense,toluaI_set_object_object_type_sense);
 tolua_tablevar(tolua_S,"object_type","found",toluaI_get_object_object_type_found,toluaI_set_object_object_type_found);
 tolua_tablevar(tolua_S,"object_type","found_aux1",toluaI_get_object_object_type_found_aux1,toluaI_set_object_object_type_found_aux1);
 tolua_tablevar(tolua_S,"object_type","found_aux2",toluaI_get_object_object_type_found_aux2,toluaI_set_object_object_type_found_aux2);
 tolua_tablevar(tolua_S,"object_type","found_aux3",toluaI_get_object_object_type_found_aux3,toluaI_set_object_object_type_found_aux3);
 tolua_tablevar(tolua_S,"object_type","found_aux4",toluaI_get_object_object_type_found_aux4,toluaI_set_object_object_type_found_aux4);
 tolua_cclass(tolua_S,"__set_type_aux","");
 tolua_tablevar(tolua_S,"__set_type_aux","present",toluaI_get_object___set_type_aux_present,toluaI_set_object___set_type_aux_present);
 tolua_tablevar(tolua_S,"__set_type_aux","a_idx",toluaI_get_object___set_type_aux_a_idx,toluaI_set_object___set_type_aux_a_idx);
 tolua_tablearray(tolua_S,"__set_type_aux","flags",toluaI_get_object___set_type_aux_flags,toluaI_set_object___set_type_aux_flags);
 tolua_cclass(tolua_S,"set_type","");
 tolua_tablevar(tolua_S,"set_type","name",toluaI_get_object_set_type_name,toluaI_set_object_set_type_name);
 tolua_tablevar(tolua_S,"set_type","desc",toluaI_get_object_set_type_desc,toluaI_set_object_set_type_desc);
 tolua_tablevar(tolua_S,"set_type","num",toluaI_get_object_set_type_num,toluaI_set_object_set_type_num);
 tolua_tablevar(tolua_S,"set_type","num_use",toluaI_get_object_set_type_num_use,toluaI_set_object_set_type_num_use);
 tolua_tablearray(tolua_S,"set_type","arts",toluaI_get_object_set_type_arts,toluaI_set_object_set_type_arts);
 tolua_tablevar(tolua_S,"set_type","load_script",toluaI_get_object_set_type_load_script,toluaI_set_object_set_type_load_script);
 tolua_constant(tolua_S,NULL,"SENSE_NONE",SENSE_NONE);
 tolua_constant(tolua_S,NULL,"SENSE_CURSED",SENSE_CURSED);
 tolua_constant(tolua_S,NULL,"SENSE_AVERAGE",SENSE_AVERAGE);
 tolua_constant(tolua_S,NULL,"SENSE_GOOD_LIGHT",SENSE_GOOD_LIGHT);
 tolua_constant(tolua_S,NULL,"SENSE_GOOD_HEAVY",SENSE_GOOD_HEAVY);
 tolua_constant(tolua_S,NULL,"SENSE_EXCELLENT",SENSE_EXCELLENT);
 tolua_constant(tolua_S,NULL,"SENSE_WORTHLESS",SENSE_WORTHLESS);
 tolua_constant(tolua_S,NULL,"SENSE_TERRIBLE",SENSE_TERRIBLE);
 tolua_constant(tolua_S,NULL,"SENSE_SPECIAL",SENSE_SPECIAL);
 tolua_constant(tolua_S,NULL,"SENSE_BROKEN",SENSE_BROKEN);
 tolua_constant(tolua_S,NULL,"SENSE_UNCURSED",SENSE_UNCURSED);
 tolua_globalvar(tolua_S,"max_k_idx",toluaI_get_object_max_k_idx,toluaI_set_object_max_k_idx);
 tolua_globalvar(tolua_S,"max_a_idx",toluaI_get_object_max_a_idx,toluaI_set_object_max_a_idx);
 tolua_globalvar(tolua_S,"max_e_idx",toluaI_get_object_max_e_idx,toluaI_set_object_max_e_idx);
 tolua_globalvar(tolua_S,"max_ra_idx",toluaI_get_object_max_ra_idx,toluaI_set_object_max_ra_idx);
 tolua_globalvar(tolua_S,"max_set_idx",toluaI_get_object_max_set_idx,toluaI_set_object_max_set_idx);
 tolua_globalarray(tolua_S,"k_info",toluaI_get_object_k_info,toluaI_set_object_k_info);
 tolua_globalarray(tolua_S,"a_info",toluaI_get_object_a_info,toluaI_set_object_a_info);
 tolua_globalarray(tolua_S,"e_info",toluaI_get_object_e_info,toluaI_set_object_e_info);
 tolua_globalarray(tolua_S,"set_info",toluaI_get_object_set_info,toluaI_set_object_set_info);
 tolua_globalvar(tolua_S,"object_level",toluaI_get_object_object_level,toluaI_set_object_object_level);
 tolua_function(tolua_S,NULL,"m_bonus",toluaI_object_m_bonus00);
 tolua_function(tolua_S,NULL,"wield_slot_ideal",toluaI_object_wield_slot_ideal00);
 tolua_function(tolua_S,NULL,"wield_slot",toluaI_object_wield_slot00);
 tolua_function(tolua_S,NULL,"object_desc_store",toluaI_object_object_desc_store00);
 tolua_function(tolua_S,NULL,"item_describe",toluaI_object_item_describe00);
 tolua_function(tolua_S,NULL,"item_increase",toluaI_object_item_increase00);
 tolua_function(tolua_S,NULL,"item_optimize",toluaI_object_item_optimize00);
 tolua_function(tolua_S,NULL,"delete_object_idx",toluaI_object_delete_object_idx00);
 tolua_function(tolua_S,NULL,"ident_all",toluaI_object_ident_all00);
 tolua_function(tolua_S,NULL,"lookup_kind",toluaI_object_lookup_kind00);
 tolua_function(tolua_S,NULL,"object_wipe",toluaI_object_object_wipe00);
 tolua_function(tolua_S,NULL,"object_prep",toluaI_object_object_prep00);
 tolua_function(tolua_S,NULL,"object_copy",toluaI_object_object_copy00);
 tolua_function(tolua_S,NULL,"inven_carry_okay",toluaI_object_inven_carry_okay00);
 tolua_function(tolua_S,NULL,"apply_magic",toluaI_object_apply_magic00);
 tolua_function(tolua_S,NULL,"place_object",toluaI_object_place_object00);
 tolua_function(tolua_S,NULL,"drop_near",toluaI_object_drop_near00);
 tolua_function(tolua_S,NULL,"acquirement",toluaI_object_acquirement00);
 tolua_function(tolua_S,NULL,"get_object",toluaI_object_get_object00);
 tolua_function(tolua_S,NULL,"set_object",toluaI_object_set_object00);
 tolua_function(tolua_S,NULL,"new_object",toluaI_object_new_object00);
 tolua_function(tolua_S,NULL,"object_dup",toluaI_object_object_dup00);
 tolua_function(tolua_S,NULL,"delete_object",toluaI_object_delete_object00);
 tolua_function(tolua_S,NULL,"item_tester_okay",toluaI_object_item_tester_okay00);
 tolua_function(tolua_S,NULL,"get_item_aux",toluaI_object_get_item_aux00);
 tolua_function(tolua_S,NULL,"lua_set_item_tester",toluaI_object_lua_set_item_tester00);
 tolua_function(tolua_S,NULL,"make_item_fully_identified",toluaI_object_make_item_fully_identified00);
 tolua_function(tolua_S,NULL,"identify_pack_fully",toluaI_object_identify_pack_fully00);
 tolua_function(tolua_S,NULL,"inven_carry_inven",toluaI_object_inven_carry_inven00);
 tolua_function(tolua_S,NULL,"calc_total_weight",toluaI_object_calc_total_weight00);
 tolua_function(tolua_S,NULL,"get_slot",toluaI_object_get_slot00);
 tolua_globalarray(tolua_S,"sense_desc",toluaI_get_object_sense_desc,toluaI_set_object_sense_desc);
 tolua_function(tolua_S,NULL,"object_pickup",toluaI_object_object_pickup00);
 tolua_function(tolua_S,NULL,"inven_takeoff",toluaI_object_inven_takeoff00);
 tolua_function(tolua_S,NULL,"inven_drop",toluaI_object_inven_drop00);
 tolua_function(tolua_S,NULL,"make_gold",toluaI_object_make_gold00);
 tolua_function(tolua_S,NULL,"place_gold",toluaI_object_place_gold00);
 tolua_function(tolua_S,NULL,"object_out_desc",toluaI_object_object_out_desc00);
 tolua_function(tolua_S,NULL,"is_artifact",toluaI_object_is_artifact00);
 tolua_function(tolua_S,NULL,"is_ego",toluaI_object_is_ego00);
 tolua_function(tolua_S,NULL,"is_known",toluaI_object_is_known00);
 tolua_function(tolua_S,NULL,"set_known",toluaI_object_set_known00);
 tolua_function(tolua_S,NULL,"is_cursed",toluaI_object_is_cursed00);
 tolua_function(tolua_S,NULL,"object_similar",toluaI_object_object_similar00);
 tolua_function(tolua_S,NULL,"object_absorb",toluaI_object_object_absorb00);
 tolua_function(tolua_S,NULL,"object_unabsorb",toluaI_object_object_unabsorb00);
 tolua_function(tolua_S,NULL,"value_check_aux1",toluaI_object_value_check_aux100);
 tolua_function(tolua_S,NULL,"value_check_aux1_magic",toluaI_object_value_check_aux1_magic00);
 tolua_function(tolua_S,NULL,"value_check_aux2",toluaI_object_value_check_aux200);
 tolua_function(tolua_S,NULL,"value_check_aux2_magic",toluaI_object_value_check_aux2_magic00);
 tolua_function(tolua_S,NULL,"select_sense",toluaI_object_select_sense00);
 tolua_function(tolua_S,NULL,"remove_curse_object",toluaI_object_remove_curse_object00);
 tolua_function(tolua_S,NULL,"object_track",toluaI_object_object_track00);
 tolua_function(tolua_S,NULL,"get_item_extra_hook",toluaI_object_get_item_extra_hook00);
 tolua_function(tolua_S,NULL,"get_item_allow",toluaI_object_get_item_allow00);
 tolua_function(tolua_S,NULL,"get_tag",toluaI_object_get_tag00);
 tolua_function(tolua_S,NULL,"object_value",toluaI_object_object_value00);
 tolua_function(tolua_S,NULL,"object_value_real",toluaI_object_object_value_real00);
 tolua_globalvar(tolua_S,"get_item_hook_find_obj_what",toluaI_get_object_get_item_hook_find_obj_what,toluaI_set_object_get_item_hook_find_obj_what);
 tolua_function(tolua_S,NULL,"get_item_hook_find_obj",toluaI_object_get_item_hook_find_obj00);
 tolua_globalarray(tolua_S,"tval_to_attr",toluaI_get_object_tval_to_attr,toluaI_set_object_tval_to_attr);
 tolua_cclass(tolua_S,"inventory_slot_type","");
 tolua_tablevar(tolua_S,"inventory_slot_type","desc",toluaI_get_object_inventory_slot_type_desc,toluaI_set_object_inventory_slot_type_desc);
 tolua_tablevar(tolua_S,"inventory_slot_type","name",toluaI_get_object_inventory_slot_type_name,toluaI_set_object_inventory_slot_type_name);
 tolua_tablevar(tolua_S,"inventory_slot_type","describe",toluaI_get_object_inventory_slot_type_describe,toluaI_set_object_inventory_slot_type_describe);
 tolua_tablevar(tolua_S,"inventory_slot_type","wear",toluaI_get_object_inventory_slot_type_wear,toluaI_set_object_inventory_slot_type_wear);
 tolua_tablevar(tolua_S,"inventory_slot_type","takeoff",toluaI_get_object_inventory_slot_type_takeoff,toluaI_set_object_inventory_slot_type_takeoff);
 tolua_globalarray(tolua_S,"inventory_slot_names",toluaI_get_object_inventory_slot_names,toluaI_set_object_inventory_slot_names);
 tolua_globalvar(tolua_S,"INVEN_PACK",toluaI_get_object_INVEN_PACK,toluaI_set_object_INVEN_PACK);
 tolua_globalvar(tolua_S,"INVEN_TOTAL",toluaI_get_object_INVEN_TOTAL,toluaI_set_object_INVEN_TOTAL);
 tolua_globalvar(tolua_S,"repair_objects",toluaI_get_object_repair_objects,toluaI_set_object_repair_objects);
 tolua_function(tolua_S,NULL,"mention_use",toluaI_object_mention_use00);
 tolua_function(tolua_S,NULL,"describe_use",toluaI_object_describe_use00);
 tolua_function(tolua_S,NULL,"preserve_artifacts_in_inven",toluaI_object_preserve_artifacts_in_inven00);
 return 1;
}
/* Close function */
void tolua_object_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"USE_EQUIP");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"USE_INVEN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"USE_FLOOR");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"USE_EXTRA");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"USE_AUTO");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"USE_DISPLAY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"USE_SHOW_NO_ITEM");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"INVEN_INVEN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"INVEN_MAX_SIZE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"INVEN_LIMIT_KEY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TV_CORPSE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SV_CORPSE_CORPSE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SV_CORPSE_SKELETON");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SV_CORPSE_HEAD");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SV_CORPSE_SKULL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SV_CORPSE_MEAT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"IDENT_SENSE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"IDENT_FIXED");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"IDENT_EMPTY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"IDENT_KNOWN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"IDENT_STOREB");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"IDENT_MENTAL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"IDENT_CURSED");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_MONSTER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_FLOOR");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_VAULT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_SPECIAL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_RUBBLE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_REWARD");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_STORE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_STOLEN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_SELFMADE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_WISH");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_DEBUG");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"OBJ_FOUND_START");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MAX_KIND_ALLOCATIONS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MAX_TVALS_PER_EGO");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MAX_EGO_FLAG_GROUPS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"MAX_EGO_PER_OBJ");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_kind");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"artifact_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ego_item_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"__set_type_aux");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"set_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_NONE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_CURSED");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_AVERAGE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_GOOD_LIGHT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_GOOD_HEAVY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_EXCELLENT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_WORTHLESS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_TERRIBLE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_SPECIAL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_BROKEN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"SENSE_UNCURSED");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_k_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_a_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_e_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_ra_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_set_idx"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"k_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"a_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"e_info");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"set_info");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"object_level"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"m_bonus");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wield_slot_ideal");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"wield_slot");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_desc_store");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"item_describe");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"item_increase");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"item_optimize");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"delete_object_idx");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ident_all");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lookup_kind");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_wipe");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_prep");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_copy");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"inven_carry_okay");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"apply_magic");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_object");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"drop_near");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"acquirement");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_object");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"set_object");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"new_object");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_dup");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"delete_object");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"item_tester_okay");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_item_aux");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lua_set_item_tester");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"make_item_fully_identified");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"identify_pack_fully");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"inven_carry_inven");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"calc_total_weight");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_slot");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"sense_desc");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_pickup");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"inven_takeoff");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"inven_drop");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"make_gold");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_gold");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_out_desc");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_artifact");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_ego");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_known");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"set_known");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_cursed");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_similar");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_absorb");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_unabsorb");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"value_check_aux1");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"value_check_aux1_magic");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"value_check_aux2");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"value_check_aux2_magic");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"select_sense");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"remove_curse_object");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_track");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_item_extra_hook");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_item_allow");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_tag");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_value");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"object_value_real");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"get_item_hook_find_obj_what"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_item_hook_find_obj");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"tval_to_attr");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"inventory_slot_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"inventory_slot_names");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"INVEN_PACK"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"INVEN_TOTAL"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"repair_objects"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"mention_use");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"describe_use");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"preserve_artifacts_in_inven");
}
