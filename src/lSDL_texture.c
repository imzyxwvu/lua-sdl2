static void lSDL_Texture_destroy(void *ptr) { SDL_DestroyTexture(ptr); }

static int lSDL_Texture_gc(lua_State *L)
{
	lSDL_unrefobject(L, 1, LSDL2_MT_TEXTURE);
	return 0;
}

static int lSDL_CreateTextureFromSurface(lua_State *L)
{
	lSDL_Object *render = lSDL_checkref(L, 1, LSDL2_MT_RENDERER);
	SDL_Surface *surface = lSDL_checkobject(L, 2, LSDL2_MT_SURFACE);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(render->ptr, surface);
	if(texture) {
		lSDL_newobject(L, texture, LSDL2_MT_TEXTURE, render, lSDL_Texture_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_ERROR, SDL_GetError());
		return 0;
	}
}

static int lSDL_Texture_tostring(lua_State *L)
{
	lua_pushstring(L, LSDL2_MT_TEXTURE);
	return 1;
}

static luaL_Reg mt_texture[] = {
	{ "__tostring", lSDL_Texture_tostring },
	{ "__gc", lSDL_Texture_gc },
	{ NULL, NULL }
};