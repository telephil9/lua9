#include <draw.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "ldraw.h"
#include "utils.h"

#define DISPLAY "Display"

typedef struct LDisplay LDisplay;

struct LDisplay
{
	Display *d;
};

void
pushdisplay(lua_State *L, Display *d)
{
	LDisplay *l;

	l = (LDisplay*)lua_newuserdata(L, sizeof(LDisplay));
	luaL_getmetatable(L, DISPLAY);
	lua_setmetatable(L, -2);
	l->d = d;
}

Display*
checkdisplay(lua_State *L, int index)
{
	LDisplay *l;

	l = (LDisplay*)luaL_checkudata(L, index, DISPLAY);
	luaL_argcheck(L, l != NULL, index, "Display expected");
	return l->d;
}

static int
display__gc(lua_State *L)
{
	/* we do not GC the display */
	lua_pushboolean(L, 0);
	return 1;
}

static int
display__tostring(lua_State *L) 
{
	void *p;

	p = lua_touserdata(L, 1);
	lua_pushfstring(L, "display: %p", p);
	return 1;
}

static int
display__index(lua_State *L)
{
	Display *d;
	const char *s;

	d = checkdisplay(L, 1);
	s = luaL_checkstring(L, 2);
	if(strncmp(s, "white", 5) == 0)
		pushimage(L, d->white);
	else if(strncmp(s, "black", 5) == 0)
		pushimage(L, d->black);
	else
		return 0;
	return 1;
}

static const struct luaL_Reg display_funcs[] = {
	{ "__gc", display__gc },
	{ "__tostring", display__tostring },
	{ "__index", display__index },
	{ NULL, NULL },
};

void
registerdisplaymeta(lua_State *L)
{
	createmetatable(L, DISPLAY, display_funcs);
}

