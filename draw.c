#include <u.h>
#include <lib9.h>
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
lreplxy(lua_State *L)
{
	int m, M, x, r;

	m = luaL_checkinteger(L, 1);
	M = luaL_checkinteger(L, 2);
	x = luaL_checkinteger(L, 3);
	r = drawreplxy(m, M, x);
	lua_pushinteger(L, r);
	return 1;
}

static int
lrepl(lua_State *L)
{
	Rectangle r;
	Point p, rp;

	r  = checkrect(L, 1);
	p  = checkpoint(L, 2);
	rp = drawrepl(r, p);
	pushpoint(L, rp);
	return 1;
}

static int
lreplclipr(lua_State *L)
{
	Image *i;
	int repl;
	Rectangle clipr;

	i     = checkimage(L, 1);
	repl  = luaL_checkinteger(L, 2);
	clipr = checkrect(L, 3);
	replclipr(i, repl, clipr);
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
lpoly(lua_State *L)
{
	Image *dst, *src;
	Point *p, sp;
	int np, end0, end1, radius;

	dst    = checkimage(L, 1);
	p      = checkpoints(L, 2, &np);
	end0   = luaL_checkinteger(L, 3);
	end1   = luaL_checkinteger(L, 4);
	radius = luaL_checkinteger(L, 5);
	src    = checkimage(L, 6);
	sp     = checkpoint(L, 7);
	poly(dst, p, np, end0, end1, radius, src, sp);
	return 0;
}

static int
lfillpoly(lua_State *L)
{
	Image *dst, *src;
	Point *p, sp;
	int np, wind;

	dst  = checkimage(L, 1);
	p    = checkpoints(L, 2, &np);
	wind = luaL_checkinteger(L, 3);
	src  = checkimage(L, 4);
	sp   = checkpoint(L, 5);
	fillpoly(dst, p, np, wind, src, sp);
	return 0;
}

static int
lbezier(lua_State *L)
{
	Image *dst, *src;
	Point p0, p1, p2, p3, sp;
	int end0, end1, radius, ret;

	dst    = checkimage(L, 1);
	p0     = checkpoint(L, 2);
	p1     = checkpoint(L, 3);
	p2     = checkpoint(L, 4);
	p3     = checkpoint(L, 5);
	end0   = luaL_checkinteger(L, 6);
	end1   = luaL_checkinteger(L, 7);
	radius = luaL_checkinteger(L, 8);
	src    = checkimage(L, 9);
	sp     = checkpoint(L, 10);
	ret    = bezier(dst, p0, p1, p2, p3, end0, end1, radius, src, sp);
	lua_pushinteger(L, ret);
	return 1;
}

static int
lfillbezier(lua_State *L)
{
	Image *dst, *src;
	Point p0, p1, p2, p3, sp;
	int w, ret;

	dst    = checkimage(L, 1);
	p0     = checkpoint(L, 2);
	p1     = checkpoint(L, 3);
	p2     = checkpoint(L, 4);
	p3     = checkpoint(L, 5);
	w      = luaL_checkinteger(L, 6);
	src    = checkimage(L, 7);
	sp     = checkpoint(L, 8);
	ret    = fillbezier(dst, p0, p1, p2, p3, w, src, sp);
	lua_pushinteger(L, ret);
	return 1;
}

static int
lbezspline(lua_State *L)
{
	Image *dst, *src;
	Point *p, sp;
	int ret, np, end0, end1, radius;

	dst    = checkimage(L, 1);
	p      = checkpoints(L, 2, &np);
	end0   = luaL_checkinteger(L, 3);
	end1   = luaL_checkinteger(L, 4);
	radius = luaL_checkinteger(L, 5);
	src    = checkimage(L, 6);
	sp     = checkpoint(L, 7);
	ret    = bezspline(dst, p, np, end0, end1, radius, src, sp);
	lua_pushinteger(L, ret);
	return 1;
}

static int
lfillbezspline(lua_State *L)
{
	Image *dst, *src;
	Point *p, sp;
	int ret, np, w;

	dst = checkimage(L, 1);
	p   = checkpoints(L, 2, &np);
	w   = luaL_checkinteger(L, 3);
	src = checkimage(L, 4);
	sp  = checkpoint(L, 5);
	ret = fillbezspline(dst, p, np, w, src, sp);
	lua_pushinteger(L, ret);
	return 1;
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
larc(lua_State *L)
{
	Image *dst, *src;
	Point c, sp;
	int a, b, thick, alpha, phi;

	dst   = checkimage(L, 1);
	c     = checkpoint(L, 2);
	a     = luaL_checkinteger(L, 3);
	b     = luaL_checkinteger(L, 4);
	thick = luaL_checkinteger(L, 5);
	src   = checkimage(L, 6);
	sp    = checkpoint(L, 7);
	alpha = luaL_checkinteger(L, 8);
	phi   = luaL_checkinteger(L, 9);
	arc(dst, c, a, b, thick, src, sp, alpha, phi);
	return 0;
}

static int
lfillarc(lua_State *L)
{
	Image *dst, *src;
	Point c, sp;
	int a, b, thick, alpha, phi;

	dst   = checkimage(L, 1);
	c     = checkpoint(L, 2);
	a     = luaL_checkinteger(L, 3);
	b     = luaL_checkinteger(L, 4);
	src   = checkimage(L, 5);
	sp    = checkpoint(L, 6);
	alpha = luaL_checkinteger(L, 7);
	phi   = luaL_checkinteger(L, 8);
	fillarc(dst, c, a, b, src, sp, alpha, phi);
	return 0;
}

static int
licossin(lua_State *L)
{
	int deg, cosp, sinp;

	deg = luaL_checkinteger(L, 1);
	icossin(deg, &cosp, &sinp);
	lua_pushinteger(L, cosp);
	lua_pushinteger(L, sinp);
	return 2;
}

static int
licossin2(lua_State *L)
{
	int x, y, cosp, sinp;

	x = luaL_checkinteger(L, 1);
	y = luaL_checkinteger(L, 2);
	icossin2(x, y, &cosp, &sinp);
	lua_pushinteger(L, cosp);
	lua_pushinteger(L, sinp);
	return 2;
}

static int
lborder(lua_State *L)
{
	Image *dst, *color;
	Rectangle r;
	Point sp;
	int i;

	dst   = checkimage(L, 1);
	r     = checkrect(L, 2);
	i     = luaL_checkinteger(L, 3);
	color = checkimage(L, 4);
	sp    = checkpoint(L, 5);
	border(dst, r, i, color, sp);
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
lstringn(lua_State *L)
{
	Image *dst, *src;
	Point p, sp, rp;
	Font *f;
	char *s;
	int len;

	dst = checkimage(L, 1);
	p   = checkpoint(L, 2);
	src = checkimage(L, 3);
	sp  = optpoint(L, 4);
	f   = checkfont(L, 5);
	s   = luaL_checkstring(L, 6);
	len = luaL_checkinteger(L, 7);
	rp  = stringn(dst, p, src, sp, f, s, len);
	pushpoint(L, rp);
	return 1;
}

static int
lstringbg(lua_State *L)
{
	Image *dst, *src, *bg;
	Font *f;
	Point p, sp, bgp, r;
	const char *s;

	dst = checkimage(L, 1);
	p   = checkpoint(L, 2);
	src = checkimage(L, 3);
	sp  = optpoint(L, 4);
	f   = checkfont(L, 5);
	s   = luaL_checkstring(L, 6);
	bg  = checkimage(L, 7);
	bgp = checkpoint(L, 8);
	r   = stringbg(dst, p, src, sp, f, s, bg, bgp);
	pushpoint(L, r);
	return 1;
}

static int
lstringnbg(lua_State *L)
{
	Image *dst, *src, *bg;
	Point p, sp, bgp, rp;
	Font *f;
	char *s;
	int len;

	dst = checkimage(L, 1);
	p   = checkpoint(L, 2);
	src = checkimage(L, 3);
	sp  = optpoint(L, 4);
	f   = checkfont(L, 5);
	s   = luaL_checkstring(L, 6);
	len = luaL_checkinteger(L, 7);
	bg  = checkimage(L, 8);
	bgp = checkpoint(L, 9);
	rp  = stringnbg(dst, p, src, sp, f, s, len, bg, bgp);
	pushpoint(L, rp);
	return 1;
}

static int
lopenfont(lua_State *L)
{
	Display *d;
	Font *f;
	char *n;
	char err[128];

	d = checkdisplay(L, 1);
	n = luaL_checkstring(L, 2);
	f = openfont(d, n);
	if(f == nil){
		errstr(err, sizeof err);
		lua_pushfstring(L, "cannot open font '%s': %s", n, err);
		return lua_error(L);
	}
	pushfont(L, f);
	return 1;
}

static int
lbuildfont(lua_State *L)
{
	Display *d;
	Font *f;
	char *n, *m;
	char err[128];

	d = checkdisplay(L, 1);
	n = luaL_checkstring(L, 2);
	m = luaL_checkstring(L, 3);
	f = buildfont(d, n, m);
	if(f == nil){
		errstr(err, sizeof err);
		lua_pushfstring(L, "cannot build font '%s': %s", n, err);
		return lua_error(L);
	}
	pushfont(L, f);
	return 1;
}

static int
lstringsize(lua_State *L)
{
	Font *f;
	char *s;
	Point p;

	f = checkfont(L, 1);
	s = luaL_checkstring(L, 2);
	p = stringsize(f, s);
	pushpoint(L, p);
	return 1;
}

static int
lstringwidth(lua_State *L)
{
	Font *f;
	char *s;
	int w;

	f = checkfont(L, 1);
	s = luaL_checkstring(L, 2);
	w = stringwidth(f, s);
	lua_pushinteger(L, w);
	return 1;
}

static int
lstringnwidth(lua_State *L)
{
	Font *f;
	char *s;
	int n, w;

	f = checkfont(L, 1);
	s = luaL_checkstring(L, 2);
	n = luaL_checkinteger(L, 3);
	w = stringnwidth(f, s, n);
	lua_pushinteger(L, w);
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

static int
lallocimagemix(lua_State *L)
{
	Display *d;
	ulong one, three;
	Image *i;

	d     = checkdisplay(L, 1);
	one   = (ulong)luaL_checkinteger(L, 2);
	three = (ulong)luaL_checkinteger(L, 3);
	i     = allocimagemix(d, one, three);
	pushimage(L, i);
	return 1;
}

static const struct luaL_Reg libdraw [] = {
	{ "init",        linitdraw },
	{ "draw",        ldraw },
	{ "replxy",      lreplxy },
	{ "repl",        lrepl },
	{ "replclipr",   lreplclipr },
	{ "line",        lline },
	{ "poly",        lpoly },
	{ "fillpoly",    lfillpoly },
	{ "bezier",      lbezier },
	{ "fillbezier",  lfillbezier },
	{ "bezspline",   lbezspline },
	{ "fillbezspline", lfillbezspline },
	{ "ellipse",     lellipse },
	{ "fillellipse", lfillellipse },
	{ "arc",         larc },
	{ "fillarc",     lfillarc },
	{ "icossin",     licossin },
	{ "icossin2",    licossin2 },
	{ "border",      lborder },
	{ "string",      lstring },
	{ "stringn",     lstringn },
	{ "stringbg",    lstringbg },
	{ "stringnbg",   lstringnbg },
	{ "openfont",    lopenfont },
	{ "buildfont",   lbuildfont },
	{ "stringsize",  lstringsize },
	{ "stringwidth", lstringwidth },
	{ "stringnwidth", lstringnwidth },
	{ "allocimage",  lallocimage },
	{ "allocimagemix", lallocimagemix },
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
