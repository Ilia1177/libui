#include "ui_win.h"

void	ui_whook_destroy_default(ui_win_t* win, SDL_Event*e, void*data)
{
	printf("freeing window %d\n", win->id);
	ui_box_t *curr = win->menu;
	ui_box_t *next = NULL;
	while(curr) {
		next = curr->next;
		ui_bhook_fire(curr->destroy, curr, e, data);
		curr = next;
	}
	curr = win->canvas;
	while(curr) {
		next = curr->next;
		ui_bhook_fire(curr->destroy, curr, e, data);
		curr = next;
	}
    // free handler lists
    ui_whook_clean(&win->render);
    ui_whook_clean(&win->update);
    ui_whook_clean(&win->destroy);
    ui_whook_clean(&win->on_click_down);
    ui_whook_clean(&win->on_click_up);
    ui_whook_clean(&win->on_mouse_motion);
	TTF_CloseFont(win->font);
	win->font = NULL;
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->win);
	free(win);
}
