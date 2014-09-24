static void lSDL_Window_destroy(void *ptr) { SDL_DestroyWindow(ptr); }

static int lSDL_Window_gc(lua_State *L)
{
	lSDL_unrefobject(L, 1, LSDL2_MT_WINDOW);
	return 0;
}

static int lSDL_ShowWindow(lua_State *L)
{
	SDL_ShowWindow(lSDL_checkobject(L, 1, LSDL2_MT_WINDOW));
	return 0;
}

static int lSDL_SetWindowTitle(lua_State *L)
{
	SDL_SetWindowTitle(lSDL_checkobject(L, 1, LSDL2_MT_WINDOW), luaL_checkstring(L, 2));
	return 0;
}

static int lSDL_HideWindow(lua_State *L)
{
	SDL_HideWindow(lSDL_checkobject(L, 1, LSDL2_MT_WINDOW));
	return 0;
}

static int lSDL_MinimizeWindow(lua_State *L)
{
	SDL_MinimizeWindow(lSDL_checkobject(L, 1, LSDL2_MT_WINDOW));
	return 0;
}

static int lSDL_CreateWindow(lua_State *L)
{
	const char *title = luaL_checkstring(L, 1);
	int w, h;
	unsigned int flags;
	SDL_Window *window;
	if(!lua_isnoneornil(L, 5)) {
		int x = luaL_checkint(L, 2), y = luaL_checkint(L, 3);
		w = luaL_checkint(L, 4), h = luaL_checkint(L, 5);
		flags = luaL_checkint(L, 6);
		window = SDL_CreateWindow(title, x, y, w, h, flags);
	} else {
		w = luaL_checkint(L, 2), h = luaL_checkint(L, 3);
		flags = luaL_checkint(L, 4);
		window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			w, h, flags);
	}
	if(window) {
		lSDL_newobject(L, window, LSDL2_MT_WINDOW, NULL, lSDL_Window_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_ERROR, SDL_GetError());
		return 0;
	}
}

static int lSDL_Window_tostring(lua_State *L)
{
	lua_pushstring(L, LSDL2_MT_WINDOW);
	return 1;
}

static luaL_Reg mt_window[] = {
	{ "__tostring", lSDL_Window_tostring },
	{ "__gc", lSDL_Window_gc },
	{ NULL, NULL }
};