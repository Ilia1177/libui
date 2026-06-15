#include "ui_win.h"
#include "ui_box.h"
// #include "libft.h"

SDL_Rect ui_area(int x, int y, int w, int h)
{
	return (SDL_Rect) {x, y, w, h};
}

static void ui_bhook_revealchild(ui_box_t *box, SDL_Event* e, void* data) {

	(void)data;
	(void)e;

	uint32_t state = box->flags;
	SDL_Point p = ui_win_mousepos(box->parent_window);
	if (state & BOX_HOVERED || state & BOX_CLICKED || ui_box_hovered(box->childs, &p)) {
	// if (box->flags & BOX_HOVERED || ui_box_hovered(box->childs, &p)) {
		ui_box_flags(box->childs, BOX_HIDDEN, false, false);
	} else {
		ui_box_t *curr = box->childs;
		while (curr) {
			if (curr->flags & BOX_HOVERED || curr->flags & BOX_FOCUSED) 
				return;
			curr = curr->next;
		}
		ui_box_flags(box->childs, BOX_HIDDEN, true, true);
	}
}

static void ui_bhook_fullwindow_button(ui_box_t* btn, SDL_Event*e, void* data) 
{
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
	if(layer)
		layer->area.x = btn->area.x + (BOX_MENU_W - layer->area.w) / 2;
	btn->parent_window->state |= WIN_DIRTY;
}

static void	ui_bhook_fullwidth(ui_box_t* box, SDL_Event* e, void* data) {
	(void)e;
	(void)data;
	box->area.w = box->parent_window->area.w;
}

static void	ui_bhook_fullheight(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	box->area.h = box->parent_window->area.h;
}

static void ui_navbar_build(ui_box_t* menu, boxtype_e type)
{
	// ui_win_t* win = menu->parent_window;
	ui_bhook_fn_t widthhandler = NULL;
	ui_bhook_fn_t heighthandler = NULL;
	if (!menu)
		return;
	switch(type) {
		case UI_HORIZONTAL_MENU:
			// menu->area = ui_area(menu->area.x, 0, win->area.x, BOX_MENU_H + MENU_GAP_Y * 2);
			widthhandler = ui_bhook_fullwidth;
			break;
		case UI_VERTICAL_MENU:
			// menu->area = ui_area( 0, 0, BOX_MENU_W + MENU_GAP_X * 2, win->area.h);
			heighthandler = ui_bhook_fullheight;
			break;
		case UI_VERTICAL_LIST:
			heighthandler = ui_bhook_fullheight;
			// menu->area = (SDL_Rect) {0, BOX_MENU_H, BOX_MENU_W, win->area.h - BOX_MENU_H};
			break;

		case UI_FULLWINDOW_MENU: case UI_NONE: 
			// menu->area = ui_area(0, 0, win->area.w, win->area.h);
			heighthandler = ui_bhook_fullheight;
			widthhandler = ui_bhook_fullwidth;
			break;
	}
	menu->flags |= BOX_DISABLE;
	ui_bhook_append(&menu->on_window_event, widthhandler); // change from prepend
	ui_bhook_append(&menu->on_window_event, heighthandler); // cahnge from prepend
}

ui_box_t* ui_menu_init(ui_win_t* win) {
	win->boxes = ui_box_create(win, ui_area(0,0,0,0), win->colors[1]);
	// win->canvas = ui_belem_canvas(win);
	return win->boxes;
}

static ui_box_t *ui_option_build(ui_box_t *list, boxtype_e type) 
{
	if (!list)
		return NULL;
	int i = 0;
	ui_box_t* curr = list;
	ui_box_t* pbox = list->parent;
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
			// curr->color = win->colors[2];
			curr->area.w = (curr->area.w - 3 * MENU_GAP_X) / 2;
			curr->area.x = MENU_GAP_X + (i % 2) * (curr->area.w + MENU_GAP_X);
			curr->area.y = MENU_GAP_Y + (i / 2) * (BOX_MENU_H + MENU_GAP_Y);
			break;
			// ui_bhook_prepend(&curr->on_window_event, ui_bhook_fullwindow_button);
			case UI_VERTICAL_LIST:
			curr->area = (SDL_Rect) {pbox->area.x, pbox->area.y + i * BOX_MENU_H, pbox->area.w, BOX_MENU_H};
			// curr->area = (SDL_Rect) {0, 0, win->area.w, BOX_MENU_H};
			// curr->color = win->colors[2];
			default:
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

static int ui_find_max_text_width(ui_box_t* list) 
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


static void ui_itemlist_build(ui_box_t* list, boxtype_e type, int rec)
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
			default:
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
		if(list->childs)
			ui_itemlist_build(list->childs, type, 0);
		list = list->next;
	}
}
