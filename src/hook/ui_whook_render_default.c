#include "ui_win.h"
#include "ui_forward.h"

int	ui_whook_render_default(ui_win_t* win, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	if (!win || !win->renderer || win->state & WIN_QUIT)
		return 1;

	ui_box_flags(win->boxes, BOX_CLICKED, false, true);
	ui_box_flags(win->canvas, BOX_CLICKED, false, true);
	// SDL_Color color = win->background_color;
	SDL_SetRenderDrawColor(win->renderer, 122, 9, 34, 128);
	SDL_RenderClear(win->renderer);
	if (win->canvas && win->canvas->render) {
		ui_bhook_fire(win->canvas->render, win->canvas, e, data);
	}
	ui_box_render_forward(win, e, NULL);
	SDL_RenderPresent(win->renderer);
	win->state &= ~WIN_DIRTY;
	return 1;
}
