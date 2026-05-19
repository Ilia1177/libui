#ifndef UI_FORWARD_H
#define UI_FORWARD_H

#include "libui.h"
#include "ui_win.h"
#include "ui_box.h"

typedef enum {
	UI_FORWARD_CLICK_DOWN,
	UI_FORWARD_CLICK_UP,
	UI_FORWARD_MOUSE_MOTION,
	UI_FORWARD_MOUSE_WHEEL,
	UI_FORWARD_KEY_DOWN,
	UI_FORWARD_WINDOW_EVENT,
	UI_FORWARD_UPDATE,
	UI_FORWARD_RENDER,
} ui_forward_type_t;

void ui_forward_to_boxes(ui_win_t *win, SDL_Event *e, void *data, ui_forward_type_t type);

#endif
