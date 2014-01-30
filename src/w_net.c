/*
** Lua binding: net
** Generated automatically by tolua 4.0a - angband on Tue Jul 27 18:33:41 2010.
*/

#include "lua/tolua.h"

/* Exported function */
int tolua_net_open (lua_State* tolua_S);
void tolua_net_close (lua_State* tolua_S);

#include "angband.h"
#include "SDL_net.h"
static IPaddress *__resolve_host(const char *host, u16b port) { IPaddress *ip; MAKE(ip, IPaddress); return (SDLNet_ResolveHost(ip, host, port) == 0) ? ip : NULL; }
static void __free_ip(IPaddress *ip) { FREE(ip, IPaddress); }
static bool __socket_ready(TCPsocket sock) { return SDLNet_SocketReady(sock); }

/* function to register type */
static void toluaI_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"IPaddress");
 tolua_usertype(tolua_S,"SDLNet_SocketSet");
}

/* error messages */
#define TOLUA_ERR_SELF tolua_error(tolua_S,"invalid 'self'")
#define TOLUA_ERR_ASSIGN tolua_error(tolua_S,"#vinvalid type in variable assignment.")

/* function: __resolve_host */
static int toluaI_net_net_resolve_host00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TSTRING,0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  const char* host = ((const char*)  tolua_getstring(tolua_S,1,0));
  u16b port = ((u16b)  tolua_getnumber(tolua_S,2,0));
 {
  IPaddress* toluaI_ret = (IPaddress*)  __resolve_host(host,port);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"IPaddress"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resolve_host'.");
 return 0;
}

/* function: __free_ip */
static int toluaI_net_net_free_ip00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"IPaddress"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  IPaddress* ip = ((IPaddress*)  tolua_getusertype(tolua_S,1,0));
 {
  __free_ip(ip);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'free_ip'.");
 return 0;
}

/* function: SDLNet_ResolveIP */
static int toluaI_net_net_resolve_ip00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"IPaddress"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  IPaddress* ip = ((IPaddress*)  tolua_getusertype(tolua_S,1,0));
 {
  const char* toluaI_ret = (const char*)  SDLNet_ResolveIP(ip);
 tolua_pushstring(tolua_S,(const char*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'resolve_ip'.");
 return 0;
}

/* function: SDLNet_TCP_Open */
static int toluaI_net_net_open00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"IPaddress"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  IPaddress* ip = ((IPaddress*)  tolua_getusertype(tolua_S,1,0));
 {
  TCPsocket toluaI_ret = (TCPsocket)  SDLNet_TCP_Open(ip);
 tolua_pushuserdata(tolua_S,(void*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'open'.");
 return 0;
}

/* function: SDLNet_TCP_Accept */
static int toluaI_net_net_accept00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  TCPsocket server = ((TCPsocket)  tolua_getuserdata(tolua_S,1,0));
 {
  TCPsocket toluaI_ret = (TCPsocket)  SDLNet_TCP_Accept(server);
 tolua_pushuserdata(tolua_S,(void*)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'accept'.");
 return 0;
}

/* function: SDLNet_TCP_GetPeerAddress */
static int toluaI_net_net_get_peer_address00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  TCPsocket sock = ((TCPsocket)  tolua_getuserdata(tolua_S,1,0));
 {
  IPaddress* toluaI_ret = (IPaddress*)  SDLNet_TCP_GetPeerAddress(sock);
 tolua_pushusertype(tolua_S,(void*)toluaI_ret,tolua_tag(tolua_S,"IPaddress"));
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_peer_address'.");
 return 0;
}

/* function: SDLNet_TCP_Close */
static int toluaI_net_net_close00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  TCPsocket sock = ((TCPsocket)  tolua_getuserdata(tolua_S,1,0));
 {
  SDLNet_TCP_Close(sock);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'close'.");
 return 0;
}

/* function: SDLNet_AllocSocketSet */
static int toluaI_net_net_set_alloc00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  s32b maxsockets = ((s32b)  tolua_getnumber(tolua_S,1,0));
 {
  SDLNet_SocketSet toluaI_ret = (SDLNet_SocketSet)  SDLNet_AllocSocketSet(maxsockets);
 {
#ifdef __cplusplus
 void* toluaI_clone = new SDLNet_SocketSet(toluaI_ret);
#else
 void* toluaI_clone = tolua_copy(tolua_S,(void*)&toluaI_ret,sizeof(SDLNet_SocketSet));
#endif
 tolua_pushusertype(tolua_S,tolua_doclone(tolua_S,toluaI_clone,tolua_tag(tolua_S,"SDLNet_SocketSet")),tolua_tag(tolua_S,"SDLNet_SocketSet"));
 }
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_alloc'.");
 return 0;
}

/* function: __socket_ready */
static int toluaI_net_net_is_ready00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  TCPsocket sock = ((TCPsocket)  tolua_getuserdata(tolua_S,1,0));
 {
  bool toluaI_ret = (bool)  __socket_ready(sock);
 tolua_pushbool(tolua_S,(int)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_ready'.");
 return 0;
}

/* function: SDLNet_TCP_AddSocket */
static int toluaI_net_net_set_add00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"SDLNet_SocketSet"),0) ||
 !tolua_istype(tolua_S,2,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  SDLNet_SocketSet set = *((SDLNet_SocketSet*)  tolua_getusertype(tolua_S,1,0));
  TCPsocket sock = ((TCPsocket)  tolua_getuserdata(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  SDLNet_TCP_AddSocket(set,sock);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_add'.");
 return 0;
}

/* function: SDLNet_TCP_DelSocket */
static int toluaI_net_net_set_del00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"SDLNet_SocketSet"),0) ||
 !tolua_istype(tolua_S,2,LUA_TUSERDATA,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  SDLNet_SocketSet set = *((SDLNet_SocketSet*)  tolua_getusertype(tolua_S,1,0));
  TCPsocket sock = ((TCPsocket)  tolua_getuserdata(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  SDLNet_TCP_DelSocket(set,sock);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_del'.");
 return 0;
}

/* function: SDLNet_CheckSockets */
static int toluaI_net_net_set_check00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"SDLNet_SocketSet"),0) ||
 !tolua_istype(tolua_S,2,LUA_TNUMBER,0) ||
 !tolua_isnoobj(tolua_S,3)
 )
 goto tolua_lerror;
 else
 {
  SDLNet_SocketSet set = *((SDLNet_SocketSet*)  tolua_getusertype(tolua_S,1,0));
  u32b timeout = ((u32b)  tolua_getnumber(tolua_S,2,0));
 {
  s32b toluaI_ret = (s32b)  SDLNet_CheckSockets(set,timeout);
 tolua_pushnumber(tolua_S,(long)toluaI_ret);
 }
 }
 return 1;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_check'.");
 return 0;
}

/* function: SDLNet_FreeSocketSet */
static int toluaI_net_net_set_free00(lua_State* tolua_S)
{
 if (
 !tolua_istype(tolua_S,1,tolua_tag(tolua_S,"SDLNet_SocketSet"),0) ||
 !tolua_isnoobj(tolua_S,2)
 )
 goto tolua_lerror;
 else
 {
  SDLNet_SocketSet set = *((SDLNet_SocketSet*)  tolua_getusertype(tolua_S,1,0));
 {
  SDLNet_FreeSocketSet(set);
 }
 }
 return 0;
tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_free'.");
 return 0;
}

/* Open function */
int tolua_net_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 toluaI_reg_types(tolua_S);
 tolua_constant(tolua_S,NULL,"INADDR_ANY",INADDR_ANY);
 tolua_constant(tolua_S,NULL,"INADDR_NONE",INADDR_NONE);
 tolua_module(tolua_S,"net");
 tolua_function(tolua_S,"net","resolve_host",toluaI_net_net_resolve_host00);
 tolua_function(tolua_S,"net","free_ip",toluaI_net_net_free_ip00);
 tolua_function(tolua_S,"net","resolve_ip",toluaI_net_net_resolve_ip00);
 tolua_function(tolua_S,"net","open",toluaI_net_net_open00);
 tolua_function(tolua_S,"net","accept",toluaI_net_net_accept00);
 tolua_function(tolua_S,"net","get_peer_address",toluaI_net_net_get_peer_address00);
 tolua_function(tolua_S,"net","close",toluaI_net_net_close00);
 tolua_function(tolua_S,"net","set_alloc",toluaI_net_net_set_alloc00);
 tolua_function(tolua_S,"net","is_ready",toluaI_net_net_is_ready00);
 tolua_function(tolua_S,"net","set_add",toluaI_net_net_set_add00);
 tolua_function(tolua_S,"net","set_del",toluaI_net_net_set_del00);
 tolua_function(tolua_S,"net","set_check",toluaI_net_net_set_check00);
 tolua_function(tolua_S,"net","set_free",toluaI_net_net_set_free00);
 return 1;
}
/* Close function */
void tolua_net_close (lua_State* tolua_S)
{
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"INADDR_ANY");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"INADDR_NONE");
 lua_pushnil(tolua_S); lua_setglobal(tolua_S,"net");
}
