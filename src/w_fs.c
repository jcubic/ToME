/*
** Lua binding: physfs
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:39 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_physfs_open (lua_State* tolua_S);
void tolua_physfs_close (lua_State* tolua_S);

#include "angband.h"
static cptr lua_my_fgets(PHYSFS_file *fff) { char buf[1024]; bool ret = my_fgets(fff, buf, 1024); return (!ret) ? buf : NULL; }
extern bool create_zip(cptr zip_path, flags_type *files, cptr signature);

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"PHYSFS_file");
 tolua_usertype(tolua_S,"flags_type");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* function: PHYSFS_getLastError */
static int toluaI_physfs_fs_get_last_error00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  const char* toluaI_ret = (const char*)  PHYSFS_getLastError();
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_last_error'.");
 return 0;
}

/* function: PHYSFS_addToSearchPath */
static int toluaI_physfs_fs_add_to_search_path00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const char* newDir = ((const char*)  tolua_getstring(tolua_S,1,0));
  s32b appendToPath = ((s32b)  tolua_getnumber(tolua_S,2,TRUE));
 {
  bool toluaI_ret = (bool)  PHYSFS_addToSearchPath(newDir,appendToPath);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_to_search_path'.");
 return 0;
}

/* function: PHYSFS_removeFromSearchPath */
static int toluaI_physfs_fs_remove_from_search_path00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* oldDir = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  PHYSFS_removeFromSearchPath(oldDir);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'remove_from_search_path'.");
 return 0;
}

/* function: PHYSFS_isDirectory */
static int toluaI_physfs_fs_is_directory00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* fname = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  PHYSFS_isDirectory(fname);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_directory'.");
 return 0;
}

/* function: PHYSFS_getRealDir */
static int toluaI_physfs_fs_get_real_dir00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* filename = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  const char* toluaI_ret = (const char*)  PHYSFS_getRealDir(filename);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_real_dir'.");
 return 0;
}

/* function: PHYSFS_getDirSeparator */
static int toluaI_physfs_fs_get_dir_separator00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  const char* toluaI_ret = (const char*)  PHYSFS_getDirSeparator();
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_dir_separator'.");
 return 0;
}

/* function: PHYSFS_getDependentPath */
static int toluaI_physfs_fs_get_dependent_path00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* fname = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  char* toluaI_ret = (char*)  PHYSFS_getDependentPath(fname);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_dependent_path'.");
 return 0;
}

/* function: PHYSFS_mkdir */
static int toluaI_physfs_fs_mkdir00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* dirName = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  PHYSFS_mkdir(dirName);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'mkdir'.");
 return 0;
}

/* function: PHYSFS_delete */
static int toluaI_physfs_fs_delete00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* filename = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  PHYSFS_delete(filename);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.");
 return 0;
}

/* function: PHYSFS_rename */
static int toluaI_physfs_fs_rename00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const char* filename = ((const char*)  tolua_getstring(tolua_S,1,0));
  const char* new_filename = ((const char*)  tolua_getstring(tolua_S,2,0));
 {
  bool toluaI_ret = (bool)  PHYSFS_rename(filename,new_filename);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rename'.");
 return 0;
}

/* function: PHYSFS_getBaseDir */
static int toluaI_physfs_fs_get_base_dir00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  const char* toluaI_ret = (const char*)  PHYSFS_getBaseDir();
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_base_dir'.");
 return 0;
}

/* function: PHYSFS_getUserDir */
static int toluaI_physfs_fs_get_user_dir00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  const char* toluaI_ret = (const char*)  PHYSFS_getUserDir();
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_user_dir'.");
 return 0;
}

/* function: PHYSFS_getWriteDir */
static int toluaI_physfs_fs_get_write_dir00(lua_State* tolua_S)
{
 if (
 !tolua_isnoobj(tolua_S,1)
 )
 goto tolua_lerror;
 else
 {
 {
  const char* toluaI_ret = (const char*)  PHYSFS_getWriteDir();
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_write_dir'.");
 return 0;
}

/* function: PHYSFS_setWriteDir */
static int toluaI_physfs_fs_set_write_dir00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* newDir = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  PHYSFS_setWriteDir(newDir);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_write_dir'.");
 return 0;
}

/* function: PHYSFS_getLastModTime */
static int toluaI_physfs_fs_last_modified00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  const char* filename = ((const char*)  tolua_getstring(tolua_S,1,0));
 {
  s32b toluaI_ret = (s32b)  PHYSFS_getLastModTime(filename);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'last_modified'.");
 return 0;
}

/* function: file_exist */
static int toluaI_physfs_fs_file_exist00(lua_State* tolua_S)
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
  bool toluaI_ret = (bool)  file_exist(buf);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'file_exist'.");
 return 0;
}

/* get function: PHYSFS_specific_mount */
static int toluaI_get_physfs_fs_PHYSFS_specific_mount(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)PHYSFS_specific_mount);
 return 1;
}

/* set function: PHYSFS_specific_mount */
static int toluaI_set_physfs_fs_PHYSFS_specific_mount(lua_State* tolua_S)
{
 if (!tolua_istype(tolua_S,1,LUA_TSTRING,0))
 TOLUA_ERR_ASSIGN;
  PHYSFS_specific_mount = ((char*)  tolua_getstring(tolua_S,1,0));
 return 0;
}

/* function: my_fopen */
static int toluaI_physfs_fs_open00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  cptr file = ((cptr)  tolua_getstring(tolua_S,1,0));
  cptr mode = ((cptr)  tolua_getstring(tolua_S,2,0));
 {
  PHYSFS_file* toluaI_ret = (PHYSFS_file*)  my_fopen(file,mode);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"PHYSFS_file"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'open'.");
 return 0;
}

/* function: my_fclose */
static int toluaI_physfs_fs_close00(lua_State* tolua_S)
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
  errr toluaI_ret = (errr)  my_fclose(fff);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'close'.");
 return 0;
}

/* function: my_fputs */
static int toluaI_physfs_fs_write00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"PHYSFS_file"),0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  PHYSFS_file* fff = ((PHYSFS_file*)  tolua_getusertype(tolua_S,1,0));
  cptr buf = ((cptr)  tolua_getstring(tolua_S,2,0));
 {
  errr toluaI_ret = (errr)  my_fputs(fff,buf);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'write'.");
 return 0;
}

/* function: PHYSFS_write */
static int toluaI_physfs_fs_write_data00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"PHYSFS_file"),0) ||
 !tolua_istype(tolua_S,2,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,3,LUA_TNUMBER,0) ||
 !tolua_istype(tolua_S,4,LUA_TNUMBER,1) ||
 !tolua_isnoobj(tolua_S,5)
 )
 goto tolua_lerror;
 else
 {
  PHYSFS_file* fff = ((PHYSFS_file*)  tolua_getusertype(tolua_S,1,0));
  cptr buf = ((cptr)  tolua_getstring(tolua_S,2,0));
  s32b obj_size = ((s32b)  tolua_getnumber(tolua_S,3,0));
  s32b obj_nb = ((s32b)  tolua_getnumber(tolua_S,4,1));
 {
  errr toluaI_ret = (errr)  PHYSFS_write(fff,buf,obj_size,obj_nb);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'write_data'.");
 return 0;
}

/* function: fd_kill */
static int toluaI_physfs_fs_delete01(lua_State* tolua_S)
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
  errr toluaI_ret = (errr)  fd_kill(file);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 return toluaI_physfs_fs_delete00(tolua_S);
}

/* function: set_loadsave_stream */
static int toluaI_physfs_fs_set_loadsave_stream00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"PHYSFS_file"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  PHYSFS_file* f = ((PHYSFS_file*)  tolua_getusertype(tolua_S,1,0));
 {
  set_loadsave_stream(f);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_loadsave_stream'.");
 return 0;
}

/* function: lua_my_fgets */
static int toluaI_physfs_fs_read00(lua_State* tolua_S)
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
  cptr toluaI_ret = (cptr)  lua_my_fgets(fff);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'read'.");
 return 0;
}

/* function: create_zip */
static int toluaI_physfs_fs_create_zip00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,tolua_tag(tolua_S,"flags_type"),0) ||
 !tolua_istype(tolua_S,3,LUA_TSTRING,1) ||
 !tolua_isnoobj(tolua_S,4)
 )
 goto tolua_lerror;
 else
 {
  cptr zip_path = ((cptr)  tolua_getstring(tolua_S,1,0));
  flags_type* files = ((flags_type*)  tolua_getusertype(tolua_S,2,0));
  cptr signature = ((cptr)  tolua_getstring(tolua_S,3,NULL));
 {
  bool toluaI_ret = (bool)  create_zip(zip_path,files,signature);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_zip'.");
 return 0;
}

/* get function: PRIVATE_USER_PATH */
static int toluaI_get_physfs_fs_TOME_USER_PATH(lua_State* tolua_S)
{
 tolua_pushstring(tolua_S,(const char*)PRIVATE_USER_PATH);
 return 1;
}

/* Open function */
int tolua_physfs_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_module(tolua_S,"fs");
 tolua_function(tolua_S,"fs","get_last_error",toluaI_physfs_fs_get_last_error00);
 tolua_function(tolua_S,"fs","add_to_search_path",toluaI_physfs_fs_add_to_search_path00);
 tolua_function(tolua_S,"fs","remove_from_search_path",toluaI_physfs_fs_remove_from_search_path00);
 tolua_function(tolua_S,"fs","is_directory",toluaI_physfs_fs_is_directory00);
 tolua_function(tolua_S,"fs","get_real_dir",toluaI_physfs_fs_get_real_dir00);
 tolua_function(tolua_S,"fs","get_dir_separator",toluaI_physfs_fs_get_dir_separator00);
 tolua_function(tolua_S,"fs","get_dependent_path",toluaI_physfs_fs_get_dependent_path00);
 tolua_function(tolua_S,"fs","mkdir",toluaI_physfs_fs_mkdir00);
 tolua_function(tolua_S,"fs","delete",toluaI_physfs_fs_delete00);
 tolua_function(tolua_S,"fs","rename",toluaI_physfs_fs_rename00);
 tolua_function(tolua_S,"fs","get_base_dir",toluaI_physfs_fs_get_base_dir00);
 tolua_function(tolua_S,"fs","get_user_dir",toluaI_physfs_fs_get_user_dir00);
 tolua_function(tolua_S,"fs","get_write_dir",toluaI_physfs_fs_get_write_dir00);
 tolua_function(tolua_S,"fs","set_write_dir",toluaI_physfs_fs_set_write_dir00);
 tolua_function(tolua_S,"fs","last_modified",toluaI_physfs_fs_last_modified00);
 tolua_function(tolua_S,"fs","file_exist",toluaI_physfs_fs_file_exist00);
 tolua_tablevar(tolua_S,"fs","PHYSFS_specific_mount",toluaI_get_physfs_fs_PHYSFS_specific_mount,toluaI_set_physfs_fs_PHYSFS_specific_mount);
 tolua_function(tolua_S,"fs","open",toluaI_physfs_fs_open00);
 tolua_function(tolua_S,"fs","close",toluaI_physfs_fs_close00);
 tolua_function(tolua_S,"fs","write",toluaI_physfs_fs_write00);
 tolua_function(tolua_S,"fs","write_data",toluaI_physfs_fs_write_data00);
 tolua_function(tolua_S,"fs","delete",toluaI_physfs_fs_delete01);
 tolua_function(tolua_S,"fs","set_loadsave_stream",toluaI_physfs_fs_set_loadsave_stream00);
 tolua_function(tolua_S,"fs","read",toluaI_physfs_fs_read00);
 tolua_function(tolua_S,"fs","create_zip",toluaI_physfs_fs_create_zip00);
 tolua_tablevar(tolua_S,"fs","TOME_USER_PATH",toluaI_get_physfs_fs_TOME_USER_PATH,NULL);
 return 1;
}
/* Close function */
void tolua_physfs_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"fs");
}
