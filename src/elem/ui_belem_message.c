#include "libui.h"

ui_box_t* ui_belem_message(ui_win_t* win, const char* msg) 
{
		ui_box_t		*message;
		SDL_Texture* texture; 

		SDL_Rect area;

		area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
		message = ui_box_create(win, area, win->colors[2]);
		message->flags |= BOX_DISABLE;
		message->color = (SDL_Color) {255,255,255,0};
		texture = ui_tex_str(message->parent_window, msg, COLOR_WHITE);
		ui_layer_make(message, texture);
    	ui_bhook_wincenter(message, NULL, &(SDL_Rect){0, 10, 0, 0});
		// ui_bhook_remove(&message->on_click_down, ui_bhook_clickdown_default);
		// ui_bhook_remove(&message->on_mouse_motion, ui_bhook_mousemotion_default);
		return message;
}
