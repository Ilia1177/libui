#include "libui.h"
int ui_init() {
    return SDL_Init(SDL_INIT_VIDEO);
}

