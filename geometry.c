#include "a.h"

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

static int
lpt(lua_State *L)
{
	int x, y;

	x = luaL_checkinteger(L, 1);
	y = luaL_checkinteger(L, 2);
	pushpoint(L, Pt(x, y));
	return 1;
}

static int
lrect(lua_State *L)
{
	int x0, x1, y0, y1;
	
	x0 = luaL_checkinteger(L, 1);
	y0 = luaL_checkinteger(L, 2);
	x1 = luaL_checkinteger(L, 3);
	y1 = luaL_checkinteger(L, 4);
	pushrect(L, Rect(x0, y0, x1, y1));
	return 1;
}

static int
lrpt(lua_State *L)
{
	Point p, q;

	p = checkpoint(L, 1);
	q = checkpoint(L, 2);
	pushrect(L, Rpt(p, q));
	return 1;
}

static int
laddpt(lua_State *L)
{
	Point p, q, r;

	p = checkpoint(L, 1);
	q = checkpoint(L, 2);
	r = addpt(p, q);
	pushpoint(L, r);
	return 1;
}

static int
lsubpt(lua_State *L)
{
	Point p, q, r;

	p = checkpoint(L, 1);
	q = checkpoint(L, 2);
	r = subpt(p, q);
	pushpoint(L, r);
	return 1;
}

static int
lmulpt(lua_State *L)
{
	Point p;
	int a;

	p = checkpoint(L, 1);
	a = luaL_checkinteger(L, 2);
	pushpoint(L, mulpt(p, a));
	return 1;
}

static int
ldivpt(lua_State *L)
{
	Point p;
	int a;

	p = checkpoint(L, 1);
	a = luaL_checkinteger(L, 2);
	pushpoint(L, divpt(p, a));
	return 1;
}

static int
lrectaddpt(lua_State *L)
{
	Rectangle r;
	Point p;

	r = checkrect(L, 1);
	p = checkpoint(L, 2);
	pushrect(L, rectaddpt(r, p));
	return 1;
}

static int
lrectsubpt(lua_State *L)
{
	Rectangle r;
	Point p;

	r = checkrect(L, 1);
	p = checkpoint(L, 2);
	pushrect(L, rectsubpt(r, p));
	return 1;
}

static int
linsetrect(lua_State *L)
{
	Rectangle r;
	int n;

	r = checkrect(L, 1);
	n = luaL_checkinteger(L, 2);
	pushrect(L, insetrect(r, n));
	return 1;
}

static int
lcanonrect(lua_State *L)
{
	Rectangle r;
	
	r = checkrect(L, 1);
	pushrect(L, canonrect(r));
	return 1;
}

static int
leqpt(lua_State *L)
{
	Point p, q;

	p = checkpoint(L, 1);
	q = checkpoint(L, 2);
	lua_pushboolean(L, eqpt(p, q));
	return 1;
}

static int
leqrect(lua_State *L)
{
	Rectangle r, s;

	r = checkrect(L, 1);
	s = checkrect(L, 2);
	lua_pushboolean(L, eqrect(r, s));
	return 1;
}

static int
lptinrect(lua_State *L)
{
	Point p;
	Rectangle r;

	p = checkpoint(L, 1);
	r = checkrect(L, 2);
	lua_pushboolean(L, ptinrect(p, r));
	return 1;
}

static int
lrectinrect(lua_State *L)
{
	Rectangle r, s;

	r = checkrect(L, 1);
	s = checkrect(L, 2);
	lua_pushboolean(L, rectinrect(r, s));
	return 1;
}

static int
lrectxrect(lua_State *L)
{
	Rectangle r, s;

	r = checkrect(L, 1);
	s = checkrect(L, 2);
	lua_pushboolean(L, rectXrect(r, s));
	return 1;
}

static int
lbadrect(lua_State *L)
{
	Rectangle r;

	r = checkrect(L, 1);
	lua_pushboolean(L, badrect(r));
	return 1;
}

static int
ldx(lua_State *L)
{
	Rectangle r;

	r = checkrect(L, 1);
	lua_pushinteger(L, Dx(r));
	return 1;
}

static int
ldy(lua_State *L)
{
	Rectangle r;

	r = checkrect(L, 1);
	lua_pushinteger(L, Dy(r));
	return 1;
}

static const struct luaL_Reg libgeometry [] = {
	{ "pt", lpt },
	{ "rect", lrect },
	{ "rpt", lrpt },
	{ "addpt", laddpt },
	{ "subpt", lsubpt },
	{ "mulpt", lmulpt },
	{ "divpt", ldivpt },
	{ "rectaddpt", lrectaddpt },
	{ "rectsubpt", lrectsubpt },
	{ "insetrect", linsetrect },
	{ "canonrect", lcanonrect },
	{ "eqpt", leqpt },
	{ "eqrect", leqrect },
	{ "ptinrect", lptinrect },
	{ "rectinrect", lrectinrect },
	{ "rectxrect", lrectxrect },
	{ "badrect", lbadrect },
	{ "dx", ldx },
	{ "dy", ldy },
	{ NULL, NULL },
};

int
openlibgeometry(lua_State *L)
{
	luaL_newlib(L, libgeometry);
	pushpoint(L, ZP);
	lua_setfield(L, -2, "ZP");
	return 1;
}

