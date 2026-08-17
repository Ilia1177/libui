#include "ui_win.h"
#include "ui_box.h"
// #include "libft.h"

SDL_Rect ui_area(int x, int y, int w, int h)
{
	return (SDL_Rect) {x, y, w, h};
}

void ui_bhook_revealchild(ui_box_t *box, SDL_Event* e, void* data) 
{
	(void)data;
	(void)e;

	uint32_t state = box->state;
	SDL_Point p = ui_win_mousepos(box->parent_window);
	if (state & BOX_HOVERED || state & BOX_CLICKED || ui_box_hovered(box->childs, &p)) {
		ui_box_flags(box->childs, BOX_HIDDEN, false, false);
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

void	ui_layout_option(ui_box_t* opt) {
	
	ui_box_t *curr;

	opt->layout |= UI_LAYOUT_DIR_COL | UI_LAYOUT_BELOW
		| UI_LAYOUT_ALIGN_CENTER_Y | UI_LAYOUT_OVERLAY;

	ui_bhook_append(&opt->on_mouse_motion, ui_bhook_revealchild);
	ui_bhook_append(&opt->on_click_down, ui_bhook_revealchild);
	curr = opt->childs;
	while(curr)
	{
		curr->layout |= UI_LAYOUT_ALIGN_LEFT | UI_LAYOUT_OVERLAY;
		curr->state |= BOX_HIDDEN;
		curr = curr->next;
	}
}

ui_box_t* ui_layout_menu(ui_box_t* menu)
{
	menu->layout |= UI_LAYOUT_FILL_X | UI_LAYOUT_DIR_ROW;
	ui_box_t* curr = menu->childs;
	while(curr) {
		ui_layout_option(curr);
		curr = curr->next;
	}
	return menu;
}
