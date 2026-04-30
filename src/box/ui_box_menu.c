#include "libui.h"

static void	ui_box_menu_update(ui_box_t* box) {
	box->area.w = box->parent_window->area.w;
	ui_box_t *current = box->child_boxes;
	while(current) {
		if (current->update)
			current->update(current);
		current = current->next;
	}
}
// create a menu list horizontal style
ui_box_t *ui_box_menu_create(ui_win_t *win, char** list)
{ 
	int height = 50;
	int width = win->area.w;
    ui_rgba_t color = {124, 56, 210, 255};
	// create main box of full width
    ui_box_t *box_menu = ui_box_create((SDL_Rect){0, 0, width, height}, color, win);
	box_menu->update = ui_box_menu_update;
	box_menu->on_click_up = ui_box_on_click_up_handler;
	box_menu->on_click_up = ui_box_on_click_down_handler;
	box_menu->on_mouse_motion = ui_box_on_mouse_motion_handler;
	box_menu->border = 2;
    color = (ui_rgba_t){138, 46, 2, 255};
	
	if (!list) return NULL;
	char *label = *list;
	SDL_Rect button_area = (SDL_Rect){50, 4, 100, height - 8};
	while(label) {
		ui_box_t *b	= ui_box_create(button_area, color, win);
		b->border = 2;
		b->on_mouse_motion = ui_box_on_mouse_motion_handler;
		b->on_click_up = ui_box_on_click_up_handler;
		b->on_click_down = ui_box_on_click_down_handler;
		b->update = ui_box_update_default;
		b->label = label;
		button_area.x += 100 + 10;
    	ui_box_add(&box_menu->child_boxes, b);
		label = *(++list);
	}
	ui_box_add(&win->boxes, box_menu);

    return box_menu;
}

