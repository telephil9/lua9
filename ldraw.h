#ifndef LDRAW_H__
#define LDRAW_H__

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

/* libs */
int openlibdraw(lua_State *L);
int openlibkey(lua_State *L);
int openlibcolor(lua_State *L);

#endif
