#include "ui_win.h"

void ui_win_on_click_down_handler(ui_win_t *win, SDL_Event *e) {
	ui_box_t *curr = win->menu;
	while(curr) {
		if(curr->on_click_down)
			curr->on_click_down(curr, e);
		curr=curr->next;
	}
}

void ui_win_on_click_up_handler(ui_win_t *win, SDL_Event* e) {
	// SDL_MouseButtonEvent *btn = &e->button;  // cast here
	// (void)btn;
	printf("windows: on click down\n");
	// update
	//  ... (nothing) ...
	//  give event to children
	fflush(stdout);
	ui_box_t *curr = win->menu;
	while(curr) {
		if (curr->on_click_up) {
			curr->on_click_up(curr, e);
		}
		curr = curr->next;
	}

}
void ui_win_on_mouse_motion_handler(ui_win_t *win, SDL_Event* e)
{
	// SDL_MouseMotionEvent *btn = &e->button; 
	// (void)btn;
	ui_box_t *curr = win->menu;
	while(curr) {
		if (curr->on_mouse_motion) {
			curr->on_mouse_motion(curr, e);
		}
		curr = curr->next;
	}
}




void ui_win_on_resize_default(ui_win_t *win, SDL_Event* e) {
	(void)win;
	(void)e;
	// win->w = e->data1;
    // win->h = e->data2;
    // ui_win_get_scale(win);              // refresh scale first
    // win->w = (int)(win->w * win->scale.x);  // then store physical size
    // win->h = (int)(win->h * win->scale.y);
}

