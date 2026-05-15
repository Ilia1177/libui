#include "ui_win.h"
#include "ui_box.h"
#include "libft.h"

ui_box_t *ui_belem_menu_list(ui_box_t *menu, const char* label) 
{
	ui_box_t *new_list;
    int list_nb;

	list_nb = ui_box_count_next(menu->childs);
    new_list = ui_belem_button(menu->parent_window, label);
	if (menu->type == UI_HORIZONTAL_MENU) {
    	new_list->area.x = MENU_OFFSET_X + (list_nb * (BOX_MENU_W + MENU_GAP_X));
    	new_list->area.y = MENU_GAP_Y;
	} else if (menu->type == UI_VERTICAL_MENU) {
		new_list->area.x = MENU_GAP_X;
	   	new_list->area.y = MENU_GAP_Y + (list_nb * (BOX_MENU_H + MENU_GAP_Y));
	} else {
		return menu;
	}
	new_list->layers->area = ui_area_center(new_list->area, new_list->layers->area);
	new_list->type = menu->type;
    new_list->border = 2;
	new_list->label = label;
    ui_bhook_add(&new_list->update, ui_bhook_revealchild);
    ui_box_add_child(menu, new_list);
    return new_list;
}



ui_box_t* ui_belem_menu_item(ui_box_t *list, const char* label, ui_bhook_fn_t fn)
{
	int btn_nb = ui_box_count_next(list->childs);
	// SDL_Rect pos = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
	ui_bhook_fn_t hook;

	hook = NULL;
	ui_box_t* btn = ui_belem_button(list->parent_window, label);
	switch(list->type) {
		case UI_HORIZONTAL_MENU:
    		btn->area.x = list->area.x;
			btn->area.y = MENU_GAP_Y + (1 + btn_nb) * BOX_MENU_H;
			break;
		case UI_VERTICAL_MENU:
			btn->area.x = BOX_MENU_W + MENU_GAP_X;
			btn->area.y = list->area.y + btn_nb * BOX_MENU_H;
			break;
		case UI_FULLWINDOW_MENU: case UI_NONE: 
			btn->area.w = (list->area.w - 3 * MENU_GAP_X) / 2;
			btn->area.x = MENU_GAP_X + (btn_nb % 2) * (btn->area.w + MENU_GAP_X);
			btn->area.y = MENU_GAP_Y + (btn_nb / 2) * (BOX_MENU_H + MENU_GAP_Y);
			hook = ui_bhook_fullwindow_button;
			break;
	}
	btn->flags |= BOX_HIDDEN;
	btn->layers->area = ui_area_center(btn->area, btn->layers->area);
	ui_bhook_add(&btn->on_window_event, hook);
	ui_bhook_add(&btn->update, ui_bhook_nofocused);
	ui_bhook_add(&btn->update, fn);
    ui_box_add_child(list, btn);
	return btn;
}

SDL_Rect ui_area(int x, int y, int w, int h)
{
	return (SDL_Rect) {x, y, w, h};
}

// create a menu list horizontal style
ui_box_t *ui_belem_menu_make(ui_win_t *win, boxtype_e type)
{
	SDL_Rect area;
	ui_box_t *navbar;

	ui_bhook_fn_t widthhandler = NULL;
	ui_bhook_fn_t heighthandler = NULL;

	switch(type) {
		case UI_HORIZONTAL_MENU:
			area = ui_area(0, 0, win->area.x, BOX_MENU_H + MENU_GAP_Y * 2);
			widthhandler = ui_bhook_fullwidth;
			break;
		case UI_VERTICAL_MENU:
			area = ui_area( 0, 0, BOX_MENU_W + MENU_GAP_X * 2, win->area.h);
			heighthandler = ui_bhook_fullheight;
			break;
		case UI_FULLWINDOW_MENU: case UI_NONE:
			area = ui_area(0, 0, win->area.w, win->area.h);
			heighthandler = ui_bhook_fullheight;
			widthhandler = ui_bhook_fullwidth;
	}
	navbar = ui_box_create(win, area, win->colors[1]);
	navbar->type = type;
	navbar->flags |= BOX_DISABLE;
	ui_bhook_add(&navbar->update, ui_bhook_nohovered);
	ui_bhook_add(&navbar->update, ui_bhook_nopressed);
	ui_bhook_add(&navbar->on_window_event, widthhandler);
	ui_bhook_add(&navbar->on_window_event, heighthandler);
	ui_box_add_root(&win->boxes, navbar);
    return navbar;
}

ui_box_t *ui_belem_canvas(ui_win_t* win) 
{
	win->canvas = ui_box_create(win, ui_area(0,0,0,0), win->colors[3]);
	ui_bhook_add(&win->canvas->update, ui_bhook_canvassize);
	return win->canvas;
}

ui_box_t *ui_belem_input(ui_win_t *win, int max_len)
{
	(void)max_len;
	ui_box_t		*input = NULL;

	SDL_Rect area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
	input = ui_box_create(win, area, COLOR_WHITE);
    input->data = ft_calloc(INPUT_SIZE_MAX + 1, sizeof(char));
    ui_bhook_add(&input->update, ui_bhook_inputfocus);
    ui_bhook_add(&input->update, ui_bhook_inputcatch);
	ui_boxhandler_t * curr = input->render;
	while(curr) {
		ui_boxhandler_t *next = curr->next;
		if (curr->fn == ui_bhook_drawlayers)
			curr->fn = ui_bhook_drawcliplayers;
		if (curr->fn == ui_bhook_drawfocused)
			curr->fn = ui_bhook_drawtextfocused;
		curr = next;
	}
    return input;
}



ui_box_t*	ui_belem_button(ui_win_t* win, const char* label)
{
		ui_box_t		*btn;
		SDL_Texture* texture; 

		SDL_Rect area;

		area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
		btn = ui_box_create(win, area, win->colors[2]);
		texture = ui_tool_text2texture(btn->parent_window, label, COLOR_WHITE);
		ui_layer_make(btn, texture);
		return btn;
}

ui_box_t* ui_belem_message(ui_win_t* win, const char* msg) 
{
		ui_box_t		*message;
		SDL_Texture* texture; 

		SDL_Rect area;

		area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
		message = ui_box_create(win, area, win->colors[2]);
		message->flags |= BOX_DISABLE;
		message->color = (SDL_Color) {255,255,255,0};
		texture = ui_tool_text2texture(message->parent_window, msg, COLOR_WHITE);
		ui_layer_make(message, texture);
    	ui_bhook_wincenter(message, NULL, NULL);
		ui_bhook_add(&message->update, ui_bhook_nopressed);
		ui_bhook_add(&message->update, ui_bhook_nohovered);
		return message;
}

ui_win_t *ui_welem_message(ui_globalApp_t *ref, const char *message)
{
	SDL_Rect area = {-1, -1, 200, 100};
    ui_win_t *popup = ui_win_create(ref, area, "pop up", 0);
	ui_whook_add(&popup->on_key_down, ui_whook_quitkey);
	popup->boxes = ui_belem_menu_make(popup, UI_FULLWINDOW_MENU);
    ui_bhook_add(&popup->boxes->update, ui_bhook_maxsize);
    ui_bhook_add(&popup->boxes->update, ui_bhook_nopressed);
    ui_bhook_add(&popup->boxes->update, ui_bhook_nohovered);
	SDL_Rect r = ui_area(0, -40, 0, 0);
	ui_box_t* msg = ui_belem_message(popup, message);
	r.y = 40;
    ui_box_add_child(popup->boxes, msg);

    ui_box_t *btn = ui_belem_button(popup,  "ok");
	btn->area.y = 40;
	ui_bhook_add(&btn->update, ui_bhook_winclose);
    ui_bhook_wincenter(btn, NULL, (void*)&r);
    ui_box_add_child(popup->boxes, btn);
    return popup;
}

ui_win_t *ui_welem_input(ui_globalApp_t *app)//, char *message)
{
	printf("welem input 1\n");
	SDL_Rect area = {-1, -1, 230, 150};
    ui_win_t *popup;

	popup = ui_win_create(app, area, "pop up", 0);
	ui_belem_menu_make(popup, UI_FULLWINDOW_MENU);
	ui_box_t* msg = ui_belem_message(popup, "Select the image path");
    ui_box_add_child(popup->boxes, msg);
    ui_box_t *input = ui_belem_input(popup, 64);
	ui_box_t* valid = ui_belem_button(popup, "load");
	ui_bhook_add(&valid->update,ui_bhook_valid_input);
    ui_bhook_wincenter(input, NULL, &(SDL_Rect){0, 40, 0, 0});
    ui_bhook_wincenter(valid, NULL, &(SDL_Rect){0, 100, 0, 0});
    ui_box_add_child(popup->boxes, input);
    ui_box_add_child(popup->boxes, valid);
	// ui_whook_add(&popup->on_window_event, ui_whook_reset_global_state);
	ui_whook_add(&popup->on_key_down, ui_whook_keydown_default);
    return popup;
}


