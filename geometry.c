#include <draw.h>
#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

void
pushrect(lua_State *L, Rectangle r)
{
	lua_newtable(L);
	lua_newtable(L);
	lua_pushinteger(L, r.min.x);
	lua_setfield(L, -2, "x");
	lua_pushinteger(L, r.min.y);
	lua_setfield(L, -2, "y");
	lua_setfield(L, -2, "min");
	lua_newtable(L);
	lua_pushinteger(L, r.max.x);
	lua_setfield(L, -2, "x");
	lua_pushinteger(L, r.max.y);
	lua_setfield(L, -2, "y");
	lua_setfield(L, -2, "max");
}

Rectangle
checkrect(lua_State *L, int index)
{
	Rectangle r;

	if(lua_istable(L, index) == 0)
		luaL_argerror(L, index, "rectangle table expected");
	lua_pushstring(L, "min");
	lua_gettable(L, index);
	lua_pushstring(L, "x");
	lua_gettable(L, -2);
	r.min.x = luaL_checkinteger(L, -1);
	lua_pushstring(L, "y");
	lua_gettable(L, -3);
	r.min.y = luaL_checkinteger(L, -1);
	lua_pop(L, 3); /* table | x | y */
	lua_pushstring(L, "max");
	lua_gettable(L, index);
	lua_pushstring(L, "x");
	lua_gettable(L, -2);
	r.max.x = luaL_checkinteger(L, -1);
	lua_pushstring(L, "y");
	lua_gettable(L, -3);
	r.max.y = luaL_checkinteger(L, -1);
	lua_pop(L, 3);
	return r;
}

void
pushpoint(lua_State *L, Point p)
{
	lua_newtable(L);
	lua_pushinteger(L, p.x);
	lua_setfield(L, -2, "x");
	lua_pushinteger(L, p.y);
	lua_setfield(L, -2, "y");
}

Point
getpoint(lua_State *L, int index)
{
	Point p;

	lua_pushstring(L, "x");
	lua_gettable(L, index);
	p.x = luaL_checkinteger(L, -1);
	lua_pushstring(L, "y");
	lua_gettable(L, index);
	p.y = luaL_checkinteger(L, -1);
	lua_pop(L, 2);
	return p;
}

Point
checkpoint(lua_State *L, int index)
{
	if(lua_istable(L, index) == 0)
		luaL_argerror(L, index, "point table expected");
	return getpoint(L, index);
}

Point
optpoint(lua_State *L, int index)
{
	if(lua_istable(L, index) == 0)
		return ZP;
	return getpoint(L, index);
}

Point*
checkpoints(lua_State *L, int index, int *np)
{
	Point *p;
	int i;

	if(lua_istable(L, index) == 0)
		luaL_argerror(L, index, "table of points expected");
	*np = luaL_len(L, index);
	if(*np == 0)
		luaL_argerror(L, index, "table of points is empty");
	p = calloc(*np, sizeof(Point));
	if(p == nil)
		luaL_error(L, "out of memory");
	for(i = 1; i <= *np; i++){
		lua_rawgeti(L, index, i);
		p[i-1] = checkpoint(L, lua_gettop(L));
	}
	return p;
}
