#include "ui_box.h"

void ui_box_on_mouse_motion_handler(ui_box_t *box, SDL_Event* e) {
	(void)e;

	ui_box_t *curr = box->child_boxes;
	while(curr) {
		if (curr->on_mouse_motion) 
			curr->on_mouse_motion(curr, e);
		curr = curr->next;
	}
}

void	ui_box_on_click_down_handler(ui_box_t *b, SDL_Event* e) {
	// SDL_MouseButtonEvent *btn = &e->button;
	// printf("box: on click down\n");
	// fflush(stdout);
	// // update 
	// // if (ui_is_mouse_in(b)) {
	// // 	b->flags |= BOX_PRESSED;
	// // }
	// ui_win_t *win = b->parent_window;
	//    int px = (int)(btn->x * win->scale.x);
	//    int py = (int)(btn->y * win->scale.y);
	//    SDL_Point p = {px, py};
	//    if (SDL_PointInRect(&p, &b->area)) {
	//        b->flags |= BOX_PRESSED;
	// 	printf("box: pressed\n");
	// 	fflush(stdout);
	//    } else {
	//        b->flags &= ~BOX_PRESSED;  // release even if mouse moved off
	//    }
	// update children
	ui_box_t *curr = b->child_boxes;
	while(curr) {
		if(curr->on_click_down)
			curr->on_click_down(curr, e);
		curr = curr->next;
	}
}

void	ui_box_on_click_up_handler(ui_box_t *b, SDL_Event* e)
{
	SDL_MouseButtonEvent *btn = &e->button;
	ui_win_t *win = b->parent_window;
    int px = (int)(btn->x * win->scale.x);
    int py = (int)(btn->y * win->scale.y);
    SDL_Point p = {px, py};
	b->flags &= ~BOX_PRESSED;  // always release on mouse up

	(void)p;
	// update children
	ui_box_t *curr = b->child_boxes;
	while(curr) {
		if(curr->on_click_up)
			curr->on_click_up(curr, e);
		curr = curr->next;
	}
}

void ui_box_update_default(ui_box_t* box) {
	//update current ....
	// if(box->on_hover_enter) {
	// 	box->on_mouse_motion(box);
	// }
	//update childs
	printf("box: update\n");
	fflush(stdout);
	ui_box_t *current = box->child_boxes;
	while(current) {
		if (current->update)
			current->update(current);
		current = current->next;
	}
}


