/*
** Lua binding: sound
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:50 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_sound_open (lua_State* tolua_S);
void tolua_sound_close (lua_State* tolua_S);

#include "angband.h"
#include "music.h"

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Mix_Chunk");
 tolua_usertype(tolua_S,"Mix_Music");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* function: music_load_music */
static int toluaI_sound_sound___load_music00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr file = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  Mix_Music* toluaI_ret = (Mix_Music*)  music_load_music(file);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Mix_Music"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__load_music'.");
 return 0;
}

/* function: music_play_music */
static int toluaI_sound_sound___play_music00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Mix_Music"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  Mix_Music* music = ((Mix_Music*)  tolua_getusertype(tolua_S,1,0));
  s32b loop = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b fade_in_time = ((s32b)  tolua_getnumber(tolua_S,3,0));
 {
  bool toluaI_ret = (bool)  music_play_music(music,loop,fade_in_time);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__play_music'.");
 return 0;
}

/* function: Mix_PlayingMusic */
static int toluaI_sound_sound_is_playing_music00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  Mix_PlayingMusic();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_playing_music'.");
 return 0;
}

/* function: Mix_PausedMusic */
static int toluaI_sound_sound_is_paused_music00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  bool toluaI_ret = (bool)  Mix_PausedMusic();
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_paused_music'.");
 return 0;
}

/* function: Mix_PauseMusic */
static int toluaI_sound_sound_pause_music00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  Mix_PauseMusic();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pause_music'.");
 return 0;
}

/* function: Mix_FadingMusic */
static int toluaI_sound_sound_is_fading_music00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  s32b toluaI_ret = (s32b)  Mix_FadingMusic();
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_fading_music'.");
 return 0;
}

/* function: Mix_ResumeMusic */
static int toluaI_sound_sound_resume_music00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  Mix_ResumeMusic();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resume_music'.");
 return 0;
}

/* function: Mix_FadeOutMusic */
static int toluaI_sound_sound_fade_out_music00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b fade_out_time = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  Mix_FadeOutMusic(fade_out_time);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fade_out_music'.");
 return 0;
}

/* function: Mix_HaltMusic */
static int toluaI_sound_sound_stop_music00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  Mix_HaltMusic();
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stop_music'.");
 return 0;
}

/* function: Mix_VolumeMusic */
static int toluaI_sound_sound_volume_music00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b volume = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  Mix_VolumeMusic(volume);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'volume_music'.");
 return 0;
}

/* function: Mix_SetMusicPosition */
static int toluaI_sound_sound_set_position_music00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b pos = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  Mix_SetMusicPosition(pos);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_position_music'.");
 return 0;
}

/* function: music_load_sample */
static int toluaI_sound_sound___load_sample00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  cptr file = ((cptr)  tolua_getstring(tolua_S,1,0));
 {
  Mix_Chunk* toluaI_ret = (Mix_Chunk*)  music_load_sample(file);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"Mix_Chunk"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__load_sample'.");
 return 0;
}

/* function: music_play_sample */
static int toluaI_sound_sound___play_sample00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"Mix_Chunk"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  Mix_Chunk* sample = ((Mix_Chunk*)  tolua_getusertype(tolua_S,1,0));
  s32b channel = ((s32b)  tolua_getnumber(tolua_S,2,0));
  s32b loop = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b fade_in_time = ((s32b)  tolua_getnumber(tolua_S,4,0));
 {
  bool toluaI_ret = (bool)  music_play_sample(sample,channel,loop,fade_in_time);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '__play_sample'.");
 return 0;
}

/* get function: sound_not_available */
static int toluaI_get_sound_sound_not_available(lua_State* tolua_S)
{
 tolua_pushbool(tolua_S,(int)sound_not_available);
 return 1;
}

/* set function: sound_not_available */
static int toluaI_set_sound_sound_not_available(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,tolua_tag(tolua_S,"bool"),0))
 TOLUA_ERR_ASSIGN;
  sound_not_available = ((bool)  tolua_getbool(tolua_S,1,0));
 return 0;
}

/* Open function */
int tolua_sound_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_module(tolua_S,"sound");
 tolua_function(tolua_S,"sound","__load_music",toluaI_sound_sound___load_music00);
 tolua_function(tolua_S,"sound","__play_music",toluaI_sound_sound___play_music00);
 tolua_function(tolua_S,"sound","is_playing_music",toluaI_sound_sound_is_playing_music00);
 tolua_function(tolua_S,"sound","is_paused_music",toluaI_sound_sound_is_paused_music00);
 tolua_function(tolua_S,"sound","pause_music",toluaI_sound_sound_pause_music00);
 tolua_function(tolua_S,"sound","is_fading_music",toluaI_sound_sound_is_fading_music00);
 tolua_function(tolua_S,"sound","resume_music",toluaI_sound_sound_resume_music00);
 tolua_function(tolua_S,"sound","fade_out_music",toluaI_sound_sound_fade_out_music00);
 tolua_function(tolua_S,"sound","stop_music",toluaI_sound_sound_stop_music00);
 tolua_function(tolua_S,"sound","volume_music",toluaI_sound_sound_volume_music00);
 tolua_function(tolua_S,"sound","set_position_music",toluaI_sound_sound_set_position_music00);
 tolua_function(tolua_S,"sound","__load_sample",toluaI_sound_sound___load_sample00);
 tolua_function(tolua_S,"sound","__play_sample",toluaI_sound_sound___play_sample00);
 tolua_tablevar(tolua_S,"sound","not_available",toluaI_get_sound_sound_not_available,toluaI_set_sound_sound_not_available);
 return 1;
}
/* Close function */
void tolua_sound_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"sound");
}
