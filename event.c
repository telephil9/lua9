#include "a.h"

static Mouse
checkmouse(lua_State *L, int index)
{
	Mouse m;

	if(lua_istable(L, index) == 0)
		luaL_argerror(L, index, "mouse table expected");
	lua_pushstring(L, "buttons");
	lua_gettable(L, index);
	m.buttons = luaL_checkinteger(L, -1);
	lua_pushstring(L, "xy");
	lua_gettable(L, index);
	m.xy = checkpoint(L, lua_gettop(L));
	lua_pushstring(L, "msec");
	lua_gettable(L, index);
	m.msec = luaL_checkinteger(L, -1);
	lua_pop(L, 3); /* msec, xy, buttons */
	return m;
}

static int
leinit(lua_State *L)
{
	lua_Integer i;

	i = luaL_checknumber(L, -1);
	einit((ulong)i);
	return 0;
}

static int
levent(lua_State *L)
{
	Event ev;
	int e;

	e = event(&ev);
	lua_pushnumber(L, e);
	lua_newtable(L);
	lua_pushinteger(L, ev.kbdc);
	lua_setfield(L, -2, "kbdc");
	lua_newtable(L);
	lua_pushinteger(L, ev.mouse.buttons);
	lua_setfield(L, -2, "buttons");
	pushpoint(L, ev.mouse.xy);
	lua_setfield(L, -2, "xy");
	lua_pushinteger(L, ev.mouse.msec);
	lua_setfield(L, -2, "msec");
	lua_setfield(L, -2, "mouse");
	lua_pushlightuserdata(L, ev.v);
	lua_setfield(L, -2, "v");
	return 2;
}

static int
lkbd(lua_State *L)
{
	int r;

	r = ekbd();
	lua_pushinteger(L, r);
	return 1;
}

static int
lcanmouse(lua_State *L)
{
	int b;

	b = ecanmouse();
	lua_pushboolean(L, b);
	return 1;
}

static int
lcankbd(lua_State *L)
{
	int b;

	b = ecankbd();
	lua_pushboolean(L, b);
	return 1;
}

static int
ltimer(lua_State *L)
{
	ulong key, r;
	int n;

	key = (ulong)luaL_checkinteger(L, 1);
	n   = luaL_checkinteger(L, 2);
	r   = etimer(key, n);
	lua_pushinteger(L, r);
	return 1;
}

static char**
checkstrings(lua_State *L, int index)
{
	char **items;
	int n, i;

	if(lua_istable(L, index) == 0)
		luaL_argerror(L, index, "table of strings expected");
	n = luaL_len(L, index);
	if(n == 0)
		luaL_argerror(L, index, "table of strings is empty");
	items = calloc(n + 1, sizeof(char*));
	if(items == nil)
		luaL_error(L, "out of memory");
	items[n] = NULL;
	for(i = 0; i < n; i++){
		lua_rawgeti(L, index, i + 1);
		items[i] = (char*)luaL_checkstring(L, lua_gettop(L));
	}
	return items;
}

static int
lmenuhit(lua_State *L)
{
	int b, r;
	Mouse m;
	Menu menu;

	b = luaL_checkinteger(L, 1);
	m = checkmouse(L, 2);
	menu.item = checkstrings(L, 3);
	r = emenuhit(b, &m, &menu);
	if(r >= 0)
		lua_pushinteger(L, r + 1);
	else
		lua_pushnil(L);
	return 1;
}

static int
lmoveto(lua_State *L)
{
	Point p;

	p = checkpoint(L, 1);
	emoveto(p);
	return 0;
}

static int
lenter(lua_State *L)
{
	char buf[1024]; /* XXX do it better */
	const char *ask, *s;
	Mouse m;
	int n;

	ask = luaL_optstring(L, 1, nil);
	s   = luaL_optstring(L, 2, "");
	m   = checkmouse(L, 3);
	if(s != nil)
		strncpy(buf, s, sizeof buf);
	n   = eenter(ask, buf, sizeof buf, &m);
	if(n > 0)
		lua_pushlstring(L, buf, n);
	else
		lua_pushnil(L);
	return 1;
}

static int
lplumb(lua_State *L)
{
	int key, ret;
	const char *port;

	key  = luaL_checkinteger(L, 1);
	port = luaL_checkstring(L, 2);
	ret  = eplumb(key, port);
	lua_pushinteger(L, ret);
	return 1;
}


static const struct luaL_Reg libevent [] = {
	{ "init",      leinit },
	{ "event",     levent },
	{ "kbd",       lkbd },
	{ "canmouse",  lcanmouse },
	{ "cankbd",    lcankbd },
	{ "timer",     ltimer },
	{ "menuhit",   lmenuhit },
	{ "moveto",    lmoveto },
	{ "enter",     lenter },
	{ "plumb",     lplumb },
	{ NULL, NULL }
};

int
openlibevent(lua_State *L)
{
	luaL_newlib(L, libevent);
	pushglobal(L, "MOUSE", Emouse);
	pushglobal(L, "KEYBOARD", Ekeyboard);
	return 1;
}
