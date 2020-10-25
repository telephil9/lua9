#include <keyboard.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "utils.h"

static int
lkeyeq(lua_State *L)
{
	int i;
	const char *s;

	i = luaL_checkinteger(L, 1);
	s = luaL_checkstring(L, 2);
	lua_pushboolean(L, i==*s);
	return 1;
}

static const struct luaL_Reg libkey[] = {
	{ "eq", lkeyeq },
	{ NULL, NULL },
};

int
openlibkey(lua_State *L)
{
	luaL_newlib(L, libkey);
	pushglobal(L, "HOME", Khome);
	pushglobal(L, "UP", Kup);
	pushglobal(L, "DOWN", Kdown);
	pushglobal(L, "PGUP", Kpgup);
	pushglobal(L, "PRINT", Kprint);
	pushglobal(L, "LEFT", Kleft);
	pushglobal(L, "RIGHT", Kright);
	pushglobal(L, "PGDOWN", Kpgdown);
	pushglobal(L, "INS", Kins);
	pushglobal(L, "END", Kend);
	pushglobal(L, "SOH", Ksoh);
	pushglobal(L, "STX", Kstx);
	pushglobal(L, "ETX", Ketx);
	pushglobal(L, "EOF", Keof);
	pushglobal(L, "ENQ", Kenq);
	pushglobal(L, "ACK", Kack);
	pushglobal(L, "BS", Kbs);
	pushglobal(L, "NACK", Knack);
	pushglobal(L, "ETB", Ketb);
	pushglobal(L, "DEL", Kdel);
	pushglobal(L, "ESC", Kesc);
	return 1;
}
