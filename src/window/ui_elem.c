#include "ui_win.h"
#include "ui_box.h"

ui_box_t *ui_belem_menu_list(ui_box_t *menu, char* label) 
{
	ui_box_t *new_option_list = NULL;
	SDL_Rect position = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
    int list_nb = ui_box_count_next(menu->list);

	if (menu->type == UI_HORIZONTAL_MENU) {
    	position.x = MENU_OFFSET_X + (list_nb * (BOX_MENU_W + MENU_GAP_X));
    	position.y = MENU_GAP_Y;
	} else if (menu->type == UI_VERTICAL_MENU) {
		position.x = MENU_GAP_X;
	   	position.y = MENU_GAP_Y + (list_nb * (BOX_MENU_H + MENU_GAP_Y));
	} else {
		return menu;
	}
    new_option_list = ui_belem_button(menu->parent_window, position, label);
	new_option_list->type = menu->type;
    new_option_list->border = 2;
	new_option_list->label = label;
    ui_bhook_add(&new_option_list->update, ui_bhook_revealchild);
    ui_box_add_child(menu, new_option_list);
    return new_option_list;
}

static void ui_bhook_fullwindow_button(ui_box_t* btn, SDL_Event*e, void* data) {
	ui_win_t* win = btn->parent_window;

	(void)e;
	(void)data;

	ui_layer_t* layer = btn->layers;
	int count = ui_box_count_prev(btn);
	btn->area.w = (win->area.w - 3 * MENU_GAP_X) / 2;
	if (count % 2 == 1 || count % 2 == 2) {
		btn->area.x = MENU_GAP_X;
	} else {
		btn->area.x = MENU_GAP_X * 2 + btn->area.w;
	}
	layer->dimension.x = btn->area.x + (BOX_MENU_W - layer->dimension.w) / 2;
}

// void ui_layer_label_new(ui_box_t *button, char* label)
// {
// 	SDL_Texture* t = ui_tool_text2texture(button->parent_window, label, COLOR_WHITE);
// 	ui_layer_t* l = ui_layer_create(button, t);
// 	l->dimension.x = button->area.x + (BOX_MENU_W - l->dimension.w) / 2;
// 	l->dimension.y = button->area.y + (BOX_MENU_H - l->dimension.h) / 2;
// 	ui_layer_add(&button->layers, l);
// }

ui_box_t* ui_belem_menu_item(ui_box_t *list, char* label, ui_bhook_fn_t fn)
{
	int btn_nb = ui_box_count_next(list->list);
	SDL_Rect pos = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
	ui_bhook_fn_t hook;

	hook = NULL;
	switch(list->type) {
		case UI_HORIZONTAL_MENU:
    		pos.x = list->area.x;
			pos.y = MENU_GAP_Y + (1 + btn_nb) * BOX_MENU_H;
			break;
		case UI_VERTICAL_MENU:
			pos.x = BOX_MENU_W + MENU_GAP_X;
			pos.y = list->area.y + btn_nb * BOX_MENU_H;
			break;
		case UI_FULLWINDOW_MENU: case UI_NONE: 
			pos.w = (list->area.w - 3 * MENU_GAP_X) / 2;
			pos.x = MENU_GAP_X + (btn_nb % 2) * (pos.w + MENU_GAP_X);
			pos.y = MENU_GAP_Y + (btn_nb / 2) * (BOX_MENU_H + MENU_GAP_Y);
			hook = ui_bhook_fullwindow_button;
			break;
	}
	ui_box_t* new_btn = ui_belem_button(list->parent_window, pos, label);
	new_btn->flags |= BOX_HIDDEN;
	ui_bhook_add(&new_btn->on_window_event, hook);
	ui_bhook_add(&new_btn->update, fn);
    ui_box_add_child(list, new_btn);
	return new_btn;
}

SDL_Rect ui_area(int x, int y, int w, int h)
{
	return (SDL_Rect) {x, y, w, h};
}

// create a menu list horizontal style
ui_box_t *ui_belem_menu_navbar(ui_win_t *win, menutype_e type)
{
	SDL_Rect area;
	ui_box_t *navbar;
	void (*widthhandler)(ui_box_t*, SDL_Event*, void*) = NULL;;
	void (*heighthandler)(ui_box_t*, SDL_Event*, void*) = NULL;

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
	navbar = ui_box_create(area, win->colors[1], win);
	navbar->type = type;
	ui_bhook_add(&navbar->update, ui_bhook_nohovered);
	ui_bhook_add(&navbar->update, ui_bhook_nopressed);
	ui_bhook_add(&navbar->on_window_event, widthhandler);
	ui_bhook_add(&navbar->on_window_event, heighthandler);
	ui_box_add_root(&win->menu, navbar);
    return navbar;
}

ui_box_t *ui_belem_input(ui_win_t *win, int max_len)
{
	ui_box_t		*input = NULL;

	SDL_Rect area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
	printf("create input box\n");
	input = ui_box_create(area, COLOR_WHITE, win);
    input->input = calloc(max_len + 1, sizeof(char));
    input->input_sizemax = max_len;
    input->input_size = 0;
    ui_bhook_add(&input->update, ui_bhook_inputfocus);
    ui_bhook_add(&input->update, ui_bhook_inputcatch);
    return input;
}

ui_box_t*	ui_belem_button(ui_win_t* win, SDL_Rect area, char* label)
{
		ui_box_t		*btn;
		SDL_Texture* texture; 
		ui_layer_t* layer;

		btn = ui_box_create(area, win->colors[2], win);
		texture = ui_tool_text2texture(btn->parent_window, label, COLOR_WHITE);
		layer = ui_layer_create(btn, texture);
		layer->dimension.x = btn->area.x + (btn->area.w - layer->dimension.w) / 2;
		layer->dimension.y = btn->area.y + (btn->area.h - layer->dimension.h) / 2;
		ui_layer_add(&btn->layers, layer);
		return btn;
}

ui_win_t *ui_welem_message(ui_win_t *win, char *message)
{
    ui_globalApp_t *ref = win->global;
	SDL_Rect area = {-1, -1, 200, 100};
    ui_win_t *popup = ui_win_create(ref, area, "pop up");

	popup->menu = ui_belem_menu_navbar(popup, UI_FULLWINDOW_MENU);
			//  popup->menu = ui_box_create(
			// (SDL_Rect){0, 0, popup->area.w, popup->area.h},
			// popup->colors[1], popup);
    ui_bhook_add(&popup->menu->update, ui_bhook_maxsize);
    ui_bhook_add(&popup->menu->update, ui_bhook_nopressed);
    ui_bhook_add(&popup->menu->update, ui_bhook_nohovered);
	SDL_Rect r = ui_area(0, -40, 0, 0);
	ui_box_t* msg = ui_belem_button(popup, r, message);
	msg->color = (SDL_Color) {255,255,255,0};
    ui_bhook_wincenter(msg, NULL, (void*)&r);
	r.y = 40;
    ui_box_add_child(popup->menu, msg);
	ui_bhook_add(&msg->update, ui_bhook_nopressed);
	ui_bhook_add(&msg->update, ui_bhook_nohovered);
    ui_box_t *btn = ui_belem_button(popup, ui_area(0, 40, BOX_MENU_W, BOX_MENU_H), "ok");
	ui_bhook_add(&btn->update, ui_bhook_winclose);
    ui_bhook_wincenter(btn, NULL, (void*)&r);
    ui_box_add_child(popup->menu, btn);
    ui_win_add(&ref->windows, popup);
    return popup;
}

static int	ui_whook_reset_loading(ui_win_t* win, SDL_Event *e, void* data)
{
	(void)data;
	ui_globalApp_t *app = win->global;

	if (!e)
		return 1;
    bool closing = (win->flags & WIN_QUIT) ||
                   (e->type == SDL_WINDOWEVENT && 
                    e->window.event == SDL_WINDOWEVENT_CLOSE);
	if(closing) {
		printf("closing input window\n");
		app->loading = false;
		if (app->input) {
			free(app->input);
			app->input = NULL;
		}
		return 1;
	}
	return 1;
}

ui_win_t *ui_welem_input(ui_win_t *win)//, char *message)
{
    ui_globalApp_t *ref = win->global;
	SDL_Rect area = {-1, -1, 200, 100};
    ui_win_t *popup;

	popup = ui_win_create(ref, area, "pop up");
	ui_whook_add(&popup->on_window_event, ui_whook_reset_loading);
	popup->menu = ui_belem_menu_navbar(popup, UI_FULLWINDOW_MENU);
    ui_bhook_add(&popup->menu->update, ui_bhook_maxsize);
    ui_bhook_add(&popup->menu->update, ui_bhook_nopressed);
    ui_bhook_add(&popup->menu->update, ui_bhook_nohovered);

	ui_box_t* msg = ui_belem_button(
			popup, 
			ui_area(0, 0, BOX_MENU_W, BOX_MENU_H),
			"choose image path");

	msg->color = (SDL_Color) {255,255,255,0};
    ui_bhook_wincenter(msg, NULL, NULL);
	msg->layers->dimension.y -= 40;
    ui_box_add_child(popup->menu, msg);
	ui_bhook_add(&msg->update, ui_bhook_nopressed);
	ui_bhook_add(&msg->update, ui_bhook_nohovered);

    ui_box_t *input = ui_belem_input(popup, 64);
    ui_bhook_wincenter(input, NULL, NULL);
	input->area.y += 40;
    ui_box_add_child(popup->menu, input);
    ui_win_add(&ref->windows, popup);
    return popup;
}
