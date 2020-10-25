#include <u.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <draw.h>
#include <event.h>
#include <keyboard.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "ldraw.h"
#include "utils.h"

static int
leinit(lua_State *L)
{
	lua_Integer i;

	i = luaL_checknumber(L, -1);
	einit((ulong)i);
	return 0;
}

static int
levent(lua_State *L)
{
	Event ev;
	int e;

	e = event(&ev);
	lua_pushnumber(L, e);
	lua_newtable(L);
	lua_pushinteger(L, ev.kbdc);
	lua_setfield(L, -2, "kbdc");
	lua_newtable(L);
	lua_pushinteger(L, ev.mouse.buttons);
	lua_setfield(L, -2, "buttons");
	pushpoint(L, ev.mouse.xy);
	lua_setfield(L, -2, "xy");
	lua_pushinteger(L, ev.mouse.msec);
	lua_setfield(L, -2, "msec");
	lua_setfield(L, -2, "mouse");
	return 2;
}

static const struct luaL_Reg libevent [] = {
	{ "init",       leinit },
	{ "event",       levent },
	{ NULL, NULL }
};

int
openlibevent(lua_State *L)
{
	luaL_newlib(L, libevent);
	pushglobal(L, "MOUSE", Emouse);
	pushglobal(L, "KEYBOARD", Ekeyboard);
	return 1;
}
