#include <u.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <draw.h>
#include <event.h>
#include <keyboard.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "ldraw.h"
#include "utils.h"

static lua_State *state;
static int ridx, tidx;

void eresized(int new) {
	if(new && getwindow(display, Refnone) < 0){
		fprintf(stderr, "cannot reattach to window: %r");
		exit(1);
	}
	lua_rawgeti(state, LUA_REGISTRYINDEX, ridx);
	lua_rawgeti(state, -1, tidx);
	lua_call(state, 0, 0);
	lua_pop(state, 2);
}

static int
linitdraw(lua_State *L) 
{
	const char *n;
	char buf[256];
	
	n = luaL_checkstring(L, -1);
	lua_newtable(L);
	ridx = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_rawgeti(L, LUA_REGISTRYINDEX, ridx);
	lua_getglobal(L, "eresized");
	if(lua_isfunction(L, -1) == 0){
		lua_pushstring(L, "eresized function not declared");
		lua_error(L);
	}
	tidx = luaL_ref(L, -2);
	lua_pop(L, 1);
	if(initdraw(nil, nil, n) < 0){
		snprintf(buf, sizeof buf, "initdraw failed: %r");
		lua_pushstring(L, buf);
		lua_error(L);
	}
	state = L;
	pushdisplay(L, display);
	lua_setglobal(L, "display");
	pushimage(L, screen);
	lua_setglobal(L, "screen");
	pushfont(L, font);
	lua_setglobal(L, "font");
	return 0;
}

static int
ldraw(lua_State *L)
{
	Image *dst, *src, *mask;
	Point p;
	Rectangle r;

	dst  = checkimage(L, 1);
	r    = checkrect(L, 2);
	src  = checkimage(L, 3);
	mask = optimage(L, 4);
	p    = checkpoint(L, 5);
	draw(dst, r, src, mask, p);
	return 0;
}

static int
lline(lua_State *L)
{
	Image *dst, *src;
	Point p0, p1, sp;
	int end0, end1, thick;

	dst   = checkimage(L, 1);
	p0    = checkpoint(L, 2);
	p1    = checkpoint(L, 3);
	end0  = luaL_checkinteger(L, 4);
	end1  = luaL_checkinteger(L, 5);
	thick = luaL_checkinteger(L, 6);
	src   = checkimage(L, 7);
	sp    = checkpoint(L, 8);
	line(dst, p0, p1, end0, end1, thick, src, sp);
	return 0;
}

static int
lellipse(lua_State *L)
{
	Image *dst, *src;
	Point c, sp;
	int a, b, thick;

	dst   = checkimage(L, 1);
	c     = checkpoint(L, 2);
	a     = luaL_checkinteger(L, 3);
	b     = luaL_checkinteger(L, 4);
	thick = luaL_checkinteger(L, 5);
	src   = checkimage(L, 6);
	sp    = checkpoint(L, 7);
	ellipse(dst, c, a, b, thick, src, sp);
	return 0;
}

static int
lfillellipse(lua_State *L)
{
	Image *dst, *src;
	Point c, sp;
	int a, b;

	dst   = checkimage(L, 1);
	c     = checkpoint(L, 2);
	a     = luaL_checkinteger(L, 3);
	b     = luaL_checkinteger(L, 4);
	src   = checkimage(L, 5);
	sp    = checkpoint(L, 6);
	fillellipse(dst, c, a, b, src, sp);
	return 0;
}

static int
lstring(lua_State *L)
{
	Image *dst, *src;
	Font *f;
	Point p, sp, r;
	const char *s;

	dst = checkimage(L, 1);
	p   = checkpoint(L, 2);
	src = checkimage(L, 3);
	sp  = optpoint(L, 4);
	f   = checkfont(L, 5);
	s   = luaL_checkstring(L, 6);
	r   = string(dst, p, src, sp, f, s);
	pushpoint(L, r);
	return 1;
}

static int
lallocimage(lua_State *L)
{
	Display *d;
	Rectangle r;
	ulong chan, col;
	int repl;
	Image *i;

	d    = checkdisplay(L, 1);
	r    = checkrect(L, 2);
	chan = (ulong)luaL_checkinteger(L, 3);
	repl = luaL_checkinteger(L, 4);
	col  = (ulong)luaL_checkinteger(L, 5);
	i    = allocimage(d, r, chan, repl, col);
	pushimage(L, i);
	return 1;
}

static const struct luaL_Reg libdraw [] = {
	{ "init",        linitdraw },
	{ "draw",        ldraw },
	{ "line",        lline },
	{ "ellipse",     lellipse },
	{ "fillellipse", lfillellipse },
	{ "string",      lstring },
	{ "allocimage",  lallocimage },
	{ NULL, NULL }
};

int
openlibdraw(lua_State *L)
{
	registerdisplaymeta(L);
	registerimagemeta(L);
	registerfontmeta(L);
	luaL_newlib(L, libdraw);
	pushglobal(L, "END_SQUARE", Endsquare);
	pushglobal(L, "END_DISC", Enddisc);
	pushglobal(L, "END_ARROW", Endarrow);
	pushglobal(L, "END_MASK", Endmask);

	return 1;
}
