#include <u.h>
#include <lib9.h>
#include <plumb.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "lplumb.h"
#include "utils.h"

static int
lopen(lua_State *L)
{
	char *port;
	int omode, fd;
	char err[128];

	port  = luaL_checkstring(L, 1);
	omode = luaL_checkinteger(L, 2);
	fd    = plumbopen(port, omode);
	if(fd < 0){
		errstr(err, sizeof err);
		lua_pushfstring(L, "unable to open plumb port '%s': %s", port, err);
		return lua_error(L);
	}
	lua_pushinteger(L, fd);
	return 1;
}

static int
lclose(lua_State *L)
{
	int fd;

	fd = luaL_checkinteger(L, 1);
	//close(fd);
	return 0;
}

static int
lsendtext(lua_State *L)
{
	char *src, *dst, *wdir, *data;
	int fd, n;

	fd   = luaL_checkinteger(L, 1);
	src  = luaL_optstring(L, 2, NULL);
	dst  = luaL_checkstring(L, 3);
	wdir = luaL_optstring(L, 4, NULL);
	data = luaL_checkstring(L, 5);
	n    = plumbsendtext(fd, src, dst, wdir, data);
	lua_pushinteger(L, n);
	return 1;
}	

static int
lrecv(lua_State *L)
{
	int fd;
	Plumbmsg *m;

	fd = luaL_checkinteger(L, 1);
	m  = plumbrecv(fd);
	pushplumbmsg(L, m);
	return 1;
}

static int
lmsg(lua_State *L)
{
	void *p;

	if(lua_islightuserdata(L, 1)){
		p = lua_touserdata(L, 1);
		pushplumbmsg(L, (Plumbmsg*)p);
	} else
		lua_pushnil(L);
	return 1;
}

static const struct luaL_Reg libplumb [] = {
	{ "open", lopen },
	{ "close", lclose },
	{ "sendtext", lsendtext },
	{ "recv", lrecv },
	{ "msg", lmsg },
	{ NULL, NULL }
};

int
openlibplumb(lua_State *L)
{
	registerplumbmsgmeta(L);
	registerplumbattrmeta(L);
	luaL_newlib(L, libplumb);
	return 1;
}
