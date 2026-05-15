#include "ui_win.h"
int ui_whook_mousemotion_default(ui_win_t *win, SDL_Event* e, void* data)
{
	if (win->state & WIN_QUIT)
		return 1;

	(void)data;
	ui_box_t *curr = win->boxes;
	while(curr) {
		if (curr->on_mouse_motion) {
			ui_bhook_fire(curr->on_mouse_motion, curr, e, data);
		}
		curr = curr->next;
	}
	ui_box_t *canvas = win->canvas;
	if(canvas && canvas->on_mouse_motion){
		ui_bhook_fire(canvas->on_mouse_motion, canvas, e, data);
	}
	return 1;
}

