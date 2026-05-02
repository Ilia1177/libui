#include "ui_box.h"

void ui_box_flags(ui_box_t *b, short flag, bool add) {
    while (b) {
        if (add)
            b->flags |= flag;
        else
            b->flags &= ~flag;
        b = b->next;
    }
}

int ui_box_count(ui_box_t *boxes) {
	int n = 0;
	while(boxes) {
		n++;
		boxes = boxes->next;
	}
	return n;
}

static void ui_box_menu_on_mouse_motion_handler(ui_box_t *box, SDL_Event* e, void* data) {

	(void)data;
	SDL_MouseMotionEvent *btn = &e->motion;
	// static int height = box->area.h;
	ui_win_t *win = box->parent_window;
    int px = (int)(btn->x * win->scale.x);
    int py = (int)(btn->y * win->scale.y);
    SDL_Point p = {px, py};
    if (SDL_PointInRect(&p, &box->area)) {
		// mouse inside
		if (!(box->flags & BOX_HOVERED)) {
			int boxnb = ui_box_count(box->child_boxes);
			box->area.h += boxnb * BOX_MENU_H;
		}
		box->flags |= BOX_HOVERED;
		ui_box_flags(box->child_boxes, BOX_HIDDEN, false);
	} else {
		box->area.h = BOX_MENU_H;
		// mouse outside
        box->flags &= ~BOX_PRESSED;  // release even if mouse moved off
		box->flags &= ~BOX_HOVERED;
		ui_box_flags(box->child_boxes, BOX_HIDDEN, true);
	}
    // convert global mouse to window-relative
	// ui_box_t *curr = box->child_boxes;
	// while(curr) {
	// 	if (curr->on_mouse_motion) 
	// 		ui_box_event_fire(curr->on_mouse_motion, curr, e);
	// 	curr = curr->next;
	// }
}

static void	ui_box_menu_on_click_up_handler(ui_box_t *b, SDL_Event* e, void* data)
{
	(void)data;
	(void)e;
    b->flags &= ~BOX_PRESSED;
	// ui_box_t *curr = b->child_boxes;
	// while(curr) {
	// 	if(curr->on_click_up)
	// 		ui_box_event_fire(curr->on_click_up, curr, e);
	// 	curr = curr->next;
	// }
}

static void	ui_box_menu_on_click_down_handler(ui_box_t *b, SDL_Event* e, void* data) {
	(void)data;
	SDL_MouseButtonEvent *btn = &e->button;
	ui_win_t *win = b->parent_window;
    int px = (int)(btn->x * win->scale.x);
    int py = (int)(btn->y * win->scale.y);
    SDL_Point p = {px, py};
    if (SDL_PointInRect(&p, &b->area) && (!b->child_boxes)) {
        b->flags |= BOX_PRESSED;
    } else {
        b->flags &= ~BOX_PRESSED;  // release even if mouse moved off
    }
	// // update children
	// ui_box_t *curr = b->child_boxes;
	// while(curr) {
	// 	if(curr->on_click_down)
	// 		ui_box_event_fire(curr->on_click_down, curr, e);
	// 	curr = curr->next;
	// }
}

void	ui_box_menu_update(ui_box_t* box, SDL_Event* e, void* data) {
	(void)e;
	(void)data;

	box->area.w = box->parent_window->area.w;
	// ui_box_t *current = box->child_boxes;
	// while(current) {
	// 	if (current->update)
	// 		ui_box_event_fire(current->update, curr, e);
	// 	current = current->next;
	// }
}

ui_box_t* ui_box_create_list(ui_win_t *win, SDL_Rect r, ui_rgba_t color, int n) {
    SDL_Rect area = {r.x, r.y, r.w, r.h};  // start below parent button
	
	ui_box_t *boxes = NULL;;
    for (int i = 0; i < n; i++) {
        area.y += r.h;  // shift DOWN for next submenu item, not right
        ui_box_t *box = ui_box_create(area, color, win);
        box->flags |= BOX_HIDDEN;
        ui_box_handler_add(&box->on_click_down, ui_box_menu_on_click_down_handler);
        ui_box_handler_add(&box->on_mouse_motion, ui_box_menu_on_mouse_motion_handler);
        ui_box_handler_add(&box->on_click_up, ui_box_menu_on_click_up_handler);
        ui_box_add(&boxes, box);
    }
	return boxes;
}

ui_box_t *ui_box_menu_list_create(char* label, ui_win_t *win, int subnb) {
		ui_globalApp_t *ref = win->global;
		SDL_Rect area = ref->button_area;
		ui_rgba_t color = ref->menu_color_2;
		ui_box_t* head_box;
		head_box = ui_box_create(area, color, win);
		head_box->border = 2;
		head_box->label = label;
		ui_box_handler_add(&head_box->on_click_down, ui_box_menu_on_click_down_handler);
		ui_box_handler_add(&head_box->on_mouse_motion, ui_box_menu_on_mouse_motion_handler);
		ui_box_handler_add(&head_box->on_click_up, ui_box_menu_on_click_up_handler);
		head_box->child_boxes = ui_box_create_list(win, area, color, subnb);
		ref->button_area.x += BOX_MENU_W + 10;
		ui_box_add(&win->menu->child_boxes, head_box);
		return head_box;
}

// create a menu list horizontal style
ui_box_t *ui_box_menu_create(ui_win_t *win, char** labels)
{ 
	(void)labels;
	ui_globalApp_t *app = win->global;
	// create main box of full width
	int height = BOX_MENU_H + 10;
	int width = win->area.w;
    ui_rgba_t color = app->menu_color_1;
    ui_box_t *box_menu = ui_box_create((SDL_Rect){0, 0, width, height}, color, win);
	ui_box_handler_add(&box_menu->update, ui_box_menu_update);
	box_menu->border = 2;
	
    return box_menu;
}

