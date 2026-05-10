#include "ui_win.h"
void	ui_winpos(ui_win_t* win, int x, int y)
{
    SDL_SetWindowPosition(win->ptr, x, y);
}

