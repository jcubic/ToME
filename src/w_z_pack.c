/*
** Lua binding: z_pack
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:57 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_z_pack_open (lua_State* tolua_S);
void tolua_z_pack_close (lua_State* tolua_S);

#include "angband.h"
static s32b get_current_window() { s32b i; for (i = 0; i < ANGBAND_TERM_MAX; i++) if (angband_term[i] == Term) return i; return 0; }
static bool set_current_window(s32b i) { if (angband_term[i]) { Term_activate(angband_term[i]); return TRUE; } else return FALSE; }
static flags_type *window_settings(s32b i) { return &window_flag[i]; }
static s32b lua_rand_int(s32b m) {return rand_int(m);}
static s32b lua_rand_range(s32b A, s32b B) {return ((A) + (rand_int(1+(B)-(A))));}
static s32b lua_rand_spread(s32b A, s32b D) {return ((A) + (rand_int(1+(D)+(D))) - (D));}
static bool lua_magik(s32b P) {return (rand_int(100) < (P));}

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"flags_type");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* function: get_current_window */
static int toluaI_z_pack_term_get_current_window00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  s32b toluaI_ret = (s32b)  get_current_window();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_current_window'.");
 return 0;
}

/* function: set_current_window */
static int toluaI_z_pack_term_set_current_window00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  set_current_window(i);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_current_window'.");
 return 0;
}

/* function: window_settings */
static int toluaI_z_pack_term_window_settings00(lua_State* tolua_S)
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
  flags_type* toluaI_ret = (flags_type*)  window_settings(i);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"flags_type"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'window_settings'.");
 return 0;
}

/* get function: Term_xtra_long */
static int toluaI_get_z_pack_term_Term_xtra_long(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)Term_xtra_long);
 return 1;
}

/* set function: Term_xtra_long */
static int toluaI_set_z_pack_term_Term_xtra_long(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  Term_xtra_long = ((long)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: Term_xtra */
static int toluaI_z_pack_term_xtra00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b n = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b v = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  errr toluaI_ret = (errr)  Term_xtra(n,v);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'xtra'.");
 return 0;
}

/* function: Term_set_cursor */
static int toluaI_z_pack_term_set_cursor00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b v = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  errr toluaI_ret = (errr)  Term_set_cursor(v);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_cursor'.");
 return 0;
}

/* function: move_cursor */
static int toluaI_z_pack_term_gotoxy00(lua_State* tolua_S)
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
  move_cursor(y,x);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gotoxy'.");
 return 0;
}

/* function: Term_putch */
static int toluaI_z_pack_term_putch00(lua_State* tolua_S)
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
  s32b x = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b y = ((s32b)  tolua_getnumber(tolua_S,2,0));
  byte a = ((byte)  tolua_getnumber(tolua_S,3,0));
  char c = ((char)  tolua_getnumber(tolua_S,4,0));
 {
  errr toluaI_ret = (errr)  Term_putch(x,y,a,c);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'putch'.");
 return 0;
}

/* function: Term_putstr */
static int toluaI_z_pack_term_putstr00(lua_State* tolua_S)
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
  s32b n = ((s32b)  tolua_getnumber(tolua_S,3,0));
  byte a = ((byte)  tolua_getnumber(tolua_S,4,0));
  cptr s = ((cptr)  tolua_getstring(tolua_S,5,0));
 {
  errr toluaI_ret = (errr)  Term_putstr(x,y,n,a,s);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'putstr'.");
 return 0;
}

/* function: Term_clear */
static int toluaI_z_pack_term_clear00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  errr toluaI_ret = (errr)  Term_clear();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.");
 return 0;
}

/* function: Term_redraw */
static int toluaI_z_pack_term_redraw00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  errr toluaI_ret = (errr)  Term_redraw();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'redraw'.");
 return 0;
}

/* function: Term_redraw_section */
static int toluaI_z_pack_term_redraw_section00(lua_State* tolua_S)
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
  s32b x1 = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b y1 = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b x2 = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b y2 = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  errr toluaI_ret = (errr)  Term_redraw_section(x1,y1,x2,y2);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'redraw_section'.");
 return 0;
}

/* function: Term_get_size */
static int toluaI_z_pack_term_get_size00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,1) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b w = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b h = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  errr toluaI_ret = (errr)  Term_get_size(&w,&h);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 tolua_pushnumber(tolua_S,(long)w);
 tolua_pushnumber(tolua_S,(long)h);
 }
 }
 return 3;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_size'.");
 return 0;
}

/* function: Term_fresh */
static int toluaI_z_pack_term_fresh00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  errr toluaI_ret = (errr)  Term_fresh();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fresh'.");
 return 0;
}

/* function: Term_addstr */
static int toluaI_z_pack_term_addstr00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  s32b n = ((s32b)  tolua_getnumber(tolua_S,1,0));
  byte a = ((byte)  tolua_getnumber(tolua_S,2,0));
  cptr s = ((cptr)  tolua_getstring(tolua_S,3,0));
 {
  errr toluaI_ret = (errr)  Term_addstr(n,a,s);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addstr'.");
 return 0;
}

/* function: Term_erase */
static int toluaI_z_pack_term_erase00(lua_State* tolua_S)
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
  s32b y = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b n = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  errr toluaI_ret = (errr)  Term_erase(x,y,n);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'erase'.");
 return 0;
}

/* function: lua_rand_int */
static int toluaI_z_pack_rng_number00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b m = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  lua_rand_int(m);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'number'.");
 return 0;
}

/* function: lua_rand_range */
static int toluaI_z_pack_rng_range00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b A = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b B = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  lua_rand_range(A,B);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'range'.");
 return 0;
}

/* function: lua_rand_spread */
static int toluaI_z_pack_rng_spread00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b A = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b D = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  lua_rand_spread(A,D);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'spread'.");
 return 0;
}

/* function: lua_magik */
static int toluaI_z_pack_rng_percent00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b P = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  lua_magik(P);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'percent'.");
 return 0;
}

/* get function: Rand_quick */
static int toluaI_get_z_pack_rng_Rand_quick(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)Rand_quick);
 return 1;
}

/* set function: Rand_quick */
static int toluaI_set_z_pack_rng_Rand_quick(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  Rand_quick = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* get function: Rand_value */
static int toluaI_get_z_pack_rng_Rand_value(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(long)Rand_value);
 return 1;
}

/* set function: Rand_value */
static int toluaI_set_z_pack_rng_Rand_value(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TNUMBER,0))
 TOLUA_ERR_ASSIGN;
  Rand_value = ((u32b)  tolua_getnumber(tolua_S,1,0));
 return 0;
}

/* function: damroll */
static int toluaI_z_pack_rng_roll00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b num = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b sides = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  damroll(num,sides);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'roll'.");
 return 0;
}

/* function: maxroll */
static int toluaI_z_pack_rng_maxroll00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b num = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b sides = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  maxroll(num,sides);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'maxroll'.");
 return 0;
}

/* function: randnor */
static int toluaI_z_pack_rng_normal00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  s32b mean = ((s32b)  tolua_getnumber(tolua_S,1,0));
  s32b stand = ((s32b)  tolua_getnumber(tolua_S,2,0));
 {
  s16b toluaI_ret = (s16b)  randnor(mean,stand);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'normal'.");
 return 0;
}

/* Open function */
int tolua_z_pack_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_EVENT",TERM_XTRA_EVENT);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_FLUSH",TERM_XTRA_FLUSH);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_CLEAR",TERM_XTRA_CLEAR);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_SHAPE",TERM_XTRA_SHAPE);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_FROSH",TERM_XTRA_FROSH);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_FRESH",TERM_XTRA_FRESH);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_NOISE",TERM_XTRA_NOISE);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_SOUND",TERM_XTRA_SOUND);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_BORED",TERM_XTRA_BORED);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_REACT",TERM_XTRA_REACT);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_ALIVE",TERM_XTRA_ALIVE);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_LEVEL",TERM_XTRA_LEVEL);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_DELAY",TERM_XTRA_DELAY);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_GET_DELAY",TERM_XTRA_GET_DELAY);
 tolua_constant(tolua_S,NULL,"TERM_XTRA_EFFECTS_DELAY",TERM_XTRA_EFFECTS_DELAY);
 tolua_module(tolua_S,"term");
 tolua_function(tolua_S,"term","get_current_window",toluaI_z_pack_term_get_current_window00);
 tolua_function(tolua_S,"term","set_current_window",toluaI_z_pack_term_set_current_window00);
 tolua_function(tolua_S,"term","window_settings",toluaI_z_pack_term_window_settings00);
 tolua_constant(tolua_S,"term","MAX_WINDOW",ANGBAND_TERM_MAX);
 tolua_tablevar(tolua_S,"term","Term_xtra_long",toluaI_get_z_pack_term_Term_xtra_long,toluaI_set_z_pack_term_Term_xtra_long);
 tolua_function(tolua_S,"term","xtra",toluaI_z_pack_term_xtra00);
 tolua_function(tolua_S,"term","set_cursor",toluaI_z_pack_term_set_cursor00);
 tolua_function(tolua_S,"term","gotoxy",toluaI_z_pack_term_gotoxy00);
 tolua_function(tolua_S,"term","putch",toluaI_z_pack_term_putch00);
 tolua_function(tolua_S,"term","putstr",toluaI_z_pack_term_putstr00);
 tolua_function(tolua_S,"term","clear",toluaI_z_pack_term_clear00);
 tolua_function(tolua_S,"term","redraw",toluaI_z_pack_term_redraw00);
 tolua_function(tolua_S,"term","redraw_section",toluaI_z_pack_term_redraw_section00);
 tolua_function(tolua_S,"term","get_size",toluaI_z_pack_term_get_size00);
 tolua_function(tolua_S,"term","fresh",toluaI_z_pack_term_fresh00);
 tolua_function(tolua_S,"term","addstr",toluaI_z_pack_term_addstr00);
 tolua_function(tolua_S,"term","erase",toluaI_z_pack_term_erase00);
 tolua_module(tolua_S,"rng");
 tolua_function(tolua_S,"rng","number",toluaI_z_pack_rng_number00);
 tolua_function(tolua_S,"rng","range",toluaI_z_pack_rng_range00);
 tolua_function(tolua_S,"rng","spread",toluaI_z_pack_rng_spread00);
 tolua_function(tolua_S,"rng","percent",toluaI_z_pack_rng_percent00);
 tolua_tablevar(tolua_S,"rng","Rand_quick",toluaI_get_z_pack_rng_Rand_quick,toluaI_set_z_pack_rng_Rand_quick);
 tolua_tablevar(tolua_S,"rng","Rand_value",toluaI_get_z_pack_rng_Rand_value,toluaI_set_z_pack_rng_Rand_value);
 tolua_function(tolua_S,"rng","roll",toluaI_z_pack_rng_roll00);
 tolua_function(tolua_S,"rng","maxroll",toluaI_z_pack_rng_maxroll00);
 tolua_function(tolua_S,"rng","normal",toluaI_z_pack_rng_normal00);
 return 1;
}
/* Close function */
void tolua_z_pack_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_EVENT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_FLUSH");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_CLEAR");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_SHAPE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_FROSH");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_FRESH");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_NOISE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_SOUND");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_BORED");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_REACT");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_ALIVE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_LEVEL");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_DELAY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_GET_DELAY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"TERM_XTRA_EFFECTS_DELAY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"term");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"rng");
}
