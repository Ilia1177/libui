#include "ui_win.h"
#include "ui_box.h"

void ui_bhook_mousemotion_default(ui_box_t *box, SDL_Event* e, void* data)
{
	(void)data;
	(void)e;
	ui_win_t* win;
	SDL_Point p;

	uint32_t previous = box->state;
	win = box->parent_window;
	p = ui_win_mousepos(win);

	ui_box_t *hovered_box = ui_box_hovered(win->boxes, &p);
	if(hovered_box == box && box->state & BOX_HOVERABLE) {
		box->state |= BOX_HOVERED;
		// box->layout |= UI_LAYOUT_DIRTY;
	} else {
		box->state &= ~BOX_HOVERED;
        box->state &= ~BOX_PRESSED;
	}
	if (previous != box->state) {
		// box->parent_window->state |= WIN_DIRTY;
		box->state |= BOX_DIRTY;
		// ui_box_flags(box->childs, BOX_DIRTY, true, true);
		// ui_box_flags(box->childs, UI_LAYOUT_DIRTY, true, true);
	}
}
