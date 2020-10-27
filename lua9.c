#include <u.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <draw.h>
#include <event.h>
#include <keyboard.h>
#include <plumb.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "ldraw.h"
#include "lplumb.h"
#include "llib9.h"
#include "utils.h"

static const luaL_Reg libs[] = {
	{ "draw", openlibdraw },
	{ "event", openlibevent },
	{ "g", openlibgeometry },
	{ "key",  openlibkey },
	{ "color", openlibcolor },
	{ "plumb", openlibplumb },
	{ "plan9", openlib9 },
	{ NULL, NULL },
};

int
main(int argc, char *argv[])
{
	lua_State *L;
	luaL_Reg *lib;
	char *s;
	int r;

	L = luaL_newstate();
	luaL_openlibs(L);
	for(lib = libs; lib->func; lib++){
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1);
	}
	r = luaL_dofile(L, argc > 1 ? argv[1] : NULL);
	if(r != LUA_OK){
		s = luaL_checkstring(L, lua_gettop(L));
		fprintf(stderr, "error: %s\n", s);
	}
	lua_close(L);
	return r == LUA_OK;
}
