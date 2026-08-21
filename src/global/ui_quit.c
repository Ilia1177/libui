#include "libui.h"
int ui_quit() {
    SDL_Quit();
	TTF_Quit();
	return 0;
}

