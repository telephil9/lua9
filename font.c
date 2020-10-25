#include <draw.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "ldraw.h"
#include "utils.h"

#define FONT "Font"

typedef struct LFont LFont;

struct LFont
{
	Font *f;
};

void
pushfont(lua_State *L, Font *f)
{
	LFont *l;

	l = (LFont*)lua_newuserdata(L, sizeof(LFont));
	luaL_getmetatable(L, FONT);
	lua_setmetatable(L, -2);
	l->f = f;
}

Font*
checkfont(lua_State *L, int index)
{
	LFont *l;

	l = (LFont*)luaL_checkudata(L, index, FONT);
	luaL_argcheck(L, l != NULL, index, "Font expected");
	return l->f;
}

static int
font__gc(lua_State *L)
{
	/* TODO */
	lua_pushboolean(L, 0);
	return 1;
}

static int
font__tostring(lua_State *L)
{
	void *p;

	p = lua_touserdata(L, 1);
	lua_pushfstring(L, "font: %p", p);
	return 1;
}

static int
font__index(lua_State *L)
{
	Font *f;
	const char *s;

	f = checkfont(L, 1);
	s = luaL_checkstring(L, 2);
	if(strncmp(s, "name", 4) == 0)
		lua_pushstring(L, f->name);
	else if(strncmp(s, "height", 6) == 0)
		lua_pushinteger(L, f->height);
	else if(strncmp(s, "ascent", 6) == 0)
		lua_pushinteger(L, f->ascent);
	else if(strncmp(s, "width", 5) == 0)
		lua_pushinteger(L, f->width);
	else if(strncmp(s, "nsub", 4) == 0)
		lua_pushinteger(L, f->nsub);
	else if(strncmp(s, "age", 3) == 0)
		lua_pushinteger(L, f->age);
	else
		return 0;
	return 1;
}

static const struct luaL_Reg font_funcs[] = {
	{ "__gc", font__gc },
	{ "__tostring", font__tostring },
	{ "__index", font__index },
	{ NULL, NULL },
};

void
registerfontmeta(lua_State *L)
{
	createmetatable(L, FONT, font_funcs);
}
