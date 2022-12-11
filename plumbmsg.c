#include "a.h"

#define PLUMBMSG "Plumbmsg"

typedef struct LPlumbmsg LPlumbmsg;

struct LPlumbmsg
{
	Plumbmsg *m;
};

void
pushplumbmsg(lua_State *L, Plumbmsg *m)
{
	LPlumbmsg *l;

	l = (LPlumbmsg*)lua_newuserdata(L, sizeof(LPlumbmsg));
	luaL_getmetatable(L, PLUMBMSG);
	lua_setmetatable(L, -2);
	l->m = m;
}

Plumbmsg*
checkplumbmsg(lua_State *L, int index)
{
	LPlumbmsg *l;

	l = (LPlumbmsg*)luaL_checkudata(L, index, PLUMBMSG);
	luaL_argcheck(L, l != NULL, index, "Plumbmsg expected");
	return l->m;
}

static int
plumbmsg__gc(lua_State *L)
{
	LPlumbmsg *l;

	l = (LPlumbmsg*)luaL_checkudata(L, 1, PLUMBMSG);
	luaL_argcheck(L, l != NULL, 1, "Plumbmsg expected");
	plumbfree(l->m);
	free(l);
	lua_pushboolean(L, 1);
	return 1;
}

static int
plumbmsg__tostring(lua_State *L)
{
	void *p;

	p = lua_touserdata(L, 1);
	lua_pushfstring(L, "plumbmsg: %p", p);
	return 1;
}

static int
plumbmsg__index(lua_State *L)
{
	Plumbmsg *m;
	const char *s;

	m = checkplumbmsg(L, 1);
	s = luaL_checkstring(L, 2);
	if(strncmp(s, "src", 3) == 0)
		lua_pushstring(L, m->src);
	else if(strncmp(s, "dst", 3) == 0)
		lua_pushstring(L, m->dst);
	else if(strncmp(s, "wdir", 4) == 0)
		lua_pushstring(L, m->wdir);
	else if(strncmp(s, "type", 4) == 0)
		lua_pushstring(L, m->type);
	else if(strncmp(s, "data", 4) == 0)
		lua_pushstring(L, m->data);
	else if(strncmp(s, "attr", 4) == 0)
		pushplumbattr(L, m->attr);
	else
		return 0;
	return 1;
}

static const struct luaL_Reg plumbmsg_funcs[] = {
	{ "__gc", plumbmsg__gc },
	{ "__tostring", plumbmsg__tostring },
	{ "__index", plumbmsg__index },
	{ NULL, NULL },
};

void
registerplumbmsgmeta(lua_State *L)
{
	createmetatable(L, PLUMBMSG, plumbmsg_funcs);
}
