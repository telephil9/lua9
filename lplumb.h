#ifndef LPLUMB_H__
#define LPLUMB_H__

void registerplumbmsgmeta(lua_State *L);
void pushplumbmsg(lua_State *L, Plumbmsg *m);
Plumbmsg* checkplumbmsg(lua_State *L, int index);

void registerplumbattrmeta(lua_State *L);
void pushplumbattr(lua_State *L, Plumbattr *a);
Plumbattr* checkplumbattr(lua_State *L, int index);

int openlibplumb(lua_State *L);

#endif
