#include "ui_win.h"

void	ui_win_destroy_default(ui_win_t* win, SDL_Event*e, void*data)
{
	ui_box_t *curr = win->menu;
	while(curr) {
		ui_box_t *next = curr->next;
		ui_box_event_fire(curr->destroy, curr, e, data);
		curr = next;
	}
    // free handler lists
    ui_win_handler_clean(&win->render);
    ui_win_handler_clean(&win->update);
    ui_win_handler_clean(&win->destroy);
    ui_win_handler_clean(&win->on_click_down);
    ui_win_handler_clean(&win->on_click_up);
    ui_win_handler_clean(&win->on_mouse_motion);
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->win);
	free(win);
}
