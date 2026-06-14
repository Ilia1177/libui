#include "libui.h"

ui_box_t*	ui_belem_button(ui_win_t* win, SDL_Texture* texture)
{
		ui_box_t*	btn;
		SDL_Rect	area;

		btn = NULL;
		area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
		btn = ui_box_create(win, area, win->colors[1]);
		btn->flags |= BOX_CLICKABLE;
		ui_layer_make(btn, texture);
		return btn;
}
