static void lSDL_Renderer_destroy(void *ptr) { SDL_DestroyRenderer(ptr); }

static int lSDL_Renderer_gc(lua_State *L)
{
	lSDL_unrefobject(L, 1, LSDL2_MT_RENDERER);
	return 0;
}

static int lSDL_RenderPresent(lua_State *L)
{
	SDL_RenderPresent(lSDL_checkobject(L, 1, LSDL2_MT_RENDERER));
	return 0;
}

static int lSDL_SetRenderDrawColor(lua_State *L)
{
	SDL_SetRenderDrawColor(
		lSDL_checkobject(L, 1, LSDL2_MT_RENDERER),
		luaL_checkinteger(L, 2),
		luaL_checkinteger(L, 3),
		luaL_checkinteger(L, 4),
		luaL_optinteger(L, 5, 255)
	);
	return 0;
}

static int lSDL_RenderClear(lua_State *L)
{
	SDL_RenderClear(lSDL_checkobject(L, 1, LSDL2_MT_RENDERER));
	return 0;
}

static int lSDL_CreateSoftwareRenderer(lua_State *L)
{
	lSDL_Object *surface = lSDL_checkref(L, 1, LSDL2_MT_SURFACE);
	SDL_Renderer *render = SDL_CreateSoftwareRenderer(surface->ptr);
	if(render) {
		lSDL_newobject(L, render, LSDL2_MT_RENDERER, surface, lSDL_Renderer_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_ERROR, SDL_GetError());
		return 0;
	}
}

static int lSDL_RenderCopy(lua_State *L)
{
	SDL_Renderer *render = lSDL_checkobject(L, 1, LSDL2_MT_RENDERER);
	SDL_Texture *texture = lSDL_checkobject(L, 2, LSDL2_MT_TEXTURE);
	SDL_Rect dstrect;
	int result;
	if(!lua_isnoneornil(L, 7)) {
		SDL_Rect srcrect;
		srcrect.x = luaL_checkinteger(L, 3); srcrect.y = luaL_checkinteger(L, 4);
		srcrect.w = luaL_checkinteger(L, 5); srcrect.h = luaL_checkinteger(L, 6);
		dstrect.x = luaL_checkinteger(L, 7); dstrect.y = luaL_checkinteger(L, 8);
		dstrect.w = luaL_checkinteger(L, 9); dstrect.h = luaL_checkinteger(L, 10);
		result = SDL_RenderCopy(render, texture, &srcrect, &dstrect);
	} else if(!lua_isnoneornil(L, 5)) {
		dstrect.x = luaL_checkinteger(L, 3); dstrect.y = luaL_checkinteger(L, 4);
		dstrect.w = luaL_checkinteger(L, 5); dstrect.h = luaL_checkinteger(L, 6);
		result = SDL_RenderCopy(render, texture, NULL, &dstrect);
	} else {
		dstrect.x = luaL_checkinteger(L, 3); dstrect.y = luaL_checkinteger(L, 4);
		SDL_QueryTexture(texture, NULL, NULL, &(dstrect.w), &(dstrect.h));
		result = SDL_RenderCopy(render, texture, NULL, &dstrect);
	}
	if (0 != result) luaL_error(L, LSDL2_FMT_ERROR, SDL_GetError());
	return 0;
}

static int lSDL_CreateRenderer(lua_State *L)
{
	lSDL_Object *window = lSDL_checkref(L, 1, LSDL2_MT_WINDOW);
	unsigned int flags = luaL_optinteger(L, 2, SDL_RENDERER_ACCELERATED);
	SDL_Renderer *render = SDL_CreateRenderer(window->ptr, -1, flags);
	if(render) {
		lSDL_newobject(L, render, LSDL2_MT_RENDERER, window, lSDL_Renderer_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_ERROR, SDL_GetError());
		return 0;
	}
}

static int lSDL_Renderer_tostring(lua_State *L)
{
	lua_pushstring(L, LSDL2_MT_RENDERER);
	return 1;
}

static luaL_Reg mt_renderer[] = {
	{ "__tostring", lSDL_Renderer_tostring },
	{ "__gc", lSDL_Renderer_gc },
	{ NULL, NULL }
};