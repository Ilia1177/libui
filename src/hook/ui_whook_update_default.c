#include "ui_win.h"
int ui_whook_update_default(ui_win_t* win, SDL_Event*e, void* data)
{
	if (win->state & WIN_QUIT)
		return 1;

	ui_box_t *current = win->boxes;
	while (current) {
		if (current->update) {
			ui_bhook_fire(current->update, current, e, data);
		}
		current = current->next;
	}
	ui_box_t *canvas = win->canvas;
	if(canvas && canvas->update){
		ui_bhook_fire(canvas->update, canvas, e, data);
	}
	return 1;
}

