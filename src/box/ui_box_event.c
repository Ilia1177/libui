#include "ui_box.h"

void ui_box_on_mouse_motion_handler(ui_box_t *box, SDL_Event* e, void* data) {
	ui_box_t *curr = box->child_boxes;
	while(curr) {
		if (curr->on_mouse_motion) 
			ui_box_event_fire(curr->on_mouse_motion, curr, e, data);
		curr = curr->next;
	}
}

void ui_box_handler_add(ui_boxhandler_t **list, void (*fn)(ui_box_t*, SDL_Event*, void*)) {
    if (!fn) return;
    ui_boxhandler_t *handler = calloc(1, sizeof(ui_boxhandler_t));
    handler->fn = fn;
    handler->next = *list;  // prepend
    *list = handler;
	printf("handler added\n");
	fflush(stdout);
}

void ui_box_handler_clean(ui_boxhandler_t **list) {
	if (!list || !*list) return;
	ui_boxhandler_t *curr = *list;
	while(curr) {
		ui_boxhandler_t *next = curr->next;
		free(curr);
		curr = next;
	}

    *list = NULL;
}

void	ui_box_on_click_down_handler(ui_box_t *b, SDL_Event* e, void* data) {
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
            ui_box_event_fire(curr->on_click_down, curr, e, data);
		curr = curr->next;
	}
}

void	ui_box_on_click_up_handler(ui_box_t *b, SDL_Event* e, void* data)
{
	// SDL_MouseButtonEvent *btn = &e->button;
	// ui_win_t *win = b->parent_window;
	//    int px = (int)(btn->x * win->scale.x);
	//    int py = (int)(btn->y * win->scale.y);
	//    SDL_Point p = {px, py};
	b->flags &= ~BOX_PRESSED;  // always release on mouse up

	// (void)p;
	// update children
	ui_box_t *curr = b->child_boxes;
	while(curr) {
		if(curr->on_click_up)
			ui_box_event_fire(curr->on_click_up, curr, e, data);
		curr = curr->next;
	}
}

void ui_box_update_default(ui_box_t* box, SDL_Event*e, void* data) {
	//update current ....
	// if(box->on_hover_enter) {
	// 	box->on_mouse_motion(box);
	// }
	//update childs
	ui_box_t *current = box->child_boxes;
	while(current) {
		if (current->update)
			ui_box_event_fire(current->update, current, e, data);
		current = current->next;
	}
}

void ui_box_event_fire(ui_boxhandler_t *list, ui_box_t *box, SDL_Event *e, void* data) {
    while (list) {
		ui_boxhandler_t *next = list->next;  // save before fn call
        if (list->fn)
            list->fn(box, e, data);
        list = next;
    }
}

