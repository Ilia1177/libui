#include "ui_win.h"
int ui_whook_windowevent_default(ui_win_t* win, SDL_Event*e, void* data)
{
	// printf("Window event default\n");
	// fflush(stdout);
	if(e->window.event == SDL_WINDOWEVENT_CLOSE) {
		win->state |= WIN_QUIT;
		reset_state_and_input(win->global, NULL);
	} else if (e->window.event == SDL_WINDOWEVENT_RESIZED && win) {
		SDL_GL_GetDrawableSize(win->ptr, &win->area.w, &win->area.h);
		ui_win_get_scale(win);
		ui_whook_fire(&win->update, win, e, (void*)win);
	} else if(e->window.event == SDL_WINDOWEVENT_MOVED && win) {
		SDL_GetWindowPosition(win->ptr, &win->area.x, &win->area.y);
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

