#ifndef UTILS_H__
#define UTILS_H__

void pushglobal(lua_State *L, const char *name, int value);
void createmetatable(lua_State *L, const char *name, luaL_Reg *funcs);

#endif
