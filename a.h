#include <u.h>
#include <libc.h>
#include <draw.h>
#include <keyboard.h>
#include <event.h>
#include <plumb.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/* display */
void registerdisplaymeta(lua_State *L);
void pushdisplay(lua_State *L, Display *d);
Display* checkdisplay(lua_State *L, int index);

/* font */
void registerfontmeta(lua_State *L);
void pushfont(lua_State *L, Font *f);
Font* checkfont(lua_State *L, int index);

/* image */
void registerimagemeta(lua_State *L);
void pushimage(lua_State *L, Image *i);
Image* checkimage(lua_State *L, int index);
Image* optimage(lua_State *L, int index);

/* geometry */
void pushrect(lua_State *L, Rectangle r);
Rectangle checkrect(lua_State *L, int index);
void pushpoint(lua_State *L, Point p);
Point checkpoint(lua_State *L, int index);
Point getpoint(lua_State *L, int index);
Point optpoint(lua_State *L, int index);
Point* checkpoints(lua_State *L, int index, int *np);

/* plumb */
void registerplumbmsgmeta(lua_State *L);
void pushplumbmsg(lua_State *L, Plumbmsg *m);
Plumbmsg* checkplumbmsg(lua_State *L, int index);
void registerplumbattrmeta(lua_State *L);
void pushplumbattr(lua_State *L, Plumbattr *a);
Plumbattr* checkplumbattr(lua_State *L, int index);

/* libs */
int openlibdraw(lua_State *L);
int openlibevent(lua_State *L);
int openlibgeometry(lua_State *L);
int openlibkey(lua_State *L);
int openlibcolor(lua_State *L);
int openlib9(lua_State *L);
int openlibplumb(lua_State *L);

/* utils */
void pushglobal(lua_State *L, const char *name, int value);
void createmetatable(lua_State *L, const char *name, luaL_Reg *funcs);

