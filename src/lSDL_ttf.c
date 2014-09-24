#include <SDL_ttf.h>

static void lSDL_Font_destroy(void *ptr) { TTF_CloseFont(ptr); }

static int lSDL_Font_gc(lua_State *L)
{
	lSDL_unrefobject(L, 1, LSDL2_MT_FONT);
	return 0;
}

static int lTTF_RenderSolid(lua_State *L)
{
	TTF_Font *font = lSDL_checkobject(L, 1, LSDL2_MT_FONT);
	SDL_Color c = luaZ_checkSDLcolor(L, 3);
	SDL_Surface *surface = TTF_RenderUTF8_Solid(font, luaL_checkstring(L, 2), c);
	if(surface) {
		lSDL_newobject(L, surface, LSDL2_MT_SURFACE, NULL, lSDL_Surface_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_TTF, TTF_GetError());
		return 0;
	}
}

static int lTTF_RenderShaded(lua_State *L)
{
	TTF_Font *font = lSDL_checkobject(L, 1, LSDL2_MT_FONT);
	SDL_Color c = luaZ_checkSDLcolor(L, 3);
	SDL_Color b = luaZ_checkSDLcolor(L, 4);
	SDL_Surface *surface = TTF_RenderUTF8_Shaded(font, luaL_checkstring(L, 2), c, b);
	if(surface) {
		lSDL_newobject(L, surface, LSDL2_MT_SURFACE, NULL, lSDL_Surface_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_TTF, TTF_GetError());
		return 0;
	}
}

static int lTTF_Size(lua_State *L)
{
	TTF_Font *font = lSDL_checkobject(L, 1, LSDL2_MT_FONT);
	int w, h;
	if(0 == TTF_SizeUTF8(font, luaL_checkstring(L, 2), &w, &h)) {
		lua_pushinteger(L, w);
		lua_pushinteger(L, h);
		return 2;
	} else {
		luaL_error(L, LSDL2_FMT_TTF, TTF_GetError());
		return 0;
	}
}

static int lTTF_RenderBlended(lua_State *L)
{
	TTF_Font *font = lSDL_checkobject(L, 1, LSDL2_MT_FONT);
	SDL_Color c = luaZ_checkSDLcolor(L, 3);
	SDL_Surface *surface = TTF_RenderUTF8_Blended(font, luaL_checkstring(L, 2), c);
	if(surface) {
		lSDL_newobject(L, surface, LSDL2_MT_SURFACE, NULL, lSDL_Surface_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_TTF, TTF_GetError());
		return 0;
	}
}

static int lTTF_RenderWrapped(lua_State *L)
{
	TTF_Font *font = lSDL_checkobject(L, 1, LSDL2_MT_FONT);
	SDL_Color c = luaZ_checkSDLcolor(L, 3);
	SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(
		font, luaL_checkstring(L, 2), c, luaL_checkinteger(L, 4));
	if(surface) {
		lSDL_newobject(L, surface, LSDL2_MT_SURFACE, NULL, lSDL_Surface_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_TTF, TTF_GetError());
		return 0;
	}
}

static int lTTF_OpenFont(lua_State *L)
{
	TTF_Font *font = TTF_OpenFont(luaL_checkstring(L, 1), luaL_checkinteger(L, 2));
	if(font) {
		lSDL_newobject(L, font, LSDL2_MT_FONT, NULL, lSDL_Font_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_TTF, TTF_GetError());
		return 0;
	}
}

static int lTTF_OpenFontIndex(lua_State *L)
{
	TTF_Font *font = TTF_OpenFontIndex(
		luaL_checkstring(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
	if(font) {
		lSDL_newobject(L, font, LSDL2_MT_FONT, NULL, lSDL_Font_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_TTF, TTF_GetError());
		return 0;
	}
}

static int lSDL_Font_tostring(lua_State *L)
{
	lua_pushstring(L, LSDL2_MT_FONT);
	return 1;
}

static luaL_Reg mt_font[] = {
	{ "__tostring", lSDL_Font_tostring },
	{ "__gc", lSDL_Font_gc },
	{ NULL, NULL }
};