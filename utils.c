#include "a.h"

/* push a global definition in the lib on the stack */
void
pushglobal(lua_State *L, const char *name, int value)
{
	lua_pushnumber(L, value);
	lua_setfield(L, -2, name);	
}

void
createmetatable(lua_State *L, const char *name, luaL_Reg *funcs)
{
	luaL_newmetatable(L, name);
	luaL_setfuncs(L, funcs, 0);
	lua_pushliteral (L, "__metatable");
	lua_pushliteral (L, "metatable access forbidden");
	lua_settable (L, -3);
}
