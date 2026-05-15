#include "ui_win.h"



int ui_whook_clickdown_default(ui_win_t *win, SDL_Event *e, void* data)
{
	if (!win || win->state & WIN_QUIT)
		return 1;
	ui_box_t *curr = win->boxes;
	while(curr) {
		if(curr->on_click_down)
			ui_bhook_fire(curr->on_click_down, curr, e, data);
		curr=curr->next;
	}
	ui_box_t *canvas = win->canvas;
	if(canvas && canvas->on_click_down){
		ui_bhook_fire(canvas->on_click_down, canvas, e, data);
	}
	return 1;
}
