#include "ui_win.h"

int ui_whook_clickup_default(ui_win_t *win, SDL_Event* e, void *data)
{
	if (!win || win->state & WIN_QUIT)
		return 1;

	ui_box_t *curr = win->boxes;
	while(curr) {
		if (curr->on_click_up) {
			ui_bhook_fire(curr->on_click_up, curr, e, data);
		}
		curr = curr->next;
	}
	ui_box_t *canvas = win->canvas;
	if(canvas && canvas->on_click_up){
		printf("update canvas\n");
		fflush(stdout);
		ui_bhook_fire(canvas->on_click_up, canvas, e, data);
	}
	return 1;

}

