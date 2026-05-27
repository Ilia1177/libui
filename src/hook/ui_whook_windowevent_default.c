#include "ui_win.h"

int ui_whook_windowevent_default(ui_win_t* win, SDL_Event*e, void* data)
{
	(void)data;
	if(e->window.event == SDL_WINDOWEVENT_CLOSE) {
		win->state |= WIN_QUIT;
		reset_state_and_input(win->global, NULL);
		ui_log("-> WIN QUIT");
	} else if (e->window.event == SDL_WINDOWEVENT_RESIZED && win) {
		SDL_GL_GetDrawableSize(win->ptr, &win->area.w, &win->area.h);
		win->state |= WIN_DIRTY;
	} else if(e->window.event == SDL_WINDOWEVENT_MOVED && win) {
		SDL_GetWindowPosition(win->ptr, &win->area.x, &win->area.y);
	}
	return 1;
}
