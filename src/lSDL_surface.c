static void lSDL_Surface_destroy(void *ptr) { SDL_FreeSurface(ptr); }

static int lSDL_Surface_gc(lua_State *L)
{
	lSDL_unrefobject(L, 1, LSDL2_MT_SURFACE);
	return 0;
}

static int lSDL_Surface_index(lua_State *L)
{
	SDL_Surface *surface = lSDL_checkobject(L, 1, LSDL2_MT_SURFACE);
	int id = luaL_checkinteger(L, 2);
	switch(id) {
	case 1:
		lua_pushinteger(L, surface->w);
		return 1;
	case 2:
		lua_pushinteger(L, surface->h);
		return 1;
	default:
		return 0;
	}
}

static int lSDL_BlitSurface(lua_State *L)
{
	SDL_Surface *src = lSDL_checkobject(L, 1, LSDL2_MT_SURFACE);
	SDL_Surface *dst = lSDL_checkobject(L, 2, LSDL2_MT_SURFACE);
	SDL_Rect dstrect;
	int result;
	if(!lua_isnoneornil(L, 5)) {
		SDL_Rect srcrect;
		srcrect.x = luaL_checkinteger(L, 3); srcrect.y = luaL_checkinteger(L, 4);
		srcrect.w = luaL_checkinteger(L, 5); srcrect.h = luaL_checkinteger(L, 6);
		dstrect.x = luaL_checkinteger(L, 7); dstrect.y = luaL_checkinteger(L, 8);
		result = SDL_BlitSurface(src, &srcrect, dst, &dstrect);
	} else {
		dstrect.x = luaL_checkinteger(L, 3); dstrect.y = luaL_checkinteger(L, 4);
		result = SDL_BlitSurface(src, NULL, dst, &dstrect);
	}
	if (0 != result) luaL_error(L, LSDL2_FMT_ERROR, SDL_GetError());
	return 0;
}

static int lSDL_FillRect(lua_State *L)
{
	SDL_Surface *surface = lSDL_checkobject(L, 1, LSDL2_MT_SURFACE);
	SDL_Rect rect;
	rect.x = luaL_checkinteger(L, 2); rect.y = luaL_checkinteger(L, 3);
	rect.w = luaL_checkinteger(L, 4); rect.h = luaL_checkinteger(L, 5);
	if (0 != SDL_FillRect(surface, &rect, SDL_MapRGBA(
		surface->format,
		luaL_checkinteger(L, 6),
		luaL_checkinteger(L, 7),
		luaL_checkinteger(L, 8),
		luaL_optinteger(L, 9, 255)
		)))
		luaL_error(L, LSDL2_FMT_ERROR, SDL_GetError());
	return 0;
}

static int lSDL_SetSurfaceAlphaMod(lua_State *L)
{
	SDL_SetSurfaceAlphaMod(
		lSDL_checkobject(L, 1, LSDL2_MT_SURFACE),
		luaL_checkinteger(L, 2)
	);
	return 0;
}

static int lSDL_SetSurfaceBlendMode(lua_State *L)
{
	SDL_SetSurfaceBlendMode(
		lSDL_checkobject(L, 1, LSDL2_MT_SURFACE),
		luaL_checkinteger(L, 2)
	);
	return 0;
}

static int lSDL_CreateRGBSurface(lua_State *L)
{
	Uint32 rmask, gmask, bmask, amask;
	SDL_Surface *surface;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00; amask = 0x000000ff;
#else
    rmask = 0x000000ff; gmask = 0x0000ff00; bmask = 0x00ff0000; amask = 0xff000000;
#endif

	surface = SDL_CreateRGBSurface(0, luaL_checkinteger(L, 1), luaL_checkinteger(L, 2),
		32, rmask, gmask, bmask, amask);
	if(surface) {
		lSDL_newobject(L, surface, LSDL2_MT_SURFACE, NULL, lSDL_Surface_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_ERROR, SDL_GetError());
		return 0;
	}
}

static int lSDL_LoadBMP(lua_State *L)
{
	SDL_Surface *surface = SDL_LoadBMP(luaL_checkstring(L, 1));
	if(surface) {
		lSDL_newobject(L, surface, LSDL2_MT_SURFACE, NULL, lSDL_Surface_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_ERROR, SDL_GetError());
		return 0;
	}
}

static int lIMG_Load(lua_State *L)
{
	SDL_Surface *surface = IMG_Load(luaL_checkstring(L, 1));
	if(surface) {
		lSDL_newobject(L, surface, LSDL2_MT_SURFACE, NULL, lSDL_Surface_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_ERROR, IMG_GetError());
		return 0;
	}
}

static int lIMG_Load_RW(lua_State *L)
{
	int l;
	const void *raw = luaL_checklstring(L, 1, &l);
	SDL_Surface *surface = IMG_Load_RW(SDL_RWFromConstMem(raw, l), 1);
	if(surface) {
		lSDL_newobject(L, surface, LSDL2_MT_SURFACE, NULL, lSDL_Surface_destroy);
		return 1;
	} else {
		luaL_error(L, LSDL2_FMT_ERROR, IMG_GetError());
		return 0;
	}
}

static int lSDL_Surface_tostring(lua_State *L)
{
	lua_pushstring(L, LSDL2_MT_SURFACE);
	return 1;
}

static luaL_Reg mt_surface[] = {
	{ "__tostring", lSDL_Surface_tostring },
	{ "__gc", lSDL_Surface_gc },
	{ "__index", lSDL_Surface_index },
	{ NULL, NULL }
};