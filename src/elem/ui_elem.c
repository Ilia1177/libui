#include "ui_win.h"
#include "ui_box.h"
#include "libft.h"

SDL_Rect ui_area(int x, int y, int w, int h)
{
	return (SDL_Rect) {x, y, w, h};
}

ui_slider_data_t ui_slider_data(float max, float min, float def) {
	return (ui_slider_data_t) {
		max, min, def, def, false
	};
}

ui_box_t* ui_menu_init(ui_win_t* win) {
	win->boxes = ui_box_create(win, ui_area(0,0,0,0), win->colors[1]);
	win->canvas = ui_belem_canvas(win);
	return win->boxes;
}

ui_box_t *ui_belem_canvas(ui_win_t* win) 
{
	win->canvas = ui_box_create(win, ui_area(0,0,0,0), win->colors[3]);
	ui_bhook_prepend(&win->canvas->update, ui_bhook_canvassize);
	return win->canvas;
}

ui_box_t *ui_belem_input(ui_win_t *win, int max_len)
{
	(void)max_len;
	ui_box_t		*input = NULL;

	SDL_Rect area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
	input = ui_box_create(win, area, COLOR_WHITE);
    input->data = ft_calloc(INPUT_SIZE_MAX + 1, sizeof(char));
    ui_bhook_prepend(&input->update, ui_bhook_inputfocus);
    ui_bhook_prepend(&input->update, ui_bhook_catch_input);
	ui_bhook_replace(input->render, ui_bhook_drawlayers, ui_bhook_drawcliplayers);
	ui_bhook_replace(input->render, ui_bhook_drawfocused, ui_bhook_drawtextfocused);
    return input;
}



ui_box_t*	ui_belem_button(ui_win_t* win, SDL_Texture* texture)
{
		ui_box_t		*btn = NULL;
		// SDL_Texture* texture; 

		SDL_Rect area;

		area = ui_area(0, 0, BOX_MENU_W, BOX_MENU_H);
		btn = ui_box_create(win, area, win->colors[1]);
		ui_layer_make(btn, texture);
		// ui_box_center_layers(btn, NULL);
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
		texture = ui_texture_text(message->parent_window, msg, COLOR_WHITE);
		ui_layer_make(message, texture);
    	ui_bhook_wincenter(message, NULL, &(SDL_Rect){0, 10, 0, 0});
		// ui_bhook_prepend(&message->update, ui_bhook_nopressed);
		// ui_bhook_prepend(&message->update, ui_bhook_nohovered);
		ui_bhook_remove(&message->on_click_down, ui_bhook_clickdown_default);
		ui_bhook_remove(&message->on_mouse_motion, ui_bhook_mousemotion_default);
		return message;
}

ui_win_t *ui_welem_message(ui_globalApp_t *ref, const char *message)
{
	SDL_Rect area = {-1, -1, 0, 0};
	TTF_SizeText(ref->windows->font, message, &area.w, &area.h);
	// area.w += 50;
	area.h *= 4;
    ui_win_t *popup = ui_win_create(ref, area, "pop up", 0);
	ui_whook_add(&popup->on_key_down, ui_whook_quitkey);
	ui_box_t *menu = ui_menu_init(popup);
    ui_bhook_append(&menu->on_window_event, ui_bhook_maxsize);
	ui_whook_remove(&popup->on_click_down, ui_whook_clickdown_default);
	ui_whook_remove(&popup->on_mouse_motion, ui_whook_mousemotion_default);
    // ui_bhook_append(&popup->boxes->update, ui_bhook_nopressed);
    // ui_bhook_append(&popup->boxes->update, ui_bhook_nohovered);
	ui_box_t* msg = ui_belem_message(popup, message);
    ui_box_t *btn = ui_belem_button(popup,  ui_texture_text(popup, "ok", COLOR_WHITE));
	ui_bhook_append(&btn->on_key_down, ui_bhook_winclose);
	ui_bhook_append(&btn->on_click_down, ui_bhook_winclose);
    ui_bhook_wincenter(btn, NULL, &(SDL_Rect){0, 40, 0, 0});
    ui_bhook_wincenter(msg, NULL, &(SDL_Rect){0, -80, 0, 0});
    ui_box_add_child(menu, btn);
    ui_box_add_child(menu, msg);
	ui_menu_build(menu, UI_NONE);
    return popup;
}

ui_win_t *ui_welem_input(ui_globalApp_t *app)//, char *message)
{
	printf("welem input 1\n");
	SDL_Rect area = {-1, -1, 230, 150};
    ui_win_t *popup;

	popup = ui_win_create(app, area, "pop up", 0);
	ui_box_t* menu = ui_menu_init(popup);
	ui_box_t* msg = ui_belem_message(popup, "Select the image path");
    ui_box_t *input = ui_belem_input(popup, 64);
	ui_box_t* valid = ui_belem_button(popup, ui_texture_text(popup, "load", COLOR_WHITE));
	ui_bhook_prepend(&valid->update,ui_bhook_valid_input);
    ui_bhook_wincenter(input, NULL, &(SDL_Rect){0, 40, 0, 0});
    ui_bhook_wincenter(valid, NULL, &(SDL_Rect){0, 100, 0, 0});
    ui_box_add_child(menu, input);
    ui_box_add_child(menu, valid);
    ui_box_add_child(menu, msg);
	ui_whook_add(&popup->on_key_down, ui_whook_keydown_default);
	ui_menu_build(menu, UI_NONE);

	printf("message color: %d, %d, %d, %d\n", msg->color.r, msg->color.g, msg->color.b, msg->color.a);
    return popup;
}

static void ui_navbar_build(ui_box_t* menu, boxtype_e type)
{
	ui_win_t* win = menu->parent_window;
	ui_bhook_fn_t widthhandler = NULL;
	ui_bhook_fn_t heighthandler = NULL;
	switch(type) {
		case UI_HORIZONTAL_MENU:
			menu->area = ui_area(0, 0, win->area.x, BOX_MENU_H + MENU_GAP_Y * 2);
			widthhandler = ui_bhook_fullwidth;
			break;
		case UI_VERTICAL_MENU:
			menu->area = ui_area( 0, 0, BOX_MENU_W + MENU_GAP_X * 2, win->area.h);
			heighthandler = ui_bhook_fullheight;
			break;
		case UI_FULLWINDOW_MENU: case UI_NONE:
			menu->area = ui_area(0, 0, win->area.w, win->area.h);
			heighthandler = ui_bhook_fullheight;
			widthhandler = ui_bhook_fullwidth;
	}
	menu->flags |= BOX_DISABLE;
	ui_bhook_prepend(&menu->update, ui_bhook_nohovered);
	ui_bhook_prepend(&menu->update, ui_bhook_nopressed);
	ui_bhook_prepend(&menu->on_window_event, widthhandler);
	ui_bhook_prepend(&menu->on_window_event, heighthandler);
}

ui_box_t *ui_option_build(ui_box_t *list, boxtype_e type) 
{
	if (!list)
		return NULL;
	int i = 0;
	ui_box_t* curr = list;
	ui_win_t *win = list->parent_window;
	int width;
	int offsetx = MENU_OFFSET_X;
	while(curr) {
		TTF_SizeText(win->font, curr->label, &width, NULL);
		switch(type) {
			case UI_HORIZONTAL_MENU:
			curr->area.w = curr->layers->area.w + 2 * MENU_GAP_X;
			curr->area.x = offsetx;
			offsetx += curr->area.w;
			curr->area.y = MENU_GAP_Y;
			curr->color = COLOR_TRANSPARENT;
			break;
			case UI_VERTICAL_MENU:
			curr->area.x = MENU_GAP_X;
			curr->area.y = MENU_GAP_Y + (i * (BOX_MENU_H + MENU_GAP_Y));
			break;
			case UI_FULLWINDOW_MENU:
			curr->color = win->colors[2];
			curr->area.w = (curr->area.w - 3 * MENU_GAP_X) / 2;
			curr->area.x = MENU_GAP_X + (i % 2) * (curr->area.w + MENU_GAP_X);
			curr->area.y = MENU_GAP_Y + (i / 2) * (BOX_MENU_H + MENU_GAP_Y);
			ui_bhook_prepend(&curr->on_window_event, ui_bhook_fullwindow_button);
			case UI_NONE:
			break;
		}
		ui_box_center_layers(curr, NULL);
		if(curr->childs) {
			ui_bhook_append(&curr->on_mouse_motion, ui_bhook_revealchild);
			ui_bhook_append(&curr->on_click_down, ui_bhook_revealchild);
			ui_bhook_append(&curr->on_key_down, ui_bhook_revealchild);
		}
		curr = curr->next;
		i++;
	}
    return list;
}

int ui_find_max_text_width(ui_box_t* list) 
{
	ui_box_t* curr = list;

	if (!list)
		return 0;
	int max_width;
	max_width = 0;
	while(curr) 
	{
		if (curr->layers && curr->layers->area.w > max_width)
			max_width = curr->layers->area.w;
		curr = curr->next;
	}
	return max_width;
}

void ui_itemlist_build(ui_box_t* list, boxtype_e type, int rec)
{
	if(!list)
		return;
	ui_box_t* curr = list;
	int offset_y;
	if (rec == 0) offset_y = 1;
	else offset_y = 0;
	int i = 0;
	int width = ui_find_max_text_width(list) + 10 * MENU_GAP_X;
	while(curr)
	{
		switch(type) {
			case UI_HORIZONTAL_MENU:
				printf("build list item width: %d rec: %d\n", width, rec);
				curr->area.w = width;
				curr->area.x = curr->parent->area.x + rec * curr->parent->area.w;
				curr->area.y = curr->parent->area.y + (offset_y + i) * BOX_MENU_H;
				break;
			case UI_VERTICAL_MENU:
				curr->area.x = BOX_MENU_W + MENU_GAP_X + rec * BOX_MENU_W;
				curr->area.y = curr->parent->area.y + i * BOX_MENU_H;
				break;
			case UI_FULLWINDOW_MENU:
				curr->area.w = (curr->area.w - 3 * MENU_GAP_X) / 2;
				curr->area.x = MENU_GAP_X + (i % 2) * (curr->area.w + MENU_GAP_X);
				curr->area.y = MENU_GAP_Y + (i / 2) * (BOX_MENU_H + MENU_GAP_Y);
				ui_bhook_prepend(&curr->on_window_event, ui_bhook_fullwindow_button);
			case UI_NONE:
				break;
		}
		curr->flags |= BOX_HIDDEN;
		ui_bhook_append(&curr->on_mouse_motion, ui_bhook_revealchild);
		ui_bhook_append(&curr->on_click_down, ui_bhook_revealchild);
		ui_bhook_append(&curr->on_key_down, ui_bhook_revealchild);
		ui_box_center_layers(curr, &(SDL_Rect){10, 0,0,0});
		ui_itemlist_build(curr->childs, type, rec + 1);
		curr = curr->next;
		i++;
	}
}

void ui_menu_build(ui_box_t* menu, boxtype_e type)
{
	ui_navbar_build(menu, type);
	ui_option_build(menu->childs, type);
	ui_box_t *list = menu->childs;
	while(list) {
		ui_itemlist_build(list->childs, type, 0);
		list = list->next;
	}
}
