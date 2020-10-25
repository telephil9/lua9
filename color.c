#include <draw.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "utils.h"

static const struct luaL_Reg libcolor[] = {
	{ NULL, NULL },
};

int
openlibcolor(lua_State *L)
{
	luaL_newlib(L, libcolor);
	pushglobal(L, "OPAQUE", DOpaque);
	pushglobal(L, "TRANSPARENT", DTransparent);
	pushglobal(L, "BLACK", DBlack);
	pushglobal(L, "WHITE", DWhite);
	pushglobal(L, "RED", DRed);
	pushglobal(L, "GREEN", DGreen);
	pushglobal(L, "BLUE", DBlue);
	pushglobal(L, "CYAN", DCyan);
	pushglobal(L, "MAGENTA", DMagenta);
	pushglobal(L, "YELLOW", DYellow);
	pushglobal(L, "PALE_YELLOW", DPaleyellow);
	pushglobal(L, "DARK_YELLOW", DDarkyellow);
	pushglobal(L, "PALE_GREEN", DPalegreen);
	pushglobal(L, "DARK_GREEN", DDarkgreen);
	pushglobal(L, "MED_GREEN", DMedgreen);
	pushglobal(L, "DARK_BLUE", DDarkblue);
	pushglobal(L, "PALE_BLUE_GREEN", DPalebluegreen);
	pushglobal(L, "PALE_BLUE", DPaleblue);
	pushglobal(L, "BLUE_GREEN", DBluegreen);
	pushglobal(L, "GREY_GREEN", DGreygreen);
	pushglobal(L, "PALE_GREY_GREEN", DPalegreygreen);
	pushglobal(L, "YELLOW_GREEN", DYellowgreen);
	pushglobal(L, "MED_BLUE", DMedblue);
	pushglobal(L, "GREY_BLUE", DGreyblue);
	pushglobal(L, "PALE_GREY_BLUE", DPalegreyblue);
	pushglobal(L, "PURPLE_BLUE", DPurpleblue);
	pushglobal(L, "NOT_A_COLOR", DNotacolor);
	pushglobal(L, "NO_FILL", DNofill);
	return 1;
}
	












