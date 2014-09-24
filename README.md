lua-sdl2
========

Bare SDL binding for LuaJIT &amp; Lua 5.2

Building
========

Build this library as a shared library called lSDL2 with your favourite compiler. Just compile the lSDL.c with:

```
MSVC: /LD /FelSDL2.dll /DLUA_BUILD_AS_DLL
gcc on Linux: --shared -o lSDL2.so
```

Usage
========

First require this library to global as SDL:
```
SDL = require "lSDL2"
```

The SDL APIs are maped to table SDL in the following format:
```
SDL_AAAAA_BBBBB => SDL.AAAAA_BBBBB
SDL_AaaaaBbbbb => SDL.AaaaaBbbbb
SDL_Color c = {r, g, b, a} => {r, g, b[, a]}
IMG_AaaaaBbbbb => SDL.IMG_AaaaaBbbbb
TTF_AaaaaBbbbb => SDL.TTF_AaaaaBbbbb
```

The SDL_Rect arguments are expanded to arguments.

Example
========
```
SDL = require "lSDL2"
Window = SDL.CreateWindow("Example Window", 1000, 567, 0)
Renderer = SDL.CreateRenderer(Window, SDL.RENDERER_ACCELERATED) -- The second argument is -1 and can't be changed.
Image = SDL.IMG_Load("image.bmp")
Texture = SDL.CreateTextureFromSurface(Renderer, Image)
SDL.RenderCopy(Renderer, Texture, 0, 0) -- If we don't want to scale or clip, only x, y are required.
SDL.RenderPresent(Renderer)
SDL.Delay(5000)
```

If You Want to Contribute
========

Commiting new API exports is just enough. If you found bugs, please report them as issues.
