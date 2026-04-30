#include "ui_win.h"

// Event registration functions
void ui_win_set_keydown(ui_win_t* win, void(*handler)(ui_win_t*, SDL_KeyboardEvent*)) {
    win->on_key_down = handler;
}

void ui_win_set_keyup(ui_win_t* win, void(*handler)(ui_win_t*, SDL_KeyboardEvent*)) {
    win->on_key_up = handler;
}

void ui_win_set_mousedown(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseButtonEvent*)) {
    win->on_mouse_down = handler;
}

void ui_win_set_mouseup(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseButtonEvent*)) {
    win->on_click_up = handler;
}

void ui_win_set_mousemotion(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseMotionEvent*)) {
    win->on_mouse_motion = handler;
}

void ui_win_set_mousewheel(ui_win_t* win, void(*handler)(ui_win_t*, SDL_MouseWheelEvent*)) {
    win->on_mouse_wheel = handler;
}

void ui_win_set_resize(ui_win_t* win, void(*handler)(ui_win_t*, SDL_WindowEvent*)) {
    win->on_windows_event = handler;
}

void ui_win_on_resize_default(ui_win_t *win, SDL_WindowEvent* e) {
	(void)win;
	(void)e;
	// win->w = e->data1;
    // win->h = e->data2;
    // ui_win_get_scale(win);              // refresh scale first
    // win->w = (int)(win->w * win->scale.x);  // then store physical size
    // win->h = (int)(win->h * win->scale.y);
}

void ui_win_on_click_up_handler(ui_win_t *win, SDL_MouseButtonEvent* e) {
	printf("windows: on click down\n");
	// update
	//  ... (nothing) ...
	//  give event to children
	fflush(stdout);
	ui_box_t *curr = win->boxes;
	while(curr) {
		if (curr->on_click_up) {
			curr->on_click_up(curr, e);
		}
		curr = curr->next;
	}

}
void ui_win_on_mouse_motion_handler(ui_win_t *win, SDL_MouseMotionEvent* e)
{
	printf("windows: on mouse motion\n");
	fflush(stdout);
	ui_box_t *curr = win->boxes;
	while(curr) {
		if (curr->on_mouse_motion) {
			curr->on_mouse_motion(curr, e);
		}
		curr = curr->next;
	}
}

// void ui_win_event(ui_win_t *win) {
//
//
//
// 	if (win->on_key_down) {
// 		win->on_key_down(win);
// 	}
// 	if (win->on_key_up) {
// 		win->on_key_up(win);
// 	}
// 	if (win->on_mouse_down) {
// 		win->on_mouse_down(win);
//
// 	}
// }
