#include "ui_win.h"
int	ui_whook_render_default(ui_win_t* win, SDL_Event* e, void* data) 
{
	if (!win || !win->renderer || win->state & WIN_QUIT)
		return 1;

	// char *time = ui_get_time();
	SDL_Color color = win->background_color;
	SDL_SetRenderDrawColor(win->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(win->renderer);
	if (win->canvas && win->canvas->render) {
		ui_bhook_fire(win->canvas->render, win->canvas, e, data);
	}
	ui_box_t *current = win->boxes;
	while(current) {
		if (current->render) {
			ui_bhook_fire(current->render, current, e, data);
		}
		current = current->next;
	}
	win->state &= ~WIN_DIRTY;

	SDL_RenderPresent(win->renderer);
	return 1;
}

