#include "ui_win.h"
int ui_whook_windowevent_default(ui_win_t* win, SDL_Event*e, void* data)
{
	if(e->window.event == SDL_WINDOWEVENT_CLOSE) {
		win->state |= WIN_QUIT;
	} else if (e->window.event == SDL_WINDOWEVENT_RESIZED && win) {
		SDL_GL_GetDrawableSize(win->ptr, &win->area.w, &win->area.h);
		ui_win_get_scale(win);
		ui_whook_fire(&win->update, win, e, (void*)win);
	} else if(e->window.event == SDL_WINDOWEVENT_MOVED && win) {
		SDL_GetWindowPosition(win->ptr, &win->area.x, &win->area.y);
	} else if (e->window.event == SDL_WINDOWEVENT_CLOSE) {
		reset_state_and_input(win->global, NULL);
	}
	ui_box_t *current = win->boxes;
	while (current) {
		if (current->on_window_event) {
			ui_bhook_fire(current->on_window_event, current, e, data);
		}
		current = current->next;
	}
	ui_box_t *canvas = win->canvas;
	if(canvas && canvas->on_window_event){
		ui_bhook_fire(canvas->on_window_event, canvas, e, data);
	}
	return 1;
}

