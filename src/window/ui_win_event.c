#include "ui_win.h"

void ui_win_handler_add(ui_winhandler_t **list, void (*fn)(ui_win_t*, SDL_Event*, void*)) {
    if (!fn) return;
    ui_winhandler_t *handler = calloc(1, sizeof(ui_winhandler_t));
    handler->fn = fn;
    handler->next = *list;  // prepend
    *list = handler;
}

void ui_win_on_click_down_handler(ui_win_t *win, SDL_Event *e, void* data) {
	(void)data;
	ui_box_t *curr = win->menu;
	while(curr) {
		if(curr->on_click_down)
			ui_box_event_fire(curr->on_click_down, curr, e, data);
		curr=curr->next;
	}
}

void ui_win_on_click_up_handler(ui_win_t *win, SDL_Event* e, void *data) {
	ui_box_t *curr = win->menu;
	while(curr) {
		if (curr->on_click_up) {
			ui_box_event_fire(curr->on_click_up, curr, e, data);
		}
		curr = curr->next;
	}

}

void ui_win_on_mouse_motion_handler(ui_win_t *win, SDL_Event* e, void* data)
{
	(void)data;
	// SDL_MouseMotionEvent *btn = &e->button; 
	// (void)btn;
	ui_box_t *curr = win->menu;
	while(curr) {
		if (curr->on_mouse_motion) {
			ui_box_event_fire(curr->on_mouse_motion, curr, e, data);
		}
		curr = curr->next;
	}
}

void ui_win_event_fire(ui_winhandler_t *list, ui_win_t *win, SDL_Event *e, void* data) {
    while (list) {
        if (list->fn)
            list->fn(win, e, data);
        list = list->next;
    }
}
void ui_win_handler_clean(ui_winhandler_t **list) {
	if (!list || !*list) return;
	ui_winhandler_t *curr = *list;
	while(curr) {
		ui_winhandler_t *next = curr->next;
		free(curr);
		curr = next;
	}

    *list = NULL;
}

// void ui_win_on_resize_default(ui_win_t *win, SDL_Event* e) {
// 	(void)win;
// 	(void)e;
// 	// win->w = e->data1;
//     // win->h = e->data2;
//     // ui_win_get_scale(win);              // refresh scale first
//     // win->w = (int)(win->w * win->scale.x);  // then store physical size
//     // win->h = (int)(win->h * win->scale.y);
// }

