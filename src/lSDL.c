#include <SDL.h>
#include <SDL_image.h>

#define LUA_LIB
#include <lua.h>
#include <lauxlib.h>

#define LSDL2_FMT_ERROR "SDL: %s"
#define LSDL2_FMT_TTF "TTF: %s"
#define LSDL2_MT_WINDOW "SDL_Window"
#define LSDL2_MT_RENDERER "SDL_Renderer"
#define LSDL2_MT_TEXTURE "SDL_Texture"
#define LSDL2_MT_SURFACE "SDL_Surface"
#define LSDL2_MT_FONT "TTF_Font"

typedef void (*lSDL_Destroy)(void *ptr);
typedef struct { void *ptr; int count; void *owner, **userdata; lSDL_Destroy destroy; } lSDL_Object;

static void *lSDL_checkobject(lua_State *L, int idx, const char *mt) {
	void **ptr = luaL_checkudata(L, idx, mt);
	lSDL_Object *obj;
	if(!*ptr) luaL_error(L, "destroyed object detected");
	obj = *ptr;
	return obj->ptr;
}

static void *lSDL_checkref(lua_State *L, int idx, const char *mt) {
	void **ptr = luaL_checkudata(L, idx, mt);
	if(!*ptr) luaL_error(L, "destroyed object detected");
	return *ptr;
}

static void lSDL_newobject(lua_State *L, void *data, const char *mt, lSDL_Object *owner, lSDL_Destroy destroy)
{
	lSDL_Object **ptr = lua_newuserdata(L, sizeof(void *));
	lSDL_Object *obj = malloc(sizeof(lSDL_Object));
	obj->ptr = data;
	obj->count = 1;
	obj->owner = owner;
	if(owner) owner->count++;
	obj->userdata = ptr;
	obj->destroy = destroy;
	*ptr = obj;
	luaL_getmetatable(L, mt);
	lua_setmetatable(L, -2);
}

static void lSDL_unrefobject(lua_State *L, int idx, const char *mt)
{
	lSDL_Object **ptr = luaL_checkudata(L, idx, mt);
	register lSDL_Object *obj = *ptr, *last;
	while(obj) {
		if(--obj->count == 0) {
			obj->destroy(obj->ptr);
			*(obj->userdata) = NULL;
			last = obj, obj = obj->owner;
			free(last);
		} else break;
	}
}

static SDL_Color luaZ_checkSDLcolor(lua_State *L, int idx)
{
	int ol;
	SDL_Color color;
	if(!lua_istable(L, idx))
		luaL_error(L, "you must pass a table as the color");
	ol = lua_objlen(L, idx);
	if(ol == 4) {
		lua_rawgeti(L, idx, 1);
		color.r = lua_tointeger(L, -1);
		lua_rawgeti(L, idx, 2);
		color.g = lua_tointeger(L, -1);
		lua_rawgeti(L, idx, 3);
		color.b = lua_tointeger(L, -1);
		lua_rawgeti(L, idx, 4);
		color.a = lua_tointeger(L, -1);
		lua_pop(L, 4);
	} else if(ol == 3) {
		lua_rawgeti(L, idx, 1);
		color.r = lua_tointeger(L, -1);
		lua_rawgeti(L, idx, 2);
		color.g = lua_tointeger(L, -1);
		lua_rawgeti(L, idx, 3);
		color.b = lua_tointeger(L, -1);
		color.a = 0xFF;
		lua_pop(L, 3);
	} else {
		luaL_error(L, "are you sure this is a color?");
	}
	return color;
}

#include "lSDL_renderer.c"
#include "lSDL_window.c"
#include "lSDL_surface.c"
#include "lSDL_ttf.c"

static int lSDL_GetTicks(lua_State *L)
{
	lua_pushinteger(L, SDL_GetTicks());
	return 1;
}

static int lSDL_Delay(lua_State *L)
{
	SDL_Delay(luaL_checkinteger(L, 1));
	return 0;
}

static int lSDL_ShowCursor(lua_State *L)
{
	lua_pushinteger(L, SDL_ShowCursor(luaL_checkinteger(L, 1)));
	return 1;
}

static int lSDL_PollEvent(lua_State *L)
{
	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		lua_newtable(L);
		switch(event.type) {
			case SDL_QUIT:
				lua_pushlstring(L, "quit", 4);
				lua_setfield(L, -2, "type");
				break;
		}
		return 1;
	}
	else return 0;
}

static luaL_Reg funcs[] = {
	{ "BlitSurface", lSDL_BlitSurface },
	{ "CreateWindow", lSDL_CreateWindow },
	{ "CreateRenderer", lSDL_CreateRenderer },
	{ "CreateRGBSurface", lSDL_CreateRGBSurface },
	{ "CreateSoftwareRenderer", lSDL_CreateSoftwareRenderer },
	{ "CreateTextureFromSurface", lSDL_CreateTextureFromSurface },
	{ "Delay", lSDL_Delay },
	{ "FillRect", lSDL_FillRect },
	{ "GetTicks", lSDL_GetTicks },
	{ "HideWindow", lSDL_HideWindow },
	{ "MinimizeWindow", lSDL_MinimizeWindow},
	{ "LoadBMP", lSDL_LoadBMP },
	{ "PollEvent", lSDL_PollEvent },
	{ "RenderClear", lSDL_RenderClear },
	{ "RenderCopy", lSDL_RenderCopy },
	{ "RenderPresent", lSDL_RenderPresent },
	{ "SetWindowTitle", lSDL_SetWindowTitle },
	{ "SetRenderDrawColor", lSDL_SetRenderDrawColor },
	{ "SetSurfaceAlphaMod", lSDL_SetSurfaceAlphaMod },
	{ "SetSurfaceBlendMode", lSDL_SetSurfaceBlendMode },
	{ "ShowCursor", lSDL_ShowCursor },
	{ "ShowWindow", lSDL_ShowWindow },
	{ "IMG_Load", lIMG_Load },
	{ "IMG_Load_RW", lIMG_Load_RW },
	{ "TTF_OpenFont", lTTF_OpenFont },
	{ "TTF_OpenFontIndex", lTTF_OpenFontIndex },
	{ "TTF_RenderShaded", lTTF_RenderShaded },
	{ "TTF_RenderBlended", lTTF_RenderBlended },
	{ "TTF_RenderSolid", lTTF_RenderSolid },
	{ "TTF_RenderWrapped", lTTF_RenderWrapped },
	{ "TTF_Size", lTTF_Size },
	{ NULL, NULL }
};

LUA_API int luaopen_lSDL2(lua_State *L)
{
	if (0 != SDL_Init(SDL_INIT_EVERYTHING))
		luaL_error(L, "can't initialize SDL");
	if (-1 == TTF_Init())
		luaL_error(L, "can't initialize TTF");
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	
	luaL_newmetatable(L, LSDL2_MT_WINDOW);
	luaL_register(L, NULL, mt_window);
	lua_pop(L, 1);
	
	luaL_newmetatable(L, LSDL2_MT_RENDERER);
	luaL_register(L, NULL, mt_renderer);
	lua_pop(L, 1);
	
	luaL_newmetatable(L, LSDL2_MT_TEXTURE);
	luaL_register(L, NULL, mt_texture);
	lua_pop(L, 1);
	
	luaL_newmetatable(L, LSDL2_MT_SURFACE);
	luaL_register(L, NULL, mt_surface);
	lua_pop(L, 1);
	
	luaL_newmetatable(L, LSDL2_MT_FONT);
	luaL_register(L, NULL, mt_font);
	lua_pop(L, 1);
	
	lua_newtable(L);
#define SET_SDL_CONST(x) lua_pushinteger(L, SDL_##x), lua_setfield(L, -2, #x)
	SET_SDL_CONST(WINDOWPOS_UNDEFINED);
	SET_SDL_CONST(WINDOW_FULLSCREEN);
	SET_SDL_CONST(WINDOW_FULLSCREEN_DESKTOP);
	SET_SDL_CONST(WINDOW_OPENGL);
	SET_SDL_CONST(WINDOW_SHOWN);
	SET_SDL_CONST(WINDOW_HIDDEN);
	SET_SDL_CONST(WINDOW_BORDERLESS);
	SET_SDL_CONST(WINDOW_RESIZABLE);
	SET_SDL_CONST(WINDOW_MINIMIZED);
	SET_SDL_CONST(WINDOW_MAXIMIZED);
	SET_SDL_CONST(WINDOW_INPUT_GRABBED);
	SET_SDL_CONST(WINDOW_INPUT_FOCUS);
	SET_SDL_CONST(WINDOW_MOUSE_FOCUS);
	SET_SDL_CONST(WINDOW_ALLOW_HIGHDPI);
	
	SET_SDL_CONST(RENDERER_SOFTWARE);
	SET_SDL_CONST(RENDERER_ACCELERATED);
	SET_SDL_CONST(RENDERER_PRESENTVSYNC);
	SET_SDL_CONST(RENDERER_TARGETTEXTURE);
	
	SET_SDL_CONST(BLENDMODE_NONE);
	SET_SDL_CONST(BLENDMODE_BLEND);
	SET_SDL_CONST(BLENDMODE_ADD);
	SET_SDL_CONST(BLENDMODE_MOD);
#undef SET_SDL_CONST
	luaL_register(L, NULL, funcs);
	
	return 1;
}