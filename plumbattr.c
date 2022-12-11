#include "a.h"

#define PLUMBATTR "Plumbattr"

typedef struct LPlumbattr LPlumbattr;

struct LPlumbattr
{
	Plumbattr *a;
};

void
pushplumbattr(lua_State *L, Plumbattr *a)
{
	LPlumbattr *l;

	l = (LPlumbattr*)lua_newuserdata(L, sizeof(LPlumbattr));
	luaL_getmetatable(L, PLUMBATTR);
	lua_setmetatable(L, -2);
	l->a = a;
}

Plumbattr*
checkplumbattr(lua_State *L, int index)
{
	LPlumbattr *l;

	l = (LPlumbattr*)luaL_checkudata(L, index, PLUMBATTR);
	luaL_argcheck(L, l != NULL, index, "Plumbattr expected");
	return l->a;
}

static int
plumbattr__gc(lua_State *L)
{
	/* already freed by plumbmsg gc */
	lua_pushboolean(L, 0);
	return 1;
}

static int
plumbattr__tostring(lua_State *L)
{
	void *p;

	p = lua_touserdata(L, 1);
	lua_pushfstring(L, "plumbattr: %p", p);
	return 1;
}

static int
plumbattr__index(lua_State *L)
{
	Plumbattr *a;
	const char *s;

	a = checkplumbattr(L, 1);
	s = luaL_checkstring(L, 2);
	if(strncmp(s, "name", 4) == 0)
		lua_pushstring(L, a->name);
	else if(strncmp(s, "value", 5) == 0)
		lua_pushstring(L, a->value);
	else if(strncmp(s, "next", 4) == 0)
		pushplumbattr(L, a->next);
	else
		return 0;
	return 1;
}

static const struct luaL_Reg plumbattr_funcs[] = {
	{ "__gc", plumbattr__gc },
	{ "__tostring", plumbattr__tostring },
	{ "__index", plumbattr__index },
	{ NULL, NULL },
};

void
registerplumbattrmeta(lua_State *L)
{
	createmetatable(L, PLUMBATTR, plumbattr_funcs);
}
