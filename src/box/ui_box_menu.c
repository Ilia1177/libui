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

int ui_box_count_all(ui_box_t* head) {
	int n = 0;
	ui_box_t* curr = head;
	while(curr) {
		n++;
		n += ui_box_count_all(curr->list);
		curr = curr->next;
	}
	return n;
}

int ui_box_count(ui_box_t *boxes) {
	int n = 0;
	while(boxes) {
		n++;
		boxes = boxes->next;
	}
	return n;
}

ui_box_t* ui_box_last(ui_box_t* boxes) {
	ui_box_t *curr = boxes;
	ui_box_t* last = NULL;;
	while (curr) {
		last = curr;
		curr = curr->next;
	}
	return last;
}



// void	ui_box_menu_on_click_up_handler(ui_box_t *b, SDL_Event* e, void* data)
// {
// 	(void)data;
// 	(void)e;
// 	(void)b;
//     // b->flags &= ~BOX_PRESSED;
// }

// void	ui_box_menu_on_click_down_handler(ui_box_t *b, SDL_Event* e, void* data) {
// 	(void)data;
// 	(void)e;
// 	(void)b;
// }

void	ui_bhook_fullwidth(ui_box_t* box, SDL_Event* e, void* data) {
	(void)e;
	(void)data;
	box->area.w = box->parent_window->area.w;
	printf("nav bar: child: %d\n", ui_box_count(box->list));
	fflush(stdout);
}

// ui_box_t* ui_box_menu_option_add(ui_win_t *win, void(*fn)(ui_box_t*, SDL_Event*, void*)) {
// 	ui_globalApp_t *app = win->global;
// 	SDL_Rect *r = &app->button_area;
// 	SDL_Color color = app->menu_color_2;
//         r->y += r->h;  // shift DOWN for next submenu item
//         ui_box_t *option = ui_box_create(*r, color, win);
//         option->flags |= BOX_HIDDEN;
// 		if(fn)
// 			ui_box_handler_add(&option->update, fn);
//         // ui_box_handler_add(&box->on_click_down, ui_box_menu_on_click_down_handler);
//         // ui_box_handler_add(&box->on_mouse_motion, ui_box_menu_on_mouse_motion_handler);
//         // ui_box_handler_add(&box->on_click_up, ui_box_menu_on_click_up_handler);
//         ui_box_add(&win->menu->list, option);
// 	return option;
// }

ui_box_t *ui_elem_menulist(ui_box_t *menu, char* label) 
{
    ui_globalApp_t *ref = menu->parent_window->global;
    SDL_Rect area = ref->button_area;

    int n = ui_box_count(menu->list);
    area.x = MENU_OFFSET_X + (n * (BOX_MENU_W + MENU_GAP_X));

    // int nn = option_list ? ui_box_count(option_list->list) : 0;
    area.y = MENU_GAP_Y;

    SDL_Color color = ref->menu_color_2;
    ui_box_t *new_option_list = ui_box_create(area, color, menu->parent_window);
    new_option_list->border = 2;
	new_option_list->label = label;
    ui_bhook_add(&new_option_list->update, ui_bhook_unhidechild);
	ui_bhook_add(&new_option_list->update, ui_bhook_label2texture);
    ui_box_add(&menu->list, new_option_list);

    return new_option_list;
}

ui_box_t* ui_elem_menubutton(ui_box_t *options, void(*fn)(ui_box_t*, SDL_Event*, void*))
{	
	// int position_x = options->area.x;
	int position_y = ui_box_count(options->list) + 1;

	ui_box_t* option = ui_elem_button(options->parent_window);
    option->area.x = options->area.x;
    option->area.y = MENU_GAP_Y + position_y * BOX_MENU_H;
    option->flags |= BOX_HIDDEN;
	if(fn)
		ui_bhook_add(&option->update, fn);
    ui_box_add(&options->list, option);
	return option;
}


// create a menu list horizontal style
ui_box_t *ui_elem_menubar(ui_win_t *win, char** labels)
{
	(void)labels;
	ui_globalApp_t *app = win->global;
	// create main box of full width
	int height = BOX_MENU_H + MENU_GAP_Y * 2;
	int width = win->area.w;
    SDL_Color color = app->menu_color_1;
    ui_box_t *nav_bar = ui_box_create((SDL_Rect){0, 0, width, height}, color, win);
	ui_bhook_add(&nav_bar->update, ui_bhook_fullwidth);
	ui_bhook_add(&nav_bar->update, ui_bhook_nohovered);
	ui_bhook_add(&nav_bar->update, ui_bhook_nopressed);
	nav_bar->border = 2;
	
	ui_box_add(&win->menu, nav_bar);
    return nav_bar;
}

