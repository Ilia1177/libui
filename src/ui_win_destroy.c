#include "ui_win.h"

void	ui_win_destroy_default(ui_win_t* win)
{
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->win);
	free(win);
}
