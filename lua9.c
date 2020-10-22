#include <u.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <draw.h>
#include <event.h>
#include <keyboard.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define IMAGE_META "Image"
#define DISPLAY_META "Display"
#define FONT_META "Font"

typedef struct ImagePtr ImagePtr;
typedef struct DisplayPtr DisplayPtr;
typedef struct FontPtr FontPtr;

struct ImagePtr {
	Image *p;
};

struct DisplayPtr {
	Display *p;
};

struct FontPtr {
	Font *p;
};

static lua_State *state;
static int ridx, tidx;

static ImagePtr* to_image(lua_State *L, Image *img) {
	ImagePtr *i;

	i = (ImagePtr*)lua_newuserdata(L, sizeof(ImagePtr));
	luaL_getmetatable(L, IMAGE_META);
	lua_setmetatable(L, -2);
	i->p = img;
	return i;
}

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

static void create_globals(lua_State *L) {
	ImagePtr *i;
	DisplayPtr *d;
	FontPtr *f;

	i = (ImagePtr*)lua_newuserdatauv(L, sizeof(ImagePtr), 1);
	luaL_getmetatable(L, IMAGE_META);
	lua_setmetatable(L, -2);
	i->p = screen;
	lua_setglobal(L, "screen");
	d = (DisplayPtr*)lua_newuserdata(L, sizeof(DisplayPtr));
	luaL_getmetatable(L, DISPLAY_META);
	lua_setmetatable(L, -2);
	d->p = display;
	lua_setglobal(L, "display");
	f = (FontPtr*)lua_newuserdata(L, sizeof(FontPtr));
	luaL_getmetatable(L, FONT_META);
	lua_setmetatable(L, -2);
	f->p = font;
	lua_setglobal(L, "font");
}

static int l_initdraw(lua_State *L) {
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
	create_globals(L);
	return 0;
}

static int l_einit(lua_State *L) {
	lua_Integer i;

	i = luaL_checknumber(L, -1);
	einit((ulong)i);
	return 0;
}

static int l_event(lua_State *L) {
	Event ev;
	int e;

	e = event(&ev);
	lua_pushnumber(L, e);
	lua_newtable(L);
	lua_pushinteger(L, ev.kbdc);
	lua_setfield(L, -2, "kbdc");
	return 2;
}

static Point l_getpoint(lua_State *L, int index)
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

static Point l_checkpoint(lua_State *L, int index)
{
	if(lua_istable(L, index) == 0)
		luaL_argerror(L, index, "draw: point table expected");
	return l_getpoint(L, index);
}

static Point l_optpoint(lua_State *L, int index)
{
	if(lua_istable(L, index) == 0)
		return ZP;
	return l_getpoint(L, index);
}

static Rectangle l_getrect(lua_State *L, int index)
{
	Rectangle r;
	int t;

	lua_pushstring(L, "min");
	lua_gettable(L, index);
	lua_pushstring(L, "x");
	lua_gettable(L, -2);
	r.min.x = luaL_checkinteger(L, -1);
	lua_pushstring(L, "y");
	t = lua_gettable(L, -3);
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

static Rectangle l_checkrect(lua_State *L, int index)
{
	if(lua_istable(L, index) == 0)
		luaL_argerror(L, index, "draw: rectangle table expected");
	return l_getrect(L, index);
}

static Image* l_checkimage(lua_State *L, int index)
{
	ImagePtr *p;

	p = (ImagePtr*)luaL_checkudata(L, index, IMAGE_META);
	luaL_argcheck(L, p != NULL, index, "draw: Image expected");
	return p->p;
}

static Image* l_optimage(lua_State *L, int index)
{
	if(lua_isnil(L, index))
		return nil;
	return l_checkimage(L, index);
}

static Font* l_checkfont(lua_State *L, int index)
{
	FontPtr *p;

	p = (FontPtr*)luaL_checkudata(L, index, FONT_META);
	luaL_argcheck(L, p != NULL, index, "draw: Font expected");
	return p->p;
}

static Display* l_checkdisplay(lua_State *L, int index)
{
	DisplayPtr *p;

	p = (DisplayPtr*)luaL_checkudata(L, index, DISPLAY_META);
	luaL_argcheck(L, p!=NULL, index, "draw: Display expected");
	return p->p;
}

static int l_draw(lua_State *L) {
	Image *dst, *src, *mask;
	Point p;
	Rectangle r;

	dst  = l_checkimage(L, 1);
	r    = l_checkrect(L, 2);
	src  = l_checkimage(L, 3);
	mask = l_optimage(L, 4);
	p    = l_checkpoint(L, 5);
	draw(dst, r, src, mask, p);
	return 0;
}

static int l_line(lua_State *L) {
	Image *dst, *src;
	Point p0, p1, sp;
	int end0, end1, thick;

	dst   = l_checkimage(L, 1);
	p0    = l_checkpoint(L, 2);
	p1    = l_checkpoint(L, 3);
	end0  = luaL_checkinteger(L, 4);
	end1  = luaL_checkinteger(L, 5);
	thick = luaL_checkinteger(L, 6);
	src   = l_checkimage(L, 7);
	sp    = l_checkpoint(L, 8);
	line(dst, p0, p1, end0, end1, thick, src, sp);
	return 0;
}

static int l_ellipse(lua_State *L)
{
	Image *dst, *src;
	Point c, sp;
	int a, b, thick;

	dst   = l_checkimage(L, 1);
	c     = l_checkpoint(L, 2);
	a     = luaL_checkinteger(L, 3);
	b     = luaL_checkinteger(L, 4);
	thick = luaL_checkinteger(L, 5);
	src   = l_checkimage(L, 6);
	sp    = l_checkpoint(L, 7);
	ellipse(dst, c, a, b, thick, src, sp);
	return 0;
}

static int l_fillellipse(lua_State *L)
{
	Image *dst, *src;
	Point c, sp;
	int a, b;

	dst   = l_checkimage(L, 1);
	c     = l_checkpoint(L, 2);
	a     = luaL_checkinteger(L, 3);
	b     = luaL_checkinteger(L, 4);
	src   = l_checkimage(L, 5);
	sp    = l_checkpoint(L, 6);
	fillellipse(dst, c, a, b, src, sp);
	return 0;
}

static int l_string(lua_State *L) {
	Image *dst, *src;
	Font *f;
	Point p, sp;
	const char *s;

	dst = l_checkimage(L, 1);
	p   = l_checkpoint(L, 2);
	src = l_checkimage(L, 3);
	sp  = l_optpoint(L, 4);
	f   = l_checkfont(L, 5);
	s   = luaL_checkstring(L, 6);
	string(dst, p, src, sp, f, s);
	return 0;
}

static int l_allocimage(lua_State *L)
{
	Display *d;
	Rectangle r;
	ulong chan, col;
	int repl;
	Image *i;

	d    = l_checkdisplay(L, 1);
	r    = l_checkrect(L, 2);
	chan = (ulong)luaL_checkinteger(L, 3);
	repl = luaL_checkinteger(L, 4);
	col  = (ulong)luaL_checkinteger(L, 5);
	i    = allocimage(d, r, chan, repl, col);
	to_image(L, i);
	return 1;
}

/* Image metatable */
static int l_image_gc(lua_State *L) {
	ImagePtr *i;

	i = (ImagePtr*)luaL_checkudata(L, 1, IMAGE_META);
	luaL_argcheck(L, i != NULL, 1, "draw: Image expected");
	if(i->p == screen) {
		lua_pushboolean(L, 0);
		return 1;
	}
	/* TODO freeimage */
	lua_pushboolean(L, 1);
	return 1;
}

static int l_image_tostring(lua_State *L) {
	void *p;
	char buf[64];

	p = lua_touserdata(L, 1);
	snprintf(buf, sizeof buf, "image: %p", p);
	lua_pushstring(L, buf);
	return 1;
}

static void l_pushrect(lua_State *L, Rectangle r)
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

static int l_image_index(lua_State *L) {
	Image *i;
	const char *s;

	i = l_checkimage(L, 1);
	s = luaL_checkstring(L, 2);
	if(!strncmp(s, "r", 1)) {
		l_pushrect(L, i->r);
	} else if(!strncmp(s, "clipr", 5)) {
		l_pushrect(L, i->clipr);
	} else if(!strncmp(s, "chan", 4)) {
		lua_pushinteger(L, i->chan);
	} else if(!strncmp(s, "depth", 5)) {
		lua_pushinteger(L, i->depth);
	} else if(!strncmp(s, "repl", 4)) {
		lua_pushinteger(L, i->repl);
	} else {
		return 0;
	}
	return 1;
}

static const struct luaL_Reg image_funcs[] = {
	{ "__gc", l_image_gc },
	{ "__tostring", l_image_tostring },
	{ "__index", l_image_index },
	{ NULL, NULL },
};

static int l_display_gc(lua_State *L) {
	/* we do not GC the display */
	lua_pushboolean(L, 0);
	return 1;
}

static int l_display_tostring(lua_State *L) {
	void *p;
	char buf[64];

	p = lua_touserdata(L, 1);
	snprintf(buf, sizeof buf, "display: %p", p);
	lua_pushstring(L, buf);
	return 1;
}

static int l_display_index(lua_State *L) {
	DisplayPtr *d;
	const char *s;

	d = (DisplayPtr*)luaL_checkudata(L, 1, DISPLAY_META);
	luaL_argcheck(L, d != NULL, 1, "draw: Display expected");
	s = luaL_checkstring(L, 2);
	if(strncmp(s, "white", 5) == 0) {
		to_image(L, d->p->white);
		return 1;
	} else if(strncmp(s, "black", 5) == 0) {
		to_image(L, d->p->black);
		return 1;
	}
	return 0;
}

static const struct luaL_Reg display_funcs[] = {
	{ "__gc", l_display_gc },
	{ "__tostring", l_display_tostring },
	{ "__index", l_display_index },
	{ NULL, NULL },
};

static int l_font_gc(lua_State *L) {
	/* TODO */
	lua_pushboolean(L, 0);
	return 1;
}

static int l_font_tostring(lua_State *L) {
	void *p;

	p = lua_touserdata(L, 1);
	lua_pushfstring(L, "font: %p", p);
	return 1;
}

static const struct luaL_Reg font_funcs[] = {
	{ "__gc", l_font_gc },
	{ "__tostring", l_font_tostring },
//	{ "__index", l_display_index },
	{ NULL, NULL },
};

static void create_metatable(lua_State *L, const char *name, luaL_Reg *funcs) {
	luaL_newmetatable(L, name);
	luaL_setfuncs(L, funcs, 0);
	lua_pushliteral (L, "__metatable");
	lua_pushliteral (L, "draw: you're not allowed to get this metatable");
	lua_settable (L, -3);
}

static const struct luaL_Reg drawlib [] = {
	{ "initdraw",    l_initdraw },
	{ "einit",       l_einit },
	{ "event",       l_event },
	{ "draw",        l_draw },
	{ "line",        l_line },
	{ "ellipse",     l_ellipse },
	{ "fillellipse", l_fillellipse },
	{ "string",      l_string },
	{ "allocimage",  l_allocimage },
	{ NULL, NULL }
};

static void l_pushglobal(lua_State *L, const char *name, int value, int index)
{
	lua_pushnumber(L, value);
	lua_setfield(L, index, name);	
}

int luaopen_drawlib (lua_State *L) {
	create_metatable(L, IMAGE_META, image_funcs);
	create_metatable(L, DISPLAY_META, display_funcs);
	create_metatable(L, FONT_META, font_funcs);
	luaL_newlib(L, drawlib);
	lua_pushnumber(L, Emouse);
	lua_setfield(L, -2, "Emouse");
	lua_pushnumber(L, Ekeyboard);
	lua_setfield(L, -2, "Ekeyboard");
	l_pushglobal(L, "Endsquare", Endsquare, -2);
	l_pushglobal(L, "Enddisc", Enddisc, -2);
	l_pushglobal(L, "Endarrow", Endarrow, -2);
	l_pushglobal(L, "Endmask", Endmask, -2);

	return 1;
}


int
main(int argc, char *argv[])
{
	lua_State *L;
	char *f = NULL;
	int r;

	L = luaL_newstate();
	luaL_openlibs(L);
    	luaL_requiref(L, "draw", luaopen_drawlib, 1);
    	lua_pop(L, 1);
	if(argc > 1)
		f = argv[1];
	r = luaL_dofile(L, f);
	lua_close(L);
	return f == LUA_OK;
}
