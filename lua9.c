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

static int l_draw(lua_State *L) {
	ImagePtr *p;
	Image *dst, *src, *mask;
	Point pt;
	Rectangle r;
	
	p = (ImagePtr*)luaL_checkudata(L, 1, IMAGE_META);
	luaL_argcheck(L, p != NULL, 1, "draw: Image expected");
	dst = p->p;
	if(lua_istable(L, 2) == 0) {
		luaL_argerror(L, 2, "draw: table expected");
	} else {
		r = l_getrect(L, 2);
	}
	p = (ImagePtr*)luaL_checkudata(L, 3, IMAGE_META);
	luaL_argcheck(L, p != NULL, 3, "draw: Image expected");
	src = p->p;
	mask = nil;
	if(lua_isnil(L, 4) == 0) {
		p = (ImagePtr*)luaL_checkudata(L, 4, IMAGE_META);
		luaL_argcheck(L, p != NULL, 4, "draw: Image expected");
		mask = p->p;
	}
	if(lua_istable(L, 5) == 0) {
		luaL_argerror(L, 2, "draw: table expected");
	} else {
		pt = l_getpoint(L, 5);
	}
	draw(dst, r, src, mask, pt);
	return 0;
}

static int l_string(lua_State *L) {
	ImagePtr *p;
	FontPtr *fp;
	Image *dst, *src;
	Font *f;
	Point pt, spt;
	const char *s;

	p = (ImagePtr*)luaL_checkudata(L, 1, IMAGE_META);
	luaL_argcheck(L, p != NULL, 1, "draw: Image expected");
	dst = p->p;
	if(lua_istable(L, 2)) {
		pt = l_getpoint(L, 2);
	} else {
		luaL_argerror(L, 2, "draw: table expected");
	}
	p = (ImagePtr*)luaL_checkudata(L, 3, IMAGE_META);
	luaL_argcheck(L, p != NULL, 3, "draw: Image expected");
	src = p->p;
	if(lua_istable(L, 4)) {
		spt = l_getpoint(L, 4);
	} else {
		spt = ZP;
	}
	fp = (FontPtr*)luaL_checkudata(L, 5, FONT_META);
	luaL_argcheck(L, fp != NULL, 5, "draw: Font expected");
	f = fp->p;
	s = luaL_checkstring(L, 6);
	string(dst, pt, src, spt, f, s);
	return 0;
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

static int l_image_index(lua_State *L) {
	ImagePtr *i;
	const char *s;
	Rectangle r;

	i = (ImagePtr*)luaL_checkudata(L, 1, IMAGE_META);
	luaL_argcheck(L, i != NULL, 1, "draw: Image expected");
	s = luaL_checkstring(L, 2);
	if(!strncmp(s, "r", 1)) {
		r = i->p->r;
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
		return 1;
	}
	return 0;
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

static ImagePtr* to_image(lua_State *L, Image *img) {
	ImagePtr *i;

	i = (ImagePtr*)lua_newuserdata(L, sizeof(ImagePtr));
	luaL_getmetatable(L, IMAGE_META);
	lua_setmetatable(L, -2);
	i->p = img;
	return i;
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
	{ "initdraw", l_initdraw },
	{ "einit",    l_einit },
	{ "event",    l_event },
	{ "draw",     l_draw },
	{ "string",   l_string },
	{ NULL, NULL }
};

int luaopen_drawlib (lua_State *L) {
	create_metatable(L, IMAGE_META, image_funcs);
	create_metatable(L, DISPLAY_META, display_funcs);
	create_metatable(L, FONT_META, font_funcs);
	luaL_newlib(L, drawlib);
	lua_pushnumber(L, Emouse);
	lua_setfield(L, -2, "Emouse");
	lua_pushnumber(L, Ekeyboard);
	lua_setfield(L, -2, "Ekeyboard");
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
