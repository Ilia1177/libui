#include "ui_win.h"
#include "ui_box.h"
// #include "libft.h"

SDL_Rect ui_area(int x, int y, int w, int h)
{
	return (SDL_Rect) {x, y, w, h};
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


void ui_bhook_revealchild(ui_box_t *box, SDL_Event* e, void* data) {

	(void)data;
	(void)e;

	uint32_t state = box->state;
	SDL_Point p = ui_win_mousepos(box->parent_window);
	if (state & BOX_HOVERED || state & BOX_CLICKED || ui_box_hovered(box->childs, &p)) {
		ui_box_flags(box->childs, BOX_HIDDEN, false, false);
		ui_box_layout(box->childs, UI_LAYOUT_DIRTY, true, true);
	} else {
		ui_box_t *curr = box->childs;
		while (curr) {
			if (curr->state & BOX_HOVERED || curr->state & BOX_FOCUSED) 
				return;
			curr = curr->next;
		}
		ui_box_flags(box->childs, BOX_HIDDEN, true, true);
	}
}

// void ui_bhook_fullwindow_button(ui_box_t* btn, SDL_Event*e, void* data) 
// {
// 	ui_win_t* win = btn->parent_window;
//
// 	(void)e;
// 	(void)data;
//
// 	ui_layer_t* layer = btn->layers;
// 	int count = ui_box_count_prev(btn);
// 	btn->area.w = (win->area.w - 3 * MENU_GAP_X) / 2;
// 	if (count % 2 == 1 || count % 2 == 2) {
// 		btn->area.x = MENU_GAP_X;
// 	} else {
// 		btn->area.x = MENU_GAP_X * 2 + btn->area.w;
// 	}
// 	if(layer)
// 		layer->area.x = btn->area.x + (BOX_MENU_W - layer->area.w) / 2;
// 	btn->parent_window->state |= WIN_DIRTY;
// }
//
void	ui_bhook_fullwidth(ui_box_t* box, SDL_Event* e, void* data) {
	(void)e;
	(void)data;
	int gap;

	gap = box->area.x;
	box->area.w = box->parent_window->area.w - gap;
}

void	ui_bhook_fullheight(ui_box_t* box, SDL_Event* e, void* data)
{
	(void)e;
	(void)data;
	int gap;

	gap = box->area.y;
	box->area.h = box->parent_window->area.h - gap;
}

 void ui_bhook_vertical_posx(ui_box_t* b, SDL_Event *e, void *d)
{
	(void)d;
	(void)e;

	const ui_win_t* win = b->parent_window;
	if(b->area.x > 0)
		b->area.x = win->area.w - b->area.w;
}

static void ui_navbar_build(ui_box_t* menu, boxtype_e type)
{
	// const ui_box_t* prev = menu->parent;
	if (!menu)
		return;
	switch(type) {
		case UI_HORIZONTAL_TOP:
			ui_bhook_append(&menu->on_window_event, ui_bhook_fullwidth); // change from prepend
			break;
		case UI_VERTICAL_LEFT:
			// if (prev) {
			// 	 menu->area.w = prev->area.w;
			// 	 menu->area.y = prev->area.y;
			// }
			// ui_bhook_append(&menu->on_window_event, ui_bhook_vertical_posx); // change from prepend
			break;
		case UI_VERTICAL_RIGHT:
			ui_bhook_append(&menu->on_window_event, ui_bhook_vertical_posx); // change from prepend
			ui_bhook_append(&menu->on_window_event, ui_bhook_fullheight); // change from prepend
			break;
		default:
			break;
	}
	menu->state |= BOX_DISABLE;
}

ui_box_t* ui_menu_init(ui_win_t* win) {
	win->boxes = ui_belem_button(win, NULL);
	// win->boxes = ui_box_create(win, ui_area(0,0,0,0), win->colors[1]);
	// win->canvas = ui_belem_canvas(win);
	return win->boxes;
}

void gp_bhook_keep_pos(ui_box_t* b, SDL_Event *e, void* d)
{
	(void)d;
	(void)e;

	if(!b->parent)
		return;
	b->area.x = b->parent->area.x;
	ui_box_center_layers(b, NULL);
}

static ui_box_t *ui_option_build(ui_box_t *list, boxtype_e type) 
{
	if (!list)
		return NULL;
	// ui_box_t* pbox = list->parent;
	ui_win_t *win = list->parent_window;
	int width;
	ui_box_t* menu = ui_box_last(list->parent);

	int offsetx = 0;
	int offsety = 0;
	ui_box_t* curr = list;
	while(curr) {
		TTF_SizeText(win->font, curr->label, &width, NULL);
		switch(type) {
			case UI_HORIZONTAL_TOP:
				if (offsetx < MENU_OFFSET_X)
					offsetx = MENU_OFFSET_X;
				curr->area.w = curr->layers->area.w + 2 * MENU_GAP_X;
				curr->area.x = offsetx;
				curr->area.y = MENU_GAP_Y;
				offsetx += curr->area.w + MENU_GAP_X;
				break;
			case UI_VERTICAL_LEFT:
				if (offsety < MENU_OFFSET_Y)
					offsety = MENU_OFFSET_Y;
				curr->area.x = menu->area.x;
				curr->area.y = menu->area.y + offsety;// (i * offsety);
				curr->area.w = menu->area.w;
				offsety += curr->area.h;
				ui_bhook_append(&curr->on_window_event, gp_bhook_keep_pos);
				break;
			case UI_VERTICAL_RIGHT:
				curr->area.y = menu->area.y + offsety;
				curr->area.x = menu->area.x;
				offsety += curr->area.h;
				curr->area.w = menu->area.w;
				ui_bhook_append(&curr->on_window_event, gp_bhook_keep_pos);
			default:
			break;
		}
		ui_box_center_layers(curr, NULL);
		if(curr->childs && type != UI_VERTICAL_RIGHT) {
			ui_bhook_append(&curr->on_mouse_motion, ui_bhook_revealchild);
			ui_bhook_append(&curr->on_click_down, ui_bhook_revealchild);
			ui_bhook_append(&curr->on_key_down, ui_bhook_revealchild);
		}
		curr = curr->next;
	}
    return list;
}

static void ui_itemlist_build(ui_box_t* list, boxtype_e type, int rec)
{
	if(!list)
		return;
	int offset_y;
	if (rec == 0) offset_y = 1;
	else offset_y = 0;
	int i = 0;
	int width = ui_find_max_text_width(list) + 10 * MENU_GAP_X;
	ui_box_t* parent = list->parent;
	ui_box_t* curr = list;
	while(curr)
	{
		curr->state |= BOX_HIDDEN;
		switch(type) {
			case UI_HORIZONTAL_TOP:
				curr->area.w = width;
				curr->area.x = curr->parent->area.x + rec * curr->parent->area.w;
				curr->area.y = curr->parent->area.y + (offset_y + i) * BOX_MENU_H;
				break;
			case UI_VERTICAL_LEFT:
				curr->area.x = parent->area.x + parent->area.w + rec * BOX_MENU_W;
				curr->area.y = parent->area.y + i * BOX_MENU_H;
				break;
			case UI_VERTICAL_RIGHT:
				curr->state &= ~BOX_HIDDEN;
				break;
			default:
				break;
		}
		ui_box_center_layers(curr, NULL);
		ui_bhook_append(&curr->on_mouse_motion, ui_bhook_revealchild);
		ui_bhook_append(&curr->on_click_down, ui_bhook_revealchild);
		ui_bhook_append(&curr->on_key_down, ui_bhook_revealchild);
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

void	ui_layout_option(ui_box_t* opt) {
	
	opt->layout |= UI_LAYOUT_DIR_COL | UI_LAYOUT_BELOW
		| UI_LAYOUT_ALIGN_CENTER_Y | UI_LAYOUT_OVERLAY;

	ui_bhook_append(&opt->on_mouse_motion, ui_bhook_revealchild);
	ui_bhook_append(&opt->on_click_down, ui_bhook_revealchild);

	ui_box_t *curr = opt->childs;
	while(curr)
	{
		curr->layout |= UI_LAYOUT_ALIGN_LEFT | UI_LAYOUT_OVERLAY;
		curr->state |= BOX_HIDDEN;
		curr = curr->next;
	}
}

// ui_box_t*	ui_layout_dropdown(ui_box_t* opt, ui_box_t* itemlist)
// {
// 	ui_box_t* dropdown = ui_belem_button(itemlist->parent_window, NULL);
// 	ui_box_t *item = itemlist;
// 	while(item)
// 	{
// 		item->layout &= ~UI_LAYOUT_ABSOLUTE;
// 		item->layout |= UI_LAYOUT_ALIGN_LEFT;
// 		item->state |= BOX_HIDDEN;
// 		// item->area.y = offsety;
// 		// offsety += item->area.h;
// 		item = item->next;
// 	}
// 	ui_box_add_child(dropdown, item);
// 	ui_box_add_child(opt, dropdown);
// 	return dropdown;
// }

ui_box_t* ui_layout_menu(ui_box_t* menu)
{
	menu->layout = UI_LAYOUT_FILL_X | UI_LAYOUT_DIR_ROW;
	ui_box_t* curr = menu->childs;
	while(curr) {
		ui_layout_option(curr);
		curr = curr->next;
	}
	return menu;
}
