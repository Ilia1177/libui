#include "libui.h"

ui_box_t* ui_belem_message(ui_win_t* win, const char* msg) 
{
	ui_box_t		*message;
	SDL_Texture* texture; 

	message = ui_box_create(win, 0, DEFAULT_BOX_COLOR);
	message->layout |= UI_LAYOUT_FIT_CONTENT;
	message->state |= BOX_DISABLE;
	// message->color = (SDL_Color) {255,255,255,0};
	message->color = DEFAULT_MESSAGE_COLOR;
	texture = ui_tex_str(message->parent_window, msg, DEFAULT_TEXT_COLOR);
	ui_layer_make(message, texture);
    	// ui_bhook_wincenter(message, NULL, &(SDL_Rect){0, 10, 0, 0});
		return message;
}
