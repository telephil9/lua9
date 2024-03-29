#include "a.h"

static int
lfork(lua_State *L)
{
	char err[128];
	int n;

	if(lua_isfunction(L, 1) == 0)
		return luaL_argerror(L, 1, "expected a function");
	switch(rfork(RFFDG|RFREND|RFPROC)){
	case -1:
		errstr(err, sizeof err);
		lua_pushfstring(L, "fork failed: %s", err);
		return lua_error(L);
	case 0:
		break;
	default:
		n = lua_gettop(L);
		lua_call(L, n - 1, 0);
	}
	return 0;
}

static int
lclose(lua_State *L)
{
	int fd;

	fd = luaL_checkinteger(L, 1);
	close(fd);
	return 0;
}
static const struct luaL_Reg lib9 [] = {
	{ "fork", lfork },
	{ "close", lclose },
	{ NULL, NULL }
};

int
openlib9(lua_State *L)
{
	luaL_newlib(L, lib9);
	pushglobal(L, "OREAD", 0);
	pushglobal(L, "OWRITE", 1);
	return 1;
}
