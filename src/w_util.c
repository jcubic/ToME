/*
** Lua binding: util
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:53 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_util_open (lua_State* tolua_S);
void tolua_util_close (lua_State* tolua_S);

#include "angband.h"
#include "SDL.h"
#include "sdl-extern.h"
#include "plots.h"
static hooks_chain* hook_get(s32b type) { return hooks_heads[type]; }
static char *path_build_lua(cptr path, cptr file){static char buf[1025]; path_build(buf, 1024, path, file); return buf;}
static const char *player_name_lua(cptr new_name){ if (new_name) { strcpy(player_name, new_name); process_player_name(FALSE); } return (const char *)player_name; }
static byte flag_node_get_ptr_type(flag_node *node) { return node->vals.ptr.type; }
static bool lua_cave_clean_bold(s32b y, s32b x) { return (cave_clean_bold(y, x)) ? TRUE : FALSE; }
static bool lua_cave_floor_bold(s32b y, s32b x) { return (cave_floor_bold(y, x)) ? TRUE : FALSE; }
static bool lua_player_has_los_bold(s32b y, s32b x) { return (player_has_los_bold(y, x)) ? TRUE : FALSE; }
static bool lua_panel_contains(s32b y, s32b x) { return panel_contains(y, x); }
static bool lua_cave_empty_bold(s32b y, s32b x) { return (cave_empty_bold(y, x)) ? TRUE : FALSE; }
static char *lua_get_rnd_line(char *file_name) { static char buf[400]; return (get_rnd_line(file_name, buf) == 0) ? buf : ""; }
static char *lua_get_line(char* fname, cptr fdir, s32b line) { static char buf[400]; return (get_line(fname, fdir, buf, line) != NULL) ? buf : ""; }
static char *lua_grab_string(string_line source, s32b offset) { return source + offset; }
static bool lua_in_bounds(s32b y, s32b x) {return in_bounds(y,x);}
static bool lua_in_bounds2(s32b y, s32b x) {return in_bounds2(y,x);}
static cptr __wrap_ptr_to_string(void *ptr) { return (cptr)ptr; }
static object_type *__wrap_ptr_to_object(void *ptr) { return (object_type*)ptr; }
static monster_blow *__wrap_ptr_to_monster_blow(void *ptr) { return (monster_blow*)ptr; }
static monster_type *__wrap_ptr_to_monster_type(void *ptr) { return (monster_type*)ptr; }
static store_type *__wrap_ptr_to_store_type(void *ptr) { return (store_type*)ptr; }
static buffer_type *__wrap_ptr_to_buffer(void *ptr) { return (buffer_type*)ptr; }
static void* __wrap_ptr_dup_buffer_to_void(void *ptr) { return (void*)data_buffer_dup((buffer_type*)ptr); }
static void* __wrap_ptr_dup_string_to_void(cptr ptr) { return (void*)string_make(ptr); }
static s32b negate_bits(s32b i) { return ~i; }
static cptr savefile_lua(cptr save) { if (save != NULL) strncpy(savefile, save, 1023); return savefile; }
extern SDL_TimerID rt_timer_add(s32b interval, void *c_callback);
extern void rt_timer_del(SDL_TimerID id);

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"monster_type");
 tolua_usertype(tolua_S,"cave_type");
 tolua_usertype(tolua_S,"buffer_type");
 tolua_usertype(tolua_S,"flags_holder_type");
 tolua_usertype(tolua_S,"flag_registry");
 tolua_usertype(tolua_S,"term_win");
 tolua_usertype(tolua_S,"hooks_chain");
 tolua_usertype(tolua_S,"object_type");
 tolua_usertype(tolua_S,"SDL_TimerID");
 tolua_usertype(tolua_S,"flags_type");
 tolua_usertype(tolua_S,"list_type");
 tolua_usertype(tolua_S,"string_line");
 tolua_usertype(tolua_S,"monster_blow");
 tolua_usertype(tolua_S,"store_type");
 tolua_usertype(tolua_S,"flag_node");
 tolua_usertype(tolua_S,"timer_type");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* get function: can_redefine_color */
static int toluaI_get_util_color_can_redefine_color(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)can_redefine_color);
 return 1;
}

/* set function: can_redefine_color */
static int toluaI_set_util_color_can_redefine_color(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  can_redefine_color = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: init_tome_color */
static int toluaI_util_color_init_tome_color00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  byte id = ((byte)  tolua_getnumber(tolua_S,1,0));
  cptr name = ((cptr)  tolua_getstring(tolua_S,2,0));
  char chr = ((char)  tolua_getnumber(tolua_S,3,0));
  s32b r = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b g = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b b = ((s32b)  tolua_getnumber(tolua_S,6,0));
 {
  init_tome_color(id,name,chr,r,g,b);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init_tome_color'.");
 return 0;
}

/* get function: conv_color */
static int toluaI_get_util_conv_color(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=16)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)conv_color[toluaI_index]);
 return 1;
}

/* set function: conv_color */
static int toluaI_set_util_conv_color(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=16)
 tolua_error(tolua_S,"array indexing out of range.");
  conv_color[toluaI_index] = ((byte)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: process_hooks_restart */
static int toluaI_get_util_hook_restart(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)process_hooks_restart);
 return 1;
}

/* set function: process_hooks_restart */
static int toluaI_set_util_hook_restart(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  process_hooks_restart = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: dump_hooks */
static int toluaI_util_hook_dump00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b h_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  dump_hooks(h_idx);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dump'.");
 return 0;
}

/* function: add_hook_script */
static int toluaI_util_hook_add_script00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  s32b h_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  char* script = ((char*)  tolua_getstring(tolua_S,2,0));
  cptr name = ((cptr)  tolua_getstring(tolua_S,3,0));
 {
  add_hook_script(h_idx,script,name);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_script'.");
 return 0;
}

/* function: del_hook_name */
static int toluaI_util_hook_del_by_name00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b h_idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
  cptr name = ((cptr)  tolua_getstring(tolua_S,2,0));
 {
  del_hook_name(h_idx,name);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_by_name'.");
 return 0;
}

/* function: lua_print_hook */
static int toluaI_util_hook_print00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr str = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  lua_print_hook(str);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'print'.");
 return 0;
}

/* function: hook_get */
static int toluaI_util_hook___get00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b type = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  hooks_chain* toluaI_ret = (hooks_chain*)  hook_get(type);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"hooks_chain"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__get'.");
 return 0;
}

/* get function: name of class  hooks_chain */
static int toluaI_get_util_hooks_chain_name(lua_State* tolua_S)
{
  hooks_chain* self = (hooks_chain*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  hooks_chain */
static int toluaI_set_util_hooks_chain_name(lua_State* tolua_S)
{
  hooks_chain* self = (hooks_chain*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: script of class  hooks_chain */
static int toluaI_get_util_hooks_chain_script(lua_State* tolua_S)
{
  hooks_chain* self = (hooks_chain*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->script);
 return 1;
}

/* set function: script of class  hooks_chain */
static int toluaI_set_util_hooks_chain_script(lua_State* tolua_S)
{
  hooks_chain* self = (hooks_chain*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->script = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: type of class  hooks_chain */
static int toluaI_get_util_hooks_chain_type(lua_State* tolua_S)
{
  hooks_chain* self = (hooks_chain*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->type);
 return 1;
}

/* set function: type of class  hooks_chain */
static int toluaI_set_util_hooks_chain_type(lua_State* tolua_S)
{
  hooks_chain* self = (hooks_chain*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->type = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: next of class  hooks_chain */
static int toluaI_get_util_hooks_chain_next(lua_State* tolua_S)
{
  hooks_chain* self = (hooks_chain*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)self->next,tolua_tag(tolua_S,"hooks_chain"));
 return 1;
}

/* set function: next of class  hooks_chain */
static int toluaI_set_util_hooks_chain_next(lua_State* tolua_S)
{
  hooks_chain* self = (hooks_chain*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"hooks_chain"),0))
 TOLUA_ERR_ASSIGN;
  self->next = ((hooks_chain*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: turn */
static int toluaI_get_util_turn(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)turn);
 return 1;
}

/* set function: turn */
static int toluaI_set_util_turn(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  turn = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: old_turn */
static int toluaI_get_util_old_turn(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)old_turn);
 return 1;
}

/* set function: old_turn */
static int toluaI_set_util_old_turn(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  old_turn = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: cur_wid */
static int toluaI_get_util_cur_wid(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)cur_wid);
 return 1;
}

/* set function: cur_wid */
static int toluaI_set_util_cur_wid(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  cur_wid = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: cur_hgt */
static int toluaI_get_util_cur_hgt(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)cur_hgt);
 return 1;
}

/* set function: cur_hgt */
static int toluaI_set_util_cur_hgt(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  cur_hgt = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: path_build_lua */
static int toluaI_util_path_build00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cptr path = ((cptr)  tolua_getstring(tolua_S,1,0));
  cptr file = ((cptr)  tolua_getstring(tolua_S,2,0));
 {
  char* toluaI_ret = (char*)  path_build_lua(path,file);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'path_build'.");
 return 0;
}

/* function: flush */
static int toluaI_util_message_flush00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  flush();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flush'.");
 return 0;
}

/* function: cmsg_print */
static int toluaI_util_message_cmsg_print00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  byte color = ((byte)  tolua_getnumber(tolua_S,1,0));
  cptr msg = ((cptr)  tolua_getstring(tolua_S,2,NULL));
 {
  cmsg_print(color,msg);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cmsg_print'.");
 return 0;
}

/* function: msg_print */
static int toluaI_util_message_msg_print00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr msg = ((cptr)  tolua_getstring(tolua_S,1,NULL));
 {
  msg_print(msg);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'msg_print'.");
 return 0;
}

/* function: message_add */
static int toluaI_util_message_add00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  byte type = ((byte)  tolua_getnumber(tolua_S,1,0));
  cptr msg = ((cptr)  tolua_getstring(tolua_S,2,0));
  byte color = ((byte)  tolua_getnumber(tolua_S,3,0));
 {
  message_add(type,msg,color);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add'.");
 return 0;
}

/* function: display_message */
static int toluaI_util_message_display00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  s32b x = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b split = ((s32b)  tolua_getnumber(tolua_S,3,0));
  byte color = ((byte)  tolua_getnumber(tolua_S,4,0));
  cptr t = ((cptr)  tolua_getstring(tolua_S,5,0));
 {
  display_message(x,y,split,color,t);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'display'.");
 return 0;
}

/* function: disturb */
static int toluaI_util_term_disturb00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b stop_search = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b flush_output = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  disturb(stop_search,flush_output);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'disturb'.");
 return 0;
}

/* function: move_cursor_relative */
static int toluaI_util_term_move_cursor_relative00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b row = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b col = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  move_cursor_relative(row,col);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'move_cursor_relative'.");
 return 0;
}

/* get function: inkey_scan */
static int toluaI_get_util_term_inkey_scan(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)inkey_scan);
 return 1;
}

/* set function: inkey_scan */
static int toluaI_set_util_term_inkey_scan(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  inkey_scan = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: inkey_dir */
static int toluaI_get_util_term_inkey_dir(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)inkey_dir);
 return 1;
}

/* set function: inkey_dir */
static int toluaI_set_util_term_inkey_dir(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  inkey_dir = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: inkey */
static int toluaI_util_term_inkey00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  s32b toluaI_ret = (s32b)  inkey();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'inkey'.");
 return 0;
}

/* function: Term_keypress */
static int toluaI_util_term_outkey00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b k = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  errr toluaI_ret = (errr)  Term_keypress(k);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'outkey'.");
 return 0;
}

/* function: screen_depth */
static int toluaI_util_term_depth00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  s32b toluaI_ret = (s32b)  screen_depth();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'depth'.");
 return 0;
}

/* function: screen_save */
static int toluaI_util_term_save00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  screen_save();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save'.");
 return 0;
}

/* function: screen_load */
static int toluaI_util_term_load00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  screen_load();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load'.");
 return 0;
}

/* function: Term_save_to */
static int toluaI_util_term_save_to00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  term_win* toluaI_ret = (term_win*)  Term_save_to();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"term_win"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_to'.");
 return 0;
}

/* function: Term_load_from */
static int toluaI_util_term_load_from00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"term_win"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  term_win* save = ((term_win*)  tolua_getusertype(tolua_S,1,0));
  bool final = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  errr toluaI_ret = (errr)  Term_load_from(save,final);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_from'.");
 return 0;
}

/* function: c_put_str */
static int toluaI_util_term_c_put_str00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  byte attr = ((byte)  tolua_getnumber(tolua_S,1,0));
  cptr str = ((cptr)  tolua_getstring(tolua_S,2,0));
  s32b row = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b col = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  c_put_str(attr,str,row,col);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_put_str'.");
 return 0;
}

/* function: put_str */
static int toluaI_util_term_put_str00(lua_State* tolua_S)
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
  cptr str = ((cptr)  tolua_getstring(tolua_S,1,0));
  s32b row = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b col = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  put_str(str,row,col);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'put_str'.");
 return 0;
}

/* function: c_prt */
static int toluaI_util_term_c_prt00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  byte attr = ((byte)  tolua_getnumber(tolua_S,1,0));
  cptr str = ((cptr)  tolua_getstring(tolua_S,2,0));
  s32b row = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b col = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  c_prt(attr,str,row,col);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_prt'.");
 return 0;
}

/* function: prt */
static int toluaI_util_term_prt00(lua_State* tolua_S)
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
  cptr str = ((cptr)  tolua_getstring(tolua_S,1,0));
  s32b row = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b col = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  prt(str,row,col);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'prt'.");
 return 0;
}

/* function: clear_from */
static int toluaI_util_term_clear_from00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b row = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  clear_from(row);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_from'.");
 return 0;
}

/* function: text_out_c */
static int toluaI_util_term_text_out_c00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  byte a = ((byte)  tolua_getnumber(tolua_S,1,0));
  cptr str = ((cptr)  tolua_getstring(tolua_S,2,0));
 {
  text_out_c(a,str);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'text_out_c'.");
 return 0;
}

/* function: darken_screen */
static int toluaI_util_term_screen_color00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  byte color = ((byte)  tolua_getnumber(tolua_S,1,0));
 {
  darken_screen(color);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'screen_color'.");
 return 0;
}

/* function: lua_apply_keymap */
static int toluaI_util_term_apply_keymap00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b ch = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  cptr toluaI_ret = (cptr)  lua_apply_keymap(ch);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'apply_keymap'.");
 return 0;
}

/* function: get_check_aux */
static int toluaI_util_get_check00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cptr prompt = ((cptr)  tolua_getstring(tolua_S,1,0));
  bool keep_cursor = ((bool)  tolua_getbool(tolua_S,2,FALSE));
 {
  bool toluaI_ret = (bool)  get_check_aux(prompt,keep_cursor);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_check'.");
 return 0;
}

/* function: get_com_lua */
static int toluaI_util_get_com00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  cptr prompt = ((cptr)  tolua_getstring(tolua_S,1,0));
  s32b com = ((s32b)  tolua_getnumber(tolua_S,2,0));
  bool beel_cursor = ((bool)  tolua_getbool(tolua_S,3,FALSE));
 {
  bool toluaI_ret = (bool)  get_com_lua(prompt,&com,beel_cursor);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)com);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_com'.");
 return 0;
}

/* function: get_quantity */
static int toluaI_util_get_quantity00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cptr prompt = ((cptr)  tolua_getstring(tolua_S,1,0));
  s32b max = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  get_quantity(prompt,max);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_quantity'.");
 return 0;
}

/* function: test_monster_name */
static int toluaI_util_test_monster_name00(lua_State* tolua_S)
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
  s32b toluaI_ret = (s32b)  test_monster_name(name);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'test_monster_name'.");
 return 0;
}

/* function: test_item_name */
static int toluaI_util_test_item_name00(lua_State* tolua_S)
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
  s32b toluaI_ret = (s32b)  test_item_name(name);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'test_item_name'.");
 return 0;
}

/* function: luck */
static int toluaI_util_luck00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b min = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b max = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  luck(min,max);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'luck'.");
 return 0;
}

/* function: value_scale */
static int toluaI_util_value_scale00(lua_State* tolua_S)
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
  s32b value = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b vmax = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b max = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b min = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  s32b toluaI_ret = (s32b)  value_scale(value,vmax,max,min);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'value_scale'.");
 return 0;
}

/* function: quit */
static int toluaI_util_quit00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr str = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  quit(str);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'quit'.");
 return 0;
}

/* function: tome_dofile */
static int toluaI_util_tome_dofile00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  char* file = ((char*)  tolua_getstring(tolua_S,1,0));
  bool bypass_redirection = ((bool)  tolua_getbool(tolua_S,2,FALSE));
 {
  bool toluaI_ret = (bool)  tome_dofile(file,bypass_redirection);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tome_dofile'.");
 return 0;
}

/* function: tome_dofile_anywhere */
static int toluaI_util_tome_dofile_anywhere00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  cptr dir = ((cptr)  tolua_getstring(tolua_S,1,0));
  char* file = ((char*)  tolua_getstring(tolua_S,2,0));
  bool test_exist = ((bool)  tolua_getbool(tolua_S,3,TRUE));
  bool bypass_redirection = ((bool)  tolua_getbool(tolua_S,4,FALSE));
 {
  bool toluaI_ret = (bool)  tome_dofile_anywhere(dir,file,test_exist,bypass_redirection);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'tome_dofile_anywhere'.");
 return 0;
}

/* function: dump_lua_stack */
static int toluaI_util_dump_lua_stack00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b min = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b max = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  dump_lua_stack(min,max);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dump_lua_stack'.");
 return 0;
}

/* function: register_savefile */
static int toluaI_util_register_savefile00(lua_State* tolua_S)
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
  register_savefile(num);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'register_savefile'.");
 return 0;
}

/* function: save_nil_key */
static int toluaI_util_save_nil_key00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  char* key = ((char*)  tolua_getstring(tolua_S,1,0));
 {
  save_nil_key(key);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_nil_key'.");
 return 0;
}

/* function: save_number_key */
static int toluaI_util_save_number_key00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  char* key = ((char*)  tolua_getstring(tolua_S,1,0));
  s32b val = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  save_number_key(key,val);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_number_key'.");
 return 0;
}

/* function: save_string_key */
static int toluaI_util_save_string_key00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  char* key = ((char*)  tolua_getstring(tolua_S,1,0));
  cptr val = ((cptr)  tolua_getstring(tolua_S,2,0));
 {
  save_string_key(key,val);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_string_key'.");
 return 0;
}

/* get function: info of class  cave_type */
static int toluaI_get_util_cave_type_info(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->info);
 return 1;
}

/* set function: info of class  cave_type */
static int toluaI_set_util_cave_type_info(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->info = ((u16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: feat of class  cave_type */
static int toluaI_get_util_cave_type_feat(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->feat);
 return 1;
}

/* set function: feat of class  cave_type */
static int toluaI_set_util_cave_type_feat(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->feat = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: inventory of class  cave_type */
static int toluaI_get_util_cave_type_inventory(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->inventory,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: inventory of class  cave_type */
static int toluaI_set_util_cave_type_inventory(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->inventory = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: m_idx of class  cave_type */
static int toluaI_get_util_cave_type_m_idx(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->m_idx);
 return 1;
}

/* set function: m_idx of class  cave_type */
static int toluaI_set_util_cave_type_m_idx(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->m_idx = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  cave_type */
static int toluaI_get_util_cave_type_flags(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  cave_type */
static int toluaI_set_util_cave_type_flags(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: mana of class  cave_type */
static int toluaI_get_util_cave_type_mana(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mana);
 return 1;
}

/* set function: mana of class  cave_type */
static int toluaI_set_util_cave_type_mana(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mana = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: mimic of class  cave_type */
static int toluaI_get_util_cave_type_mimic(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->mimic);
 return 1;
}

/* set function: mimic of class  cave_type */
static int toluaI_set_util_cave_type_mimic(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->mimic = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: effect of class  cave_type */
static int toluaI_get_util_cave_type_effect(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->effect);
 return 1;
}

/* set function: effect of class  cave_type */
static int toluaI_set_util_cave_type_effect(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->effect = ((s16b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: activations of class  cave_type */
static int toluaI_get_util_cave_type_activations(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->activations,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: activations of class  cave_type */
static int toluaI_set_util_cave_type_activations(lua_State* tolua_S)
{
  cave_type* self = (cave_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->activations = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: ANGBAND_SYS */
static int toluaI_get_util_ANGBAND_SYS(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)ANGBAND_SYS);
 return 1;
}

/* set function: ANGBAND_SYS */
static int toluaI_set_util_ANGBAND_SYS(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  ANGBAND_SYS = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: ANGBAND_KEYBOARD */
static int toluaI_get_util_ANGBAND_KEYBOARD(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)ANGBAND_KEYBOARD);
 return 1;
}

/* set function: ANGBAND_KEYBOARD */
static int toluaI_set_util_ANGBAND_KEYBOARD(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  ANGBAND_KEYBOARD = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TOME_GRAPHICS */
static int toluaI_get_util_TOME_GRAPHICS(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TOME_GRAPHICS);
 return 1;
}

/* set function: TOME_GRAPHICS */
static int toluaI_set_util_TOME_GRAPHICS(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TOME_GRAPHICS = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR */
static int toluaI_get_util_TENGINE_DIR(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR);
 return 1;
}

/* set function: TENGINE_DIR */
static int toluaI_set_util_TENGINE_DIR(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_SCORES */
static int toluaI_get_util_TENGINE_DIR_SCORES(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_SCORES);
 return 1;
}

/* set function: TENGINE_DIR_SCORES */
static int toluaI_set_util_TENGINE_DIR_SCORES(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_SCORES = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_BONE */
static int toluaI_get_util_TENGINE_DIR_BONE(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_BONE);
 return 1;
}

/* set function: TENGINE_DIR_BONE */
static int toluaI_set_util_TENGINE_DIR_BONE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_BONE = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_CMOV */
static int toluaI_get_util_TENGINE_DIR_CMOV(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_CMOV);
 return 1;
}

/* set function: TENGINE_DIR_CMOV */
static int toluaI_set_util_TENGINE_DIR_CMOV(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_CMOV = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_DATA */
static int toluaI_get_util_TENGINE_DIR_DATA(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_DATA);
 return 1;
}

/* set function: TENGINE_DIR_DATA */
static int toluaI_set_util_TENGINE_DIR_DATA(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_DATA = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_DNGN */
static int toluaI_get_util_TENGINE_DIR_DNGN(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_DNGN);
 return 1;
}

/* set function: TENGINE_DIR_DNGN */
static int toluaI_set_util_TENGINE_DIR_DNGN(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_DNGN = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_ENGINE */
static int toluaI_get_util_TENGINE_DIR_ENGINE(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_ENGINE);
 return 1;
}

/* set function: TENGINE_DIR_ENGINE */
static int toluaI_set_util_TENGINE_DIR_ENGINE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_ENGINE = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_FILE */
static int toluaI_get_util_TENGINE_DIR_FILE(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_FILE);
 return 1;
}

/* set function: TENGINE_DIR_FILE */
static int toluaI_set_util_TENGINE_DIR_FILE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_FILE = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_FONT */
static int toluaI_get_util_TENGINE_DIR_FONT(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_FONT);
 return 1;
}

/* set function: TENGINE_DIR_FONT */
static int toluaI_set_util_TENGINE_DIR_FONT(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_FONT = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_GRAF */
static int toluaI_get_util_TENGINE_DIR_GRAF(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_GRAF);
 return 1;
}

/* set function: TENGINE_DIR_GRAF */
static int toluaI_set_util_TENGINE_DIR_GRAF(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_GRAF = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_HELP */
static int toluaI_get_util_TENGINE_DIR_HELP(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_HELP);
 return 1;
}

/* set function: TENGINE_DIR_HELP */
static int toluaI_set_util_TENGINE_DIR_HELP(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_HELP = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_INFO */
static int toluaI_get_util_TENGINE_DIR_INFO(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_INFO);
 return 1;
}

/* set function: TENGINE_DIR_INFO */
static int toluaI_set_util_TENGINE_DIR_INFO(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_INFO = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_MODULES */
static int toluaI_get_util_TENGINE_DIR_MODULES(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_MODULES);
 return 1;
}

/* set function: TENGINE_DIR_MODULES */
static int toluaI_set_util_TENGINE_DIR_MODULES(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_MODULES = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_NOTE */
static int toluaI_get_util_TENGINE_DIR_NOTE(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_NOTE);
 return 1;
}

/* set function: TENGINE_DIR_NOTE */
static int toluaI_set_util_TENGINE_DIR_NOTE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_NOTE = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_PATCH */
static int toluaI_get_util_TENGINE_DIR_PATCH(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_PATCH);
 return 1;
}

/* set function: TENGINE_DIR_PATCH */
static int toluaI_set_util_TENGINE_DIR_PATCH(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_PATCH = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_PREF */
static int toluaI_get_util_TENGINE_DIR_PREF(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_PREF);
 return 1;
}

/* set function: TENGINE_DIR_PREF */
static int toluaI_set_util_TENGINE_DIR_PREF(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_PREF = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_RAW */
static int toluaI_get_util_TENGINE_DIR_RAW(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_RAW);
 return 1;
}

/* set function: TENGINE_DIR_RAW */
static int toluaI_set_util_TENGINE_DIR_RAW(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_RAW = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_SAVE */
static int toluaI_get_util_TENGINE_DIR_SAVE(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_SAVE);
 return 1;
}

/* set function: TENGINE_DIR_SAVE */
static int toluaI_set_util_TENGINE_DIR_SAVE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_SAVE = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_SCPT */
static int toluaI_get_util_TENGINE_DIR_SCPT(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_SCPT);
 return 1;
}

/* set function: TENGINE_DIR_SCPT */
static int toluaI_set_util_TENGINE_DIR_SCPT(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_SCPT = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_USER */
static int toluaI_get_util_TENGINE_DIR_USER(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_USER);
 return 1;
}

/* set function: TENGINE_DIR_USER */
static int toluaI_set_util_TENGINE_DIR_USER(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_USER = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: TENGINE_DIR_XTRA */
static int toluaI_get_util_TENGINE_DIR_XTRA(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)TENGINE_DIR_XTRA);
 return 1;
}

/* set function: TENGINE_DIR_XTRA */
static int toluaI_set_util_TENGINE_DIR_XTRA(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  TENGINE_DIR_XTRA = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* function: distance */
static int toluaI_util_distance00(lua_State* tolua_S)
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
  s32b toluaI_ret = (s32b)  distance(y1,x1,y2,x2);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'distance'.");
 return 0;
}

/* function: los */
static int toluaI_util_los00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  los(y1,x1,y2,x2);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'los'.");
 return 0;
}

/* function: cave_feat_get */
static int toluaI_util_cave_feat_get00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"cave_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cave_type* c_ptr = ((cave_type*)  tolua_getusertype(tolua_S,1,0));
  s32b flag = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  cave_feat_get(c_ptr,flag);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cave_feat_get'.");
 return 0;
}

/* function: cave_feat_is */
static int toluaI_util_cave_feat_is00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"cave_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cave_type* c_ptr = ((cave_type*)  tolua_getusertype(tolua_S,1,0));
  s32b flag = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  cave_feat_is(c_ptr,flag);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cave_feat_is'.");
 return 0;
}

/* function: cave_feat_get */
static int toluaI_util_cave_feat_get01(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"cave_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cave_type* c_ptr = ((cave_type*)  tolua_getusertype(tolua_S,1,0));
  s32b flag = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  cave_feat_get(c_ptr,flag);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 return toluaI_util_cave_feat_get00(tolua_S);
}

/* function: cave_mimic_is */
static int toluaI_util_cave_mimic_is00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"cave_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cave_type* c_ptr = ((cave_type*)  tolua_getusertype(tolua_S,1,0));
  s32b flag = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  cave_mimic_is(c_ptr,flag);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cave_mimic_is'.");
 return 0;
}

/* function: lua_get_cave */
static int toluaI_util_cave00(lua_State* tolua_S)
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
  cave_type* toluaI_ret = (cave_type*)  lua_get_cave(y,x);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"cave_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cave'.");
 return 0;
}

/* function: set_target */
static int toluaI_util_set_target00(lua_State* tolua_S)
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
  set_target(y,x);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_target'.");
 return 0;
}

/* function: get_target */
static int toluaI_util_get_target00(lua_State* tolua_S)
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
  s32b dir = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  get_target(dir,&y,&x);
 tolua_pushnumber(tolua_S,(long)y);
 tolua_pushnumber(tolua_S,(long)x);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_target'.");
 return 0;
}

/* get function: m_allow_special */
static int toluaI_get_util_m_allow_special(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_r_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushbool(tolua_S,(int)m_allow_special[toluaI_index]);
 return 1;
}

/* set function: m_allow_special */
static int toluaI_set_util_m_allow_special(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_r_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  m_allow_special[toluaI_index] = ((bool)  tolua_getbool(tolua_S,3,0));
 return 0;
}

/* get function: k_allow_special */
static int toluaI_get_util_k_allow_special(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_k_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushbool(tolua_S,(int)k_allow_special[toluaI_index]);
 return 1;
}

/* set function: k_allow_special */
static int toluaI_set_util_k_allow_special(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_k_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  k_allow_special[toluaI_index] = ((bool)  tolua_getbool(tolua_S,3,0));
 return 0;
}

/* get function: a_allow_special */
static int toluaI_get_util_a_allow_special(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_a_idx)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushbool(tolua_S,(int)a_allow_special[toluaI_index]);
 return 1;
}

/* set function: a_allow_special */
static int toluaI_set_util_a_allow_special(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=max_a_idx)
 tolua_error(tolua_S,"array indexing out of range.");
  a_allow_special[toluaI_index] = ((bool)  tolua_getbool(tolua_S,3,0));
 return 0;
}

/* function: cave_set_feat */
static int toluaI_util_cave_set_feat00(lua_State* tolua_S)
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
  s32b feat = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  cave_set_feat(y,x,feat);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cave_set_feat'.");
 return 0;
}

/* function: show_file */
static int toluaI_util_show_file00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  cptr name = ((cptr)  tolua_getstring(tolua_S,1,0));
  cptr what = ((cptr)  tolua_getstring(tolua_S,2,0));
  s32b line = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b mode = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  bool toluaI_ret = (bool)  show_file(name,what,line,mode);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'show_file'.");
 return 0;
}

/* get function: target_who */
static int toluaI_get_util_target_who(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)target_who);
 return 1;
}

/* set function: target_who */
static int toluaI_set_util_target_who(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  target_who = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: target_col */
static int toluaI_get_util_target_col(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)target_col);
 return 1;
}

/* set function: target_col */
static int toluaI_set_util_target_col(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  target_col = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: target_row */
static int toluaI_get_util_target_row(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)target_row);
 return 1;
}

/* set function: target_row */
static int toluaI_set_util_target_row(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  target_row = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_bact */
static int toluaI_get_util_max_bact(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_bact);
 return 1;
}

/* set function: max_bact */
static int toluaI_set_util_max_bact(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_bact = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: ddd */
static int toluaI_get_util_ddd(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)ddd[toluaI_index]);
 return 1;
}

/* set function: ddd */
static int toluaI_set_util_ddd(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
  ddd[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: ddx */
static int toluaI_get_util_ddx(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=10)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)ddx[toluaI_index]);
 return 1;
}

/* set function: ddx */
static int toluaI_set_util_ddx(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=10)
 tolua_error(tolua_S,"array indexing out of range.");
  ddx[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: ddy */
static int toluaI_get_util_ddy(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=10)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)ddy[toluaI_index]);
 return 1;
}

/* set function: ddy */
static int toluaI_set_util_ddy(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=10)
 tolua_error(tolua_S,"array indexing out of range.");
  ddy[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: ddx_ddd */
static int toluaI_get_util_ddx_ddd(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)ddx_ddd[toluaI_index]);
 return 1;
}

/* set function: ddx_ddd */
static int toluaI_set_util_ddx_ddd(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
  ddx_ddd[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* get function: ddy_ddd */
static int toluaI_get_util_ddy_ddd(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)ddy_ddd[toluaI_index]);
 return 1;
}

/* set function: ddy_ddd */
static int toluaI_set_util_ddy_ddd(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=9)
 tolua_error(tolua_S,"array indexing out of range.");
  ddy_ddd[toluaI_index] = ((s16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* function: alloc_room */
static int toluaI_util_alloc_room00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,9)
 )
 goto tolua_lerror;
 else
 {
  s32b by0 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b bx0 = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b ysize = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b xsize = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b y2 = ((s32b)  tolua_getnumber(tolua_S,7,0));
  s32b x2 = ((s32b)  tolua_getnumber(tolua_S,8,0));
 {
  bool toluaI_ret = (bool)  alloc_room(by0,bx0,ysize,xsize,&y1,&x1,&y2,&x2);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)y1);
 tolua_pushnumber(tolua_S,(long)x1);
 tolua_pushnumber(tolua_S,(long)y2);
 tolua_pushnumber(tolua_S,(long)x2);
 }
 }
 return 5;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'alloc_room'.");
 return 0;
}

/* function: is_quest */
static int toluaI_util_is_quest00(lua_State* tolua_S)
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
  s32b toluaI_ret = (s32b)  is_quest(level);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_quest'.");
 return 0;
}

/* function: draw_box */
static int toluaI_util_draw_box00(lua_State* tolua_S)
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
  draw_box(y,x,h,w);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'draw_box'.");
 return 0;
}

/* function: lua_input_box */
static int toluaI_util_input_box00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  cptr title = ((cptr)  tolua_getstring(tolua_S,1,0));
  s32b max = ((s32b)  tolua_getnumber(tolua_S,2,0));
  cptr def = ((cptr)  tolua_getstring(tolua_S,3,""));
 {
  char* toluaI_ret = (char*)  lua_input_box(title,max,def);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'input_box'.");
 return 0;
}

/* function: lua_msg_box */
static int toluaI_util_msg_box00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr title = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  lua_msg_box(title);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'msg_box'.");
 return 0;
}

/* function: rescale */
static int toluaI_util_rescale00(lua_State* tolua_S)
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
  s32b x = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b max = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b new_max = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  s32b toluaI_ret = (s32b)  rescale(x,max,new_max);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rescale'.");
 return 0;
}

/* function: player_name_lua */
static int toluaI_util_player_name00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr new_name = ((cptr)  tolua_getstring(tolua_S,1,NULL));
 {
  const char* toluaI_ret = (const char*)  player_name_lua(new_name);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'player_name'.");
 return 0;
}

/* function: lua_make_temp_file */
static int toluaI_util_make_temp_file00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  lua_make_temp_file();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_temp_file'.");
 return 0;
}

/* function: lua_write_temp_file */
static int toluaI_util_write_temp_file00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr str = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  errr toluaI_ret = (errr)  lua_write_temp_file(str);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'write_temp_file'.");
 return 0;
}

/* function: lua_close_temp_file */
static int toluaI_util_close_temp_file00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  lua_close_temp_file();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'close_temp_file'.");
 return 0;
}

/* function: lua_end_temp_file */
static int toluaI_util_end_temp_file00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  lua_end_temp_file();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'end_temp_file'.");
 return 0;
}

/* function: lua_get_temp_name */
static int toluaI_util_get_temp_name00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  cptr toluaI_ret = (cptr)  lua_get_temp_name();
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_temp_name'.");
 return 0;
}

/* function: quark_str */
static int toluaI_util_quark_str00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s16b num = ((s16b)  tolua_getnumber(tolua_S,1,0));
 {
  cptr toluaI_ret = (cptr)  quark_str(num);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'quark_str'.");
 return 0;
}

/* function: quark_add */
static int toluaI_util_quark_add00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr str = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  s16b toluaI_ret = (s16b)  quark_add(str);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'quark_add'.");
 return 0;
}

/* get function: game_module */
static int toluaI_get_util_game_module(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)game_module);
 return 1;
}

/* set function: game_module */
static int toluaI_set_util_game_module(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  game_module = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* function: get_keymap_dir */
static int toluaI_util_get_keymap_dir00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  char ch = ((char)  tolua_getnumber(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  get_keymap_dir(ch);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_keymap_dir'.");
 return 0;
}

/* get function: next of class  timer_type */
static int toluaI_get_util_timer_type_next(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)self->next,tolua_tag(tolua_S,"timer_type"));
 return 1;
}

/* set function: next of class  timer_type */
static int toluaI_set_util_timer_type_next(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"timer_type"),0))
 TOLUA_ERR_ASSIGN;
  self->next = ((timer_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: enabled of class  timer_type */
static int toluaI_get_util_timer_type_enabled(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushbool(tolua_S,(int)self->enabled);
 return 1;
}

/* set function: enabled of class  timer_type */
static int toluaI_set_util_timer_type_enabled(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  self->enabled = ((bool)  tolua_getbool(tolua_S,2,0));
 return 0;
}

/* get function: delay of class  timer_type */
static int toluaI_get_util_timer_type_delay(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->delay);
 return 1;
}

/* set function: delay of class  timer_type */
static int toluaI_set_util_timer_type_delay(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->delay = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: countdown of class  timer_type */
static int toluaI_get_util_timer_type_countdown(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->countdown);
 return 1;
}

/* set function: countdown of class  timer_type */
static int toluaI_set_util_timer_type_countdown(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->countdown = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: callback of class  timer_type */
static int toluaI_get_util_timer_type_callback(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->callback);
 return 1;
}

/* set function: callback of class  timer_type */
static int toluaI_set_util_timer_type_callback(lua_State* tolua_S)
{
  timer_type* self = (timer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->callback = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* function: lua_create_list */
static int toluaI_util_create_list00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b size = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  list_type* toluaI_ret = (list_type*)  lua_create_list(size);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"list_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_list'.");
 return 0;
}

/* function: lua_delete_list */
static int toluaI_util_delete_list00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"list_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  list_type* tolua_var_1 = ((list_type*)  tolua_getusertype(tolua_S,1,0));
  s32b size = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  lua_delete_list(tolua_var_1,size);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete_list'.");
 return 0;
}

/* function: lua_add_to_list */
static int toluaI_util_add_to_list00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"list_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  list_type* tolua_var_2 = ((list_type*)  tolua_getusertype(tolua_S,1,0));
  s32b idx = ((s32b)  tolua_getnumber(tolua_S,2,0));
  cptr str = ((cptr)  tolua_getstring(tolua_S,3,0));
 {
  lua_add_to_list(tolua_var_2,idx,str);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_to_list'.");
 return 0;
}

/* function: lua_display_list */
static int toluaI_util_display_list00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,6,tolua_tag(tolua_S,"list_type"),0) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,8,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,9,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,10,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,11,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,12)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b h = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b w = ((s32b)  tolua_getnumber(tolua_S,4,0));
  cptr title = ((cptr)  tolua_getstring(tolua_S,5,0));
  list_type* list = ((list_type*)  tolua_getusertype(tolua_S,6,0));
  s32b max = ((s32b)  tolua_getnumber(tolua_S,7,0));
  s32b begin = ((s32b)  tolua_getnumber(tolua_S,8,0));
  s32b sel = ((s32b)  tolua_getnumber(tolua_S,9,0));
  byte sel_color = ((byte)  tolua_getnumber(tolua_S,10,0));
  bool show_scrollbar = ((bool)  tolua_getbool(tolua_S,11,FALSE));
 {
  lua_display_list(y,x,h,w,title,list,max,begin,sel,sel_color,show_scrollbar);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'display_list'.");
 return 0;
}

/* get function: key of class  flag_node */
static int toluaI_get_util_flag_node_key(lua_State* tolua_S)
{
  flag_node* self = (flag_node*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->key);
 return 1;
}

/* set function: key of class  flag_node */
static int toluaI_set_util_flag_node_key(lua_State* tolua_S)
{
  flag_node* self = (flag_node*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->key = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  flag_node */
static int toluaI_get_util_flag_node_flags(lua_State* tolua_S)
{
  flag_node* self = (flag_node*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->flags);
 return 1;
}

/* set function: flags of class  flag_node */
static int toluaI_set_util_flag_node_flags(lua_State* tolua_S)
{
  flag_node* self = (flag_node*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->flags = ((byte)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: next of class  flag_node */
static int toluaI_get_util_flag_node_next(lua_State* tolua_S)
{
  flag_node* self = (flag_node*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)self->next,tolua_tag(tolua_S,"flag_node"));
 return 1;
}

/* set function: next of class  flag_node */
static int toluaI_set_util_flag_node_next(lua_State* tolua_S)
{
  flag_node* self = (flag_node*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flag_node"),0))
 TOLUA_ERR_ASSIGN;
  self->next = ((struct flag_node*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* function: flag_node_get_ptr_type */
static int toluaI_util_flag_node_get_ptr_type00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flag_node"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flag_node* node = ((flag_node*)  tolua_getusertype(tolua_S,1,0));
 {
  byte toluaI_ret = (byte)  flag_node_get_ptr_type(node);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_node_get_ptr_type'.");
 return 0;
}

/* get function: node of class  flags_type */
static int toluaI_get_util_flags_type_node(lua_State* tolua_S)
{
 int toluaI_index;
  flags_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (flags_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=32536)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushusertype(tolua_S,(void*)&self->node[toluaI_index],tolua_tag(tolua_S,"flag_node"));
 return 1;
}

/* set function: node of class  flags_type */
static int toluaI_set_util_flags_type_node(lua_State* tolua_S)
{
 int toluaI_index;
  flags_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (flags_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=32536)
 tolua_error(tolua_S,"array indexing out of range.");
  self->node[toluaI_index] = *((flag_node*)  tolua_getusertype(tolua_S,3,0));
 return 0;
}

/* get function: size of class  flags_type */
static int toluaI_get_util_flags_type_size(lua_State* tolua_S)
{
  flags_type* self = (flags_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->size);
 return 1;
}

/* set function: size of class  flags_type */
static int toluaI_set_util_flags_type_size(lua_State* tolua_S)
{
  flags_type* self = (flags_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->size = ((s32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: firstfree of class  flags_type */
static int toluaI_get_util_flags_type_firstfree(lua_State* tolua_S)
{
  flags_type* self = (flags_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)self->firstfree,tolua_tag(tolua_S,"flag_node"));
 return 1;
}

/* set function: firstfree of class  flags_type */
static int toluaI_set_util_flags_type_firstfree(lua_State* tolua_S)
{
  flags_type* self = (flags_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flag_node"),0))
 TOLUA_ERR_ASSIGN;
  self->firstfree = ((flag_node*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: flags of class  flags_holder_type */
static int toluaI_get_util_flags_holder_type_flags(lua_State* tolua_S)
{
  flags_holder_type* self = (flags_holder_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)&self->flags,tolua_tag(tolua_S,"flags_type"));
 return 1;
}

/* set function: flags of class  flags_holder_type */
static int toluaI_set_util_flags_holder_type_flags(lua_State* tolua_S)
{
  flags_holder_type* self = (flags_holder_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0))
 TOLUA_ERR_ASSIGN;
  self->flags = *((flags_type*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* get function: index of class  flag_registry */
static int toluaI_get_util_flag_registry_index(lua_State* tolua_S)
{
  flag_registry* self = (flag_registry*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->index);
 return 1;
}

/* set function: index of class  flag_registry */
static int toluaI_set_util_flag_registry_index(lua_State* tolua_S)
{
  flag_registry* self = (flag_registry*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->index = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: name of class  flag_registry */
static int toluaI_get_util_flag_registry_name(lua_State* tolua_S)
{
  flag_registry* self = (flag_registry*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushstring(tolua_S,(const char*)self->name);
 return 1;
}

/* set function: name of class  flag_registry */
static int toluaI_set_util_flag_registry_name(lua_State* tolua_S)
{
  flag_registry* self = (flag_registry*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  self->name = ((cptr)  tolua_getstring(tolua_S,2,0));
 return 0;
}

/* get function: next of class  flag_registry */
static int toluaI_get_util_flag_registry_next(lua_State* tolua_S)
{
  flag_registry* self = (flag_registry*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushusertype(tolua_S,(void*)self->next,tolua_tag(tolua_S,"flag_registry"));
 return 1;
}

/* set function: next of class  flag_registry */
static int toluaI_set_util_flag_registry_next(lua_State* tolua_S)
{
  flag_registry* self = (flag_registry*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flag_registry"),0))
 TOLUA_ERR_ASSIGN;
  self->next = ((struct flag_registry*)  tolua_getusertype(tolua_S,2,0));
 return 0;
}

/* function: register_flag */
static int toluaI_util_register_flag00(lua_State* tolua_S)
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
  u32b toluaI_ret = (u32b)  register_flag(name);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'register_flag'.");
 return 0;
}

/* function: find_flag */
static int toluaI_util_find_flag00(lua_State* tolua_S)
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
  flag_registry* toluaI_ret = (flag_registry*)  find_flag(index);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"flag_registry"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_flag'.");
 return 0;
}

/* function: find_flag_by_name */
static int toluaI_util_find_flag_by_name00(lua_State* tolua_S)
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
  flag_registry* toluaI_ret = (flag_registry*)  find_flag_by_name(name);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"flag_registry"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_flag_by_name'.");
 return 0;
}

/* function: get_flag_name */
static int toluaI_util_get_flag_name00(lua_State* tolua_S)
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
  cptr toluaI_ret = (cptr)  get_flag_name(index);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_flag_name'.");
 return 0;
}

/* function: get_flag_index */
static int toluaI_util_get_flag_index00(lua_State* tolua_S)
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
  u32b toluaI_ret = (u32b)  get_flag_index(name);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_flag_index'.");
 return 0;
}

/* function: flag_set_full */
static int toluaI_util_flag_set_full00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,tolua_tag(tolua_S,"flags_type"),1) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
  byte flags = ((byte)  tolua_getnumber(tolua_S,3,0));
  s32b value = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b value2 = ((s32b)  tolua_getnumber(tolua_S,5,0));
  flags_type* value_flags = ((flags_type*)  tolua_getusertype(tolua_S,6,0));
 {
  flag_set_full(t,key,flags,value,value2,value_flags);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_set_full'.");
 return 0;
}

/* function: flag_set */
static int toluaI_util_flag_set00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
  s32b value = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  flag_set(t,key,value);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_set'.");
 return 0;
}

/* function: flag_remove */
static int toluaI_util_flag_remove00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  flag_remove(t,key);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_remove'.");
 return 0;
}

/* function: flag_remove_rand */
static int toluaI_util_flag_remove_rand00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  flag_remove_rand(t);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_remove_rand'.");
 return 0;
}

/* function: flag_get_node */
static int toluaI_util_flag_get_node00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  flag_node* toluaI_ret = (flag_node*)  flag_get_node(t,key);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"flag_node"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_get_node'.");
 return 0;
}

/* function: flag_get */
static int toluaI_util_flag_get00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  flag_get(t,key);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_get'.");
 return 0;
}

/* function: flag_get2 */
static int toluaI_util_flag_get200(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  flag_get2(t,key);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_get2'.");
 return 0;
}

/* function: flag_get_flags */
static int toluaI_util_flag_get_flags00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  flags_type* toluaI_ret = (flags_type*)  flag_get_flags(t,key);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"flags_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_get_flags'.");
 return 0;
}

/* function: flag_exists */
static int toluaI_util_flag_exists00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  flag_exists(t,key);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_exists'.");
 return 0;
}

/* function: flag_new */
static int toluaI_util_flag_new00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b size = ((s32b)  tolua_getnumber(tolua_S,1,2));
 {
  flags_type* toluaI_ret = (flags_type*)  flag_new(size);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"flags_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_new'.");
 return 0;
}

/* function: flag_init */
static int toluaI_util_flag_init00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  s32b size = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  flag_init(t,size);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_init'.");
 return 0;
}

/* function: flag_free */
static int toluaI_util_flag_free00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  bool full = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  flag_free(t,full);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_free'.");
 return 0;
}

/* function: flags_print */
static int toluaI_util_flags_print00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  flags_print(t);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flags_print'.");
 return 0;
}

/* function: flag_remove_zap */
static int toluaI_util_flag_remove_zap00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  flag_remove_zap(t,key);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_remove_zap'.");
 return 0;
}

/* function: flag_remove_rand_zap */
static int toluaI_util_flag_remove_rand_zap00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  flag_remove_rand_zap(t);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_remove_rand_zap'.");
 return 0;
}

/* function: flag_first_free_key */
static int toluaI_util_flag_first_free_key00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b min = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  u32b toluaI_ret = (u32b)  flag_first_free_key(t,min);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_first_free_key'.");
 return 0;
}

/* function: flag_max_key */
static int toluaI_util_flag_max_key00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  u32b toluaI_ret = (u32b)  flag_max_key(t);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_max_key'.");
 return 0;
}

/* function: flag_empty */
static int toluaI_util_flag_empty00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  flag_empty(t);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_empty'.");
 return 0;
}

/* function: flag_empty_zap */
static int toluaI_util_flag_empty_zap00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  flag_empty_zap(t);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_empty_zap'.");
 return 0;
}

/* function: flag_dup */
static int toluaI_util_flag_dup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  flags_type* toluaI_ret = (flags_type*)  flag_dup(src);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"flags_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_dup'.");
 return 0;
}

/* function: flag_intersects */
static int toluaI_util_flag_intersects00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* f1 = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* f2 = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  flag_intersects(f1,f2);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_intersects'.");
 return 0;
}

/* function: flag_copy */
static int toluaI_util_flag_copy00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  flag_copy(dest,src);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_copy'.");
 return 0;
}

/* function: flag_contains */
static int toluaI_util_flag_contains00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* f1 = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* f2 = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  flag_contains(f1,f2);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_contains'.");
 return 0;
}

/* function: flag_contains_nb */
static int toluaI_util_flag_contains_nb00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* f1 = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* f2 = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  u32b toluaI_ret = (u32b)  flag_contains_nb(f1,f2);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_contains_nb'.");
 return 0;
}

/* function: flag_equal */
static int toluaI_util_flag_equal00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  flag_equal(dest,src);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_equal'.");
 return 0;
}

/* function: flag_used */
static int toluaI_util_flag_used00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  flag_used(t);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_used'.");
 return 0;
}

/* function: flag_add */
static int toluaI_util_flag_add00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  flag_add(dest,src);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_add'.");
 return 0;
}

/* function: flag_add_new */
static int toluaI_util_flag_add_new00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  flag_add_new(dest,src);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_add_new'.");
 return 0;
}

/* function: flag_del */
static int toluaI_util_flag_del00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  flag_del(dest,src);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_del'.");
 return 0;
}

/* function: flag_add_value_from */
static int toluaI_util_flag_add_value_from00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
  flags_type* from = ((flags_type*)  tolua_getusertype(tolua_S,3,0));
 {
  flag_add_value_from(dest,src,from);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_add_value_from'.");
 return 0;
}

/* function: flag_add_increase */
static int toluaI_util_flag_add_increase00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  flag_add_increase(dest,src);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_add_increase'.");
 return 0;
}

/* function: flag_add_higher */
static int toluaI_util_flag_add_higher00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  flag_add_higher(dest,src);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_add_higher'.");
 return 0;
}

/* function: flag_get_rand */
static int toluaI_util_flag_get_rand00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  u32b toluaI_ret = (u32b)  flag_get_rand(t);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_get_rand'.");
 return 0;
}

/* function: flag_get_ptr */
static int toluaI_util_flag_get_ptr00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
  byte type = ((byte)  tolua_getnumber(tolua_S,3,0));
 {
  void* toluaI_ret = (void*)  flag_get_ptr(t,key,type);
 tolua_pushuserdata(tolua_S,(void*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_get_ptr'.");
 return 0;
}

/* function: flag_set_ptr */
static int toluaI_util_flag_set_ptr00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
  byte type = ((byte)  tolua_getnumber(tolua_S,3,0));
  void* ptr = ((void*)  tolua_getuserdata(tolua_S,4,0));
 {
  flag_set_ptr(t,key,type,ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_set_ptr'.");
 return 0;
}

/* function: flag_shallow_dup */
static int toluaI_util_flag_shallow_dup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  flags_type* toluaI_ret = (flags_type*)  flag_shallow_dup(src);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"flags_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_shallow_dup'.");
 return 0;
}

/* function: flag_shallow_copy */
static int toluaI_util_flag_shallow_copy00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* dest = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  flags_type* src = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
 {
  flag_shallow_copy(dest,src);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_shallow_copy'.");
 return 0;
}

/* function: flag_shallow_free */
static int toluaI_util_flag_shallow_free00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  bool full = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  flag_shallow_free(t,full);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_shallow_free'.");
 return 0;
}

/* function: flag_learn */
static int toluaI_util_flag_learn00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
  bool learn = ((bool)  tolua_getbool(tolua_S,3,0));
 {
  flag_learn(t,key,learn);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_learn'.");
 return 0;
}

/* function: flag_is_known */
static int toluaI_util_flag_is_known00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  u32b key = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  flag_is_known(t,key);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_is_known'.");
 return 0;
}

/* function: flag_learn_all */
static int toluaI_util_flag_learn_all00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  flags_type* t = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
 {
  flag_learn_all(t);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flag_learn_all'.");
 return 0;
}

/* function: lua_cave_clean_bold */
static int toluaI_util_cave_clean_bold00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_cave_clean_bold(y,x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cave_clean_bold'.");
 return 0;
}

/* function: lua_cave_floor_bold */
static int toluaI_util_cave_floor_bold00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_cave_floor_bold(y,x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cave_floor_bold'.");
 return 0;
}

/* function: lua_player_has_los_bold */
static int toluaI_util_player_has_los_bold00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_player_has_los_bold(y,x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'player_has_los_bold'.");
 return 0;
}

/* function: lua_panel_contains */
static int toluaI_util_panel_contains00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_panel_contains(y,x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'panel_contains'.");
 return 0;
}

/* function: lua_cave_empty_bold */
static int toluaI_util_cave_empty_bold00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_cave_empty_bold(y,x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cave_empty_bold'.");
 return 0;
}

/* function: lua_get_rnd_line */
static int toluaI_util_get_rnd_line00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  char* file_name = ((char*)  tolua_getstring(tolua_S,1,0));
 {
  char* toluaI_ret = (char*)  lua_get_rnd_line(file_name);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_rnd_line'.");
 return 0;
}

/* function: lua_get_line */
static int toluaI_util_get_line00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  char* fname = ((char*)  tolua_getstring(tolua_S,1,0));
  cptr fdir = ((cptr)  tolua_getstring(tolua_S,2,0));
  s32b line = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  char* toluaI_ret = (char*)  lua_get_line(fname,fdir,line);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_line'.");
 return 0;
}

/* get function: delay_factor */
static int toluaI_get_util_delay_factor(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)delay_factor);
 return 1;
}

/* set function: delay_factor */
static int toluaI_set_util_delay_factor(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  delay_factor = ((byte)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: target_able */
static int toluaI_util_target_able00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  target_able(m_idx);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'target_able'.");
 return 0;
}

/* function: target_okay */
static int toluaI_util_target_okay00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  target_okay();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'target_okay'.");
 return 0;
}

/* function: target_set */
static int toluaI_util_target_set00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b mode = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  target_set(mode);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'target_set'.");
 return 0;
}

/* function: mmove2 */
static int toluaI_util_mmove200(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b y2 = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b x2 = ((s32b)  tolua_getnumber(tolua_S,6,0));
 {
  mmove2(&y,&x,y1,x1,y2,x2);
 tolua_pushnumber(tolua_S,(long)y);
 tolua_pushnumber(tolua_S,(long)x);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'mmove2'.");
 return 0;
}

/* function: forget_los_tracks */
static int toluaI_util_forget_los_tracks00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  forget_los_tracks();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'forget_los_tracks'.");
 return 0;
}

/* function: print_rel */
static int toluaI_util_print_rel00(lua_State* tolua_S)
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
  char c = ((char)  tolua_getnumber(tolua_S,1,0));
  byte a = ((byte)  tolua_getnumber(tolua_S,2,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  print_rel(c,a,y,x);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'print_rel'.");
 return 0;
}

/* function: note_spot */
static int toluaI_util_note_spot00(lua_State* tolua_S)
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
  note_spot(y,x);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'note_spot'.");
 return 0;
}

/* function: lite_spot */
static int toluaI_util_lite_spot00(lua_State* tolua_S)
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
  lite_spot(y,x);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lite_spot'.");
 return 0;
}

/* get function: max_panel_rows */
static int toluaI_get_util_max_panel_rows(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_panel_rows);
 return 1;
}

/* set function: max_panel_rows */
static int toluaI_set_util_max_panel_rows(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_panel_rows = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: max_panel_cols */
static int toluaI_get_util_max_panel_cols(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)max_panel_cols);
 return 1;
}

/* set function: max_panel_cols */
static int toluaI_set_util_max_panel_cols(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  max_panel_cols = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: panel_row_min */
static int toluaI_get_util_panel_row_min(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)panel_row_min);
 return 1;
}

/* set function: panel_row_min */
static int toluaI_set_util_panel_row_min(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  panel_row_min = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: panel_row_max */
static int toluaI_get_util_panel_row_max(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)panel_row_max);
 return 1;
}

/* set function: panel_row_max */
static int toluaI_set_util_panel_row_max(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  panel_row_max = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: panel_col_min */
static int toluaI_get_util_panel_col_min(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)panel_col_min);
 return 1;
}

/* set function: panel_col_min */
static int toluaI_set_util_panel_col_min(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  panel_col_min = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: panel_col_max */
static int toluaI_get_util_panel_col_max(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)panel_col_max);
 return 1;
}

/* set function: panel_col_max */
static int toluaI_set_util_panel_col_max(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  panel_col_max = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: panel_col_prt */
static int toluaI_get_util_panel_col_prt(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)panel_col_prt);
 return 1;
}

/* set function: panel_col_prt */
static int toluaI_set_util_panel_col_prt(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  panel_col_prt = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: panel_row_prt */
static int toluaI_get_util_panel_row_prt(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)panel_row_prt);
 return 1;
}

/* set function: panel_row_prt */
static int toluaI_set_util_panel_row_prt(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  panel_row_prt = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: ROW_MAP */
static int toluaI_get_util_ROW_MAP(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)ROW_MAP);
 return 1;
}

/* set function: ROW_MAP */
static int toluaI_set_util_ROW_MAP(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  ROW_MAP = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: COL_MAP */
static int toluaI_get_util_COL_MAP(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)COL_MAP);
 return 1;
}

/* set function: COL_MAP */
static int toluaI_set_util_COL_MAP(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  COL_MAP = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: ROW_MAP_HGT */
static int toluaI_get_util_ROW_MAP_HGT(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)ROW_MAP_HGT);
 return 1;
}

/* set function: ROW_MAP_HGT */
static int toluaI_set_util_ROW_MAP_HGT(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  ROW_MAP_HGT = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: COL_MAP_WID */
static int toluaI_get_util_COL_MAP_WID(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)COL_MAP_WID);
 return 1;
}

/* set function: COL_MAP_WID */
static int toluaI_set_util_COL_MAP_WID(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  COL_MAP_WID = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: message_row */
static int toluaI_get_util_message_row(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)message_row);
 return 1;
}

/* set function: message_row */
static int toluaI_set_util_message_row(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  message_row = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: message_col */
static int toluaI_get_util_message_col(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)message_col);
 return 1;
}

/* set function: message_col */
static int toluaI_set_util_message_col(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  message_col = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: message_col_max */
static int toluaI_get_util_message_col_max(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)message_col_max);
 return 1;
}

/* set function: message_col_max */
static int toluaI_set_util_message_col_max(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  message_col_max = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: lua_grab_string */
static int toluaI_util_grab_string00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"string_line"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  string_line source = *((string_line*)  tolua_getusertype(tolua_S,1,0));
  s32b offset = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  char* toluaI_ret = (char*)  lua_grab_string(source,offset);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'grab_string'.");
 return 0;
}

/* function: do_cmd_redraw */
static int toluaI_util_do_cmd_redraw00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  do_cmd_redraw();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_cmd_redraw'.");
 return 0;
}

/* function: panel_bounds */
static int toluaI_util_panel_bounds00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  panel_bounds();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'panel_bounds'.");
 return 0;
}

/* get function: done_loading_data_files */
static int toluaI_get_util_done_loading_data_files(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)done_loading_data_files);
 return 1;
}

/* set function: done_loading_data_files */
static int toluaI_set_util_done_loading_data_files(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  done_loading_data_files = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: lua_in_bounds */
static int toluaI_util_in_bounds00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_in_bounds(y,x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'in_bounds'.");
 return 0;
}

/* function: lua_in_bounds2 */
static int toluaI_util_in_bounds200(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  lua_in_bounds2(y,x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'in_bounds2'.");
 return 0;
}

/* get function: DAY */
static int toluaI_get_util_time_DAY(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)DAY);
 return 1;
}

/* set function: DAY */
static int toluaI_set_util_time_DAY(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  DAY = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: YEAR */
static int toluaI_get_util_time_YEAR(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)YEAR);
 return 1;
}

/* set function: YEAR */
static int toluaI_set_util_time_YEAR(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  YEAR = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: HOUR */
static int toluaI_get_util_time_HOUR(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)HOUR);
 return 1;
}

/* set function: HOUR */
static int toluaI_set_util_time_HOUR(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  HOUR = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: MINUTE */
static int toluaI_get_util_time_MINUTE(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)MINUTE);
 return 1;
}

/* set function: MINUTE */
static int toluaI_set_util_time_MINUTE(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  MINUTE = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: DAY_START */
static int toluaI_get_util_time_DAY_START(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)DAY_START);
 return 1;
}

/* set function: DAY_START */
static int toluaI_set_util_time_DAY_START(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  DAY_START = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: bst */
static int toluaI_util_time___exec_module00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b what = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b t = ((s32b)  tolua_getnumber(tolua_S,2,turn));
 {
  s32b toluaI_ret = (s32b)  bst(what,t);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__exec_module'.");
 return 0;
}

/* function: new_timer */
static int toluaI_util_time___new_timer00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cptr callback = ((cptr)  tolua_getstring(tolua_S,1,0));
  s32b delay = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  timer_type* toluaI_ret = (timer_type*)  new_timer(callback,delay);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"timer_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__new_timer'.");
 return 0;
}

/* function: del_timer */
static int toluaI_util_time_del_timer00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"timer_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  timer_type* t_ptr = ((timer_type*)  tolua_getusertype(tolua_S,1,0));
 {
  del_timer(t_ptr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_timer'.");
 return 0;
}

/* function: cli_add */
static int toluaI_util_cli_add00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  cptr active = ((cptr)  tolua_getstring(tolua_S,1,0));
  cptr trigger = ((cptr)  tolua_getstring(tolua_S,2,0));
  cptr descr = ((cptr)  tolua_getstring(tolua_S,3,0));
 {
  cli_add(active,trigger,descr);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cli_add'.");
 return 0;
}

/* function: do_cmd_automatizer */
static int toluaI_util_do_cmd_automatizer00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  do_cmd_automatizer();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_cmd_automatizer'.");
 return 0;
}

/* function: squeltch_grid */
static int toluaI_util_squeltch_grid00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  squeltch_grid();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'squeltch_grid'.");
 return 0;
}

/* function: squeltch_inventory */
static int toluaI_util_squeltch_inventory00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  squeltch_inventory();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'squeltch_inventory'.");
 return 0;
}

/* function: do_cmd_options_win */
static int toluaI_util_do_cmd_options_win00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  do_cmd_options_win();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_cmd_options_win'.");
 return 0;
}

/* function: __wrap_ptr_to_string */
static int toluaI_util___wrap_ptr_to_string00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  void* ptr = ((void*)  tolua_getuserdata(tolua_S,1,0));
 {
  cptr toluaI_ret = (cptr)  __wrap_ptr_to_string(ptr);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'to_string'.");
 return 0;
}

/* function: __wrap_ptr_to_object */
static int toluaI_util___wrap_ptr_to_object00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  void* ptr = ((void*)  tolua_getuserdata(tolua_S,1,0));
 {
  object_type* toluaI_ret = (object_type*)  __wrap_ptr_to_object(ptr);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"object_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'to_object'.");
 return 0;
}

/* function: __wrap_ptr_to_monster_blow */
static int toluaI_util___wrap_ptr_to_monster_blow00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  void* ptr = ((void*)  tolua_getuserdata(tolua_S,1,0));
 {
  monster_blow* toluaI_ret = (monster_blow*)  __wrap_ptr_to_monster_blow(ptr);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"monster_blow"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'to_monster_blow'.");
 return 0;
}

/* function: __wrap_ptr_to_monster_type */
static int toluaI_util___wrap_ptr_to_monster_type00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  void* ptr = ((void*)  tolua_getuserdata(tolua_S,1,0));
 {
  monster_type* toluaI_ret = (monster_type*)  __wrap_ptr_to_monster_type(ptr);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"monster_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'to_monster_type'.");
 return 0;
}

/* function: __wrap_ptr_to_store_type */
static int toluaI_util___wrap_ptr_to_store_type00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  void* ptr = ((void*)  tolua_getuserdata(tolua_S,1,0));
 {
  store_type* toluaI_ret = (store_type*)  __wrap_ptr_to_store_type(ptr);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"store_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'to_store_type'.");
 return 0;
}

/* function: __wrap_ptr_dup_buffer_to_void */
static int toluaI_util___wrap_ptr_dup_buffer_to_void00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  void* ptr = ((void*)  tolua_getuserdata(tolua_S,1,0));
 {
  void* toluaI_ret = (void*)  __wrap_ptr_dup_buffer_to_void(ptr);
 tolua_pushuserdata(tolua_S,(void*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dup_buffer_to_void'.");
 return 0;
}

/* function: __wrap_ptr_to_buffer */
static int toluaI_util___wrap_ptr_to_buffer00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  void* ptr = ((void*)  tolua_getuserdata(tolua_S,1,0));
 {
  buffer_type* toluaI_ret = (buffer_type*)  __wrap_ptr_to_buffer(ptr);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"buffer_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'to_buffer'.");
 return 0;
}

/* function: __wrap_ptr_dup_string_to_void */
static int toluaI_util___wrap_ptr_dup_string_to_void00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr ptr = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  void* toluaI_ret = (void*)  __wrap_ptr_dup_string_to_void(ptr);
 tolua_pushuserdata(tolua_S,(void*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dup_string_to_void'.");
 return 0;
}

/* get function: rogue_like_commands */
static int toluaI_get_util_game_options_rogue_like_commands(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)rogue_like_commands);
 return 1;
}

/* set function: rogue_like_commands */
static int toluaI_set_util_game_options_rogue_like_commands(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  rogue_like_commands = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: quick_messages */
static int toluaI_get_util_game_options_quick_messages(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)quick_messages);
 return 1;
}

/* set function: quick_messages */
static int toluaI_set_util_game_options_quick_messages(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  quick_messages = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: other_query_flag */
static int toluaI_get_util_game_options_other_query_flag(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)other_query_flag);
 return 1;
}

/* set function: other_query_flag */
static int toluaI_set_util_game_options_other_query_flag(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  other_query_flag = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: carry_query_flag */
static int toluaI_get_util_game_options_carry_query_flag(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)carry_query_flag);
 return 1;
}

/* set function: carry_query_flag */
static int toluaI_set_util_game_options_carry_query_flag(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  carry_query_flag = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: use_old_target */
static int toluaI_get_util_game_options_use_old_target(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)use_old_target);
 return 1;
}

/* set function: use_old_target */
static int toluaI_set_util_game_options_use_old_target(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  use_old_target = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: always_pickup */
static int toluaI_get_util_game_options_always_pickup(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)always_pickup);
 return 1;
}

/* set function: always_pickup */
static int toluaI_set_util_game_options_always_pickup(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  always_pickup = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: prompt_pickup_heavy */
static int toluaI_get_util_game_options_prompt_pickup_heavy(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)prompt_pickup_heavy);
 return 1;
}

/* set function: prompt_pickup_heavy */
static int toluaI_set_util_game_options_prompt_pickup_heavy(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  prompt_pickup_heavy = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: always_repeat */
static int toluaI_get_util_game_options_always_repeat(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)always_repeat);
 return 1;
}

/* set function: always_repeat */
static int toluaI_set_util_game_options_always_repeat(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  always_repeat = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: depth_in_feet */
static int toluaI_get_util_game_options_depth_in_feet(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)depth_in_feet);
 return 1;
}

/* set function: depth_in_feet */
static int toluaI_set_util_game_options_depth_in_feet(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  depth_in_feet = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: stack_force_notes */
static int toluaI_get_util_game_options_stack_force_notes(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)stack_force_notes);
 return 1;
}

/* set function: stack_force_notes */
static int toluaI_set_util_game_options_stack_force_notes(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  stack_force_notes = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: stack_force_costs */
static int toluaI_get_util_game_options_stack_force_costs(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)stack_force_costs);
 return 1;
}

/* set function: stack_force_costs */
static int toluaI_set_util_game_options_stack_force_costs(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  stack_force_costs = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: show_labels */
static int toluaI_get_util_game_options_show_labels(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)show_labels);
 return 1;
}

/* set function: show_labels */
static int toluaI_set_util_game_options_show_labels(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  show_labels = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: show_weights */
static int toluaI_get_util_game_options_show_weights(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)show_weights);
 return 1;
}

/* set function: show_weights */
static int toluaI_set_util_game_options_show_weights(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  show_weights = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: show_inven_graph */
static int toluaI_get_util_game_options_show_inven_graph(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)show_inven_graph);
 return 1;
}

/* set function: show_inven_graph */
static int toluaI_set_util_game_options_show_inven_graph(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  show_inven_graph = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: show_equip_graph */
static int toluaI_get_util_game_options_show_equip_graph(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)show_equip_graph);
 return 1;
}

/* set function: show_equip_graph */
static int toluaI_set_util_game_options_show_equip_graph(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  show_equip_graph = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: show_store_graph */
static int toluaI_get_util_game_options_show_store_graph(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)show_store_graph);
 return 1;
}

/* set function: show_store_graph */
static int toluaI_set_util_game_options_show_store_graph(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  show_store_graph = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: show_choices */
static int toluaI_get_util_game_options_show_choices(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)show_choices);
 return 1;
}

/* set function: show_choices */
static int toluaI_set_util_game_options_show_choices(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  show_choices = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: show_details */
static int toluaI_get_util_game_options_show_details(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)show_details);
 return 1;
}

/* set function: show_details */
static int toluaI_set_util_game_options_show_details(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  show_details = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: ring_bell */
static int toluaI_get_util_game_options_ring_bell(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)ring_bell);
 return 1;
}

/* set function: ring_bell */
static int toluaI_set_util_game_options_ring_bell(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  ring_bell = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: use_color */
static int toluaI_get_util_game_options_use_color(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)use_color);
 return 1;
}

/* set function: use_color */
static int toluaI_set_util_game_options_use_color(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  use_color = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: find_ignore_stairs */
static int toluaI_get_util_game_options_find_ignore_stairs(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)find_ignore_stairs);
 return 1;
}

/* set function: find_ignore_stairs */
static int toluaI_set_util_game_options_find_ignore_stairs(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  find_ignore_stairs = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: find_ignore_doors */
static int toluaI_get_util_game_options_find_ignore_doors(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)find_ignore_doors);
 return 1;
}

/* set function: find_ignore_doors */
static int toluaI_set_util_game_options_find_ignore_doors(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  find_ignore_doors = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: find_cut */
static int toluaI_get_util_game_options_find_cut(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)find_cut);
 return 1;
}

/* set function: find_cut */
static int toluaI_set_util_game_options_find_cut(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  find_cut = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: find_examine */
static int toluaI_get_util_game_options_find_examine(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)find_examine);
 return 1;
}

/* set function: find_examine */
static int toluaI_set_util_game_options_find_examine(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  find_examine = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: disturb_move */
static int toluaI_get_util_game_options_disturb_move(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)disturb_move);
 return 1;
}

/* set function: disturb_move */
static int toluaI_set_util_game_options_disturb_move(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  disturb_move = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: disturb_near */
static int toluaI_get_util_game_options_disturb_near(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)disturb_near);
 return 1;
}

/* set function: disturb_near */
static int toluaI_set_util_game_options_disturb_near(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  disturb_near = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: disturb_panel */
static int toluaI_get_util_game_options_disturb_panel(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)disturb_panel);
 return 1;
}

/* set function: disturb_panel */
static int toluaI_set_util_game_options_disturb_panel(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  disturb_panel = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: disturb_detect */
static int toluaI_get_util_game_options_disturb_detect(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)disturb_detect);
 return 1;
}

/* set function: disturb_detect */
static int toluaI_set_util_game_options_disturb_detect(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  disturb_detect = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: disturb_state */
static int toluaI_get_util_game_options_disturb_state(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)disturb_state);
 return 1;
}

/* set function: disturb_state */
static int toluaI_set_util_game_options_disturb_state(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  disturb_state = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: disturb_minor */
static int toluaI_get_util_game_options_disturb_minor(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)disturb_minor);
 return 1;
}

/* set function: disturb_minor */
static int toluaI_set_util_game_options_disturb_minor(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  disturb_minor = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: disturb_other */
static int toluaI_get_util_game_options_disturb_other(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)disturb_other);
 return 1;
}

/* set function: disturb_other */
static int toluaI_set_util_game_options_disturb_other(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  disturb_other = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: alert_hitpoint */
static int toluaI_get_util_game_options_alert_hitpoint(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)alert_hitpoint);
 return 1;
}

/* set function: alert_hitpoint */
static int toluaI_set_util_game_options_alert_hitpoint(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  alert_hitpoint = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: alert_failure */
static int toluaI_get_util_game_options_alert_failure(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)alert_failure);
 return 1;
}

/* set function: alert_failure */
static int toluaI_set_util_game_options_alert_failure(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  alert_failure = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: last_words */
static int toluaI_get_util_game_options_last_words(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)last_words);
 return 1;
}

/* set function: last_words */
static int toluaI_set_util_game_options_last_words(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  last_words = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: speak_unique */
static int toluaI_get_util_game_options_speak_unique(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)speak_unique);
 return 1;
}

/* set function: speak_unique */
static int toluaI_set_util_game_options_speak_unique(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  speak_unique = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: auto_destroy */
static int toluaI_get_util_game_options_auto_destroy(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)auto_destroy);
 return 1;
}

/* set function: auto_destroy */
static int toluaI_set_util_game_options_auto_destroy(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  auto_destroy = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: wear_confirm */
static int toluaI_get_util_game_options_confirm_wear(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)wear_confirm);
 return 1;
}

/* set function: wear_confirm */
static int toluaI_set_util_game_options_confirm_wear(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  wear_confirm = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: confirm_stairs */
static int toluaI_get_util_game_options_confirm_stairs(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)confirm_stairs);
 return 1;
}

/* set function: confirm_stairs */
static int toluaI_set_util_game_options_confirm_stairs(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  confirm_stairs = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: disturb_pets */
static int toluaI_get_util_game_options_disturb_pets(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)disturb_pets);
 return 1;
}

/* set function: disturb_pets */
static int toluaI_set_util_game_options_disturb_pets(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  disturb_pets = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: easy_inven */
static int toluaI_get_util_game_options_easy_inven(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)easy_inven);
 return 1;
}

/* set function: easy_inven */
static int toluaI_set_util_game_options_easy_inven(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  easy_inven = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: easy_open */
static int toluaI_get_util_game_options_easy_open(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)easy_open);
 return 1;
}

/* set function: easy_open */
static int toluaI_set_util_game_options_easy_open(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  easy_open = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: easy_disarm */
static int toluaI_get_util_game_options_easy_disarm(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)easy_disarm);
 return 1;
}

/* set function: easy_disarm */
static int toluaI_set_util_game_options_easy_disarm(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  easy_disarm = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: easy_tunnel */
static int toluaI_get_util_game_options_easy_tunnel(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)easy_tunnel);
 return 1;
}

/* set function: easy_tunnel */
static int toluaI_set_util_game_options_easy_tunnel(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  easy_tunnel = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: auto_haggle */
static int toluaI_get_util_game_options_auto_haggle(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)auto_haggle);
 return 1;
}

/* set function: auto_haggle */
static int toluaI_set_util_game_options_auto_haggle(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  auto_haggle = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: auto_scum */
static int toluaI_get_util_game_options_auto_scum(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)auto_scum);
 return 1;
}

/* set function: auto_scum */
static int toluaI_set_util_game_options_auto_scum(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  auto_scum = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: stack_allow_items */
static int toluaI_get_util_game_options_stack_allow_items(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)stack_allow_items);
 return 1;
}

/* set function: stack_allow_items */
static int toluaI_set_util_game_options_stack_allow_items(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  stack_allow_items = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: stack_allow_wands */
static int toluaI_get_util_game_options_stack_allow_wands(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)stack_allow_wands);
 return 1;
}

/* set function: stack_allow_wands */
static int toluaI_set_util_game_options_stack_allow_wands(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  stack_allow_wands = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: expand_look */
static int toluaI_get_util_game_options_expand_look(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)expand_look);
 return 1;
}

/* set function: expand_look */
static int toluaI_set_util_game_options_expand_look(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  expand_look = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: expand_list */
static int toluaI_get_util_game_options_expand_list(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)expand_list);
 return 1;
}

/* set function: expand_list */
static int toluaI_set_util_game_options_expand_list(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  expand_list = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: view_perma_grids */
static int toluaI_get_util_game_options_view_perma_grids(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)view_perma_grids);
 return 1;
}

/* set function: view_perma_grids */
static int toluaI_set_util_game_options_view_perma_grids(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  view_perma_grids = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: view_torch_grids */
static int toluaI_get_util_game_options_view_torch_grids(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)view_torch_grids);
 return 1;
}

/* set function: view_torch_grids */
static int toluaI_set_util_game_options_view_torch_grids(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  view_torch_grids = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: monster_lite */
static int toluaI_get_util_game_options_monster_lite(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)monster_lite);
 return 1;
}

/* set function: monster_lite */
static int toluaI_set_util_game_options_monster_lite(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  monster_lite = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: dungeon_align */
static int toluaI_get_util_game_options_dungeon_align(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)dungeon_align);
 return 1;
}

/* set function: dungeon_align */
static int toluaI_set_util_game_options_dungeon_align(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  dungeon_align = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: dungeon_stair */
static int toluaI_get_util_game_options_dungeon_stair(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)dungeon_stair);
 return 1;
}

/* set function: dungeon_stair */
static int toluaI_set_util_game_options_dungeon_stair(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  dungeon_stair = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: player_symbols */
static int toluaI_get_util_game_options_player_symbols(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)player_symbols);
 return 1;
}

/* set function: player_symbols */
static int toluaI_set_util_game_options_player_symbols(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  player_symbols = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: plain_descriptions */
static int toluaI_get_util_game_options_plain_descriptions(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)plain_descriptions);
 return 1;
}

/* set function: plain_descriptions */
static int toluaI_set_util_game_options_plain_descriptions(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  plain_descriptions = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: smart_learn */
static int toluaI_get_util_game_options_smart_learn(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)smart_learn);
 return 1;
}

/* set function: smart_learn */
static int toluaI_set_util_game_options_smart_learn(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  smart_learn = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: smart_cheat */
static int toluaI_get_util_game_options_smart_cheat(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)smart_cheat);
 return 1;
}

/* set function: smart_cheat */
static int toluaI_set_util_game_options_smart_cheat(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  smart_cheat = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: stupid_monsters */
static int toluaI_get_util_game_options_stupid_monsters(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)stupid_monsters);
 return 1;
}

/* set function: stupid_monsters */
static int toluaI_set_util_game_options_stupid_monsters(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  stupid_monsters = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: small_levels */
static int toluaI_get_util_game_options_small_levels(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)small_levels);
 return 1;
}

/* set function: small_levels */
static int toluaI_set_util_game_options_small_levels(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  small_levels = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: empty_levels */
static int toluaI_get_util_game_options_empty_levels(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)empty_levels);
 return 1;
}

/* set function: empty_levels */
static int toluaI_set_util_game_options_empty_levels(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  empty_levels = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: view_reduce_lite */
static int toluaI_get_util_game_options_view_reduce_lite(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)view_reduce_lite);
 return 1;
}

/* set function: view_reduce_lite */
static int toluaI_set_util_game_options_view_reduce_lite(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  view_reduce_lite = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: view_reduce_view */
static int toluaI_get_util_game_options_view_reduce_view(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)view_reduce_view);
 return 1;
}

/* set function: view_reduce_view */
static int toluaI_set_util_game_options_view_reduce_view(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  view_reduce_view = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: avoid_abort */
static int toluaI_get_util_game_options_avoid_abort(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)avoid_abort);
 return 1;
}

/* set function: avoid_abort */
static int toluaI_set_util_game_options_avoid_abort(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  avoid_abort = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: avoid_shimmer */
static int toluaI_get_util_game_options_avoid_shimmer(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)avoid_shimmer);
 return 1;
}

/* set function: avoid_shimmer */
static int toluaI_set_util_game_options_avoid_shimmer(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  avoid_shimmer = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: avoid_other */
static int toluaI_get_util_game_options_avoid_other(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)avoid_other);
 return 1;
}

/* set function: avoid_other */
static int toluaI_set_util_game_options_avoid_other(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  avoid_other = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: flush_failure */
static int toluaI_get_util_game_options_flush_failure(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)flush_failure);
 return 1;
}

/* set function: flush_failure */
static int toluaI_set_util_game_options_flush_failure(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  flush_failure = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: flush_disturb */
static int toluaI_get_util_game_options_flush_disturb(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)flush_disturb);
 return 1;
}

/* set function: flush_disturb */
static int toluaI_set_util_game_options_flush_disturb(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  flush_disturb = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: flush_command */
static int toluaI_get_util_game_options_flush_command(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)flush_command);
 return 1;
}

/* set function: flush_command */
static int toluaI_set_util_game_options_flush_command(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  flush_command = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: fresh_before */
static int toluaI_get_util_game_options_fresh_before(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)fresh_before);
 return 1;
}

/* set function: fresh_before */
static int toluaI_set_util_game_options_fresh_before(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  fresh_before = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: fresh_after */
static int toluaI_get_util_game_options_fresh_after(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)fresh_after);
 return 1;
}

/* set function: fresh_after */
static int toluaI_set_util_game_options_fresh_after(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  fresh_after = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: fresh_message */
static int toluaI_get_util_game_options_fresh_message(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)fresh_message);
 return 1;
}

/* set function: fresh_message */
static int toluaI_set_util_game_options_fresh_message(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  fresh_message = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: compress_savefile */
static int toluaI_get_util_game_options_compress_savefile(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)compress_savefile);
 return 1;
}

/* set function: compress_savefile */
static int toluaI_set_util_game_options_compress_savefile(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  compress_savefile = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: hilite_player */
static int toluaI_get_util_game_options_hilite_player(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)hilite_player);
 return 1;
}

/* set function: hilite_player */
static int toluaI_set_util_game_options_hilite_player(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  hilite_player = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: view_lited_grid */
static int toluaI_get_util_game_options_view_lited_grid(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)view_lited_grid);
 return 1;
}

/* set function: view_lited_grid */
static int toluaI_set_util_game_options_view_lited_grid(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  view_lited_grid = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: center_player */
static int toluaI_get_util_game_options_center_player(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)center_player);
 return 1;
}

/* set function: center_player */
static int toluaI_set_util_game_options_center_player(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  center_player = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: exp_need */
static int toluaI_get_util_game_options_exp_need(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)exp_need);
 return 1;
}

/* set function: exp_need */
static int toluaI_set_util_game_options_exp_need(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  exp_need = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: auto_more */
static int toluaI_get_util_game_options_auto_more(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)auto_more);
 return 1;
}

/* set function: auto_more */
static int toluaI_set_util_game_options_auto_more(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  auto_more = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: player_char_health */
static int toluaI_get_util_game_options_player_char_health(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)player_char_health);
 return 1;
}

/* set function: player_char_health */
static int toluaI_set_util_game_options_player_char_health(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  player_char_health = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: linear_stats */
static int toluaI_get_util_game_options_linear_stats(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)linear_stats);
 return 1;
}

/* set function: linear_stats */
static int toluaI_set_util_game_options_linear_stats(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  linear_stats = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: inventory_no_move */
static int toluaI_get_util_game_options_inventory_no_move(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)inventory_no_move);
 return 1;
}

/* set function: inventory_no_move */
static int toluaI_set_util_game_options_inventory_no_move(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  inventory_no_move = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: maximize */
static int toluaI_get_util_game_options_maximize(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)maximize);
 return 1;
}

/* set function: maximize */
static int toluaI_set_util_game_options_maximize(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  maximize = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: preserve */
static int toluaI_get_util_game_options_preserve(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)preserve);
 return 1;
}

/* set function: preserve */
static int toluaI_set_util_game_options_preserve(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  preserve = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: autoroll */
static int toluaI_get_util_game_options_autoroll(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)autoroll);
 return 1;
}

/* set function: autoroll */
static int toluaI_set_util_game_options_autoroll(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  autoroll = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: point_based */
static int toluaI_get_util_game_options_point_based(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)point_based);
 return 1;
}

/* set function: point_based */
static int toluaI_set_util_game_options_point_based(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  point_based = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: permanent_levels */
static int toluaI_get_util_game_options_permanent_levels(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)permanent_levels);
 return 1;
}

/* set function: permanent_levels */
static int toluaI_set_util_game_options_permanent_levels(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  permanent_levels = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: ironman_rooms */
static int toluaI_get_util_game_options_ironman_rooms(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)ironman_rooms);
 return 1;
}

/* set function: ironman_rooms */
static int toluaI_set_util_game_options_ironman_rooms(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  ironman_rooms = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: take_notes */
static int toluaI_get_util_game_options_take_notes(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)take_notes);
 return 1;
}

/* set function: take_notes */
static int toluaI_set_util_game_options_take_notes(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  take_notes = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: auto_notes */
static int toluaI_get_util_game_options_auto_notes(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)auto_notes);
 return 1;
}

/* set function: auto_notes */
static int toluaI_set_util_game_options_auto_notes(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  auto_notes = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: fast_autoroller */
static int toluaI_get_util_game_options_fast_autoroller(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)fast_autoroller);
 return 1;
}

/* set function: fast_autoroller */
static int toluaI_set_util_game_options_fast_autoroller(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  fast_autoroller = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: joke_monsters */
static int toluaI_get_util_game_options_joke_monsters(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)joke_monsters);
 return 1;
}

/* set function: joke_monsters */
static int toluaI_set_util_game_options_joke_monsters(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  joke_monsters = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: always_small_level */
static int toluaI_get_util_game_options_always_small_level(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)always_small_level);
 return 1;
}

/* set function: always_small_level */
static int toluaI_set_util_game_options_always_small_level(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  always_small_level = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: cheat_peek */
static int toluaI_get_util_game_options_cheat_peek(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)cheat_peek);
 return 1;
}

/* set function: cheat_peek */
static int toluaI_set_util_game_options_cheat_peek(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  cheat_peek = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: cheat_hear */
static int toluaI_get_util_game_options_cheat_hear(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)cheat_hear);
 return 1;
}

/* set function: cheat_hear */
static int toluaI_set_util_game_options_cheat_hear(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  cheat_hear = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: cheat_room */
static int toluaI_get_util_game_options_cheat_room(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)cheat_room);
 return 1;
}

/* set function: cheat_room */
static int toluaI_set_util_game_options_cheat_room(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  cheat_room = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: cheat_xtra */
static int toluaI_get_util_game_options_cheat_xtra(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)cheat_xtra);
 return 1;
}

/* set function: cheat_xtra */
static int toluaI_set_util_game_options_cheat_xtra(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  cheat_xtra = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: cheat_know */
static int toluaI_get_util_game_options_cheat_know(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)cheat_know);
 return 1;
}

/* set function: cheat_know */
static int toluaI_set_util_game_options_cheat_know(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  cheat_know = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: cheat_live */
static int toluaI_get_util_game_options_cheat_live(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)cheat_live);
 return 1;
}

/* set function: cheat_live */
static int toluaI_set_util_game_options_cheat_live(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  cheat_live = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: autosave_t */
static int toluaI_get_util_game_options_autosave_t(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)autosave_t);
 return 1;
}

/* set function: autosave_t */
static int toluaI_set_util_game_options_autosave_t(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  autosave_t = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: autosave_l */
static int toluaI_get_util_game_options_autosave_l(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)autosave_l);
 return 1;
}

/* set function: autosave_l */
static int toluaI_set_util_game_options_autosave_l(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  autosave_l = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: autosave_freq */
static int toluaI_get_util_game_options_autosave_freq(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)autosave_freq);
 return 1;
}

/* set function: autosave_freq */
static int toluaI_set_util_game_options_autosave_freq(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  autosave_freq = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: automatizer_enabled */
static int toluaI_get_util_game_options_automatizer_enabled(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)automatizer_enabled);
 return 1;
}

/* set function: automatizer_enabled */
static int toluaI_set_util_game_options_automatizer_enabled(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  automatizer_enabled = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: character_generated */
static int toluaI_get_util_game_options_character_generated(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)character_generated);
 return 1;
}

/* set function: character_generated */
static int toluaI_set_util_game_options_character_generated(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  character_generated = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: hitpoint_warn */
static int toluaI_get_util_game_options_hitpoint_warn(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)hitpoint_warn);
 return 1;
}

/* set function: hitpoint_warn */
static int toluaI_set_util_game_options_hitpoint_warn(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  hitpoint_warn = ((byte)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: delay_factor */
static int toluaI_get_util_game_options_delay_factor(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)delay_factor);
 return 1;
}

/* set function: delay_factor */
static int toluaI_set_util_game_options_delay_factor(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  delay_factor = ((byte)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: option_ingame_help */
static int toluaI_get_util_game_options_option_ingame_help(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)option_ingame_help);
 return 1;
}

/* set function: option_ingame_help */
static int toluaI_set_util_game_options_option_ingame_help(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  option_ingame_help = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: savefile_compress */
static int toluaI_get_util_game_options_savefile_compress(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)savefile_compress);
 return 1;
}

/* set function: savefile_compress */
static int toluaI_set_util_game_options_savefile_compress(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  savefile_compress = ((s16b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: keypad_autorun */
static int toluaI_get_util_game_options_keypad_autorun(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)keypad_autorun);
 return 1;
}

/* set function: keypad_autorun */
static int toluaI_set_util_game_options_keypad_autorun(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  keypad_autorun = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: repeat__idx */
static int toluaI_get_util_repeat_keys_cur_idx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)repeat__idx);
 return 1;
}

/* set function: repeat__idx */
static int toluaI_set_util_repeat_keys_cur_idx(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  repeat__idx = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* get function: repeat__cnt */
static int toluaI_get_util_repeat_keys_cur_cnt(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)repeat__cnt);
 return 1;
}

/* set function: repeat__cnt */
static int toluaI_set_util_repeat_keys_cur_cnt(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  repeat__cnt = ((s32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: repeat_pop */
static int toluaI_util_repeat_keys_pop00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  repeat_pop();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pop'.");
 return 0;
}

/* function: repeat_push */
static int toluaI_util_repeat_keys_push00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b what = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  repeat_push(what);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'push'.");
 return 0;
}

/* function: repeat_pull */
static int toluaI_util_repeat_keys_pull00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b what = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  repeat_pull(&what);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)what);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pull'.");
 return 0;
}

/* function: repeat_check */
static int toluaI_util_repeat_keys_check00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  repeat_check();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'check'.");
 return 0;
}

/* function: get_count */
static int toluaI_util_repeat_keys_get_count00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b number = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b max = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  get_count(number,max);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_count'.");
 return 0;
}

/* function: add_new_inventory_slot */
static int toluaI_util_add_new_inventory_slot00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,4,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,5,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  cptr desc = ((cptr)  tolua_getstring(tolua_S,1,0));
  cptr name = ((cptr)  tolua_getstring(tolua_S,2,0));
  cptr describe = ((cptr)  tolua_getstring(tolua_S,3,0));
  cptr wear = ((cptr)  tolua_getstring(tolua_S,4,0));
  cptr takeoff = ((cptr)  tolua_getstring(tolua_S,5,0));
 {
  s16b toluaI_ret = (s16b)  add_new_inventory_slot(desc,name,describe,wear,takeoff);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_new_inventory_slot'.");
 return 0;
}

/* get function: len of class  buffer_type */
static int toluaI_get_util_buffer_type_len(lua_State* tolua_S)
{
  buffer_type* self = (buffer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 tolua_pushnumber(tolua_S,(long)self->len);
 return 1;
}

/* set function: len of class  buffer_type */
static int toluaI_set_util_buffer_type_len(lua_State* tolua_S)
{
  buffer_type* self = (buffer_type*)  tolua_getusertype(tolua_S,1,0);
 if (!self) TOLUA_ERR_SELF;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  self->len = ((u32b)  tolua_getnumber(tolua_S,2,0));
 return 0;
}

/* get function: buf of class  buffer_type */
static int toluaI_get_util_buffer_type_buf(lua_State* tolua_S)
{
 int toluaI_index;
  buffer_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (buffer_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=0xFFFFFFFF)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)self->buf[toluaI_index]);
 return 1;
}

/* set function: buf of class  buffer_type */
static int toluaI_set_util_buffer_type_buf(lua_State* tolua_S)
{
 int toluaI_index;
  buffer_type* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (buffer_type*)  lua_touserdata(tolua_S,-1);
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=0xFFFFFFFF)
 tolua_error(tolua_S,"array indexing out of range.");
  self->buf[toluaI_index] = ((char)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* function: data_buffer_alloc */
static int toluaI_util_data_buffer_alloc00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  buffer_type* toluaI_ret = (buffer_type*)  data_buffer_alloc();
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"buffer_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'data_buffer_alloc'.");
 return 0;
}

/* function: data_buffer_delete */
static int toluaI_util_data_buffer_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"buffer_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  buffer_type* buf = ((buffer_type*)  tolua_getusertype(tolua_S,1,0));
 {
  data_buffer_delete(buf);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'data_buffer_delete'.");
 return 0;
}

/* function: data_buffer_dup */
static int toluaI_util_data_buffer_dup00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"buffer_type"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  buffer_type* buf = ((buffer_type*)  tolua_getusertype(tolua_S,1,0));
 {
  buffer_type* toluaI_ret = (buffer_type*)  data_buffer_dup(buf);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"buffer_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'data_buffer_dup'.");
 return 0;
}

/* function: do_store */
static int toluaI_util_loadsave_do_store00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"store_type"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  byte type = ((byte)  tolua_getnumber(tolua_S,1,0));
  store_type* str = ((store_type*)  tolua_getusertype(tolua_S,2,0));
  s32b flag = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  do_store(type,str,flag);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_store'.");
 return 0;
}

/* function: do_monster */
static int toluaI_util_loadsave_do_monster00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  byte type = ((byte)  tolua_getnumber(tolua_S,1,0));
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,2,0));
  s32b flag = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  do_monster(type,m_ptr,flag);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_monster'.");
 return 0;
}

/* function: do_item */
static int toluaI_util_loadsave_do_item00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  byte type = ((byte)  tolua_getnumber(tolua_S,1,0));
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,2,0));
  s32b flag = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  do_item(type,o_ptr,flag);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_item'.");
 return 0;
}

/* function: do_char */
static int toluaI_util_loadsave_do_char00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  char* tolua_var_3 = ((char*)  tolua_getstring(tolua_S,1,0));
  s32b tolua_var_4 = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  do_char(tolua_var_3,tolua_var_4);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_char'.");
 return 0;
}

/* function: do_byte */
static int toluaI_util_loadsave_do_byte00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  byte tolua_var_5 = ((byte)  tolua_getnumber(tolua_S,1,0));
  s32b tolua_var_6 = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  do_byte(&tolua_var_5,tolua_var_6);
 tolua_pushnumber(tolua_S,(long)tolua_var_5);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_byte'.");
 return 0;
}

/* function: do_u16b */
static int toluaI_util_loadsave_do_u16b00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  u16b tolua_var_7 = ((u16b)  tolua_getnumber(tolua_S,1,0));
  s32b tolua_var_8 = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  do_u16b(&tolua_var_7,tolua_var_8);
 tolua_pushnumber(tolua_S,(long)tolua_var_7);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_u16b'.");
 return 0;
}

/* function: do_s16b */
static int toluaI_util_loadsave_do_s16b00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s16b tolua_var_9 = ((s16b)  tolua_getnumber(tolua_S,1,0));
  s32b tolua_var_10 = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  do_s16b(&tolua_var_9,tolua_var_10);
 tolua_pushnumber(tolua_S,(long)tolua_var_9);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_s16b'.");
 return 0;
}

/* function: do_u32b */
static int toluaI_util_loadsave_do_u32b00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  u32b tolua_var_11 = ((u32b)  tolua_getnumber(tolua_S,1,0));
  s32b tolua_var_12 = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  do_u32b(&tolua_var_11,tolua_var_12);
 tolua_pushnumber(tolua_S,(long)tolua_var_11);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_u32b'.");
 return 0;
}

/* function: do_s32b */
static int toluaI_util_loadsave_do_s32b00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b tolua_var_13 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b tolua_var_14 = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  do_s32b(&tolua_var_13,tolua_var_14);
 tolua_pushnumber(tolua_S,(long)tolua_var_13);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_s32b'.");
 return 0;
}

/* function: do_string_nolen */
static int toluaI_util_loadsave_do_string_nolen00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  char* tolua_var_15 = ((char*)  tolua_getstring(tolua_S,1,0));
  s32b tolua_var_16 = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  do_string_nolen(&tolua_var_15,tolua_var_16);
 tolua_pushstring(tolua_S,(const char*)tolua_var_15);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_string_nolen'.");
 return 0;
}

/* function: do_flags */
static int toluaI_util_loadsave_do_flags00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  flags_type* f = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  s32b flag = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  do_flags(f,flag);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_flags'.");
 return 0;
}

/* function: load_dungeon */
static int toluaI_util_loadsave_load_dungeon00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  s32b dungeon = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b level = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b saved_turn = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  load_dungeon(dungeon,level,&saved_turn);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)saved_turn);
 }
 }
 return 2;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_dungeon'.");
 return 0;
}

/* function: save_dungeon */
static int toluaI_util_loadsave_save_dungeon00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b saved_turn = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  save_dungeon(saved_turn);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_dungeon'.");
 return 0;
}

/* function: save_player */
static int toluaI_util_loadsave_save_player00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  save_player();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_player'.");
 return 0;
}

/* function: load_player */
static int toluaI_util_loadsave_load_player00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  bool quickstart = ((bool)  tolua_getbool(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  load_player(quickstart);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_player'.");
 return 0;
}

/* function: sroot */
static int toluaI_util_sroot00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b n = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  sroot(n);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sroot'.");
 return 0;
}

/* function: is_a_vowel */
static int toluaI_util_is_a_vowel00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b ch = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  is_a_vowel(ch);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_a_vowel'.");
 return 0;
}

/* function: check_prevent_cmd_aux */
static int toluaI_util_check_prevent_cmd_aux00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  object_type* o_ptr = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  char cmd = ((char)  tolua_getnumber(tolua_S,2,'\0'));
 {
  bool toluaI_ret = (bool)  check_prevent_cmd_aux(o_ptr,cmd);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'check_prevent_cmd_aux'.");
 return 0;
}

/* function: negate_bits */
static int toluaI_util_negate_bits00(lua_State* tolua_S)
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
  s32b toluaI_ret = (s32b)  negate_bits(i);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'negate_bits'.");
 return 0;
}

/* function: place_trap */
static int toluaI_util_place_trap00(lua_State* tolua_S)
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
  s32b faction = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  place_trap(y,x,faction);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'place_trap'.");
 return 0;
}

/* function: get_num_traps */
static int toluaI_util_get_num_traps00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  flags_type* traps = ((flags_type*)  tolua_getusertype(tolua_S,1,0));
  bool known_traps = ((bool)  tolua_getbool(tolua_S,2,FALSE));
  s16b faction = ((s16b)  tolua_getnumber(tolua_S,3,-1));
  bool friendly_traps = ((bool)  tolua_getbool(tolua_S,4,FALSE));
  bool apparent_traps = ((bool)  tolua_getbool(tolua_S,5,FALSE));
 {
  s32b toluaI_ret = (s32b)  get_num_traps(traps,known_traps,faction,friendly_traps,apparent_traps);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_num_traps'.");
 return 0;
}

/* function: get_num_location_traps */
static int toluaI_util_get_num_location_traps00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_istype(tolua_S,6,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,7)
 )
 goto tolua_lerror;
 else
 {
  s32b y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b x = ((s32b)  tolua_getnumber(tolua_S,2,0));
  bool known_traps = ((bool)  tolua_getbool(tolua_S,3,FALSE));
  s16b faction = ((s16b)  tolua_getnumber(tolua_S,4,-1));
  bool friendly_traps = ((bool)  tolua_getbool(tolua_S,5,FALSE));
  bool apparent_traps = ((bool)  tolua_getbool(tolua_S,6,FALSE));
 {
  s32b toluaI_ret = (s32b)  get_num_location_traps(y,x,known_traps,faction,friendly_traps,apparent_traps);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_num_location_traps'.");
 return 0;
}

/* function: get_num_object_traps */
static int toluaI_util_get_num_object_traps00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"object_type"),0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,4,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_istype(tolua_S,5,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,6)
 )
 goto tolua_lerror;
 else
 {
  object_type* obj = ((object_type*)  tolua_getusertype(tolua_S,1,0));
  bool known_traps = ((bool)  tolua_getbool(tolua_S,2,FALSE));
  s16b faction = ((s16b)  tolua_getnumber(tolua_S,3,-1));
  bool friendly_traps = ((bool)  tolua_getbool(tolua_S,4,FALSE));
  bool apparent_traps = ((bool)  tolua_getbool(tolua_S,5,FALSE));
 {
  s32b toluaI_ret = (s32b)  get_num_object_traps(obj,known_traps,faction,friendly_traps,apparent_traps);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_num_object_traps'.");
 return 0;
}

/* function: do_play_cmovie */
static int toluaI_util_do_play_cmovie00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cptr cmov_file = ((cptr)  tolua_getstring(tolua_S,1,0));
  bool show_info = ((bool)  tolua_getbool(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  do_play_cmovie(cmov_file,show_info);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'do_play_cmovie'.");
 return 0;
}

/* get function: character_icky */
static int toluaI_get_util_character_icky(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)character_icky);
 return 1;
}

/* set function: character_icky */
static int toluaI_set_util_character_icky(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  character_icky = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* function: pathfind_get_first */
static int toluaI_util_pathfind_get_first00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"monster_type"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,5,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,6,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,7,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,8,tolua_tag(tolua_S,"bool"),1) ||
 !tolua_isnoobj(tolua_S,9)
 )
 goto tolua_lerror;
 else
 {
  monster_type* m_ptr = ((monster_type*)  tolua_getusertype(tolua_S,1,0));
  s32b start_y = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b start_x = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b goal_y = ((s32b)  tolua_getnumber(tolua_S,4,0));
  s32b goal_x = ((s32b)  tolua_getnumber(tolua_S,5,0));
  s32b res_y = ((s32b)  tolua_getnumber(tolua_S,6,0));
  s32b res_x = ((s32b)  tolua_getnumber(tolua_S,7,0));
  bool complete = ((bool)  tolua_getbool(tolua_S,8,FALSE));
 {
  bool toluaI_ret = (bool)  pathfind_get_first(m_ptr,start_y,start_x,goal_y,goal_x,&res_y,&res_x,&complete);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)res_y);
 tolua_pushnumber(tolua_S,(long)res_x);
 tolua_pushbool(tolua_S,(int)complete);
 }
 }
 return 4;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_first'.");
 return 0;
}

/* function: pathfind_get_next */
static int toluaI_util_pathfind_get_next00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b res_y = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b res_x = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  pathfind_get_next(&res_y,&res_x);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)res_y);
 tolua_pushnumber(tolua_S,(long)res_x);
 }
 }
 return 3;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_next'.");
 return 0;
}

/* function: savefile_lua */
static int toluaI_util_savefile00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,1) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr save = ((cptr)  tolua_getstring(tolua_S,1,NULL));
 {
  cptr toluaI_ret = (cptr)  savefile_lua(save);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'savefile'.");
 return 0;
}

/* function: rt_timer_add */
static int toluaI_util_time___rt_add00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TUSERDATA,1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b interval = ((s32b)  tolua_getnumber(tolua_S,1,0));
  void* c_callback = ((void*)  tolua_getuserdata(tolua_S,2,NULL));
 {
  SDL_TimerID toluaI_ret = (SDL_TimerID)  rt_timer_add(interval,c_callback);
 {
#ifdef __cplusplus
 void* toluaI_clone = new SDL_TimerID(toluaI_ret);
#else
 void* toluaI_clone = tolua_copy(tolua_S,(void*)&toluaI_ret,sizeof(SDL_TimerID));
#endif
 tolua_pushusertype(tolua_S,tolua_doclone(tolua_S,toluaI_clone,tolua_tag(tolua_S,"SDL_TimerID")),tolua_tag(tolua_S,"SDL_TimerID"));
 }
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__rt_add'.");
 return 0;
}

/* function: rt_timer_del */
static int toluaI_util_time___rt_del00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"SDL_TimerID"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  SDL_TimerID id = *((SDL_TimerID*)  tolua_getusertype(tolua_S,1,0));
 {
  rt_timer_del(id);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__rt_del'.");
 return 0;
}

/* function: index_to_label */
static int toluaI_util_index_to_label00(lua_State* tolua_S)
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
  char toluaI_ret = (char)  index_to_label(i);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'index_to_label'.");
 return 0;
}

/* get function: ENGINE_DIR */
static int toluaI_get_util_ENGINE_DIR(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)ENGINE_DIR);
 return 1;
}

/* set function: ENGINE_DIR */
static int toluaI_set_util_ENGINE_DIR(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  ENGINE_DIR = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* get function: ENGINE_DIR_DEP */
static int toluaI_get_util_ENGINE_DIR_DEP(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)ENGINE_DIR_DEP);
 return 1;
}

/* set function: ENGINE_DIR_DEP */
static int toluaI_set_util_ENGINE_DIR_DEP(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  ENGINE_DIR_DEP = ((cptr)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* function: sdl_utl_fast_draw */
static int toluaI_util_sdl_utl_fast_draw00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  bool enable = ((bool)  tolua_getbool(tolua_S,1,0));
 {
  sdl_utl_fast_draw(enable);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sdl_utl_fast_draw'.");
 return 0;
}

/* get function: extract_energy */
static int toluaI_get_util___extract_energy(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=300)
 tolua_error(tolua_S,"array indexing out of range.");
 tolua_pushnumber(tolua_S,(long)extract_energy[toluaI_index]);
 return 1;
}

/* set function: extract_energy */
static int toluaI_set_util___extract_energy(lua_State* tolua_S)
{
 int toluaI_index;
 if (!tolua_istype(tolua_S,2,LUA_TNUMBER,0))
 tolua_error(tolua_S,"invalid type in array indexing.");
 toluaI_index = (int)tolua_getnumber(tolua_S,2,0)-1;
 if (toluaI_index<0 || toluaI_index>=300)
 tolua_error(tolua_S,"array indexing out of range.");
  extract_energy[toluaI_index] = ((u16b)  tolua_getnumber(tolua_S,3,0));
 return 0;
}

/* Open function */
int tolua_util_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_constant(tolua_S,NULL,"FALSE",FALSE);
 tolua_constant(tolua_S,NULL,"ESCAPE",ESCAPE);
 tolua_module(tolua_S,"color");
 tolua_constant(tolua_S,"color","DARK",TERM_DARK);
 tolua_constant(tolua_S,"color","WHITE",TERM_WHITE);
 tolua_constant(tolua_S,"color","SLATE",TERM_SLATE);
 tolua_constant(tolua_S,"color","ORANGE",TERM_ORANGE);
 tolua_constant(tolua_S,"color","RED",TERM_RED);
 tolua_constant(tolua_S,"color","GREEN",TERM_GREEN);
 tolua_constant(tolua_S,"color","BLUE",TERM_BLUE);
 tolua_constant(tolua_S,"color","UMBER",TERM_UMBER);
 tolua_constant(tolua_S,"color","LIGHT_DARK",TERM_L_DARK);
 tolua_constant(tolua_S,"color","LIGHT_WHITE",TERM_L_WHITE);
 tolua_constant(tolua_S,"color","VIOLET",TERM_VIOLET);
 tolua_constant(tolua_S,"color","YELLOW",TERM_YELLOW);
 tolua_constant(tolua_S,"color","LIGHT_RED",TERM_L_RED);
 tolua_constant(tolua_S,"color","LIGHT_GREEN",TERM_L_GREEN);
 tolua_constant(tolua_S,"color","LIGHT_BLUE",TERM_L_BLUE);
 tolua_constant(tolua_S,"color","LIGHT_UMBER",TERM_L_UMBER);
 tolua_tablevar(tolua_S,"color","can_redefine_color",toluaI_get_util_color_can_redefine_color,toluaI_set_util_color_can_redefine_color);
 tolua_function(tolua_S,"color","init_tome_color",toluaI_util_color_init_tome_color00);
 tolua_globalarray(tolua_S,"conv_color",toluaI_get_util_conv_color,toluaI_set_util_conv_color);
 tolua_module(tolua_S,"hook");
 tolua_constant(tolua_S,"hook","MONSTER_DEATH",HOOK_MONSTER_DEATH);
 tolua_constant(tolua_S,"hook","MONSTER_REMOVED",HOOK_MONSTER_REMOVED);
 tolua_constant(tolua_S,"hook","DO_DROP_CORPSE",HOOK_DO_DROP_CORPSE);
 tolua_constant(tolua_S,"hook","CORPSE_CREATE_PRE",HOOK_CORPSE_CREATE_PRE);
 tolua_constant(tolua_S,"hook","CORPSE_CREATE_POST",HOOK_CORPSE_CREATE_POST);
 tolua_constant(tolua_S,"hook","OPEN_PRE",HOOK_OPEN_PRE);
 tolua_constant(tolua_S,"hook","OPEN_POST",HOOK_OPEN_POST);
 tolua_constant(tolua_S,"hook","CLOSE_PRE",HOOK_CLOSE_PRE);
 tolua_constant(tolua_S,"hook","CLOSE_POST",HOOK_CLOSE_POST);
 tolua_constant(tolua_S,"hook","GEN_QUEST",HOOK_GEN_QUEST);
 tolua_constant(tolua_S,"hook","END_TURN",HOOK_END_TURN);
 tolua_constant(tolua_S,"hook","FEELING",HOOK_FEELING);
 tolua_constant(tolua_S,"hook","NEW_MONSTER_PRE",HOOK_NEW_MONSTER_PRE);
 tolua_constant(tolua_S,"hook","NEW_MONSTER_POST",HOOK_NEW_MONSTER_POST);
 tolua_constant(tolua_S,"hook","CREATE_DROP",HOOK_CREATE_DROP);
 tolua_constant(tolua_S,"hook","GEN_LEVEL",HOOK_GEN_LEVEL);
 tolua_constant(tolua_S,"hook","BUILD_ROOM1",HOOK_BUILD_ROOM1);
 tolua_constant(tolua_S,"hook","NEW_LEVEL",HOOK_NEW_LEVEL);
 tolua_constant(tolua_S,"hook","QUEST_FINISH",HOOK_QUEST_FINISH);
 tolua_constant(tolua_S,"hook","QUEST_FAIL",HOOK_QUEST_FAIL);
 tolua_constant(tolua_S,"hook","GIVE_PRE",HOOK_GIVE_PRE);
 tolua_constant(tolua_S,"hook","GIVE_POST",HOOK_GIVE_POST);
 tolua_constant(tolua_S,"hook","CHAR_DUMP",HOOK_CHAR_DUMP);
 tolua_constant(tolua_S,"hook","INIT_QUEST",HOOK_INIT_QUEST);
 tolua_constant(tolua_S,"hook","WILD_GEN",HOOK_WILD_GEN);
 tolua_constant(tolua_S,"hook","WILD_GEN_MON",HOOK_WILD_GEN_MON);
 tolua_constant(tolua_S,"hook","DROP_PRE",HOOK_DROP_PRE);
 tolua_constant(tolua_S,"hook","DROP_POST",HOOK_DROP_POST);
 tolua_constant(tolua_S,"hook","DROPPED_NEAR",HOOK_DROPPED_NEAR);
 tolua_constant(tolua_S,"hook","IDENTIFY_POST",HOOK_IDENTIFY_POST);
 tolua_constant(tolua_S,"hook","MOVE_PRE_DIR",HOOK_MOVE_PRE_DIR);
 tolua_constant(tolua_S,"hook","MOVE_PRE",HOOK_MOVE_PRE);
 tolua_constant(tolua_S,"hook","MOVE_OK",HOOK_MOVE_OK);
 tolua_constant(tolua_S,"hook","MOVE_POST",HOOK_MOVE_POST);
 tolua_constant(tolua_S,"hook","STAY_POST",HOOK_STAY_POST);
 tolua_constant(tolua_S,"hook","WILD_SMALL_MOVE_POST",HOOK_WILD_SMALL_MOVE_POST);
 tolua_constant(tolua_S,"hook","WILD_LARGE_NEW_AREA_PRE",HOOK_WILD_LARGE_NEW_AREA_PRE);
 tolua_constant(tolua_S,"hook","WILD_LARGE_NEW_AREA_POST",HOOK_WILD_LARGE_NEW_AREA_POST);
 tolua_constant(tolua_S,"hook","STAIR_PRE",HOOK_STAIR_PRE);
 tolua_constant(tolua_S,"hook","STAIR_DO",HOOK_STAIR_DO);
 tolua_constant(tolua_S,"hook","STAIR_POST",HOOK_STAIR_POST);
 tolua_constant(tolua_S,"hook","HIT_WALL",HOOK_HIT_WALL);
 tolua_constant(tolua_S,"hook","MONSTER_AI_TARGET",HOOK_MONSTER_AI_TARGET);
 tolua_constant(tolua_S,"hook","MONSTER_AI_MOVE",HOOK_MONSTER_AI_MOVE);
 tolua_constant(tolua_S,"hook","PLAYER_LEVEL",HOOK_PLAYER_LEVEL);
 tolua_constant(tolua_S,"hook","WIELD_PRE",HOOK_WIELD_PRE);
 tolua_constant(tolua_S,"hook","WIELD_INFO",HOOK_WIELD_INFO);
 tolua_constant(tolua_S,"hook","WIELD_POST",HOOK_WIELD_POST);
 tolua_constant(tolua_S,"hook","INIT",HOOK_INIT);
 tolua_constant(tolua_S,"hook","CALC_BONUS",HOOK_CALC_BONUS);
 tolua_constant(tolua_S,"hook","CALC_POWERS",HOOK_CALC_POWERS);
 tolua_constant(tolua_S,"hook","KEYPRESS",HOOK_KEYPRESS);
 tolua_constant(tolua_S,"hook","KEYPRESS_UNKNOWN",HOOK_KEYPRESS_UNKNOWN);
 tolua_constant(tolua_S,"hook","CHAT",HOOK_CHAT);
 tolua_constant(tolua_S,"hook","MON_SPEAK",HOOK_MON_SPEAK);
 tolua_constant(tolua_S,"hook","MKEY",HOOK_MKEY);
 tolua_constant(tolua_S,"hook","MKEY_HIDE",HOOK_MKEY_HIDE);
 tolua_constant(tolua_S,"hook","BIRTH_OBJECTS",HOOK_BIRTH_OBJECTS);
 tolua_constant(tolua_S,"hook","ACTIVATE_DESC",HOOK_ACTIVATE_DESC);
 tolua_constant(tolua_S,"hook","INIT_GAME",HOOK_INIT_GAME);
 tolua_constant(tolua_S,"hook","ACTIVATE_POWER",HOOK_ACTIVATE_POWER);
 tolua_constant(tolua_S,"hook","ITEM_NAME",HOOK_ITEM_NAME);
 tolua_constant(tolua_S,"hook","SAVE_GAME",HOOK_SAVE_GAME);
 tolua_constant(tolua_S,"hook","LOAD_GAME",HOOK_LOAD_GAME);
 tolua_constant(tolua_S,"hook","LEVEL_REGEN",HOOK_LEVEL_REGEN);
 tolua_constant(tolua_S,"hook","LEVEL_END_GEN",HOOK_LEVEL_END_GEN);
 tolua_constant(tolua_S,"hook","BUILDING_ACTION",HOOK_BUILDING_ACTION);
 tolua_constant(tolua_S,"hook","PROCESS_WORLD",HOOK_PROCESS_WORLD);
 tolua_constant(tolua_S,"hook","PROCESS_WORLD_WILD",HOOK_PROCESS_WORLD_WILD);
 tolua_constant(tolua_S,"hook","PROCESS_WORLD_NON_WILD",HOOK_PROCESS_WORLD_NON_WILD);
 tolua_constant(tolua_S,"hook","WIELD_SLOT",HOOK_WIELD_SLOT);
 tolua_constant(tolua_S,"hook","STORE_STOCK",HOOK_STORE_STOCK);
 tolua_constant(tolua_S,"hook","STORE_BUY",HOOK_STORE_BUY);
 tolua_constant(tolua_S,"hook","GEN_LEVEL_BEGIN",HOOK_GEN_LEVEL_BEGIN);
 tolua_constant(tolua_S,"hook","GEN_LEVEL_AFTER_LOAD",HOOK_GEN_LEVEL_AFTER_LOAD);
 tolua_constant(tolua_S,"hook","GET_PRE",HOOK_GET_PRE);
 tolua_constant(tolua_S,"hook","GET_POST",HOOK_GET_POST);
 tolua_constant(tolua_S,"hook","REDRAW",HOOK_REDRAW);
 tolua_constant(tolua_S,"hook","RECALC_SKILLS",HOOK_RECALC_SKILLS);
 tolua_constant(tolua_S,"hook","ENTER_DUNGEON",HOOK_ENTER_DUNGEON);
 tolua_constant(tolua_S,"hook","ENTERED_DUNGEON",HOOK_ENTERED_DUNGEON);
 tolua_constant(tolua_S,"hook","DIE_PRE",HOOK_DIE_PRE);
 tolua_constant(tolua_S,"hook","DIED_POST",HOOK_DIE_POST);
 tolua_constant(tolua_S,"hook","CALC_HP",HOOK_CALC_HP);
 tolua_constant(tolua_S,"hook","DAM_TYPE_COLOR",HOOK_DAM_COLOR);
 tolua_constant(tolua_S,"hook","DAM_TYPE_EXEC",HOOK_DAM_EXEC);
 tolua_constant(tolua_S,"hook","PROJECT_P_PRE_LUA",HOOK_PROJECT_P_PRE_LUA);
 tolua_constant(tolua_S,"hook","PROJECT_P_POST_LUA",HOOK_PROJECT_P_POST_LUA);
 tolua_constant(tolua_S,"hook","CALC_MANA",HOOK_CALC_MANA);
 tolua_constant(tolua_S,"hook","LOAD_GAME_END",HOOK_LOAD_GAME_END);
 tolua_constant(tolua_S,"hook","SAVE_GAME_END",HOOK_SAVE_GAME_END);
 tolua_constant(tolua_S,"hook","RECALL",HOOK_RECALL);
 tolua_constant(tolua_S,"hook","FOLLOW_GOD",HOOK_FOLLOW_GOD);
 tolua_constant(tolua_S,"hook","SACRIFICE_GOD",HOOK_SACRIFICE_GOD);
 tolua_constant(tolua_S,"hook","BODY_PARTS",HOOK_BODY_PARTS);
 tolua_constant(tolua_S,"hook","APPLY_MAGIC",HOOK_APPLY_MAGIC);
 tolua_constant(tolua_S,"hook","PLAYER_EXP",HOOK_PLAYER_EXP);
 tolua_constant(tolua_S,"hook","BIRTH",HOOK_BIRTH);
 tolua_constant(tolua_S,"hook","CALC_LITE",HOOK_CALC_LITE);
 tolua_constant(tolua_S,"hook","LEARN_ABILITY",HOOK_LEARN_ABILITY);
 tolua_constant(tolua_S,"hook","GAME_START",HOOK_GAME_START);
 tolua_constant(tolua_S,"hook","TAKEOFF_PRE",HOOK_TAKEOFF_PRE);
 tolua_constant(tolua_S,"hook","TAKEOFF_POST",HOOK_TAKEOFF_POST);
 tolua_constant(tolua_S,"hook","CALC_WEIGHT",HOOK_CALC_WEIGHT);
 tolua_constant(tolua_S,"hook","FORBID_TRAVEL",HOOK_FORBID_TRAVEL);
 tolua_constant(tolua_S,"hook","SAVE_REGISTER",HOOK_SAVE_REGISTER);
 tolua_constant(tolua_S,"hook","FLAG_GETTER",HOOK_FLAG_GETTER);
 tolua_constant(tolua_S,"hook","OBJECT_DESC",HOOK_OBJECT_DESC);
 tolua_constant(tolua_S,"hook","OBJECT_VALUE",HOOK_OBJECT_VALUE);
 tolua_constant(tolua_S,"hook","DEBUG_COMMAND",HOOK_DEBUG_COMMAND);
 tolua_constant(tolua_S,"hook","LEVEL_LOOP",HOOK_LEVEL_LOOP);
 tolua_constant(tolua_S,"hook","ITEM_NAME_INSERT",HOOK_ITEM_NAME_INSERT);
 tolua_constant(tolua_S,"hook","INFO_DATA_LOADED",HOOK_INFO_DATA_LOADED);
 tolua_constant(tolua_S,"hook","CALC_BONUS_END",HOOK_CALC_BONUS_END);
 tolua_constant(tolua_S,"hook","MODULE_SCREEN",HOOK_MODULE_SCREEN);
 tolua_constant(tolua_S,"hook","TARGET",HOOK_TARGET);
 tolua_constant(tolua_S,"hook","MONSTER_LEVEL",HOOK_MONSTER_LEVEL);
 tolua_constant(tolua_S,"hook","OBJECT_SIMILAR",HOOK_OBJECT_SIMILAR);
 tolua_constant(tolua_S,"hook","OBJECT_ABSORB",HOOK_OBJECT_ABSORB);
 tolua_constant(tolua_S,"hook","OBJECT_UNABSORB",HOOK_OBJECT_UNABSORB);
 tolua_constant(tolua_S,"hook","PIETY_CHANGE",HOOK_PIETY_CHANGE);
 tolua_constant(tolua_S,"hook","PLAYER_CAN_ENTER",HOOK_PLAYER_CAN_ENTER);
 tolua_constant(tolua_S,"hook","DESTROY_PRE",HOOK_DESTROY_PRE);
 tolua_constant(tolua_S,"hook","DESTROY_POST",HOOK_DESTROY_POST);
 tolua_constant(tolua_S,"hook","PLAYER_REGEN",HOOK_PLAYER_REGEN);
 tolua_constant(tolua_S,"hook","PLAYER_DESTROY_PRE",HOOK_PLAYER_DESTROY_PRE);
 tolua_constant(tolua_S,"hook","PLAYER_DESTROY_POST",HOOK_PLAYER_DESTROY_POST);
 tolua_constant(tolua_S,"hook","INIT_DONE",HOOK_INIT_DONE);
 tolua_constant(tolua_S,"hook","DISTURB",HOOK_DISTURB);
 tolua_constant(tolua_S,"hook","OBJECT_LEVEL_PRE",HOOK_OBJECT_LEVEL_PRE);
 tolua_constant(tolua_S,"hook","OBJECT_LEVEL_DO",HOOK_OBJECT_LEVEL_DO);
 tolua_constant(tolua_S,"hook","OBJECT_LEVEL_POST",HOOK_OBJECT_LEVEL_POST);
 tolua_constant(tolua_S,"hook","MONSTER_TAKE_HIT",HOOK_MONSTER_TAKE_HIT);
 tolua_constant(tolua_S,"hook","REPEAT_KEY_CHAIN_DESTROY",HOOK_REPEAT_KEY_CHAIN_DESTROY);
 tolua_constant(tolua_S,"hook","PLAYER_USED_ENERGY",HOOK_PLAYER_USED_ENERGY);
 tolua_constant(tolua_S,"hook","WILD_LARGE_CUSTOM_GENERATOR",HOOK_WILD_LARGE_CUSTOM_GENERATOR);
 tolua_constant(tolua_S,"hook","WILD_LARGE_TWEAK_GENERATOR",HOOK_WILD_LARGE_TWEAK_GENERATOR);
 tolua_constant(tolua_S,"hook","ALTER_GRID",HOOK_ALTER_GRID);
 tolua_constant(tolua_S,"hook","BODY_CHANGE_PRE",HOOK_BODY_CHANGE_PRE);
 tolua_constant(tolua_S,"hook","BODY_CHANGE",HOOK_BODY_CHANGE);
 tolua_constant(tolua_S,"hook","BODY_CHANGE_POST",HOOK_BODY_CHANGE_POST);
 tolua_constant(tolua_S,"hook","MON_CHANGE_FACTION_PRE",HOOK_MON_CHANGE_FACTION_PRE);
 tolua_constant(tolua_S,"hook","MON_CHANGE_FACTION_POST",HOOK_MON_CHANGE_FACTION_POST);
 tolua_constant(tolua_S,"hook","NEW_NEST",HOOK_NEW_NEST);
 tolua_constant(tolua_S,"hook","NEW_PIT",HOOK_NEW_PIT);
 tolua_constant(tolua_S,"hook","NEST_PIT_POST",HOOK_NEST_PIT_POST);
 tolua_constant(tolua_S,"hook","MONSTER_FORBID",HOOK_MONSTER_FORBID);
 tolua_constant(tolua_S,"hook","HARDCODED_MAX",MAX_HOOKS);
 tolua_tablevar(tolua_S,"hook","restart",toluaI_get_util_hook_restart,toluaI_set_util_hook_restart);
 tolua_function(tolua_S,"hook","dump",toluaI_util_hook_dump00);
 tolua_function(tolua_S,"hook","add_script",toluaI_util_hook_add_script00);
 tolua_function(tolua_S,"hook","del_by_name",toluaI_util_hook_del_by_name00);
 tolua_function(tolua_S,"hook","print",toluaI_util_hook_print00);
 tolua_function(tolua_S,"hook","__get",toluaI_util_hook___get00);
 tolua_constant(tolua_S,"hook","HOOK_TYPE_C",HOOK_TYPE_C);
 tolua_constant(tolua_S,"hook","HOOK_TYPE_LUA",HOOK_TYPE_LUA);
 tolua_cclass(tolua_S,"hooks_chain","");
 tolua_tablevar(tolua_S,"hooks_chain","name",toluaI_get_util_hooks_chain_name,toluaI_set_util_hooks_chain_name);
 tolua_tablevar(tolua_S,"hooks_chain","script",toluaI_get_util_hooks_chain_script,toluaI_set_util_hooks_chain_script);
 tolua_tablevar(tolua_S,"hooks_chain","type",toluaI_get_util_hooks_chain_type,toluaI_set_util_hooks_chain_type);
 tolua_tablevar(tolua_S,"hooks_chain","next",toluaI_get_util_hooks_chain_next,toluaI_set_util_hooks_chain_next);
 tolua_globalvar(tolua_S,"turn",toluaI_get_util_turn,toluaI_set_util_turn);
 tolua_globalvar(tolua_S,"old_turn",toluaI_get_util_old_turn,toluaI_set_util_old_turn);
 tolua_globalvar(tolua_S,"cur_wid",toluaI_get_util_cur_wid,toluaI_set_util_cur_wid);
 tolua_globalvar(tolua_S,"cur_hgt",toluaI_get_util_cur_hgt,toluaI_set_util_cur_hgt);
 tolua_function(tolua_S,NULL,"path_build",toluaI_util_path_build00);
 tolua_module(tolua_S,"message");
 tolua_function(tolua_S,"message","flush",toluaI_util_message_flush00);
 tolua_function(tolua_S,"message","cmsg_print",toluaI_util_message_cmsg_print00);
 tolua_function(tolua_S,"message","msg_print",toluaI_util_message_msg_print00);
 tolua_function(tolua_S,"message","add",toluaI_util_message_add00);
 tolua_function(tolua_S,"message","display",toluaI_util_message_display00);
 tolua_module(tolua_S,"term");
 tolua_function(tolua_S,"term","disturb",toluaI_util_term_disturb00);
 tolua_function(tolua_S,"term","move_cursor_relative",toluaI_util_term_move_cursor_relative00);
 tolua_tablevar(tolua_S,"term","inkey_scan",toluaI_get_util_term_inkey_scan,toluaI_set_util_term_inkey_scan);
 tolua_tablevar(tolua_S,"term","inkey_dir",toluaI_get_util_term_inkey_dir,toluaI_set_util_term_inkey_dir);
 tolua_function(tolua_S,"term","inkey",toluaI_util_term_inkey00);
 tolua_function(tolua_S,"term","outkey",toluaI_util_term_outkey00);
 tolua_function(tolua_S,"term","depth",toluaI_util_term_depth00);
 tolua_function(tolua_S,"term","save",toluaI_util_term_save00);
 tolua_function(tolua_S,"term","load",toluaI_util_term_load00);
 tolua_function(tolua_S,"term","save_to",toluaI_util_term_save_to00);
 tolua_function(tolua_S,"term","load_from",toluaI_util_term_load_from00);
 tolua_function(tolua_S,"term","c_put_str",toluaI_util_term_c_put_str00);
 tolua_function(tolua_S,"term","put_str",toluaI_util_term_put_str00);
 tolua_function(tolua_S,"term","c_prt",toluaI_util_term_c_prt00);
 tolua_function(tolua_S,"term","prt",toluaI_util_term_prt00);
 tolua_function(tolua_S,"term","clear_from",toluaI_util_term_clear_from00);
 tolua_function(tolua_S,"term","text_out_c",toluaI_util_term_text_out_c00);
 tolua_function(tolua_S,"term","screen_color",toluaI_util_term_screen_color00);
 tolua_function(tolua_S,"term","apply_keymap",toluaI_util_term_apply_keymap00);
 tolua_function(tolua_S,NULL,"get_check",toluaI_util_get_check00);
 tolua_function(tolua_S,NULL,"get_com",toluaI_util_get_com00);
 tolua_function(tolua_S,NULL,"get_quantity",toluaI_util_get_quantity00);
 tolua_function(tolua_S,NULL,"test_monster_name",toluaI_util_test_monster_name00);
 tolua_function(tolua_S,NULL,"test_item_name",toluaI_util_test_item_name00);
 tolua_function(tolua_S,NULL,"luck",toluaI_util_luck00);
 tolua_function(tolua_S,NULL,"value_scale",toluaI_util_value_scale00);
 tolua_function(tolua_S,NULL,"quit",toluaI_util_quit00);
 tolua_function(tolua_S,NULL,"tome_dofile",toluaI_util_tome_dofile00);
 tolua_function(tolua_S,NULL,"tome_dofile_anywhere",toluaI_util_tome_dofile_anywhere00);
 tolua_function(tolua_S,NULL,"dump_lua_stack",toluaI_util_dump_lua_stack00);
 tolua_function(tolua_S,NULL,"register_savefile",toluaI_util_register_savefile00);
 tolua_function(tolua_S,NULL,"save_nil_key",toluaI_util_save_nil_key00);
 tolua_function(tolua_S,NULL,"save_number_key",toluaI_util_save_number_key00);
 tolua_function(tolua_S,NULL,"save_string_key",toluaI_util_save_string_key00);
 tolua_cclass(tolua_S,"cave_type","");
 tolua_tablevar(tolua_S,"cave_type","info",toluaI_get_util_cave_type_info,toluaI_set_util_cave_type_info);
 tolua_tablevar(tolua_S,"cave_type","feat",toluaI_get_util_cave_type_feat,toluaI_set_util_cave_type_feat);
 tolua_tablevar(tolua_S,"cave_type","inventory",toluaI_get_util_cave_type_inventory,toluaI_set_util_cave_type_inventory);
 tolua_tablevar(tolua_S,"cave_type","m_idx",toluaI_get_util_cave_type_m_idx,toluaI_set_util_cave_type_m_idx);
 tolua_tablevar(tolua_S,"cave_type","flags",toluaI_get_util_cave_type_flags,toluaI_set_util_cave_type_flags);
 tolua_tablevar(tolua_S,"cave_type","mana",toluaI_get_util_cave_type_mana,toluaI_set_util_cave_type_mana);
 tolua_tablevar(tolua_S,"cave_type","mimic",toluaI_get_util_cave_type_mimic,toluaI_set_util_cave_type_mimic);
 tolua_tablevar(tolua_S,"cave_type","effect",toluaI_get_util_cave_type_effect,toluaI_set_util_cave_type_effect);
 tolua_tablevar(tolua_S,"cave_type","activations",toluaI_get_util_cave_type_activations,toluaI_set_util_cave_type_activations);
 tolua_globalvar(tolua_S,"ANGBAND_SYS",toluaI_get_util_ANGBAND_SYS,toluaI_set_util_ANGBAND_SYS);
 tolua_globalvar(tolua_S,"ANGBAND_KEYBOARD",toluaI_get_util_ANGBAND_KEYBOARD,toluaI_set_util_ANGBAND_KEYBOARD);
 tolua_globalvar(tolua_S,"TOME_GRAPHICS",toluaI_get_util_TOME_GRAPHICS,toluaI_set_util_TOME_GRAPHICS);
 tolua_globalvar(tolua_S,"TENGINE_DIR",toluaI_get_util_TENGINE_DIR,toluaI_set_util_TENGINE_DIR);
 tolua_globalvar(tolua_S,"TENGINE_DIR_SCORES",toluaI_get_util_TENGINE_DIR_SCORES,toluaI_set_util_TENGINE_DIR_SCORES);
 tolua_globalvar(tolua_S,"TENGINE_DIR_BONE",toluaI_get_util_TENGINE_DIR_BONE,toluaI_set_util_TENGINE_DIR_BONE);
 tolua_globalvar(tolua_S,"TENGINE_DIR_CMOV",toluaI_get_util_TENGINE_DIR_CMOV,toluaI_set_util_TENGINE_DIR_CMOV);
 tolua_globalvar(tolua_S,"TENGINE_DIR_DATA",toluaI_get_util_TENGINE_DIR_DATA,toluaI_set_util_TENGINE_DIR_DATA);
 tolua_globalvar(tolua_S,"TENGINE_DIR_DNGN",toluaI_get_util_TENGINE_DIR_DNGN,toluaI_set_util_TENGINE_DIR_DNGN);
 tolua_globalvar(tolua_S,"TENGINE_DIR_ENGINE",toluaI_get_util_TENGINE_DIR_ENGINE,toluaI_set_util_TENGINE_DIR_ENGINE);
 tolua_globalvar(tolua_S,"TENGINE_DIR_FILE",toluaI_get_util_TENGINE_DIR_FILE,toluaI_set_util_TENGINE_DIR_FILE);
 tolua_globalvar(tolua_S,"TENGINE_DIR_FONT",toluaI_get_util_TENGINE_DIR_FONT,toluaI_set_util_TENGINE_DIR_FONT);
 tolua_globalvar(tolua_S,"TENGINE_DIR_GRAF",toluaI_get_util_TENGINE_DIR_GRAF,toluaI_set_util_TENGINE_DIR_GRAF);
 tolua_globalvar(tolua_S,"TENGINE_DIR_HELP",toluaI_get_util_TENGINE_DIR_HELP,toluaI_set_util_TENGINE_DIR_HELP);
 tolua_globalvar(tolua_S,"TENGINE_DIR_INFO",toluaI_get_util_TENGINE_DIR_INFO,toluaI_set_util_TENGINE_DIR_INFO);
 tolua_globalvar(tolua_S,"TENGINE_DIR_MODULES",toluaI_get_util_TENGINE_DIR_MODULES,toluaI_set_util_TENGINE_DIR_MODULES);
 tolua_globalvar(tolua_S,"TENGINE_DIR_NOTE",toluaI_get_util_TENGINE_DIR_NOTE,toluaI_set_util_TENGINE_DIR_NOTE);
 tolua_globalvar(tolua_S,"TENGINE_DIR_PATCH",toluaI_get_util_TENGINE_DIR_PATCH,toluaI_set_util_TENGINE_DIR_PATCH);
 tolua_globalvar(tolua_S,"TENGINE_DIR_PREF",toluaI_get_util_TENGINE_DIR_PREF,toluaI_set_util_TENGINE_DIR_PREF);
 tolua_globalvar(tolua_S,"TENGINE_DIR_RAW",toluaI_get_util_TENGINE_DIR_RAW,toluaI_set_util_TENGINE_DIR_RAW);
 tolua_globalvar(tolua_S,"TENGINE_DIR_SAVE",toluaI_get_util_TENGINE_DIR_SAVE,toluaI_set_util_TENGINE_DIR_SAVE);
 tolua_globalvar(tolua_S,"TENGINE_DIR_SCPT",toluaI_get_util_TENGINE_DIR_SCPT,toluaI_set_util_TENGINE_DIR_SCPT);
 tolua_globalvar(tolua_S,"TENGINE_DIR_USER",toluaI_get_util_TENGINE_DIR_USER,toluaI_set_util_TENGINE_DIR_USER);
 tolua_globalvar(tolua_S,"TENGINE_DIR_XTRA",toluaI_get_util_TENGINE_DIR_XTRA,toluaI_set_util_TENGINE_DIR_XTRA);
 tolua_function(tolua_S,NULL,"distance",toluaI_util_distance00);
 tolua_function(tolua_S,NULL,"los",toluaI_util_los00);
 tolua_function(tolua_S,NULL,"cave_feat_get",toluaI_util_cave_feat_get00);
 tolua_function(tolua_S,NULL,"cave_feat_is",toluaI_util_cave_feat_is00);
 tolua_function(tolua_S,NULL,"cave_feat_get",toluaI_util_cave_feat_get01);
 tolua_function(tolua_S,NULL,"cave_mimic_is",toluaI_util_cave_mimic_is00);
 tolua_function(tolua_S,NULL,"cave",toluaI_util_cave00);
 tolua_function(tolua_S,NULL,"set_target",toluaI_util_set_target00);
 tolua_function(tolua_S,NULL,"get_target",toluaI_util_get_target00);
 tolua_globalarray(tolua_S,"m_allow_special",toluaI_get_util_m_allow_special,toluaI_set_util_m_allow_special);
 tolua_globalarray(tolua_S,"k_allow_special",toluaI_get_util_k_allow_special,toluaI_set_util_k_allow_special);
 tolua_globalarray(tolua_S,"a_allow_special",toluaI_get_util_a_allow_special,toluaI_set_util_a_allow_special);
 tolua_function(tolua_S,NULL,"cave_set_feat",toluaI_util_cave_set_feat00);
 tolua_function(tolua_S,NULL,"show_file",toluaI_util_show_file00);
 tolua_globalvar(tolua_S,"target_who",toluaI_get_util_target_who,toluaI_set_util_target_who);
 tolua_globalvar(tolua_S,"target_col",toluaI_get_util_target_col,toluaI_set_util_target_col);
 tolua_globalvar(tolua_S,"target_row",toluaI_get_util_target_row,toluaI_set_util_target_row);
 tolua_globalvar(tolua_S,"max_bact",toluaI_get_util_max_bact,toluaI_set_util_max_bact);
 tolua_globalarray(tolua_S,"ddd",toluaI_get_util_ddd,toluaI_set_util_ddd);
 tolua_globalarray(tolua_S,"ddx",toluaI_get_util_ddx,toluaI_set_util_ddx);
 tolua_globalarray(tolua_S,"ddy",toluaI_get_util_ddy,toluaI_set_util_ddy);
 tolua_globalarray(tolua_S,"ddx_ddd",toluaI_get_util_ddx_ddd,toluaI_set_util_ddx_ddd);
 tolua_globalarray(tolua_S,"ddy_ddd",toluaI_get_util_ddy_ddd,toluaI_set_util_ddy_ddd);
 tolua_function(tolua_S,NULL,"alloc_room",toluaI_util_alloc_room00);
 tolua_function(tolua_S,NULL,"is_quest",toluaI_util_is_quest00);
 tolua_function(tolua_S,NULL,"draw_box",toluaI_util_draw_box00);
 tolua_function(tolua_S,NULL,"input_box",toluaI_util_input_box00);
 tolua_function(tolua_S,NULL,"msg_box",toluaI_util_msg_box00);
 tolua_function(tolua_S,NULL,"rescale",toluaI_util_rescale00);
 tolua_function(tolua_S,NULL,"player_name",toluaI_util_player_name00);
 tolua_function(tolua_S,NULL,"make_temp_file",toluaI_util_make_temp_file00);
 tolua_function(tolua_S,NULL,"write_temp_file",toluaI_util_write_temp_file00);
 tolua_function(tolua_S,NULL,"close_temp_file",toluaI_util_close_temp_file00);
 tolua_function(tolua_S,NULL,"end_temp_file",toluaI_util_end_temp_file00);
 tolua_function(tolua_S,NULL,"get_temp_name",toluaI_util_get_temp_name00);
 tolua_function(tolua_S,NULL,"quark_str",toluaI_util_quark_str00);
 tolua_function(tolua_S,NULL,"quark_add",toluaI_util_quark_add00);
 tolua_globalvar(tolua_S,"game_module",toluaI_get_util_game_module,toluaI_set_util_game_module);
 tolua_function(tolua_S,NULL,"get_keymap_dir",toluaI_util_get_keymap_dir00);
 tolua_cclass(tolua_S,"timer_type","");
 tolua_tablevar(tolua_S,"timer_type","next",toluaI_get_util_timer_type_next,toluaI_set_util_timer_type_next);
 tolua_tablevar(tolua_S,"timer_type","enabled",toluaI_get_util_timer_type_enabled,toluaI_set_util_timer_type_enabled);
 tolua_tablevar(tolua_S,"timer_type","delay",toluaI_get_util_timer_type_delay,toluaI_set_util_timer_type_delay);
 tolua_tablevar(tolua_S,"timer_type","countdown",toluaI_get_util_timer_type_countdown,toluaI_set_util_timer_type_countdown);
 tolua_tablevar(tolua_S,"timer_type","callback",toluaI_get_util_timer_type_callback,toluaI_set_util_timer_type_callback);
 tolua_cclass(tolua_S,"list_type","");
 tolua_function(tolua_S,NULL,"create_list",toluaI_util_create_list00);
 tolua_function(tolua_S,NULL,"delete_list",toluaI_util_delete_list00);
 tolua_function(tolua_S,NULL,"add_to_list",toluaI_util_add_to_list00);
 tolua_function(tolua_S,NULL,"display_list",toluaI_util_display_list00);
 tolua_cclass(tolua_S,"flag_node","");
 tolua_tablevar(tolua_S,"flag_node","key",toluaI_get_util_flag_node_key,toluaI_set_util_flag_node_key);
 tolua_tablevar(tolua_S,"flag_node","flags",toluaI_get_util_flag_node_flags,toluaI_set_util_flag_node_flags);
 tolua_tablevar(tolua_S,"flag_node","next",toluaI_get_util_flag_node_next,toluaI_set_util_flag_node_next);
 tolua_function(tolua_S,NULL,"flag_node_get_ptr_type",toluaI_util_flag_node_get_ptr_type00);
 tolua_cclass(tolua_S,"flags_type","");
 tolua_tablearray(tolua_S,"flags_type","node",toluaI_get_util_flags_type_node,toluaI_set_util_flags_type_node);
 tolua_tablevar(tolua_S,"flags_type","size",toluaI_get_util_flags_type_size,toluaI_set_util_flags_type_size);
 tolua_tablevar(tolua_S,"flags_type","firstfree",toluaI_get_util_flags_type_firstfree,toluaI_set_util_flags_type_firstfree);
 tolua_cclass(tolua_S,"flags_holder_type","");
 tolua_tablevar(tolua_S,"flags_holder_type","flags",toluaI_get_util_flags_holder_type_flags,toluaI_set_util_flags_holder_type_flags);
 tolua_cclass(tolua_S,"flag_registry","");
 tolua_tablevar(tolua_S,"flag_registry","index",toluaI_get_util_flag_registry_index,toluaI_set_util_flag_registry_index);
 tolua_tablevar(tolua_S,"flag_registry","name",toluaI_get_util_flag_registry_name,toluaI_set_util_flag_registry_name);
 tolua_tablevar(tolua_S,"flag_registry","next",toluaI_get_util_flag_registry_next,toluaI_set_util_flag_registry_next);
 tolua_function(tolua_S,NULL,"register_flag",toluaI_util_register_flag00);
 tolua_function(tolua_S,NULL,"find_flag",toluaI_util_find_flag00);
 tolua_function(tolua_S,NULL,"find_flag_by_name",toluaI_util_find_flag_by_name00);
 tolua_function(tolua_S,NULL,"get_flag_name",toluaI_util_get_flag_name00);
 tolua_function(tolua_S,NULL,"get_flag_index",toluaI_util_get_flag_index00);
 tolua_function(tolua_S,NULL,"flag_set_full",toluaI_util_flag_set_full00);
 tolua_function(tolua_S,NULL,"flag_set",toluaI_util_flag_set00);
 tolua_function(tolua_S,NULL,"flag_remove",toluaI_util_flag_remove00);
 tolua_function(tolua_S,NULL,"flag_remove_rand",toluaI_util_flag_remove_rand00);
 tolua_function(tolua_S,NULL,"flag_get_node",toluaI_util_flag_get_node00);
 tolua_function(tolua_S,NULL,"flag_get",toluaI_util_flag_get00);
 tolua_function(tolua_S,NULL,"flag_get2",toluaI_util_flag_get200);
 tolua_function(tolua_S,NULL,"flag_get_flags",toluaI_util_flag_get_flags00);
 tolua_function(tolua_S,NULL,"flag_exists",toluaI_util_flag_exists00);
 tolua_function(tolua_S,NULL,"flag_new",toluaI_util_flag_new00);
 tolua_function(tolua_S,NULL,"flag_init",toluaI_util_flag_init00);
 tolua_function(tolua_S,NULL,"flag_free",toluaI_util_flag_free00);
 tolua_function(tolua_S,NULL,"flags_print",toluaI_util_flags_print00);
 tolua_function(tolua_S,NULL,"flag_remove_zap",toluaI_util_flag_remove_zap00);
 tolua_function(tolua_S,NULL,"flag_remove_rand_zap",toluaI_util_flag_remove_rand_zap00);
 tolua_function(tolua_S,NULL,"flag_first_free_key",toluaI_util_flag_first_free_key00);
 tolua_function(tolua_S,NULL,"flag_max_key",toluaI_util_flag_max_key00);
 tolua_function(tolua_S,NULL,"flag_empty",toluaI_util_flag_empty00);
 tolua_function(tolua_S,NULL,"flag_empty_zap",toluaI_util_flag_empty_zap00);
 tolua_function(tolua_S,NULL,"flag_dup",toluaI_util_flag_dup00);
 tolua_function(tolua_S,NULL,"flag_intersects",toluaI_util_flag_intersects00);
 tolua_function(tolua_S,NULL,"flag_copy",toluaI_util_flag_copy00);
 tolua_function(tolua_S,NULL,"flag_contains",toluaI_util_flag_contains00);
 tolua_function(tolua_S,NULL,"flag_contains_nb",toluaI_util_flag_contains_nb00);
 tolua_function(tolua_S,NULL,"flag_equal",toluaI_util_flag_equal00);
 tolua_function(tolua_S,NULL,"flag_used",toluaI_util_flag_used00);
 tolua_function(tolua_S,NULL,"flag_add",toluaI_util_flag_add00);
 tolua_function(tolua_S,NULL,"flag_add_new",toluaI_util_flag_add_new00);
 tolua_function(tolua_S,NULL,"flag_del",toluaI_util_flag_del00);
 tolua_function(tolua_S,NULL,"flag_add_value_from",toluaI_util_flag_add_value_from00);
 tolua_function(tolua_S,NULL,"flag_add_increase",toluaI_util_flag_add_increase00);
 tolua_function(tolua_S,NULL,"flag_add_higher",toluaI_util_flag_add_higher00);
 tolua_function(tolua_S,NULL,"flag_get_rand",toluaI_util_flag_get_rand00);
 tolua_function(tolua_S,NULL,"flag_get_ptr",toluaI_util_flag_get_ptr00);
 tolua_function(tolua_S,NULL,"flag_set_ptr",toluaI_util_flag_set_ptr00);
 tolua_function(tolua_S,NULL,"flag_shallow_dup",toluaI_util_flag_shallow_dup00);
 tolua_function(tolua_S,NULL,"flag_shallow_copy",toluaI_util_flag_shallow_copy00);
 tolua_function(tolua_S,NULL,"flag_shallow_free",toluaI_util_flag_shallow_free00);
 tolua_function(tolua_S,NULL,"flag_learn",toluaI_util_flag_learn00);
 tolua_function(tolua_S,NULL,"flag_is_known",toluaI_util_flag_is_known00);
 tolua_function(tolua_S,NULL,"flag_learn_all",toluaI_util_flag_learn_all00);
 tolua_constant(tolua_S,NULL,"FLAG_FLAG_USED",FLAG_FLAG_USED);
 tolua_constant(tolua_S,NULL,"FLAG_FLAG_RAND",FLAG_FLAG_RAND);
 tolua_constant(tolua_S,NULL,"FLAG_FLAG_MBONUS",FLAG_FLAG_MBONUS);
 tolua_constant(tolua_S,NULL,"FLAG_FLAG_BOTH",FLAG_FLAG_BOTH);
 tolua_constant(tolua_S,NULL,"FLAG_FLAG_RAND_MBONUS",FLAG_FLAG_RAND_MBONUS);
 tolua_constant(tolua_S,NULL,"FLAG_FLAG_FLAGS",FLAG_FLAG_FLAGS);
 tolua_constant(tolua_S,NULL,"FLAG_FLAG_PTR",FLAG_FLAG_PTR);
 tolua_constant(tolua_S,NULL,"FLAG_FLAG_KNOWN",FLAG_FLAG_KNOWN);
 tolua_constant(tolua_S,NULL,"FLAG_TYPE_OBJ",FLAG_TYPE_OBJ);
 tolua_constant(tolua_S,NULL,"FLAG_TYPE_STRING",FLAG_TYPE_STRING);
 tolua_constant(tolua_S,NULL,"FLAG_TYPE_MONSTER_BLOW",FLAG_TYPE_MONSTER_BLOW);
 tolua_constant(tolua_S,NULL,"FLAG_TYPE_MONSTER",FLAG_TYPE_MONSTER);
 tolua_constant(tolua_S,NULL,"FLAG_TYPE_DATA_BUFFER",FLAG_TYPE_DATA_BUFFER);
 tolua_constant(tolua_S,NULL,"FLAG_TYPE_STORE",FLAG_TYPE_STORE);
 tolua_function(tolua_S,NULL,"cave_clean_bold",toluaI_util_cave_clean_bold00);
 tolua_function(tolua_S,NULL,"cave_floor_bold",toluaI_util_cave_floor_bold00);
 tolua_function(tolua_S,NULL,"player_has_los_bold",toluaI_util_player_has_los_bold00);
 tolua_function(tolua_S,NULL,"panel_contains",toluaI_util_panel_contains00);
 tolua_function(tolua_S,NULL,"cave_empty_bold",toluaI_util_cave_empty_bold00);
 tolua_function(tolua_S,NULL,"get_rnd_line",toluaI_util_get_rnd_line00);
 tolua_function(tolua_S,NULL,"get_line",toluaI_util_get_line00);
 tolua_globalvar(tolua_S,"delay_factor",toluaI_get_util_delay_factor,toluaI_set_util_delay_factor);
 tolua_function(tolua_S,NULL,"target_able",toluaI_util_target_able00);
 tolua_function(tolua_S,NULL,"target_okay",toluaI_util_target_okay00);
 tolua_function(tolua_S,NULL,"target_set",toluaI_util_target_set00);
 tolua_function(tolua_S,NULL,"mmove2",toluaI_util_mmove200);
 tolua_function(tolua_S,NULL,"forget_los_tracks",toluaI_util_forget_los_tracks00);
 tolua_function(tolua_S,NULL,"print_rel",toluaI_util_print_rel00);
 tolua_function(tolua_S,NULL,"note_spot",toluaI_util_note_spot00);
 tolua_function(tolua_S,NULL,"lite_spot",toluaI_util_lite_spot00);
 tolua_globalvar(tolua_S,"max_panel_rows",toluaI_get_util_max_panel_rows,toluaI_set_util_max_panel_rows);
 tolua_globalvar(tolua_S,"max_panel_cols",toluaI_get_util_max_panel_cols,toluaI_set_util_max_panel_cols);
 tolua_globalvar(tolua_S,"panel_row_min",toluaI_get_util_panel_row_min,toluaI_set_util_panel_row_min);
 tolua_globalvar(tolua_S,"panel_row_max",toluaI_get_util_panel_row_max,toluaI_set_util_panel_row_max);
 tolua_globalvar(tolua_S,"panel_col_min",toluaI_get_util_panel_col_min,toluaI_set_util_panel_col_min);
 tolua_globalvar(tolua_S,"panel_col_max",toluaI_get_util_panel_col_max,toluaI_set_util_panel_col_max);
 tolua_globalvar(tolua_S,"panel_col_prt",toluaI_get_util_panel_col_prt,toluaI_set_util_panel_col_prt);
 tolua_globalvar(tolua_S,"panel_row_prt",toluaI_get_util_panel_row_prt,toluaI_set_util_panel_row_prt);
 tolua_globalvar(tolua_S,"ROW_MAP",toluaI_get_util_ROW_MAP,toluaI_set_util_ROW_MAP);
 tolua_globalvar(tolua_S,"COL_MAP",toluaI_get_util_COL_MAP,toluaI_set_util_COL_MAP);
 tolua_globalvar(tolua_S,"ROW_MAP_HGT",toluaI_get_util_ROW_MAP_HGT,toluaI_set_util_ROW_MAP_HGT);
 tolua_globalvar(tolua_S,"COL_MAP_WID",toluaI_get_util_COL_MAP_WID,toluaI_set_util_COL_MAP_WID);
 tolua_globalvar(tolua_S,"message_row",toluaI_get_util_message_row,toluaI_set_util_message_row);
 tolua_globalvar(tolua_S,"message_col",toluaI_get_util_message_col,toluaI_set_util_message_col);
 tolua_globalvar(tolua_S,"message_col_max",toluaI_get_util_message_col_max,toluaI_set_util_message_col_max);
 tolua_function(tolua_S,NULL,"grab_string",toluaI_util_grab_string00);
 tolua_function(tolua_S,NULL,"do_cmd_redraw",toluaI_util_do_cmd_redraw00);
 tolua_function(tolua_S,NULL,"panel_bounds",toluaI_util_panel_bounds00);
 tolua_globalvar(tolua_S,"done_loading_data_files",toluaI_get_util_done_loading_data_files,toluaI_set_util_done_loading_data_files);
 tolua_function(tolua_S,NULL,"in_bounds",toluaI_util_in_bounds00);
 tolua_function(tolua_S,NULL,"in_bounds2",toluaI_util_in_bounds200);
 tolua_module(tolua_S,"time");
 tolua_tablevar(tolua_S,"time","DAY",toluaI_get_util_time_DAY,toluaI_set_util_time_DAY);
 tolua_tablevar(tolua_S,"time","YEAR",toluaI_get_util_time_YEAR,toluaI_set_util_time_YEAR);
 tolua_tablevar(tolua_S,"time","HOUR",toluaI_get_util_time_HOUR,toluaI_set_util_time_HOUR);
 tolua_tablevar(tolua_S,"time","MINUTE",toluaI_get_util_time_MINUTE,toluaI_set_util_time_MINUTE);
 tolua_tablevar(tolua_S,"time","DAY_START",toluaI_get_util_time_DAY_START,toluaI_set_util_time_DAY_START);
 tolua_function(tolua_S,"time","__exec_module",toluaI_util_time___exec_module00);
 tolua_function(tolua_S,"time","__new_timer",toluaI_util_time___new_timer00);
 tolua_function(tolua_S,"time","del_timer",toluaI_util_time_del_timer00);
 tolua_function(tolua_S,NULL,"cli_add",toluaI_util_cli_add00);
 tolua_function(tolua_S,NULL,"do_cmd_automatizer",toluaI_util_do_cmd_automatizer00);
 tolua_function(tolua_S,NULL,"squeltch_grid",toluaI_util_squeltch_grid00);
 tolua_function(tolua_S,NULL,"squeltch_inventory",toluaI_util_squeltch_inventory00);
 tolua_function(tolua_S,NULL,"do_cmd_options_win",toluaI_util_do_cmd_options_win00);
 tolua_module(tolua_S,"__wrap_ptr");
 tolua_function(tolua_S,"__wrap_ptr","to_string",toluaI_util___wrap_ptr_to_string00);
 tolua_function(tolua_S,"__wrap_ptr","to_object",toluaI_util___wrap_ptr_to_object00);
 tolua_function(tolua_S,"__wrap_ptr","to_monster_blow",toluaI_util___wrap_ptr_to_monster_blow00);
 tolua_function(tolua_S,"__wrap_ptr","to_monster_type",toluaI_util___wrap_ptr_to_monster_type00);
 tolua_function(tolua_S,"__wrap_ptr","to_store_type",toluaI_util___wrap_ptr_to_store_type00);
 tolua_function(tolua_S,"__wrap_ptr","dup_buffer_to_void",toluaI_util___wrap_ptr_dup_buffer_to_void00);
 tolua_function(tolua_S,"__wrap_ptr","to_buffer",toluaI_util___wrap_ptr_to_buffer00);
 tolua_function(tolua_S,"__wrap_ptr","dup_string_to_void",toluaI_util___wrap_ptr_dup_string_to_void00);
 tolua_module(tolua_S,"game_options");
 tolua_tablevar(tolua_S,"game_options","rogue_like_commands",toluaI_get_util_game_options_rogue_like_commands,toluaI_set_util_game_options_rogue_like_commands);
 tolua_tablevar(tolua_S,"game_options","quick_messages",toluaI_get_util_game_options_quick_messages,toluaI_set_util_game_options_quick_messages);
 tolua_tablevar(tolua_S,"game_options","other_query_flag",toluaI_get_util_game_options_other_query_flag,toluaI_set_util_game_options_other_query_flag);
 tolua_tablevar(tolua_S,"game_options","carry_query_flag",toluaI_get_util_game_options_carry_query_flag,toluaI_set_util_game_options_carry_query_flag);
 tolua_tablevar(tolua_S,"game_options","use_old_target",toluaI_get_util_game_options_use_old_target,toluaI_set_util_game_options_use_old_target);
 tolua_tablevar(tolua_S,"game_options","always_pickup",toluaI_get_util_game_options_always_pickup,toluaI_set_util_game_options_always_pickup);
 tolua_tablevar(tolua_S,"game_options","prompt_pickup_heavy",toluaI_get_util_game_options_prompt_pickup_heavy,toluaI_set_util_game_options_prompt_pickup_heavy);
 tolua_tablevar(tolua_S,"game_options","always_repeat",toluaI_get_util_game_options_always_repeat,toluaI_set_util_game_options_always_repeat);
 tolua_tablevar(tolua_S,"game_options","depth_in_feet",toluaI_get_util_game_options_depth_in_feet,toluaI_set_util_game_options_depth_in_feet);
 tolua_tablevar(tolua_S,"game_options","stack_force_notes",toluaI_get_util_game_options_stack_force_notes,toluaI_set_util_game_options_stack_force_notes);
 tolua_tablevar(tolua_S,"game_options","stack_force_costs",toluaI_get_util_game_options_stack_force_costs,toluaI_set_util_game_options_stack_force_costs);
 tolua_tablevar(tolua_S,"game_options","show_labels",toluaI_get_util_game_options_show_labels,toluaI_set_util_game_options_show_labels);
 tolua_tablevar(tolua_S,"game_options","show_weights",toluaI_get_util_game_options_show_weights,toluaI_set_util_game_options_show_weights);
 tolua_tablevar(tolua_S,"game_options","show_inven_graph",toluaI_get_util_game_options_show_inven_graph,toluaI_set_util_game_options_show_inven_graph);
 tolua_tablevar(tolua_S,"game_options","show_equip_graph",toluaI_get_util_game_options_show_equip_graph,toluaI_set_util_game_options_show_equip_graph);
 tolua_tablevar(tolua_S,"game_options","show_store_graph",toluaI_get_util_game_options_show_store_graph,toluaI_set_util_game_options_show_store_graph);
 tolua_tablevar(tolua_S,"game_options","show_choices",toluaI_get_util_game_options_show_choices,toluaI_set_util_game_options_show_choices);
 tolua_tablevar(tolua_S,"game_options","show_details",toluaI_get_util_game_options_show_details,toluaI_set_util_game_options_show_details);
 tolua_tablevar(tolua_S,"game_options","ring_bell",toluaI_get_util_game_options_ring_bell,toluaI_set_util_game_options_ring_bell);
 tolua_tablevar(tolua_S,"game_options","use_color",toluaI_get_util_game_options_use_color,toluaI_set_util_game_options_use_color);
 tolua_tablevar(tolua_S,"game_options","find_ignore_stairs",toluaI_get_util_game_options_find_ignore_stairs,toluaI_set_util_game_options_find_ignore_stairs);
 tolua_tablevar(tolua_S,"game_options","find_ignore_doors",toluaI_get_util_game_options_find_ignore_doors,toluaI_set_util_game_options_find_ignore_doors);
 tolua_tablevar(tolua_S,"game_options","find_cut",toluaI_get_util_game_options_find_cut,toluaI_set_util_game_options_find_cut);
 tolua_tablevar(tolua_S,"game_options","find_examine",toluaI_get_util_game_options_find_examine,toluaI_set_util_game_options_find_examine);
 tolua_tablevar(tolua_S,"game_options","disturb_move",toluaI_get_util_game_options_disturb_move,toluaI_set_util_game_options_disturb_move);
 tolua_tablevar(tolua_S,"game_options","disturb_near",toluaI_get_util_game_options_disturb_near,toluaI_set_util_game_options_disturb_near);
 tolua_tablevar(tolua_S,"game_options","disturb_panel",toluaI_get_util_game_options_disturb_panel,toluaI_set_util_game_options_disturb_panel);
 tolua_tablevar(tolua_S,"game_options","disturb_detect",toluaI_get_util_game_options_disturb_detect,toluaI_set_util_game_options_disturb_detect);
 tolua_tablevar(tolua_S,"game_options","disturb_state",toluaI_get_util_game_options_disturb_state,toluaI_set_util_game_options_disturb_state);
 tolua_tablevar(tolua_S,"game_options","disturb_minor",toluaI_get_util_game_options_disturb_minor,toluaI_set_util_game_options_disturb_minor);
 tolua_tablevar(tolua_S,"game_options","disturb_other",toluaI_get_util_game_options_disturb_other,toluaI_set_util_game_options_disturb_other);
 tolua_tablevar(tolua_S,"game_options","alert_hitpoint",toluaI_get_util_game_options_alert_hitpoint,toluaI_set_util_game_options_alert_hitpoint);
 tolua_tablevar(tolua_S,"game_options","alert_failure",toluaI_get_util_game_options_alert_failure,toluaI_set_util_game_options_alert_failure);
 tolua_tablevar(tolua_S,"game_options","last_words",toluaI_get_util_game_options_last_words,toluaI_set_util_game_options_last_words);
 tolua_tablevar(tolua_S,"game_options","speak_unique",toluaI_get_util_game_options_speak_unique,toluaI_set_util_game_options_speak_unique);
 tolua_tablevar(tolua_S,"game_options","auto_destroy",toluaI_get_util_game_options_auto_destroy,toluaI_set_util_game_options_auto_destroy);
 tolua_tablevar(tolua_S,"game_options","confirm_wear",toluaI_get_util_game_options_confirm_wear,toluaI_set_util_game_options_confirm_wear);
 tolua_tablevar(tolua_S,"game_options","confirm_stairs",toluaI_get_util_game_options_confirm_stairs,toluaI_set_util_game_options_confirm_stairs);
 tolua_tablevar(tolua_S,"game_options","disturb_pets",toluaI_get_util_game_options_disturb_pets,toluaI_set_util_game_options_disturb_pets);
 tolua_tablevar(tolua_S,"game_options","easy_inven",toluaI_get_util_game_options_easy_inven,toluaI_set_util_game_options_easy_inven);
 tolua_tablevar(tolua_S,"game_options","easy_open",toluaI_get_util_game_options_easy_open,toluaI_set_util_game_options_easy_open);
 tolua_tablevar(tolua_S,"game_options","easy_disarm",toluaI_get_util_game_options_easy_disarm,toluaI_set_util_game_options_easy_disarm);
 tolua_tablevar(tolua_S,"game_options","easy_tunnel",toluaI_get_util_game_options_easy_tunnel,toluaI_set_util_game_options_easy_tunnel);
 tolua_tablevar(tolua_S,"game_options","auto_haggle",toluaI_get_util_game_options_auto_haggle,toluaI_set_util_game_options_auto_haggle);
 tolua_tablevar(tolua_S,"game_options","auto_scum",toluaI_get_util_game_options_auto_scum,toluaI_set_util_game_options_auto_scum);
 tolua_tablevar(tolua_S,"game_options","stack_allow_items",toluaI_get_util_game_options_stack_allow_items,toluaI_set_util_game_options_stack_allow_items);
 tolua_tablevar(tolua_S,"game_options","stack_allow_wands",toluaI_get_util_game_options_stack_allow_wands,toluaI_set_util_game_options_stack_allow_wands);
 tolua_tablevar(tolua_S,"game_options","expand_look",toluaI_get_util_game_options_expand_look,toluaI_set_util_game_options_expand_look);
 tolua_tablevar(tolua_S,"game_options","expand_list",toluaI_get_util_game_options_expand_list,toluaI_set_util_game_options_expand_list);
 tolua_tablevar(tolua_S,"game_options","view_perma_grids",toluaI_get_util_game_options_view_perma_grids,toluaI_set_util_game_options_view_perma_grids);
 tolua_tablevar(tolua_S,"game_options","view_torch_grids",toluaI_get_util_game_options_view_torch_grids,toluaI_set_util_game_options_view_torch_grids);
 tolua_tablevar(tolua_S,"game_options","monster_lite",toluaI_get_util_game_options_monster_lite,toluaI_set_util_game_options_monster_lite);
 tolua_tablevar(tolua_S,"game_options","dungeon_align",toluaI_get_util_game_options_dungeon_align,toluaI_set_util_game_options_dungeon_align);
 tolua_tablevar(tolua_S,"game_options","dungeon_stair",toluaI_get_util_game_options_dungeon_stair,toluaI_set_util_game_options_dungeon_stair);
 tolua_tablevar(tolua_S,"game_options","player_symbols",toluaI_get_util_game_options_player_symbols,toluaI_set_util_game_options_player_symbols);
 tolua_tablevar(tolua_S,"game_options","plain_descriptions",toluaI_get_util_game_options_plain_descriptions,toluaI_set_util_game_options_plain_descriptions);
 tolua_tablevar(tolua_S,"game_options","smart_learn",toluaI_get_util_game_options_smart_learn,toluaI_set_util_game_options_smart_learn);
 tolua_tablevar(tolua_S,"game_options","smart_cheat",toluaI_get_util_game_options_smart_cheat,toluaI_set_util_game_options_smart_cheat);
 tolua_tablevar(tolua_S,"game_options","stupid_monsters",toluaI_get_util_game_options_stupid_monsters,toluaI_set_util_game_options_stupid_monsters);
 tolua_tablevar(tolua_S,"game_options","small_levels",toluaI_get_util_game_options_small_levels,toluaI_set_util_game_options_small_levels);
 tolua_tablevar(tolua_S,"game_options","empty_levels",toluaI_get_util_game_options_empty_levels,toluaI_set_util_game_options_empty_levels);
 tolua_tablevar(tolua_S,"game_options","view_reduce_lite",toluaI_get_util_game_options_view_reduce_lite,toluaI_set_util_game_options_view_reduce_lite);
 tolua_tablevar(tolua_S,"game_options","view_reduce_view",toluaI_get_util_game_options_view_reduce_view,toluaI_set_util_game_options_view_reduce_view);
 tolua_tablevar(tolua_S,"game_options","avoid_abort",toluaI_get_util_game_options_avoid_abort,toluaI_set_util_game_options_avoid_abort);
 tolua_tablevar(tolua_S,"game_options","avoid_shimmer",toluaI_get_util_game_options_avoid_shimmer,toluaI_set_util_game_options_avoid_shimmer);
 tolua_tablevar(tolua_S,"game_options","avoid_other",toluaI_get_util_game_options_avoid_other,toluaI_set_util_game_options_avoid_other);
 tolua_tablevar(tolua_S,"game_options","flush_failure",toluaI_get_util_game_options_flush_failure,toluaI_set_util_game_options_flush_failure);
 tolua_tablevar(tolua_S,"game_options","flush_disturb",toluaI_get_util_game_options_flush_disturb,toluaI_set_util_game_options_flush_disturb);
 tolua_tablevar(tolua_S,"game_options","flush_command",toluaI_get_util_game_options_flush_command,toluaI_set_util_game_options_flush_command);
 tolua_tablevar(tolua_S,"game_options","fresh_before",toluaI_get_util_game_options_fresh_before,toluaI_set_util_game_options_fresh_before);
 tolua_tablevar(tolua_S,"game_options","fresh_after",toluaI_get_util_game_options_fresh_after,toluaI_set_util_game_options_fresh_after);
 tolua_tablevar(tolua_S,"game_options","fresh_message",toluaI_get_util_game_options_fresh_message,toluaI_set_util_game_options_fresh_message);
 tolua_tablevar(tolua_S,"game_options","compress_savefile",toluaI_get_util_game_options_compress_savefile,toluaI_set_util_game_options_compress_savefile);
 tolua_tablevar(tolua_S,"game_options","hilite_player",toluaI_get_util_game_options_hilite_player,toluaI_set_util_game_options_hilite_player);
 tolua_tablevar(tolua_S,"game_options","view_lited_grid",toluaI_get_util_game_options_view_lited_grid,toluaI_set_util_game_options_view_lited_grid);
 tolua_tablevar(tolua_S,"game_options","center_player",toluaI_get_util_game_options_center_player,toluaI_set_util_game_options_center_player);
 tolua_tablevar(tolua_S,"game_options","exp_need",toluaI_get_util_game_options_exp_need,toluaI_set_util_game_options_exp_need);
 tolua_tablevar(tolua_S,"game_options","auto_more",toluaI_get_util_game_options_auto_more,toluaI_set_util_game_options_auto_more);
 tolua_tablevar(tolua_S,"game_options","player_char_health",toluaI_get_util_game_options_player_char_health,toluaI_set_util_game_options_player_char_health);
 tolua_tablevar(tolua_S,"game_options","linear_stats",toluaI_get_util_game_options_linear_stats,toluaI_set_util_game_options_linear_stats);
 tolua_tablevar(tolua_S,"game_options","inventory_no_move",toluaI_get_util_game_options_inventory_no_move,toluaI_set_util_game_options_inventory_no_move);
 tolua_tablevar(tolua_S,"game_options","maximize",toluaI_get_util_game_options_maximize,toluaI_set_util_game_options_maximize);
 tolua_tablevar(tolua_S,"game_options","preserve",toluaI_get_util_game_options_preserve,toluaI_set_util_game_options_preserve);
 tolua_tablevar(tolua_S,"game_options","autoroll",toluaI_get_util_game_options_autoroll,toluaI_set_util_game_options_autoroll);
 tolua_tablevar(tolua_S,"game_options","point_based",toluaI_get_util_game_options_point_based,toluaI_set_util_game_options_point_based);
 tolua_tablevar(tolua_S,"game_options","permanent_levels",toluaI_get_util_game_options_permanent_levels,toluaI_set_util_game_options_permanent_levels);
 tolua_tablevar(tolua_S,"game_options","ironman_rooms",toluaI_get_util_game_options_ironman_rooms,toluaI_set_util_game_options_ironman_rooms);
 tolua_tablevar(tolua_S,"game_options","take_notes",toluaI_get_util_game_options_take_notes,toluaI_set_util_game_options_take_notes);
 tolua_tablevar(tolua_S,"game_options","auto_notes",toluaI_get_util_game_options_auto_notes,toluaI_set_util_game_options_auto_notes);
 tolua_tablevar(tolua_S,"game_options","fast_autoroller",toluaI_get_util_game_options_fast_autoroller,toluaI_set_util_game_options_fast_autoroller);
 tolua_tablevar(tolua_S,"game_options","joke_monsters",toluaI_get_util_game_options_joke_monsters,toluaI_set_util_game_options_joke_monsters);
 tolua_tablevar(tolua_S,"game_options","always_small_level",toluaI_get_util_game_options_always_small_level,toluaI_set_util_game_options_always_small_level);
 tolua_tablevar(tolua_S,"game_options","cheat_peek",toluaI_get_util_game_options_cheat_peek,toluaI_set_util_game_options_cheat_peek);
 tolua_tablevar(tolua_S,"game_options","cheat_hear",toluaI_get_util_game_options_cheat_hear,toluaI_set_util_game_options_cheat_hear);
 tolua_tablevar(tolua_S,"game_options","cheat_room",toluaI_get_util_game_options_cheat_room,toluaI_set_util_game_options_cheat_room);
 tolua_tablevar(tolua_S,"game_options","cheat_xtra",toluaI_get_util_game_options_cheat_xtra,toluaI_set_util_game_options_cheat_xtra);
 tolua_tablevar(tolua_S,"game_options","cheat_know",toluaI_get_util_game_options_cheat_know,toluaI_set_util_game_options_cheat_know);
 tolua_tablevar(tolua_S,"game_options","cheat_live",toluaI_get_util_game_options_cheat_live,toluaI_set_util_game_options_cheat_live);
 tolua_tablevar(tolua_S,"game_options","autosave_t",toluaI_get_util_game_options_autosave_t,toluaI_set_util_game_options_autosave_t);
 tolua_tablevar(tolua_S,"game_options","autosave_l",toluaI_get_util_game_options_autosave_l,toluaI_set_util_game_options_autosave_l);
 tolua_tablevar(tolua_S,"game_options","autosave_freq",toluaI_get_util_game_options_autosave_freq,toluaI_set_util_game_options_autosave_freq);
 tolua_tablevar(tolua_S,"game_options","automatizer_enabled",toluaI_get_util_game_options_automatizer_enabled,toluaI_set_util_game_options_automatizer_enabled);
 tolua_tablevar(tolua_S,"game_options","character_generated",toluaI_get_util_game_options_character_generated,toluaI_set_util_game_options_character_generated);
 tolua_tablevar(tolua_S,"game_options","hitpoint_warn",toluaI_get_util_game_options_hitpoint_warn,toluaI_set_util_game_options_hitpoint_warn);
 tolua_tablevar(tolua_S,"game_options","delay_factor",toluaI_get_util_game_options_delay_factor,toluaI_set_util_game_options_delay_factor);
 tolua_tablevar(tolua_S,"game_options","option_ingame_help",toluaI_get_util_game_options_option_ingame_help,toluaI_set_util_game_options_option_ingame_help);
 tolua_tablevar(tolua_S,"game_options","savefile_compress",toluaI_get_util_game_options_savefile_compress,toluaI_set_util_game_options_savefile_compress);
 tolua_tablevar(tolua_S,"game_options","keypad_autorun",toluaI_get_util_game_options_keypad_autorun,toluaI_set_util_game_options_keypad_autorun);
 tolua_module(tolua_S,"repeat_keys");
 tolua_tablevar(tolua_S,"repeat_keys","cur_idx",toluaI_get_util_repeat_keys_cur_idx,toluaI_set_util_repeat_keys_cur_idx);
 tolua_tablevar(tolua_S,"repeat_keys","cur_cnt",toluaI_get_util_repeat_keys_cur_cnt,toluaI_set_util_repeat_keys_cur_cnt);
 tolua_function(tolua_S,"repeat_keys","pop",toluaI_util_repeat_keys_pop00);
 tolua_function(tolua_S,"repeat_keys","push",toluaI_util_repeat_keys_push00);
 tolua_function(tolua_S,"repeat_keys","pull",toluaI_util_repeat_keys_pull00);
 tolua_function(tolua_S,"repeat_keys","check",toluaI_util_repeat_keys_check00);
 tolua_function(tolua_S,"repeat_keys","get_count",toluaI_util_repeat_keys_get_count00);
 tolua_function(tolua_S,NULL,"add_new_inventory_slot",toluaI_util_add_new_inventory_slot00);
 tolua_cclass(tolua_S,"buffer_type","");
 tolua_tablevar(tolua_S,"buffer_type","len",toluaI_get_util_buffer_type_len,toluaI_set_util_buffer_type_len);
 tolua_tablearray(tolua_S,"buffer_type","buf",toluaI_get_util_buffer_type_buf,toluaI_set_util_buffer_type_buf);
 tolua_function(tolua_S,NULL,"data_buffer_alloc",toluaI_util_data_buffer_alloc00);
 tolua_function(tolua_S,NULL,"data_buffer_delete",toluaI_util_data_buffer_delete00);
 tolua_function(tolua_S,NULL,"data_buffer_dup",toluaI_util_data_buffer_dup00);
 tolua_module(tolua_S,"loadsave");
 tolua_function(tolua_S,"loadsave","do_store",toluaI_util_loadsave_do_store00);
 tolua_function(tolua_S,"loadsave","do_monster",toluaI_util_loadsave_do_monster00);
 tolua_function(tolua_S,"loadsave","do_item",toluaI_util_loadsave_do_item00);
 tolua_function(tolua_S,"loadsave","do_char",toluaI_util_loadsave_do_char00);
 tolua_function(tolua_S,"loadsave","do_byte",toluaI_util_loadsave_do_byte00);
 tolua_function(tolua_S,"loadsave","do_u16b",toluaI_util_loadsave_do_u16b00);
 tolua_function(tolua_S,"loadsave","do_s16b",toluaI_util_loadsave_do_s16b00);
 tolua_function(tolua_S,"loadsave","do_u32b",toluaI_util_loadsave_do_u32b00);
 tolua_function(tolua_S,"loadsave","do_s32b",toluaI_util_loadsave_do_s32b00);
 tolua_function(tolua_S,"loadsave","do_string_nolen",toluaI_util_loadsave_do_string_nolen00);
 tolua_function(tolua_S,"loadsave","do_flags",toluaI_util_loadsave_do_flags00);
 tolua_function(tolua_S,"loadsave","load_dungeon",toluaI_util_loadsave_load_dungeon00);
 tolua_function(tolua_S,"loadsave","save_dungeon",toluaI_util_loadsave_save_dungeon00);
 tolua_function(tolua_S,"loadsave","save_player",toluaI_util_loadsave_save_player00);
 tolua_function(tolua_S,"loadsave","load_player",toluaI_util_loadsave_load_player00);
 tolua_constant(tolua_S,"loadsave","LS_LOAD",LS_LOAD);
 tolua_constant(tolua_S,"loadsave","LS_SAVE",LS_SAVE);
 tolua_function(tolua_S,NULL,"sroot",toluaI_util_sroot00);
 tolua_function(tolua_S,NULL,"is_a_vowel",toluaI_util_is_a_vowel00);
 tolua_function(tolua_S,NULL,"check_prevent_cmd_aux",toluaI_util_check_prevent_cmd_aux00);
 tolua_function(tolua_S,NULL,"negate_bits",toluaI_util_negate_bits00);
 tolua_constant(tolua_S,NULL,"DISARM_SUCCESS",DISARM_SUCCESS);
 tolua_constant(tolua_S,NULL,"DISARM_FAILURE",DISARM_FAILURE);
 tolua_constant(tolua_S,NULL,"DISARM_TRIGGERED",DISARM_TRIGGERED);
 tolua_function(tolua_S,NULL,"place_trap",toluaI_util_place_trap00);
 tolua_function(tolua_S,NULL,"get_num_traps",toluaI_util_get_num_traps00);
 tolua_function(tolua_S,NULL,"get_num_location_traps",toluaI_util_get_num_location_traps00);
 tolua_function(tolua_S,NULL,"get_num_object_traps",toluaI_util_get_num_object_traps00);
 tolua_function(tolua_S,NULL,"do_play_cmovie",toluaI_util_do_play_cmovie00);
 tolua_globalvar(tolua_S,"character_icky",toluaI_get_util_character_icky,toluaI_set_util_character_icky);
 tolua_module(tolua_S,"pathfind");
 tolua_function(tolua_S,"pathfind","get_first",toluaI_util_pathfind_get_first00);
 tolua_function(tolua_S,"pathfind","get_next",toluaI_util_pathfind_get_next00);
 tolua_function(tolua_S,NULL,"savefile",toluaI_util_savefile00);
 tolua_module(tolua_S,"time");
 tolua_function(tolua_S,"time","__rt_add",toluaI_util_time___rt_add00);
 tolua_function(tolua_S,"time","__rt_del",toluaI_util_time___rt_del00);
 tolua_function(tolua_S,NULL,"index_to_label",toluaI_util_index_to_label00);
 tolua_globalvar(tolua_S,"ENGINE_DIR",toluaI_get_util_ENGINE_DIR,toluaI_set_util_ENGINE_DIR);
 tolua_globalvar(tolua_S,"ENGINE_DIR_DEP",toluaI_get_util_ENGINE_DIR_DEP,toluaI_set_util_ENGINE_DIR_DEP);
 tolua_function(tolua_S,NULL,"sdl_utl_fast_draw",toluaI_util_sdl_utl_fast_draw00);
 tolua_globalarray(tolua_S,"__extract_energy",toluaI_get_util___extract_energy,toluaI_set_util___extract_energy);
 return 1;
}
/* Close function */
void tolua_util_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FALSE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ESCAPE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"color");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"conv_color");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"hook");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"hooks_chain");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"turn"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"old_turn"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"cur_wid"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"cur_hgt"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"path_build");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"message");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"term");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_check");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_com");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_quantity");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"test_monster_name");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"test_item_name");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"luck");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"value_scale");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"quit");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"tome_dofile");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"tome_dofile_anywhere");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"dump_lua_stack");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"register_savefile");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"save_nil_key");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"save_number_key");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"save_string_key");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave_type");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"ANGBAND_SYS"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"ANGBAND_KEYBOARD"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TOME_GRAPHICS"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_SCORES"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_BONE"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_CMOV"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_DATA"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_DNGN"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_ENGINE"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_FILE"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_FONT"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_GRAF"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_HELP"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_INFO"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_MODULES"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_NOTE"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_PATCH"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_PREF"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_RAW"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_SAVE"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_SCPT"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_USER"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"TENGINE_DIR_XTRA"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"distance");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"los");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave_feat_get");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave_feat_is");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave_feat_get");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave_mimic_is");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"set_target");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_target");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"m_allow_special");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"k_allow_special");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"a_allow_special");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave_set_feat");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"show_file");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"target_who"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"target_col"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"target_row"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_bact"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ddd");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ddx");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ddy");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ddx_ddd");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"ddy_ddd");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"alloc_room");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_quest");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"draw_box");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"input_box");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"msg_box");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"rescale");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"player_name");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"make_temp_file");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"write_temp_file");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"close_temp_file");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"end_temp_file");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_temp_name");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"quark_str");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"quark_add");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"game_module"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_keymap_dir");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"timer_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"list_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"create_list");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"delete_list");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"add_to_list");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"display_list");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_node");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_node_get_ptr_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flags_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flags_holder_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_registry");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"register_flag");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_flag");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"find_flag_by_name");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_flag_name");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_flag_index");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_set_full");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_set");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_remove");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_remove_rand");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_get_node");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_get");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_get2");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_get_flags");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_exists");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_new");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_init");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_free");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flags_print");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_remove_zap");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_remove_rand_zap");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_first_free_key");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_max_key");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_empty");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_empty_zap");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_dup");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_intersects");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_copy");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_contains");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_contains_nb");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_equal");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_used");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_add");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_add_new");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_del");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_add_value_from");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_add_increase");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_add_higher");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_get_rand");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_get_ptr");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_set_ptr");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_shallow_dup");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_shallow_copy");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_shallow_free");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_learn");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_is_known");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"flag_learn_all");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_FLAG_USED");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_FLAG_RAND");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_FLAG_MBONUS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_FLAG_BOTH");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_FLAG_RAND_MBONUS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_FLAG_FLAGS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_FLAG_PTR");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_FLAG_KNOWN");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_TYPE_OBJ");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_TYPE_STRING");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_TYPE_MONSTER_BLOW");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_TYPE_MONSTER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_TYPE_DATA_BUFFER");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"FLAG_TYPE_STORE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave_clean_bold");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave_floor_bold");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"player_has_los_bold");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"panel_contains");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cave_empty_bold");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_rnd_line");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_line");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"delay_factor"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"target_able");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"target_okay");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"target_set");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"mmove2");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"forget_los_tracks");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"print_rel");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"note_spot");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"lite_spot");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_panel_rows"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"max_panel_cols"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"panel_row_min"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"panel_row_max"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"panel_col_min"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"panel_col_max"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"panel_col_prt"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"panel_row_prt"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"ROW_MAP"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"COL_MAP"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"ROW_MAP_HGT"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"COL_MAP_WID"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"message_row"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"message_col"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"message_col_max"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"grab_string");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_cmd_redraw");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"panel_bounds");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"done_loading_data_files"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"in_bounds");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"in_bounds2");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"time");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"cli_add");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_cmd_automatizer");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"squeltch_grid");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"squeltch_inventory");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_cmd_options_win");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"__wrap_ptr");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"game_options");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"repeat_keys");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"add_new_inventory_slot");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"buffer_type");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"data_buffer_alloc");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"data_buffer_delete");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"data_buffer_dup");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"loadsave");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"sroot");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"is_a_vowel");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"check_prevent_cmd_aux");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"negate_bits");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DISARM_SUCCESS");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DISARM_FAILURE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"DISARM_TRIGGERED");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"place_trap");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_num_traps");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_num_location_traps");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"get_num_object_traps");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"do_play_cmovie");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"character_icky"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"pathfind");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"savefile");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"time");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"index_to_label");
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"ENGINE_DIR"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_getglobals(tolua_S);
 lua_pushstring(tolua_S,"ENGINE_DIR_DEP"); lua_pushnil(tolua_S); lua_rawset(tolua_S,-3);
 lua_pop(tolua_S,1);
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"sdl_utl_fast_draw");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"__extract_energy");
}
