#include "ui_win.h"

int ui_whook_mousewheel_default(ui_win_t *win, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	if (win->state & WIN_QUIT)
		return 1;
	return 1;
}
