#include "a.h"

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

static void 
createargtable (lua_State *L, char **argv, int argc, int script)
{
	int i, narg;

	if (script == argc)
		script = 0;  /* no script name? */
	narg = argc - (script + 1);  /* number of positive indices */
	lua_createtable(L, narg, script + 1);
	for (i = 0; i < argc; i++) {
	  lua_pushstring(L, argv[i]);
	  lua_rawseti(L, -2, i - script);
	}
	lua_setglobal(L, "arg");
}

int
main(int argc, char *argv[])
{
	lua_State *L;
	const luaL_Reg *lib;
	const char *s;
	int r;

	L = luaL_newstate();
	luaL_openlibs(L);
	for(lib = libs; lib->func; lib++){
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1);
	}
	createargtable(L, argv, argc, 1);
	r = luaL_dofile(L, argc > 1 ? argv[1] : NULL);
	if(r != LUA_OK){
		s = luaL_checkstring(L, lua_gettop(L));
		fprintf(stderr, "error: %s\n", s);
	}
	lua_close(L);
	return r == LUA_OK;
}
