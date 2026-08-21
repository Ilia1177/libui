#include "libui.h"
int ui_init() {
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	SDL_SetHint(SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0");
    SDL_SetHint("SDL_APP_ID", "com.yourname.guimp");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return -1;
    }
	if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init error: %s\n", TTF_GetError());
        return -1;
    }
	return 0;
}

