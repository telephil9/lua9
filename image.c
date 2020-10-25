#include <draw.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "ldraw.h"
#include "utils.h"

#define IMAGE "Image"

typedef struct LImage LImage;

struct LImage
{
	Image *i;
};

void pushimage(lua_State *L, Image *i)
{
	LImage *l;

	l = (LImage*)lua_newuserdata(L, sizeof(LImage));
	luaL_getmetatable(L, IMAGE);
	lua_setmetatable(L, -2);
	l->i = i;
}

Image*
checkimage(lua_State *L, int index)
{
	LImage *l;

	l = (LImage*)luaL_checkudata(L, index, IMAGE);
	luaL_argcheck(L, l != NULL, index, "Image expected");
	return l->i;
}

Image*
optimage(lua_State *L, int index)
{
	if(lua_isnil(L, index))
		return nil;
	return checkimage(L, index);
}

static int
image__gc(lua_State *L)
{
	Image *i;

	i = checkimage(L, 1);
	if(i == screen) {
		lua_pushboolean(L, 0);
		return 1;
	}
	/* TODO freeimage */
	lua_pushboolean(L, 1);
	return 1;
}

static int
image__tostring(lua_State *L)
{
	void *p;
	char buf[64];

	p = lua_touserdata(L, 1);
	lua_pushfstring(L, "image: %p", p);
	return 1;
}

static int
image__index(lua_State *L)
{
	Image *i;
	const char *s;

	i = checkimage(L, 1);
	s = luaL_checkstring(L, 2);
	if(!strncmp(s, "r", 1))
		pushrect(L, i->r);
	else if(!strncmp(s, "clipr", 5))
		pushrect(L, i->clipr);
	else if(!strncmp(s, "chan", 4))
		lua_pushinteger(L, i->chan);
	else if(!strncmp(s, "depth", 5))
		lua_pushinteger(L, i->depth);
	else if(!strncmp(s, "repl", 4))
		lua_pushinteger(L, i->repl);
	else
		return 0;
	return 1;
}

static const struct luaL_Reg image_funcs[] = {
	{ "__gc", image__gc },
	{ "__tostring", image__tostring },
	{ "__index", image__index },
	{ NULL, NULL },
};

void
registerimagemeta(lua_State *L)
{
	createmetatable(L, IMAGE, image_funcs);
}
